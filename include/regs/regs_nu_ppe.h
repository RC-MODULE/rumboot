#ifndef REGS_NU_PPE_H
#define REGS_NU_PPE_H

// RDMA registers
#define NU_PPE_RDMA_STATUS              0X000
#define NU_PPE_RDMA_POINTER             0X004
#define NU_PPE_RDMA_OP_ENABLE           0X008
#define NU_PPE_RDMA_INT_STATUS          0X010
#define NU_PPE_RDMA_INT_RESET           0X014
#define NU_PPE_RDMA_INT_SET     		0X018
#define NU_PPE_RDMA_INT_MASK		    0X018
#define NU_PPE_RDMA_BASE_ADDR 			0X020
#define NU_PPE_RDMA_BORDER_X     		0X030
#define NU_PPE_RDMA_BORDER_Y  			0X034
#define NU_PPE_RDMA_BORDER_Z            0X038
#define NU_PPE_RDMA_STRIDE_X	        0X03C
#define NU_PPE_RDMA_STRIDE_Y       		0X040
#define NU_PPE_RDMA_STRIDE_Z       		0X044
#define NU_PPE_RDMA_FRAG_SIZE      		0X048
#define NU_PPE_RDMA_FRAG_LAST_SIZE     	0X04C
#define NU_PPE_RDMA_XYZ_DRCT       		0X050
#define NU_PPE_RDMA_BOX_ST_SIZE_X       0X054
#define NU_PPE_RDMA_BOX_ST_SIZE_Y       0X058
#define NU_PPE_RDMA_BOX_ST_SIZE_Z       0X05C
#define NU_PPE_RDMA_BOX_SIZE_X       	0X060
#define NU_PPE_RDMA_BOX_SIZE_Y       	0X064
#define NU_PPE_RDMA_BOX_SIZE_Z       	0X068
#define NU_PPE_RDMA_BOX_OFFSET_X       	0X06C
#define NU_PPE_RDMA_BOX_OFFSET_Y       	0X070
#define NU_PPE_RDMA_BOX_OFFSET_Z       	0X074

// PPE+WDMA registers
#define NU_PPE_STATUS                   0X000
#define NU_PPE_POINTER                  0X004
#define NU_PPE_OP_ENABLE                0X008
#define NU_PPE_WDMA_INT_STATUS          0X010
#define NU_PPE_WDMA_INT_RESET           0X014
#define NU_PPE_WDMA_INT_SET           	0X018
#define NU_PPE_WDMA_INT_MASK            0X01C
#define NU_PPE_WDMA_BASE_ADDR           0X020
#define NU_PPE_WDMA_BORDER_X            0X030
#define NU_PPE_WDMA_BORDER_Y            0X034
#define NU_PPE_WDMA_BORDER_Z            0X038
#define NU_PPE_WDMA_STRIDE_X	        0X03C
#define NU_PPE_WDMA_STRIDE_Y       		0X040
#define NU_PPE_WDMA_STRIDE_Z       		0X044
#define NU_PPE_WDMA_FRAG_SIZE      		0X048
#define NU_PPE_WDMA_FRAG_LAST_SIZE     	0X04C
#define NU_PPE_WDMA_XYZ_DRCT       		0X050
#define NU_PPE_WDMA_BOX_ST_SIZE_X       0X054
#define NU_PPE_WDMA_BOX_ST_SIZE_Y       0X058
#define NU_PPE_WDMA_BOX_ST_SIZE_Z       0X05C
#define NU_PPE_WDMA_BOX_SIZE_X       	0X060
#define NU_PPE_WDMA_BOX_SIZE_Y       	0X064
#define NU_PPE_WDMA_BOX_SIZE_Z       	0X068
#define NU_PPE_WDMA_BOX_OFFSET_X       	0X06C
#define NU_PPE_WDMA_BOX_OFFSET_Y       	0X070
#define NU_PPE_WDMA_BOX_OFFSET_Z       	0X074

#define NU_PPE_INP_BOX_ST_SIZE_X       	0X078
#define NU_PPE_INP_BOX_SIZE_X       	0X07C
#define NU_PPE_INP_BOX_OFFSET_X       	0X080

#define NU_PPE_DATA_W_IN       			0X084
#define NU_PPE_DATA_H_IN       			0X088
#define NU_PPE_DATA_C_IN       			0X08C
#define NU_PPE_DATA_W_OUT       		0X090
#define NU_PPE_DATA_H_OUT       		0X094
#define NU_PPE_DATA_C_OUT       		0X098
#define NU_PPE_OP_MODE       			0X09C
#define NU_PPE_KERNEL       			0X0A0
#define NU_PPE_RECIP_KERNEL_W       	0X0A4
#define NU_PPE_RECIP_KERNEL_H       	0X0A8

#define NU_PPE_PADDING       			0X0AC
#define NU_PPE_PADDING_VALUE_1       	0X0B0
#define NU_PPE_PADDING_VALUE_2       	0X0B4
#define NU_PPE_PADDING_VALUE_3       	0X0B8
#define NU_PPE_PADDING_VALUE_4       	0X0BC
#define NU_PPE_PADDING_VALUE_5       	0X0C0
#define NU_PPE_PADDING_VALUE_6       	0X0C4
#define NU_PPE_PADDING_VALUE_7       	0X0C8
#define NU_PPE_INF_NUM_IN       		0X0CC
#define NU_PPE_NAN_NUM_IN       		0X0D0
#define NU_PPE_NAN_NUM_OUT       		0X0D4
#define NU_PPE_STATUS_DONE       		0X0D8
#endif
