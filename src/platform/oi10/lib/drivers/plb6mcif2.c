/**
 *
 */
#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/test_assert.h>
#include <platform/devices/plb6mcif2.h>
#include <plb6bc.h>

void plb6mcif2_init( uint32_t const base_addr, plb6mcif2_cfg const *const cfg ) {
    register uint32_t reg_value;

    TEST_ASSERT( cfg != 0, "No configuration is given" );

    reg_value = plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr );
    TEST_ASSERT( !(reg_value & (1 << PLB6MCIF2_PLBCFG_Br_Enable_i)), "Bridge is already enabled" );

    switch( cfg->clk.relation )
    {
        case PLB6MCIF2_PLB_EQ_MCIF: {
            reg_value = (1 << PLB6MCIF2_PLBASYNC_PLBeqMCIF_i) | (0 << PLB6MCIF2_PLBASYNC_PLBgtMCIF_i);
            break;
        }
        case PLB6MCIF2_PLB_LT_MCIF: {
            reg_value = (0 << PLB6MCIF2_PLBASYNC_PLBeqMCIF_i) | (0 << PLB6MCIF2_PLBASYNC_PLBgtMCIF_i);
            break;
        }
        case PLB6MCIF2_PLB_GT_MCIF: {
            reg_value = (0 << PLB6MCIF2_PLBASYNC_PLBeqMCIF_i) | (1 << PLB6MCIF2_PLBASYNC_PLBgtMCIF_i);
            break;
        }
        default: TEST_ASSERT( false, "Unknown PLB6MCIF2_CLK_RELATION" );
    }
    reg_value   |=  (cfg->clk.plb6_cycles_ovrd << PLB6MCIF2_PLBASYNC_PLB6NUM_i)
                |   (cfg->clk.mcif_cycles_ovrd << PLB6MCIF2_PLBASYNC_MCIFNUM_i)
