#ifndef REGS_NU_VPE_H
#define REGS_NU_VPE_H

// #define NU_VPE_MYREG 0x000

// OFFSET NU_VPE & NU_VPE_RDMA -------------------
#define NU_VPE          (0x0 << 9)

// OFFSET DMA CHANNEL ----------------------------
//#define NU_VPE_RDMA     (0x1 << 9)
#define NU_VPE_SRC_RDMA ((0x1 << 9) + (0x0 << 7))
#define NU_VPE_OP0_RDMA ((0x1 << 9) + (0x1 << 7))
#define NU_VPE_OP1_RDMA ((0x1 << 9) + (0x2 << 7))
#define NU_VPE_OP2_RDMA ((0x1 << 9) + (0x3 << 7))

// VPE Registers address -------------------------
#define NU_VPE_STATUS                  0x000
#define NU_VPE_POINTER                 0x004

#define NU_VPE_OP_ENABLE               0x008
#define NU_VPE_DATA_W_OUT              0x00C
#define NU_VPE_DATA_H_OUT              0x010
#define NU_VPE_DATA_C_OUT              0x014
#define NU_VPE_WDMA_DST_BASE_L         0x018
#define NU_VPE_WDMA_DST_BASE_H         0x01C
#define NU_VPE_WDMA_DST_LINE_STRIDE    0x020
#define NU_VPE_WDMA_DST_VECTOR_STRIDE  0x024
#define NU_VPE_WDMA_DST_CFG            0x028

#define NU_VPE_OP_MODE                 0x02C
#define NU_VPE_OP0_CFG                 0x030
#define NU_VPE_OP0_ALU_CFG             0x034
#define NU_VPE_OP0_ALU_SRC_VAL         0x038
#define NU_VPE_OP0_MUL_CFG             0x03C
#define NU_VPE_OP0_MUL_SRC_VAL         0x040
#define NU_VPE_OP0_NORM_PARAM          0x044
#define NU_VPE_OP1_CFG                 0x048
#define NU_VPE_OP1_ALU_CFG             0x04C
#define NU_VPE_OP1_ALU_SRC_VAL         0x050
#define NU_VPE_OP1_MUL_CFG             0x054
#define NU_VPE_OP1_MUL_SRC_VAL         0x058
#define NU_VPE_OP1_NORM_PARAM          0x05C
#define NU_VPE_OP2_CFG                 0x060
#define NU_VPE_OP2_ALU_CFG             0x064
#define NU_VPE_OP2_ALU_SRC_VAL         0x068
#define NU_VPE_OP2_ALU_CVT_OFFSET_VAL  0x06C
#define NU_VPE_OP2_ALU_CVT_SCALE_VAL   0x070
#define NU_VPE_OP2_ALU_CVT_TRUNC_VAL   0x074
#define NU_VPE_OP2_ALU_CMP_RESULT      0x078
#define NU_VPE_OP2_MUL_CFG             0x07C
#define NU_VPE_OP2_MUL_SRC_VAL         0x080
#define NU_VPE_OP2_MUL_CVT_OFFSET_VAL  0x084
#define NU_VPE_OP2_MUL_CVT_SCALE_VAL   0x088
#define NU_VPE_OP2_MUL_CVT_TRUNC_VAL   0x08C
#define NU_VPE_OP2_NORM_PARAM          0x090
#define NU_VPE_LUT_ACCESS_CFG          0x094
#define NU_VPE_LUT_ACCESS_DATA         0x098
#define NU_VPE_LUT_CFG                 0x09C
#define NU_VPE_LUT_SYM_POINT           0x0A0
#define NU_VPE_LUT_TAB1_X_SHIFT        0x0A4
#define NU_VPE_LUT_TAB2_X_SHIFT        0x0A8
#define NU_VPE_LUT_TAB1_X_START        0x0AC
#define NU_VPE_LUT_TAB1_X_END          0x0B0
#define NU_VPE_LUT_TAB2_X_START        0x0B4
#define NU_VPE_LUT_TAB2_X_END          0x0B8
#define NU_VPE_LUT_TAB1_SLOPE_SCALE    0x0BC
#define NU_VPE_LUT_TAB1_SLOPE_SHIFT    0x0C0
#define NU_VPE_LUT_TAB2_SLOPE_SCALE    0x0C4
#define NU_VPE_LUT_TAB2_SLOPE_SHIFT    0x0C8
#define NU_VPE_OUT_CVT_OFFSET_VAL      0x0CC
#define NU_VPE_OUT_CVT_SCALE_VAL       0x0D0
#define NU_VPE_OUT_CVT_TRUNC_VAL       0x0D4
#define NU_VPE_INF_NUM_IN              0x0D8
#define NU_VPE_NAN_NUM_IN              0x0DC
#define NU_VPE_NAN_NUM_OUT             0x0E0
#define NU_VPE_SATUR_NUM_OUT           0x0E4

