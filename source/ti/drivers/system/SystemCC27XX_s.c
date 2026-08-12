#include <stdbool.h>
#include "psa/service.h"

#include <ti/drivers/Power.h>
#include <ti/drivers/tfm/SecureCallback.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(driverlib/pmctl.h)
#include DeviceFamily_constructPath(driverlib/ckmd.h)

/* The range of pins available on this device
 * TODO: Make configurable
 */
const uint_least8_t GPIO_pinLowerBound = 0U;
const uint_least8_t GPIO_pinUpperBound = 28U;

/* SecureCallback config, used by SecureCallback_s_post() */
const SecureCallback_Config SecureCallback_config = {
    .intNumber   = INT_SW1,
    .intPriority = 0xFFU,
};

void SystemCC27XX_init(void);
void System_sp_main(void);

/*
 *  ======== SystemCC27XX_init ========
 */
void SystemCC27XX_init(void)
{
#if CONFIG_SYSTEM_OVERRIDE_CAP_ARRAY == 1
    /* Apply cap array configuration, as defined in config_tfm_project.h */
    CKMDSetInitialCapTrim(CONFIG_SYSTEM_CAP_ARRAY_Q1, CONFIG_SYSTEM_CAP_ARRAY_Q2);
    CKMDSetTargetCapTrim(CONFIG_SYSTEM_CAP_ARRAY_Q1, CONFIG_SYSTEM_CAP_ARRAY_Q2);
#endif

    /* Initialize power driver. Ignore return value since it never returns an
     * error.
     */
    (void)Power_init();

    /* TODO: Make LFCLK and voltage regulator configurable */
    PowerLPF3_selectLFXT();
    PMCTLSetVoltageRegulator(PMCTL_VOLTAGE_REGULATOR_DCDC);
}

/*
 *  ======== System_sp_main ========
 */
void System_sp_main(void)
{
    /* Dummy event loop.
     * Continually wait for input signals. Don't handle any.
     */
    while (true)
    {
        (void)psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
    }
}
