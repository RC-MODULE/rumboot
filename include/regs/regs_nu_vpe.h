#ifndef REGS_NU_VPE_H
#define REGS_NU_VPE_H

// #define NU_VPE_MYREG 0x000

// OFFSET NU_VPE & NU_VPE_RDMA -------------------
#define NU_VPE          (0x0 << 9)

// OFFSET DMA CHANNEL ----------------------------
#define NU_VPE_DST_WDMA  0x300
#define NU_VPE_SRC_RDMA  0x400
#define NU_VPE_OP0_RDMA  0x500
#define NU_VPE_OP1_RDMA  0x600
#define NU_VPE_OP2_RDMA  0x700

// VPE Registers address -------------------------
#define NU_VPE_NEXT_CNTX               0x000
#define NU_VPE_DEV_ID                  0x004
#define NU_VPE_SOFT_RESET              0x008
#define NU_VPE_CUBE_SIZE               0x00C
#define NU_VPE_INT_STATUS              0x010
#define NU_VPE_INT_RESET               0x014
#define NU_VPE_INT_SET                 0x018
#define NU_VPE_INT_MASK                0x01C
#define NU_VPE_CUBE_SIZE_C             0x020
#define NU_VPE_CUBE_SIZE_W             0x024
#define NU_VPE_CUBE_SIZE_H             0x028
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
#define NU_VPE_LUT_XOFFSET             0x0A0
#define NU_VPE_LUT_YOFFSET             0x0A4
#define NU_VPE_LUT_TAB0_XSHIFT         0x0A8
#define NU_VPE_LUT_TAB1_XSHIFT         0x0AC
#define NU_VPE_LUT_TAB0_XMIN           0x0B0
#define NU_VPE_LUT_TAB0_XMAX           0x0B4
#define NU_VPE_LUT_TAB1_XMIN           0x0B8
#define NU_VPE_LUT_TAB1_XMAX           0x0BC
#define NU_VPE_LUT_TAB0_SLOPE_SCALE    0x0C0
#define NU_VPE_LUT_TAB0_SLOPE_SHIFT    0x0C4
#define NU_VPE_LUT_TAB1_SLOPE_SCALE    0x0C8
#define NU_VPE_LUT_TAB1_SLOPE_SHIFT    0x0CC
#define NU_VPE_OUT_CVT_OFFSET_VAL      0x0D0
#define NU_VPE_OUT_CVT_SCALE_VAL       0x0D4
#define NU_VPE_OUT_CVT_TRUNC_VAL       0x0D8
#define NU_VPE_INF_NUM_IN              0x0DC
#define NU_VPE_NAN_NUM_IN              0x0E0
#define NU_VPE_NAN_NUM_OUT             0x0E4
#define NU_VPE_SATUR_NUM_OUT           0x0E8
#define LUT_CNT_OVRF                   0x0EC
#define LUT_CNT_UNDF                   0x0F0
#define LUT_CNT_HIT_TAB0               0x0F4
#define LUT_CNT_HIT_TAB1               0x0F8
#define LUT_CNT_HIT_HYBR               0x0FC


// VPE RDMA Registers address --------------------
#define NU_VPE_DMA_CFG                 0x000
#define NU_VPE_DMA_POINTER             0x004
#define NU_VPE_DMA_AXI_PARAM           0x00C
#define NU_VPE_INT_STATUS              0x010     
#define NU_VPE_INT_RESET               0x014     
#define NU_VPE_INT_SET                 0x018  
#define NU_VPE_INT_MASK                0x01C 
#define NU_VPE_DMA_BASE                0x020
#define NU_VPE_DMA_CUBE_SIZE_C         0x024
#define NU_VPE_DMA_CUBE_SIZE_W         0x028
#define NU_VPE_DMA_CUBE_SIZE_H         0x02C
#define NU_VPE_DMA_BORDER_X            0x030
#define NU_VPE_DMA_BORDER_Y            0x034
#define NU_VPE_DMA_BORDER_Z            0x038
#define NU_VPE_DMA_STRIDE_X            0x03C
#define NU_VPE_DMA_STRIDE_Y            0x040
#define NU_VPE_DMA_STRIDE_Z            0x044
#define NU_VPE_DMA_FRAG_SIZE_ADDR      0x048
#define NU_VPE_DMA_FRAG_LAST_SIZE_ADDR 0x04C
#define NU_VPE_DMA_XYZ_DRCT_ADDR       0x050
#define NU_VPE_DMA_BOX_ST_SIZE_X       0x054
#define NU_VPE_DMA_BOX_ST_SIZE_Y       0x058
#define NU_VPE_DMA_BOX_ST_SIZE_Z       0x05C
#define NU_VPE_DMA_BOX_SIZE_X          0x060
#define NU_VPE_DMA_BOX_SIZE_Y          0x064
#define NU_VPE_DMA_BOX_SIZE_Z          0x068
#define NU_VPE_DMA_BOX_OFFSET_SIZE_X   0x06C
#define NU_VPE_DMA_BOX_OFFSET_SIZE_Y   0x070
#define NU_VPE_DMA_BOX_OFFSET_SIZE_Z   0x074
#define NU_VPE_DMA_BATCH_SIZE          0x078
#define NU_VPE_DMA_BATCH_STRIDE        0x07C

  // Diff Between The Offset Of The Last Group And Offset Of The First Group
  //  + The Size Of The Last Group
#define NU_VPE_REG_MAP_SIZE ( (NU_VPE_OP2_RDMA - NU_VPE) + (NU_VPE_DMA_BATCH_STRIDE - NU_VPE_DMA_CFG + 4) )

#endif