#define NU_VPE_WDMA_BOX_ST_SIZE_X      0x0E8
#define NU_VPE_WDMA_BOX_ST_SIZE_Y      0x0EC
#define NU_VPE_WDMA_BOX_ST_SIZE_Z      0x0F0
#define NU_VPE_WDMA_BOX_SIZE_X         0x0F4
#define NU_VPE_WDMA_BOX_SIZE_Y         0x0F8
#define NU_VPE_WDMA_BOX_SIZE_Z         0x0FC
#define NU_VPE_WDMA_BOX_OFFSET_SIZE_X  0x100
#define NU_VPE_WDMA_BOX_OFFSET_SIZE_Y  0x104
#define NU_VPE_WDMA_BOX_OFFSET_SIZE_Z  0x108
#define NU_VPE_WDMA_ELEM_STRIDE        0x10C
#define NU_VPE_CTRL_NC                 0x110
#define NU_VPE_AXI_PARAM               0x114
#define NU_VPE_CUBE_SIZE               0x118

// INTERRUPT Registers address -----------------------------------
#define  REG_VPE_IRQ_RD_ADDR           0x120
#define  REG_VPE_IRQ_CLR_ADDR          0x124
#define  REG_VPE_IRQ_SET_ADDR          0x128
#define  REG_VPE_IRQ_MASK_ADDR         0x12C

// WDMA additional registers address -----------------------------------
#define NU_VPE_WDMA_ELMT_LAST_SIZE_ADDR 0x130
#define NU_VPE_WDMA_ELMT_SIZE_ADDR      0x134
#define NU_VPE_WDMA_XYZ_DRCT_ADDR       0x138


// VPE RDMA Registers address --------------------
#define NU_VPE_RDMA_STATUS              0x000
#define NU_VPE_RDMA_POINTER             0x004
#define NU_VPE_RDMA_OP_ENABLE           0x008
#define NU_VPE_RDMA_DATA_W_IN           0x00C
#define NU_VPE_RDMA_DATA_H_IN           0x010
#define NU_VPE_RDMA_DATA_C_IN           0x014
#define NU_VPE_RDMA_CFG                 0x018
#define NU_VPE_RDMA_BASE_L              0x01C
#define NU_VPE_RDMA_BASE_H              0x020
#define NU_VPE_RDMA_LINE_STRIDE         0x024
#define NU_VPE_RDMA_VECTOR_STRIDE       0x028
#define NU_VPE_RDMA_BATCH_SIZE          0x02C
#define NU_VPE_RDMA_BATCH_STRIDE        0x030
#define NU_VPE_RDMA_BOX_ST_SIZE_X       0x034
#define NU_VPE_RDMA_BOX_ST_SIZE_Y       0x038
#define NU_VPE_RDMA_BOX_ST_SIZE_Z       0x03C
#define NU_VPE_RDMA_BOX_SIZE_X          0x040
#define NU_VPE_RDMA_BOX_SIZE_Y          0x044
#define NU_VPE_RDMA_BOX_SIZE_Z          0x048
#define NU_VPE_RDMA_BOX_OFFSET_SIZE_X   0x04C
#define NU_VPE_RDMA_BOX_OFFSET_SIZE_Y   0x050
#define NU_VPE_RDMA_BOX_OFFSET_SIZE_Z   0x054
#define NU_VPE_RDMA_ELEM_STRIDE         0x058
#define NU_VPE_RDMA_ELMT_LAST_SIZE_ADDR 0x060
#define NU_VPE_RDMA_ELMT_SIZE_ADDR      0x064
#define NU_VPE_RDMA_XYZ_DRCT_ADDR       0x068



#endif



