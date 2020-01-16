#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>

#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <arch/ibm_bit_ordering_macros.h>
#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>


static void fill_utlb_entries( void );
static void check_utlb_entries( void );

int main() {
    test_event_send_test_id("test_oi10_cpu_utlb");

    fill_utlb_entries();

    check_utlb_entries();

    return 0;
}


/* <<<<<<<<<<Implementation>>>>>>>>>> */

#define CALC_TS( index )        (0x00000001 & index)                /* TS = odd(index[0:7]) */
#define CALC_TID( index )       (index << 8)                        /* TID[0:15] = {index[0:7], 8'h0} */
#define CALC_EPN( index, way )  ( (index << 12) | (way << 10) )     /* EPN[0:19] = {index[0:7], way[0:1], 10'h0} */
#define CALC_ERPN( index, way ) ( (way << 8) | index )              /* ERPN[0:9] = {way[0:1], index[0:7]} */
#define CALC_UXWRSXWR( index )  (index >> 2)                        /* {UXWR[0:2], SXWR[0:2]} = EPN[0:5] */
#define CALC_MMU_TLBE_ATTR( ts, erpn, uxwrsxwr ) (\
    ( (ts       << MMU_TLBE_ATTR_IL1I_i)                            /* L1I = TS */\
    | (erpn     << MMU_TLBE_ATTR_E_i)                               /* {L1D, U[0:3], WIMGE[0:4]} = {way[0:1], index[0:7]} */\
    | (uxwrsxwr << MMU_TLBE_ATTR_SR_i) )\
    & ~(0b1 << MMU_TLBE_ATTR_I_i)                                   /* I = 0 for enable L1I, L1D values */\
)

static void fill_utlb_way( uint32_t const way ) {
    uint32_t const mmube0 = spr_read( SPR_MMUBE0 );

    uint32_t ts, tid, epn, erpn, uxwrsxwr, ra;
    uint32_t mmucr, tlbe_tag, tlbe_data, tlbe_attr;

    for( uint32_t index = 0; index < MMU_UTLB_INDEXES_N; index++ ) {
        if( (way != 0)
         || ( (GET_BITS(mmube0, MMU_MMUBE0_IBE0_i, MMU_MMUBE0_IBE0_n) != index )
           && (GET_BITS(mmube0, MMU_MMUBE0_IBE1_i, MMU_MMUBE0_IBE1_n) != index )
           && (GET_BITS(mmube0, MMU_MMUBE0_IBE2_i, MMU_MMUBE0_IBE2_n) != index ) ) ) {
            tid         = CALC_TID( index );
            epn         = CALC_EPN( index, way );
            ts          = CALC_TS( index );
            erpn        = CALC_ERPN( index, way );
            uxwrsxwr    = CALC_UXWRSXWR( index );
            ra          = MMU_TLBWE_RA( (MMU_TLBWE_WAY_0|way), MMU_TLBWE_BE_UND );

            mmucr       = MMU_MMUCR( 0b0,  0b0,0b0,    ts,     tid );
            tlbe_tag    = MMU_TLBE_TAG( epn,   0b1,    ts,     MMU_TLBE_DSIZ_16MB,     0b0,    0b0,0b0,0b0 );
            tlbe_data   = MMU_TLBE_DATA( erpn,epn,     0b0,0b0 );
            tlbe_attr   = CALC_MMU_TLBE_ATTR( ts, erpn, uxwrsxwr );

            spr_write(  SPR_MMUCR, mmucr );
            tlbwe( tlbe_tag, ra, MMU_TLB_ENTRY_TAG );
            tlbwe( tlbe_data,ra, MMU_TLB_ENTRY_DATA );
            tlbwe( tlbe_attr,ra, MMU_TLB_ENTRY_ATTR );
        }
    }
}

static void fill_utlb_entries( void ) {
    rumboot_printf( "Fill UTLB entries\n");

    for( uint32_t way=0; way < MMU_UTLB_WAYS_N; way++ ) {
        fill_utlb_way( way );
    }
    isync();
}

enum {
    TEC_CHECK_UTLB_ENTRIES = TEST_EVENT_CODE_MIN,
};

static void check_utlb_entries( void ) {
    rumboot_printf( "Check UTLB entries\n");

    test_event( TEC_CHECK_UTLB_ENTRIES );

    uint32_t ts, tid, epn, erpn, uxwrsxwr, ra, tlbe_attr_only;
    uint32_t mmucr, tlbe_tag, tlbe_data, tlbe_attr;

    for( uint32_t index = 0; index < MMU_UTLB_INDEXES_N; index++ ) {
        for( uint32_t way = 0; way < MMU_UTLB_WAYS_N; way++ ) {
            ra = MMU_TLBRE_RA( index, way );

            tlbe_tag        = tlbre( ra, MMU_TLB_ENTRY_TAG );
            if( !GET_BIT(tlbe_tag, MMU_TLBE_TAG_BLTD_i) ) {
                mmucr       = spr_read(  SPR_MMUCR );
                tlbe_data   = tlbre( ra, MMU_TLB_ENTRY_DATA );
                tlbe_attr   = tlbre( ra, MMU_TLB_ENTRY_ATTR );

                tid             = CALC_TID( index );
                epn             = CALC_EPN( index, way );
                ts              = CALC_TS( index );
                erpn            = CALC_ERPN( index, way );
                uxwrsxwr        = CALC_UXWRSXWR( index );
                tlbe_attr_only  = CALC_MMU_TLBE_ATTR( ts, erpn, uxwrsxwr );

                TEST_ASSERT( tlbe_tag == MMU_TLBE_TAG( epn, 0b1, ts, MMU_TLBE_DSIZ_16MB, 0b0, parity32( epn ), parity32( 0b1 ^ ts ^ MMU_TLBE_DSIZ_16MB ), parity32( tid ) ), "Wrong TAG");
                TEST_ASSERT( GET_BITS(mmucr, MMU_MMUCR_STID_i, MMU_MMUCR_STID_n) == tid, "Wrong TAG");
                TEST_ASSERT( tlbe_data == MMU_TLBE_DATA( erpn,epn, parity32( erpn ), parity32( epn ) ), "Wrong DATA");
                TEST_ASSERT( tlbe_attr == (tlbe_attr_only | ( parity32( tlbe_attr_only ) << MMU_TLBE_ATTR_SPPar_i )), "Wrong ATTR");
            }
        }
    }

}
