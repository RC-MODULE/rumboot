#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_test_macro.h>
#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 


int nu_mpe_decide_dma_config(
    ConfigMPE* cfg, 
    CubeMetrics* cube_metrics, 
    WarrMetrics* warr_metrics,
    void* cube,
    void* warr,
    void* cfg_lut
) {
    cfg->dma_d_config.rdma.BFCA  = nu_virt_to_dma(cube); // Data Base Address
    cfg->dma_w_config.rdma.BFCA  = nu_virt_to_dma(warr); // Weights Base Address
    cfg->dma_d_config.depend_mask = 0; // Does Not Depend On Any Other Channel Run
    cfg->dma_w_config.depend_mask = 0;
    if(nu_mpe_look_up_dma_config(cfg,cfg_lut)!=0) return -1; // Search For The MPE DMA Configuration (That Is Predefined In cfg_lut Table)
    if(nu_mpe_decide_dma_config_trivial(cfg, cube_metrics, warr_metrics)!=0) return -1; // Fill Other Fields
    return 0;
}

void nu_vpe_decide_dma_config (
    ConfigVPE* cfg,
    CubeMetrics* in_metrics,
    uint8_t axi_len,
    void*in_data,
    void*op0,
    void*op1,
    void*op2,
    //~ CubeMetrics*res_metrics,
    void*res_data,
    Enable PPE_ENABLED,
    bool last_turn
) {
    cfg->trace_mode = TraceMode_MPE;  // Make VPE To Use MPE Trace Mode // FIRST!!!! Because nu_vpe_decide_dma_config_trivial depends On It

    cfg->src_rdma_config.dma_bsize=0;   // Batch Parameters Are Default In This Program (Do Not Use Batch)
    cfg->src_rdma_config.dma_bstride=0;
    cfg->op0_rdma_config.dma_bsize=0;
    cfg->op0_rdma_config.dma_bstride=0;
    cfg->op1_rdma_config.dma_bsize=0;
    cfg->op1_rdma_config.dma_bstride=0;
    cfg->op2_rdma_config.dma_bsize=0;
    cfg->op2_rdma_config.dma_bstride=0;
    cfg->wdma_config.dma_bsize=0;
    cfg->wdma_config.dma_bstride=0;
    cfg->depend_mask=0; // Does Not Depend On Any Other Channel Run

    cfg->src_flying = Enable_En;  // Make VPE To Get Data From MPE
    if(PPE_ENABLED==Enable_En)    //  And Send Them To PPE If PPE_ENABLED
        cfg->dst_flying = Enable_En;
    else
        cfg->dst_flying = Enable_NotEn;

    nu_vpe_decide_dma_config_trivial(cfg,in_metrics); // Fill Other cfg Fields
  
    if(PPE_ENABLED==Enable_NotEn && last_turn) 
        cfg->mark = Enable_En;

      // Data Base Addresses
    cfg->src_rdma_config.dma_baddr = (uint32_t) 0xDEADBEEF; // Should Be Not Used
    cfg->op0_rdma_config.dma_baddr = nu_virt_to_dma(op0);
    cfg->op1_rdma_config.dma_baddr = nu_virt_to_dma(op1);
    cfg->op2_rdma_config.dma_baddr = nu_virt_to_dma(op2);
    cfg->wdma_config.dma_baddr     = nu_virt_to_dma(res_data);

    cfg->src_rdma_config.dma_axi_len = axi_len;
    cfg->op0_rdma_config.dma_axi_len = axi_len;
    cfg->op1_rdma_config.dma_axi_len = axi_len;
    cfg->op2_rdma_config.dma_axi_len = axi_len;
    cfg->wdma_config.dma_axi_len     = axi_len;

}

void nu_ppe_decide_dma_config(
    ConfigPPE* cfg,
    ConfigREGPPE* cfg_reg,
    CubeMetrics* res_metrics,
    void*in_data,
    void*res_data
){
    cfg_reg->rBALi = (uintptr_t)0xDEADBEEF; // Data Base Addresses
    cfg_reg->wBALo = nu_virt_to_dma(res_data);

    cfg_reg->wOpM = 3 << 8; // FLYING_BOXED 
                         // Other Fields Of wOpM Will Be Appended By nu_ppe_decide_dma_config_trivial
    nu_ppe_decide_dma_config_trivial(cfg, res_metrics, cfg_reg);
    cfg_reg->wOpEn  = 0x1; // This Is Needed Because Some Other Setup Functions Write Into DMA Start Reg
}

