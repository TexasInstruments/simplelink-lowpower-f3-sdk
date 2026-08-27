/******************************************************************************

@file  ms_oad_profile.c

@brief This file contains Multistep-OAD application logic

Group: WCS, BTS
Target Device: cc23xx

******************************************************************************

 Copyright (c) 2022-2026, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************


*****************************************************************************/

#ifdef MS_OAD

//*****************************************************************************
//! Includes
//*****************************************************************************
#include "ti_ble_config.h"
#include "ti/ble/profiles/oad/ms_oad_profile.h"
#include "ti/ble/profiles/oad/oad_profile.h"
#include "ti/ble/app_util/common/util.h"
#include "ti/ble/app_util/sw_update/sw_update.h"
#include <app_main.h>
#include <ti_drivers_config.h>
#include <ti/drivers/NVS.h>
#include <stddef.h>

#include <ti/log/Log.h>

//*****************************************************************************
//! Defines
//*****************************************************************************

#define MS_OAD_HEADER_MAGIC_REVOKED     ((uint32_t) 0xFFFFFFFF)
#define MS_OAD_IMG_OK                   ((uint8_t) 1)

// Journal constants
#define MS_OAD_JOURNAL_MAGIC            ((uint32_t) 0x96F3B83D)
#define MS_OAD_JOURNAL_PAGE_SIZE        ((uint32_t) 2048)
#define MS_OAD_VERSION_ENTRY_SIZE       ((uint32_t) 24)  // sizeof(msOadVersionEntry_t)
#define MS_OAD_STATE_ENTRY_SIZE         ((uint32_t) 12)  // sizeof(msOadStateEntry_t)
#define MS_OAD_MAX_STATE_ENTRIES        ((MS_OAD_JOURNAL_PAGE_SIZE - MS_OAD_VERSION_ENTRY_SIZE) / MS_OAD_STATE_ENTRY_SIZE)

// Bitmap flag bit positions
#define MS_OAD_FLAG_BIT_INIT_OAD        (0)  // bit 0: initMsOad
#define MS_OAD_FLAG_BIT_OAD_STARTED     (1)  // bit 1: oadStarted
#define MS_OAD_FLAG_BIT_REVERT          (2)  // bit 2: revert

// Bitmap operations (flash-friendly: 1=unset/erased, 0=set)
// All 1s (0xFFFFFFFF) = no flags set (erased flash)
// Clearing a bit to 0 = setting that flag (only requires 1->0 bit flip, no erase)
#define MS_OAD_BITMAP_ERASED                ((uint32_t) 0xFFFFFFFF)
#define MS_OAD_BITMAP_IS_SET(bitmap, bit)   (((bitmap) & (1u << (bit))) == 0)
#define MS_OAD_BITMAP_SET(bitmap, bit)      ((bitmap) & ~(1u << (bit)))

// Predefined state bitmaps
#define MS_OAD_BITMAP_INIT      ((uint32_t) MS_OAD_BITMAP_SET(MS_OAD_BITMAP_ERASED, MS_OAD_FLAG_BIT_INIT_OAD))
#define MS_OAD_BITMAP_REVERT    ((uint32_t) MS_OAD_BITMAP_SET(MS_OAD_BITMAP_SET(MS_OAD_BITMAP_ERASED, MS_OAD_FLAG_BIT_OAD_STARTED), MS_OAD_FLAG_BIT_REVERT))

// Entry validation — valid only if both start and end magic words match
#define MS_OAD_IS_ENTRY_VALID(entry)        ((entry).magic1 == MS_OAD_JOURNAL_MAGIC && (entry).magic2 == MS_OAD_JOURNAL_MAGIC)

// Entry erasure check — erased only if all fields are 0xFFFFFFFF (safe to write)
#define MS_OAD_IS_STATE_ENTRY_ERASED(entry) ((entry).magic1 == 0xFFFFFFFF && (entry).bitmap == 0xFFFFFFFF && (entry).magic2 == 0xFFFFFFFF)

//*****************************************************************************
//! Typedefs
//*****************************************************************************

typedef enum
{
    MS_OAD_EVT_INIT_AND_RESET       = 0x00,     // Initialize MS-OAD and reset device
    MS_OAD_EVT_IMG_VERIFIED         = 0x01,     // Application verified the full-image successfully
} msOadApiEvent_e;

typedef enum
{
    MS_OAD_BOOT_MODE_REGULAR        = 0x0,
    MS_OAD_BOOT_MODE_HALF           = 0x1,
    MS_OAD_BOOT_MODE_TEST_IMAGE     = 0x2,
    MS_OAD_BOOT_MODE_ERROR          = 0x3,
} msOadBootMode_e;

// Version entry — always at offset 0 of the journal.
// Written once when MS-OAD is initiated. magic1 is written first, magic2 last.
// Entry is valid only if both magic1 and magic2 match MS_OAD_JOURNAL_MAGIC.
#ifdef __IAR_SYSTEMS_ICC__
typedef struct __attribute__((__packed__))
#else
PACKED_TYPEDEF_STRUCT
#endif
{
    uint32_t             magic1;             // 4 bytes — start marker (written first)
    struct image_version committedVersion;   // 8 bytes — original committed version
    struct image_version pendingVersion;     // 8 bytes — new version being processed
    uint32_t             magic2;             // 4 bytes — end marker (written last)
} msOadVersionEntry_t;  // 24 bytes

// State entry — appended sequentially after the version entry in the journal.
// Each entry carries the cumulative bitmap of all flags set so far.
// magic1 is written first, magic2 last.
// Entry is valid only if both magic1 and magic2 match MS_OAD_JOURNAL_MAGIC.
#ifdef __IAR_SYSTEMS_ICC__
typedef struct __attribute__((__packed__))
#else
PACKED_TYPEDEF_STRUCT
#endif
{
    uint32_t magic1;  // Start marker (written first)
    uint32_t bitmap;  // Flag bitmap: bit=1 means unset (erased), bit=0 means set
    uint32_t magic2;  // End marker (written last)
} msOadStateEntry_t;  // 12 bytes

_Static_assert(sizeof(msOadVersionEntry_t) == MS_OAD_VERSION_ENTRY_SIZE,
               "MS_OAD_VERSION_ENTRY_SIZE mismatch with msOadVersionEntry_t");
_Static_assert(sizeof(msOadStateEntry_t) == MS_OAD_STATE_ENTRY_SIZE,
               "MS_OAD_STATE_ENTRY_SIZE mismatch with msOadStateEntry_t");

// Type of callback function to be called for internal events
typedef bStatus_t (*msOadInternalCallback)(void *pData);

// Type of callback function to be called for multistep oad events
typedef bStatus_t (*msOadEventCallback)(void *pData);

// Type of callback function to be called for api events
typedef bStatus_t (*msOadApiEventCallback)(void *pData);

// Structure holding all MS-OAD event callbacks. Set by the module on boot, depends on the boot mode
typedef struct
{
    msOadInternalCallback onEntry;              // Called when entering a state

    msOadEventCallback onNewImageIdentify;      // Handles @ref OAD_PROFILE_MSG_NEW_IMG_IDENTIFY event
    msOadEventCallback onDownloadStart;         // Handles @ref OAD_PROFILE_MSG_START_DOWNLOAD event
    msOadEventCallback onDownloadDone;          // Handles @ref OAD_PROFILE_MSG_FINISH_DOWNLOAD event
    msOadEventCallback onDownloadFailed;        // Handles @ref OAD_PROFILE_MSG_DOWNLOAD_FAILED event
    msOadEventCallback onDownloadCancelled;     // Handles @ref OAD_PROFILE_MSG_CANCEL_DOWNLOAD event
    msOadEventCallback onTimeout;               // Handles @ref OAD_PROFILE_MSG_TIMEOUT event
    msOadEventCallback onGetState;              // Handles @ref OAD_PROFILE_MSG_GET_MS_OAD_STATE event
    msOadEventCallback onResetReq;              // Handles @ref OAD_PROFILE_MSG_RESET_REQ event

    msOadApiEventCallback onInitAndReset;       // Handles @ref MS_OAD_EVT_INIT_AND_RESET event
    msOadApiEventCallback onImgVerified;        // Handles @ref MS_OAD_EVT_IMG_VERIFIED event
} msOadStateCallbacks_t;

typedef struct
{
    bool isImageIdentified;
} msOadProfileData_t;

// Structure holding all MS-OAD module data
typedef struct
{
    // Public state of the MS-OAD process, used for boot state resolution on reboot and for API calls
    msOadState_e currentState;

    // Previous state before entering DOWNLOADING state (used to determine action on download done/failed)
    msOadState_e stateBeforeDownloading;

    // Callbacks for the current state, set on boot by msOad_OnBootResolveState function
    msOadStateCallbacks_t stateCallbacks;

    // Journal state tracking — populated at boot by msOad_ScanJournal()
    uint32_t             journalNextOffset;   // Next free offset for appending state entries
    uint32_t             currentBitmap;       // Current cumulative bitmap (from last valid entry)
    msOadVersionEntry_t  versionEntry;        // Cached version entry (read at boot)

    // Handle for NVS operations on MS-OAD flash space
    NVS_Handle msOadNvsHandle;

    // Data related to OAD profile operations
    msOadProfileData_t oadProfileData;
} msOadData_t;

//*****************************************************************************
//! Prototypes
//*****************************************************************************

static OADProfile_AppCommand_e msOad_ProfileCallback(OADProfile_App_Msg_e msg, void *pData);
static void msOad_CriticalErrorHandler(int32 errorCode , void* pInfo);
static void msOad_StackInitDoneHandler(gapDeviceInitDoneEvent_t *deviceInitDoneData);

static void msOad_Init(void);
static void msOad_ValidateJournalAndImageOk(void);
static msOadBootMode_e msOad_OnBootResolveState(void);
static bStatus_t msOad_HandleOadProfileEvents(OADProfile_App_Msg_e event, void* pData);
static bStatus_t msOad_HandleApiEvents(msOadApiEvent_e event, void* pData);

// API event handlers
static bStatus_t msOad_HandleInitAndResetEvent(void* pData);
static bStatus_t msOad_HandleImageVerifiedEvent(void* pData);

// OAD profile event handlers
static bStatus_t msOad_HandleNewImageIdentifyEvent(struct image_header* pImageHeader, bool expectBootable);
static bStatus_t msOad_HandleNewImageIdentifyEventWaitFirst(void* pData);
static bStatus_t msOad_HandleNewImageIdentifyEventWaitSecond(void* pData);
static bStatus_t msOad_HandleDownloadStartEventWaitFirstHalf(void* pData);
static bStatus_t msOad_HandleDownloadStartEventGeneric(void* pData);
static bStatus_t msOad_HandleDownloadDoneDownloading(void* pData);
static bStatus_t msOad_HandleDownloadFailedDownloading(void* pData);
static bStatus_t msOad_HandleDownloadCancelledWaitFirstHalf(void* pData);
static bStatus_t msOad_HandleDownloadCancelledOther(void* pData);
static bStatus_t msOad_HandleDownloadCancelledWaitImageEnable(void* pData);
static bStatus_t msOad_HandleTimeoutWaitImageEnable(void* pData);
static bStatus_t msOad_HandleGetStateEvent(void* pData);
static bStatus_t msOad_HandleGetStateEventDownloading(void* pData);
static bStatus_t msOad_HandleResetReqEvent(void* pData);

// State entry functions
static bStatus_t msOad_EnterIdle(void* pData);
static bStatus_t msOad_EnterWaitFirstHalf(void* pData);
static bStatus_t msOad_EnterWaitSecondHalf(void* pData);
static bStatus_t msOad_EnterWaitFirstHalfRevert(void* pData);
static bStatus_t msOad_EnterWaitSecondHalfRevert(void* pData);
static bStatus_t msOad_EnterDownloading(void* pData);

// Journal helper functions
static bStatus_t msOad_ScanJournal(void);
static void msOad_DiagnoseJournal(void);
static bStatus_t msOad_WriteVersionEntry(const struct image_version *pCommitted, const struct image_version *pPending);
static bStatus_t msOad_AppendStateEntry(uint32_t bitmap);
static bStatus_t msOad_NvsWriteWord(uint32_t offset, uint32_t value);
static bool msOad_IsJournalErased(void);
static bStatus_t msOad_EraseJournal(void);

