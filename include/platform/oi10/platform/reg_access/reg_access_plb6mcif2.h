
#ifndef __DDR_PLB6MCIF2_REG_ACCESS_H__
#define __DDR_PLB6MCIF2_REG_ACCESS_H__

#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/reg_access/macros.h>
#include <platform/regs/em2/regs_plb6mcif2.h>


inline static uint32_t plb6mcif2_dcr_read_PLB6MCIF2_BESR( uint32_t const base_addr ) {
    return mfdcrx( base_addr+PLB6MCIF2_BESR_read );
}
inline static void plb6mcif2_dcr_set_PLB6MCIF2_BESR( uint32_t const base_addr, uint32_t const value ) {
    mtdcrx( base_addr+PLB6MCIF2_BESR_set, value );
}
inline static void plb6mcif2_dcr_clear_PLB6MCIF2_BESR( uint32_t const base_addr, uint32_t const value ) {
    mtdcrx( base_addr+PLB6MCIF2_BESR_clear, value );
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

#define PLB6MCIF2_MRxCF_ACCESS( z, i, dummy )\
REG_READ_DCR( plb6mcif2, PLB6MCIF2_MR##i##CF, 32 )\
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MR##i##CF, 32 )
BOOST_PP_REPEAT( PLB6MCIF2_MEMRANK_N, PLB6MCIF2_MRxCF_ACCESS, dummy )

#define PLB6MCIF2_SRxCF_ACCESS( z, i, dummy )\
REG_READ_DCR( plb6mcif2, PLB6MCIF2_SR##i##CF, 32 )
BOOST_PP_REPEAT( 4, PLB6MCIF2_SRxCF_ACCESS, dummy )

#define PLB6MCIF2_P6BMTAGx_ACCESS( z, i, dummy )\
REG_READ_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG##i, 32 )\
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG##i, 32 )
BOOST_PP_REPEAT_FROM_TO( 1, 4, PLB6MCIF2_P6BMTAGx_ACCESS, dummy )

#define PLB6MCIF2_MAPxCF_ACCESS( z, i, dummy )\
REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAP##i##CF, 32 )\
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAP##i##CF, 32 )
BOOST_PP_REPEAT( PLB6MCIF2_MEMRANK_N, PLB6MCIF2_MAPxCF_ACCESS, dummy )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_STATUS, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_RID, 32 )


#endif // __DDR_PLB6MCIF2_REG_ACCESS_H__
