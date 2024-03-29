#ifndef REGS_NA_CU_H_
#define REGS_NA_CU_H_

#define NA_CU_MPE_UNIT_MODE (1<<0)
#define NA_CU_VPE_UNIT_MODE (1<<1)
#define NA_CU_PPE_UNIT_MODE (1<<2)

#define NA_UNITS_MODE       	0x000
#define NA_ID               	0x004
#define NA_STAT             	0x008
#define NA_PAUSE            	0x00C
#define NA_SOFT_RESET       	0x010
#define NA_PPE_SOFT_RESET   	0x014
#define NA_VPE_SOFT_RESET   	0x018
#define NA_MPE_SOFT_RESET   	0x01C
#define NA_CMD_FIFO_STAT    	0x020
#define NA_MPE_D_CMD_FIFO_STAT  0x024
#define NA_MPE_W_CMD_FIFO_STAT  0x028
#define NA_MPE_MA_CMD_FIFO_STAT 0x02C
#define NA_VPE_CMD_FIFO_STAT  	0x030
#define NA_PPE_R_CMD_FIFO_STAT 	0x034
#define NA_PPE_W_CMD_FIFO_STAT 	0x038
#define NA_CMD_FIFO_LEVEL 		0x03C
#define NA_INT_UNITS_STATUS 	0x040
#define NA_INT_UNITS_RESET  	0x044
#define NA_INT_UNITS_SET    	0x048
#define NA_INT_UNITS_MASK   	0x04C
#define NA_INT_STATUS       	0x050
#define NA_INT_RESET        	0x054
#define NA_INT_SET          	0x058
#define NA_INT_MASK         	0x05C
#define NA_INT_AXI_STATUS      	0x060
#define NA_INT_AXI_RESET       	0x064
#define NA_INT_AXI_SET         	0x068
#define NA_INT_AXI_MASK        	0x06C
#define NA_TIMER_CTRL           0x070
#define NA_TIMER_L              0x074
#define NA_TIMER_H              0x078
#define CMD_DMA_CFG             0x080
#define CMD_DMA_POINTER         0x084
#define CMD_DMA_AXI_PARAM       0x08C
#define CMD_DMA_INT_STATUS      0x090
#define CMD_DMA_INT_RESET       0x094
#define CMD_DMA_INT_SET         0x098
#define CMD_DMA_INT_MASK        0x09C
#define CMD_DMA_BASE_ADDR       0x0A0
#define CMD_DMA_PAGE_SIZE       0x0CC

#endif