// Helper functions for flash operations and version comparison
static bStatus_t msOad_GetTrailer(struct image_trailer *pTrailer);
static void msOad_ReadImageHeader(uint32_t hdrAddr, struct image_header *pHeader);
static void msOad_GetSlotVersion(uint32_t hdrAddr, struct image_version *pVersion);

// Helper functions for version classification (used in boot state resolution)
static bool msOad_CompareVersions(const struct image_version *v1, const struct image_version *v2);
static bool msOad_IsImageRevoked(const struct image_header *pHeader);
static bool msOad_IsSecondaryImageRevoked();
static bool msOad_IsSlotBootable(uint32_t slotHeaderFlags);
static bool msOad_IsVersionValidAndBootable(const struct image_header *pHeader,
                                            const struct image_version *pVersionReference);
static bool msOad_IsVersionValidAndNonBootable(const struct image_header *pHeader,
                                               const struct image_version *pVersionReference);

// Helper functions for state transitions
static void msOad_SetState(msOadState_e newState);
static void msOad_RebootDevice(void);
static bStatus_t msOad_SetImageOk(void);
static bool msOad_IsImageOkSet(void);
static bStatus_t msOad_RevokeSecondaryImageIfRequired(void);
static bStatus_t msOad_EnterRevertMode();
static void msOad_SetImageIdentified(bool identified);
static void msOad_ClearProfileData(void);

//*****************************************************************************
//! Globals
//*****************************************************************************

// Parameters that should be given as input to the BLEAppUtil_init function
BLEAppUtil_GeneralParams_t appMsOadMainParams =
{
    .taskPriority = 1,
    .taskStackSize = 1024,
    .profileRole = (BLEAppUtil_Profile_Roles_e)(HOST_CONFIG),
    .addressMode = DEFAULT_ADDRESS_MODE,
    .deviceNameAtt = attDeviceName,
    .pDeviceRandomAddress = pRandomAddress,
};

#if defined( HOST_CONFIG ) && ( HOST_CONFIG & ( PERIPHERAL_CFG | CENTRAL_CFG ) )
BLEAppUtil_PeriCentParams_t appMsOadMainPeriCentParams =
{
#if defined( HOST_CONFIG ) && ( HOST_CONFIG & ( PERIPHERAL_CFG ) )
 .connParamUpdateDecision = DEFAULT_PARAM_UPDATE_REQ_DECISION,
#endif //#if defined( HOST_CONFIG ) && ( HOST_CONFIG & ( PERIPHERAL_CFG ) )

#ifdef GAP_BOND_MGR
 .gapBondParams = &gapBondParams
#endif //GAP_BOND_MGR
};
#else //observer || broadcaster
BLEAppUtil_PeriCentParams_t appMsOadMainPeriCentParams;
#endif //#if defined( HOST_CONFIG ) && ( HOST_CONFIG & ( PERIPHERAL_CFG | CENTRAL_CFG ) )

static const msOadStateCallbacks_t statesCallbacks[MS_OAD_NUM_STATES] =
    {
        [MS_OAD_UNINITIALIZED] = {
            .onGetState           = msOad_HandleGetStateEvent,
        },
        [MS_OAD_IDLE] = {
            .onEntry              = msOad_EnterIdle,
            .onGetState           = msOad_HandleGetStateEvent,
            .onInitAndReset       = msOad_HandleInitAndResetEvent,
        },
        [MS_OAD_WAIT_FIRST_HALF] = {
            .onEntry              = msOad_EnterWaitFirstHalf,
            .onNewImageIdentify   = msOad_HandleNewImageIdentifyEventWaitFirst,
            .onDownloadStart      = msOad_HandleDownloadStartEventWaitFirstHalf,
            .onDownloadCancelled  = msOad_HandleDownloadCancelledWaitFirstHalf,
            .onTimeout            = msOad_HandleDownloadCancelledWaitFirstHalf,
            .onGetState           = msOad_HandleGetStateEvent,
        },
        [MS_OAD_WAIT_SECOND_HALF] = {
            .onEntry              = msOad_EnterWaitSecondHalf,
            .onNewImageIdentify   = msOad_HandleNewImageIdentifyEventWaitSecond,
            .onDownloadStart      = msOad_HandleDownloadStartEventGeneric,
            .onDownloadCancelled  = msOad_HandleDownloadCancelledOther,
            .onTimeout            = msOad_HandleDownloadCancelledOther,
            .onGetState           = msOad_HandleGetStateEvent,
        },
        [MS_OAD_WAIT_FIRST_HALF_REVERT] = {
            .onEntry              = msOad_EnterWaitFirstHalfRevert,
            .onNewImageIdentify   = msOad_HandleNewImageIdentifyEventWaitFirst,
            .onDownloadStart      = msOad_HandleDownloadStartEventGeneric,
            .onDownloadCancelled  = msOad_HandleDownloadCancelledOther,
            .onTimeout            = msOad_HandleDownloadCancelledOther,
            .onGetState           = msOad_HandleGetStateEvent,
        },
        [MS_OAD_WAIT_SECOND_HALF_REVERT] = {
            .onEntry              = msOad_EnterWaitSecondHalfRevert,
            .onNewImageIdentify   = msOad_HandleNewImageIdentifyEventWaitSecond,
            .onDownloadStart      = msOad_HandleDownloadStartEventGeneric,
            .onDownloadCancelled  = msOad_HandleDownloadCancelledOther,
            .onTimeout            = msOad_HandleDownloadCancelledOther,
            .onGetState           = msOad_HandleGetStateEvent,
        },
        [MS_OAD_NEW_IMAGE_READY] = {
            .onGetState           = msOad_HandleGetStateEvent,
            .onImgVerified        = msOad_HandleImageVerifiedEvent,
        },
        [MS_OAD_IMAGE_READY_REVERT] = {
            .onGetState           = msOad_HandleGetStateEvent,
            .onImgVerified        = msOad_HandleImageVerifiedEvent,
        },
        [MS_OAD_WAIT_IMAGE_ENABLE] = {
            .onGetState           = msOad_HandleGetStateEvent,
            .onDownloadCancelled  = msOad_HandleDownloadCancelledWaitImageEnable,
            .onTimeout            = msOad_HandleTimeoutWaitImageEnable,
            .onResetReq           = msOad_HandleResetReqEvent,
        },
        [MS_OAD_DOWNLOADING] = {
            .onEntry              = msOad_EnterDownloading,
            .onDownloadDone       = msOad_HandleDownloadDoneDownloading,
            .onDownloadFailed     = msOad_HandleDownloadFailedDownloading,
            .onDownloadCancelled  = msOad_HandleDownloadFailedDownloading,  // Cancel during download is treated as failure since the image is likely incomplete/invalid
            .onTimeout            = msOad_HandleDownloadFailedDownloading,  // Timeout during download is treated as failure since the image is likely incomplete/invalid
            .onGetState           = msOad_HandleGetStateEventDownloading,
        }
    };

msOadData_t gMsOadData =
{
    .currentState = MS_OAD_UNINITIALIZED, // determined in @ref msOad_OnBootResolveState
    .stateBeforeDownloading = MS_OAD_UNINITIALIZED,
    .stateCallbacks =
    {
        .onNewImageIdentify    = NULL,
        .onDownloadStart       = NULL,
        .onDownloadDone        = NULL,
        .onDownloadFailed      = NULL,
        .onDownloadCancelled   = NULL,
        .onTimeout             = NULL,
        .onGetState            = NULL,
        .onResetReq            = NULL,
        .onInitAndReset        = NULL,
        .onImgVerified         = NULL,
    },
    .journalNextOffset = MS_OAD_VERSION_ENTRY_SIZE,  // First state entry starts after version entry
    .currentBitmap = MS_OAD_BITMAP_ERASED,           // No flags set initially
    .versionEntry =
    {
        .committedVersion = {0, 0, 0, 0},
        .pendingVersion = {0, 0, 0, 0},
        .magic1 = 0,
        .magic2 = 0,
    },
    .msOadNvsHandle = NULL,
    .oadProfileData = {false},
};

//*****************************************************************************
//! Public Functions
//*****************************************************************************

bool MSOAD_InitIfRequired()
{
    // Init the module
    msOad_Init();

    // Resolve the boot state based on flash data and slot versions
    // This determines the initial MS-OAD state and boot mode
    msOadBootMode_e bootMode = msOad_OnBootResolveState();

    if (bootMode == MS_OAD_BOOT_MODE_REGULAR ||
        bootMode == MS_OAD_BOOT_MODE_TEST_IMAGE)
    {
        return false;
    }
    else if (bootMode == MS_OAD_BOOT_MODE_ERROR)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: Boot mode error detected");
        HAL_ASSERT( HAL_ASSERT_CAUSE_UNEXPECTED_ERROR );
        return false;
    }

    // Currently in the middle of MS-OAD process, Initiate the stack here
    // Call the BLEAppUtil module init function
    BLEAppUtil_init(&msOad_CriticalErrorHandler, &msOad_StackInitDoneHandler,
                    &appMsOadMainParams, &appMsOadMainPeriCentParams);

    return true;
}

bStatus_t MSOAD_InitAndReset(struct image_version* pNewImageVersion)
{
    return msOad_HandleApiEvents(MS_OAD_EVT_INIT_AND_RESET, (void*) pNewImageVersion);
}

bStatus_t MSOAD_ImageVerified()
{
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: MSOAD_ImageVerified() called");

    // Call state machine with IMG_VERIFIED event
    return msOad_HandleApiEvents(MS_OAD_EVT_IMG_VERIFIED, NULL);
}

msOadState_e MSOAD_GetState()
{
    return gMsOadData.currentState;
}

//*****************************************************************************
//! Callback Functions
//*****************************************************************************

/*********************************************************************
 * @fn      msOad_ProfileCallback
 *
 * @brief   Callback from OAD profile to manage MS-OAD session.
 *
 *          This callback receives events from the OAD profile and maps
 *          them to the appropriate MS-OAD state machine events.
 *
 *          OAD Session Context:
 *          - In MS-OAD, the OAD profile is used to download half-images
 *          - Depending on state, the download can be:
 *            - Version 2.1 (bootable new image) - when in WAIT_FIRST_HALF
 *            - Version 2.2 (non-bootable new image) - when in WAIT_SECOND_HALF
 *            - Version 1.1 (bootable original) - when in WAIT_FIRST_HALF_REVERT
 *            - Version 1.2 (non-bootable original) - when in WAIT_SECOND_HALF_REVERT
 *
 *          Note: The OAD profile does not natively notify about download
 *          failures. Failure detection requires external timeout monitoring
 *          or modification to the OAD profile.
 *
 * @param   msg - OAD profile message
 *
 * @return  OAD_PROFILE_PROCEED or OAD_PROFILE_CANCEL
 */
static OADProfile_AppCommand_e msOad_ProfileCallback(OADProfile_App_Msg_e msg, void *pData)
{
    OADProfile_AppCommand_e cmd = OAD_PROFILE_CANCEL;

    switch(msg)
    {
        case OAD_PROFILE_MSG_NEW_IMG_IDENTIFY:
        case OAD_PROFILE_MSG_START_DOWNLOAD:
        case OAD_PROFILE_MSG_FINISH_DOWNLOAD:
        case OAD_PROFILE_MSG_CANCEL_DOWNLOAD:
        case OAD_PROFILE_MSG_DOWNLOAD_FAILED:
        case OAD_PROFILE_MSG_TIMEOUT:
        case OAD_PROFILE_MSG_GET_MS_OAD_STATE:
        case OAD_PROFILE_MSG_RESET_REQ:
        {
            if (msOad_HandleOadProfileEvents(msg, pData) == SUCCESS)
            {
                cmd = OAD_PROFILE_PROCEED;
            }

            break;
        }
        case OAD_PROFILE_MSG_REVOKE_IMG_HDR:
        case OAD_PROFILE_MSG_MS_OAD_INIT_REQ:
        case OAD_PROFILE_MSG_MS_OAD_COMMIT:
        {
            cmd = OAD_PROFILE_CANCEL;
            break;
        }

        default:
        {
            cmd = OAD_PROFILE_CANCEL;
            break;
        }
    }

    return (cmd);
}

