
#ifndef __DDR_PLB6MCIF2_REG_ACCESS_H__
#define __DDR_PLB6MCIF2_REG_ACCESS_H__

#include <arch/ppc_476fp_lib_c.h>
#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/reg_access/macros.h>
#include <platform/regs/regs_plb6mcif2.h>

inline static uint32_t plb6mcif2_dcr_read_PLB6MCIF2_BESR( uint32_t const base_addr ) {
    return dcr_read( base_addr+PLB6MCIF2_BESR_read );
}
inline static void plb6mcif2_dcr_set_PLB6MCIF2_BESR( uint32_t const base_addr, uint32_t const value ) {
    dcr_write( base_addr+PLB6MCIF2_BESR_set, value );
}
inline static void plb6mcif2_dcr_clear_PLB6MCIF2_BESR( uint32_t const base_addr, uint32_t const value ) {
    dcr_write( base_addr+PLB6MCIF2_BESR_clear, value );
}

REG_READ_DCR( plb6mcif2, PLB6MCIF2_BEARL, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_BEARU, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_INTR_EN, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_INTR_EN, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_PLBASYNC, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_PLBASYNC, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_PLBORD, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_PLBORD, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_PLBCFG, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_PLBCFG, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAXMEM, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAXMEM, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_PUABA, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_PUABA, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MR0CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MR0CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MR1CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MR1CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MR2CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MR2CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MR3CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MR3CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_SR0CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_SR0CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_SR1CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_SR1CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_SR2CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_SR2CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_SR3CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_SR3CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG1, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG1, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG2, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG2, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG3, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG3, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAP0CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAP0CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAP1CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAP1CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAP2CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAP2CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAP3CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAP3CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_STATUS, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_RID, 32 )


#endif // __DDR_PLB6MCIF2_REG_ACCESS_H__
