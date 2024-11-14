/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * ZBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: Generic Finite State Machine header file.
*/

#ifndef _ZB_FSM_H_
#define _ZB_FSM_H_ 1

#include "zb_types.h"
#include "zb_errors.h"

/**
   @page GW-FSM-LLD GW FSM Low Level Design

   <hr>
   @section GW-FSM-OVERVIEW Overview
Finite State Machine (FSM) is designed to process logically chained events. As a result,
some "logical task" should be completed. Good example of "logical task" is a device
commissioning process. A whole commissioning may be split into "logical tasks" -
actions that should be done for each particular cluster. In this case 1 FSM describes
a number of actions that are done during commissioning for configuring 1 cluster.
In common commissioning process several FSMs should be processed successively. To
describe FSM processing sequence a list of FSM descriptors is used on application level.
Each FSM descriptor is identified by its index in FSM descriptor list, see
zb_fsm_sys_t::fsms_desc.

   <hr>
   @section GW-FSM-IMPL Implementation Details
FSM is described by FSM descriptor - see zb_fsm_desc_t. FSM descriptor defines:
- all possible states and allowed transitions
- FSM main handler
- other handlers (if needed)
FSM state is unambiguously defined by its state zb_fsm_t::fsm_state.
FSM state is changed as a result of handling of incoming event or because of some
internal processing. There are several types of events, see enum zb_fsm_ev_type_e.

To initiate FSM processing external stimulus should arrive. This stimulus is posted
event, see zb_fsm_event_post(). NULL event is allowed.
FSM handler is called by internal function run_fsm(). This function should not
be called by high level application.

run_fsm() calls FSM handler until FSM run state zb_fsm_t::fsm_run is ZB_FSM_RUN.
FSM run state is changed on post event or on event processing. Several successive
handler calls are allowed.

   <hr>
   @section GW-FSM-HIGHLIGHT Design Highlights
To pass event to FSM, event container is used, see zb_fsm_ev_hdr_t. Event container is
get from a pool and must be put to the pool on handling finish. It is not allowed
to store event containers, pass it to any async calls - it must be handled synchronously.
If any async operation is done, all needed data should be saved to some context and
event container should be released.
*/

#define ZB_FSM_DESC_GET_TYPE(_fsm) ((_fsm)->fsm_desc - (_fsm)->fsm_sys->fsms_desc)

/*************** FSM events ****************/

/**
 * @brief FSM event types
 *
 * Possible values of fe_type field in @ref zb_fsm_ev_hdr_t event structure.
 * FSM event types are common for all FSMs.
 */
enum zb_fsm_ev_type_e
{
  ZB_FSM_EV_NONE = 0, /*!< Event is not used */
  ZB_FSM_EV_UNDEF,    /*!< Event is allocated but not initialized properly */
  ZB_FSM_EV_APP       /*!< App starts its specific events IDs with this value */
};


typedef struct zb_fsm_ev_hdr_s
{
  zb_uint8_t fe_type;           /**< Event type, one of the @ref zb_fsm_ev_type_e values  */
}
zb_fsm_ev_hdr_t;

/***************** FSM description ******************/
/**
 * @brief FSM descriptor types
 *
 * FSM descriptor types - per one for each cluster.
 */
typedef enum zb_fsm_desc_type_e
{
  ZB_FSM_DESC_NULL = 0, /**< Reserved descriptor type. Used for
                           initial FSM configuration */
  ZB_FSM_DESC_APP,      /**< Application starts its FSM descriptors
                         * enumeration with this value */

  ZB_FSM_DESC_DONE = (zb_uint8_t)-1, /**< Reserved descriptor type */
} zb_fsm_desc_type_t;

/* Forward declaration */
struct zb_fsm_s;

