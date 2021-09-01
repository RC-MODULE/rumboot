
#include "regs_nu_mpe.h"
#include "regs_nu_vpe.h"
#include "regs_nu_ppe.h"

#define NA_CU_BYPASS_ACCESS_BASE (1<<13)
#define NA_CU_BYPASS_ACCESS_MPE_BASE (NA_CU_BYPASS_ACCESS_BASE + 0x0000)
#define NA_CU_BYPASS_ACCESS_VPE_BASE (NA_CU_BYPASS_ACCESS_BASE + 0x1000)
#define NA_CU_BYPASS_ACCESS_PPE_BASE (NA_CU_BYPASS_ACCESS_BASE + 0x1800)
