#ifndef __MPIC128_REG_ACCESS_H__
#define __MPIC128_REG_ACCESS_H__


#include <platform/oi10/platform/mivem_macro.h>
#include <platform/oi10/platform/mivem_regs_access.h>
#include <platform/oi10/platform/mpic128/mpic128_regs.h>


#define SET_NONCR_INT_HANDLER(handler_ptr) \
        SPR_IVPR_write((uint32_t) (handler_ptr+0x00) & 0xFFFF0000);\
        SPR_IVOR4_write((uint32_t) (handler_ptr+0x00) & 0x0000FFF0)\

#define SET_CR_INT_HANDLER(handler_ptr) \
        SPR_IVPR_write((uint32_t) handler_ptr & 0xFFFF0000);\
        SPR_IVOR0_write((uint32_t) handler_ptr & 0x0000FFF0)\

#define MPIC128x_READ_DCR(i, REG) \
REG_READ_DCR(MPIC128, REG##i,32)

#define MPIC128x_WRITE_DCR(i, REG) \
REG_WRITE_DCR(MPIC128, REG##i, 32)

REPEAT( MPIC128x_VPx_REGS_COUNT, MPIC128x_READ_DCR,     MPIC128x_VP )
REPEAT( MPIC128x_VPx_REGS_COUNT, MPIC128x_WRITE_DCR,    MPIC128x_VP )

REPEAT( MPIC128x_DSTx_REGS_COUNT, MPIC128x_READ_DCR,    MPIC128x_DST)
REPEAT( MPIC128x_DSTx_REGS_COUNT, MPIC128x_WRITE_DCR,   MPIC128x_DST)

REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_WRITE_DCR,      MPIC128x_IPID)

REG_READ_DCR( MPIC128,                                  MPIC128x_CTP_PR, 32 )
REG_WRITE_DCR( MPIC128,                                 MPIC128x_CTP_PR, 32 )
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_CTP)
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_WRITE_DCR,      MPIC128x_CTP)

REG_READ_DCR( MPIC128,                                  MPIC128x_WAI_PR, 32 )
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_WAI)

REG_READ_DCR( MPIC128,                                  MPIC128x_NCIAR_PR, 32 )
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_NCIAR)

REG_READ_DCR( MPIC128,                                  MPIC128x_NCEOI_PR, 32 )
REG_WRITE_DCR( MPIC128,                                 MPIC128x_NCEOI_PR, 32 )
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_NCEOI)
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_WRITE_DCR,      MPIC128x_NCEOI)

REG_READ_DCR( MPIC128,                                  MPIC128x_CIAR_PR, 32 )
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_CIAR)

REG_READ_DCR( MPIC128,                                  MPIC128x_CEOI_PR, 32 )
REG_WRITE_DCR( MPIC128,                                 MPIC128x_CEOI_PR, 32 )
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_CEOI)
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_WRITE_DCR,      MPIC128x_CEOI)

REG_READ_DCR( MPIC128,                                  MPIC128x_IAR_PR, 32 )
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_IAR)

REG_READ_DCR( MPIC128,                                  MPIC128x_EOI_PR, 32 )
REG_WRITE_DCR( MPIC128,                                 MPIC128x_EOI_PR, 32 )
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_EOI)
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_WRITE_DCR,      MPIC128x_EOI)

REG_READ_DCR( MPIC128,                                  MPIC128x_FRG0, 32 )

REG_READ_DCR( MPIC128,                                  MPIC128x_GCF0, 32 )
REG_WRITE_DCR( MPIC128,                                 MPIC128x_GCF0, 32 )

REG_READ_DCR( MPIC128,                                  MPIC128x_VITC, 32 )
REG_WRITE_DCR( MPIC128,                                 MPIC128x_VITC, 32 )

REG_READ_DCR( MPIC128,                                  MPIC128x_VRID, 32 )
REG_WRITE_DCR( MPIC128,                                 MPIC128x_VRID, 32 )

REG_READ_DCR( MPIC128,                                  MPIC128x_REVID, 32 )

REG_READ_DCR( MPIC128,                                  MPIC128x_VIR0, 32 )

REG_READ_DCR( MPIC128,                                  MPIC128x_PINI, 32 )
REG_WRITE_DCR( MPIC128,                                 MPIC128x_PINI, 32 )

REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_IVP)
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_WRITE_DCR,      MPIC128x_IVP)

REG_READ_DCR( MPIC128,                                  MPIC128x_SPV, 32 )
REG_WRITE_DCR( MPIC128,                                 MPIC128x_SPV, 32 )

REG_READ_DCR( MPIC128,                                  MPIC128x_TFR, 32 )
REG_WRITE_DCR( MPIC128,                                 MPIC128x_TFR, 32 )

REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_TCC)

REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_TBC)
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_WRITE_DCR,      MPIC128x_TBC)

REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_TVP)
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_WRITE_DCR,      MPIC128x_TVP)

REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_READ_DCR,       MPIC128x_TDST)
REPEAT( MPIC128x_REGS_COUNT_4, MPIC128x_WRITE_DCR,      MPIC128x_TDST)


#endif // __MPIC128_REG_ACCESS_H__
