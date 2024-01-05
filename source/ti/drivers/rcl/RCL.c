/*
 * Copyright (c) 2021-2023, Texas Instruments Incorporated
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
/*
 *  ======== RCL.c ========
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include <ti/drivers/rcl/hal/hal.h>
#include <ti/drivers/rcl/RCL.h>
#include <ti/drivers/rcl/LRF.h>
#include <ti/drivers/rcl/RCL_Scheduler.h>
#include <ti/drivers/rcl/RCL_Profiling.h>
#include <ti/drivers/rcl/RCL_Tracer.h>
#include <ti/drivers/rcl/RCL_Debug.h>

#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/HwiP.h>

#include <ti/log/Log.h>

RCL rclState;

static void rclCommandHwi(void);
static void rclDispatchHwi(void);
static void rclSchedulerHwi(void);
static void rclPowerNotify(RCL_PowerEvent eventType);
static RCL_CommandStatus rclStop(RCL_Command_Handle c, RCL_StopType stopType, RCL_SchedulerStopReason stopReason);

/* Hooks */

/**
 * @brief Hook function to determine what stop type the ongoing command should get
 *
 * @param[in] currentCmd Currently running command
 * @param[in] newCmd New command to potentially take its place
 *
 * @return ::RCL_StopType How the ongoing command will be terminated if not already terminated when needed
 */
RCL_StopType policyHook(RCL_Command *currentCmd, RCL_Command *newCmd);

/**
 * @brief Hook function to allow dynamic PHY change
 *
 * @param[in] rclState RCL state
 * @param[in] client Current RCL client
 * @param[in] cmd Currently running command
 *
 */
void phyHook(RCL *rclState, RCL_Handle client, RCL_Command *cmd);

/**
 * @brief Hook function to schedule new command
 *
 * @param[in] rclSchedulerState Scheduler state
 * @param[in] client Current RCL client
 * @param[in] cmd Command to be scheduled
 *
 */
void scheduleHook(RCL_SchedulerState *rclSchedulerState, RCL_Handle client, RCL_Command *cmd);