/**
 * @brief FSM main handler
 *
 * Handles events.
 *
 * @param [in] fsm     Pointer to the FSM entity
 * @param [in] evt     Pointer to the event
 *
 * @return One of the following values: RET_AGAIN, RET_ERROR, RET_BLOCKED.
 * RET_AGAIN: Call handler once more
 * RET_BLOCKED: Wait for an event
 * RET_ERROR: Some error appeared, cancel processing
*/
typedef zb_ret_t(*zb_fsm_hnd_t)(struct zb_fsm_s *fsm, zb_fsm_ev_hdr_t *);

/**
 * @brief FSM configuration
 *
 * Determines allowed transition mask.
 * FSM reserved states are defined in enum zb_fsm_common_state_e.
 * Other states are application defined.
 *
 * @note ZB_FSM_STATE_FINAL should not be described in the conf list.
 */
typedef struct zb_fsm_conf_s
{
  zb_uint32_t fc_allowed; /**< Allowed transitions bitmask */
}
zb_fsm_conf_t;

/**
 * @brief FSM descriptor
 *
 * Contains FSM descriptor configuration array and event handler
 */
typedef struct zb_fsm_desc_s
{
  const zb_fsm_conf_t *fd_conf;      /**< Array of FSM configurations.
                                          Array index MUST be equal to FSM state */
  zb_uint8_t           fd_max_state; /**< FSM state maximum value */
  zb_fsm_hnd_t         fd_hnd;       /**< FSM event handler */
}
zb_fsm_desc_t;


#define __CAT_(a,b,c) a##b##c

/**
 * FSM chain is used to execute FSM descriptors sequentially,
 * one after another. Chain size depends on App requirements.
 * For Simple case - do not use it.
 */
#define ZB_FSM_CHAIN_TYPE_DECLARE(name, chain_size) \
  typedef struct __CAT_(zb_fsm_chain_, name, _s)    \
  {                                                 \
    zb_uint8_t fsc_read_idx;                        \
    zb_uint8_t fsc_chain_size;                      \
    zb_uint8_t fsc_chain[chain_size];               \
  } __CAT_(zb_fsm_chain_, name, _t)

/* declare type zb_fsm_chain_1_t */
ZB_FSM_CHAIN_TYPE_DECLARE(1, 1);


/**
 * Event iterator returns next event from App defined event
 * pool.
 *
 * @note Event contains common header, but different data
 */
typedef zb_fsm_ev_hdr_t*(*zb_fsm_ev_itr_t)(zb_fsm_ev_hdr_t *pool, zb_fsm_ev_hdr_t *fsm_ev);

/**
 * @brief FSM subsystem context
 *
 * Contains FSM descriptor array and event pool
 */
typedef struct zb_fsm_sys_s
{
  const zb_fsm_desc_t  *fsms_desc;        /**< FSM descriptor array */
  zb_fsm_ev_hdr_t      *fsms_ev_pool;     /**< FSM event pool, App specific */
  zb_fsm_ev_itr_t       fsms_ev_iterator; /**< App specific event-pool iterator */
}
zb_fsm_sys_t;

/**
 * @brief FSM running status
 *
 * Determines FSM running status.
 */
enum zb_fsm_run_e
{
  ZB_FSM_WAIT = 0, /**< FSM is waiting an event */
  ZB_FSM_RUN       /**< FSM is running  */
};

typedef void(*zb_fsm_finish_cb_t)(struct zb_fsm_s *fsm);

/**
 * @brief FSM context
 *
 * Keeps information of current FSM state, event and active FSM
 * descriptor.
 */
typedef struct zb_fsm_s
{
  zb_uint8_t           fsm_state;     /**< Current FSM state  */
  zb_uint8_t           fsm_run;       /**< FSM running status @ref zb_fsm_run_e */
  zb_fsm_ev_hdr_t     *fsm_ev;        /**< Posted event */
  const zb_fsm_desc_t *fsm_desc;      /**< Active FSM descriptor */
  zb_fsm_sys_t        *fsm_sys;       /**< FSM subsystem context */
  zb_fsm_chain_1_t    *fsm_chain;     /**< FSM descriptors chain */
  zb_fsm_finish_cb_t   fsm_finish_cb; /**< Callback to be called on FSM finish. */
  zb_ret_t             fsm_ret;       /**< Return code. Can be checked by user. */
}
zb_fsm_t;

