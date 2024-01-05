/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*! ============================================================================
 *  @file       I2SLPF3.h
 *
 *  @brief      I2S driver implementation for LPF3 devices
 *
 * # Limitations #
 *
 * ## Supported #I2S_MemoryLength values ##
 *  Only the following memory lengths are supported:
 *   - #I2S_MEMORY_LENGTH_16BITS
 *   - #I2S_MEMORY_LENGTH_32BITS
 *
 * ## #I2S_Params Attributes Limitations ##
 *
 * Some attributes in the #I2S_Params structure have a limited set of supported
 * values. These limitations are described below:
 *  - #I2S_Params.isDMAUnused
 *    - Must be false. All transmissions are always performed by the I2S module's
 *      own DMA.
 *  - #I2S_Params.isMSBFirst
 *    - Must be true. All samples are always transmitted MSB first.
 *  - #I2S_Params.memorySlotLength
 *    - Must be one of the suported #I2S_MemoryLength values listed in the above
 *      section.
 *  - #I2S_Params.fixedBufferLength
 *    - Additional restriction: Must be an even multiple of the number of bytes
 *      in a frame.
 *
 * <hr>
 * # Sample Buffers #
 *
 * This section describes the structure and requirements for the sample buffers
 * used in the #I2S_Transaction objects.
 *
 * Sample words are read from or written to the sample buffers in little-endian
 * byte order, meaning that the least significant byte (LSByte) is stored at the
 * lower byte address, and the most significant byte (MSByte) is stored at the
 * higher byte address.
 *
 * The sample buffers are divided into frames which are further subdivided into
 * channels, and if a channel is used by both SD0 and SD1 (where the direction
 * of the two pins are the same), then that channel is further subdivided into a
 * sample word for first SD0 and then SD1.
 *
 * ## Buffer Size Requirements ##
 *
 * The size of the buffers used in #I2S_Transaction objects must be an even
 * multiple of the number of bytes per frame. I.e. the number of bytes in the
 * buffers must be of the form: 2*n*k, where k is the size of a frame in bytes
 * and n is an integer satisfying n>=2. 2*n is the number of frames in the
 * buffer.
 *
 * ## General Sample Buffer Structure ##
 *
 * Below code describes the general structure of a sample buffer if SD0 and SD1
 * are configured to the same direction.
 *
 * @code
 * struct
 * {
 * #if SD0_USE_CHANNEL_0 || SD1_USE_CHANNEL_0
 *     struct
 *     {
 *   #if SD0_USE_CHANNEL_0
 *         uint8_t sd0SampleWord[BYTES_PER_WORD];
 *   #endif
 *   #if SD1_USE_CHANNEL_0
 *         uint8_t sd1SampleWord[BYTES_PER_WORD];
 *   #endif
 *     } channel0;
 * #endif
 * #if SD0_USE_CHANNEL_1 || SD1_USE_CHANNEL_1
 *     struct
 *     {
 *   #if SD0_USE_CHANNEL_1
 *         uint8_t sd0SampleWord[BYTES_PER_WORD];
 *   #endif
 *   #if SD1_USE_CHANNEL_1
 *         uint8_t sd1SampleWord[BYTES_PER_WORD];
 *   #endif
 *     } channel1;
 * #endif
 * // ...
 * #if SD0_USE_CHANNEL_8 || SD1_USE_CHANNEL_8
 *     struct
 *     {
 *   #if SD0_USE_CHANNEL_8
 *         uint8_t sd0SampleWord[BYTES_PER_WORD];
 *   #endif
 *   #if SD1_USE_CHANNEL_8
 *         uint8_t sd1SampleWord[BYTES_PER_WORD];
 *   #endif
 *     } channel8;
 * #endif
 * } sampleBufferFrames[FRAMES_PER_BUFFER];
 * @endcode
 *
 * Notes:
 *  - \c SD0_USE_CHANNEL_n should be true if SD0 uses channel n, otherwise false.
 *  - \c SD1_USE_CHANNEL_n should be true if SD1 uses channel n, otherwise false.
 *  - \c BYTES_PER_WORD is based on the configured memory length:
 *    - #I2S_MEMORY_LENGTH_16BITS: 2
 *    - #I2S_MEMORY_LENGTH_32BITS: 4
 *  - \c FRAMES_PER_BUFFER must be divisible by 2
 *  - \c sampleBufferFrames needs to be cast to an \c uint8_t pointer to be used
 *    with the I2S driver.
 *
 * If SD0 and SD1 are not configured to the same direction (or only one is used)
 * then the structure can be simplified as below:
 * @code
 * struct
 * {
 * #if USE_CHANNEL_0
 *     uint8_t channel0SampleWord[BYTES_PER_WORD];
 * #endif
 * #if USE_CHANNEL_1
 *     uint8_t channel1SampleWord[BYTES_PER_WORD];
 * #endif
 * // ...
 * #if USE_CHANNEL_8
 *     uint8_t channel8SampleWord[BYTES_PER_WORD];
 * #endif
 * } sampleBufferFrames[FRAMES_PER_BUFFER];
 * @endcode
 *
 * Notes:
 *  - \c USE_CHANNEL_n should be true if channel n is used, otherwise false.
 *
 *
 * ### Sample Buffer Structure Example ###
 *
 * If for example SD0 and SD1 are configured to the same direction and if
 * channel 0 and 1 are used for SD0 and channel 0 is used for SD1, then the
 * sample buffer would be structured as in the code below.
 *
 * @code
 * struct
 * {
 *     struct
 *     {
 *         uint8_t sd0SampleWord[BYTES_PER_WORD];
 *         uint8_t sd1SampleWord[BYTES_PER_WORD];
 *     } channel0;
 *     struct
 *     {
 *         uint8_t sd0SampleWord[BYTES_PER_WORD];
 *     } channel1;
 * } sampleBufferFrames[FRAMES_PER_BUFFER];
 *
 *  // Access LSB of sample 10 of channel 0 on SD1
 *  uint8_t tmp = sampleBufferFrames[10].channel0.sd1SampleWord[0];
 *
 * @endcode
 *
 * <hr>
 * @anchor ti_drivers_i2s_I2SLPF3_AFCLK
 * # Selecting the Audio Frequency Clock #
 *
 * Using #I2SLPF3_HWAttrs.afclkSrc it is possible to select the audio frequency
 * clock (AFCLK) source. AFCLK is used by the I2S hardware module when the
 * I2S driver is opened in Controller mode
 * (#I2S_Params.moduleRole == I2S_CONTROLLER)
 * The I2S hardware module will divide the AFCLK to generate the I2S clocks
 * (BCLK (SCK), WCLK (Ws) and CCLK). The AFCLK therefore directly determines
 * the possible sample rates, i.e. the frequency of WCLK.
 *
 * To achieve a desired sample frequency (f_s), the following requirements must
 * be met, where f_afclk is the frequency of the AFCLK:
 *  - f_afclk/f_s must be an integer. I.e. f_afclk = n * f_s, where n is an
 *    integer.
 *  - The total number of bits per frame must evenly divide into f_afclk/f_s
 *
 * For example, if f_s = 44.1 kHz, then the first requirement is met if for
 * example f_afclk = 90.3168 MHz = 2048 * 44.1 kHz. The second requirement is
 * met if for example the number of bits per frame is 32, because it evenly
 * divides into 2048.
 *
 * If the requirements are not met, the closest possible sampling frequency to
 * the desired sampling frequency will be used.
 *
 * To achieve the most common sample frequencies, it is suggested to select
 * AFOSC as the AFCLK source, since its frequency can be configured to be a
 * multiple of the most common sample rates.
 *
 * ## Using AFOSC ##
 *
 * See #PowerLPF3_AfoscFreq for the list of possible frequencies that the AFOSC
 * can be configured to. The frequencies most suited to be used with I2S are
 * listed below:
 *  - #PowerLPF3_AFOSC_FREQ_98P304MHZ:
 *    - This can be used to achieve sample rates of 8 kHz, 16 kHz, 24 kHz,
 *      32 kHz and 48 kHz.
 *       - For 8 kHz, 16 kHz and 32 kHz the number of bits per frame must be of
 *         the form: 2^n * 3^k, where n is an integer and k is either 0 or 1.
 *       - For 24 kHz and 48 kHz the number of bits per frame must be of the
 *         form: 2^n, where n is an integer.
 *  - #PowerLPF3_AFOSC_FREQ_90P3168MHZ:
 *    - This can be used to achieve sample rates of 22.05 kHz and 44.1 kHz. The
 *      number of bits per frame must be of the form: 2^n, where n is an
 *      integer.
 * .
 *
 *  ============================================================================
 */
#ifndef ti_drivers_i2s_I2SLPF3__include
#define ti_drivers_i2s_I2SLPF3__include

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/I2S.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/Power.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_ckmd.h)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief      Audio Frequency Clock Source
 *
 *  Option to select the clock source used by the I2S hardware module when the
 *  I2S driver is opened in Controller mode.
 *
 * \sa @ref ti_drivers_i2s_I2SLPF3_AFCLK "Selecting the Audio Frequency Clock"
 */