/* command - highest pri hwi  */
static void rclCommandHwi(void)
{
    RCL_Command *cmd = rclSchedulerState.currCmd;
    /* We expect no call with NULL pointer, but if debug is off, handle the error by returning safely. */
    RCL_Debug_assert(NULL != cmd);

    /* If command is already finished, return without doing anything */
    if (NULL == cmd || cmd->status >= RCL_CommandStatus_Finished)
    {
        return;
    }

    RCL_Client *client = cmd->runtime.client;

    /*** 1. Get events */
    /* 1a. Posted RCL events */
    RCL_Events rclEventsIn = rclSchedulerState.postedRclEvents;
    RCL_Events rclEventsOut = RCL_EventNone;
    rclSchedulerState.postedRclEvents = RCL_EventNone;

    /* 1b. Hardware LRF events */
    LRF_Events lrfEvents = {.value = hal_get_ifg_reg()};

    /* 1c. Timer events mapped to RCL events */
    HalTimerEvent timerEvt = hal_check_clear_timer_compare();
    switch (timerEvt)
    {
        case HAL_TIMER_EVT_SETUP:
            rclEventsIn.setup = 1;
            break;
        case HAL_TIMER_EVT_START:
            rclEventsIn.timerStart = 1;
            break;
        case HAL_TIMER_EVT_HARD_STOP:
            rclEventsIn.hardStop = 1;
            break;
        case HAL_TIMER_EVT_GRACEFUL_STOP:
            rclEventsIn.gracefulStop = 1;
            break;
        default:
            break;
    }

    Log_printf(RclCore, Log_DEBUG, "Command input events RCL: 0x%08X; LRF: 0x%08X", rclEventsIn.value, lrfEvents.value);

    /*** 2. Handle stop or setup event (either from timer or posted SW event) */
    /* Hardstop immediately, graceful is up to the handler */
    if (rclEventsIn.gracefulStop || rclEventsIn.hardStop || rclEventsIn.descheduleStop)
    {
        /* If not started yet, just cancel timer and increment status to
         * finished; bypasses handler later */
        if (cmd->status == RCL_CommandStatus_Scheduled)
        {
            hal_cancel_start_time();
            if (rclEventsIn.hardStop)
            {
                rclSchedulerState.descheduleReason = rclSchedulerState.hardStopInfo.stopReason;
            }
            else if (rclEventsIn.gracefulStop)
            {
                rclSchedulerState.descheduleReason = rclSchedulerState.gracefulStopInfo.stopReason;
            }
            else
            {
                /* The event is descheduleStop - source already set */
            }
            cmd->status = RCL_Scheduler_findStopStatus(RCL_StopType_DescheduleOnly);
        }
        else /* Command started, handle hardStop here */
        {
            if (rclEventsIn.hardStop)
            {
                LRF_hardStop();
            }
        }
    }
    else if (rclEventsIn.setup)
    {
        /* Load TOPsm images, settings, etc */
        LRF_RadioState lrfState = rclState.lrfState;
        if (rclState.lrfConfig != client->lrfConfig)
        {
            /* Different config than last time: Ensure settings are reloaded */
            rclState.lrfConfig = client->lrfConfig;
            if (lrfState > RadioState_ImagesLoaded)
            {
                lrfState = RadioState_ImagesLoaded;
            }
        }
        LRF_SetupResult result = LRF_setupRadio(rclState.lrfConfig, cmd->phyFeatures, lrfState);
        if (result != SetupResult_Ok)
        {
            Log_printf(RclCoreShort, Log_ERROR, "Setup failed with code %1d", result);
            cmd->status = RCL_CommandStatus_Error_Setup;
            rclEventsOut.lastCmdDone = 1;
            rclState.lrfState = RadioState_Down;
        }
        else
        {
            rclState.lrfState = RadioState_Configured;
        }

        /* Set power constraints since the radio has been set up */
        if (rclState.powerState == RCL_standbyAllow)
        {
            hal_power_set_constraint();
            rclState.powerState = RCL_standbyDisallow;
        }
    }

    /*** 3. Invoke handler FSM with new events */
    if (cmd->status >= RCL_CommandStatus_Scheduled && cmd->status < RCL_CommandStatus_Finished)
    {
        rclEventsOut = cmd->runtime.handler(cmd, lrfEvents, rclEventsIn);
    }
    Log_printf(RclCore, Log_DEBUG, "RCL out: 0x%08X", rclEventsOut.value);

    /*** 4. If the command was caused to start now, configure end timeouts */
    if (rclEventsOut.cmdStarted)
    {
        RCL_StopType immediateStop = RCL_Scheduler_setStopTimes();
        if (immediateStop != RCL_StopType_None)
        {
            rclStop(rclSchedulerState.currCmd, immediateStop, RCL_SchedulerStopReason_Timeout);
        }
    }
    /*** 5. If the command raises an event indicating the need for partial radio setup, do it */
    if (rclEventsOut.partialSetup)
    {
        /* Rerun the radio setup */
        LRF_RadioState lrfState = rclState.lrfState;

        LRF_SetupResult result = LRF_setupRadio(rclState.lrfConfig, rclSchedulerState.requestedPhyFeatures, lrfState);
        if (result != SetupResult_Ok)
        {
            Log_printf(RclCoreShort, Log_ERROR, "Setup failed with code %1d", result);
            cmd->status = RCL_CommandStatus_Error_Setup;
            rclEventsOut.lastCmdDone = 1;
            rclState.lrfState = RadioState_Down;
        }
        else
        {
            rclState.lrfState = RadioState_Configured;
        }
        /* Invoke command handler again to resume the ongoing operation */
        rclEventsIn.handlerCmdUpdate = 1;
        rclEventsOut = cmd->runtime.handler(cmd, lrfEvents, rclEventsIn);
    }

    /*** 6. If stop did not happen, queued event might be delayed */
    if (rclEventsIn.gracefulStop && cmd->status < RCL_CommandStatus_Finished)
    {
        rclEventsOut.stopDelayed = 1;
    }
    if (rclEventsIn.descheduleStop && cmd->status < RCL_CommandStatus_Finished)
    {
        rclEventsOut.stopRejected = 1;
    }

    /* We pass the stop things to dispatch even if not subscribed to current,
     * since they affect the next queued command
     */
    RCL_Events rclEventMask = cmd->runtime.rclCallbackMask;
    rclEventMask.stopDelayed = 1;
    rclEventMask.stopRejected = 1;

    /*** 7. Filter events, pass stop-rejected to setup, to alert nextCmd */
    client->deferredLrfEvents.value |= cmd->runtime.lrfCallbackMask.value & lrfEvents.value;
    client->deferredRclEvents.value |= rclEventMask.value & rclEventsOut.value;

    /*** 8. If finished or have events, invoke setup */
    if (cmd->status >= RCL_CommandStatus_Finished || client->deferredLrfEvents.value || client->deferredRclEvents.value)
    {
        if (cmd->status >= RCL_CommandStatus_Finished)
        {
            hal_cancel_graceful_stop_time();
            hal_cancel_hard_stop_time();
            hal_disable_radio_interrupts();

            /* It's now safe to go into standby */
            if (rclState.powerState != RCL_standbyAllow)
            {
                hal_power_release_constraint();
                rclState.powerState = RCL_standbyAllow;
            }
        }
        hal_trigger_dispatch_fsm();
    }
}

