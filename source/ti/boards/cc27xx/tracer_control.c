
#include <ti/drivers/rcl/LRF.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_lrfddbell.h)
#include DeviceFamily_constructPath(inc/hw_lrfdtrc.h)

#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_clkctl.h)
#include DeviceFamily_constructPath(inc/hw_ioc.h)


void RCL_Tracer_enable(void)
{
    /* Enable tracer clock */
    LRF_setAppClockEnable(LRFDDBELL_CLKCTL_TRC_M);

    /* Enable the Tracer */
    HWREG(LRFDTRC_BASE + LRFDTRC_O_CFG) = LRFDTRC_CFG_CH1EN_NORM |
                                          LRFDTRC_CFG_CH2EN_TOPSM |
                                          LRFDTRC_CFG_CH3EN_TOPSM |
                                          LRFDTRC_CFG_TSEN_M |
                                          LRFDTRC_CFG_PRESCAL_DIV1;

#ifdef DeviceFamily_CC23X0R2
    /* Set tracer on GPO6 */
    HWREG(LRFDDBELL_BASE + LRFDDBELL_O_GPOSEL0) |= (LRFDDBELL_GPOSEL0_SRC2_RFCTRC);
    HWREG(IOC_BASE + IOC_O_IOC6) = IOC_IOC6_PORTCFG_PFUNC4; // LRFD2 [trace]
#elif defined(DeviceFamily_CC23X0R5)
    /* Set tracer on GPO5 */
    HWREG(LRFDDBELL_BASE + LRFDDBELL_O_GPOSEL1) |= LRFDDBELL_GPOSEL1_SRC6_RFCTRC;
    HWREG(IOC_BASE + IOC_O_IOC5) = IOC_IOC5_PORTCFG_PFUNC3; // LRFD6 [trace]
#elif defined (DeviceFamily_CC27XX)
    /* Set tracer on GPO20 */
    HWREG(LRFDDBELL_BASE + LRFDDBELL_O_GPOSEL0) |= (LRFDDBELL_GPOSEL0_SRC3_RFCTRC);
    HWREG(IOC_BASE + IOC_O_IOC20) = IOC_IOC20_PORTCFG_PFUNC5; // LRFD3 [trace]
#endif
}

void RCL_Tracer_disable(void)
{
#ifdef DeviceFamily_CC23X0R2
    HWREG(IOC_BASE + IOC_O_IOC6) = 0;
#elif defined(DeviceFamily_CC23X0R5)
    HWREG(IOC_BASE + IOC_O_IOC5) = 0;
#elif defined (DeviceFamily_CC27XX)
    HWREG(IOC_BASE + IOC_O_IOC20) = 0;
#endif

    /* Disable the tracer */
    HWREG(LRFDTRC_BASE + LRFDTRC_O_CFG) = (LRFDTRC_CFG_CH1EN_OFF) |
                (LRFDTRC_CFG_CH2EN_OFF) |
                (LRFDTRC_CFG_CH3EN_OFF) |
                (LRFDTRC_CFG_TSEN_OFF);

    /* Disable tracer clock */
    LRF_clearAppClockEnable(LRFDDBELL_CLKCTL_TRC_M);
}