typedef enum
{
    I2SLPF3_AFCLK_SRC_CLKREF = CKMD_AFCLKSEL_SRC_CLKREF, /*!< 48MHz reference clock (HFXT) */
    I2SLPF3_AFCLK_SRC_CLKHF  = CKMD_AFCLKSEL_SRC_CLKHF,  /*!< 96MHz CLKHF */
    I2SLPF3_AFCLK_SRC_CLKAF  = CKMD_AFCLKSEL_SRC_CLKAF,  /*!< AFOSC clock */
} I2SLPF3_AfclkSrc;

/*!
 *  @brief  I2S Hardware attributes
 *
 *  intPriority is the I2S peripheral's interrupt priority, as defined by the
 *  RTOS kernel. This value is passed unmodified to HwiP_construct().
 *
 *  A sample structure is shown below:
 *  @code
 *  const I2SLPF3_HWAttrs i2sHWAttrs[CONFIG_TI_DRIVERS_I2S_COUNT] = {
 *      {
 *         .afclkSrc     =  I2SLPF3_AFCLK_SRC_CLKAF,
 *         .afoscFreq    =  PowerLPF3_AFOSC_FREQ_98P304MHZ,
 *         .sd1Pin       =  CONFIG_I2S_ADI,
 *         .sd1PinMux    =  GPIO_MUX_PORTCFG_PFUNC4,
 *         .sd0Pin       =  CONFIG_I2S_ADO,
 *         .sd0PinMux    =  GPIO_MUX_PORTCFG_PFUNC4,
 *         .sckPin       =  CONFIG_I2S_BCLK,
 *         .sckPinMux    =  GPIO_MUX_PORTCFG_PFUNC1,
 *         .cclkPin      =  CONFIG_I2S_CCLK,
 *         .cclkPinMux   =  GPIO_MUX_PORTCFG_PFUNC1,
 *         .wsPin        =  CONFIG_I2S_WCLK,
 *         .wsPinMux     =  GPIO_MUX_PORTCFG_PFUNC1,
 *         .intPriority  =  ~0,
 *      },
 *  };
 *  @endcode
 */