static void rclDispatchHwi(void)
{
    /* Keep track for callback */
    RCL_Command *currCmd = rclSchedulerState.currCmd;
    if (NULL == currCmd)
    {
        /* An extra pending IRQ may happen if the command HWI comes in and
           finishes the command during this ISR. If so, the extra IRQ can be ignored. */
        return;
    }
    RCL_Client *currClient = currCmd->runtime.client;
    RCL_Debug_assert(currClient != NULL);

    /* If command completed, take out of circulation for possible reuse and
        * trigger scheduler in any case to tear down / start next.
        */
    if (currCmd->status >= RCL_CommandStatus_Finished)
    {
        /* It's done or failed */
        Log_printf(RclCore, Log_DEBUG, "Finished; Clearing currCmd, calling scheduleHwi");
        RCL_Command *doneCmd = rclSchedulerState.currCmd;
        rclSchedulerState.currCmd = NULL;

        if (doneCmd->runtime.client->pendCmd == doneCmd)
        {
            Log_printf(RclCore, Log_INFO, "Unpending client: 0x%08X", doneCmd->runtime.client);
            doneCmd->runtime.client->pendCmd = NULL;
            SemaphoreP_post(&doneCmd->runtime.client->pendSem);
        }

        /* See if more waiting, schedule-fsm runs after this hwi */
        hal_trigger_scheduler_fsm();
        /* Now rclSchedulerState.currCmd is free in case client needs to queue something new before scheduler */
    }

    /* Atomically retrieve what is supposed to get done since higher priority
     * interrupt sets these
     */
    uintptr_t key = HwiP_disable();
    LRF_Events lrfEvents = currClient->deferredLrfEvents;
    RCL_Events rclEvents = { .value = currClient->deferredRclEvents.value & currCmd->runtime.rclCallbackMask.value };
    RCL_Events stopEvents = { .value = currClient->deferredRclEvents.value &  ((RCL_Events){.stopDelayed = 1, .stopRejected = 1}).value };
    RCL_Callback callback = currCmd->runtime.callback;

    currClient->deferredLrfEvents = LRF_EventNone;
    currClient->deferredRclEvents = RCL_EventNone;
    HwiP_restore(key);

    /* Check if failed stop is a problem for the next command */
    if (stopEvents.value) {
        RCL_Command *nextCmd = rclState.nextCmd;
        if (nextCmd && !nextCmd->allowDelay && rclSchedulerState.nextWantsStop)
        {
            Log_printf(RclCore, Log_DEBUG, "Command deplanned due to rejected start: 0x%08X", nextCmd);

            /* Next command does not allow delay, and stop did not take immediate effect */
            rclState.nextCmd = NULL;
            rclSchedulerState.nextWantsStop = false;
            /* In a world where there can be a queue of commands, get a new next command, lower pri executes after this hwi */
            hal_trigger_scheduler_fsm();

            /* Synthesize rejected start */
            nextCmd->status = RCL_CommandStatus_RejectedStart;
            RCL_Events rejectedEvents = RCL_EventNone;
            rejectedEvents.lastCmdDone = 1;
            rejectedEvents.startRejected = 1;

            /* Notify owner that command was rejected, if they care */
            RCL_Callback callback = nextCmd->runtime.callback;
            if (callback && rejectedEvents.value & nextCmd->runtime.rclCallbackMask.value)
            {
                callback(nextCmd, LRF_EventNone, rejectedEvents);
            }
        }
    }

    /* Notify owner about events */
    Log_printf(RclCore, Log_INFO, "Client callback: LRF: 0x%08X, RCL: 0x%08X", lrfEvents.value, rclEvents.value);
    if ((lrfEvents.value || rclEvents.value) && callback)
    {
        callback(currCmd, lrfEvents, rclEvents);
    }
}