/* Set to fit 4byte mask variable */
#define ZB_FSM_MAX_ALLOWED_STATE_NUM 30U

/**
 * FSM reserved states
 */
enum zb_fsm_common_state_e
{
  ZB_FSM_STATE_NONE = 0,
  /**
   * Initial FSM-specific state.
   * Application should enumerate FSM states starting from this value.
   */
  ZB_FSM_STATE_APP_INITIAL,
  /** Virtual state: no configuration defined for it */
  ZB_FSM_STATE_FINAL = 31
};


/********************* FSM API *************************/

/**
 *  @brief Initialize FSM subsystem
 *  To be used in constructions like:
 *  @code
 *  zb_fsm_sys_init(&g_mmt_fsm_sys);
 *  @endcode
 *  @param [in] fsm_sys - pointer to application-defined FSM subsystem.
 *  @return RET_OK if success, error code otherwise.
 */
zb_ret_t zb_fsm_sys_init(zb_fsm_sys_t    *fsm_sys,
                         zb_fsm_desc_t   *desc,
                         zb_fsm_ev_hdr_t *evt_pool,
                         zb_fsm_ev_itr_t  ev_iterator);

/**
 *  @brief Initialize FSM and attach FSM subsystem
 *  To be used in constructions like:
 *  @code
 *  typedef struct mmt_parent_fsm_s
 *  {
 *    zb_fsm_t fsm;
 *    mmt_private_t data;
 *  } mmt_parent_fsm_t;
 *
 *  ...
 *
 *  mmt_parent_fsm_t parent_fsm;
 *
 *  zb_fsm_sys_init(&g_mmt_fsm_sys);
 *  zb_fsm_init(&parent_fsm.fsm, &g_mmt_fsm_sys, NULL);
 *  @endcode
 *  @param fsm - pointer to FSM entity.
 *  @param fsm_sys - pointer to application-defined FSM subsystem.
 *  @param chain - FSM descriptors chain to be executed.
 *  @param finish_cb - Callback to be called when FSM transits to
 *                     ZB_FSM_STATE_FINAL state. Can be NULL.
 *  @return RET_OK if success, error code otherwise.
 */
zb_ret_t zb_fsm_init(zb_fsm_t           *fsm,
                     zb_fsm_sys_t       *fsm_sys,
                     zb_fsm_chain_1_t   *chain,
                     zb_fsm_finish_cb_t  finish_cb);

/**
  Post event to FSM, set FSM run state to ZB_FSM_RUN.
  Event is synchronously handled. NULL event may be posted.
*/

/**
 *  @brief Post event to FSM, set FSM run state to ZB_FSM_RUN.
 *  Event is synchronously handled. NULL event may be posted.
 *
 *  To be used in constructions like:
 *  @code
 *  evt = zb_fsm_event_get(&parent_fsm.fsm.fsm_sys);
 *
 *  evt->fe_type = ZB_FSM_EV_ZB_CMD;
 *  evt->fe_data.fe_zb_cmd.evz_cmd_id = ZB_ZB_CMD_CONFIG_REPORT_RESP;
 *  evt->fe_data.fe_zb_cmd.evz_buf_param = 0x42;
 *  zb_fsm_event_post(&parent_fsm.fsm, evt);
 *  @endcode
 *  @param [in] fsm - pointer to FSM entity.
 *  @param [in] evt - pointer to event. NULL-pointer can be passed.
 *  @return
 *   RET_OK - Ok, event successfully handled
 *   RET_EOF - Ok, event successfully handled and FSM processing is finished
 *   RET_DOES_NOT_EXIST - Error, FSM descriptor is not set
 *   RET_INVALID_PARAMETER
*/
zb_ret_t zb_fsm_event_post(zb_fsm_t *fsm, zb_fsm_ev_hdr_t *evt);

