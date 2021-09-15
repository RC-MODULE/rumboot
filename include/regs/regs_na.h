
#ifndef REGS_NA_H_
#define REGS_NA_H_

#include "regs_nu_mpe.h"
#include "regs_nu_vpe.h"
#include "regs_nu_ppe.h"
#include "regs_na_cu.h"

#define NA_CU_REGS_BASE (1<<13)
#define NA_MPE_BASE 0x0000
#define NA_VPE_BASE 0x1000

#define NA_PPE_RDMA_BASE 0x00001800
#define NA_PPE_WDMA_BASE 0x00001C00

#define MPE_RDMA_D_BASE 0x0000
#define MPE_RDMA_W_BASE 0x0400
#define MPE_MA_BASE     0x0800

#endif