/*********************************************************************
 * @fn      msOad_CriticalErrorHandler
 *
 * @brief   Critical error handler for MS-OAD operations.
 *
 *          This callback is registered with BLEAppUtil to handle
 *          critical errors during MS-OAD processing. Currently a
 *          placeholder implementation.
 *
 * @param   errorCode - Error code identifying the failure
 * @param   pInfo - Additional error information (may be NULL)
 *
 * @return  None
 */
static void msOad_CriticalErrorHandler(int32 errorCode , void* pInfo)
{
    msOad_RebootDevice();
}

/*********************************************************************
 * @fn      msOad_StackInitDoneHandler
 *
 * @brief   BLE stack initialization complete handler for MS-OAD.
 *
 *          This callback is invoked when the BLE stack completes
 *          initialization during MS-OAD processing. It starts the
 *          OAD profile, peripheral module, and pairing module.
 *
 * @param   deviceInitDoneData - Stack initialization completion data
 *
 * @return  None
 */
static void msOad_StackInitDoneHandler(gapDeviceInitDoneEvent_t *deviceInitDoneData)
{
    bStatus_t status;

    status = OADProfile_start(&msOad_ProfileCallback);

    if (status == SUCCESS)
    {
        status = MsOadPeripheral_start();
    }

    if (status == SUCCESS)
    {
        status = MsOadPairing_start();
    }

    if (status != SUCCESS)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: Failed to start OAD profile or modules, status=%d", status);
        HAL_ASSERT( HAL_ASSERT_CAUSE_UNEXPECTED_ERROR );

        // Reboot on failure to start, as we cannot proceed without the OAD profile running
        msOad_RebootDevice();
    }

#ifdef GAP_BOND_MGR
    // Notify bond manager of service change on every boot, since the OAD process can change the image and thus the GATT database layout.
    // This ensures bonded clients will rediscover services after an OAD, preventing potential issues from stale client caches.
    GAPBondMgr_ServiceChangeInd(0xFFFF, TRUE);
#endif //GAP_BOND_MGR
}

//*****************************************************************************
//! Local Functions
//*****************************************************************************

/*********************************************************************
 * @fn      msOad_Init
 *
 * @brief   Initialize the MS-OAD module and its internal state.
 *
 *          This function opens the NVS handle for the MS-OAD journal,
 *          scans the journal to populate internal data structures,
 *          validates journal consistency with the image_ok flag, and
 *          performs any necessary journal diagnostics and repairs.
 *
 *          Called during early boot before state resolution.
 *
 * @return  None
 */
static void msOad_Init(void)
{
    gMsOadData.msOadNvsHandle = NVS_open(CONFIG_NV_MS_OAD, NULL);

    if (gMsOadData.msOadNvsHandle == NULL)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: Failed to open NVS handle");
        HAL_ASSERT( HAL_ASSERT_CAUSE_UNEXPECTED_ERROR );

        // Reboot on failure to open NVS, as we cannot proceed without access to the journal
        msOad_RebootDevice();
    }

    // Scan the journal first so gMsOadData fields are populated
    // before validation needs to inspect them.
    msOad_ScanJournal();

    // Validate journal vs image_ok consistency (uses scan results)
    msOad_ValidateJournalAndImageOk();

    // Diagnose journal state and take corrective action if needed
    msOad_DiagnoseJournal();

    msOad_ClearProfileData();
}


/*********************************************************************
 * @fn      msOad_ValidateJournalAndImageOk
 *
 * @brief   Pre-step before boot state resolution. Ensures the state
 *          page and image_ok flag are in a consistent state:
 *
 *          1. If journal is erased (idle) and image_ok is NOT set:
 *             Set image_ok to confirm the current image.
 *
 *          2. If journal is NOT erased (OAD in progress) and
 *             image_ok IS set: Erase the journal, since the
 *             previous OAD completed but the journal wasn't cleaned.
 *
 * @return  None
 */
static void msOad_ValidateJournalAndImageOk(void)
{
    bool stateErased = msOad_IsJournalErased();
    bool imageOkSet = msOad_IsImageOkSet();

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ValidateJournalAndImageOk(): stateErased=%d, imageOkSet=%d", stateErased, imageOkSet);

    if (stateErased && !imageOkSet)
    {
        // Journal is clean (idle) but image_ok not set - confirm the current image
        // This case is always true on first boot (after flashing the device).
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ValidateJournalAndImageOk(): Setting image_ok (state erased, image not confirmed)");
        msOad_SetImageOk();

        // ============ CRITICAL POINT K ============
    }
    else if (!stateErased && imageOkSet)
    {
        // Journal has data and image_ok is set. Possible scenarios:
        //
        // (a) Fresh OAD init: journal has version entry + initMsOad state entry,
        //     image_ok was set from the previous normal boot. Preserve journal.
        //
        // (b) Active OAD/revert in progress: MCUboot set image_ok after reverting
        //     to the known-good image. Journal has valid OAD tracking data
        //     (oadStarted, possibly revert flag). Preserve journal.
        //
        // (c) Stale commit data: image_ok already set but erasing the journal
        //     failed due to a power loss - the page left partially erased / corrupted.
        //
        // Distinguishing factor: a valid version entry means the journal has
        // meaningful data (scenarios a or b) — let boot resolve handle it.
        // An invalid version entry means the page is corrupted from a partial
        // erase or partial init (scenario c) — safe to erase.
        if (MS_OAD_IS_ENTRY_VALID(gMsOadData.versionEntry))
        {
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ValidateJournalAndImageOk(): "
                       "Journal has data + image_ok set, valid version entry — preserving journal for boot resolve");


            // Handle a case where we already set image ok but didn't erase the journal
            // This can happen if the device was power cycled at a specific time.
            // If both slots version are the same and equal to the commited\pending versions,
            // erase the journal.
            struct image_header firstSlotHeader;
            struct image_header secondSlotHeader;
            msOad_ReadImageHeader(MS_OAD_SLOT_1_HDR_ADDR, &firstSlotHeader);
            msOad_ReadImageHeader(MS_OAD_SLOT_2_HDR_ADDR, &secondSlotHeader);

            if (msOad_CompareVersions(&firstSlotHeader.ih_ver, &secondSlotHeader.ih_ver) == true &&
                msOad_CompareVersions(&firstSlotHeader.ih_ver, &gMsOadData.versionEntry.pendingVersion) == true)
            {
                if(msOad_EraseJournal() != SUCCESS)
                {
                    // Reboot on failure to erase, as we cannot recover without a clean journal
                    msOad_RebootDevice();
                }
            }
        }
        else
        {
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ValidateJournalAndImageOk(): "
                       "Erasing journal (image_ok set, journal not clean, invalid version entry — corrupted page)");

            // Erase the journal as the page is corrupted and cannot be trusted.
            // This is a recovery mechanism to handle power loss during journal erase or init.
            if(msOad_EraseJournal() != SUCCESS)
            {
                // Reboot on failure to erase, as we cannot recover without a clean journal
                msOad_RebootDevice();
            }
        }
    }
}

/*********************************************************************
 * @fn      msOad_OnBootResolveState
 *
 * @brief   Resolve the initial MS-OAD state at boot time based on
 *          flash data flags and image versions in slots.
 *
 *          This function implements the decision table from:
 *          Multistep_OAD_Boot_Resolve_State_Decision_Table.xlsx
 *
 *          Decision Table Summary:
 *          -----------------------
 *          The decision is based on three flags stored in flash:
 *          - initMsOad: Set when MS-OAD process is initiated
 *          - oadStarted: Set when OAD download has started
 *          - revert: Set when reverting to previous image
 *
 *          Note: The flags are active-low in flash. A bit value of 0 means the
 *          flag is set, and 1 means unset. This is because flash bits are erased
 *          to 1 and can only be cleared to 0 without a full page erase.
 *
 *          And two version comparisons:
 *          - firstSlotVersion: Can be 1.1 (committed) or 2.1 (pending)
 *          - secondSlotVersion: Can be 1.1, 1.2, 2.1, 2.2, or invalid
 *
 *          Version Notation:
 *          - 1.1 = committed version, bootable image
 *          - 1.2 = committed version, non-bootable image
 *          - 2.1 = pending (new) version, bootable image
 *          - 2.2 = pending (new) version, non-bootable image
 *
 *          Decision Table:
 *          ---------------
 *          Case | init | oad | rev | firstSlot             | secondSlot             | Action  | NextState               | Flags
 *          -----+------+-----+-----+-----------------------+------------------------+---------+-------------------------+-----------
 *           0   |  0   |  0  |  X  | X                     | X                      | Regular | IDLE                    |
 *           1   |  1   |  0  |  X  | X                     | X                      | Half    | WAIT_FIRST_HALF         | -initMsOad
 *           2   |  1   |  1  |  X  | X                     | X                      | Error   | UNINITIALIZED           |
 *           3   |  0   |  1  |  0  | 1.1                   | 2.1                    | Half    | WAIT_SECOND_HALF_REVERT | +revert (*)
 *           4   |  0   |  1  |  0  | 1.1                   | 1.2                    | Half    | WAIT_SECOND_HALF_REVERT | +revert (*)
 *           5   |  0   |  1  |  0  | 1.1                   | INVALID (not 1.2/2.1)  | Half    | WAIT_SECOND_HALF_REVERT | +revert (*)
 *           6   |  0   |  1  |  0  | 2.1                   | 1.1                    | Half    | WAIT_SECOND_HALF        |
 *           7   |  0   |  1  |  0  | 2.1                   | 2.2                    | Test    | IMG_READY               | +revert
 *           8   |  0   |  1  |  0  | 2.1                   | INVALID (not 1.1/2.2)  | Half    | WAIT_FIRST_HALF_REVERT  |
 *           9   |  0   |  1  |  1  | 1.1                   | 2.1                    | Half    | WAIT_SECOND_HALF_REVERT |
 *          10   |  0   |  1  |  1  | 1.1                   | 1.2                    | Test    | IMG_READY_REVERT        |
 *          11   |  0   |  1  |  1  | 1.1                   | INVALID (not 1.2/2.1)  | Half    | WAIT_SECOND_HALF_REVERT |
 *          12   |  0   |  1  |  1  | 2.1                   | X                      | Half    | WAIT_FIRST_HALF_REVERT  |
 *          13   |  0   |  1  |  X  | INVALID (not 1.1/2.1) | X                      | Error   | UNINITIALIZED           |
 *
 *          (*) revert flag set via msOad_EnterWaitSecondHalfRevert onEntry handler,
 *              not directly in this function.
 *
 *          Possible Journal Bitmap Values:
 *          --------------------------------
 *          Value       | Bits cleared          | Meaning                   | Written by
 *          ------------+-----------------------+---------------------------+----------------------------------
 *          0xFFFFFFFF  | none                  | No flags set              | Boot resolve (consumes init flag)
 *          0xFFFFFFFE  | bit 0 (initMsOad)     | OAD init requested        | msOad_HandleInitAndResetEvent
 *          0xFFFFFFFD  | bit 1 (oadStarted)    | Download started          | msOad_HandleDownloadStartEvent
 *          0xFFFFFFF9  | bits 1+2 (oadStarted  | Revert requested          | msOad_EnterRevertMode
 *                      |          + revert)    |                           |
 *
 * @return  Boot mode to use (REGULAR, HALF, or TEST_IMAGE)
 */