NPEReg* nu_npe_add_diff_reg_map(NPEReg* associative_regs_dump_curr_ptr, NPEIterationDescriptor iteration_desc, uint32_t i) {
    if (i == 0) {
        associative_regs_dump_curr_ptr = nu_mpe_add_diff_reg_map(associative_regs_dump_curr_ptr, NULL, iteration_desc.next_regs_dump->mpe);
        associative_regs_dump_curr_ptr = nu_mpe_add_diff_start(associative_regs_dump_curr_ptr, iteration_desc.cfg_mpe);
        associative_regs_dump_curr_ptr = nu_vpe_add_diff_reg_map(associative_regs_dump_curr_ptr, NULL, iteration_desc.next_regs_dump->vpe, iteration_desc.cfg_vpe->op2_config.lut_en);
        associative_regs_dump_curr_ptr = nu_vpe_add_diff_start(associative_regs_dump_curr_ptr, iteration_desc.cfg_vpe);
        if(iteration_desc.PPE_ENABLED==Enable_En) {
            associative_regs_dump_curr_ptr = nu_ppe_rdma_add_diff_reg_map(associative_regs_dump_curr_ptr, NULL, iteration_desc.next_regs_dump->ppe_rdma);
            associative_regs_dump_curr_ptr = nu_ppe_wdma_add_diff_reg_map(associative_regs_dump_curr_ptr, NULL, iteration_desc.next_regs_dump->ppe_wdma);
            associative_regs_dump_curr_ptr = nu_ppe_wdma_add_diff_start(associative_regs_dump_curr_ptr, iteration_desc.cfg_reg_ppe);
        }
    }
    else {
        associative_regs_dump_curr_ptr = nu_mpe_add_diff_reg_map(associative_regs_dump_curr_ptr, iteration_desc.curr_regs_dump->mpe, iteration_desc.next_regs_dump->mpe);
        associative_regs_dump_curr_ptr = nu_mpe_add_diff_start(associative_regs_dump_curr_ptr, iteration_desc.cfg_mpe);
        associative_regs_dump_curr_ptr = nu_vpe_add_diff_reg_map(associative_regs_dump_curr_ptr, iteration_desc.curr_regs_dump->vpe, iteration_desc.next_regs_dump->vpe, iteration_desc.cfg_vpe->op2_config.lut_en);
        associative_regs_dump_curr_ptr = nu_vpe_add_diff_start(associative_regs_dump_curr_ptr, iteration_desc.cfg_vpe);
        if(iteration_desc.PPE_ENABLED==Enable_En) {
            associative_regs_dump_curr_ptr = nu_ppe_rdma_add_diff_reg_map(associative_regs_dump_curr_ptr, iteration_desc.curr_regs_dump->ppe_rdma, iteration_desc.next_regs_dump->ppe_rdma);
            associative_regs_dump_curr_ptr = nu_ppe_wdma_add_diff_reg_map(associative_regs_dump_curr_ptr, iteration_desc.curr_regs_dump->ppe_wdma, iteration_desc.next_regs_dump->ppe_wdma);
            associative_regs_dump_curr_ptr = nu_ppe_wdma_add_diff_start(associative_regs_dump_curr_ptr, iteration_desc.cfg_reg_ppe);
        }
    }
    return associative_regs_dump_curr_ptr;
}


void nu_setup_next_regs_dump(NPEIterationDescriptor iteration_desc) {
    nu_mpe_setup_or_dump(1,(uintptr_t)iteration_desc.next_regs_dump->mpe, iteration_desc.cfg_mpe);
    nu_vpe_setup_or_dump(1,(uintptr_t)iteration_desc.next_regs_dump->vpe, iteration_desc.cfg_vpe);
    if(iteration_desc.PPE_ENABLED==Enable_En) {
        nu_ppe_setup_reg_or_dump(1,(uintptr_t)iteration_desc.next_regs_dump->ppe_rdma, (uintptr_t)iteration_desc.next_regs_dump->ppe_wdma, iteration_desc.cfg_reg_ppe);
    }
}