/*
 *  ======== policyHook ========
 */
__attribute__((weak)) RCL_StopType policyHook(RCL_Command *currentCmd, RCL_Command *newCmd)
{
    (void) currentCmd;

    switch(newCmd->conflictPolicy)
    {
        case RCL_ConflictPolicy_AlwaysInterrupt:
            return RCL_StopType_Hard;
        case RCL_ConflictPolicy_Polite:
            return RCL_StopType_Graceful;
        case RCL_ConflictPolicy_NeverInterrupt:
            return RCL_StopType_DescheduleOnly;
        default:
            return RCL_StopType_None;
    }
}

/*
 *  ======== phyHook ========
 */
__attribute__((weak)) void phyHook(RCL *rclState, RCL_Handle client, RCL_Command *cmd)
{
    (void)rclState;
    (void)client;
    (void)cmd;
}

/*
 *  ======== scheduleHook ========
 */
__attribute__((weak)) void scheduleHook(RCL_SchedulerState *rclSchedulerState, RCL_Handle client, RCL_Command *cmd)
{
    (void) client;

    RCL_StopType stopType = policyHook(rclSchedulerState->currCmd, cmd);

    if (rclSchedulerState->currCmd && stopType > RCL_StopType_None)
    {
        uint32_t now = RCL_Scheduler_getCurrentTime();
        uint32_t then = cmd->timing.absStartTime - RCL_SCHEDULER_MARGIN_LOAD*2;
        bool urgent = !RCL_Scheduler_isLater(now, then);

        rclSchedulerState->nextWantsStop = true;
        if (cmd->scheduling == RCL_Schedule_Now || urgent)
        {
            Log_printf(RclCore, Log_VERBOSE, "Stopping old command immediately, urgent=%d", urgent);
            rclStop(rclSchedulerState->currCmd, stopType, RCL_SchedulerStopReason_Scheduling);
        }
        else
        {
            Log_printf(RclCore, Log_VERBOSE, "Setting running command stop-time to 0x%08X", then);
            RCL_SchedulerStopInfo *stopInfo;
            switch (stopType)
            {
                case RCL_StopType_Hard:
                    stopInfo = &rclSchedulerState->hardStopInfo;
                    break;
                case RCL_StopType_Graceful:
                    stopInfo = &rclSchedulerState->gracefulStopInfo;
                    break;
                default:
                    stopInfo = NULL;
                    break;
            }
            if (stopInfo != NULL)
            {
                /* Set selected scheduler stop */
                /* Check if this changes active stop times */
                RCL_StopType immediateStop = RCL_Scheduler_setSchedStopTime(stopInfo, then);
                /* Stop now if new stop time is in the past */
                if (immediateStop != RCL_StopType_None)
                {
                    rclStop(rclSchedulerState->currCmd, immediateStop, RCL_SchedulerStopReason_Scheduling);
                }
            }
        }
    }
}