static msOadBootMode_e msOad_OnBootResolveState(void)
{
    msOadState_e stateToSet = MS_OAD_IDLE;
    msOadBootMode_e bootMode = MS_OAD_BOOT_MODE_REGULAR;
    struct image_header firstSlotHeader;
    struct image_header secondSlotHeader;
    bool firstIsCommitted;              // First slot has committed version (1.1)
    bool firstIsPending;                // First slot has pending version (2.1)
    bool secondIsCommittedBootable;     // Second slot has 1.1
    bool secondIsCommittedNonBootable;  // Second slot has 1.2
    bool secondIsPendingNonBootable;    // Second slot has 2.2

    // Read flags from journal bitmap
    bool initMsOadSet  = MS_OAD_BITMAP_IS_SET(gMsOadData.currentBitmap, MS_OAD_FLAG_BIT_INIT_OAD);
    bool oadStartedSet = MS_OAD_BITMAP_IS_SET(gMsOadData.currentBitmap, MS_OAD_FLAG_BIT_OAD_STARTED);
    bool revertSet     = MS_OAD_BITMAP_IS_SET(gMsOadData.currentBitmap, MS_OAD_FLAG_BIT_REVERT);

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_OnBootResolveState(): initMsOad=%d, oadStarted=%d, revert=%d, versionValid=%d",
               initMsOadSet, oadStartedSet, revertSet,
               MS_OAD_IS_ENTRY_VALID(gMsOadData.versionEntry));

    // Get headers from both slots
    msOad_ReadImageHeader(MS_OAD_SLOT_1_HDR_ADDR, &firstSlotHeader);
    msOad_ReadImageHeader(MS_OAD_SLOT_2_HDR_ADDR, &secondSlotHeader);

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_OnBootResolveState(): firstSlotMagic=0x%08X, firstSlotFlags=0x%08X",
               firstSlotHeader.ih_magic, firstSlotHeader.ih_flags);
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_OnBootResolveState(): firstSlotVersion=%d.%d.%d.%d",
               firstSlotHeader.ih_ver.iv_major, firstSlotHeader.ih_ver.iv_minor, firstSlotHeader.ih_ver.iv_revision, firstSlotHeader.ih_ver.iv_build_num);
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_OnBootResolveState(): secondSlotMagic=0x%08X, secondSlotFlags=0x%08X",
               secondSlotHeader.ih_magic, secondSlotHeader.ih_flags);
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_OnBootResolveState(): secondSlotVersion=%d.%d.%d.%d",
               secondSlotHeader.ih_ver.iv_major, secondSlotHeader.ih_ver.iv_minor, secondSlotHeader.ih_ver.iv_revision, secondSlotHeader.ih_ver.iv_build_num);

    struct image_trailer imgTrailer;
    if (msOad_GetTrailer(&imgTrailer) != SUCCESS)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_OnBootResolveState(): Failed to get image OK flag");
    }
    else
    {
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_OnBootResolveState(): Swap Type flag=%d", imgTrailer.swap_type);
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_OnBootResolveState(): Copy Done flag=%d", imgTrailer.copy_done);
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_OnBootResolveState(): Image OK flag=%d", imgTrailer.image_ok);
    }

    // Classify first slot version (use cached version entry)
    firstIsCommitted =  msOad_IsVersionValidAndBootable(&firstSlotHeader, &gMsOadData.versionEntry.committedVersion);
    firstIsPending   =  msOad_IsVersionValidAndBootable(&firstSlotHeader, &gMsOadData.versionEntry.pendingVersion);

    // Classify second slot version
    secondIsCommittedBootable    =  msOad_IsVersionValidAndBootable(&secondSlotHeader,    &gMsOadData.versionEntry.committedVersion);
    secondIsCommittedNonBootable =  msOad_IsVersionValidAndNonBootable(&secondSlotHeader, &gMsOadData.versionEntry.committedVersion);
    secondIsPendingNonBootable   =  msOad_IsVersionValidAndNonBootable(&secondSlotHeader, &gMsOadData.versionEntry.pendingVersion);

    //=========================================================================
    // Decision Table Implementation
    //=========================================================================
    // Reference: Multistep_OAD_Boot_Resolve_State_Decision_Table.xlsx
    //
    // The table has the following structure:
    // | initMsOad | oadStarted | revert | firstSlot | secondSlot | -> NextState + flag updates
    //=========================================================================

    // Case 0: initMsOad=0, oadStarted=0
    if (!initMsOadSet && !oadStartedSet)
    {
        stateToSet = MS_OAD_IDLE;
        bootMode = MS_OAD_BOOT_MODE_REGULAR;
    }
    // Case 1: initMsOad=1, oadStarted=0
    else if (initMsOadSet && !oadStartedSet)
    {
        stateToSet = MS_OAD_WAIT_FIRST_HALF;
        bootMode = MS_OAD_BOOT_MODE_HALF;

        // Consume initMsOad by appending an erased bitmap entry (valid magic, no flags).
        // oadStarted will be set later when the download actually begins.
        if (msOad_AppendStateEntry(MS_OAD_BITMAP_ERASED) != SUCCESS)
        {
            stateToSet = MS_OAD_UNINITIALIZED;
            bootMode = MS_OAD_BOOT_MODE_ERROR;
        }
    }
    // Case 2: initMsOad=1, oadStarted=1 — unexpected/invalid state combination
    else if (initMsOadSet && oadStartedSet)
    {
        stateToSet = MS_OAD_UNINITIALIZED;
        bootMode = MS_OAD_BOOT_MODE_ERROR;
    }
    // Cases 3-13: initMsOad=0, oadStarted=1 — branch by first slot version
    else
    {
        if (firstIsCommitted)
        {
            // Cases 3,4,5,9,11: first=1.1, any second (except revert+1.2) -> WAIT_SECOND_HALF_REVERT
            // Case 10:           first=1.1, revert=1, second=1.2           -> IMG_READY_REVERT
            if (revertSet && secondIsCommittedNonBootable)
            {
                // Case 10: firstSlot=1.1, secondSlot=1.2, revert=1
                stateToSet = MS_OAD_IMAGE_READY_REVERT;
                bootMode = MS_OAD_BOOT_MODE_TEST_IMAGE;
            }
            else
            {
                // Cases 3,4,5,9,11
                stateToSet = MS_OAD_WAIT_SECOND_HALF_REVERT;
                bootMode = MS_OAD_BOOT_MODE_HALF;
            }
        }
        else if (firstIsPending)
        {
            // Case 6:     first=2.1, revert=0, second=1.1  -> WAIT_SECOND_HALF
            // Case 7:     first=2.1, revert=0, second=2.2  -> IMG_READY
            // Cases 8,12: first=2.1, anything else         -> WAIT_FIRST_HALF_REVERT
            if (!revertSet && secondIsCommittedBootable)
            {
                // Case 6: firstSlot=2.1, secondSlot=1.1, revert=0
                stateToSet = MS_OAD_WAIT_SECOND_HALF;
                bootMode = MS_OAD_BOOT_MODE_HALF;
            }
            else if (!revertSet && secondIsPendingNonBootable)
            {
                // Case 7: firstSlot=2.1, secondSlot=2.2, revert=0
                stateToSet = MS_OAD_NEW_IMAGE_READY;
                bootMode = MS_OAD_BOOT_MODE_TEST_IMAGE;

                // Set revert=1 in case the new image is unable to create a connection,
                // so that on next boot we revert to the original image.
                // Note: Check why mcuboot doesn't erase the second slot in this case
                if (msOad_EnterRevertMode() != SUCCESS)
                {
                    stateToSet = MS_OAD_UNINITIALIZED;
                    bootMode = MS_OAD_BOOT_MODE_ERROR;
                }
            }
            else
            {
                // Cases 8,12: firstSlot=2.1, all other second slot / revert combinations
                stateToSet = MS_OAD_WAIT_FIRST_HALF_REVERT;
                bootMode = MS_OAD_BOOT_MODE_HALF;
            }
        }
        else
        {
            // Case 13: first slot version unrecognized
            stateToSet = MS_OAD_UNINITIALIZED;
            bootMode = MS_OAD_BOOT_MODE_ERROR;
        }
    }

    // Set the resolved public state (triggers onEntry for the new state)
    msOad_SetState(stateToSet);

    return bootMode;
}

/*********************************************************************
 * @fn      msOad_HandleOadProfileEvents
 *
 * @brief   Dispatch OAD profile events to state-specific handlers.
 *
 *          This function receives events from the OAD profile callback
 *          and dispatches them to the appropriate handler based on the
 *          current MS-OAD state. The state machine uses a callback
 *          table to map events to handlers.
 *
 * @param   event - OAD profile message/event type
 * @param   pData - Event-specific data (may be NULL)
 *
 * @return  SUCCESS if event was handled, FAILURE otherwise
 */
static bStatus_t msOad_HandleOadProfileEvents(OADProfile_App_Msg_e event, void* pData)
{
    bStatus_t status = FAILURE;
    msOadEventCallback cb = NULL;

    switch (event)
    {
        case OAD_PROFILE_MSG_NEW_IMG_IDENTIFY:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleOadProfileEvents(): Received NEW_IMG_IDENTIFY event from OAD profile");
            cb = gMsOadData.stateCallbacks.onNewImageIdentify;
            break;
        case OAD_PROFILE_MSG_START_DOWNLOAD:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleOadProfileEvents(): Received START_DOWNLOAD event from OAD profile");
            cb = gMsOadData.stateCallbacks.onDownloadStart;
            break;
        case OAD_PROFILE_MSG_FINISH_DOWNLOAD:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleOadProfileEvents(): Received FINISH_DOWNLOAD event from OAD profile");
            cb = gMsOadData.stateCallbacks.onDownloadDone;
            break;
        case OAD_PROFILE_MSG_DOWNLOAD_FAILED:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleOadProfileEvents(): Received DOWNLOAD_FAILED event from OAD profile");
            cb = gMsOadData.stateCallbacks.onDownloadFailed;
            break;
        case OAD_PROFILE_MSG_CANCEL_DOWNLOAD:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleOadProfileEvents(): Received CANCEL_DOWNLOAD event from OAD profile");
            cb = gMsOadData.stateCallbacks.onDownloadCancelled;
            break;
        case OAD_PROFILE_MSG_TIMEOUT:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleOadProfileEvents(): Received TIMEOUT event from OAD profile");
            cb = gMsOadData.stateCallbacks.onTimeout;
            break;
        case OAD_PROFILE_MSG_GET_MS_OAD_STATE:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleOadProfileEvents(): Received GET_MS_OAD_STATE event from OAD profile");
            cb = gMsOadData.stateCallbacks.onGetState;
            break;
        case OAD_PROFILE_MSG_RESET_REQ:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleOadProfileEvents(): Received RESET_REQ event from OAD profile");
            cb = gMsOadData.stateCallbacks.onResetReq;
            break;
        default:
            // Unknown event - ignore
            status = FAILURE;
            Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_HandleOadProfileEvents(): Received unknown event from OAD profile: %d", event);
            break;
    }

    if (cb != NULL)
    {
        status = cb(pData);
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleOadProfileEvents(): Event handler returned status: %d", status);
    }

    return status;
}

/*********************************************************************
 * @fn      msOad_HandleApiEvents
 *
 * @brief   Dispatch MS-OAD API events to state-specific handlers.
 *
 *          This function handles events from the public MS-OAD API
 *          (MSOAD_InitAndReset, MSOAD_ImageVerified) and dispatches
 *          them to the appropriate handler based on the current state.
 *
 * @param   event - MS-OAD API event type
 * @param   pData - Event-specific data (may be NULL)
 *
 * @return  SUCCESS if event was handled, FAILURE otherwise
 */
static bStatus_t msOad_HandleApiEvents(msOadApiEvent_e event, void* pData)
{
    bStatus_t status = FAILURE;
    msOadApiEventCallback cb = NULL;

    switch (event)
    {
        case MS_OAD_EVT_INIT_AND_RESET:
            cb = gMsOadData.stateCallbacks.onInitAndReset;
            break;
        case MS_OAD_EVT_IMG_VERIFIED:
            cb = gMsOadData.stateCallbacks.onImgVerified;
            break;
        default:
            status = FAILURE;
            // Unknown event - ignore
            break;
    }

    if (cb != NULL)
    {
        status = cb(pData);
    }

    return status;
}

/*********************************************************************
 * @fn      msOad_HandleInitAndResetEvent
 *
 * @brief   Initialize MS-OAD process and prepare for new image download.
 *
 *          This handler initiates the MS-OAD state machine by:
 *          1. Validating the new image version is greater than committed
 *          2. Erasing the journal
 *          3. Writing the version entry with committed and pending versions
 *          4. Setting the initMsOad flag
 *          5. Rebooting to enter WAIT_FIRST_HALF state
 *
 * @param   pData - Pointer to new image version (struct image_version*)
 *
 * @return  SUCCESS if initialization succeeded, FAILURE otherwise
 */
