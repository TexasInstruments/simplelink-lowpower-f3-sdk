#ifndef __tracer_control_h___
#define __tracer_control_h___


#include <stdint.h>
#include <stdbool.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ioc.h)

extern void RCL_Tracer_enable(void);
extern void RCL_Tracer_disable(void);

#endif
