/*
 * Copyright (c) 2015-2025, Texas Instruments Incorporated
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
/*!****************************************************************************
 *  @file       Power.h
 *  @brief      Power Manager
 *
 *  @anchor ti_drivers_Power_Overview
 *  # Overview
 *
 *  The Power Manager facilitates the transition of the MCU from active states
 *  to sleep states and vice versa. It provides other drivers the
 *  ability to set and release dependencies on hardware resources, and keeps
 *  reference counts on each resource to know when to enable or disable the
 *  resource. It provides drivers the ability to register callback functions
 *  to be invoked upon specific power events. In addition, drivers and
 *  applications can set or release constraints to prevent the MCU from
 *  transitioning into specific active or sleep states. Refer to the device
 *  specific power driver header file device specific information.
 *
 *  <hr>
 *  @anchor ti_drivers_Power_Usage
 *  # Usage
 *
 *  This documentation provides a basic @ref ti_drivers_Power_Synopsis
 *  "usage summary" and a set of @ref ti_drivers_Power_Examples "examples"
 *  in the form of commented code fragments. Detailed descriptions of the
 *  APIs are provided in subsequent sections.
 *  @anchor ti_drivers_Power_Synopsis
 *  ## Synopsis
 *  @anchor ti_drivers_Power_Synopsis_Code
 *
 *  @note  <b> The following example demonstrates usage of some of the Power
 *  driver APIs.This example is intended for reference only and is not intended
 *  for application use. You should refer to the device specific Power driver
 *  header for valid API usage and arguments. </b>
 *
 *
 *  @code
 *  // Import Power Driver definitions
 *  #include <ti/drivers/Power.h>
 *
 *  // One-time initialization of Power manager
 *  Power_init();
 *
 *  // Set power dependency on a resource
 *  status = Power_setDependency(resourceId);
 *  if (status != Power_SOK) {
 *      // Error occurred
 *  }
 *
 *  // Set a power constraint
 *  status = Power_setConstraint(constraintId);
 *  if (status != Power_SOK) {
 *      // Error occurred
 *  }
 *
 *  // Other application code
 *
 *  // Release a previously set power constraint
 *  status = Power_releaseConstraint(constraintId);
 *  if (status != Power_SOK) {
 *      // Error occurred
 *  }
 *
 *  status = Power_releaseDependency(resourceId);
 *  if (status != Power_SOK) {
 *      // Error occurred
 *  }
 *  @endcode
 *
 *
 *  <hr>
 *  @anchor ti_drivers_Power_Examples
 *  # Examples
 *
 *  @note
 *  <b>The following examples are intended for reference only and are not
 *  intended for application use. You should refer to the device specific
 *  Power driver header file for more usage information.</b>
 *
 *  @li @ref ti_drivers_Power_Examples_enable "Enabling power policy"
 *  @li @ref ti_drivers_Power_Examples_disable "Disabling power policy"
 *  @li @ref ti_drivers_Power_Examples_constraint "Using power constraints"
 *  @li @ref ti_drivers_Power_Examples_dependency "Using power dependency"
 *  @li @ref ti_drivers_Power_Examples_notify "Using power notify"
 *  @li @ref ti_drivers_Power_Examples_transistion "Power transitions"
 *
 *
 *  @anchor ti_drivers_Power_Examples_enable
 *  ## Enabling Power Policy
 *
 *  @code
 *  // Import Power Driver definitions
 *  #include <ti/drivers/Power.h>
 *
 *  // One-time initialization of Power manager
 *  Power_init();
 *
 *  // Enable power policy
 *  Power_enablePolicy();
 *  @endcode
 *
 *
 *  @anchor ti_drivers_Power_Examples_disable
 *  ## Disabling Power Policy
 *
 *  @code
 *  // Import Power Driver definitions
 *  #include <ti/drivers/Power.h>
 *
 *  bool flag;
 *
 *  // One-time initialization of Power manager
 *  Power_init();
 *
 *  // Disable power policy
 *  flag = Power_disablePolicy();
 *  if (flag == false) {
 *      // Power policy was already disabled
 *  }
 *  @endcode
 *
 *
 *  @anchor ti_drivers_Power_Examples_constraint
 *  ## Using Power Constraints
 *
 *  @code
 *  // Import Power Driver definitions
 *  #include <ti/drivers/Power.h>
 *
 *  uint32_t mask;
 *  int16_t status;
 *
 *  // One-time initialization of Power manager
 *  Power_init();
 *
 *  // Set a power constraint
 *  status = Power_setConstraint(constraintId);
 *  if (status != Power_SOK) {
 *      // Error occurred setting constraint
 *  }
 *
 *  // Read mask of currently set power constraints
 *  mask = Power_getConstraintMask();
 *
 *  // Release previously set constraint
 *  status = Power_releaseConstraint(constraintId);
 *  if (status != Power_SOK) {
 *      // Error occurred releasing constraint
 *  }
 *  @endcode
 *
 *
 *  @anchor ti_drivers_Power_Examples_dependency
 *  ## Using Power Dependency
 *
 *  @code
 *  // Import Power Driver definitions
 *  #include <ti/drivers/Power.h>
 *
 *  int16_t count;
 *  int16_t status;
 *
 *  // One-time initialization of Power manager
 *  Power_init();
 *
 *  // Set a power dependency
 *  status = Power_setDependency(resourceId);
 *  if (status != Power_SOK) {
 *      // Error occurred setting dependency
 *  }
 *
 *  // Get the dependency count of the resource
 *  count = Power_getDependencyCount(resourceId);
 *  if (count == Power_EINVALIDINPUT) {
 *      // Invalid resourceId used
 *  }
 *
 *  if (count > 0) {
 *      // At least 1 dependency exists for the resource.
 *      // Regardless, we may safely release the dependency when we
 *      // no longer need the resource.
 *  }
 *
 *  // Release a power dependency
 *  status = Power_releaseDependency(resourceId);
 *  if (status != Power_SOK) {
 *      // Error occurred releasing dependency
 *  }
 *  @endcode
 *
 *
 *  @anchor ti_drivers_Power_Examples_notify
 *  ## Using Power Notify
 *
 *  The application must define a #Power_NotifyFxn function and
 *  allocate memory for the #Power_NotifyObj object.
 *
 *  @code
 *  // Import Power Driver definitions
 *  #include <ti/drivers/Power.h>
 *
 *  // Application Power_NotifyObj object
 *  Power_NotifyObj powerNotifyObj;
 *
 *  // Application Power_NotifyFxn function prototype
 *  static int postNotifyFxn(unsigned int eventType, uintptr_t eventArg,
 *                           uintptr_t clientArg);
 * @endcode
 *
 *  The application must register for the event. Here, we use pseudo event
 *  names. You should refer to the device specific power driver header file
 *  for eventTypes. Inside the infinite loop, we wait for a semaphore to be
 *  post from our notification callback.
 *
 * @code
 *  // Application thread
 *  void thread(void)
 *  {
 *      int16_t status;
 *      unsigned int eventTypes = LOW_POWER_EXIT | LOW_POWER_ENTER;
 *      uintptr_t clientArg = semaphoreHandle;
 *
 *      status = Power_registerNotify(&powerNotifyObj, eventTypes,
 *                                    postNotifyFxn, clientArg);
 *
 *      while (1)
 *      {
 *          sem_wait(semaphoreHandle);
 *          // Do something
 *
 *          // Unregister for the notification. After this call,
 *          // our postNotifyFxn() will never be called again unless
 *          // we use Power_registerNotify() again.
 *          Power_unregisterNotify(&powerNotifyObj);
 *
 *          break;
 *      }
 *  }
 *  @endcode
 *
 *  The application may implement the power notify function to fit their
 *  needs. The #Power_NotifyFxn should always return #Power_NOTIFYDONE or
 *  #Power_NOTIFYERROR.
 *
 *  @code
 *  // Application Power_NotifyFxn function implementation
 *  static int postNotifyFxn(unsigned int eventType, uintptr_t eventArg,
 *                           uintptr_t clientArg)
 *  {
 *      sem_t semaphoreHandle = (sem_t) clientArg;
 *
 *      if (eventType == LOW_POWER_EXIT) {
 *          sem_post(semaphoreHandle);
 *          return (Power_NOTIFYDONE);
 *      }
 *
 *      if (eventType == LOW_POWER_ENTER) {
 *          // Store something in RAM
 *          return (Power_NOTIFYDONE);
 *      }
 *
 *      // We received an unexpected event type
 *      return (Power_NOTIFYERROR);
 *  }
 *  @endcode
 *
 *
 *  @anchor ti_drivers_Power_Examples_transistion
 *  ## Power transitions
 *
 *  @code
 *  // Import Power Driver definitions
 *  #include <ti/drivers/Power.h>
 *
 *  uint32_t totalLatency, resumeLatency;
 *  int16_t status;
 *
 *  // One-time initialization of Power manager
 *  Power_init();
 *
 *  // Get the current power transition state
 *  status = Power_getTransitionState();
 *
 *  switch (status)
 *  {
 *      case Power_ACTIVE:
 *        // No transitions in progress
 *        break;
 *      case Power_ENTERING_SLEEP:
 *        // Transition to sleep in progress
 *        break;
 *      case Power_EXITING_SLEEP:
 *        // Transition from sleep in progress
 *        break;
 *  }
 *
 *  // Get the Power_TOTAL and Power_RESUME transition latency for a
 *  // device specific sleepState. Latency is in microseconds.
 *  totalLatency = Power_getTransitionLatency(sleepState, Power_TOTAL);
 *  resumeLatency = Power_getTransitionLatency(sleepState, Power_RESUME);
 *  @endcode
 *
 *
 *  <hr>
 *  @anchor ti_drivers_Power_Configuration
 *  # Configuration
 *
 *  @note The Power Manager APIs and configuration parameters are described here.
 *  For a detailed description of terms and concepts, and usage by different
 *  types of software components (peripheral drivers, power policies,
 *  and applications) please see the
 *  <a href='../../Power_Management.pdf'>SimpleLink SDK Power Management User's Guide</a>.
 *  <hr>
 ******************************************************************************
 */