//                |   (cfg->clk.wrdone_cycles_ovrd << PLB6MCIF2_PLBASYNC_WRDONE_TUNE_i)
//                |   (cfg->clk.wrrdy_cycles_ovrd << PLB6MCIF2_PLBASYNC_WRRDY_TUNE_i)
                |   (cfg->clk.rdrdy_cycles_ovrd << PLB6MCIF2_PLBASYNC_RDRDY_TUNE_i );
    plb6mcif2_dcr_write_PLB6MCIF2_PLBASYNC( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_PLBASYNC( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->read_around_write.enabled << PLB6MCIF2_PLBORD_RaW_i)
                | (GET_BITS( cfg->read_around_write.threshold,  PLB6MCIF2_PLBORD_Threshold_1_3_n,   PLB6MCIF2_PLBORD_Threshold_0_n )    << PLB6MCIF2_PLBORD_Threshold_0_i)
                | (GET_BITS( cfg->read_around_write.threshold,  0,                                  PLB6MCIF2_PLBORD_Threshold_1_3_n )  << PLB6MCIF2_PLBORD_Threshold_1_3_i)
                | (cfg->intra_thread_out_of_order_rd_ret_enabled << PLB6MCIF2_PLBORD_IT_OOO_i)
                | (cfg->transaction_order_mode << PLB6MCIF2_PLBORD_TOM_i);
    plb6mcif2_dcr_write_PLB6MCIF2_PLBORD( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_PLBORD( base_addr ) == reg_value, "Register value is not updated" );

    switch( cfg->max_mem ) {
        case PLB6MCIF2_MAXMEM_64GB: break;
        case PLB6MCIF2_MAXMEM_32GB: {
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[0].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  0),     1 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       28),    1 ),"" );
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[1].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  0),     1 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       28),    1 ),"" );
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[2].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  0),     1 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       28),    1 ),"" );
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[3].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  0),     1 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       28),    1 ),"" );
            break;
        }
        case PLB6MCIF2_MAXMEM_16GB: {
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[0].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  1),     2 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       29),    2 ),"" );
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[1].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  1),     2 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       29),    2 ),"" );
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[2].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  1),     2 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       29),    2 ),"" );
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[3].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  1),     2 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       29),    2 ),"" );
            break;
        }
        case PLB6MCIF2_MAXMEM_8GB: {
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[0].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  2),     3 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       30),    3 ),"" );
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[1].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  2),     3 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       30),    3 ),"" );
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[2].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  2),     3 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       30),    3 ),"" );
            TEST_ASSERT(    GET_BITS( cfg->mem_rank[3].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  2),     3 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       30),    3 ),"" );
            break;
        }
    }
    reg_value   = (cfg->max_mem << PLB6MCIF2_MAXMEM_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MAXMEM( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_MAXMEM( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->upper_addr_base_addr << PLB6MCIF2_PUABA_i);
    plb6mcif2_dcr_write_PLB6MCIF2_PUABA( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_PUABA( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->master[0].id << PLB6MCIF2_P6BMTAG1_Mst0ID_i)
                | (cfg->master[1].id << PLB6MCIF2_P6BMTAG1_Mst1ID_i)
                | (cfg->master[2].id << PLB6MCIF2_P6BMTAG1_Mst2ID_i)
                | (cfg->master[3].id << PLB6MCIF2_P6BMTAG1_Mst3ID_i)
                | (cfg->master[4].id << PLB6MCIF2_P6BMTAG1_Mst4ID_i)
                | (cfg->master[5].id << PLB6MCIF2_P6BMTAG1_Mst5ID_i);
    plb6mcif2_dcr_write_PLB6MCIF2_P6BMTAG1( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_P6BMTAG1( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->master[6].id << PLB6MCIF2_P6BMTAG2_Mst6ID_i)
                | (cfg->master[7].id << PLB6MCIF2_P6BMTAG2_Mst7ID_i)
                | (cfg->master[8].id << PLB6MCIF2_P6BMTAG2_Mst8ID_i)
                | (cfg->master[9].id << PLB6MCIF2_P6BMTAG2_Mst9ID_i)
                | (cfg->master[10].id << PLB6MCIF2_P6BMTAG2_Mst10ID_i)
                | (cfg->master[11].id << PLB6MCIF2_P6BMTAG2_Mst11ID_i);
    plb6mcif2_dcr_write_PLB6MCIF2_P6BMTAG2( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_P6BMTAG2( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->master[12].id << PLB6MCIF2_P6BMTAG3_Mst12ID_i)
                | (cfg->master[13].id << PLB6MCIF2_P6BMTAG3_Mst13ID_i)
                | (cfg->master[14].id << PLB6MCIF2_P6BMTAG3_Mst14ID_i)
                | (cfg->master[15].id << PLB6MCIF2_P6BMTAG3_Mst15ID_i);
    plb6mcif2_dcr_write_PLB6MCIF2_P6BMTAG3( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_P6BMTAG3( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->mem_rank[0].master_acc.write << PLB6MCIF2_MAPxCF_MstIDWrAcc_i)
                | (cfg->mem_rank[0].master_acc.read << PLB6MCIF2_MAPxCF_MstIDRdAcc_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MAP0CF( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_MAP0CF( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->mem_rank[1].master_acc.write << PLB6MCIF2_MAPxCF_MstIDWrAcc_i)
                | (cfg->mem_rank[1].master_acc.read << PLB6MCIF2_MAPxCF_MstIDRdAcc_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MAP1CF( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_MAP1CF( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->mem_rank[2].master_acc.write << PLB6MCIF2_MAPxCF_MstIDWrAcc_i)
                | (cfg->mem_rank[2].master_acc.read << PLB6MCIF2_MAPxCF_MstIDRdAcc_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MAP2CF( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_MAP2CF( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->mem_rank[3].master_acc.write << PLB6MCIF2_MAPxCF_MstIDWrAcc_i)
                | (cfg->mem_rank[3].master_acc.read << PLB6MCIF2_MAPxCF_MstIDRdAcc_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MAP3CF( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_MAP3CF( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->mem_rank[0].base_addr << PLB6MCIF2_MRxCF_M_BA_i)
                | (cfg->mem_rank[0].size << PLB6MCIF2_MRxCF_M_SZ_i)
                | (cfg->mem_rank[0].enabled << PLB6MCIF2_MRxCF_M_RE_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MR0CF( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_MR0CF( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->mem_rank[1].base_addr << PLB6MCIF2_MRxCF_M_BA_i)
                | (cfg->mem_rank[1].size << PLB6MCIF2_MRxCF_M_SZ_i)
                | (cfg->mem_rank[1].enabled << PLB6MCIF2_MRxCF_M_RE_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MR1CF( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_MR1CF( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->mem_rank[2].base_addr << PLB6MCIF2_MRxCF_M_BA_i)
                | (cfg->mem_rank[2].size << PLB6MCIF2_MRxCF_M_SZ_i)
                | (cfg->mem_rank[2].enabled << PLB6MCIF2_MRxCF_M_RE_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MR2CF( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_MR2CF( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->mem_rank[3].base_addr << PLB6MCIF2_MRxCF_M_BA_i)
                | (cfg->mem_rank[3].size << PLB6MCIF2_MRxCF_M_SZ_i)
                | (cfg->mem_rank[3].enabled << PLB6MCIF2_MRxCF_M_RE_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MR3CF( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_MR3CF( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (GET_BITS( cfg->errors.logs.MxET1,    PLB6MCIF2_BESR_MxET1_1_5_n, PLB6MCIF2_BESR_MxET1_0_n )      << (PLB6MCIF2_BESR_MxET1_0_i+PLB6MCIF2_INTR_EN_ERR_EN_i))
                | (GET_BITS( cfg->errors.logs.MxET1,    0,                          PLB6MCIF2_BESR_MxET1_1_5_n )    << (PLB6MCIF2_BESR_MxET1_1_5_i+PLB6MCIF2_INTR_EN_ERR_EN_i))
                | (cfg->errors.logs.MxET2 << (PLB6MCIF2_BESR_MxET2_i+PLB6MCIF2_INTR_EN_ERR_EN_i))
                | (cfg->errors.logs.MxET3 << (PLB6MCIF2_BESR_MxET3_i+PLB6MCIF2_INTR_EN_ERR_EN_i))
                | (cfg->errors.logs.RS_Err << (PLB6MCIF2_BESR_RS_Err_1_2_i+PLB6MCIF2_INTR_EN_ERR_EN_i))
                | (GET_BITS( cfg->errors.interrupts.MxET1,  PLB6MCIF2_BESR_MxET1_1_5_n, PLB6MCIF2_BESR_MxET1_0_n )      << (PLB6MCIF2_BESR_MxET1_0_i+PLB6MCIF2_INTR_EN_INTR_EN_i))
                | (GET_BITS( cfg->errors.interrupts.MxET1,  0,                          PLB6MCIF2_BESR_MxET1_1_5_n )    << (PLB6MCIF2_BESR_MxET1_1_5_i+PLB6MCIF2_INTR_EN_INTR_EN_i))
                | (cfg->errors.interrupts.MxET2 << (PLB6MCIF2_BESR_MxET2_i+PLB6MCIF2_INTR_EN_INTR_EN_i))
                | (cfg->errors.interrupts.MxET3 << (PLB6MCIF2_BESR_MxET3_i+PLB6MCIF2_INTR_EN_INTR_EN_i))
                | (cfg->errors.interrupts.RS_Err << (PLB6MCIF2_BESR_RS_Err_1_2_i+PLB6MCIF2_INTR_EN_INTR_EN_i));
    plb6mcif2_dcr_write_PLB6MCIF2_INTR_EN( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_INTR_EN( base_addr ) == reg_value, "Register value is not updated" );

    reg_value   = (cfg->non_coherent << PLB6MCIF2_PLBCFG_Non_Coherent_i)
                | (cfg->tsnoop << PLB6MCIF2_PLBCFG_Tsnoop_i)
                | (cfg->master_acc_check_enabled << PLB6MCIF2_PLBCFG_PmstrID_chk_EN_i)
                | (cfg->paryties << PLB6MCIF2_PLBCFG_Parity_Enables_i)
                | (cfg->clk.gating << PLB6MCIF2_PLBCFG_CG_ENABLE_i)
                | (cfg->enabled << PLB6MCIF2_PLBCFG_Br_Enable_i);
    plb6mcif2_dcr_write_PLB6MCIF2_PLBCFG( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr ) == reg_value, "Register value is not updated" );
}

void plb6mcif2_simple_init( uint32_t base_addr, const uint32_t puaba )
{
    plb6mcif2_dcr_write_PLB6MCIF2_INTR_EN(base_addr,
            //enable logging but disable interrupts
              reg_field(15, 0xFFFF)
            | reg_field(31, 0x0000));

    plb6mcif2_dcr_write_PLB6MCIF2_PLBASYNC(base_addr,
            reg_field(1, 0b1)); //PLB clock equals MCIF2 clock

    plb6mcif2_dcr_write_PLB6MCIF2_PLBORD(base_addr,
              reg_field(0, 0b0)  //Raw
            | reg_field(3, 0b010) //Threshold[1:3]
            | reg_field(7, 0b0)   //Threshold[0]
            | reg_field(16, 0b0) //IT_OOO
            | reg_field(18, 0b00) //TOM
            );

    plb6mcif2_dcr_write_PLB6MCIF2_MAXMEM(base_addr,
            reg_field(1, 0b00)); //Set MAXMEM = 8GB

    plb6mcif2_dcr_write_PLB6MCIF2_PUABA(base_addr,
            puaba); //Set PLB upper address base address from I_S_ADDR[0:30]

    plb6mcif2_dcr_write_PLB6MCIF2_MR0CF(base_addr,
            //Set Rank0 base addr
            //M_BA[0:2] = PUABA[28:30]
            //M_BA[3:12] = 0b0000000000
              reg_field(3, (puaba & 0b1110)) | reg_field(12, 0b0000000000)
            | reg_field(19, 0b1001) //Set Rank0 size = 4GB
            | reg_field(31, 0b1)); //Enable Rank0

    plb6mcif2_dcr_write_PLB6MCIF2_MR1CF(base_addr,
            //Set Rank1 base addr
            //M_BA[0:2] = PUABA[28:30]
            //M_BA[3:12] = 0b1000000000
            reg_field(3, (puaba & 0b1110)) | reg_field(12, 0b1000000000)
          | reg_field(19, 0b1001) //Set Rank1 size = 4GB
          | reg_field(31, 0b1)); //Enable Rank1

    plb6mcif2_dcr_write_PLB6MCIF2_MR2CF(base_addr,
          reg_field(31, 0b0)); //Disable Rank2

    plb6mcif2_dcr_write_PLB6MCIF2_MR3CF(base_addr,
            reg_field(31, 0b0)); //Disable Rank3

    const uint32_t Tsnoop = (plb6bc_dcr_read_PLB6BC_TSNOOP(PLB6_BC_BASE) & (0b1111 << 28)) >> 28;
    trace_msg("Tsnoop=");
    trace_hex(Tsnoop);
    plb6mcif2_dcr_write_PLB6MCIF2_PLBCFG(base_addr,
            //PLB6MCIF2 enable [31] = 0b1
              reg_field(0, 0b0) //Non-coherent
            | reg_field(4, Tsnoop) //T-snoop
            | reg_field(5, 0b0) //PmstrID_chk_EN
            | reg_field(27, 0b1111) //Parity enables
            | reg_field(30, 0b000) //CG_Enable
            | reg_field(31, 0b1)); //BR_Enable

}

void plb6mcif2_get_cfg( uint32_t const base_addr, plb6mcif2_cfg *const cfg ) {
    TEST_ASSERT( cfg != 0, "No configuration storage is given" );

    TEST_ASSERT( false, "Not implemented");
}

void plb6mcif2_enable( uint32_t const base_addr ) {
    register uint32_t reg_value = plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr );
    SET_BIT( reg_value, PLB6MCIF2_PLBCFG_Br_Enable_i );
    plb6mcif2_dcr_write_PLB6MCIF2_PLBCFG( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr ) == reg_value, "Register value is not updated" );
}

void plb6mcif2_disable( uint32_t const base_addr ) {
    register uint32_t reg_value = plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr );
    CLEAR_BIT( reg_value, PLB6MCIF2_PLBCFG_Br_Enable_i );
    plb6mcif2_dcr_write_PLB6MCIF2_PLBCFG( base_addr, reg_value );
    TEST_ASSERT( plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr ) == reg_value, "Register value is not updated" );
}
