#ifndef NU_TEST_MACRO_H_
#define NU_TEST_MACRO_H_
  // Macros To Distinguish DUTs In SCR1 Environment
  // The DUT Macro Is Set From scr1.cmake
#include <regs/regs_na.h>
#include <platform/devices.h>

#ifndef DUT
  #define DUT 'N'
#endif

#define DUT_IS_NPE ( DUT == 'N' )
#define DUT_IS_MPE ( DUT == 'M' )
#define DUT_IS_VPE ( DUT == 'V' )
#define DUT_IS_PPE ( DUT == 'P' )

#if DUT_IS_VPE
  #define MY_VPE_REGS_BASE NU_VPE_STANDALONE_BASE
#elif DUT_IS_PPE
  #define MY_PPE_RDMA_BASE NU_PPE_STANDALONE_RDMA_BASE
  #define MY_PPE_REGS_BASE NU_PPE_STANDALONE_WDMA_BASE
#else // DUT_IS_NPE By Default
  #define MY_MPE_REGS_BASE (NPE_BASE + NA_MPE_BASE)
  #define MY_VPE_REGS_BASE (NPE_BASE + NA_VPE_BASE)
  #define MY_PPE_RDMA_BASE (NPE_BASE + NA_PPE_RDMA_BASE)
  #define MY_PPE_REGS_BASE (NPE_BASE + NA_PPE_WDMA_BASE)
#endif

#endif