static bStatus_t msOad_HandleInitAndResetEvent(void* pData)
{
    struct image_version committedVersion;
    struct image_version newImageVersion;

    if (pData == NULL)
    {
        return FAILURE;
    }

    // Cast to image version
    newImageVersion = *((struct image_version*)(pData));

    // Get committed version from current image in slot 1
    msOad_GetSlotVersion(MS_OAD_SLOT_1_HDR_ADDR, &committedVersion);

    // Ensure new image version is greater than committed version
    if (SwUpdate_IsVersionGreater(&newImageVersion, &committedVersion) == false)
    {
        return FAILURE;
    }

    if (gMsOadData.msOadNvsHandle == NULL)
    {
        return FAILURE;
    }

    // Erase the journal and reset journal tracking
    if (msOad_EraseJournal() != SUCCESS)
    {
        return FAILURE;
    }

    // ============ CRITICAL POINT A ============

    // Write version entry at offset 0 (word-by-word, magic last)
    if (msOad_WriteVersionEntry(&committedVersion, &newImageVersion) != SUCCESS)
    {
        return FAILURE;
    }

    // ============ CRITICAL POINT B ============

    // Append state entry with initMsOad flag set
    if (msOad_AppendStateEntry(MS_OAD_BITMAP_INIT) != SUCCESS)
    {
        return FAILURE;
    }

    // Reboot to enter WAIT_FIRST_HALF state
    msOad_RebootDevice();

    // Shouldn't get here, but for compiler satisfaction
    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleImageVerifiedEvent
 *
 * @brief   Handle image verification completion from the application.
 *
 *          This handler is called when the application confirms the
 *          downloaded image is valid and functional. It:
 *          1. Sets the image_ok flag to confirm the current image
 *          2. Erases the journal to mark OAD completion
 *          3. Reboots the device to enter IDLE state
 *
 * @param   pData - Unused (may be NULL)
 *
 * @return  SUCCESS if verification completed, FAILURE otherwise
 */
static bStatus_t msOad_HandleImageVerifiedEvent(void* pData)
{
    VOID pData;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleImageVerifiedEvent() called");

    // Step 1: Set image_ok flag in MCUboot trailer to confirm current image
    if (msOad_SetImageOk() != SUCCESS)
    {
        return FAILURE;
    }

    // ============ CRITICAL POINT C ============

    // Step 2: Erase the entire journal — marks completion
    // On next boot: page is erased → IDLE
    if (msOad_EraseJournal() != SUCCESS)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_HandleImageVerifiedEvent(): msOad_EraseJournal() FAILED");
        // Don't return, continue with reboot
    }

    msOad_RebootDevice();

    // Shouldn't get here, but for compiler satisfaction
    return FAILURE;
}

/*********************************************************************
 * @fn      msOad_HandleNewImageIdentifyEvent
 *
 * @brief   Validate incoming image header for a new image identify event.
 *
 *          Validates that the incoming image:
 *          - Matches the expected bootability (bootable for first half,
 *            non-bootable for second half)
 *          - Matches expected version (pending for normal OAD,
 *            committed for revert operation)
 *
 *          If valid, marks the image as identified to allow download.
 *
 * @param   pImageHeader   - Pointer to the incoming image header
 * @param   expectBootable - true if the image must be bootable (first half),
 *                           false if it must be non-bootable (second half)
 *
 * @return  SUCCESS if image is valid, FAILURE otherwise
 */
static bStatus_t msOad_HandleNewImageIdentifyEvent(struct image_header* pImageHeader, bool expectBootable)
{
    struct image_version *pExpectedVersion;

    if (msOad_IsSlotBootable(pImageHeader->ih_flags) != expectBootable)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_HandleNewImageIdentifyEvent(): Image bootability mismatch (expected bootable=%d)", expectBootable);
        return FAILURE;
    }

    // Determine expected version based on revert flag
    if (MS_OAD_BITMAP_IS_SET(gMsOadData.currentBitmap, MS_OAD_FLAG_BIT_REVERT))
    {
        // In revert mode: expect committed version
        pExpectedVersion = &gMsOadData.versionEntry.committedVersion;
    }
    else
    {
        // Normal mode: expect pending version
        pExpectedVersion = &gMsOadData.versionEntry.pendingVersion;
    }

    if (msOad_CompareVersions(&pImageHeader->ih_ver, pExpectedVersion) == false)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_HandleNewImageIdentifyEvent(): Image version does not match expected version");
        return FAILURE;
    }

    // Mark that new image is identified and valid so that download can proceed
    msOad_SetImageIdentified(true);

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleNewImageIdentifyEventWaitFirst
 *
 * @brief   onNewImageIdentify handler for WAIT_FIRST_HALF / WAIT_FIRST_HALF_REVERT.
 *          First half image must be bootable.
 *
 * @param   pData - Pointer to image header (struct image_header*)
 *
 * @return  SUCCESS if image is valid, FAILURE otherwise
 */
static bStatus_t msOad_HandleNewImageIdentifyEventWaitFirst(void* pData)
{
    if (pData == NULL)
    {
        return FAILURE;
    }

    // Note: pData comes from OAD profile which provides a properly aligned local buffer,
    // not direct flash memory, so packed struct field access is safe
    return msOad_HandleNewImageIdentifyEvent((struct image_header*)pData, true);
}

/*********************************************************************
 * @fn      msOad_HandleNewImageIdentifyEventWaitSecond
 *
 * @brief   onNewImageIdentify handler for WAIT_SECOND_HALF / WAIT_SECOND_HALF_REVERT.
 *          Second half image must be non-bootable.
 *
 * @param   pData - Pointer to image header (struct image_header*)
 *
 * @return  SUCCESS if image is valid, FAILURE otherwise
 */
static bStatus_t msOad_HandleNewImageIdentifyEventWaitSecond(void* pData)
{
    if (pData == NULL)
    {
        return FAILURE;
    }

    // Note: pData comes from OAD profile which provides a properly aligned local buffer,
    // not direct flash memory, so packed struct field access is safe
    return msOad_HandleNewImageIdentifyEvent((struct image_header*)pData, false);
}

/*********************************************************************
 * @fn      msOad_HandleDownloadStartEventWaitFirstHalf
 *
 * @brief   Handle download start request when waiting for first half image.
 *
 *          This handler processes the OAD profile's request to begin
 *          downloading the first half image. If the image was identified
 *          and validation passed, it sets the oadStarted flag in the
 *          journal and transitions to DOWNLOADING state.
 *
 *          If SUCCESS is returned, the OAD profile will proceed with
 *          the download. If FAILURE is returned, the download is cancelled.
 *
 * @param   pData - Unused (may be NULL)
 *
 * @return  SUCCESS to allow download, FAILURE to cancel
 */
static bStatus_t msOad_HandleDownloadStartEventWaitFirstHalf(void* pData)
{
    VOID pData;

    // Check that image was identified before starting download
    if (gMsOadData.oadProfileData.isImageIdentified == false)
    {
        return FAILURE;
    }

    // Set oadStarted in the journal now that the download has actually begun.
    uint32_t newBitmap = MS_OAD_BITMAP_SET(gMsOadData.currentBitmap, MS_OAD_FLAG_BIT_OAD_STARTED);
    if (msOad_AppendStateEntry(newBitmap) != SUCCESS)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_HandleDownloadStartEventWaitFirstHalf(): Failed to append oadStarted entry");
        return FAILURE;
    }

    // ============ CRITICAL POINT D ============

    msOad_SetState(MS_OAD_DOWNLOADING);

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleDownloadStartEventGeneric
 *
 * @brief   Handle download start request for subsequent downloads.
 *
 *          This handler processes the OAD profile's request to begin
 *          downloading when not in WAIT_FIRST_HALF state. It verifies
 *          the image was identified and transitions to DOWNLOADING state.
 *
 *          If SUCCESS is returned, the OAD profile will proceed with
 *          the download. If FAILURE is returned, the download is cancelled.
 *
 * @param   pData - Unused (may be NULL)
 *
 * @return  SUCCESS to allow download, FAILURE to cancel
 */