void nu_get_temp_depend_table(NPETestDescriptor* test_desc, int iterations) {
    int depend_rd_after_wr_table[2][2][7] = {
        {
            { -1, -1, -1, -1, -1, -1, -1}, // WDMA VPE 0
            { -1, -1, -1, -1, -1, -1, -1}, // WDMA PPE 0
        },
        {
            { -1, -1, -1, -1, -1, -1, -1}, // WDMA VPE 1
            {  0, -1, -1, -1, -1, -1, -1}, // WDMA PPE 1
        },
    };

    int depend_wr_after_rd_table[2][2][7] = {
        {
            { -1, -1, -1, -1, -1, -1, -1}, // WDMA VPE 0
            { -1, -1, -1, -1, -1, -1, -1}, // WDMA PPE 0
        },
        {
            { -1, -1, -1, -1, -1, -1, -1},  // WDMA VPE 1
            { -1, -1, -1, -1, -1, -1, -1},  // WDMA PPE 1
        },
    };

    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 7; k++) {
                test_desc->array_of_depend_table[i].read_after_write[j][k] = depend_rd_after_wr_table[i][j][k];
                test_desc->array_of_depend_table[i].write_after_read[j][k] = depend_wr_after_rd_table[i][j][k];
            }
        }
    }
}


NPETestDescriptor test_desc;
NPEIterationDescriptor iteration_desc;