/* scheduler - lowest pri hwi */
static void rclSchedulerHwi(void)
{
    /* Find next command */
    /* TODO: See RCL-344 */
    RCL_Command *nextCmd = rclState.nextCmd;
    Log_printf(RclCore, Log_VERBOSE, "SchedulerHwi nextCmd: 0x%08X", nextCmd);

    /* If nothing is pending, pack up */
    if (NULL == nextCmd)
    {
        RCL_Profiling_eventHook(RCL_ProfilingEvent_PostprocStop);
        return;
    }

    int32_t deltaTime = (nextCmd->scheduling == RCL_Schedule_Now)
                            ? (int32_t)RCL_SCHEDULER_TRIG_NOW_DELAY
                            : RCL_Scheduler_delta(RCL_Scheduler_getCurrentTime(), nextCmd->timing.absStartTime);
    /* Event must be in future */
    if (false == nextCmd->allowDelay && deltaTime < (int32_t)RCL_SCHEDULER_TRIG_NOW_DELAY)
    {
        nextCmd->status = RCL_CommandStatus_Error_StartTooLate;
        return;
    }

    /* ScheduleHook might immediately terminate running command and cmdHwi might update currCmd to NULL */
    scheduleHook(&rclSchedulerState, nextCmd->runtime.client, nextCmd);

    /* Determine if can progress nextCmd to currCmd and invoke cmdHwi */
    if (rclSchedulerState.currCmd)
    {
        /* If there is a current command, we will be triggered again after it is
         * finished.
         */
        Log_printf(RclCore, Log_VERBOSE, "Could not promote; command 0x%08X running, status 0x%02X",
                                rclSchedulerState.currCmd,
                                rclSchedulerState.currCmd->status);
        /* A finished command should not be the current command */
        RCL_Debug_assert(rclSchedulerState.currCmd->status < RCL_CommandStatus_Finished);
        return;
    }

    /* Adopt new command */
    rclState.nextCmd = NULL;
    memset((void *)&rclSchedulerState, 0, sizeof(rclSchedulerState));
    rclSchedulerState.currCmd = nextCmd;

    /* Next command may need different PHY applied; prepare this */
    phyHook(&rclState, nextCmd->runtime.client, rclSchedulerState.currCmd);

    if (deltaTime <= (int32_t)RCL_SCHEDULER_SLEEP_CUTOFF)
    {
        Log_printf(RclCore, Log_DEBUG, "Calling setup immediately, %d µs until event", deltaTime >> 2);

        /* Command handler does last mile config and trigger */
        RCL_Scheduler_postEvent(rclSchedulerState.currCmd, RCL_EventSetup);
    }
    else
    {
        /* TODO: See RCL-275 */
        uint32_t margin = RCL_SCHEDULER_MARGIN_ARM + RCL_SCHEDULER_WAKEUP_MARGIN;
        if (rclState.lrfState < RadioState_Configured ||
            nextCmd->runtime.client->lrfConfig != rclState.lrfConfig)
        {
            margin += RCL_SCHEDULER_MARGIN_CONFIGURE;
        }
        if (rclState.lrfState < RadioState_ImagesLoaded ||
            LRF_imagesNeedUpdate(nextCmd->runtime.client->lrfConfig))
        {
            margin += RCL_SCHEDULER_MARGIN_LOAD;
        }

        /* Use LRF:systim0 (SYSTIM:CH2) as wakeup source */
        hal_setup_setup_time(rclSchedulerState.currCmd->timing.absStartTime - margin);

        /* SetupFSM triggers command handler due to timer */
        Log_printf(RclCore, Log_DEBUG, "Wakeup scheduled at 0x%08X (.25µs) with margin subtracted from deltaTime: %d µs", rclSchedulerState.currCmd->timing.absStartTime, deltaTime >> 2);
    }
}