static bStatus_t msOad_HandleDownloadStartEventGeneric(void* pData)
{
    VOID pData;
    // Check that image was identified before starting download
    if (gMsOadData.oadProfileData.isImageIdentified == false)
    {
        return FAILURE;
    }

    msOad_SetState(MS_OAD_DOWNLOADING);

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleDownloadDoneDownloading
 *
 * @brief   Handle download done event while in DOWNLOADING state.
 *          Uses common logic regardless of previous state.
 *
 * @param   pData - unused
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_HandleDownloadDoneDownloading(void* pData)
{
    VOID pData;
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleDownloadDoneDownloading() - Download complete, transitioning to MS_OAD_WAIT_IMAGE_ENABLE");
    msOad_SetState(MS_OAD_WAIT_IMAGE_ENABLE);
    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleDownloadFailedDownloading
 *
 * @brief   Handle download failed\cancelled event while in DOWNLOADING state.
 *          Uses the saved previous state to determine appropriate action.
 *
 * @param   pData - unused
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_HandleDownloadFailedDownloading(void* pData)
{
    VOID pData;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleDownloadFailedDownloading() - Download failed from state %d",
               gMsOadData.stateBeforeDownloading);

    // Handle based on the state we were in before downloading started
    switch (gMsOadData.stateBeforeDownloading)
    {
        case MS_OAD_WAIT_FIRST_HALF:
        case MS_OAD_WAIT_SECOND_HALF:
            // Download failed - enter revert mode and reboot
            msOad_RevokeSecondaryImageIfRequired();
            // ============ CRITICAL POINT G ============
            // TODO: Handle failure of the next function
            msOad_EnterRevertMode();
            // ============ CRITICAL POINT H ============
            msOad_RebootDevice();
            break;

        case MS_OAD_WAIT_FIRST_HALF_REVERT:
        case MS_OAD_WAIT_SECOND_HALF_REVERT:
            // Download failed during revert - reboot
            msOad_RevokeSecondaryImageIfRequired();
            msOad_RebootDevice();
            break;
        default:
            Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_HandleDownloadFailedDownloading() - Unexpected previous state: %d",
                       gMsOadData.stateBeforeDownloading);
            return FAILURE;
    }

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleDownloadCancelledWaitFirstHalf
 *
 * @brief   Handle download cancelled event while in WAIT_FIRST_HALF state.
 *          When getting cancel in wait_first_half state, we erase the
 *          journal and reboot the device.
 *
 * @param   pData - unused
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_HandleDownloadCancelledWaitFirstHalf(void* pData)
{
    VOID pData;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleDownloadCancelledWaitFirstHalf() - Cancel received in WAIT_FIRST_HALF");

    // Erase the journal
    if (msOad_EraseJournal() != SUCCESS)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_HandleDownloadCancelledWaitFirstHalf(): msOad_EraseJournal() FAILED");
    }

    // Reboot the device. On next boot - device will enter IDLE state as journal is erased
    msOad_RebootDevice();

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleDownloadCancelledOther
 *
 * @brief   Handle download cancelled event while in WAIT_SECOND_HALF,
 *          WAIT_FIRST_HALF_REVERT, or WAIT_SECOND_HALF_REVERT states.
 *          When getting cancel in these states, we enter revert mode
 *          and clear profile data.
 *
 * @param   pData - unused
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_HandleDownloadCancelledOther(void* pData)
{
    VOID pData;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleDownloadCancelledOther() - Cancel received in config state");

    // Enter revert mode
    if (msOad_EnterRevertMode() != SUCCESS)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_HandleDownloadCancelledOther(): msOad_EnterRevertMode() FAILED");
        return FAILURE;
    }

    // Reboot the device, next state will be one of the revert states
    msOad_RebootDevice();

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleDownloadCancelledWaitImageEnable
 *
 * @brief   Handle download cancelled event while in WAIT_IMAGE_ENABLE state.
 *          When getting cancel in this state, we ignore and count it like
 *          image is enabled, and reboot the device.
 *
 * @param   pData - unused
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_HandleDownloadCancelledWaitImageEnable(void* pData)
{
    VOID pData;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleDownloadCancelledWaitImageEnable() - Cancel received in WAIT_IMAGE_ENABLE, treating as image enable");

    // Ignore the cancel and treat it like image is enabled
    // Just reboot the device (no journal erase or revert mode)
    msOad_RebootDevice();

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleTimeoutWaitImageEnable
 *
 * @brief   Handle timeout event while in WAIT_IMAGE_ENABLE state.
 *          When timeout occurs in this state, just reboot the device.
 *
 * @param   pData - unused
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_HandleTimeoutWaitImageEnable(void* pData)
{
    VOID pData;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleTimeoutWaitImageEnable() - Timeout in WAIT_IMAGE_ENABLE, rebooting");

    // Just reboot the device
    msOad_RebootDevice();

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleGetStateEvent
 *
 * @brief   Handle get state event from OAD profile.
 *          Returns the current MS-OAD state via pData pointer.
 *
 * @param   pData - pointer to store the current state (cast to uint8*)
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_HandleGetStateEvent(void* pData)
{
    // Return the state via pData pointer
    *((uint8*)pData) = (uint8) gMsOadData.currentState;

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleGetStateEventDownloading
 *
 * @brief   Handle get state event from OAD profile, when currently
 *          downloading.
 *          Returns the MS-OAD state via pData pointer.
 *
 * @param   pData - pointer to store the current state (cast to uint8*)
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_HandleGetStateEventDownloading(void* pData)
{
    // Return the saved state (before entering DOWNLOADING),
    // as Downloading is not a public state
    *((uint8*)pData) = (uint8) gMsOadData.stateBeforeDownloading;

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_HandleResetReqEvent
 *
 * @brief   Handle reset request event from OAD profile.
 *          This is called when the link terminates and OAD profile
 *          requests permission to reset the device.
 *          In WAITING_IMAGE_ENABLE state, we allow the reset.
 *
 * @param   pData - unused
 *
 * @return  SUCCESS to allow reset
 */
static bStatus_t msOad_HandleResetReqEvent(void* pData)
{
    VOID pData;
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_HandleResetReqEvent() - Allowing reset in WAITING_IMAGE_ENABLE state");
    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_EnterIdle
 *
 * @brief   Entry handler for IDLE state.
 *
 *          Erases the journal if it contains data, resetting it to a
 *          clean state. Reboots if the erase fails, as the system cannot
 *          recover without a clean journal.
 *
 * @param   pData - Unused (may be NULL)
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_EnterIdle(void* pData)
{
    VOID pData;

    if(msOad_EraseJournal() != SUCCESS)
    {
        // Reboot on failure to erase, as we cannot recover without a clean journal
        msOad_RebootDevice();
    }

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_EnterWaitFirstHalf
 *
 * @brief   Entry handler for WAIT_FIRST_HALF state.
 *
 *          Prepares the module to receive the first half (bootable)
 *          of the new image by clearing OAD profile data.
 *
 * @param   pData - Unused (may be NULL)
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_EnterWaitFirstHalf(void* pData)
{
    VOID pData;
    // Clear Profile Data
    msOad_ClearProfileData();

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_EnterWaitSecondHalf
 *
 * @brief   Entry handler for WAIT_SECOND_HALF state.
 *
 *          Prepares the module to receive the second half (non-bootable)
 *          of the new image by clearing OAD profile data.
 *
 * @param   pData - Unused (may be NULL)
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_EnterWaitSecondHalf(void* pData)
{
    VOID pData;
    // Clear Profile Data
    msOad_ClearProfileData();

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_EnterWaitFirstHalfRevert
 *
 * @brief   Entry handler for WAIT_FIRST_HALF_REVERT state.
 *
 *          Prepares the module to revert to the original image by:
 *          1. Entering revert mode (setting revert flag)
 *          2. Clearing OAD profile data
 *
 *          The device will then receive the first half (bootable)
 *          of the original (committed) image.
 *
 * @param   pData - Unused (may be NULL)
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_EnterWaitFirstHalfRevert(void* pData)
{
    VOID pData;
    // TODO: Handle failure of the next function
    msOad_EnterRevertMode();

    // ============ CRITICAL POINT I ============

    // Clear Profile Data
    msOad_ClearProfileData();

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_EnterWaitSecondHalfRevert
 *
 * @brief   Entry handler for WAIT_SECOND_HALF_REVERT state.
 *
 *          Prepares the module to revert to the original image by:
 *          1. Entering revert mode (setting revert flag)
 *          2. Clearing OAD profile data
 *
 *          The device will then receive the second half (non-bootable)
 *          of the original (committed) image.
 *
 * @param   pData - Unused (may be NULL)
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_EnterWaitSecondHalfRevert(void* pData)
{
    VOID pData;
    // TODO: Handle failure of the next function
    msOad_EnterRevertMode();

    // ============ CRITICAL POINT J ============

    // Clear Profile Data
    msOad_ClearProfileData();

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_EnterDownloading
 *
 * @brief   Entry handler for DOWNLOADING state.
 *          Saves the current state (before transition) so we can
 *          determine the correct action on download completion/failure.
 *
 * @param   pData - unused
 *
 * @return  SUCCESS
 */
static bStatus_t msOad_EnterDownloading(void* pData)
{
    VOID pData;

    // Save the current state before we transition to DOWNLOADING
    // Note: When onEntry is called, currentState still holds the previous state
    gMsOadData.stateBeforeDownloading = gMsOadData.currentState;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_EnterDownloading(): Saved previous state %d",
               gMsOadData.stateBeforeDownloading);

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_NvsWriteWord
 *
 * @brief   Write a single 32-bit word to the NVS region at the given offset.
 *          Uses NVS_WRITE_POST_VERIFY to confirm the write.
 *          No erase is performed — only 1->0 bit flips on erased flash.
 *
 * @param   offset - byte offset within the NVS region
 * @param   value  - 32-bit word to write
 *
 * @return  SUCCESS if write succeeded, FAILURE otherwise
 */
static bStatus_t msOad_NvsWriteWord(uint32_t offset, uint32_t value)
{
    if (gMsOadData.msOadNvsHandle == NULL)
    {
        return FAILURE;
    }

    if (NVS_write(gMsOadData.msOadNvsHandle, offset, (uint8_t *)&value,
                  sizeof(uint32_t), NVS_WRITE_POST_VERIFY) != NVS_STATUS_SUCCESS)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_NvsWriteWord(): NVS_write failed at offset 0x%lx", (unsigned long)offset);
        return FAILURE;
    }

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_IsJournalErased
 *
 * @brief   Check if the entire MS-OAD journal is erased by reading
 *          the NVS region directly and verifying all bytes are 0xFF.
 *
 * @return  true if entire page is erased (all 0xFF), false otherwise
 */
static bool msOad_IsJournalErased(void)
{
    NVS_Attrs regionAttrs;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_IsJournalErased() called");

    if (gMsOadData.msOadNvsHandle == NULL)
    {
        return false;
    }

    NVS_getAttrs(gMsOadData.msOadNvsHandle, &regionAttrs);

    // Read directly from flash memory — NVS region is memory-mapped
    const uint8_t *pFlash = (const uint8_t *)regionAttrs.regionBase;

    for (uint32_t i = 0; i < regionAttrs.regionSize; i++)
    {
        if (pFlash[i] != 0xFF)
        {
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_IsJournalErased(): page NOT erased (byte[%lu]=0x%02x)",
                       (unsigned long)i, pFlash[i]);
            return false;
        }
    }

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_IsJournalErased(): page is erased (%lu bytes all 0xFF)",
               (unsigned long)regionAttrs.regionSize);
    return true;
}

/*********************************************************************
 * @fn      msOad_EraseJournal
 *
 * @brief   Erases the journal and resets tracking variables.
 *
 * @note    When journal is already erased, only resets tracking variables.
 *
 * @return  SUCCESS if erase succeeded or journal is already erased,
 *          FAILURE otherwise
 */
static bStatus_t msOad_EraseJournal(void)
{
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_EraseJournal() called");

    if (gMsOadData.msOadNvsHandle == NULL)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_EraseJournal(): NVS handle is NULL");
        return FAILURE;
    }

    // Erase the journal only if not already erased.
    if (!msOad_IsJournalErased())
    {
        if (NVS_erase(gMsOadData.msOadNvsHandle, 0, MS_OAD_JOURNAL_PAGE_SIZE) != NVS_STATUS_SUCCESS)
        {
            Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_EraseJournal(): NVS_erase failed");
            return FAILURE;
        }
    }

    // Reset journal tracking
    memset(&gMsOadData.versionEntry, 0xFF, sizeof(gMsOadData.versionEntry));
    gMsOadData.currentBitmap = MS_OAD_BITMAP_ERASED;
    gMsOadData.journalNextOffset = MS_OAD_VERSION_ENTRY_SIZE;

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_WriteVersionEntry
 *
 * @brief   Write the version entry at offset 0 of the journal.
 *          Writes word-by-word with the magic written last for power-loss safety.
 *          After success, updates gMsOadData.versionEntry cache and resets
 *          journal tracking to the first state entry slot.
 *
 * @param   pCommitted - committed (original) version
 * @param   pPending   - pending (new) version
 *
 * @return  SUCCESS if all words written successfully, FAILURE otherwise
 */
static bStatus_t msOad_WriteVersionEntry(const struct image_version *pCommitted, const struct image_version *pPending)
{
    uint32_t offset = 0;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_WriteVersionEntry(): Writing version entry");

    // Write magic1 FIRST — start marker
    if (msOad_NvsWriteWord(offset, MS_OAD_JOURNAL_MAGIC) != SUCCESS) return FAILURE;
    offset += sizeof(uint32_t);

    // ============ CRITICAL POINT L ============

    // Write committedVersion then pendingVersion (8 bytes = 2 words each)
    // Use memcpy to safely extract words from packed struct
    const struct image_version *versions[] = {pCommitted, pPending};
    uint32_t word;
    for (uint8_t v = 0; v < 2; v++)
    {
        for (uint8_t i = 0; i < sizeof(struct image_version) / sizeof(uint32_t); i++)
        {
            memcpy(&word, (const uint8_t *)versions[v] + i * sizeof(uint32_t), sizeof(uint32_t));
            if (msOad_NvsWriteWord(offset, word) != SUCCESS)
            {
                return FAILURE;
            }

            // ============ CRITICAL POINT M ============

            offset += sizeof(uint32_t);
        }
    }

    // Write magic2 LAST — end marker, validates the entry
    if (msOad_NvsWriteWord(offset, MS_OAD_JOURNAL_MAGIC) != SUCCESS) return FAILURE;

    // Update cached version entry
    gMsOadData.versionEntry.committedVersion = *pCommitted;
    gMsOadData.versionEntry.pendingVersion = *pPending;
    gMsOadData.versionEntry.magic1 = MS_OAD_JOURNAL_MAGIC;
    gMsOadData.versionEntry.magic2 = MS_OAD_JOURNAL_MAGIC;

    // Reset journal tracking — first state entry starts right after version entry
    gMsOadData.journalNextOffset = MS_OAD_VERSION_ENTRY_SIZE;
    gMsOadData.currentBitmap = MS_OAD_BITMAP_ERASED;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_WriteVersionEntry(): committed=%d.%d.%d.%d",
               pCommitted->iv_major, pCommitted->iv_minor, pCommitted->iv_revision, pCommitted->iv_build_num);

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_WriteVersionEntry(): pending=%d.%d.%d.%d",
               pPending->iv_major, pPending->iv_minor, pPending->iv_revision, pPending->iv_build_num);

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_AppendStateEntry
 *
 * @brief   Append a state entry at the next free journal slot.
 *          Writes bitmap first, then magic last for power-loss safety.
 *          Updates gMsOadData.currentBitmap and journalNextOffset on success.
 *
 *          If the journal is full, returns FAILURE.
 *
 * @param   bitmap - cumulative bitmap for the new entry
 *
 * @return  SUCCESS if entry written or last valid entry already set to the
 *          given bitmap, FAILURE otherwise
 */
static bStatus_t msOad_AppendStateEntry(uint32_t bitmap)
{
    uint32_t offset = gMsOadData.journalNextOffset;

    if (bitmap == gMsOadData.currentBitmap)
    {
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_AppendStateEntry(): Bitmap unchanged, skipping write");
        return SUCCESS;
    }

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_AppendStateEntry(): bitmap=0x%08lx at offset=0x%lx",
               (unsigned long)bitmap, (unsigned long)offset);

    // Check if journal is full
    if (offset + MS_OAD_STATE_ENTRY_SIZE > MS_OAD_JOURNAL_PAGE_SIZE)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_AppendStateEntry(): Journal full!");
        return FAILURE;
    }

    // Write magic1 FIRST — start marker
    if (msOad_NvsWriteWord(offset, MS_OAD_JOURNAL_MAGIC) != SUCCESS) return FAILURE;

    // =========== CRITICAL POINT N ============

    // Write bitmap word
    if (msOad_NvsWriteWord(offset + sizeof(uint32_t), bitmap) != SUCCESS) return FAILURE;

    // =========== CRITICAL POINT O ============

    // Write magic2 LAST — end marker, validates the entry
    if (msOad_NvsWriteWord(offset + 2 * sizeof(uint32_t), MS_OAD_JOURNAL_MAGIC) != SUCCESS) return FAILURE;

    // Update tracking on success
    gMsOadData.currentBitmap = bitmap;
    gMsOadData.journalNextOffset = offset + MS_OAD_STATE_ENTRY_SIZE;

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_ScanJournal
 *
 * @brief   Scan the entire journal at boot to recover state.
 *
 *          1. Read and validate the version entry at offset 0.
 *             If invalid magic → page is considered erased/invalid.
 *          2. Scan ALL state entry slots from offset 20 to end of page.
 *             For each slot: if magic is valid, remember it.
 *             Continue scanning all slots — do NOT stop at first invalid.
 *             The last valid entry found is the ground truth.
 *          3. Populate gMsOadData with cached version, bitmap, and next offset.
 *
 * @return  SUCCESS if scan completed (even if page is erased), FAILURE on error
 */
static bStatus_t msOad_ScanJournal(void)
{
    NVS_Attrs regionAttrs;
    const uint8_t *pFlash;
    const msOadVersionEntry_t *pVersion;
    const msOadStateEntry_t *pState;
    uint32_t offset;
    uint32_t lastValidOffset = 0;
    bool foundValidState = false;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal() called");

    if (gMsOadData.msOadNvsHandle == NULL)
    {
        return FAILURE;
    }

    NVS_getAttrs(gMsOadData.msOadNvsHandle, &regionAttrs);
    pFlash = (const uint8_t *)regionAttrs.regionBase;

    // Step 1: Validate version entry at offset 0
    // Use memcpy for safe access to packed struct from flash
    msOadVersionEntry_t versionEntryTemp;
    memcpy(&versionEntryTemp, pFlash, sizeof(msOadVersionEntry_t));
    pVersion = &versionEntryTemp;

    if (!MS_OAD_IS_ENTRY_VALID(*pVersion))
    {
        // Version entry invalid — page is erased or corrupted
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal(): Version entry magic invalid (magic1=0x%08lx, magic2=0x%08lx), treating as erased",
                   (unsigned long)pVersion->magic1, (unsigned long)pVersion->magic2);

        gMsOadData.versionEntry.magic1 = 0;
        gMsOadData.versionEntry.magic2 = 0;
        gMsOadData.versionEntry.committedVersion = ((struct image_version){0, 0, 0, 0});
        gMsOadData.versionEntry.pendingVersion = ((struct image_version){0, 0, 0, 0});
        gMsOadData.currentBitmap = MS_OAD_BITMAP_ERASED;
        gMsOadData.journalNextOffset = MS_OAD_VERSION_ENTRY_SIZE;
        return SUCCESS;
    }

    // Version entry is valid — cache it
    gMsOadData.versionEntry = *pVersion;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal(): Version entry valid: committed=%d.%d.%d.%d",
               pVersion->committedVersion.iv_major, pVersion->committedVersion.iv_minor,
               pVersion->committedVersion.iv_revision, pVersion->committedVersion.iv_build_num);

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal(): Version entry valid: pending=%d.%d.%d.%d",
                pVersion->pendingVersion.iv_major, pVersion->pendingVersion.iv_minor,
                pVersion->pendingVersion.iv_revision, pVersion->pendingVersion.iv_build_num);

    // Step 2: Scan ALL state entry slots
    // We need to find:
    // 1. The last valid entry (for the current bitmap)
    // 2. The first FULLY ERASED entry (all 0xFF - where we can write next)
    // Note: Corrupted entries (not valid, not fully erased) cannot be overwritten
    msOadStateEntry_t stateEntryTemp;
    uint32_t firstErasedOffset = 0;
    bool foundFirstErased = false;

    for (offset = MS_OAD_VERSION_ENTRY_SIZE;
         offset + MS_OAD_STATE_ENTRY_SIZE <= MS_OAD_JOURNAL_PAGE_SIZE;
         offset += MS_OAD_STATE_ENTRY_SIZE)
    {
        // Use memcpy for safe access to packed struct from flash
        memcpy(&stateEntryTemp, pFlash + offset, sizeof(msOadStateEntry_t));
        pState = &stateEntryTemp;

        if (MS_OAD_IS_ENTRY_VALID(*pState))
        {
            // Valid entry — remember it (overwrite previous)
            foundValidState = true;
            lastValidOffset = offset;
        }
        else if (!foundFirstErased)
        {
            // Check if this entry is fully erased (all 0xFF)
            // We can only write to erased entries, not corrupted ones
            if (MS_OAD_IS_STATE_ENTRY_ERASED(*pState))
            {
                // This is the first fully erased entry — we can write here
                firstErasedOffset = offset;
                foundFirstErased = true;
            }
            // If not erased, it's corrupted — skip it (we can't overwrite without erasing page)
        }

        // Uncomment in order to print the journal
        // Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal(): [offset=0x%lx]", (unsigned long)offset);
        // Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal():         magic1=0x%08lx", (unsigned long)pState->magic1);
        // Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal():         bitmap=0x%08lx", (unsigned long)pState->bitmap);
        // Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal():         magic2=0x%08lx", (unsigned long)pState->magic2);
    }

    // Step 3: Populate journal tracking
    if (foundValidState)
    {
        // Use memcpy for safe access to packed struct from flash
        memcpy(&stateEntryTemp, pFlash + lastValidOffset, sizeof(msOadStateEntry_t));
        pState = &stateEntryTemp;
        gMsOadData.currentBitmap = pState->bitmap;

        // Set journalNextOffset to first erased entry
        if (foundFirstErased)
        {
            gMsOadData.journalNextOffset = firstErasedOffset;
        }
        else
        {
            // No erased entries found — journal is full or corrupted
            // Point to end of scanned area (diagnostic function will handle this)
            gMsOadData.journalNextOffset = offset;
        }

        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal(): Last valid state at offset 0x%lx, bitmap=0x%08lx",
                   (unsigned long)lastValidOffset, (unsigned long)pState->bitmap);
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal(): Next write offset=0x%lx",
                   (unsigned long)gMsOadData.journalNextOffset);
    }
    else
    {
        // No valid state entries — treat as freshly initialized (version written but no state yet)
        gMsOadData.currentBitmap = MS_OAD_BITMAP_ERASED;
        gMsOadData.journalNextOffset = MS_OAD_VERSION_ENTRY_SIZE;

        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_ScanJournal(): No valid state entries found");
    }

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_DiagnoseJournal
 *
 * @brief   Diagnose the journal state after scanning and take corrective
 *          action if needed. This function analyzes the scan results to
 *          determine journal health and whether OAD is in progress.
 *
 *          Diagnosis cases:
 *          1. No version entry → Fresh/erased journal, OAD not started (OK)
 *          2. Version valid, no states → OAD interrupted before first state
 *             entry written. Erase journal to recover to IDLE.
 *          3. Version valid, states exist, space available → Normal (OK)
 *          4. Version valid, states exist, NO space → Journal full/corrupted.
 *             Erase and rewrite version + current state to continue.
 *
 * @return  None (takes corrective action directly)
 */
static void msOad_DiagnoseJournal(void)
{
    bool versionValid = MS_OAD_IS_ENTRY_VALID(gMsOadData.versionEntry);
    bool stateEntriesExist = (gMsOadData.currentBitmap != MS_OAD_BITMAP_ERASED);
    bool spaceAvailable = (gMsOadData.journalNextOffset + MS_OAD_STATE_ENTRY_SIZE <= MS_OAD_JOURNAL_PAGE_SIZE);

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_DiagnoseJournal(): versionValid=%d, stateEntriesExist=%d, spaceAvailable=%d",
               versionValid, stateEntriesExist, spaceAvailable);

    if (!versionValid)
    {
        // Case 1: Fresh/erased journal - no version entry
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_DiagnoseJournal(): Journal is fresh/erased - OAD not started");
        // No action needed - boot resolution will handle IDLE state
    }
    else if (!stateEntriesExist)
    {
        // Case 2: Version entry exists but no state entries
        // OAD was initiated (CRITICAL POINT A passed) but interrupted before first state entry (CRITICAL POINT B)
        // This is an incomplete initialization - erase journal to recover to IDLE
        Log_printf(LogModule_OAD_MS, Log_WARNING, "MS_OAD: msOad_DiagnoseJournal(): Incomplete OAD initialization detected");
        Log_printf(LogModule_OAD_MS, Log_WARNING, "MS_OAD: msOad_DiagnoseJournal(): Version entry exists but no state entries - erasing journal");

        // Erase journal to recover to clean state (IDLE)
        if(msOad_EraseJournal() != SUCCESS)
        {
            // Reboot on failure to erase, as we cannot recover without a clean journal
            msOad_RebootDevice();
        }
    }
    else if (spaceAvailable)
    {
        // Case 3: Normal operation - version and states exist, space available
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_DiagnoseJournal(): Journal healthy - OAD in progress with space available");
        // No action needed - normal operation
    }
    else
    {
        // Case 4: CRITICAL ERROR - journal full or corrupted (no erased entries available)
        // This should NEVER happen in normal operation (would need 168+ state entries).
        // Recovery: Erase journal and rewrite version + current state to preserve MS-OAD progress
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_DiagnoseJournal(): CRITICAL ERROR - Journal full or corrupted!");
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_DiagnoseJournal(): journalNextOffset=0x%lx (max=0x%lx)",
                   (unsigned long)gMsOadData.journalNextOffset, (unsigned long)MS_OAD_JOURNAL_PAGE_SIZE);

        // Save current state before erasing
        struct image_version committedVersion = gMsOadData.versionEntry.committedVersion;
        struct image_version pendingVersion = gMsOadData.versionEntry.pendingVersion;
        uint32_t currentBitmap = gMsOadData.currentBitmap;

        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_DiagnoseJournal(): Erasing journal and rewriting state to recover");

        // Erase journal
        if (msOad_EraseJournal() != SUCCESS)
        {
            Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_DiagnoseJournal(): Failed to erase journal!");

            // Reboot on failure to erase, as we cannot recover with a corrupted journal
            msOad_RebootDevice();
            return;
        }

        // Rewrite version entry
        if (msOad_WriteVersionEntry(&committedVersion, &pendingVersion) != SUCCESS)
        {
            Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_DiagnoseJournal(): Failed to rewrite version entry!");
            return;
        }

        // Rewrite current state entry
        if (msOad_AppendStateEntry(currentBitmap) != SUCCESS)
        {
            Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_DiagnoseJournal(): Failed to rewrite state entry!");
            return;
        }

        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_DiagnoseJournal(): Journal recovery successful - MS-OAD can continue");
    }
}

/*********************************************************************
 * @fn      msOad_GetTrailer
 *
 * @brief   Read the MCUboot image trailer from the primary slot.
 *          The trailer contains swap_type, copy_done, and image_ok flags.
 *
 * @param   pTrailer - pointer to store the trailer data
 *
 * @return  SUCCESS if read succeeded, FAILURE otherwise
 */
static bStatus_t msOad_GetTrailer(struct image_trailer *pTrailer)
{
    NVS_Handle nvsHandle;
    NVS_Attrs regionAttrs;

    if (pTrailer == NULL)
    {
        return FAILURE;
    }

    // Open NV for primary image trailer to get region attributes
    nvsHandle = NVS_open(CONFIG_NV_PRIMARY_IMAGE_TRAILER, NULL);
    if (nvsHandle == NULL)
    {
        return FAILURE;
    }

    // Get region attributes to determine base address and size
    NVS_getAttrs(nvsHandle, &regionAttrs);

    // Close NV handle - no longer needed for direct memory read
    NVS_close(nvsHandle);

    // Trailer is located at the end of the NV region
    const uint32_t trailerAddr = (uint32_t)regionAttrs.regionBase + regionAttrs.regionSize - sizeof(struct image_trailer);

    // Read trailer directly from flash memory
    memcpy(pTrailer, (void *)trailerAddr, sizeof(struct image_trailer));

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_ReadImageHeader
 *
 * @brief   Read an image header from flash at the specified address.
 *
 *          Uses memcpy for safe access to avoid alignment issues
 *          with the packed struct in flash memory.
 *
 * @param   hdrAddr - Flash address of the image header
 * @param   pHeader - Pointer to store the header data
 *
 * @return  None
 */
static void msOad_ReadImageHeader(uint32_t hdrAddr, struct image_header *pHeader)
{
    if (pHeader == NULL)
    {
        return;
    }

    // Read directly from flash using memcpy to avoid alignment issues with packed struct
    memcpy(pHeader, (const void *)hdrAddr, sizeof(struct image_header));
}

/*********************************************************************
 * @fn      msOad_GetSlotVersion
 *
 * @brief   Get the version of an image slot.
 *
 *          Reads the image version from flash memory at the specified
 *          header address. Uses memcpy for safe access to avoid
 *          alignment issues with the packed struct.
 *
 * @param   hdrAddr - Flash address of the image header
 * @param   pVersion - Pointer to store the version
 *
 * @return  None
 */
static void msOad_GetSlotVersion(uint32_t hdrAddr, struct image_version *pVersion)
{
    // Get pointer to version in flash and copy safely using memcpy to avoid alignment issues with packed struct
    const void *imgVer = SwUpdate_GetSWVersion(hdrAddr);
    memcpy(pVersion, imgVer, sizeof(struct image_version));
}

/*********************************************************************
 * @fn      msOad_CompareVersions
 *
 * @brief   Compare two image versions
 *
 * @param   v1 - first version
 * @param   v2 - second version
 *
 * @return  true if versions are equal, false otherwise
 */
static bool msOad_CompareVersions(const struct image_version *v1, const struct image_version *v2)
{
    return (v1->iv_major == v2->iv_major &&
            v1->iv_minor == v2->iv_minor &&
            v1->iv_revision == v2->iv_revision &&
            v1->iv_build_num == v2->iv_build_num);
}

/*********************************************************************
 * @fn      msOad_IsImageRevoked
 *
 * @brief   Check if slot is revoked (All header bytes equals to 0xFF)
 *
 * @param   pVersion - pointer to the version to check
 *
 * @return  true if slot is revoked, false otherwise
 */
static bool msOad_IsImageRevoked(const struct image_header *pHeader)
{
    if (pHeader == NULL)
    {
        return false;
    }

    if (pHeader->ih_magic == MS_OAD_HEADER_MAGIC_REVOKED)
    {
        return true;
    }

    return false;
}

/*********************************************************************
 * @fn      msOad_IsSecondaryImageRevoked
 *
 * @brief   Check if the secondary slot image is revoked.
 *
 *          Reads the secondary slot (slot 2) header and checks if
 *          the image has been marked as revoked (all 0xFF).
 *
 * @return  true if secondary image is revoked, false otherwise
 */
static bool msOad_IsSecondaryImageRevoked()
{
    struct image_header secondSlotHeader;
    msOad_ReadImageHeader(MS_OAD_SLOT_2_HDR_ADDR, &secondSlotHeader);
    return msOad_IsImageRevoked(&secondSlotHeader);
}

/*********************************************************************
 * @fn      msOad_IsSlotBootable
 *
 * @brief   Check if slot is bootable based on image flags
 *
 * @param   slotHeaderFlags - image header flags of the slot
 *
 * @return  true if slot is bootable
 */
static bool msOad_IsSlotBootable(uint32_t slotHeaderFlags)
{
    return ((slotHeaderFlags & IMAGE_F_NON_BOOTABLE) == 0);
}

/*********************************************************************
 * @fn      msOad_IsVersionValidAndBootable
 *
 * @brief   Check if image header represents a valid bootable image
 *          that matches the reference version.
 *          Returns false if the image is revoked.
 *
 * @param   pHeader - pointer to image header to check
 * @param   pVersionReference - reference version to compare against
 *
 * @return  true if image is valid, bootable, and matches reference version
 */
static bool msOad_IsVersionValidAndBootable(const struct image_header *pHeader,
                                            const struct image_version *pVersionReference)
{
    if (pHeader == NULL || pVersionReference == NULL)
    {
        return false;
    }

    // Check if image is revoked
    if (msOad_IsImageRevoked(pHeader))
    {
        return false;
    }

    return (msOad_CompareVersions(&pHeader->ih_ver, pVersionReference) &&
            msOad_IsSlotBootable(pHeader->ih_flags));
}

/*********************************************************************
 * @fn      msOad_IsVersionValidAndNonBootable
 *
 * @brief   Check if image header represents a valid non-bootable image
 *          that matches the reference version.
 *          Returns false if the image is revoked.
 *
 * @param   pHeader - pointer to image header to check
 * @param   pVersionReference - reference version to compare against
 *
 * @return  true if image is valid, non-bootable, and matches reference version
 */
static bool msOad_IsVersionValidAndNonBootable(const struct image_header *pHeader,
                                               const struct image_version *pVersionReference)
{
    if (pHeader == NULL || pVersionReference == NULL)
    {
        return false;
    }

    // Check if image is revoked
    if (msOad_IsImageRevoked(pHeader))
    {
        return false;
    }

    return (msOad_CompareVersions(&pHeader->ih_ver, pVersionReference) &&
            !msOad_IsSlotBootable(pHeader->ih_flags));
}

/*********************************************************************
 * @fn      msOad_SetState
 *
 * @brief   Transition the MS-OAD state machine to a new state
 *
 * @param   newState - the target state to transition to
 *
 * @return  None
 */
static void msOad_SetState(msOadState_e newState)
{
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_SetState() called");

    // Check that the given state is valid
    if (newState >= MS_OAD_NUM_STATES)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_SetState(): Invalid state %d", newState);
        return;
    }

    switch(newState)
    {
        case MS_OAD_UNINITIALIZED:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: State ----> UNINITIALIZED");
            break;
        case MS_OAD_IDLE:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: State ----> IDLE");
            break;
        case MS_OAD_WAIT_FIRST_HALF:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: State ----> WAIT_FIRST_HALF");
            break;
        case MS_OAD_WAIT_SECOND_HALF:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: State ----> WAIT_SECOND_HALF");
            break;
        case MS_OAD_WAIT_FIRST_HALF_REVERT:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: State ----> WAIT_FIRST_HALF_REVERT");
            break;
        case MS_OAD_WAIT_SECOND_HALF_REVERT:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: State ----> WAIT_SECOND_HALF_REVERT");
            break;
        case MS_OAD_NEW_IMAGE_READY:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: State ----> NEW_IMAGE_READY");
            break;
        case MS_OAD_IMAGE_READY_REVERT:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: State ----> IMAGE_READY_REVERT");
            break;
        case MS_OAD_WAIT_IMAGE_ENABLE:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: State ----> WAIT_IMAGE_ENABLE");
            break;
        case MS_OAD_DOWNLOADING:
            Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: State ----> DOWNLOADING");
            break;
        default:
            // Should not reach here
            break;
    }

    // Call the NEW state's onEntry callback if defined (before updating currentState)
    if (statesCallbacks[newState].onEntry != NULL)
    {
        statesCallbacks[newState].onEntry(NULL);
    }

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS OAD: msOad_SetState(): state change to %d", newState);
    gMsOadData.currentState = newState;
    gMsOadData.stateCallbacks = statesCallbacks[newState];
}

/*********************************************************************
 * @fn      msOad_RebootDevice
 *
 * @brief   Trigger a system reset to allow MCUboot to process
 *          the new image configuration
 *
 * @return  None (does not return)
 */
static void msOad_RebootDevice(void)
{
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_RebootDevice() called - rebooting system");

    // Use system reset to reboot - MCUboot will process slot changes
    SystemReset();
}

/*********************************************************************
 * @fn      msOad_SetImageOk
 *
 * @brief   Set the image_ok flag in the MCUboot trailer of the
 *          primary slot. This confirms the current image to MCUboot
 *          so it won't revert on next boot.
 *
 * @return  SUCCESS if write succeeded, FAILURE otherwise
 */
static bStatus_t msOad_SetImageOk(void)
{
    NVS_Handle nvsHandle;
    NVS_Attrs regionAttrs;
    uint8_t imageOkValue = MS_OAD_IMG_OK;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_SetImageOk() called");

    nvsHandle = NVS_open(CONFIG_NV_PRIMARY_IMAGE_TRAILER, NULL);
    if (nvsHandle == NULL)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_SetImageOk(): NVS_open() FAILED");
        return FAILURE;
    }

    NVS_getAttrs(nvsHandle, &regionAttrs);

    const uint32_t trailerOffset = regionAttrs.regionSize - sizeof(struct image_trailer);
    const uint32_t imageOkOffset = trailerOffset + offsetof(struct image_trailer, image_ok);

    if (NVS_write(nvsHandle, imageOkOffset, &imageOkValue, sizeof(imageOkValue),
                  NVS_WRITE_POST_VERIFY) != NVS_STATUS_SUCCESS)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_SetImageOk(): NVS_write() FAILED");
        NVS_close(nvsHandle);
        return FAILURE;
    }

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_SetImageOk(): image_ok set successfully");
    NVS_close(nvsHandle);
    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_IsImageOkSet
 *
 * @brief   Check if the image_ok flag is set in the MCUboot trailer.
 *
 * @return  true if image_ok == MS_OAD_IMG_OK, false otherwise
 */