typedef struct
{
    I2SLPF3_AfclkSrc afclkSrc;     /*!< Audio frequency clock (AFCLK) source. */
    PowerLPF3_AfoscFreq afoscFreq; /*!< The AFOSC frequency to use if #afclkSrc is #I2SLPF3_AFCLK_SRC_CLKAF */
    uint_least8_t sd1Pin;          /*!< Pin used for SD1 signal. */
    uint_least8_t sd1PinMux;       /*!< Mux value used to mux the SD1 signal to #sd1Pin. */
    uint_least8_t sd0Pin;          /*!< Pin used for SD0 signal. */
    uint_least8_t sd0PinMux;       /*!< Mux value used to mux the SD0 signal to #sd0Pin. */
    uint_least8_t sckPin;          /*!< Pin used for SCLK signal. */
    uint_least8_t sckPinMux;       /*!< Mux value used to mux the SCLK signal to #sckPin. */
    uint_least8_t cclkPin;         /*!< Pin used for CCLK signal. Not used in most of the applications. */
    uint_least8_t cclkPinMux;      /*!< Mux value used to mux the CCLK signal to #cclkPin. */
    uint_least8_t wsPin;           /*!< Pin used for WS signal. */
    uint_least8_t wsPinMux;        /*!< Mux value used to mux the WS signal to #wsPin. */
    uint8_t intPriority;           /*!< I2S Peripheral's interrupt priority. */
} I2SLPF3_HWAttrs;

/*!
 *  @cond NODOC
 *  I2S data-interface
 *
 *  This enum defines how the physical I2S interface (SD0/SD1) behaves.
 *  Do not modify.
 */
typedef struct
{
    uint8_t numberOfChannelsUsed;         /*!< Number of channels used on SDx. */
    I2S_ChannelConfig channelsUsed;       /*!< List of the used channels. */
    I2S_DataInterfaceUse interfaceConfig; /*!< IN / OUT / UNUSED */
} I2SLPF3_DataInterface;
/*! @endcond */

/*!
 *  @cond NODOC
 *  I2S interface
 *
 *  This enum defines one of the interfaces (READ or WRITE) of the I2S module.
 *  Do not modify.
 */
typedef struct
{
    /*! Size of the memory step to access the following sample */
    uint16_t memoryStep;

    /*! Number of WS cycles to wait before starting the first transfer.
     *  This value is mostly used when performing constant latency transfers.
     */
    uint16_t delay;

    /*! Pointer to callback */
    I2S_Callback callback;

    /*! Pointer to the function used to update PTR-NEXT */
    I2S_RegUpdate pointerSet;

    /*! Pointer to the function used to stop the interface */
    I2S_StopInterface stopInterface;

    /*! Pointer to the ongoing transfer */
    I2S_Transaction *activeTransfer;
} I2SLPF3_Interface;
/*! @endcond */