int main() {
    int heap_id;
    int i;
    int iterations;
    uint8_t  axi_len;

    rumboot_printf("Hello\n");

    heap_id = nu_get_heap_id();

      // Read The Number Of Test Iterations
    rumboot_platform_request_file_ex("num_iterations_file_tag",(uintptr_t) &iterations,sizeof(iterations));
    rumboot_printf("Number of iterations %d\n",iterations);

    // Zero The Test Descriptor Fields
    nu_npe_init_test_desc(&test_desc);
    // Overwrite The PPE_ENABLED Test Property If This Program Is Compiled Not To Use The PPE
    //  If You Need A Scenario Where You Can Dynamically Enable Or Disable PPE - You Should Write Another Program
#ifdef DONT_USE_PPE
    test_desc.PPE_ENABLED=Enable_NotEn;
#endif
    axi_len = 15; // 15 - Most Performance-Friendly Option

      // Load All The Test Data Into Memory
    if(nu_npe_place_arrays(heap_id,&test_desc,iterations) !=0) return -1;
    if (nu_npe_place_associative_regs_dump(heap_id,&test_desc,iterations) !=0) return -1;
    if (nu_npe_place_array_of_depend_table(heap_id,&test_desc, iterations) !=0) return -1;
    nu_get_temp_depend_table(&test_desc, iterations);
    
      // Make Trivial iteration_cfg_map
    if( nu_malloc_iteration_cfg_map(heap_id, &(test_desc.iteration_cfg_map), iterations) != 0) return -1;
    nu_fill_iteration_cfg_map(
    &(test_desc.iteration_cfg_map), 
      test_desc.array_of_depend_table, 
      test_desc.array_of_cfg_mpe,
      test_desc.array_of_cfg_vpe,
      test_desc.array_of_cfg_ppe,
      test_desc.array_of_cfg_reg_ppe,
      iterations
    );

      // Make The Iteration Descriptor To Point At The First Test Data
    if (nu_npe_place_regs_dump(heap_id,&test_desc) !=0) return -1;
    nu_npe_init_iteration_desc(&test_desc,&iteration_desc);

    NPEReg* associative_regs_dump_curr_ptr = test_desc.associative_regs_dump_start_ptr;
    for(i=0;i<iterations;i++) {
        rumboot_printf("Deciding DMA Configuration for iteration %d\n",i);

        // Prepare Stuff
        nu_npe_iteration_start(&iteration_desc);

        // Fill The cfg_mpe Fields That Are Not Loaded From File
        if(nu_mpe_decide_dma_config(
            iteration_desc.cfg_mpe,
            iteration_desc.in_metrics,
            iteration_desc.warr_metrics,
            iteration_desc.in_data,
            iteration_desc.warr,
            test_desc.mpe_cfg_lut )!=0) return -1;

        rumboot_printf("Fill The cfg_mpe Fields That Are Not Loaded From File\n");
          // mpe_out_metrics - Metrics Of VPE Input Data (Not Seen In Memory) Needed For Configuring VPE Lower
        nu_calc_mpe_cube_out_metrics(iteration_desc.cfg_mpe, &iteration_desc.mpe_out_metrics); 
    
          // Fill The cfg_vpe Fields That Are Not Loaded From File
        nu_vpe_decide_dma_config (
            iteration_desc.cfg_vpe,
           &iteration_desc.mpe_out_metrics, // We Recalculate This Struct Each Iteration
            axi_len,
            NULL,
            iteration_desc.op0,
            iteration_desc.op1,
            iteration_desc.op2,
            //~ iteration_desc.res_metrics,
            iteration_desc.res_data,
            iteration_desc.PPE_ENABLED,
            i==iterations-1);
    
          // Fill The cfg_reg_ppe That Is Not Loaded From File
        if(iteration_desc.PPE_ENABLED==Enable_En)
            nu_ppe_decide_dma_config (
                iteration_desc.cfg_ppe,
                iteration_desc.cfg_reg_ppe,
                iteration_desc.res_metrics,
                NULL,
                iteration_desc.res_data
            );

        nu_npe_add_depend_rd_after_wr(&test_desc, i);
        nu_npe_add_depend_wr_after_rd(&test_desc, i);
    
        nu_setup_next_regs_dump(iteration_desc);
        //nu_mpe_print_reg_map((uintptr_t)iteration_desc.next_regs_dump->mpe);
        //nu_vpe_print_reg_map((uintptr_t)iteration_desc.next_regs_dump->vpe, iteration_desc.cfg_vpe->op2_config.lut_en);
        //if(iteration_desc.PPE_ENABLED==Enable_En) {
        //    nu_ppe_rdma_print_reg_map((uintptr_t)iteration_desc.next_regs_dump->ppe_rdma);
        //    nu_ppe_wdma_print_reg_map((uintptr_t)iteration_desc.next_regs_dump->ppe_wdma);
        //}
        associative_regs_dump_curr_ptr = nu_npe_add_diff_reg_map(associative_regs_dump_curr_ptr, iteration_desc, i);

          // Point At The Next Iteration Data
        nu_npe_iterate_desc(&iteration_desc);
    }
  
    test_desc.associative_regs_dump_end_ptr = associative_regs_dump_curr_ptr;

    uint32_t associative_regs_dump_byte_size = (
        test_desc.associative_regs_dump_end_ptr -
        test_desc.associative_regs_dump_start_ptr
    )*sizeof(NPEReg);

    //nu_npe_run(NPE_BASE, test_desc.associative_regs_dump_start_ptr, test_desc.associative_regs_dump_end_ptr);
    nu_npe_cmd_dma_setup(NPE_BASE, (uint32_t)test_desc.associative_regs_dump_start_ptr, associative_regs_dump_byte_size);
    nu_npe_cmd_dma_run(NPE_BASE);
    nu_npe_cmd_dma_wait_page_complete(NPE_BASE);

    // Wait For The Corresponding DMA Channels To Complete
    if(test_desc.PPE_ENABLED==Enable_En)
        nu_ppe_wait_complete(MY_PPE_REGS_BASE);
    else
        nu_vpe_wait(MY_VPE_REGS_BASE, &test_desc.array_of_cfg_vpe[iterations-1]);


      // Init It Again - We Start Iterate From The Beginning
    nu_npe_init_iteration_desc(&test_desc,&iteration_desc);
    for(i = 0; i < iterations; i++) {
        rumboot_printf("Comparing iteration %d..\n",i);
        nu_npe_iteration_start(&iteration_desc);

          // Result vs Etalon Comparision
        if(nu_bitwise_compare(iteration_desc.res_data, iteration_desc.etalon, iteration_desc.res_metrics->s) == 0)
            rumboot_printf("Iteration %d PASSED\n",i);
        else {
            nu_mpe_print_config(iteration_desc.cfg_mpe);
            nu_vpe_print_config(iteration_desc.cfg_vpe);
            if(iteration_desc.PPE_ENABLED==Enable_En)
                nu_ppe_print_config(iteration_desc.cfg_ppe);
            rumboot_printf("Test FAILED at iteration %d\n",i);
            return 1;
        }
      // Point At The Next Iteration Data
        nu_npe_iterate_desc(&iteration_desc);
    }

    rumboot_printf("Test Passed\n");
    return 0;
}