/**
 *  @brief Returns free event container from FSM sys event pool
 *
 *  To be used in constructions like:
 *  @code
 *  evt = zb_fsm_event_get(&parent_fsm.fsm.fsm_sys);
 *
 *  evt->fe_type = ZB_FSM_EV_ZB_CMD;
 *  evt->fe_data.fe_zb_cmd.evz_cmd_id = GW_ZB_CMD_CONFIG_REPORT_RESP;
 *  evt->fe_data.fe_zb_cmd.evz_buf_param = 0x42;
 *  zb_fsm_event_post(&parent_fsm.fsm, evt);
 *  @endcode
 *  @param [in] fsm - pointer to FSM entity.
 *  @return Pointer to event to be used.
 */
zb_fsm_ev_hdr_t* zb_fsm_event_get(zb_fsm_t *fsm);

/**
 *  @brief Put event container back to FSM sys event pool.
 *
 *  To be used only from FSM handler:
 *  @code
 *  zb_ret_t zb_dev_join_hnd(zb_fsm_t *fsm, zb_fsm_ev_hdr_t *evt)
 *  {
 *    zb_ret_t ret = RET_OK;
 *
 *    switch (fsm->fsm_state)
 *    {
 *      case STATE1:
 *        process_event(evt);
 *        zb_fsm_event_put(fsm, evt);
 *        zb_fsm_state_set(fsm, ZB_FSM_DEV_JOIN_STATE_STEP_4);
 *        break;
 *  @endcode
 *  @param [in] evt - pointer to event entity.
 *  @return RET_OK if success, error code otherwise.
 */
zb_ret_t zb_fsm_event_put(zb_fsm_t *fsm, zb_fsm_ev_hdr_t *evt);

/**
 *  @brief Set new FSM active state.
 *
 *  To be used only from FSM handler:
 *  @code
 *  zb_ret_t zb_dev_join_hnd(zb_fsm_t *fsm, zb_fsm_ev_hdr_t *evt)
 *  {
 *    zb_ret_t ret = RET_OK;
 *
 *    switch (fsm->fsm_state)
 *    {
 *      case STATE1:
 *        process_event(evt);
 *        zb_fsm_event_put(fsm, evt);
 *        zb_fsm_state_set(fsm, ZB_FSM_DEV_JOIN_STATE_STEP_4);
 *        break;
 *  @endcode
 *  @param [in] fsm - pointer to FSM context.
 *  @param [in] new_state - New state to be set.
 */
void zb_fsm_state_set(zb_fsm_t *fsm, zb_uint8_t new_state);


#define ZB_FSM_FAILURE(fsm, error_code)                         \
zb_fsm_failure(fsm, error_code, ZB_TRACE_FILE_ID, __LINE__)

/**
 * Set error code and go to ZB_FSM_STATE_FINAL state.
 */
void zb_fsm_failure(zb_fsm_t *fsm, zb_ret_t error_code, zb_uint16_t from_file,
    zb_uint16_t from_line);

/**
 *  @brief Set new FSM active descriptor.
 *  @param [in] fsm - pointer to FSM context.
 *  @param [in] desc_type - Descriptor to be set (by type).
 *  @return RET_OK if success, error code otherwise.
 */
zb_ret_t zb_fsm_desc_set(zb_fsm_t *fsm, zb_uint8_t desc_type);

void zb_fsm_chain_init(zb_fsm_chain_1_t *chain, zb_uint8_t chain_size);
zb_ret_t   zb_fsm_chain_add(zb_fsm_t *fsm, zb_fsm_desc_type_t desc_type);
zb_uint8_t zb_fsm_chain_next(zb_fsm_t *fsm);
zb_ret_t   zb_fsm_chain_clear(zb_fsm_t *fsm);


#endif /* _ZB_FSM_H_ */