static bool msOad_IsImageOkSet(void)
{
    struct image_trailer trailer;

    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_IsImageOkSet() called");

    if (msOad_GetTrailer(&trailer) != SUCCESS)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_IsImageOkSet(): msOad_GetTrailer() FAILED");
        return false;
    }

    bool result = (trailer.image_ok == MS_OAD_IMG_OK);
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_IsImageOkSet(): image_ok=0x%02x, result=%d", trailer.image_ok, result);

    return result;
}

/*********************************************************************
 * @fn      msOad_RevokeSecondaryImageIfRequired
 *
 * @brief   Revoke the secondary slot image if not already revoked.
 *
 *          This function checks if the secondary image is already
 *          revoked (all 0xFF in header). If not revoked, it calls
 *          the SwUpdate module to revoke the image by erasing its
 *          header.
 *
 * @return  SUCCESS if image was revoked or already revoked, FAILURE otherwise
 */
static bStatus_t msOad_RevokeSecondaryImageIfRequired(void)
{
    if (!msOad_IsSecondaryImageRevoked())
    {
        Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_RevokeSecondaryImageIfRequired(): Revoke secondary slot image");

        if (SwUpdate_RevokeSecondaryImage() != SUCCESS)
        {
            Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_RevokeSecondaryImageIfRequired(): SwUpdate_RevokeSecondaryImage() FAILED");
            return FAILURE;
        }
    }

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_EnterRevertMode
 *
 * @brief   Set the revert flag in flash data to indicate that
 *          the MS-OAD process should revert to the original image.
 *          This is called when download fails and revert is needed.
 *
 *          Also revokes the second slot.
 *
 * @return  None
 */
static bStatus_t msOad_EnterRevertMode()
{
    Log_printf(LogModule_OAD_MS, Log_INFO, "MS_OAD: msOad_EnterRevertMode() Called");

    // Set revert flag via journal append if not already set
    if (msOad_AppendStateEntry(MS_OAD_BITMAP_REVERT) != SUCCESS)
    {
        Log_printf(LogModule_OAD_MS, Log_ERROR, "MS_OAD: msOad_EnterRevertMode(): msOad_AppendStateEntry() FAILED");
        return FAILURE;
    }

    return SUCCESS;
}

/*********************************************************************
 * @fn      msOad_SetImageIdentified
 *
 * @brief   Set the image identified flag in OAD profile data.
 *
 *          This flag indicates whether the incoming image header
 *          has been validated and download can proceed. Set to true
 *          after successful image header validation.
 *
 * @param   identified - true if image identified and valid, false otherwise
 *
 * @return  None
 */
static void msOad_SetImageIdentified(bool identified)
{
    gMsOadData.oadProfileData.isImageIdentified = identified;
}

/*********************************************************************
 * @fn      msOad_ClearProfileData
 *
 * @brief   Clear OAD profile data flags.
 *
 *          Resets the image identified flag to prepare for a new
 *          OAD session. Called when entering wait states.
 *
 * @return  None
 */
static void msOad_ClearProfileData(void)
{
    gMsOadData.oadProfileData.isImageIdentified = false;
}

#endif //MS_OAD