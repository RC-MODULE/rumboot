#ifndef O32T_DEVICES_H
#define O32T_DEVICES_H

#include <platform/oi10/platform/devices.h>

#undef AXI32HSIFS_CTRL_BASE
#undef AXI64HSIFS_CTRL_BASE
#undef SCRB_BASE

#define IM3_BASE                0xC0060000
#define IM3_SIZE                (128 * 1024 * 1024)

#define MKIO2_BASE              0xC0021000
#define MKIO3_BASE              0xC0031000
#define AXI32_LEFT_CTRL_BASE    0xC0100000
#define AXI32_RIGHT_CTRL_BASE   0xC0200000
#define AXI64_CTRL_BASE         0xC0400000
#define COM0_BASE           	0xC0304000
#define COM1_BASE               0xC0305000
#define RCM_NDMA_BASE           0xC0306000
#define SCRB_BASE               0xC0307000


#define DCR_IIRQ_BASE           0x80091000
#define DCR_APBFIFO_BASE        0x80092000

#endif // !O32T_DEVICES_H