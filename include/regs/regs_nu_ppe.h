#ifndef REGS_NU_PPE_H
#define REGS_NU_PPE_H

// RDMA registers
#define NU_PPE_RDMA_STATUS              0X000
#define NU_PPE_RDMA_OP_ENABLE           0X004
#define NU_PPE_RDMA_PLANE_W_IN          0X008
#define NU_PPE_RDMA_PLANE_H_IN          0X00C
#define NU_PPE_RDMA_PLANE_C_IN          0X010
#define NU_PPE_RDMA_SRC_BASE_ADDR_L     0X014
#define NU_PPE_RDMA_SRC_BASE_ADDR_H     0X018
#define NU_PPE_RDMA_SRC_VECTOR_STRIDE   0X01C
#define NU_PPE_RDMA_SRC_LINE_STRIDE     0X020
#define NU_PPE_RDMA_SRC_ELEMENT_STRIDE  0X024
#define NU_PPE_RDMA_OP_MODE             0X028
#define NU_PPE_RDMA_BOX_SIZE_W_IN       0X02C
#define NU_PPE_RDMA_BOX_SIZE_H_IN       0X030
#define NU_PPE_RDMA_BOX_SIZE_C_IN       0X034
#define NU_PPE_RDMA_BOX_START_W_IN      0X038
#define NU_PPE_RDMA_BOX_OFFSET_W_IN     0X03C
// #define NU_PPE_RDMA_KERNEL              0X040

// PPE+WDMA registers
#define NU_PPE_STATUS                   0X000
#define NU_PPE_OP_ENABLE                0X004
#define NU_PPE_DATA_W_IN                0X008
#define NU_PPE_DATA_H_IN                0X00C
#define NU_PPE_DATA_C_IN                0X010
#define NU_PPE_DATA_W_OUT               0X014
#define NU_PPE_DATA_H_OUT               0X018
#define NU_PPE_DATA_C_OUT               0X01C
#define NU_PPE_PLANE_W_OUT              0X020
#define NU_PPE_PLANE_H_OUT              0X024
#define NU_PPE_PLANE_C_OUT              0X028
#define NU_PPE_DST_BASE_ADDR_L          0X02C
#define NU_PPE_DST_BASE_ADDR_H          0X030
#define NU_PPE_DST_VECTOR_STRIDE        0X034
#define NU_PPE_DST_LINE_STRIDE          0X038
#define NU_PPE_DST_ELEMENT_STRIDE       0X03C
#define NU_PPE_OP_MODE                  0X040
#define NU_PPE_BOX_SIZE_W_IN            0X044
#define NU_PPE_BOX_SIZE_H_IN            0X048
#define NU_PPE_BOX_SIZE_C_IN            0X04C
#define NU_PPE_BOX_START_W_IN           0X050
#define NU_PPE_BOX_OFFSET_W_IN          0X054
#define NU_PPE_BOX_SIZE_W_OUT           0X058
#define NU_PPE_BOX_SIZE_H_OUT           0X05C
#define NU_PPE_BOX_SIZE_C_OUT           0X060
#define NU_PPE_BOX_START_W_OUT          0X064
#define NU_PPE_BOX_OFFSET_W_OUT         0X068
#define NU_PPE_KERNEL                   0X06C
#define NU_PPE_RECIP_KERNEL_W           0X070
#define NU_PPE_RECIP_KERNEL_H           0X074
#define NU_PPE_PADDING                  0X078
#define NU_PPE_PADDING_VALUE_1          0X07C
#define NU_PPE_PADDING_VALUE_2          0X080
#define NU_PPE_PADDING_VALUE_3          0X084
#define NU_PPE_PADDING_VALUE_4          0X088
#define NU_PPE_PADDING_VALUE_5          0X08C
#define NU_PPE_PADDING_VALUE_6          0X090
#define NU_PPE_PADDING_VALUE_7          0X094
#define NU_PPE_INF_NUM_IN               0X098
#define NU_PPE_NAN_NUM_IN               0X09C
#define NU_PPE_NAN_NUM_OUT              0X0A0
#define NU_PPE_STATUS_DONE              0X0A4

#endif
