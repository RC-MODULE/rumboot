#ifndef REGS_NA_CU_H_
#define REGS_NA_CU_H_

#define NA_CU_UNITS_MODE 0x000

#define NA_CU_MPE_UNIT_MODE (1<<0)
#define NA_CU_VPE_UNIT_MODE (1<<1)
#define NA_CU_PPE_UNIT_MODE (1<<2)

#define NA_UNITS_MODE       0x000
#define NA_ID               0x004
#define NA_STAT             0x008
#define NA_PAUSE            0x00C
#define NA_SOFT_RESET       0x010
#define NA_PPE_SOFT_RESET   0x014
#define NA_VPE_SOFT_RESET   0x018
#define NA_MPE_SOFT_RESET   0x01C
#define NA_CMD_FIFO_STAT    0x020
#define NA_CMD_FIFO_LEVEL   0x024
#define NA_INT_UNITS_STATUS 0x028
#define NA_INT_UNITS_RESET  0x02C
#define NA_INT_UNITS_SET    0x030
#define NA_INT_UNITS_MASK   0x034
#define NA_INT_STATUS       0x038
#define NA_INT_RESET        0x03C
#define NA_INT_SET          0x040
#define NA_INT_MASK         0x044
#define AXI_INT_STATUS      0x048
#define AXI_INT_RESET       0x04C
#define AXI_INT_SET         0x050
#define AXI_INT_MASK        0x054

#endif