#ifndef ti_drivers_Power__include
#define ti_drivers_Power__include

/* @cond */
#include <stdbool.h>
#include <stdint.h>
/* @endcond */

#include <ti/drivers/utils/List.h>
#include <ti/devices/DeviceFamily.h>

/* Note: Device specific Power include files are included in the bottom of this file. */

#ifdef __cplusplus
extern "C" {
#endif

/*! @addtogroup Power_Latency_Type
 *  @{
 */
#define Power_TOTAL  (1U) /*!< total latency */
#define Power_RESUME (2U) /*!< resume latency */
/*! @}*/

/*! @addtogroup Power_Notify_Response
 *  @{
 */
#define Power_NOTIFYDONE  (0)  /*!< OK, notify completed */
#define Power_NOTIFYERROR (-1) /*!< an error occurred during notify */
/*! @}*/

/*! @addtogroup Power_Status
 *  @{
 */
#define Power_SOK                 (0)  /*!< OK, operation succeeded */
#define Power_EFAIL               (-1) /*!< general failure */
#define Power_EINVALIDINPUT       (-2) /*!< invalid data value */
#define Power_EINVALIDPOINTER     (-3) /*!< invalid pointer */
#define Power_ECHANGE_NOT_ALLOWED (-4) /*!< change is not allowed */
#define Power_EBUSY               (-5) /*!< busy with another transition */
/*! @}*/

/*! @addtogroup Power_Transition_State
 *  @{
 */
#define Power_ACTIVE            (1U) /*!< normal active state */
#define Power_ENTERING_SLEEP    (2U) /*!< entering a sleep state */
#define Power_EXITING_SLEEP     (3U) /*!< exiting a sleep state */
#define Power_ENTERING_SHUTDOWN (4U) /*!< entering a shutdown state */
/*! @}*/

/*!
 *  @brief      Power policy initialization function pointer
 */
typedef void (*Power_PolicyInitFxn)(void);

/*!
 *  @brief      Power policy function pointer
 */
typedef void (*Power_PolicyFxn)(void);

/*!
 *  @brief      Power notify callback function used with the
 *              Power_registerNotify()
 *
 *  @param[in]  eventType    The eventTypes parameter identifies the type of
 *  power event for which the notify callback function was called.
 *
 *  @param[in]  eventArg    An optional @p eventType specific argument.
 *
 *  @param[in]  clientArg    Pointer to a custom argument.
 *
 *  @retval #Power_NOTIFYDONE if the client processed the notification
 *  successfully
 *
 *  @retval #Power_NOTIFYERROR if an error occurred during notification.
 *
 *  @sa  Power_registerNotify()
 *  @sa  Power_unregisterNotify()
 *  @sa  Power_NotifyObj
 *  @sa  @ref ti_drivers_Power_Examples_notify "Using power notify"
 */
typedef int_fast16_t (*Power_NotifyFxn)(uint_fast16_t eventType, uintptr_t eventArg, uintptr_t clientArg);

/*!
 *  @brief      Power notify object structure.
 *
 *  This structure specification is for internal use. Notification clients must
 *  pre-allocate a notify object when registering for a notification;
 *  Power_registerNotify() will take care initializing the internal elements
 *  appropriately.
 *
 *  @sa  @ref ti_drivers_Power_Examples_notify "Using power notify"
 */
typedef struct
{
    List_Elem link;            /*!< for placing on the notify list */
    uint_fast16_t eventTypes;  /*!< the event type */
    Power_NotifyFxn notifyFxn; /*!< notification function */
    uintptr_t clientArg;       /*!< argument provided by client */
} Power_NotifyObj;

/*!
 *  @brief      Power resource identifier
 */
typedef uint32_t Power_Resource;

/*!
 *  @brief  Disable the configured power policy from running when the CPU is
 *  idle
 *
 *  Calling this function clears the flag that controls whether the configured
 *  power policy function is invoked on each pass through the Idle loop.
 *  This function call will override both a 'true' setting of the
 *  "enablePolicy" setting in the Power Manager configuration object, as well
 *  as a previous runtime call to the Power_enablePolicy() function.
 *
 *  @return The old value of "enablePolicy".
 *
 *  @sa  Power_enablePolicy()
 *  @sa  @ref ti_drivers_Power_Examples_enable "Enabling power policy"
 *  @sa  @ref ti_drivers_Power_Examples_disable "Disabling power policy"
 */
bool Power_disablePolicy(void);

/*!
 *  @brief  Enable the configured power policy to run when the CPU is idle
 *
 *  Calling this function sets a flag that will cause the configured power
 *  policy function to be invoked on each pass through the Idle loop. This
 *  function call will override both a 'false' setting of the "enablePolicy"
 *  setting in the Power Manager configuration object, as well as a previous
 *  runtime call to the Power_disablePolicy() function.
 *
 *  For some processor families, automatic power transitions can make initial
 *  application development more difficult, as well as being at odds with
 *  basic debugger operation.  This convenience function allows an application
 *  to be initially configured, built, and debugged, without automatic power
 *  transitions during idle time.  When the application is found to be working,
 *  this function can be called (typically in main()) to enable the policy
 *  to run, without having to change the application configuration.
 *
 *  @sa  Power_disablePolicy()
 *  @sa  @ref ti_drivers_Power_Examples_enable "Enabling power policy"
 *  @sa  @ref ti_drivers_Power_Examples_disable "Disabling power policy"
 */
void Power_enablePolicy(void);

/*!
 *  @brief  Get the constraints that have been declared with Power
 *
 *  This function returns a bitmask indicating the constraints that are
 *  currently declared to the Power Manager (via previous calls to
 *  Power_setConstraint()).  For each constraint that is currently declared,
 *  the corresponding bit in the bitmask will be set.  For example, if two
 *  clients have independently declared two different constraints, the returned
 *  bitmask will have two bits set.
 *
 *  Constraint identifiers are device specific, and defined in the
 *  device-specific Power include file.  For example, the constraints for
 *  CC26XX are defined in PowerCC26XX.h.  The corresponding bit in the
 *  bitmask returned by this function can be derived by a left-shift using
 *  the constraint identifier.  For example, for CC26XX, for the corresponding
 *  bit for the PowerCC26XX_DISALLOW_STANDBY constraint, the bit position is
 *  determined by the operation: (1 << PowerCC26XX_DISALLOW_STANDBY)
 *
 *  @return A bitmask of the currently declared constraints.
 *
 *  @sa  Power_setConstraint()
 *  @sa  @ref ti_drivers_Power_Examples_constraint "Using power constraints"
 */
uint_fast32_t Power_getConstraintMask(void);

/*!
 *  @brief  Get the current dependency count for a resource
 *
 *  This function returns the number of dependencies that are currently
 *  declared upon a resource.
 *
 *  Resource identifiers are device specific, and defined in the
 *  device-specific Power include file.
 *
 *  @param[in]  resourceId  resource id
 *
 *  @return  The number of dependencies declared for the resource.
 *
 *  @retval  #Power_EINVALIDINPUT if the @p resourceId is invalid or this
 *  function is not supported by the device specific implementation.
 *
 *  @sa  Power_setDependency()
 *  @sa  @ref ti_drivers_Power_Examples_dependency "Using power dependency"
 */
int_fast16_t Power_getDependencyCount(Power_Resource resourceId);

/*!
 *  @brief  Get the current constraint count for an operational transition
 *
 *  This function returns the current number of constraints that is set on the
 *  given operational transition.
 *
 *  Constraint identifiers are device specific, and defined in the
 *  device-specific Power include file.  For example, the constraints for
 *  CC26XX are defined in PowerCC26XX.h, and to see how many constraints there
 *  currently are on entering standby, call this function with argument
 *  PowerCC26XX_DISALLOW_STANDBY
 *
 *  @param[in]  constraintId  constraint identifier
 *
 *  @return  The count for the given power constraint identifier
 *
 *  @retval  #Power_EINVALIDINPUT if the @p constraintId is invalid or this
 *  function is not supported by the device specific implementation.
 *
 *  @sa  Power_setConstraint()
 */
int_fast16_t Power_getConstraintCount(uint_fast16_t constraintId);

/*!
 *  @brief  Get the hardware transition latency for a sleep state
 *
 *  This function reports the minimal hardware transition latency for a specific
 *  sleep state.  The reported latency is that for a direct transition, and does
 *  not include any additional latency that might occur due to software-based
 *  notifications.
 *
 *  Sleep states are device specific, and defined in the device-specific Power
 *  include file.
 *
 *  This function is typically called by the power policy function. The latency
 *  is reported in units of microseconds.
 *
 *  @param[in]  sleepState  the sleep state
 *
 *  @param[in]  type    @ref Power_Latency_Type (#Power_TOTAL or #Power_RESUME)
 *
 *  @return The latency value, in units of microseconds.
 *
 *  @sa  @ref ti_drivers_Power_Examples_transistion "Power transitions"
 */
uint_fast32_t Power_getTransitionLatency(uint_fast16_t sleepState, uint_fast16_t type);

/*!
 *  @brief  Get the current transition state of the Power Manager
 *
 *  @return The current @ref Power_Transition_State.
 *
 *  @retval #Power_ACTIVE returned when no transitions are in progress.
 *
 *  @retval #Power_ENTERING_SLEEP returned during the transition to
 *  sleep, before sleep has occurred.
 *
 *  @retval #Power_EXITING_SLEEP returned after wakeup, as the device is
 *  being transitioned back to #Power_ACTIVE.
 *
 *  @sa  @ref ti_drivers_Power_Examples_transistion "Power transitions"
 */
uint_fast16_t Power_getTransitionState(void);

/*!
 *  @brief  Power function to be added to the application idle loop
 *
 *  This function should be added to the application idle loop. (The method to
 *  do this depends upon the operating system being used.)  This function
 *  will invoke the configured power policy function when appropriate. The
 *  specific policy function to be invoked is configured as the 'policyFxn'
 *  in the application-defined Power configuration object.
 *
 */
void Power_idleFunc(void);

/*!
 *  @brief  Power initialization function
 *
 *  This function initializes Power Manager internal state.
 *
 *  @warning The application is responsible for ensuring this function is
 *  called prior to any other Power API. Additionally, this function must be
 *  be called prior to any other TI-Driver's APIs. This function is normally
 *  called prior to any operating system initialization.
 *
 *  @return #Power_SOK
 */
int_fast16_t Power_init(void);

/*!
 *  @brief  Register a function to be called upon a specific power event
 *
 *  This function registers a function to be called when a Power event occurs.
 *  Registrations and the corresponding notifications are processed in
 *  first-in-first-out (FIFO) order. The function registered must behave as
 *  described later, below.
 *
 *  The pNotifyObj parameter is a pointer to a pre-allocated, opaque object
 *  that will be used by Power to support the notification.  This object could
 *  be dynamically allocated, or declared as a global object. This function
 *  will properly initialized the object's fields as appropriate; the caller
 *  just needs to provide a pointer to this pre-existing object.
 *
 *  The eventTypes parameter identifies the type of power event(s) for which
 *  the notify function being registered is to be called. (Event identifiers are
 *  device specific, and defined in the device-specific Power include file.
 *  For example, the events for CC26XX are defined in PowerCC26XX.h.)  The
 *  eventTypes parameter for this function call is treated as a bitmask, so
 *  multiple event types can be registered at once, using a common callback
 *  function.  For example, to call the specified notifyFxn when both
 *  the entering deepsleep and awake from deepsleep events occur, eventTypes
 *  should be specified as: PowerCC26XX_ENTERING_STANDBY |
 *  PowerCC26XX_AWAKE_STANDBY
 *
 *  The notifyFxn parameter specifies a callback function to be called when the
 *  specified Power event occurs. The notifyFxn must implement the following
 *  signature:
 *       status = notifyFxn(eventType, eventArg, clientArg);
 *
 *  Where: eventType identifies the event being signaled, eventArg is an
 *  optional event-specific argument, and clientArg is an arbitrary argument
 *  specified by the client at registration.  Note that multiple types of events
 *  can be specified when registering the notification callback function,
 *  but when the callback function is actually called by Power, only a
 *  single eventType will be specified for the callback (i.e., the current
 *  event).  The status returned by the client notification function must
 *  be one of the following constants: Power_NOTIFYDONE if the client processed
 *  the notification successfully, or Power_NOTIFYERROR if an error occurred
 *  during notification.
 *
 *  The clientArg parameter is an arbitrary, client-defined argument to be
 *  passed back to the client upon notification. This argument may allow one
 *  notify function to be used by multiple instances of a driver (that is, the
 *  clientArg can be used to identify the instance of the driver that is being
 *  notified).
 *
 *  @param[in]  pNotifyObj    #Power_NotifyObj preallocated by caller
 *
 *  @param[in]  eventTypes    event type or types
 *
 *  @param[in]  notifyFxn    client's #Power_NotifyFxn function
 *
 *  @param[in]  clientArg    client-specified argument to pass with
 *  notification
 *
 *  @retval  #Power_SOK on success.
 *
 *  @retval  #Power_EINVALIDPOINTER if either @p pNotifyObj or @p notifyFxn
 *  are NULL.
 *
 *  @sa  Power_unregisterNotify()
 *  @sa  @ref ti_drivers_Power_Examples_notify "Using power notify"
 */
int_fast16_t Power_registerNotify(Power_NotifyObj *pNotifyObj,
                                  uint_fast16_t eventTypes,
                                  Power_NotifyFxn notifyFxn,
                                  uintptr_t clientArg);

/*!
 *  @brief  Release a previously declared constraint
 *
 *  This function releases a constraint that was previously declared with
 *  Power_setConstraint().  For example, if a device driver is starting an I/O
 *  transaction and wants to prohibit activation of a sleep state during the
 *  transaction, it uses Power_setConstraint() to declare the constraint,
 *  before starting the transaction.  When the transaction completes, the
 *  driver calls this function to release the constraint, to allow the Power
 *  manager to once again allow transitions to sleep.
 *
 *  Constraint identifiers are device specific, and defined in the
 *  device-specific Power include file.  For example, the constraints for
 *  CC26XX are defined in PowerCC26XX.h.
 *
 *  Only one constraint can be specified with each call to this function; to
 *  release multiple constraints this function must be called multiple times.
 *
 *  It is critical that clients call Power_releaseConstraint() when operational
 *  constraints no longer exists. Otherwise, Power may be left unnecessarily
 *  restricted from activating power savings.
 *
 *  @pre  Power_setConstraint() must have been called first.
 *
 *  @param[in]  constraintId      constraint id
 *
 *  @return <b>CC26XX/CC13XX only</b>: #Power_SOK. To minimize code size
 *          asserts are used internally to check that the constraintId is
 *          valid,valid, and that the constraint count is not already zero;
 *          the function always returns #Power_SOK.
 *
 *  @return <b>All other devices</b>: #Power_SOK on success,
 *          #Power_EINVALIDINPUT if the constraintId is invalid, and
 *          #Power_EFAIL if the constraint count is already zero.
 *
 *  @sa  Power_setConstraint()
 *  @sa  @ref ti_drivers_Power_Examples_constraint "Using power constraints"
 */
int_fast16_t Power_releaseConstraint(uint_fast16_t constraintId);

/*!
 *  @brief  Release a previously declared dependency
 *
 *  This function releases a dependency that had been previously declared upon
 *  a resource (by a call to Power_setDependency()).
 *
 *  Resource identifiers are device specific, and defined in the
 *  device-specific Power include file.
 *
 *  @param[in]  resourceId      resource id
 *
 *  @return <b>CC26XX/CC13XX only</b>: #Power_SOK. To minimize code size
 *          asserts are used internally to check that the resourceId is valid,
 *          and that the resource reference count is not already zero;
 *          the function always returns #Power_SOK.
 *
 *  @return <b>All other devices</b>: #Power_SOK on success,
 *          #Power_EINVALIDINPUT if the resourceId is invalid, and #Power_EFAIL
 *          if the resource reference count is already zero.
 *
 *  @sa  Power_setDependency()
 *  @sa  @ref ti_drivers_Power_Examples_dependency "Using power dependency"
 */
int_fast16_t Power_releaseDependency(Power_Resource resourceId);

/*!
 *  @brief  Resets the system and causes it to reboot
 *
 *  This function causes the system to reset and then boot up again. The impact
 *  this has on the existing system state such as what memory is retained is
 *  device-specific. Unless otherwise specified in the device-specific
 *  documentation, this function will trigger the most comprehensive reset of
 *  the system triggerable from software.
 */
void Power_reset(void);

/*!
 *  @brief  Declare an operational constraint
 *
 *  Before taking certain actions, the Power Manager checks to see if the
 *  requested action would conflict with a client-declared constraint. If the
 *  action does conflict, Power will not proceed with the request.  This is the
 *  function that allows clients to declare their constraints with Power.
 *
 *  Constraint identifiers are device specific, and defined in the
 *  device-specific Power include file.  For example, the constraints for
 *  CC26XX are defined in PowerCC26XX.h.
 *
 *  Only one constraint can be specified with each call to this function; to
 *  declare multiple constraints this function must be called multiple times.
 *
 *  @param[in]  constraintId      constraint id
 *
 *  @return <b>CC26XX/CC13XX only</b>: #Power_SOK. To minimize code size an
 *          assert is used internally to check that the constraintId is valid;
 *          the function always returns #Power_SOK.
 *
 *  @return <b>All other devices</b>: #Power_SOK on success,
 *          #Power_EINVALIDINPUT if the constraintId is invalid.
 *
 *  @sa  Power_releaseConstraint()
 *  @sa  @ref ti_drivers_Power_Examples_constraint "Using power constraints"
 */
int_fast16_t Power_setConstraint(uint_fast16_t constraintId);

/*!
 *  @brief  Declare a dependency upon a resource
 *
 *  This function declares a dependency upon a resource. For example, if a
 *  UART driver needs a specific UART peripheral, it uses this function to
 *  declare this to the Power Manager.  If the resource had been inactive,
 *  then Power will activate the peripheral during this function call.
 *
 *  What is needed to make a peripheral resource 'active' will vary by device
 *  family. For some devices this may be a simple enable of a clock to the
 *  specified peripheral.  For others it may also require a power on of a
 *  power domain.  In either case, the Power Manager will take care of these
 *  details, and will also implement reference counting for resources and their
 *  interdependencies.  For example, if multiple UART peripherals reside in
 *  a shared serial power domain, the Power Manager will power up the serial
 *  domain when it is first needed, and then automatically power the domain off
 *  later, when all related dependencies for the relevant peripherals are
 *  released.
 *
 *  Resource identifiers are device specific, and defined in the
 *  device-specific Power include file.
 *
 *  @param[in]  resourceId      resource id
 *
 *  @return <b>CC26XX/CC13XX only</b>: #Power_SOK. To minimize code size an
 *          assert is used internally to check that the resourceId is valid;
 *          the function always returns #Power_SOK.
 *
 *  @return <b>All other devices</b>: #Power_SOK on success,
 *          #Power_EINVALIDINPUT if the reseourceId is invalid.
 *
 *  @sa  Power_releaseDependency()
 *  @sa  @ref ti_drivers_Power_Examples_dependency "Using power dependency"
 */
int_fast16_t Power_setDependency(Power_Resource resourceId);

/*!
 *  @brief  Set a new Power policy
 *
 *  This function allows a new #Power_PolicyFxn function to be selected at
 *  runtime.
 *
 *  @param[in]  policy      the new #Power_PolicyFxn function
 */
void Power_setPolicy(Power_PolicyFxn policy);

/*!
 *  @brief  Put the device into a shutdown state
 *
 *  This function will transition the device into a shutdown state. Before the
 *  actual transition is initiated, notifications will be sent to any clients
 *  who've registered (with Power_registerNotify()) for an 'entering shutdown'
 *  event.  The event name is device specific, and defined in the
 *  device-specific Power include file. Once notifications have been completed,
 *  the device shutdown will commence.
 *
 *  If the device is successfully transitioned to shutdown, this function
 *  call will never return.  Upon wakeup, the device and application will
 *  be rebooted (through a device reset).  If the transition is not
 *  successful, one of the error codes listed below will be returned.
 *
 *  On some devices a timed wakeup from shutdown can be specified, using
 *  the shutdownTime parameter.  This enables an autonomous application reboot
 *  at a future time.  For example, an application can go to shutdown, and then
 *  automatically reboot at a future time to do some work. And once that work
 *  is done, the application can shutdown again, for another timed interval.
 *  The time interval is specified via the shutdownTime parameter. (On devices
 *  that do not support this feature, any value specified for shutdownTime will
 *  be ignored.)  If the specified shutdownTime is zero, or otherwise less than
 *  the total shutdown latency for the device, the shutdownTime parameter will
 *  be ignored.  The shutdown latency for the device can be found in the
 *  device-specific Power include file.
 *
 *  @param[in]  shutdownState    the device-specific shutdown state
 *
 *  @param[in]  shutdownTime    the amount of time (in milliseconds) to keep
 *  the device in the shutdown state; this parameter is not supported on all
 *  device families.
 *
 *  @retval  #Power_ECHANGE_NOT_ALLOWED if a constraint is prohibiting
 *  shutdown.
 *
 *  @retval  #Power_EFAIL if an error occurred during client notifications.
 *
 *  @retval  #Power_EINVALIDINPUT if the shutdownState is invalid.
 *
 *  @retval  #Power_EBUSY if another transition is already in progress.
 */
int_fast16_t Power_shutdown(uint_fast16_t shutdownState, uint_fast32_t shutdownTime);

/*!
 *  @brief  Transition the device into a sleep state
 *
 *  This function is called from the power policy when it has made a decision
 *  to put the device in a specific sleep state.  This function returns to the
 *  caller (the policy function) once the device has awoken from sleep.
 *
 *  @warning This function must be called with interrupts disabled, and
 *  should not be called directly by the application, or by any drivers.
 *  This function does not check declared constraints; the policy function
 *  must check constraints before calling this function to initiate sleep.
 *
 *  @param[in]  sleepState    the sleep state
 *
 *  @retval  #Power_SOK on success, the device has slept and is awake again.
 *
 *  @retval  #Power_EFAIL if an error occurred during client notifications, or
 *  if a general failure occurred.
 *
 *  @retval  #Power_EINVALIDINPUT if the @p sleepState is invalid.
 *
 *  @retval  #Power_EBUSY if another transition is already in progress.
 */
int_fast16_t Power_sleep(uint_fast16_t sleepState);

/*!
 *  @brief  Unregister previously registered notifications
 *
 *  This function unregisters for event notifications that were previously
 *  registered with Power_registerNotify(). The caller must specify a pointer
 *  to the same notification object used during registration.
 *
 *  @param[in]  pNotifyObj    The #Power_NotifyObj used with the original
 *  call to Power_registerNotify()
 *
 *  @sa  Power_registerNotify()
 *  @sa  @ref ti_drivers_Power_Examples_notify "Using power notify"
 */
void Power_unregisterNotify(Power_NotifyObj *pNotifyObj);

#ifdef __cplusplus
}
#endif

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X0_CC26X0 || \
     DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X1_CC26X1 || \
     DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X2_CC26X2 || \
     DeviceFamily_PARENT == DeviceFamily_PARENT_CC13X4_CC26X3_CC26X4)
    #include <ti/drivers/power/PowerCC26XX.h>
#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X0)
    #include <ti/drivers/power/PowerCC23X0.h>
#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #include <ti/drivers/power/PowerCC27XX.h>
#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    #include <ti/drivers/power/PowerWFF3.h>
#endif

#endif /* ti_drivers_Power__include */