/* Power event routine */
static void rclPowerNotify(RCL_PowerEvent eventType)
{
    if (eventType == RCL_POWER_STANDBY_AWAKE)
    {
        /*
        * Executed everytime the device exits the standby sleep state.
        */
        /* Reinitialize the tracer */
        RCL_Tracer_enable();

        /* Mark radio as not configured */
        if (rclState.lrfState > RadioState_ImagesLoaded)
        {
            rclState.lrfState = RadioState_ImagesLoaded;
        }
        RCL_Profiling_eventHook(RCL_ProfilingEvent_PreprocStart);
        /* TODO: RCL-287. Enable all clocks here. */
        LRF_rclEnableRadioClocks();
        /* Check if there is an active command at the time of wakeup */
        /* If so, enable start timer interrupt */
        if (rclSchedulerState.currCmd != NULL)
        {
            /* Restore the DBELL interrupt mask */
            hal_enable_setup_time_irq();
        }
    }
    else
    {
        /* No action */
    }
}

/*
 *  ======== RCL_init ========
 */
void RCL_init(void)
{
    /* Initialize state  */
    rclState = (RCL){
        .numClients = 0,
        .lrfState = RadioState_Down,
        .lrfConfig = NULL,
    };

    hal_init_fsm(rclDispatchHwi, rclSchedulerHwi, rclCommandHwi);

    /* Ensure temperature compensation of TX output power and RF Trims */
    hal_temperature_init();
}

/*
 *  ======== RCL_open ========
 */
RCL_Handle RCL_open(RCL_Client *c, const LRF_Config *lrfConfig)
{
    *c = (RCL_Client){ 0 };

    SemaphoreP_Params sp;
    SemaphoreP_Params_init(&sp);
    sp.mode = SemaphoreP_Mode_BINARY;
    SemaphoreP_construct(&c->pendSem, 0, &sp);

    if (rclState.numClients == 0)
    {
        /* Do the operations below if no client was open prior to this one */
        hal_power_open(&rclPowerNotify);

        /* Initialize the tracer */
        RCL_Tracer_enable();

        /* Temporary solution: Enable all needed clocks here */
        LRF_rclEnableRadioClocks();

        /* Temporary solution: Enable the high performance clock buffer.
         * This could be done together with synth REFSYS at the start and end of the command */
        hal_enable_clk_buffer();
    }
    rclState.numClients += 1;
    c->lrfConfig = lrfConfig;

    return c;
}

/*
 *  ======== RCL_close ========
 */
void RCL_close(RCL_Handle h)
{
    rclState.numClients -= 1;

    if (h->lrfConfig == rclState.lrfConfig)
    {
        /* Closing a client using the current LRF config */
        /* Invalidate config, in case it gets changed before reused in a new client */
        rclState.lrfConfig = NULL;
    }

    if (rclState.numClients == 0)
    {
        /* Temporary solution: Disable clocks here */
        LRF_rclDisableRadioClocks();
        /* Disable RF tracer */
        RCL_Tracer_disable();
        hal_power_close();
    }

    return;
}

/*
 *  ======== submitHook ========
 */
__attribute__((weak)) bool submitHook(RCL *rclState, RCL_Handle h, RCL_Command *c)
{
    (void) h;
    /* Reject if already pending, can't be bothered with list  */
    if (rclState->nextCmd != NULL)
    {
        c->status = RCL_CommandStatus_Error;
        return false;
    }

    /* Schedule command */
    rclState->nextCmd = c;
    c->status = RCL_CommandStatus_Scheduled;

    return true;
}

/*
 *  ======== RCL_Command_submit ========
 */
RCL_CommandStatus RCL_Command_submit(RCL_Handle h, RCL_Command_Handle c)
{
    RCL_Command *cmd = (RCL_Command *)c;

    /* Can't submit again if already submitted */
    if (cmd->status != RCL_CommandStatus_Idle && cmd->status < RCL_CommandStatus_Finished)
    {
        return RCL_CommandStatus_Error_AlreadySubmitted;
    }

    /* Extra check in case user modified status field */
    if (cmd == rclSchedulerState.currCmd || cmd == rclState.nextCmd)
    {
        return RCL_CommandStatus_Error_AlreadySubmitted;
    }

    /* Point back to originator */
    cmd->runtime.client = h;

    /* Try to submit */
    if (!submitHook(&rclState, h, cmd))
    {
        return RCL_CommandStatus_Error;
    }

    /* Trigger scheduling FSM */
    hal_trigger_scheduler_fsm();

    return cmd->status;
}

