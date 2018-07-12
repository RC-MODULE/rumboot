#ifndef __DDR_PLB6MCIF2_H__
#define __DDR_PLB6MCIF2_H__


#include <stdint.h>
#include <stdbool.h>
#include <platform/reg_access/reg_access_plb6mcif2.h>
#include <platform/regs/fields/plb6mcif2.h>

typedef enum PLB6MCIF2_CLK_RELATION
{
    PLB6MCIF2_PLB_EQ_MCIF,  // PLB6 clock period equal to MCIF clock period within +/-5%
    PLB6MCIF2_PLB_LT_MCIF,  // PLB6 clock period is less than 0.95 MCIF clock period
    PLB6MCIF2_PLB_GT_MCIF,  // PLB6 clock period is greater than 1.05 MCIF clock period
} PLB6MCIF2_CLK_RELATION;


typedef struct plb6mcif2_errors
{
    PLB6MCIF2_ERROR_TYPE1   MxET1:     PLB6MCIF2_BESR_MxET1_0_n+PLB6MCIF2_BESR_MxET1_1_5_n;
    uint8_t                 MxET2:     PLB6MCIF2_BESR_MxET2_n;
    PLB6MCIF2_ERROR_TYPE3   MxET3:     PLB6MCIF2_BESR_MxET3_n;
    uint8_t                 RS_Err:    PLB6MCIF2_BESR_RS_Err_1_2_n;
} plb6mcif2_errors;


typedef struct plb6mcif2_master_map
{
    uint8_t id: PLB6MCIF2_P6BMTAGx_MstxID_n;
} plb6mcif2_master_map;


typedef struct plb6mcif2_memrank_cfg
{
    uint32_t                base_addr:  PLB6MCIF2_MRxCF_M_BA_n;
    PLB6MCIF2_MEMRANK_SZ    size:       PLB6MCIF2_MRxCF_M_SZ_n;
    bool                    enabled;
    struct
    {
        uint32_t            write:      PLB6MCIF2_MAPxCF_MstIDWrAcc_n;
        uint32_t            read:       PLB6MCIF2_MAPxCF_MstIDRdAcc_n;
    } master_acc;
} plb6mcif2_memrank_cfg;


typedef struct plb6mcif2_cfg
{
    bool                            enabled;
    struct
    {
        PLB6MCIF2_CLK_RELATION      relation;
        uint32_t                    plb6_cycles_ovrd:       PLB6MCIF2_PLBASYNC_PLB6NUM_n;
        uint32_t                    mcif_cycles_ovrd:       PLB6MCIF2_PLBASYNC_MCIFNUM_n;
//        uint32_t                    wrdone_cycles_ovrd:     PLB6MCIF2_PLBASYNC_WRDONE_TUNE_n;
//        uint32_t                    wrrdy_cycles_ovrd:      PLB6MCIF2_PLBASYNC_WRRDY_TUNE_n;
        uint32_t                    rdrdy_cycles_ovrd:      PLB6MCIF2_PLBASYNC_RDRDY_TUNE_n;
        PLB6MCIF2_CLK_GATING        gating:                 PLB6MCIF2_PLBCFG_CG_ENABLE_n;
    } clk;
    bool                            non_coherent;
    PLB6MCIF2_TSNOOP                tsnoop:                 PLB6MCIF2_PLBCFG_Tsnoop_n;
    uint8_t                         paryties:               PLB6MCIF2_PLBCFG_Parity_Enables_n;
    struct
    {
        plb6mcif2_errors            logs;
        plb6mcif2_errors            interrupts;
    } errors;
    struct
    {
        bool                        enabled;
        PLB6MCIF2_PLB_RAW_THRESHOLD threshold:              PLB6MCIF2_PLBORD_Threshold_n;
    } read_around_write;
    bool                            intra_thread_out_of_order_rd_ret_enabled;
    PLB6MCIF2_MCIF2_TOM             transaction_order_mode: PLB6MCIF2_PLBORD_TOM_n;
    PLB6MCIF2_PLB_MAXMEM            max_mem:                PLB6MCIF2_MAXMEM_n;
    uint32_t                        upper_addr_base_addr:   PLB6MCIF2_PUABA_n;
    bool                            master_acc_check_enabled;
    plb6mcif2_master_map            master[PLB6MCIF2_MASTER_N];
    plb6mcif2_memrank_cfg           mem_rank[PLB6MCIF2_MEMRANK_N];
} plb6mcif2_cfg;

void plb6mcif2_init( uint32_t base_addr, plb6mcif2_cfg const * cfg );
void plb6mcif2_simple_init( uint32_t base_addr, const uint32_t puaba );

void plb6mcif2_get_cfg( uint32_t base_addr, plb6mcif2_cfg * cfg );


void plb6mcif2_enable( uint32_t base_addr );

void plb6mcif2_disable( uint32_t base_addr );


#endif // __DDR_PLB6MCIF2_H__
