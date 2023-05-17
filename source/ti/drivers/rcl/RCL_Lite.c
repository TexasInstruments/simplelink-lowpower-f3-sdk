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
 *  ======== RCL_Lite.c ========
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <ti/drivers/rcl/hal/hal.h>
#include <ti/drivers/rcl/RCL_Command.h>
#include <ti/drivers/rcl/RCL_Lite.h>
#include <ti/drivers/rcl/LRF.h>
#include <ti/drivers/rcl/RCL_Scheduler.h>
#include <ti/drivers/rcl/RCL_Debug.h>

#include <ti/drivers/dpl/HwiP.h>

#include <ti/log/Log.h>

#define RCL_LITE_SCHEDULE_MARGIN RCL_SCHEDULER_SYSTIM_US(150)

/*
 *  ======== RCL_Lite_CommandHwi ========
 */
void RCL_Lite_CommandHwi(void)
{
    RCL_Command *cmd = rclSchedulerState.currCmd;

    if (cmd == NULL)
    {
        return;
    }

    /*** 1. Get events */
    /* 1a. Posted RCL events */
    RCL_Events rclEventsIn = rclSchedulerState.postedRclEvents;
    RCL_Events rclEventsOut = RCL_EventNone;
    rclSchedulerState.postedRclEvents = RCL_EventNone;

    /* 1b. Timer events mapped to RCL events */
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

    /* 1c. Hardware LRF events */
    LRF_Events lrfEvents = {.value = hal_get_ifg_reg()};

    Log(RclCore, Log_INFO2, "Command input events RCL: %08X; LRF: %08X", rclEventsIn.value, lrfEvents.value);

    /*** 2. Handle stop or setup event (either from timer or posted SW event) */
    /* Hardstop immediately, graceful is up to the handler */
    if (rclEventsIn.gracefulStop || rclEventsIn.hardStop || rclEventsIn.descheduleStop)
    {
        /* If not started yet, just cancel timer and increment status to
         * finished; bypasses handler later */
        if (cmd->status == RCL_CommandStatus_Scheduled)
        {
            hal_cancel_setup_time();
            cmd->status = RCL_CommandStatus_Descheduled;
        }
        else /* Command started, handle hardStop here */
        {
            if (rclEventsIn.hardStop)
            {
                LRF_hardStop();
            }
        }
    }

    /*** 3. Invoke handler FSM with new events */
    Log(RclCoreShort, Log_INFO2, "Handling %04X; state %02X", cmd->cmdId, cmd->status);
    Log(RclCore, Log_INFO2, "RCL in: %08X, LRF in: %08X", rclEventsIn.value, lrfEvents.value);
    if (cmd->status >= RCL_CommandStatus_Scheduled && cmd->status < RCL_CommandStatus_Finished)
    {
        rclEventsOut = cmd->runtime.handler(cmd, lrfEvents, rclEventsIn);
    }
    Log(RclCore, Log_INFO2, "RCL out: %08X", rclEventsOut.value);

    /*** 4. If the command was caused to start now, configure end timeouts */
    if (rclEventsOut.cmdStarted)
    {
        RCL_StopType immediateStop = RCL_Scheduler_setStopTimes();
        if (immediateStop != RCL_StopType_None)
        {
            RCL_Lite_Command_stop(immediateStop);
        }
    }

    /*** 5. If events, do callbacks */
    if (cmd->status != RCL_CommandStatus_Descheduled)
    {
        /* Do callbacks */
        if ((((lrfEvents.value & cmd->runtime.lrfCallbackMask.value) != 0) ||
            ((rclEventsOut.value & cmd->runtime.rclCallbackMask.value) != 0)) &&
            (cmd->runtime.callback != NULL))
        {
            cmd->runtime.callback(cmd, lrfEvents, rclEventsOut);
        }
    }

    /*** 6. If finished, clean up */
    if (cmd->status >= RCL_CommandStatus_Finished)
    {
        Log(RclCore, Log_INFO2, "Done. Status %02X", cmd->status);
        /* Stop any timer events still active */
        hal_cancel_graceful_stop_time();
        hal_cancel_hard_stop_time();
        /* Disable and clear any pending interrupts on command ISR */
        hal_disable_radio_interrupts();
        /* Clear global scheduler state */
        rclSchedulerState = (RCL_SchedulerState) {0};
    }
}

/*
 *  ======== RCL_Lite_Command_start ========
 */
void RCL_Lite_Command_start(RCL_Command_Handle c)
{
    RCL_Command *cmd = (RCL_Command *)c;
    RCL_Debug_assert(cmd->status == RCL_CommandStatus_Idle);

    /* Try to submit */
    if (rclSchedulerState.currCmd == NULL)
    {
        rclSchedulerState.currCmd = cmd;
        cmd->status = RCL_CommandStatus_Scheduled;
        /* Check scheduling */
        if (rclSchedulerState.currCmd->scheduling == RCL_Schedule_Now)
        {
            /* Trigger immediate start */
            RCL_Scheduler_postEvent(cmd, RCL_EventSetup);
        }
        else
        {
            /* Program start time, but do not enable interrupt yet */
            uint32_t programmedStartTime = rclSchedulerState.currCmd->timing.absStartTime - RCL_LITE_SCHEDULE_MARGIN;
            uintptr_t key = HwiP_disable();
            hal_setup_setup_time(programmedStartTime);
            if (!RCL_Scheduler_isLater(RCL_Scheduler_getCurrentTime(), programmedStartTime))
            {
                /* Start time is in the past  */
                if (rclSchedulerState.currCmd->allowDelay)
                {
                    /* Start immediately */
                    hal_cancel_setup_time();

                    /* Trigger immediate start */
                    RCL_Scheduler_postEvent(cmd, RCL_EventSetup);
                }
                else
                {
                    cmd->status = RCL_CommandStatus_Error_StartTooLate;
                    HwiP_restore(key);
                    return;
                }
            }
            HwiP_restore(key);
        }
    }
}

/*
 *  ======== RCL_Lite_Command_isFinished ========
 */
bool  RCL_Lite_Command_isFinished(void)
{
    return (rclSchedulerState.currCmd == NULL);
}

/*
 *  ======== RCL_Lite_Command_stop ========
 */
RCL_Command_Handle *RCL_Lite_Command_stop(RCL_StopType stopType)
{
    RCL_Command_Handle *c;
    uintptr_t key = HwiP_disable();

    RCL_Command *cmd = rclSchedulerState.currCmd;

    /* Check if command is already finished */
    if (cmd == NULL)
    {
        c = NULL;
    }
    else
    {
        RCL_Events rclEvent = RCL_EventNone;
        switch (stopType)
        {
            case RCL_StopType_DescheduleOnly:
                rclEvent.descheduleStop = 1;
                break;
            case RCL_StopType_Graceful:
                rclEvent.gracefulStop = 1;
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

        c = (RCL_Command_Handle *)cmd;
    }

    HwiP_restore(key);

    return c;
}
