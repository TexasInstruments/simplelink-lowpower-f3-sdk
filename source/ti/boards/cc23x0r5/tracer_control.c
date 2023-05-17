
#include <ti/drivers/rcl/LRF.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/dbell_regs.h)
#include DeviceFamily_constructPath(driverlib/trc_regs.h)

#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_clkctl.h)
#include DeviceFamily_constructPath(inc/hw_ioc.h)


void RCL_Tracer_enable(void)
{
    /* Enable tracer clock */
    LRF_setAppClockEnable(DBELL_CLKCTL_TRC_BM);

    /* Enable the Tracer */
    S_TRC_CFG  = (TRC_CFG_CH1EN_NORM << TRC_CFG_CH1EN) |
        (TRC_CFG_CH2EN_TOPSM << TRC_CFG_CH2EN) |
        (TRC_CFG_CH3EN_TOPSM << TRC_CFG_CH3EN) |
        TRC_CFG_TSEN_BM |
        (TRC_CFG_PRESCAL_DIV1 << TRC_CFG_PRESCAL);

#if defined(SOCFPGA) || defined(DeviceFamily_CC23X0R2)
    /* Set tracer on GPO6 */
    S_DBELL_GPOSEL0 = (S_DBELL_GPOSEL0 & ~DBELL_GPOSEL0_SRC2_BM) | (DBELL_GPOSEL0_SRC2_RFCTRC << DBELL_GPOSEL0_SRC2);
    HWREG(IOC_BASE + IOC_O_IOC6) = IOC_IOC6_PORTCFG_PFUNC4; // LRFD2 [trace]
#else
    /* Set tracer on GPO5 */
    S_DBELL_GPOSEL1 = DBELL_GPOSEL1_SRC6_RFCTRC << DBELL_GPOSEL1_SRC6;
    HWREG(IOC_BASE + IOC_O_IOC5) = IOC_IOC5_PORTCFG_PFUNC3; // LRFD6 [trace]
#endif
}

void RCL_Tracer_disable(void)
{
    HWREG(IOC_BASE + IOC_O_IOC6) = 0;

    /* Disable the tracer */
    S_TRC_CFG  = (TRC_CFG_CH1EN_OFF << TRC_CFG_CH1EN) |
        (TRC_CFG_CH2EN_OFF << TRC_CFG_CH2EN) |
        (TRC_CFG_CH3EN_OFF << TRC_CFG_CH3EN) |
        (TRC_CFG_TSEN_OFF << TRC_CFG_TSEN);

    /* Disable tracer clock */
    LRF_clearAppClockEnable(DBELL_CLKCTL_TRC_BM);
}