/*!
 *  @cond NODOC
 *  @brief      The definition of a function used by the I2S driver
 *              to refresh the pointer
 *
 *  @param      I2S_Handle  I2S_Handle
 *
 *  @param      I2SLPF3_Interface *interface Pointer on the interface to update
 *
 */
typedef void (*I2SLPF3_PtrUpdate)(I2S_Handle handle, I2SLPF3_Interface *interface);

/*!
 *  @cond NODOC
 *  I2S Object.  The application must not access any member variables
 *  of this structure!
 */
typedef struct
{
    /*! To avoid multiple openings of the I2S. */
    bool isOpen;

    /*! WS inversion.
     *  - false: The WS signal is not internally inverted.
     *  - true:  The WS signal is internally inverted.
     */
    bool invertWS;

    /*! Select the size of the memory used using DriverLib defines.
     *  The two options are 16 bits and 24 bits. Any value can be selected,
     *  whatever the value of #bitsPerWord.
     *  - I2S_MEM_LENGTH_16: Memory length is 16 bits.
     *  - I2S_MEM_LENGTH_32: Memory length is 32 bits.
     */
    uint32_t memorySlotLength;

    /*! When dataShift is set to 0, data are read/write on the data lines from
     *  the first SCK period of the half WS period to the last SCK edge of the
     *  WS half period. By setting dataShift to a value different from zero, you
     *  can postpone the moment when data are read/write during the WS half
     *  period. For example, by setting dataShift to 1, data are read/write on
     *  the data lines from the second SCK period of the half WS period to the
     *  first SCK edge of the next WS half period. If no padding is activated,
     *  this corresponds to the I2S standard.
     */
    uint8_t dataShift;

    /*! Number of bits per word (must be between 8 and 24 bits). */
    uint8_t bitsPerWord;

    /*! Number of SCK periods between the first WS edge and the MSB of the first
     * audio channel data transferred during the phase.
     */
    uint8_t beforeWordPadding;

    /*! Number of SCK periods between the LSB of the last audio channel data
     *  transferred during the phase and the following WS edge.
     */
    uint8_t afterWordPadding;

    /*! Number of consecutive frames in the samples buffers to be handled during
     *  one DMA transfer. This field must be set to a value x between 2 and 256,
     *  both included. All the data buffers used must contain N*x frames or
     *  N*x*b bytes (with N an integer satisfying N>0, and b
     *  being the number of bytes per frame).
     */
    uint16_t dmaBuffSizeConfig;

    /*! Select edge sampling type. */
    I2S_SamplingEdge samplingEdge;

    /*! Select if the current device is a Target or a Controller. */
    I2S_Role moduleRole;

    /*! Select phase type. */
    I2S_PhaseType phaseType;

    /*! Frequency divider for the CCLK signal. */
    uint16_t CCLKDivider;

    /*! Frequency divider for the SCK signal. */
    uint16_t SCKDivider;

    /*! Frequency divider for the WS signal. */
    uint16_t WSDivider;

    /*! Time (in number of WS cycles) to wait before the first transfer. */
    uint16_t startUpDelay;

    /*! Structure to describe the SD0 interface */
    I2SLPF3_DataInterface dataInterfaceSD0;

    /*! Structure to describe the SD1 interface */
    I2SLPF3_DataInterface dataInterfaceSD1;

    /*
     * I2S SYS/BIOS objects
     */
    /*! Hwi object for interrupts */
    HwiP_Struct hwi;

    /*! Pointer on the function used to update IN and OUT PTR-NEXT */
    I2SLPF3_PtrUpdate ptrUpdateFxn;

    /*! Structure to describe the read (in) interface */
    I2SLPF3_Interface read;

    /*! Structure to describe the write (out) interface */
    I2SLPF3_Interface write;

    /*! Pointer to error callback */
    I2S_Callback errorCallback;

    /*
     * I2S pre and post notification functions
     */
    /*! I2S pre-notification function pointer */
    void *i2sPreFxn;

    /*! I2S post-notification function pointer */
    void *i2sPostFxn;

    /*! I2S pre-notification object */
    Power_NotifyObj i2sPreObj;

    /*! I2S post-notification object */
    Power_NotifyObj i2sPostObj;

    /*! I2S power constraint flag, guard to avoid power constraints getting out
     *  of sync
     */
    volatile bool i2sPowerConstraint;

} I2SLPF3_Object;
/*! @endcond */

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_i2s_I2SLPF3__include */
