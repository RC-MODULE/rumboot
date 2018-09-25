#ifndef SCTL__
#define SCTL__

#define SCTL_BOOTM    0x0
/* --- CAN --- */
#define SCTL_CAN0     0x030
#define SCTL_CAN1     0x034
#define SCTL_BISR_L2C 0x038
#define SCTL_BISR_CLK_EN 0x03c

//SCTL REG
#define SCTL_START_i 0
#define SCTL_PROG_START_i 1
#define SCTL_FINISH_i 2
#define SCTL_RSLT_i 3
#define SCTL_REDOK_i 4

#endif
