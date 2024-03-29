#ifndef REGS_NU_PPE_H
#define REGS_NU_PPE_H

// RDMA registers
#define NU_PPE_OP_ENABLE            0X000
#define NU_PPE_POINTER              0X004
#define NU_PPE_STATUS               0X008
#define NU_PPE_RDMA_AXI_PARAM       0X00C
#define NU_PPE_RDMA_INT_STATUS      0X010
#define NU_PPE_RDMA_INT_RESET       0X014
#define NU_PPE_RDMA_INT_SET         0X018
#define NU_PPE_RDMA_INT_MASK        0X01C
#define NU_PPE_RDMA_BASE_ADDR       0X020
#define NU_PPE_RDMA_BORDER_X        0X030
#define NU_PPE_RDMA_BORDER_Y        0X034
#define NU_PPE_RDMA_BORDER_Z        0X038
#define NU_PPE_RDMA_STRIDE_X        0X03C
#define NU_PPE_RDMA_STRIDE_Y        0X040
#define NU_PPE_RDMA_STRIDE_Z        0X044
#define NU_PPE_RDMA_FRAG_SIZE       0X048
#define NU_PPE_RDMA_FRAG_LAST_SIZE  0X04C
#define NU_PPE_RDMA_XYZ_DRCT        0X050
#define NU_PPE_RDMA_BOX_ST_SIZE_X   0X054
#define NU_PPE_RDMA_BOX_ST_SIZE_Y   0X058
#define NU_PPE_RDMA_BOX_ST_SIZE_Z   0X05C
#define NU_PPE_RDMA_BOX_SIZE_X      0X060
#define NU_PPE_RDMA_BOX_SIZE_Y      0X064
#define NU_PPE_RDMA_BOX_SIZE_Z      0X068
#define NU_PPE_RDMA_BOX_OFFSET_X    0X06C
#define NU_PPE_RDMA_BOX_OFFSET_Y    0X070
#define NU_PPE_RDMA_BOX_OFFSET_Z    0X074
  // Size Of Reg Map - Diff Between Last Reg And First Reg
#define NU_PPE_RDMA_REG_MAP_SIZE (NU_PPE_RDMA_BOX_OFFSET_Z - NU_PPE_OP_ENABLE)

// PPE+WDMA registers
//#define NU_PPE_OP_ENABLE           0x000 // Duplicate (Defined Higher)
//#define NU_PPE_POINTER             0x004
//#define NU_PPE_STATUS              0x008
#define NU_PPE_WDMA_AXI_PARAM       0x00C
#define NU_PPE_WDMA_INT_STATUS      0x010
#define NU_PPE_WDMA_INT_RESET       0x014
#define NU_PPE_WDMA_INT_SET         0x018
#define NU_PPE_WDMA_INT_MASK        0x01C
#define NU_PPE_WDMA_BASE_ADDR       0x020
#define NU_PPE_WDMA_BORDER_X        0x030
#define NU_PPE_WDMA_BORDER_Y        0x034
#define NU_PPE_WDMA_BORDER_Z        0x038
#define NU_PPE_WDMA_STRIDE_X        0x03C
#define NU_PPE_WDMA_STRIDE_Y        0x040
#define NU_PPE_WDMA_STRIDE_Z        0x044
#define NU_PPE_WDMA_FRAG_SIZE       0x048
#define NU_PPE_WDMA_FRAG_LAST_SIZE  0x04C
#define NU_PPE_WDMA_XYZ_DRCT        0x050
#define NU_PPE_WDMA_BOX_ST_SIZE_X   0x054
#define NU_PPE_WDMA_BOX_ST_SIZE_Y   0x058
#define NU_PPE_WDMA_BOX_ST_SIZE_Z   0x05C
#define NU_PPE_WDMA_BOX_SIZE_X      0x060
#define NU_PPE_WDMA_BOX_SIZE_Y      0x064
#define NU_PPE_WDMA_BOX_SIZE_Z      0x068
#define NU_PPE_WDMA_BOX_OFFSET_X    0x06C
#define NU_PPE_WDMA_BOX_OFFSET_Y    0x070
#define NU_PPE_WDMA_BOX_OFFSET_Z    0x074
#define NU_PPE_INP_BOX_ST_SIZE_X    0x078
#define NU_PPE_INP_BOX_SIZE_X       0x07C
#define NU_PPE_INP_BOX_OFFSET_X     0x080
#define NU_PPE_DATA_W_IN            0x084
#define NU_PPE_DATA_H_IN            0x088
#define NU_PPE_DATA_C_IN            0x08C
#define NU_PPE_DATA_W_OUT           0x090
#define NU_PPE_DATA_H_OUT           0x094

#define NU_PPE_OP_MODE              0x09C
#define NU_PPE_KERNEL               0x0A0

#define NU_PPE_RECIP_KERNEL_WH      0x0A4

#define NU_PPE_PADDING              0x0AC
#define NU_PPE_PADDING_VALUE_1      0x0B0
#define NU_PPE_PADDING_VALUE_2      0x0B4
#define NU_PPE_PADDING_VALUE_3      0x0B8
#define NU_PPE_PADDING_VALUE_4      0x0BC
#define NU_PPE_PADDING_VALUE_5      0x0C0
#define NU_PPE_PADDING_VALUE_6      0x0C4
#define NU_PPE_PADDING_VALUE_7      0x0C8
#define NU_PPE_INF_NUM_IN           0x0CC
#define NU_PPE_NAN_NUM_IN           0x0D0
#define NU_PPE_NAN_NUM_OUT          0x0D4
#define NU_PPE_STATUS_DONE          0x0D8
  // Diff Between Last Meaningful Reg And First Reg
#define NU_PPE_WDMA_REG_MAP_SIZE (NU_PPE_PADDING_VALUE_7 - NU_PPE_OP_ENABLE + 4)
#endif