/*
 *  ======== RCL_Command_pend ========
 */
RCL_CommandStatus RCL_Command_pend(RCL_Command_Handle c)
{
    RCL_Command *cmd = (RCL_Command *)c;
    RCL_Handle h = cmd->runtime.client;

    RCL_Debug_assert(cmd != NULL);
    RCL_Debug_assert(h != NULL);

    /* Indicate interest */
    h->pendCmd = cmd;

    /* Check if already completed. */
    if (cmd->status > RCL_CommandStatus_Active)
    {
        h->pendCmd = NULL;
        return cmd->status;
    }

    /* Wait */
    SemaphoreP_pend(&h->pendSem, SemaphoreP_WAIT_FOREVER);

    return cmd->status;
}

/*
 *  ======== RCL_Command_stop ========
 */
RCL_CommandStatus RCL_Command_stop(RCL_Command_Handle c, RCL_StopType stopType)
{
    return rclStop(c, stopType, RCL_SchedulerStopReason_Api);
}

/*
 *  ======== rclStop ========
 */
static RCL_CommandStatus rclStop(RCL_Command_Handle c, RCL_StopType stopType, RCL_SchedulerStopReason stopReason)
{
    RCL_Command *cmd = (RCL_Command *)c;

    /* Check if command is already finished or no stop is done*/
    uintptr_t key = HwiP_disable();
    if (cmd->status < RCL_CommandStatus_Queued || cmd->status >= RCL_CommandStatus_Finished || stopType == RCL_StopType_None)
    {
        HwiP_restore(key);
        Log_printf(RclCoreShort, Log_DEBUG, "Stop called with type: %d, resulting status: 0x%02X", stopType, cmd->status);
        return cmd->status;
    }

    if (cmd->status == RCL_CommandStatus_Queued)
    {
        rclState.nextCmd = NULL;
        cmd->status = RCL_Scheduler_findStopStatus(RCL_StopType_DescheduleOnly);;
    }
    else
    {
        RCL_Events rclEvent = RCL_EventNone;
        switch (stopType)
        {
            case RCL_StopType_DescheduleOnly:
                rclSchedulerState.descheduleReason = stopReason;
                rclEvent.descheduleStop = 1;
                break;
            case RCL_StopType_Graceful:
                rclEvent.gracefulStop = 1;
                rclSchedulerState.gracefulStopInfo.stopReason = stopReason;
                /* Do not send graceful stop if any stop is already sent */
                if (rclSchedulerState.gracefulStopInfo.apiStopEnabled == 0 &&
                    rclSchedulerState.hardStopInfo.apiStopEnabled == 0)
                {
                    LRF_sendGracefulStop();
                    rclSchedulerState.gracefulStopInfo.apiStopEnabled = 1;
                }
                break;
            case RCL_StopType_Hard:
                /* Do not send hard stop if already sent (but send if graceful stop is sent) */
                rclEvent.hardStop = 1;
                rclSchedulerState.hardStopInfo.stopReason = stopReason;
                if (rclSchedulerState.hardStopInfo.apiStopEnabled == 0)
                {
                    LRF_sendHardStop();
                    rclSchedulerState.hardStopInfo.apiStopEnabled = 1;
                }
                break;
            case RCL_StopType_None:
            default:
                RCL_Debug_assert(1 == 0);
                break;
        }
        RCL_Scheduler_postEvent(cmd, rclEvent);
    }
    HwiP_restore(key);

    Log_printf(RclCoreShort, Log_DEBUG, "Stop called with type: %d, resulting status: 0x%02X", stopType, cmd->status);
    return cmd->status;
}

/*
 *  ======== RCL_readRssi ========
 */
int8_t RCL_readRssi(void)
{
    int8_t rssiVal;

    if (rclState.lrfState < RadioState_Configured)
    {
        rssiVal = LRF_RSSI_INVALID;
    }
    else
    {
        rssiVal = LRF_readRssi();
    }
    return rssiVal;
}
