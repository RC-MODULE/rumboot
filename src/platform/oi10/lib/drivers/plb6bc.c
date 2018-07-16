/**
 *
 */

#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/test_assert.h>
#include <platform/plb6bc.h>
#include <platform/devices.h>


void plb6bc_clear_status( uint32_t const base_addr, plb6bc_status const *const clear_status ) {
    register uint32_t reg_value = 0x00000000;

    switch( clear_status->err.type ) {
        case PLB6BC_ERR_no_slave_resp: {
            reg_value   =   (0b1 << PLB6BC_ERR_BC_NSRSP_i);
            break;
        }
        case PLB6BC_ERR_mult_slaves_resp: {
            reg_value   =   (0b1 << PLB6BC_ERR_BC_MSRSP_i);
            break;
        }
        case PLB6BC_ERR_snoopable_req: {
            reg_value   =   (0b1 << PLB6BC_ERR_BC_SRSNZ_i);
            break;
        }
        case PLB6BC_ERR_all: {
            reg_value   =   (0b1 << PLB6BC_ERR_BC_NSRSP_i)
                        |   (0b1 << PLB6BC_ERR_BC_MSRSP_i)
                        |   (0b1 << PLB6BC_ERR_BC_SRSNZ_i);
            break;
        }
    }
    reg_value   |=  (clear_status->err.master_id << PLB6BC_ERR_BC_SREMID_i)
                |   (clear_status->err.mtag << PLB6BC_ERR_BC_SREMT_i)
                |   (clear_status->err.transaction_type << PLB6BC_ERR_BC_SRERD_i)
                |   (clear_status->err.segment_id << PLB6BC_ERR_BC_SEGID_i)
                |   (clear_status->err.slave_responded << PLB6BC_ERR_BC_SPVAL_i  );
    plb6bc_dcr_write_PLB6BC_ERR( base_addr, reg_value );
    TEST_ASSERT( !(plb6bc_dcr_read_PLB6BC_ERR( base_addr ) & reg_value), "Register value is not updated" );

    reg_value   =   clear_status->segment_slave_responded;
    plb6bc_dcr_write_PLB6BC_MSRSP( base_addr, reg_value );
    TEST_ASSERT( !(plb6bc_dcr_read_PLB6BC_MSRSP( base_addr ) & reg_value), "Register value is not updated" );

    reg_value   =   (clear_status->hang_detected << PLB6BC_HD_i);
    plb6bc_dcr_write_PLB6BC_HD( base_addr, reg_value );
    TEST_ASSERT( !(plb6bc_dcr_read_PLB6BC_HD( base_addr ) & reg_value), "Register value is not updated" );

    reg_value   =   (clear_status->quiesce_detected << PLB6BC_QD_i);
    plb6bc_dcr_write_PLB6BC_QD( base_addr, reg_value );
    TEST_ASSERT( !(plb6bc_dcr_read_PLB6BC_QD( base_addr ) & reg_value), "Register value is not updated" );

    reg_value   =   (clear_status->sys_hung_detected << PLB6BC_SHD_i);
    plb6bc_dcr_write_PLB6BC_SHD( base_addr, reg_value );
    TEST_ASSERT( !(plb6bc_dcr_read_PLB6BC_SHD( base_addr ) & reg_value), "Register value is not updated" );
}

void plb6bc_init( uint32_t const base_addr,  plb6bc_cfg const *const cfg ) {
    register uint32_t reg_value;

    TEST_ASSERT( cfg != 0, "No configuration is given" );

    static struct plb6bc_status const clear_all_status = {
        .hang_detected              = 0xFFFF,
        .quiesce_detected           = 0xFFFF,
        .sys_hung_detected          = 0xFFFF,
        .segment_slave_responded    = 0xFFFFFFFF,
        .err = {
            .type                   = PLB6BC_ERR_all,
            .master_id              = 0b1111,
            .mtag                   = 0b11111,
            .transaction_type       = 0b1,
            .segment_id             = 0b111,
            .slave_responded        = 0b1111,
        }
    };
    plb6bc_clear_status( base_addr, &clear_all_status );

    reg_value   =   0x00000000;
    #define PLB6BC_MASTERi_PRIORITY_SET( z, i, dummy )\
                |   (cfg->priority.CAT( master,i ) << CAT(CAT( PLB6BC_PRI_M,i),P_i ))
    //REPEAT( PLB6BC_MASTER_N, PLB6BC_MASTERi_PRIORITY_SET, dummy );
    plb6bc_dcr_write_PLB6BC_PRI( base_addr, reg_value );
    TEST_ASSERT( plb6bc_dcr_read_PLB6BC_PRI( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   =   (cfg->comb_resp_timeout << PLB6BC_PAAM_WIN_EXT_i);
    plb6bc_dcr_write_PLB6BC_PAAM_WIN_EXT( base_addr, reg_value );
    TEST_ASSERT( plb6bc_dcr_read_PLB6BC_PAAM_WIN_EXT( base_addr ) == reg_value, "Register value is not updated" );

    #define PLB6BC_SEGMENTi_ADDR_SET( z, i, dummy )\
    reg_value   =   (cfg->CAT(BOOST_PP_CAT( segment,i),_addr ) << PLB6BC_SGDi_i);\
    CAT( plb6bc_dcr_write_PLB6BC_SGD,i )( base_addr, reg_value );\
    TEST_ASSERT( BOOST_PP_CAT( plb6bc_dcr_read_PLB6BC_SGD,i )( base_addr ) == reg_value, "Register value is not updated" );

    //BOOST_PP_REPEAT_FROM_TO( 1, PLB6BC_SEGMENT_N, PLB6BC_SEGMENTi_ADDR_SET, dummy )

    reg_value   =   (cfg->hang_count_pulse_period << PLB6BC_HCPP_i);
    plb6bc_dcr_write_PLB6BC_HCPP( base_addr, reg_value );
    TEST_ASSERT( plb6bc_dcr_read_PLB6BC_HCPP( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   =   (cfg->mask_err.snoopable_req << PLB6BC_CR0_BC_MASK_SRSNZ_i)
                |   (cfg->mask_err.mult_slaves_resp << PLB6BC_CR0_BC_MASK_MSRSP_i)
                |   (cfg->enabled << PLB6BC_CR0_BC_DSBS_i );
    plb6bc_dcr_write_PLB6BC_CR0( base_addr, reg_value );
    TEST_ASSERT( plb6bc_dcr_read_PLB6BC_CR0( base_addr ) == reg_value, "Register value is not updated" );
}

/*void plb6bc_get_cfg( uint32_t const base_addr,  plb6bc_cfg *const cfg ) {
    ASSERT( cfg != 0 );
}*/

void plb6bc_enable( uint32_t const base_addr ) {
    register uint32_t reg_value = plb6bc_dcr_read_PLB6BC_CR0( base_addr );
    SET_BIT( reg_value, PLB6BC_CR0_BC_DSBS_i );
    plb6bc_dcr_write_PLB6BC_CR0( base_addr, reg_value );
    TEST_ASSERT( plb6bc_dcr_read_PLB6BC_CR0( base_addr ) == reg_value, "Register value is not updated" );
}
