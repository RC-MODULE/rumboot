
#include <stdint.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <regs/regs_na.h>

#include <devices/rcm_na/nu_cpdmac_lib.h>
#include <devices/rcm_na/nu_lib.h>


void nu_vpe_load_config(ConfigVPE* cfg, void* cfg_bin) {
  int32_t* ptr;

  ptr = (int32_t*) cfg_bin;

  cfg->in_data_type=*ptr;ptr++;
  cfg->out_data_type=*ptr;ptr++;
  cfg->op0_en=*ptr;ptr++;
  cfg->op1_en=*ptr;ptr++;
  cfg->op2_en=*ptr;ptr++;
  cfg->c3_offset=*ptr;ptr++;
  cfg->c3_scale=*ptr;ptr++;
  cfg->c3_trunc=*ptr;ptr++;
  cfg->c3_satur_en=*ptr;ptr++;
  cfg->c3_round_mode=*ptr;ptr++;
  cfg->nan_to_zero_input=*ptr;ptr++;
  cfg->nan_to_zero_output=*ptr;ptr++;
  cfg->op0_config.coef_type=*ptr;ptr++;
  cfg->op0_config.alu_en=*ptr;ptr++;
  cfg->op0_config.mux_en=*ptr;ptr++;
  cfg->op0_config.relu_en=*ptr;ptr++;
  cfg->op0_config.prelu_en=*ptr;ptr++;
  cfg->op0_config.alu_operation=*ptr;ptr++;
  cfg->op0_config.lshift_value=*ptr;ptr++;
  cfg->op0_config.alu_mode=*ptr;ptr++;
  cfg->op0_config.alu_value=*ptr;ptr++;
  cfg->op0_config.mux_mode=*ptr;ptr++;
  cfg->op0_config.mux_value=*ptr;ptr++;
  cfg->op0_config.norm_round_mode=*ptr;ptr++;
  cfg->op0_config.norm_saturation_en=*ptr;ptr++;
  cfg->op0_config.norm_round_size=*ptr;ptr++;
  cfg->op1_config.coef_type=*ptr;ptr++;
  cfg->op1_config.alu_en=*ptr;ptr++;
  cfg->op1_config.mux_en=*ptr;ptr++;
  cfg->op1_config.relu_en=*ptr;ptr++;
  cfg->op1_config.prelu_en=*ptr;ptr++;
  cfg->op1_config.alu_operation=*ptr;ptr++;
  cfg->op1_config.lshift_value=*ptr;ptr++;
  cfg->op1_config.alu_mode=*ptr;ptr++;
  cfg->op1_config.alu_value=*ptr;ptr++;
  cfg->op1_config.mux_mode=*ptr;ptr++;
  cfg->op1_config.mux_value=*ptr;ptr++;
  cfg->op1_config.norm_round_mode=*ptr;ptr++;
  cfg->op1_config.norm_saturation_en=*ptr;ptr++;
  cfg->op1_config.norm_round_size=*ptr;ptr++;
  cfg->op2_config.coef_type=*ptr;ptr++;
  cfg->op2_config.alu_en=*ptr;ptr++;
  cfg->op2_config.mux_en=*ptr;ptr++;
  cfg->op2_config.lut_en=*ptr;ptr++;
  cfg->op2_config.prelu_en=*ptr;ptr++;
  cfg->op2_config.alu_operation=*ptr;ptr++;
  cfg->op2_config.c1_en=*ptr;ptr++;
  cfg->op2_config.c2_en=*ptr;ptr++;
  cfg->op2_config.alu_mode=*ptr;ptr++;
  cfg->op2_config.alu_value=*ptr;ptr++;
  cfg->op2_config.mux_mode=*ptr;ptr++;
  cfg->op2_config.mux_value=*ptr;ptr++;
  cfg->op2_config.norm_round_mode=*ptr;ptr++;
  cfg->op2_config.norm_saturation_en=*ptr;ptr++;
  cfg->op2_config.norm_round_size=*ptr;ptr++;
  cfg->op2_config.c1_offset=*ptr;ptr++;
  cfg->op2_config.c1_scale=*ptr;ptr++;
  cfg->op2_config.c1_trunc=*ptr;ptr++;
  cfg->op2_config.c2_offset=*ptr;ptr++;
  cfg->op2_config.c2_scale=*ptr;ptr++;
  cfg->op2_config.c2_trunc=*ptr;ptr++;
  cfg->op2_config.lut_sel=*ptr;ptr++;
  cfg->op2_config.lut_right_priority=*ptr;ptr++;
  cfg->op2_config.lut_left_priority=*ptr;ptr++;
  cfg->op2_config.lut_tab1_x_start=*ptr;ptr++;
  cfg->op2_config.lut_tab1_x_end=*ptr;ptr++;
  cfg->op2_config.lut_tab2_x_start=*ptr;ptr++;
  cfg->op2_config.lut_tab2_x_end=*ptr;ptr++;
  cfg->op2_config.lut_tab1_slope_ovrf_scale=*ptr;ptr++;
  cfg->op2_config.lut_tab1_slope_undf_scale=*ptr;ptr++;
  cfg->op2_config.lut_tab2_slope_ovrf_scale=*ptr;ptr++;
  cfg->op2_config.lut_tab2_slope_undf_scale=*ptr;ptr++;
  cfg->op2_config.lut_tab1_slope_ovrf_shift=*ptr;ptr++;
  cfg->op2_config.lut_tab1_slope_undf_shift=*ptr;ptr++;
  cfg->op2_config.lut_tab2_slope_ovrf_shift=*ptr;ptr++;
  cfg->op2_config.lut_tab2_slope_undf_shift=*ptr;ptr++;
  cfg->op2_config.lut_mrr_en=*ptr;ptr++;
  cfg->op2_config.lut_mrr_type=*ptr;ptr++;
  cfg->op2_config.lut_xoffset=*ptr;ptr++;
  cfg->op2_config.lut_yoffset=*ptr;ptr++;
  cfg->ninf_cnt_en=*ptr;ptr++;
  cfg->ovrf_cnt_en=*ptr;ptr++;
  cfg->lut_cnt_en=*ptr;ptr++;

}

void nu_vpe_load_status_regs(StatusRegs* status_regs, void* status_regs_bin) {
  uint32_t* ptr;
  
  ptr = (uint32_t*) status_regs_bin;
  
  status_regs->cmp_result=*ptr;ptr++;
  status_regs->inf_num_in=*ptr;ptr++;
  status_regs->nan_num_in=*ptr;ptr++;
  status_regs->nan_num_out=*ptr;ptr++;
  status_regs->satur_num_out=*ptr;ptr++;
  status_regs->lut_cnt_ovrf=*ptr;ptr++;
  status_regs->lut_cnt_undf=*ptr;ptr++;
  status_regs->lut_cnt_hit_tab0=*ptr;ptr++;
  status_regs->lut_cnt_hit_tab1=*ptr;ptr++;
  status_regs->lut_cnt_hit_hybr=*ptr;ptr++;
  
}

void nu_mpe_load_config(ConfigMPE* cfg, void* cfg_bin) {
  uint32_t* ptr;

  ptr = (uint32_t*) cfg_bin;

  cfg->H          =*ptr;ptr++;
  cfg-> W         =*ptr;ptr++;
  cfg-> C         =*ptr;ptr++;
  cfg-> Tp        =*ptr;ptr++;
  cfg-> Bp        =*ptr;ptr++;
  cfg-> Lp        =*ptr;ptr++;
  cfg-> Rp        =*ptr;ptr++;
  cfg-> R         =*ptr;ptr++;
  cfg-> S         =*ptr;ptr++;
  cfg-> Dr        =*ptr;ptr++;
  cfg-> Ds        =*ptr;ptr++;
  cfg-> Sh        =*ptr;ptr++;
  cfg-> Sw        =*ptr;ptr++;
  cfg-> K         =*ptr;ptr++;
  cfg-> dt        =*ptr;ptr++;
  cfg-> rnd_mode  =*ptr;ptr++;
  cfg-> sat_en    =*ptr;ptr++;
  cfg->rnd_size   =*ptr;ptr++;
  cfg->batch_size =*ptr;ptr++;
}

void nu_ppe_load_config(ConfigPPE* cfg, void* cfg_bin) {
  uint32_t* ptr;

  ptr = (uint32_t*) cfg_bin;

    cfg-> H     =*ptr;ptr++;
    cfg-> W     =*ptr;ptr++;
    cfg-> C     =*ptr;ptr++;
    cfg-> Kh    =*ptr;ptr++;
    cfg-> Kw    =*ptr;ptr++;
    cfg-> Sh    =*ptr;ptr++;
    cfg-> Sw    =*ptr;ptr++;
    cfg-> Tp    =*ptr;ptr++;
    cfg-> Bp    =*ptr;ptr++;
    cfg-> Lp    =*ptr;ptr++;
    cfg-> Rp    =*ptr;ptr++;

    cfg-> Kwh_r  =*ptr;ptr++;

    cfg-> pv[0] =*ptr;ptr++;
    cfg-> pv[1] =*ptr;ptr++;
    cfg-> pv[2] =*ptr;ptr++;
    cfg-> pv[3] =*ptr;ptr++;
    cfg-> pv[4] =*ptr;ptr++;
    cfg-> pv[5] =*ptr;ptr++;
    cfg-> pv[6] =*ptr;ptr++;

    cfg-> meth  =*ptr;ptr++;
    cfg-> dt    =*ptr;ptr++;

    cfg-> nan_to_zero_input = *ptr;ptr++;
}

int nu_ppe_reg_load_config (ConfigREGPPE* cfg_reg, void* cfg_reg_bin) {
  int res = 0;

  uint32_t* ptr = NULL;

  if (cfg_reg == NULL || cfg_reg_bin == NULL) res = 1;

  if (!res) {
    ptr = (uint32_t*) cfg_reg_bin;

    cfg_reg->rOpEn  = *ptr; ptr++;
    cfg_reg->rAXIp  = *ptr; ptr++;
    cfg_reg->rBALi  = *ptr; ptr++;
    cfg_reg->rBrdX  = *ptr; ptr++;
    cfg_reg->rBrdY  = *ptr; ptr++;
    cfg_reg->rBrdZ  = *ptr; ptr++;
    cfg_reg->rStrX  = *ptr; ptr++;
    cfg_reg->rStrY  = *ptr; ptr++;
    cfg_reg->rStrZ  = *ptr; ptr++;
    cfg_reg->rFrgs  = *ptr; ptr++;
    cfg_reg->rFrgl  = *ptr; ptr++;
    cfg_reg->rXYZd  = *ptr; ptr++;
    cfg_reg->rBstX  = *ptr; ptr++;
    cfg_reg->rBstY  = *ptr; ptr++;
    cfg_reg->rBstZ  = *ptr; ptr++;
    cfg_reg->rBxtX  = *ptr; ptr++;
    cfg_reg->rBxtY  = *ptr; ptr++;
    cfg_reg->rBxtZ  = *ptr; ptr++;
    cfg_reg->rBffX  = *ptr; ptr++;
    cfg_reg->rBffY  = *ptr; ptr++;
    cfg_reg->rBffZ  = *ptr; ptr++;
    
    cfg_reg->wOpEn  = *ptr; ptr++;
    cfg_reg->wAXIp  = *ptr; ptr++;
    cfg_reg->wBALo  = *ptr; ptr++;
    cfg_reg->wBrdX  = *ptr; ptr++;
    cfg_reg->wBrdY  = *ptr; ptr++;
    cfg_reg->wBrdZ  = *ptr; ptr++;
    cfg_reg->wStrX  = *ptr; ptr++;
    cfg_reg->wStrY  = *ptr; ptr++;
    cfg_reg->wStrZ  = *ptr; ptr++;
    cfg_reg->wFrgs  = *ptr; ptr++;
    cfg_reg->wFrgl  = *ptr; ptr++;
    cfg_reg->wXYZd  = *ptr; ptr++;
    cfg_reg->wBstX  = *ptr; ptr++;
    cfg_reg->wBstY  = *ptr; ptr++;
    cfg_reg->wBstZ  = *ptr; ptr++;
    cfg_reg->wBxtX  = *ptr; ptr++;
    cfg_reg->wBxtY  = *ptr; ptr++;
    cfg_reg->wBxtZ  = *ptr; ptr++;
    cfg_reg->wBffX  = *ptr; ptr++;
    cfg_reg->wBffY  = *ptr; ptr++;
    cfg_reg->wBffZ  = *ptr; ptr++;
    cfg_reg->wIstX  = *ptr; ptr++;
    cfg_reg->wIxtX  = *ptr; ptr++;
    cfg_reg->wIffX  = *ptr; ptr++;
    cfg_reg->wWi    = *ptr; ptr++;
    cfg_reg->wHi    = *ptr; ptr++;
    cfg_reg->wCi    = *ptr; ptr++;
    cfg_reg->wWo    = *ptr; ptr++;
    cfg_reg->wHo    = *ptr; ptr++;
    cfg_reg->wCo    = *ptr; ptr++;
    cfg_reg->wOpM   = *ptr; ptr++;
    cfg_reg->wK     = *ptr; ptr++;

    cfg_reg->wKWHr   = *ptr; ptr++;

    cfg_reg->wP     = *ptr; ptr++;
    cfg_reg->wPV1   = *ptr; ptr++;
    cfg_reg->wPV2   = *ptr; ptr++;
    cfg_reg->wPV3   = *ptr; ptr++;
    cfg_reg->wPV4   = *ptr; ptr++;
    cfg_reg->wPV5   = *ptr; ptr++;
    cfg_reg->wPV6   = *ptr; ptr++;
    cfg_reg->wPV7   = *ptr; ptr++;
  }

  return res;
}

  void nu_vpe_print_Mode(Mode mode, char* name) {
    static char* ModeNames[]= {
      "Unitary",
      "X",   // Because Of Mode Coding (0, 2, 3)
      "Channel",
      "Element"
    };

    rumboot_printf("  %s = %s \n", name, ModeNames[(int) mode] );
  }


  void nu_vpe_print_RoundMode(RoundMode mode, char* name) {
    static char* RoundModeNames[]= {
      "Down",
      "TowardsZero",
      "Up",
      "AwayFromZero",
      "HalfDown",
      "HalfTowardsZero",
      "Nearest", // Half Up
      "HalfAwayFromZero"
    };

    rumboot_printf("  %s = %s \n", name, RoundModeNames[(int) mode] );
  }
  
  void nu_vpe_print_TraceMode(TraceMode mode, char* name) {
    static char* TraceModeNames[]= {
      "TraceMode_PPE",
      "TraceMode_MPE"
    };
  
    rumboot_printf("  %s = %s \n", name, TraceModeNames[(int) mode] );
  }


  void nu_vpe_print_Enable(Enable enable, char* name) {
    static char* EnableNames[]= {
      "NotEn",
      "En"
    };

    rumboot_printf("  %s = %s \n", name, EnableNames[(int) enable] );
  }


  void nu_vpe_print_AluOperationSwitch(AluOperationSwitch operation, char* name) {
    static char* AluOperationSwitchNames[]= {
      "Max",
      "Min",
      "Sum",
      "Sub"
    };

    rumboot_printf("  %s = %s \n", name, AluOperationSwitchNames[(int) operation] );
  }


  void nu_vpe_print_AluOperationExtSwitch(AluOperationExtSwitch operation, char* name) {
    static char* AluOperationExtSwitchNames[]= {
      "Max",
      "Min",
      "Sum",
      "Sub",
      "Eql"
    };

    rumboot_printf("  %s = %s \n", name, AluOperationExtSwitchNames[(int) operation] );
  }


  void nu_vpe_print_PoolingOperationSwitch(PoolingOperationSwitch operation, char* name) {
    static char* PoolingOperationSwitchNames[]= {
      "Avg",
      "Max",
      "Min"
    };

    rumboot_printf("  %s = %s \n", name, PoolingOperationSwitchNames[(int) operation] );
  }


  void nu_vpe_print_DataType(DataType data_type, char* name) {
    static char* DataTypeNames[]= {
      "Int8",
      "Int16",
      "Fp16"
    };

    rumboot_printf("  %s = %s \n", name, DataTypeNames[(int) data_type] );
  }


  void nu_vpe_print_DataTypeExt(DataTypeExt data_type, char* name) {
    static char* DataTypeExtNames[]= {
      "Int8",
      "Int16",
      "Fp16",
      "Int32",
      "Fp32"
    };

    rumboot_printf("  %s = %s \n", name, DataTypeExtNames[(int) data_type] );
  }
  
  void nu_vpe_print_DmaRamType(DmaRamType rt, char* name) {
    static char* DmaRamTypeNames[] = {
      "CV",
      "MC"
    };
    
    rumboot_printf("  %s = %s \n", name,DmaRamTypeNames[(int) rt] );
  }
  
  void nu_vpe_print_DmaDSizeType(DmaDSizeType st, char* name) {
    static char* DmaDSizeTypeNames[] = {
      "DmaDSize_One_Byte",
      "DmaDSize_Two_Byte"
    };
    
    rumboot_printf("  %s = %s \n",name,DmaDSizeTypeNames[(int) st] );
  }
  
  void nu_vpe_print_DmaDUseType(DmaDUseType ut, char* name) {
    static char* DmaDUseTypeNames[] =  {
      "Off",
      "Mux",
      "Alu",
      "Both"
    };
    
    rumboot_printf("  %s = %s \n", name, DmaDUseTypeNames[(int) ut]);
  }
  void nu_vpe_print_DmaXYZDirectionType(DmaXYZDirectionType ut, char* name) {
    static char* DmaXYZDirectionTypeNames[] = {
      "DmaXYZDirection_X",
      "DmaXYZDirection_Y",
      "DmaXYZDirection_Z"
    };
    
    rumboot_printf("  %s = %s \n",name,DmaXYZDirectionTypeNames[(int) ut]);
  }


void nu_vpe_print_config(ConfigVPE* cfg){
#ifndef NU_NO_PRINT
  rumboot_printf("ConfigVPE:\n");
  nu_vpe_print_Enable(cfg->src_flying,"src_flying(data from MPE)");
  nu_vpe_print_Enable(cfg->dst_flying,"dst_flying(data to PPE)");
  nu_vpe_print_DataTypeExt(cfg->in_data_type,"in_data_type");
  nu_vpe_print_DataType(cfg->out_data_type,"out_data_type");
  nu_vpe_print_Enable(cfg->mark,"mark");
  rumboot_printf("  cube_size = %d\n" , cfg->cube_size);
  rumboot_printf("  depend_mask = 0x%x\n" , cfg->depend_mask);
  nu_vpe_print_Enable(cfg->op0_en,"op0_en");
  nu_vpe_print_Enable(cfg->op1_en,"op1_en");
  nu_vpe_print_Enable(cfg->op2_en,"op2_en");
  rumboot_printf("  c3_offset = %d\n" , cfg->c3_offset);
  rumboot_printf("  c3_scale = %d\n" , cfg->c3_scale);
  rumboot_printf("  c3_trunc = %d\n" , cfg->c3_trunc);
  rumboot_printf("  c3_satur_en = %d\n" , cfg->c3_satur_en  );
  nu_vpe_print_RoundMode( cfg->c3_round_mode, "c3_round_mode");
  nu_vpe_print_Enable(cfg->nan_to_zero_input,"nan_to_zero_input");
  nu_vpe_print_Enable(cfg->nan_to_zero_output,"nan_to_zero_output");
  nu_vpe_print_TraceMode(cfg->trace_mode,"trace_mode");

  rumboot_printf("ConfigOp01 (0):\n");

  nu_vpe_print_DataType(cfg->op0_config.coef_type,"coef_type");
  nu_vpe_print_Enable(cfg->op0_config.alu_en,"alu_en");
  nu_vpe_print_Enable(cfg->op0_config.mux_en,"mux_en");
  nu_vpe_print_Enable(cfg->op0_config.relu_en,"relu_en");
  nu_vpe_print_Enable(cfg->op0_config.prelu_en,"prelu_en");
  nu_vpe_print_AluOperationSwitch(cfg->op0_config.alu_operation,"alu_operation");
  rumboot_printf("  lshift_value = %d\n" , cfg->op0_config.lshift_value);
  nu_vpe_print_Mode(cfg->op0_config.alu_mode,"alu_mode");
  rumboot_printf("  alu_value = %d\n" , cfg->op0_config.alu_value);
  nu_vpe_print_Mode(cfg->op0_config.mux_mode,"mux_mode");
  rumboot_printf("  mux_value = %d\n" , cfg->op0_config.mux_value);
  nu_vpe_print_RoundMode(cfg->op0_config.norm_round_mode,"norm_round_mode");
  nu_vpe_print_Enable(cfg->op0_config.norm_saturation_en,"norm_saturation_en");
  rumboot_printf("  norm_round_size = %d\n" , cfg->op0_config.norm_round_size);

  rumboot_printf("ConfigOp01 (1):\n");
  nu_vpe_print_DataType(cfg->op1_config.coef_type,"coef_type");
  nu_vpe_print_Enable(cfg->op1_config.alu_en,"alu_en");
  nu_vpe_print_Enable(cfg->op1_config.mux_en,"mux_en");
  nu_vpe_print_Enable(cfg->op1_config.relu_en,"relu_en");
  nu_vpe_print_Enable(cfg->op1_config.prelu_en,"prelu_en");
  nu_vpe_print_AluOperationSwitch(cfg->op1_config.alu_operation,"alu_operation");
  rumboot_printf("  lshift_value = %d\n" , cfg->op1_config.lshift_value);
  nu_vpe_print_Mode(cfg->op1_config.alu_mode,"alu_mode");
  rumboot_printf("  alu_value = %d\n" , cfg->op1_config.alu_value);
  nu_vpe_print_Mode(cfg->op1_config.mux_mode,"mux_mode");
  rumboot_printf("  mux_value = %d\n" , cfg->op1_config.mux_value);
  nu_vpe_print_RoundMode(cfg->op1_config.norm_round_mode,"norm_round_mode");
  nu_vpe_print_Enable(cfg->op1_config.norm_saturation_en,"norm_saturation_en");
  rumboot_printf("  norm_round_size = %d\n" , cfg->op1_config.norm_round_size);

  rumboot_printf("ConfigOp2:\n");
  nu_vpe_print_DataType(cfg->op2_config.coef_type,"coef_type");
  nu_vpe_print_Enable(cfg->op2_config.alu_en,"alu_en");
  nu_vpe_print_Enable(cfg->op2_config.mux_en,"mux_en");
  nu_vpe_print_Enable(cfg->op2_config.lut_en,"lut_en");
  nu_vpe_print_Enable(cfg->op2_config.prelu_en,"prelu_en");
  nu_vpe_print_AluOperationExtSwitch(cfg->op2_config.alu_operation,"alu_operation");
  nu_vpe_print_Enable(cfg->op2_config.c1_en,"c1_en");
  nu_vpe_print_Enable(cfg->op2_config.c2_en,"c2_en");
  nu_vpe_print_Mode(cfg->op2_config.alu_mode,"alu_mode");
  rumboot_printf("  alu_value = %d\n" , cfg->op2_config.alu_value);
  nu_vpe_print_Mode(cfg->op2_config.mux_mode,"mux_mode");
  rumboot_printf("  mux_value = %d\n" , cfg->op2_config.mux_value);
  nu_vpe_print_RoundMode(cfg->op2_config.norm_round_mode,"norm_round_mode");
  nu_vpe_print_Enable(cfg->op2_config.norm_saturation_en,"norm_saturation_en");
  rumboot_printf("  norm_round_size = %d \n" , cfg->op2_config.norm_round_size);
  rumboot_printf("  c1_offset = %d \n" , cfg->op2_config.c1_offset);
  rumboot_printf("  c1_scale = %d \n" , cfg->op2_config.c1_scale);
  rumboot_printf("  c1_trunc = %d \n" , cfg->op2_config.c1_trunc);
  rumboot_printf("  c2_offset = %d \n" , cfg->op2_config.c2_offset);
  rumboot_printf("  c2_scale = %d \n" , cfg->op2_config.c2_scale);
  rumboot_printf("  c2_trunc = %d \n" , cfg->op2_config.c2_trunc);
  rumboot_printf("  lut_sel = %d \n" , cfg->op2_config.lut_sel);
  rumboot_printf("  lut_right_priority = %d \n" , cfg->op2_config.lut_right_priority);
  rumboot_printf("  lut_left_priority = %d \n" , cfg->op2_config.lut_left_priority);
  rumboot_printf("  lut_tab1_x_start = %x \n" , cfg->op2_config.lut_tab1_x_start);
  rumboot_printf("  lut_tab1_x_end = %x \n" , cfg->op2_config.lut_tab1_x_end);
  rumboot_printf("  lut_tab2_x_start = %x \n" , cfg->op2_config.lut_tab2_x_start);
  rumboot_printf("  lut_tab2_x_end = %x \n" , cfg->op2_config.lut_tab2_x_end);
  rumboot_printf("  lut_tab1_slope_ovrf_scale = %x \n" , cfg->op2_config.lut_tab1_slope_ovrf_scale);
  rumboot_printf("  lut_tab1_slope_undf_scale = %x \n" , cfg->op2_config.lut_tab1_slope_undf_scale);
  rumboot_printf("  lut_tab2_slope_ovrf_scale = %x \n" , cfg->op2_config.lut_tab2_slope_ovrf_scale);
  rumboot_printf("  lut_tab2_slope_undf_scale = %x \n" , cfg->op2_config.lut_tab2_slope_undf_scale);
  rumboot_printf("  lut_tab1_slope_ovrf_shift = %d \n" , cfg->op2_config.lut_tab1_slope_ovrf_shift);
  rumboot_printf("  lut_tab1_slope_undf_shift = %d \n" , cfg->op2_config.lut_tab1_slope_undf_shift);
  rumboot_printf("  lut_tab2_slope_ovrf_shift = %d \n" , cfg->op2_config.lut_tab2_slope_ovrf_shift);
  rumboot_printf("  lut_tab2_slope_undf_shift = %d \n" , cfg->op2_config.lut_tab2_slope_undf_shift);
  nu_vpe_print_Enable(cfg->op2_config.lut_mrr_en,"lut_mrr_en");
  rumboot_printf("  lut_mrr_type = %d \n", cfg->op2_config.lut_mrr_type);
  rumboot_printf("  lut_xoffset = %x \n", cfg->op2_config.lut_xoffset);
  rumboot_printf("  lut_yoffset = %x \n", cfg->op2_config.lut_yoffset);
  
  nu_vpe_print_Enable(cfg->ninf_cnt_en,"ninf_cnt_en");
  nu_vpe_print_Enable(cfg->ovrf_cnt_en,"ovrf_cnt_en");
  nu_vpe_print_Enable(cfg->lut_cnt_en,"lut_cnt_en");

  nu_print_config_dma(&(cfg->src_rdma_config),"src_rdma_config");
  nu_print_config_dma(&(cfg->op0_rdma_config),"op0_rdma_config");
  nu_print_config_dma(&(cfg->op1_rdma_config),"op1_rdma_config");
  nu_print_config_dma(&(cfg->op2_rdma_config),"op2_rdma_config");
  nu_print_config_dma(&(cfg->wdma_config),"wdma_config");
#endif // NU_NO_PRINT
}

void nu_print_config_dma(ConfigDMA * cfg_dma,char* name) {
#ifndef NU_NO_PRINT
  rumboot_printf("ConfigDMA (%s):\n",name);
  nu_vpe_print_Enable(cfg_dma->dma_en,"dma_en");
  rumboot_printf("  dma_cube_size_h = %d\n",cfg_dma->dma_cube_size_h);
  rumboot_printf("  dma_cube_size_w = %d\n",cfg_dma->dma_cube_size_w);
  rumboot_printf("  dma_cube_size_c = %d\n",cfg_dma->dma_cube_size_c);
  nu_vpe_print_DmaRamType(cfg_dma->dma_ram_type,"dma_ram_type");
  nu_vpe_print_Mode(cfg_dma->dma_data_mode,"dma_data_mode");
  nu_vpe_print_DmaDSizeType(cfg_dma->dma_data_size,"dma_data_size");
  nu_vpe_print_DmaDUseType(cfg_dma->dma_data_use,"dma_data_use");
  rumboot_printf("  dma_baddr = 0x%x\n",cfg_dma->dma_baddr);
  nu_vpe_print_DmaXYZDirectionType(cfg_dma->dma_xyz_drct,"dma_xyz_drct");
  rumboot_printf("  dma_border_x = %d\n",cfg_dma->dma_border_x);
  rumboot_printf("  dma_border_y = %d\n",cfg_dma->dma_border_y);
  rumboot_printf("  dma_border_z = %d\n",cfg_dma->dma_border_z);
  rumboot_printf("  dma_stride_y = %d\n",cfg_dma->dma_stride_y);
  rumboot_printf("  dma_stride_x = %d\n",cfg_dma->dma_stride_x);
  rumboot_printf("  dma_stride_z = %d\n",cfg_dma->dma_stride_z);
  rumboot_printf("  dma_box_st_size_x = %d\n",cfg_dma->dma_box_st_size_x);
  rumboot_printf("  dma_box_st_size_y = %d\n",cfg_dma->dma_box_st_size_y);
  rumboot_printf("  dma_box_st_size_z = %d\n",cfg_dma->dma_box_st_size_z);
  rumboot_printf("  dma_box_size_x = %d\n",cfg_dma->dma_box_size_x);
  rumboot_printf("  dma_box_size_y = %d\n",cfg_dma->dma_box_size_y);
  rumboot_printf("  dma_box_size_z = %d\n",cfg_dma->dma_box_size_z);
  rumboot_printf("  dma_box_offset_x = %d\n",cfg_dma->dma_box_offset_x);
  rumboot_printf("  dma_box_offset_y = %d\n",cfg_dma->dma_box_offset_y);
  rumboot_printf("  dma_box_offset_z = %d\n",cfg_dma->dma_box_offset_z);
  rumboot_printf("  dma_bsize = %d\n",cfg_dma->dma_bsize);
  rumboot_printf("  dma_bstride = 0x%x\n",cfg_dma->dma_bstride);
  rumboot_printf("  dma_axi_len = %d\n",cfg_dma->dma_axi_len);
#endif // NU_NO_PRINT
  
}

void nu_vpe_print_status_regs_etalon(StatusRegs* status_regs) {
#ifndef NU_NO_PRINT
  rumboot_printf("VPEStatusRegs:\n");
  rumboot_printf("  cmp_result = %d\n",status_regs->cmp_result);
  rumboot_printf("  inf_num_in = %d\n",status_regs->inf_num_in);
  rumboot_printf("  nan_num_in = %d\n",status_regs->nan_num_in);
  rumboot_printf("  nan_num_out = %d\n",status_regs->nan_num_out);
  rumboot_printf("  satur_num_out = %d\n",status_regs->satur_num_out);
  rumboot_printf("  lut_cnt_ovrf = %d\n",status_regs->lut_cnt_ovrf);
  rumboot_printf("  lut_cnt_undf = %d\n",status_regs->lut_cnt_undf);
  rumboot_printf("  lut_cnt_hit_tab0 = %d\n",status_regs->lut_cnt_hit_tab0);
  rumboot_printf("  lut_cnt_hit_tab1 = %d\n",status_regs->lut_cnt_hit_tab1);
  rumboot_printf("  lut_cnt_hit_hybr = %d\n",status_regs->lut_cnt_hit_hybr);
#endif // NU_NO_PRINT
}



void nu_mpe_print_ConfigRDDMAMPEBias(ConfigRDDMAMPEBias* bias,int index) {
  rumboot_printf("ConfigRDDMAMPEBias(%d):\n",index);
    nu_vpe_print_Enable(bias->BiasEn,"BiasEn");

    if (index < 6) {
      rumboot_printf("  ThreCtrl = %d \n", bias->ThreCtrl);
      rumboot_printf("  DecCtrl = %d \n", bias->DecCtrl);
    }

    nu_vpe_print_Enable(bias->PXBSEn,"PXBSEn");
    nu_vpe_print_Enable(bias->PYBSEn,"PYBSEn");
    rumboot_printf("  Bias = 0x%x \n",bias->Bias);

    rumboot_printf("  CntSha = %d \n",bias->CntSha);

    if (index < 6) {
      nu_vpe_print_Enable(bias->CntOffsetEn,"CntOffsetEn");
      rumboot_printf("  CntOffset = %d \n",bias->CntOffset);
    }

    rumboot_printf("  CntThresholdSha = %d \n",bias->CntThresholdSha);   
    rumboot_printf("  CntCmp = %d \n",bias->CntCmp);
}

void nu_mpe_print_ConfigWRDMAMPEBias(ConfigWRDMAMPEBias* bias,int index) {
  rumboot_printf("ConfigWRDMAMPEBias(%d):\n",index);
    rumboot_printf("  CntSha = %d \n",bias->CntSha);
    rumboot_printf("  CntCmp = %d \n",bias->CntCmp);
}

void nu_mpe_print_ConfigRDDMAMPE(ConfigRDDMAMPE* cfg,char* name) {
  rumboot_printf("ConfigRDDMAMPE(%s):\n",name);
    rumboot_printf("  BFCA = 0x%x\n",cfg->BFCA);
    rumboot_printf("  AOffset = 0x%x\n",cfg->AOffset);
    rumboot_printf("  LPXOffset = %d \n",cfg->LPXOffset);
    rumboot_printf("  RPXOffset = %d \n",cfg->RPXOffset);
    rumboot_printf("  TPYOffset = %d \n",cfg->TPYOffset);
    rumboot_printf("  BPYOffset = %d \n",cfg->BPYOffset);
    rumboot_printf("  CntSha = %d \n",cfg->CntSha);
    rumboot_printf("  CntThresholdSha = %d \n",cfg->CntThresholdSha);
    rumboot_printf("  ThreCtrl = %d \n",cfg->ThreCtrl);
    nu_vpe_print_Enable(cfg->LPXEn,"LPXEn");
    nu_vpe_print_Enable(cfg->RPXEn,"RPXEn");
    nu_vpe_print_Enable(cfg->TPYEn,"TPYEn");
    nu_vpe_print_Enable(cfg->BPYEn,"BPYEn");
    rumboot_printf("  LPXData = 0x%x\n",cfg->LPXData);
    rumboot_printf("  RPXData = 0x%x\n",cfg->RPXData);
    rumboot_printf("  TPYData = 0x%x\n",cfg->TPYData);
    rumboot_printf("  BPYData = 0x%x\n",cfg->BPYData);
  
  for(int i=0;i<7;i++) {
    nu_mpe_print_ConfigRDDMAMPEBias(& (cfg->Bias[i]),i);
  }
  
}

void nu_mpe_print_ConfigWRDMAMPE(ConfigWRDMAMPE* cfg, char* name) {
  rumboot_printf("ConfigWRDMAMPE(%s):\n",name);
    rumboot_printf("  BADR = %d\n",cfg->BADR);
    rumboot_printf("  LADR = %d\n",cfg->LADR);
    nu_vpe_print_Enable(cfg->USED,"USED" );
    nu_vpe_print_Enable(cfg->ADR_PROL,"ADR_PROL" );
    nu_vpe_print_Enable(cfg->C_BND,"C_BND" );
    nu_vpe_print_Enable(cfg->GLUE_EN,"GLUE_EN" );
    rumboot_printf("  BStrideX = %d\n",cfg->BStrideX) ;
    rumboot_printf("  BConvX = %d\n",cfg->BConvX) ;
    rumboot_printf("  Batch = %d\n",cfg->Batch) ;
    rumboot_printf("  Thre_PLC = %d\n",cfg->Thre_PLC);
    rumboot_printf("  Thre_VLC = %d\n",cfg->Thre_VLC) ;
    rumboot_printf("  Thre_CXC = %d\n",cfg->Thre_CXC) ;
    rumboot_printf("  Dec_PLC = %d\n",cfg->Dec_PLC);
    rumboot_printf("  PLC_CntSha = %d\n",cfg->PLC_CntSha);
    rumboot_printf("  VLC_CntSha = %d\n",cfg->VLC_CntSha);
    rumboot_printf("  PLC_ThreSha = %d\n",cfg->PLC_ThreSha);
    rumboot_printf("  VLC_ThreSha = %d\n",cfg->VLC_ThreSha);
  for(int i=0;i<6;i++) {
    nu_mpe_print_ConfigWRDMAMPEBias(& (cfg->Bias[i]), i);
  }
  
}

void nu_mpe_print_ConfigDMAMPE(ConfigDMAMPE* cfg,char* name) {
  rumboot_printf("ConfigDMAMPE(%s):\n",name);
  rumboot_printf("  MAINCNT = %d\n",cfg->MAINCNT);
  rumboot_printf("  depend_mask = 0x%x\n",cfg->depend_mask);
  nu_mpe_print_ConfigRDDMAMPE(& cfg->rdma, name);
  nu_mpe_print_ConfigWRDMAMPE(& cfg->wdma, name);
}

void nu_mpe_print_config(ConfigMPE* cfg){
#ifndef NU_NO_PRINT
  rumboot_printf("ConfigMPE:\n");

    rumboot_printf("  H        = %d \n" , cfg->H);
    rumboot_printf("  W        = %d \n" , cfg->W);
    rumboot_printf("  C        = %d \n" , cfg->C);
    rumboot_printf("  Tp       = %d \n" , cfg->Tp);
    rumboot_printf("  Bp       = %d \n" , cfg->Bp);
    rumboot_printf("  Lp       = %d \n" , cfg->Lp);
    rumboot_printf("  Rp       = %d \n" , cfg->Rp);
    rumboot_printf("  R        = %d \n" , cfg->R);
    rumboot_printf("  S        = %d \n" , cfg->S);
    rumboot_printf("  Dr       = %d \n" , cfg->Dr);
    rumboot_printf("  Ds       = %d \n" , cfg->Ds);
    rumboot_printf("  Sw       = %d \n" , cfg->Sw);
    rumboot_printf("  Sh       = %d \n" , cfg->Sh);
    rumboot_printf("  K        = %d \n" , cfg->K);
    nu_vpe_print_DataType(cfg->dt,"dt      ");
    nu_vpe_print_RoundMode(cfg->rnd_mode,"rnd_mode");
    nu_vpe_print_Enable(cfg->sat_en, "sat_en  ");
    rumboot_printf("  rnd_size = %d \n" , cfg->rnd_size);
    rumboot_printf("  batch_size = %d \n" , cfg->batch_size);

  rumboot_printf("ConfigMAMPE:\n");
  
    rumboot_printf("  ADD_CountI0  = %d \n",cfg->ma_config.ADD_CountI0);
    rumboot_printf("  CMP_CountI0  = %d \n",cfg->ma_config.CMP_CountI0);
    rumboot_printf("  ADD_CountI1  = %d \n",cfg->ma_config.ADD_CountI1);
    rumboot_printf("  CMP_CountI1  = %d \n",cfg->ma_config.CMP_CountI1);
    rumboot_printf("  ADD_CountJ   = %d \n",cfg->ma_config.ADD_CountJ);
    rumboot_printf("  CMP_CountJ   = %d \n",cfg->ma_config.CMP_CountJ);
    rumboot_printf("  ADD_CountPPS = %d \n",cfg->ma_config.ADD_CountPPS);
    rumboot_printf("  CMP_CountPPS = %d \n",cfg->ma_config.CMP_CountPPS);
    rumboot_printf("  ADD_CountLCZ = %d \n",cfg->ma_config.ADD_CountLCZ);
    rumboot_printf("  CMP_CountLCZ = %d \n",cfg->ma_config.CMP_CountLCZ);
    rumboot_printf("  ADD_CountPLC = %d \n",cfg->ma_config.ADD_CountPLC);
    rumboot_printf("  CMP_CountPLC = %d \n",cfg->ma_config.CMP_CountPLC);
    rumboot_printf("  ADD_CountX   = %d \n",cfg->ma_config.ADD_CountX);
    rumboot_printf("  CMP_CountX   = %d \n",cfg->ma_config.CMP_CountX);
    rumboot_printf("  ADD_CountY   = %d \n",cfg->ma_config.ADD_CountY);
    rumboot_printf("  CMP_CountY   = %d \n",cfg->ma_config.CMP_CountY);
    rumboot_printf("  ADD_CountM   = %d \n",cfg->ma_config.ADD_CountM);
    rumboot_printf("  CMP_CountM   = %d \n",cfg->ma_config.CMP_CountM);
    
    rumboot_printf("  BRDR   = %d \n",cfg->ma_config.BRDR);
    rumboot_printf("  WA     = %d \n",cfg->ma_config.WA);
    rumboot_printf("  DA     = %d \n",cfg->ma_config.DA);
    rumboot_printf("  VRA    = %d \n",cfg->ma_config.VRA)  ;
    rumboot_printf("  NR     = %d \n",cfg->ma_config.NR)   ;
    rumboot_printf("  D_BIAS = %d \n",cfg->ma_config.D_BIAS);
    rumboot_printf("  D_NULL = %d \n",cfg->ma_config.D_NULL);
    
  nu_mpe_print_ConfigDMAMPE(& (cfg->dma_d_config), "D");
  nu_mpe_print_ConfigDMAMPE(& (cfg->dma_w_config), "W");
#endif // NU_NO_PRINT
}

void nu_mpe_print_config_dma(ConfigDMAMPE* cfg) {
#ifndef NU_NO_PRINT
#endif //NU_NO_PRINT
}

void nu_ppe_print_config(ConfigPPE* cfg){
#ifndef NU_NO_PRINT
  rumboot_printf("ConfigPPE:\n");
    rumboot_printf("  H      = %d \n" , cfg->H);
    rumboot_printf("  W      = %d \n" , cfg->W);
    rumboot_printf("  C      = %d \n" , cfg->C);
    rumboot_printf("  Kh     = %d \n" , cfg->Kh);
    rumboot_printf("  Kw     = %d \n" , cfg->Kw);
    rumboot_printf("  Sh     = %d \n" , cfg->Sh);
    rumboot_printf("  Sw     = %d \n" , cfg->Sw);

    rumboot_printf("  Kwh_r   = 0x%x \n" , cfg->Kwh_r);

    rumboot_printf("  Tp     = %d \n" , cfg->Tp);
    rumboot_printf("  Bp     = %d \n" , cfg->Bp);
    rumboot_printf("  Lp     = %d \n" , cfg->Lp);
    rumboot_printf("  Rp     = %d \n" , cfg->Rp);
    rumboot_printf("  pv     = 0x%x \n" , cfg->pv[0]);
    rumboot_printf("           0x%x \n" , cfg->pv[1]);
    rumboot_printf("           0x%x \n" , cfg->pv[2]);
    rumboot_printf("           0x%x \n" , cfg->pv[3]);
    rumboot_printf("           0x%x \n" , cfg->pv[4]);
    rumboot_printf("           0x%x \n" , cfg->pv[5]);
    rumboot_printf("           0x%x \n" , cfg->pv[6]);
    nu_vpe_print_PoolingOperationSwitch(cfg->meth,"meth    ");
    nu_vpe_print_DataType(cfg->dt,"dt      ");
#endif // NU_NO_PRINT
}

void nu_ppe_print_config_reg(ConfigREGPPE* cfg_reg){
#ifndef NU_NO_PRINT
  rumboot_printf("ConfigREGPPE:\n");

  //rumboot_printf ("rOpEn = %x\n", cfg_reg->rOpEn);
  //rumboot_printf ("rAXIp = %x\n", cfg_reg->rAXIp);
  rumboot_printf ("rBALi = 0x%x\n", cfg_reg->rBALi);
  rumboot_printf ("rBrdX = %d\n", cfg_reg->rBrdX);
  rumboot_printf ("rBrdY = %d\n", cfg_reg->rBrdY);
  rumboot_printf ("rBrdZ = %d\n", cfg_reg->rBrdZ);
  rumboot_printf ("rStrX = %d\n", cfg_reg->rStrX);
  rumboot_printf ("rStrY = %d\n", cfg_reg->rStrY);
  rumboot_printf ("rStrZ = %d\n", cfg_reg->rStrZ);
  rumboot_printf ("rFrgs = %d\n", cfg_reg->rFrgs);
  rumboot_printf ("rFrgl = %d\n", cfg_reg->rFrgl);
  rumboot_printf ("rXYZd = %d\n", cfg_reg->rXYZd);
  rumboot_printf ("rBstX = %d\n", cfg_reg->rBstX);
  rumboot_printf ("rBstY = %d\n", cfg_reg->rBstY);
  rumboot_printf ("rBstZ = %d\n", cfg_reg->rBstZ);
  rumboot_printf ("rBxtX = %d\n", cfg_reg->rBxtX);
  rumboot_printf ("rBxtY = %d\n", cfg_reg->rBxtY);
  rumboot_printf ("rBxtZ = %d\n", cfg_reg->rBxtZ);
  rumboot_printf ("rBffX = %d\n", cfg_reg->rBffX);
  rumboot_printf ("rBffY = %d\n", cfg_reg->rBffY);
  rumboot_printf ("rBffZ = %d\n", cfg_reg->rBffZ);
  
  //rumboot_printf ("wOpEn = %x\n", cfg_reg->wOpEn);
  //rumboot_printf ("wAXIp = %x\n", cfg_reg->wAXIp);
  rumboot_printf ("wBALo = 0x%x\n", cfg_reg->wBALo);
  rumboot_printf ("wBrdX = %d\n", cfg_reg->wBrdX);
  rumboot_printf ("wBrdY = %d\n", cfg_reg->wBrdY);
  rumboot_printf ("wBrdZ = %d\n", cfg_reg->wBrdZ);
  rumboot_printf ("wStrX = %d\n", cfg_reg->wStrX);
  rumboot_printf ("wStrY = %d\n", cfg_reg->wStrY);
  rumboot_printf ("wStrZ = %d\n", cfg_reg->wStrZ);
  rumboot_printf ("wFrgs = %d\n", cfg_reg->wFrgs);
  rumboot_printf ("wFrgl = %d\n", cfg_reg->wFrgl);
  rumboot_printf ("wXYZd = %d\n", cfg_reg->wXYZd);
  rumboot_printf ("wBstX = %d\n", cfg_reg->wBstX);
  rumboot_printf ("wBstY = %d\n", cfg_reg->wBstY);
  rumboot_printf ("wBstZ = %d\n", cfg_reg->wBstZ);
  rumboot_printf ("wBxtX = %d\n", cfg_reg->wBxtX);
  rumboot_printf ("wBxtY = %d\n", cfg_reg->wBxtY);
  rumboot_printf ("wBxtZ = %d\n", cfg_reg->wBxtZ);
  rumboot_printf ("wBffX = %d\n", cfg_reg->wBffX);
  rumboot_printf ("wBffY = %d\n", cfg_reg->wBffY);
  rumboot_printf ("wBffZ = %d\n", cfg_reg->wBffZ);
  rumboot_printf ("wIstX = %d\n", cfg_reg->wIstX);
  rumboot_printf ("wIxtX = %d\n", cfg_reg->wIxtX);
  rumboot_printf ("wIffX = %d\n", cfg_reg->wIffX);
  rumboot_printf ("wWi   = %d\n", cfg_reg->wWi  );
  rumboot_printf ("wHi   = %d\n", cfg_reg->wHi  );
  rumboot_printf ("wCi   = %d\n", cfg_reg->wCi  );
  rumboot_printf ("wWo   = %d\n", cfg_reg->wWo  );
  rumboot_printf ("wHo   = %d\n", cfg_reg->wHo  );
  rumboot_printf ("wCo   = %d\n", cfg_reg->wCo  );
  rumboot_printf ("wOpM  = 0x%x\n", cfg_reg->wOpM );
  rumboot_printf ("wK    = %d\n", cfg_reg->wK   );

  rumboot_printf ("wKWHr = 0x%x\n", cfg_reg->wKWHr );

  rumboot_printf ("wP    = 0x%x\n", cfg_reg->wP   );
  rumboot_printf ("wPV1  = 0x%x\n", cfg_reg->wPV1 );
  rumboot_printf ("wPV2  = 0x%x\n", cfg_reg->wPV2 );
  rumboot_printf ("wPV3  = 0x%x\n", cfg_reg->wPV3 );
  rumboot_printf ("wPV4  = 0x%x\n", cfg_reg->wPV4 );
  rumboot_printf ("wPV5  = 0x%x\n", cfg_reg->wPV5 );
  rumboot_printf ("wPV6  = 0x%x\n", cfg_reg->wPV6 );
  rumboot_printf ("wPV7  = 0x%x\n", cfg_reg->wPV7 );
#endif // NU_NO_PRINT
}

uint32_t nu_lut_log2_i(uint32_t a) {  // log2(0)=0; log2(1)=0; log2(2)=1; log2(8)=3; ...
  uint32_t t;
  uint32_t r;
  
  t=a;r=0;
  while((t!=0) && (t!=1)) { t>>=1; r++; }
  return r;
}

int32_t nu_lut_log2_f(float a) {
  uint32_t temp_ui;
  uint32_t mant;
  int32_t temp_i;
  void* temp_ptr;
  
  temp_ptr = &a;
  temp_ui = *((uint32_t*) temp_ptr); // Take A Bit Couple
  
  temp_ui = temp_ui & 0x7FFFFFFF; // Take Sign Out
  mant = temp_ui & ((1<<23)-1);  // Mantissa
  temp_ui = temp_ui >> 23;  // Exponent
  temp_i = temp_ui;
  temp_i = temp_i - 127;
  if(mant & (1<<22))     // Round To Nearest
    temp_i = temp_i + 1;
  return temp_i;
}

void nu_vpe_load_lut(uintptr_t base, void* lut1, void* lut2) {
  uint16_t* ptr;
  uint32_t command;
  uint32_t lut1_size = NU_VPE_LUT1_SIZE; // Predefined By HW
  uint32_t lut2_size = NU_VPE_LUT2_SIZE;
  
  command = (1<<17)|(0<<16);// NU_VPE_LUT_ACCESS_CFG - Write LUT1 From Begining
  iowrite32(command, base + NU_VPE + NU_VPE_LUT_ACCESS_CFG);           // Write Command + Cell Address
  ptr = (uint16_t*) lut1;
  
  for(uint32_t i=0;i<lut1_size;i++) {
    iowrite32((uint32_t) *ptr++, base + NU_VPE + NU_VPE_LUT_ACCESS_DATA);  // Write Data
  }
  
  command = (1<<17)|(1<<16); // Write LUT2
  iowrite32(command,base + NU_VPE + NU_VPE_LUT_ACCESS_CFG);
  
  ptr = (uint16_t*) lut2;
  for(uint32_t i=0;i<lut2_size;i++) {
    iowrite32((uint32_t) *ptr++, base + NU_VPE + NU_VPE_LUT_ACCESS_DATA);
  }
  
}


void nu_vpe_setup_op01(uintptr_t base_, Enable op_en, ConfigOp01* op_config) {
  uint32_t tmp_data;
  
  tmp_data = (!op_config->relu_en      << 6) | // RELU_BYPASS
             ( op_config->prelu_en     << 5) | // MUL_PRELU
             (!op_config->mux_en       << 4) | // MUL_BYPASS
             (op_config->alu_operation << 2) | // ALU_ALGO
             (!op_config->alu_en       << 1) | // ALU_BYPASS
             (!op_en                   << 0);  //  BYPASS
  iowrite32(tmp_data, base_ + NU_VPE_OP0_CFG);
  
  tmp_data = (op_config->lshift_value << 8) | ((op_config->alu_mode>>1) << 0) ;
  iowrite32(tmp_data, base_ + NU_VPE_OP0_ALU_CFG);
  
  iowrite32(op_config->alu_value, base_ + NU_VPE_OP0_ALU_SRC_VAL);
  
  iowrite32((op_config->mux_mode>>1) , base_ + NU_VPE_OP0_MUL_CFG);
  iowrite32(op_config->mux_value, base_ + NU_VPE_OP0_MUL_SRC_VAL);

  tmp_data = (op_config->norm_round_mode << 16) | (op_config->norm_saturation_en << 8) | (op_config->norm_round_size << 0);
  iowrite32(tmp_data, base_ + NU_VPE_OP0_NORM_PARAM);  
  
}

void nu_vpe_setup_lut(uintptr_t base_, ConfigOp2* op_config, DataTypeExt in_data_type) {
  int32_t shift_i1;
  int32_t shift_i2;
  uint32_t tmp_data;
  
  tmp_data = (op_config->lut_mrr_type       << 4) | 
             (op_config->lut_mrr_en         << 3) |
             (op_config->lut_sel            << 2) | // HYBR_SEL
             (op_config->lut_right_priority << 1) | // OVRF_SEL
             (op_config->lut_left_priority  << 0) ; // UNDF_SEL
  iowrite32(tmp_data, base_ + NU_VPE_LUT_CFG);
  
  iowrite32(op_config->lut_tab1_x_start, base_ + NU_VPE_LUT_TAB0_XMIN);
  iowrite32(op_config->lut_tab1_x_end  , base_ + NU_VPE_LUT_TAB0_XMAX);
  iowrite32(op_config->lut_tab2_x_start, base_ + NU_VPE_LUT_TAB1_XMIN);
  iowrite32(op_config->lut_tab2_x_end  , base_ + NU_VPE_LUT_TAB1_XMAX);
  
  tmp_data = 0x000000000 | ((op_config->lut_tab1_slope_ovrf_scale & 0x0000FFFF) << 16) |
                           ((op_config->lut_tab1_slope_undf_scale & 0x0000FFFF)      );
  iowrite32(tmp_data, base_ + NU_VPE_LUT_TAB0_SLOPE_SCALE);
  
  tmp_data = 0x000000000 | ((op_config->lut_tab1_slope_ovrf_shift & 0x1F) << 8) |
                           ((op_config->lut_tab1_slope_undf_shift & 0x1F)     );
  iowrite32(tmp_data, base_ + NU_VPE_LUT_TAB0_SLOPE_SHIFT);
  
  tmp_data = 0x000000000 | ((op_config->lut_tab2_slope_ovrf_scale & 0x0000FFFF) << 16) |
                           ((op_config->lut_tab2_slope_undf_scale & 0x0000FFFF)      );
  iowrite32(tmp_data, base_ + NU_VPE_LUT_TAB1_SLOPE_SCALE);
  
  tmp_data = 0x000000000 | ((op_config->lut_tab2_slope_ovrf_shift & 0x1F) << 8) |
                           ((op_config->lut_tab2_slope_undf_shift & 0x1F)     );
  iowrite32(tmp_data, base_ + NU_VPE_LUT_TAB1_SLOPE_SHIFT);
  
  iowrite32(op_config->lut_xoffset, base_ + NU_VPE_LUT_XOFFSET);
  tmp_data = 0x000000000 | op_config->lut_yoffset;
  iowrite32(tmp_data, base_ + NU_VPE_LUT_YOFFSET);
  
  
  if(in_data_type == DataTypeExt_Fp16 || in_data_type == DataTypeExt_Fp32) { // Floating Point LUT
    float start_f;
    float end_f;
    float diff_f;
    void* temp_ptr;
    
    temp_ptr = &(op_config->lut_tab1_x_end)  ;end_f   = *((float*) temp_ptr);// Read The Couple Of Bits 
    temp_ptr = &(op_config->lut_tab1_x_start);start_f = *((float*) temp_ptr);//  And Store It In A float Variable
    diff_f = end_f  - start_f;
    shift_i1 = nu_lut_log2_f(diff_f) - 8;
    
    temp_ptr = &(op_config->lut_tab2_x_end)  ;end_f   = *((float*) temp_ptr);// Read The Couple Of Bits 
    temp_ptr = &(op_config->lut_tab2_x_start);start_f = *((float*) temp_ptr);//  And Store It In A float Variable
    diff_f = end_f  - start_f;
    shift_i2 = nu_lut_log2_f(diff_f) - 6;
  }
  else { // Integer LUT
    shift_i1 = nu_lut_log2_i(op_config->lut_tab1_x_end - op_config->lut_tab1_x_start) - 8;
    shift_i2 = nu_lut_log2_i(op_config->lut_tab2_x_end - op_config->lut_tab2_x_start) - 6;
  }
  
  rumboot_printf("LUTs: shift_i1 = %x, shift_i2 = %x\n",shift_i1,shift_i2);
  iowrite32(shift_i1,base_ + NU_VPE_LUT_TAB0_XSHIFT);
  iowrite32(shift_i2,base_ + NU_VPE_LUT_TAB1_XSHIFT);
}

void nu_vpe_setup_op2(uintptr_t base_, Enable op_en, ConfigOp2* op_config, DataTypeExt in_data_type) {
  uint32_t tmp_data;
  
  tmp_data = ( (op_config->alu_operation & (1<<2)) << (7-2) ) | // ALU_ALGO Split Field MSB
             (!op_config->lut_en                   <<  6)     | // LUT_BYPASS
             ( op_config->prelu_en                 <<  5)     | // MUL_PRELU
             (!op_config->mux_en                   <<  4)     | // MUL_BYPASS
             ( (op_config->alu_operation & 3)      <<  2 )    | // ALU_ALGO Split Field LSB
             (!op_config->alu_en                   <<  1)     | // ALU_BYPASS
             (!op_en                               <<  0)     ;  //  BYPASS
  iowrite32(tmp_data, base_ + NU_VPE_OP2_CFG);   
  
  tmp_data = (!op_config->c2_en << 1) | ((op_config->alu_mode>>1) << 0) ;
  iowrite32(tmp_data, base_ + NU_VPE_OP2_ALU_CFG);  

  iowrite32(op_config->alu_value, base_ + NU_VPE_OP2_ALU_SRC_VAL);
  
  iowrite32(op_config->c2_offset, base_ + NU_VPE_OP2_ALU_CVT_OFFSET_VAL);
  iowrite32(op_config->c2_scale , base_ + NU_VPE_OP2_ALU_CVT_SCALE_VAL );
  iowrite32(op_config->c2_trunc , base_ + NU_VPE_OP2_ALU_CVT_TRUNC_VAL );


  tmp_data = (!op_config->c1_en << 1) | ((op_config->mux_mode>>1) << 0) ;
  iowrite32(tmp_data, base_ + NU_VPE_OP2_MUL_CFG);  

  iowrite32(op_config->mux_value, base_ + NU_VPE_OP2_MUL_SRC_VAL);
  
  iowrite32(op_config->c1_offset, base_ + NU_VPE_OP2_MUL_CVT_OFFSET_VAL);
  iowrite32(op_config->c1_scale , base_ + NU_VPE_OP2_MUL_CVT_SCALE_VAL );
  iowrite32(op_config->c1_trunc , base_ + NU_VPE_OP2_MUL_CVT_TRUNC_VAL );  
  
  tmp_data = (op_config->norm_round_mode << 16) | (op_config->norm_saturation_en << 8) | (op_config->norm_round_size << 0);
  iowrite32(tmp_data, base_ + NU_VPE_OP2_NORM_PARAM);

  if(op_config->lut_en) {
    nu_vpe_setup_lut(base_, op_config, in_data_type);
  }
}

void nu_vpe_setup_dma(uintptr_t base_, ConfigDMA* dma_config, TraceMode trace_mode) {
  uint32_t tmp_data;
  
    // CRUTCH - Because Of Wrong Batch Mode Algorythm This Fields Calculated Incorrectly By nu_vpe_decide_dma_cube_config
    //          We Should Fix Them Here
    //          Waiting For The Batch Mode Algorythm Edition To Remove This CRUTCH CHECK
  if(trace_mode==TraceMode_MPE || trace_mode==TraceMode_MPE_DW) {
    dma_config->dma_box_st_size_x = (128/(dma_config->dma_bsize+1)) - 1;
    dma_config->dma_box_size_x    = (128/(dma_config->dma_bsize+1)) - 1;
  }
    // 
  
  tmp_data =( dma_config->dma_ram_type        << 8) | 
            ((dma_config->dma_data_mode >> 1) << 7) | 
            ( dma_config->dma_data_size       << 6) | 
            ( dma_config->dma_data_use        << 4) | 
            ( dma_config->dma_en              << 0) ;
  iowrite32(tmp_data, base_ + NU_VPE_DMA_CFG);

  tmp_data = (2                       << 16) | // AXI_PROT
             (dma_config->dma_axi_len <<  0) ; // AXI_LEN
  iowrite32(tmp_data, base_ + NU_VPE_DMA_AXI_PARAM );

  iowrite32(dma_config->dma_baddr           , base_ + NU_VPE_DMA_BASE                ) ;
   iowrite32(((dma_config->dma_cube_size_c-1)/16 + 1)*dma_config->dma_cube_size_w*dma_config->dma_cube_size_h - 1     , base_ + NU_VPE_DMA_AXI_PARAM-4         ) ;  // ?????
  iowrite32(dma_config->dma_cube_size_c-1   , base_ + NU_VPE_DMA_CUBE_SIZE_C         ) ;  // ?????
  iowrite32(dma_config->dma_cube_size_w-1   , base_ + NU_VPE_DMA_CUBE_SIZE_W         ) ;  // ?????
  iowrite32(dma_config->dma_cube_size_h-1   , base_ + NU_VPE_DMA_CUBE_SIZE_H         ) ;  // ?????
  iowrite32(dma_config->dma_border_x        , base_ + NU_VPE_DMA_BORDER_X            ) ;
  iowrite32(dma_config->dma_border_y        , base_ + NU_VPE_DMA_BORDER_Y            ) ;
  iowrite32(dma_config->dma_border_z        , base_ + NU_VPE_DMA_BORDER_Z            ) ;
  iowrite32(dma_config->dma_stride_x        , base_ + NU_VPE_DMA_STRIDE_X            ) ;
  iowrite32(dma_config->dma_stride_y        , base_ + NU_VPE_DMA_STRIDE_Y            ) ;
  iowrite32(dma_config->dma_stride_z        , base_ + NU_VPE_DMA_STRIDE_Z            ) ;
  iowrite32(dma_config->dma_frag_last_size  , base_ + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR ) ;
  iowrite32(dma_config->dma_frag_size       , base_ + NU_VPE_DMA_FRAG_SIZE_ADDR      ) ;
  iowrite32(dma_config->dma_xyz_drct        , base_ + NU_VPE_DMA_XYZ_DRCT_ADDR       ) ;
  iowrite32(dma_config->dma_box_st_size_x   , base_ + NU_VPE_DMA_BOX_ST_SIZE_X       ) ;
  iowrite32(dma_config->dma_box_st_size_y   , base_ + NU_VPE_DMA_BOX_ST_SIZE_Y       ) ;
  iowrite32(dma_config->dma_box_st_size_z   , base_ + NU_VPE_DMA_BOX_ST_SIZE_Z       ) ;
  iowrite32(dma_config->dma_box_size_x      , base_ + NU_VPE_DMA_BOX_SIZE_X          ) ;
  iowrite32(dma_config->dma_box_size_y      , base_ + NU_VPE_DMA_BOX_SIZE_Y          ) ;
  iowrite32(dma_config->dma_box_size_z      , base_ + NU_VPE_DMA_BOX_SIZE_Z          ) ;
  iowrite32(dma_config->dma_box_offset_x    , base_ + NU_VPE_DMA_BOX_OFFSET_SIZE_X   ) ;
  iowrite32(dma_config->dma_box_offset_y    , base_ + NU_VPE_DMA_BOX_OFFSET_SIZE_Y   ) ;
  iowrite32(dma_config->dma_box_offset_z    , base_ + NU_VPE_DMA_BOX_OFFSET_SIZE_Z   ) ;
  iowrite32(dma_config->dma_bsize           , base_ + NU_VPE_DMA_BATCH_SIZE          ) ;
  iowrite32(dma_config->dma_bstride         , base_ + NU_VPE_DMA_BATCH_STRIDE        ) ;
}

void nu_vpe_setup(uintptr_t base, ConfigVPE* cfg) {
  rumboot_printf("Configuring VPE..\n");

  // iowrite32(cfg->MYFIELD, base + NU_VPE_MYREG);

  int32_t     tmp_data;
  DataType    tmp_type;

  // Configuration of VPE -------------------------------------------------------------------------
  // CUBE_SIZE 
  iowrite32(cfg->cube_size, base + NU_VPE + NU_VPE_CUBE_SIZE );

  iowrite32(cfg->wdma_config.dma_cube_size_c-1, base + NU_VPE + NU_VPE_CUBE_SIZE_C );                             // is it right to use wdma params here?
  iowrite32(cfg->wdma_config.dma_cube_size_w-1, base + NU_VPE + NU_VPE_CUBE_SIZE_W );  // * (cfg->wdma_config.dma_bsize+1) - 1, base + NU_VPE + NU_VPE_CUBE_SIZE_W );  // is it right to use wdma params here?
  iowrite32(cfg->wdma_config.dma_cube_size_h-1, base + NU_VPE + NU_VPE_CUBE_SIZE_H );                             // is it right to use wdma params here?
  
  if      (cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Int16) tmp_type = DataType_Int16 ;
  else if (cfg->in_data_type == DataTypeExt_Fp32  || cfg->in_data_type == DataTypeExt_Fp16)  tmp_type = DataType_Fp16  ;
  else                                                                                       tmp_type = DataType_Int8  ;
  tmp_data = ( (cfg->ninf_cnt_en&1) << 30) | 
             ( (cfg->ovrf_cnt_en&1) << 29) | 
             ( (cfg->lut_cnt_en&1)  << 28) |
             ((cfg->wdma_config.dma_bsize) << 20) |   // why wdma&&&& maybe rdma????  -1
             (cfg->out_data_type << 16) | 
             ((cfg->op2_config.coef_type>>1) << 15) | ((cfg->op2_rdma_config.dma_data_mode&0x1)<<14) |
             ((cfg->op1_config.coef_type>>1) << 13) | ((cfg->op1_rdma_config.dma_data_mode&0x1)<<12) |
             ((cfg->op0_config.coef_type>>1) << 11) | ((cfg->op0_rdma_config.dma_data_mode&0x1)<<10) |
             (                 (tmp_type>>1) <<  9) | ( ((cfg->trace_mode==TraceMode_MPE  || cfg->trace_mode==TraceMode_MPE_DW)?1:0)<<8) |
             (cfg->nan_to_zero_input << 4) | (cfg->dst_flying << 1) | (cfg->src_flying << 0) ;
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP_MODE);

  // Configuration OUT --------------------------------------------------
  iowrite32(cfg->c3_offset, base + NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL);
  iowrite32(cfg->c3_scale , base + NU_VPE + NU_VPE_OUT_CVT_SCALE_VAL );
  
  tmp_data = (cfg->c3_round_mode << 12) | (cfg->c3_satur_en << 8) | (cfg->c3_trunc << 0) ;
  iowrite32(tmp_data , base + NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL );

  // OP0 ----------------------------------------------------------------
  nu_vpe_setup_op01(base + NU_VPE                                  , cfg->op0_en, &(cfg->op0_config) );
  // OP1 ----------------------------------------------------------------
  nu_vpe_setup_op01(base + NU_VPE + (NU_VPE_OP1_CFG-NU_VPE_OP0_CFG), cfg->op1_en, &(cfg->op1_config) );
  // OP2 ----------------------------------------------------------------
  nu_vpe_setup_op2 (base + NU_VPE                                  , cfg->op2_en, &(cfg->op2_config), cfg->in_data_type );
  // Configuration of WDMA -------------------------------------------------------------------------
  nu_vpe_setup_dma (base + NU_VPE_DST_WDMA                                      , &(cfg->wdma_config)    , cfg->trace_mode );
  // Configuration SRC_RDMA ------------------------------------------------------
  nu_vpe_setup_dma (base + NU_VPE_SRC_RDMA                                      , &(cfg->src_rdma_config), cfg->trace_mode );
  // Configuration OP0_RDMA ------------------------------------------------------
  nu_vpe_setup_dma (base + NU_VPE_OP0_RDMA                                      , &(cfg->op0_rdma_config), cfg->trace_mode );
  // Configuration OP1_RDMA ------------------------------------------------------
  nu_vpe_setup_dma (base + NU_VPE_OP1_RDMA                                      , &(cfg->op1_rdma_config), cfg->trace_mode );
  // Configuration OP2_RDMA ------------------------------------------------------
  nu_vpe_setup_dma (base + NU_VPE_OP2_RDMA                                      , &(cfg->op2_rdma_config), cfg->trace_mode );
}


void nu_vpe_decide_dma_cube_config(ConfigDMA* dma_cfg, TraceMode trace_mode, CubeMetrics* metrics) {
  uint32_t elem_size     ;
  uint32_t tmp_data_use  ;
  uint32_t tmp_data_size ;
  
  dma_cfg->dma_cube_size_w = metrics->W ;
  dma_cfg->dma_cube_size_h = metrics->H ;
  dma_cfg->dma_cube_size_c = metrics->C ;
  
  if (dma_cfg->dma_en == Enable_NotEn) {
    dma_cfg->dma_xyz_drct       = DmaXYZDirection_X;
    dma_cfg->dma_frag_size      = 0 ;
    dma_cfg->dma_frag_last_size = 0 ;
    dma_cfg->dma_stride_z       = 0 ;
    dma_cfg->dma_stride_x       = 0 ;
    dma_cfg->dma_stride_y       = 0 ;
    dma_cfg->dma_border_z       = 0 ;
    dma_cfg->dma_border_x       = 0 ;
    dma_cfg->dma_border_y       = 0 ;
    
    dma_cfg->dma_box_st_size_x  = 0 ;
    dma_cfg->dma_box_st_size_y  = 0 ;
    dma_cfg->dma_box_st_size_z  = 0 ;
    dma_cfg->dma_box_size_x     = 0 ;
    dma_cfg->dma_box_size_y     = 0 ;
    dma_cfg->dma_box_size_z     = 128/16 - 1 ;  // CHECK CRUTCH - This Register Works Even When Channel Is Off
    dma_cfg->dma_box_offset_x   = 0 ;
    dma_cfg->dma_box_offset_y   = 0 ;
    dma_cfg->dma_box_offset_z   = 0 ;
  }
  else {
    // elem_size definition-------------------
    if   (dma_cfg->dma_data_use  == DmaDUse_Both) tmp_data_use = 2 ;
    else                                          tmp_data_use = 1 ;
    
    if   (dma_cfg->dma_data_size == DmaDSize_Two_Byte) tmp_data_size = 2 ;
    else                                               tmp_data_size = 1 ;
    
    elem_size = tmp_data_use * tmp_data_size; 
    
    //------------
    dma_cfg->dma_frag_size = 16 * elem_size ;
    dma_cfg->dma_xyz_drct = DmaXYZDirection_Z;
    
    if (dma_cfg->dma_data_mode == Mode_Channel) {
      dma_cfg->dma_xyz_drct        = DmaXYZDirection_X      ;
      dma_cfg->dma_frag_last_size  = metrics->C * elem_size ; 
      dma_cfg->dma_stride_z        = 0                      ; //coef_z == vector_size * elem_size
      dma_cfg->dma_stride_x        = 0                      ; //coef_x == full_line_z             = full_line_C*elem_size
      dma_cfg->dma_stride_y        = 0                      ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
      dma_cfg->dma_border_z        = 0                      ; //line_size (bytes)               = (Z-1)*elem_size
      dma_cfg->dma_border_x        = 0                      ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
      dma_cfg->dma_border_y        = 0                      ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
      dma_cfg->dma_box_st_size_x   = 0 ;
      dma_cfg->dma_box_st_size_y   = 0 ;
      dma_cfg->dma_box_st_size_z   = 0 ;
      dma_cfg->dma_box_size_x      = 0 ;
      dma_cfg->dma_box_size_y      = 0 ;
      dma_cfg->dma_box_size_z      = 0 ;
      dma_cfg->dma_box_offset_x    = 0 ;
      dma_cfg->dma_box_offset_y    = 0 ;
      dma_cfg->dma_box_offset_z    = 0 ;
    }
    else if (trace_mode == TraceMode_Linear) {
      // for flying testcases only. Linear data read mode. NOT MPE read mode !!!!!!!!!!!! correct only for Cube sizes < 128
      dma_cfg->dma_frag_last_size = metrics->C              * elem_size      ;
      dma_cfg->dma_stride_z       = metrics->C              * elem_size      ; //coef_z == vector_size * elem_size
      dma_cfg->dma_stride_x       = metrics->C              * elem_size      ; //coef_x == full_line_z             = full_line_C*elem_size
      dma_cfg->dma_stride_y       = metrics->C * metrics->W * elem_size      ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
      dma_cfg->dma_border_z       = 0                                        ; //line_size (bytes)               = (Z-1)*elem_size
      dma_cfg->dma_border_x       = (metrics->W - 1) * dma_cfg->dma_stride_x ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
      dma_cfg->dma_border_y       = (metrics->H - 1) * dma_cfg->dma_stride_y ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
      dma_cfg->dma_box_st_size_x  = metrics->W - 1 ;
      dma_cfg->dma_box_st_size_y  = metrics->H - 1 ;
      dma_cfg->dma_box_st_size_z  = 1 - 1 ;
      dma_cfg->dma_box_size_x     = metrics->W - 1 ;
      dma_cfg->dma_box_size_y     = metrics->H - 1 ;
      dma_cfg->dma_box_size_z     = 1 - 1 ;
      dma_cfg->dma_box_offset_x   = 0 ;
      dma_cfg->dma_box_offset_y   = 0 ;
      dma_cfg->dma_box_offset_z   = 0 ;
    }
    else if (trace_mode == TraceMode_MPE) { // MPE read mode
      dma_cfg->dma_frag_last_size = ((metrics->C%16) + ((metrics->C%16) == 0)*16) * elem_size ;
      dma_cfg->dma_stride_z       = 16                      * elem_size                       ; //coef_z == vector_size * elem_size
      dma_cfg->dma_stride_x       = metrics->C              * elem_size                       ; //coef_x == full_line_z             = full_line_C*elem_size
      dma_cfg->dma_stride_y       = metrics->C * metrics->W * elem_size                       ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
      dma_cfg->dma_border_z       = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size ; //line_size (bytes)               = (Z-1)*elem_size
      dma_cfg->dma_border_x       = (metrics->W - 1) * dma_cfg->dma_stride_x                  ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
      dma_cfg->dma_border_y       = (metrics->H - 1) * dma_cfg->dma_stride_y                  ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
      dma_cfg->dma_box_st_size_x  = (128/(dma_cfg->dma_bsize+1)) - 1 ;
      dma_cfg->dma_box_st_size_y  = 1      - 1 ;
      dma_cfg->dma_box_st_size_z  = 128/16 - 1 ;
      dma_cfg->dma_box_size_x     = (128/(dma_cfg->dma_bsize+1)) - 1 ;
      dma_cfg->dma_box_size_y     = 1      - 1 ;
      dma_cfg->dma_box_size_z     = 128/16 - 1 ;
      dma_cfg->dma_box_offset_x   = 0 ;
      dma_cfg->dma_box_offset_y   = 0 ;
      dma_cfg->dma_box_offset_z   = 0 ;
    }
    else if (trace_mode == TraceMode_MPE_DW) { // MPE DeepWise read mode
      dma_cfg->dma_frag_last_size = ((metrics->C%16) + ((metrics->C%16) == 0)*16) * elem_size ;
      dma_cfg->dma_stride_z       = 16                      * elem_size                       ; //coef_z == vector_size * elem_size
      dma_cfg->dma_stride_x       = metrics->C              * elem_size                       ; //coef_x == full_line_z             = full_line_C*elem_size
      dma_cfg->dma_stride_y       = metrics->C * metrics->W * elem_size                       ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
      dma_cfg->dma_border_z       = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size ; //line_size (bytes)               = (Z-1)*elem_size
      dma_cfg->dma_border_x       = (metrics->W - 1) * dma_cfg->dma_stride_x                  ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
      dma_cfg->dma_border_y       = (metrics->H - 1) * dma_cfg->dma_stride_y                  ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
      dma_cfg->dma_box_st_size_x  = (128/(dma_cfg->dma_bsize+1)) - 1 ;
      dma_cfg->dma_box_st_size_y  = 1      - 1 ;
      dma_cfg->dma_box_st_size_z  = 16/16  - 1 ;
      dma_cfg->dma_box_size_x     = (128/(dma_cfg->dma_bsize+1)) - 1 ;
      dma_cfg->dma_box_size_y     = 1      - 1 ;
      dma_cfg->dma_box_size_z     = 16/16  - 1 ;
      dma_cfg->dma_box_offset_x   = 0 ;
      dma_cfg->dma_box_offset_y   = 0 ;
      dma_cfg->dma_box_offset_z   = 0 ;
    }
    else { // PPE read mode
      dma_cfg->dma_frag_last_size = ((metrics->C%16) + ((metrics->C%16) == 0)*16) * elem_size ;
      dma_cfg->dma_stride_z       = 16                      * elem_size                       ; //coef_z == vector_size * elem_size
      dma_cfg->dma_stride_x       = metrics->C              * elem_size                       ; //coef_x == full_line_z             = full_line_C*elem_size
      dma_cfg->dma_stride_y       = metrics->C * metrics->W * elem_size                       ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
      dma_cfg->dma_border_z       = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size ; //line_size (bytes)               = (Z-1)*elem_size
      dma_cfg->dma_border_x       = (metrics->W - 1) * dma_cfg->dma_stride_x                  ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
      dma_cfg->dma_border_y       = (metrics->H - 1) * dma_cfg->dma_stride_y                  ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
      dma_cfg->dma_box_st_size_x  = metrics->W - 1 ;
      dma_cfg->dma_box_st_size_y  = metrics->H - 1 ;
      dma_cfg->dma_box_st_size_z  = 1 - 1 ;
      dma_cfg->dma_box_size_x     = metrics->W - 1 ;
      dma_cfg->dma_box_size_y     = metrics->H - 1 ;
      dma_cfg->dma_box_size_z     = 1 - 1 ;
      dma_cfg->dma_box_offset_x   = 0 ;
      dma_cfg->dma_box_offset_y   = 0 ;
      dma_cfg->dma_box_offset_z   = 0 ;
    }
  }
}

void nu_vpe_decide_op01_rdma_config(ConfigOp01* op_config, ConfigDMA* op_rdma_config, TraceMode trace_mode, CubeMetrics* metrics) {

#define  NU_VPE_DECIDE_OP_RDMA_CONFIG_BODY \
  if((op_config->alu_en == Enable_En && op_config->alu_mode != Mode_Unitary) ||         \
     (op_config->mux_en == Enable_En && op_config->mux_mode != Mode_Unitary)  ) {       \
    op_rdma_config->dma_en = Enable_En;                                                 \
  }                                                                                     \
  else {                                                                                \
    op_rdma_config->dma_en = Enable_NotEn;                                              \
  }                                                                                     \
                                                                                        \
  op_rdma_config->dma_ram_type = 0;                                                     \
                                                                                        \
                                                                                        \
  if      (op_config->alu_en == Enable_En && op_config->alu_mode != Mode_Unitary) op_rdma_config->dma_data_mode = op_config->alu_mode;       \
  else if (op_config->mux_en == Enable_En && op_config->mux_mode != Mode_Unitary) op_rdma_config->dma_data_mode = op_config->mux_mode;       \
  else                                                                            op_rdma_config->dma_data_mode = Mode_Unitary;              \
                                                                                                                                             \
  if ((op_config->alu_en == Enable_En && op_config->alu_mode != Mode_Unitary) &&                                                             \
      (op_config->mux_en == Enable_En && op_config->mux_mode != Mode_Unitary) &&                                                             \
      (op_config->mux_mode != op_config->alu_mode)                                ) {                                                        \
    rumboot_printf("ERROR! parameters alu_mode & mux_mode must be equal..\n");                                                               \
  }                                                                                                                                          \
                                                                                                                                             \
                                                                                                                                             \
  if (op_config->coef_type == DataType_Int8) op_rdma_config->dma_data_size = DmaDSize_One_Byte ;                                             \
  else                                       op_rdma_config->dma_data_size = DmaDSize_Two_Byte ;                                             \
                                                                                                                                             \
                                                                                                                                             \
  if      ((op_config->alu_en == Enable_En && op_config->alu_mode != Mode_Unitary) &&                                                        \
           (op_config->mux_en == Enable_En && op_config->mux_mode != Mode_Unitary) ){                                                        \
    op_rdma_config->dma_data_use = DmaDUse_Both ;                                                                                            \
  }                                                                                                                                          \
  else if (( op_config->alu_en == Enable_En && op_config->alu_mode != Mode_Unitary) &&                                                       \
           ( op_config->mux_en != Enable_En || op_config->mux_mode == Mode_Unitary) ){                                                       \
    op_rdma_config->dma_data_use = DmaDUse_Alu ;                                                                                             \
  }                                                                                                                                          \
  else if (( op_config->mux_en == Enable_En && op_config->mux_mode != Mode_Unitary) &&                                                       \
           ( op_config->alu_en != Enable_En || op_config->alu_mode == Mode_Unitary) ){                                                       \
    op_rdma_config->dma_data_use = DmaDUse_Mux ;                                                                                             \
  }                                                                                                                                          \
  else op_rdma_config->dma_data_use = DmaDUse_Off ;                                                                                          \
                                                                                                                                             \
                                                                                                                                             \
  nu_vpe_decide_dma_cube_config(op_rdma_config, trace_mode, metrics);                                                                        \
  
  
  NU_VPE_DECIDE_OP_RDMA_CONFIG_BODY
}
void nu_vpe_decide_op2_rdma_config(ConfigOp2* op_config, ConfigDMA* op_rdma_config, TraceMode trace_mode, CubeMetrics* metrics) {
  NU_VPE_DECIDE_OP_RDMA_CONFIG_BODY
}



void nu_vpe_decide_dma_config_trivial(ConfigVPE* cfg, CubeMetrics* metrics) {

  cfg->mark = Enable_NotEn;  // Why it's here?

  // CUBE_SIZE in vectors
  cfg->cube_size = (metrics->C/16 + ((metrics->C%16) != 0 ? 1 : 0)) * metrics->W * metrics->H - 1;  // Why it's here?
    
  // WDMA --------------------------------------------------------------------------------------------
  
  if   (cfg->dst_flying == Enable_En) cfg->wdma_config.dma_en = Enable_NotEn ;
  else                                cfg->wdma_config.dma_en = Enable_En    ;
  
  cfg->wdma_config.dma_ram_type = 0            ;  // Not Used Actually, But We Should Init
  cfg->wdma_config.dma_data_use = DmaDUse_Mux  ;  // 
  cfg->wdma_config.dma_data_mode= Mode_Element ;  // 

  // dma_data_size definition------------------- 
  if (cfg->out_data_type == DataType_Int8) cfg->wdma_config.dma_data_size = DmaDSize_One_Byte ;
  else                                     cfg->wdma_config.dma_data_size = DmaDSize_Two_Byte ;
  
  nu_vpe_decide_dma_cube_config(&(cfg->wdma_config), cfg->trace_mode, metrics);

  // SRC_DMA --------------------------------------------------------------------------------------------
    // if 32bit Input Data then Main Channel Works
  if(cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32) cfg->src_rdma_config.dma_en = Enable_NotEn ;
  else                                                                                cfg->src_rdma_config.dma_en = Enable_En    ;

  cfg->src_rdma_config.dma_data_use = DmaDUse_Mux  ;
  cfg->src_rdma_config.dma_ram_type  = 0 ;
  cfg->src_rdma_config.dma_data_mode = Mode_Element ;
  
  if (cfg->in_data_type == DataTypeExt_Int8) cfg->src_rdma_config.dma_data_size = DmaDSize_One_Byte ;
  else                                       cfg->src_rdma_config.dma_data_size = DmaDSize_Two_Byte ;
  
  nu_vpe_decide_dma_cube_config(&(cfg->src_rdma_config), cfg->trace_mode, metrics);
  
  // OP0_RDMA -------------------------------------------------------------------------------------------- 
  nu_vpe_decide_op01_rdma_config(&(cfg->op0_config), &(cfg->op0_rdma_config), cfg->trace_mode, metrics);
  // OP1_RDMA -------------------------------------------------------------------------------------------- 
  nu_vpe_decide_op01_rdma_config(&(cfg->op1_config), &(cfg->op1_rdma_config), cfg->trace_mode, metrics);
  // OP2_RDMA -------------------------------------------------------------------------------------------- 
  nu_vpe_decide_op2_rdma_config(&(cfg->op2_config), &(cfg->op2_rdma_config), cfg->trace_mode, metrics);
}

int out_dim_comp(int in_dim, int k_dim, int k_str) {
  int out_dim;

  if (k_str > k_dim) {

    if (k_str+1 > in_dim) out_dim = 1;
    else {
      out_dim = in_dim/k_str;

      if (in_dim%k_str + 1 > k_dim) out_dim = out_dim + 1;
    }
  }
  else out_dim = (in_dim-k_dim)/k_str + 1;

//  out_dim = (in_dim-k_dim)/k_str + 1;
//
//  if (k_str>k_dim) {
//    if ((in_dim%k_str != 0) && (in_dim%k_str < k_dim)) rumboot_printf("k_str>k_dim && out_dim is not integer");
//  }
//  else {
//    if ((in_dim-k_dim)%k_str != 0) rumboot_printf("k_str<=k_dim && out_dim is not integer");
//  }

  return out_dim;
}

void nu_calc_mpe_cube_out_metrics(ConfigMPE* cfg, CubeMetrics* out_m) {

  int H ;
  int W ;
  int Tp;
  int Bp;
  int Lp;
  int Rp;
  int R ;
  int S ;
  int Dr;
  int Ds;
  int Sw;
  int Sh;
  int K ;

  int Rd, Sd, Hp, Wp, Ho, Wo, dt_b, so;

  if (cfg == NULL || out_m == NULL) {
    rumboot_printf("ConfigMPE ptr or OutCubeMetrics ptr is NULL ptr");
  }
  else {
    H   = cfg->H  ;
    W   = cfg->W  ;
    Tp  = cfg->Tp ;
    Bp  = cfg->Bp ;
    Lp  = cfg->Lp ;
    Rp  = cfg->Rp ;
    R   = cfg->R  ;
    S   = cfg->S  ;
    Dr  = cfg->Dr ;
    Ds  = cfg->Ds ;
    Sh  = cfg->Sh ;
    Sw  = cfg->Sw ;
    K   = cfg->K  ;

    DataType dt = cfg->dt ;

    dt_b =  dt == DataType_Int8 ? 0x1 :
            dt == DataType_Int16 || dt == DataType_Fp16 ? 0x2 : 0x0
    ;

    Hp  = H+Tp+Bp;
    Wp  = W+Lp+Rp;

    Rd  = (R-1)*Dr + 1;
    Sd  = (S-1)*Ds + 1;

    Ho  = out_dim_comp(Hp, Rd, Sh);
    Wo  = out_dim_comp(Wp, Sd, Sw);

    so  = Ho*Wo*K*dt_b;

    out_m->s = so ;
    out_m->H = Ho ;
    out_m->W = Wo ;
    out_m->C = K  ;
  }
}

int nu_vpe_check_reg(uintptr_t addr, char* name, int shift, uint32_t mask, uint32_t etalon) {
  uint32_t temp;
  temp = ioread32(addr);
  // rumboot_printf("Checking %s 0x%x vs 0x%x, shift=%d, mask=0x%x\n",name,temp,etalon,shift,mask);
  temp = (temp>>shift) & mask;
  if( temp != etalon) {
    rumboot_printf("Mismatch %s: Read %d, Expected %d\n",name,temp, etalon);
    return 1;
  }
  return 0;
}

int nu_vpe_check_status_regs(uintptr_t base, StatusRegs* status_regs) {
  rumboot_printf("Checking Status Regs\n");
  
  if(
    nu_vpe_check_reg(base + NU_VPE_OP2_ALU_CMP_RESULT,"CMP_RESULT"      ,0,1         ,status_regs->cmp_result      ) +
    nu_vpe_check_reg(base + NU_VPE_INF_NUM_IN        ,"INF_NUM_IN"      ,0,0xFFFFFFFF,status_regs->inf_num_in      ) +
    nu_vpe_check_reg(base + NU_VPE_NAN_NUM_IN        ,"NAN_NUM_IN"      ,0,0xFFFFFFFF,status_regs->nan_num_in      ) +
    nu_vpe_check_reg(base + NU_VPE_NAN_NUM_OUT       ,"NAN_NUM_OUT"     ,0,0xFFFFFFFF,status_regs->nan_num_out     ) +
    nu_vpe_check_reg(base + NU_VPE_SATUR_NUM_OUT     ,"SATUR_NUM_OUT"   ,0,0xFFFFFFFF,status_regs->satur_num_out   ) +
    nu_vpe_check_reg(base + LUT_CNT_OVRF             ,"LUT_CNT_OVRF"    ,0,0xFFFFFFFF,status_regs->lut_cnt_ovrf    ) +
    nu_vpe_check_reg(base + LUT_CNT_UNDF             ,"LUT_CNT_UNDF"    ,0,0xFFFFFFFF,status_regs->lut_cnt_undf    ) +
    nu_vpe_check_reg(base + LUT_CNT_HIT_TAB0         ,"LUT_CNT_HIT_TAB0",0,0xFFFFFFFF,status_regs->lut_cnt_hit_tab0) +
    nu_vpe_check_reg(base + LUT_CNT_HIT_TAB1         ,"LUT_CNT_HIT_TAB1",0,0xFFFFFFFF,status_regs->lut_cnt_hit_tab1) +
    nu_vpe_check_reg(base + LUT_CNT_HIT_HYBR         ,"LUT_CNT_HIT_HYBR",0,0xFFFFFFFF,status_regs->lut_cnt_hit_hybr) 
    != 0
  )
    return 1;
  
  return 0;
}


int nu_mpe_get_size_in_partitions(int size_in_bytes) {
  int res;
  res = size_in_bytes / (NU_MPE_BUF01 - NU_MPE_BUF00);
  if( (size_in_bytes % (NU_MPE_BUF01 - NU_MPE_BUF00)) != 0 )
    res++;

  return res;
}





void nu_mpe_load_ma_config_from_table_row(ConfigMAMPE* cfg, uint32_t** ptr_) {
  uint32_t* ptr;
  
  ptr = *ptr_;
  
  cfg->ADD_CountI0=(uint16_t) *ptr;ptr++;
  cfg->CMP_CountI0=(uint16_t) *ptr;ptr++;
  cfg->ADD_CountI1=(uint16_t) *ptr;ptr++;
  cfg->CMP_CountI1=(uint16_t) *ptr;ptr++;
  cfg->ADD_CountJ=(uint16_t) *ptr;ptr++;
  cfg->CMP_CountJ=(uint16_t) *ptr;ptr++;
  cfg->ADD_CountPPS=(uint16_t) *ptr;ptr++;
  cfg->CMP_CountPPS=(uint16_t) *ptr;ptr++;
  cfg->ADD_CountLCZ=(uint8_t) *ptr;ptr++;
  cfg->CMP_CountLCZ=(uint8_t) *ptr;ptr++;
  cfg->ADD_CountPLC=(uint16_t) *ptr;ptr++;
  cfg->CMP_CountPLC=(uint16_t) *ptr;ptr++;
  cfg->ADD_CountX=(uint16_t) *ptr;ptr++;
  cfg->CMP_CountX=(uint16_t) *ptr;ptr++;
  cfg->ADD_CountY=(uint16_t) *ptr;ptr++;
  cfg->CMP_CountY=(uint16_t) *ptr;ptr++;
  cfg->ADD_CountM=(uint16_t) *ptr;ptr++;
  cfg->CMP_CountM=(uint16_t) *ptr;ptr++;
  
  cfg->BRDR=(uint16_t) *ptr;ptr++;
  cfg->WA=(uint16_t) *ptr;ptr++;
  cfg->DA=(uint16_t) *ptr;ptr++;
  cfg->VRA=(uint16_t) *ptr;ptr++;
  cfg->NR=(uint8_t) *ptr;ptr++;
  cfg->D_BIAS=(uint8_t) *ptr;ptr++;
  cfg->D_NULL=(uint32_t) *ptr;ptr++;
  
  *ptr_=ptr;
}


void nu_mpe_load_dma_config_from_table_row(ConfigDMAMPE* cfg, uint32_t** ptr_) {
  uint32_t* ptr;
  
  ptr = *ptr_;
  
  cfg->MAINCNT=(uint16_t)        *ptr;ptr++;
  
//                                  ptr+=5; //Skipped 
  cfg->rdma.AOffset=             *ptr;ptr++;
  cfg->rdma.LPXOffset=           *ptr;ptr++;
  cfg->rdma.RPXOffset=           *ptr;ptr++;
  cfg->rdma.TPYOffset=           *ptr;ptr++;
  cfg->rdma.BPYOffset=           *ptr;ptr++;
  
  cfg->rdma.CntSha=(uint16_t)    *ptr;ptr++;
  
//                                  ptr+=5;
  cfg->rdma.CntThresholdSha=(uint16_t)*ptr;ptr++;
  cfg->rdma.ThreCtrl=(uint8_t)   *ptr;ptr++;
  cfg->rdma.LPXEn=               *ptr;ptr++;
  cfg->rdma.RPXEn=               *ptr;ptr++;
  cfg->rdma.TPYEn=               *ptr;ptr++;
  cfg->rdma.BPYEn=               *ptr;ptr++;
//
  for(int i=0;i<7;i++){
    cfg->rdma.Bias[i].BiasEn=                    *ptr;ptr++;

    if (i < 6) {
      cfg->rdma.Bias[i].ThreCtrl=(uint8_t)         *ptr;ptr++;
      cfg->rdma.Bias[i].DecCtrl=(uint8_t)          *ptr;ptr++;
    }

    cfg->rdma.Bias[i].PXBSEn=                    *ptr;ptr++;
    cfg->rdma.Bias[i].PYBSEn=                    *ptr;ptr++;
    cfg->rdma.Bias[i].Bias=                      *ptr;ptr++;
//                                                      ptr++; // Skipped AOffset

    cfg->rdma.Bias[i].CntSha=(uint16_t)          *ptr;ptr++;

    if (i < 6) {
      cfg->rdma.Bias[i].CntOffsetEn=               *ptr;ptr++;
      cfg->rdma.Bias[i].CntOffset=(uint8_t)        *ptr;ptr++;
    }

    cfg->rdma.Bias[i].CntThresholdSha=(uint16_t) *ptr;ptr++;
    cfg->rdma.Bias[i].CntCmp=(uint16_t)          *ptr;ptr++;
  }
  
  
  cfg->wdma.BADR=(uint16_t)            *ptr;ptr++;
  cfg->wdma.LADR=(uint16_t)            *ptr;ptr++;
  cfg->wdma.USED=                      *ptr;ptr++;
  cfg->wdma.ADR_PROL=                  *ptr;ptr++;
  cfg->wdma.C_BND=                     *ptr;ptr++;
  cfg->wdma.GLUE_EN=                   *ptr;ptr++;
  cfg->wdma.BStrideX=(uint8_t)         *ptr;ptr++;
  cfg->wdma.BConvX=(uint8_t)           *ptr;ptr++;
  cfg->wdma.Batch=(uint8_t)            *ptr;ptr++;
  cfg->wdma.Thre_PLC=(uint8_t)         *ptr;ptr++;
  cfg->wdma.Thre_VLC=(uint8_t)         *ptr;ptr++;
  cfg->wdma.Thre_CXC=(uint8_t)         *ptr;ptr++;
  cfg->wdma.Dec_PLC=(uint8_t)          *ptr;ptr++;
  cfg->wdma.PLC_CntSha=(uint8_t)       *ptr;ptr++;
  cfg->wdma.VLC_CntSha=(uint8_t)       *ptr;ptr++;
  cfg->wdma.PLC_ThreSha=(uint16_t)     *ptr;ptr++;
  cfg->wdma.VLC_ThreSha=(uint16_t)     *ptr;ptr++;
  
  for(int i=0;i<6;i++) {
    cfg->wdma.Bias[i].CntSha=(uint16_t)        *ptr;ptr++;
    cfg->wdma.Bias[i].CntCmp=(uint16_t)        *ptr;ptr++;
  }
  
  *ptr_=ptr;
}

int nu_mpe_look_up_dma_config(ConfigMPE* cfg, void* table) {
  uint32_t* tab_ptr;
  uint32_t* cfg_ptr;
  uint32_t* row_ptr;
  int res;
  int in_param_match;
  int cnt;

    // Search The Corresponding Row Of The Table
  row_ptr = (uint32_t*) table;
  res =  -1;
  in_param_match=0;
  while(*row_ptr !=0 && !in_param_match) { // Til We Find A Match Or Reach An Empty Row
    cfg_ptr = (uint32_t*) cfg;  // First Element Of ConfigMPE
    tab_ptr = row_ptr;          // First Element Of A Row
    
      // First 15 Elements Of The Row - Is A Row Header
      //  We Search Where It Matches The First 15 Elements Of ConfigMPE struct
    cnt=0;
    do {
      if(cnt==14) // dt - Special Parameter Which Is Compared Zero Or Not Zero
        in_param_match = *tab_ptr == *cfg_ptr || (*tab_ptr !=0 && *cfg_ptr !=0);
      else
        in_param_match = *tab_ptr == *cfg_ptr;

      cnt++;tab_ptr++;cfg_ptr++;
      
    } while(in_param_match && cnt<16); // 15 -> 16 batch_size
    
    row_ptr = row_ptr + NU_MPE_DMA_PARAM_TABLE_ROW_SIZE;
  }
  
  if(in_param_match) {
    nu_mpe_load_ma_config_from_table_row(& (cfg->ma_config),&tab_ptr);
    nu_mpe_load_dma_config_from_table_row(& (cfg->dma_d_config),&tab_ptr);
    nu_mpe_load_dma_config_from_table_row(& (cfg->dma_w_config),&tab_ptr);
    res=0;
  }

  if (res) rumboot_printf("Error:%s: Couldn't find row match\n", __func__);

  return res;
}

int nu_mpe_decide_dma_config_trivial(ConfigMPE* cfg, CubeMetrics* cube_metrics, WarrMetrics* warr_metrics) {
  
  uint32_t sizeof_C;
  // uint32_t sizeof_C_div128;
  
  // uint32_t Hp;
  // uint32_t Wp;
  // uint32_t Rd;
  // uint32_t Sd;
  // uint32_t Ho;
  // uint32_t Wo;
  
  // Hp  = cfg->H+cfg->Tp+cfg->Bp; // H after null padding
  // Wp  = cfg->W+cfg->Lp+cfg->Rp; // W after null padding

  // Rd  = (cfg->R-1)*cfg->Dr + 1; // R kernel size after dilation
  // Sd  = (cfg->S-1)*cfg->Ds + 1; // S kernel size after dilation

  // Ho  = (Hp-Rd)/cfg->Sh + 1; // Hout with respect to striding, dilation, null padding, conv
  // Wo  = (Wp-Sd)/cfg->Sw + 1; // Wout with respect to striding, dilation, null padding, conv

  
  
    // [C/128] - it is a size in bytes, division with rounding up
  sizeof_C = cfg->C << (cfg->dt == DataType_Int16 || cfg->dt == DataType_Fp16 ? 1 : 0);

  /*
  if (sizeof_C > 127) {
    if ((sizeof_C & 0x7F) > 0)  sizeof_C_div128 = (sizeof_C >> 7) + 1;
    else                        sizeof_C_div128 = sizeof_C >> 7;
  }
  else sizeof_C_div128 = 1;


  cfg->ma_config. ADD_CountI0 = 16;
  cfg->ma_config. CMP_CountI0 = cfg->K;
  cfg->ma_config. ADD_CountI1 = cfg->Sw;
  cfg->ma_config. CMP_CountI1 = cfg->S;
  cfg->ma_config. ADD_CountJ  = cfg->Sw;
  cfg->ma_config. CMP_CountJ  = cfg->S;
  cfg->ma_config. ADD_CountPPS= 1;
  cfg->ma_config. CMP_CountPPS= (cfg->S < cfg->Sw) ? cfg->S : cfg->Sw;
  cfg->ma_config. ADD_CountLCZ= 1;
  cfg->ma_config. CMP_CountLCZ= sizeof_C_div128;
  cfg->ma_config. ADD_CountPLC= 1;
  cfg->ma_config. CMP_CountPLC= cfg->R;
  cfg->ma_config. ADD_CountX  = 128;
  cfg->ma_config. CMP_CountX  = Wo;
  cfg->ma_config. ADD_CountY  = 1;
  cfg->ma_config. CMP_CountY  = Ho;
  cfg->ma_config. ADD_CountM  = 128;
  cfg->ma_config. CMP_CountM  = cfg->K;
  
  cfg->ma_config. BRDR  = 32*16 >> 1;
  cfg->ma_config. WA    = 32*16 >> 1;
  cfg->ma_config. DA    = 0;
  cfg->ma_config. VRA   = 0;
  cfg->ma_config. NR    = 0x7F;
  cfg->ma_config. D_BIAS= 1;
  cfg->ma_config. D_NULL= 0xFF;
  */
  
  //////////////////////////////
  cfg->dma_d_config.rdma.LPXEn = (cfg->Lp) ? Enable_En : Enable_NotEn;
  cfg->dma_d_config.rdma.RPXEn = (cfg->Rp) ? Enable_En : Enable_NotEn;
  cfg->dma_d_config.rdma.TPYEn = (cfg->Tp) ? Enable_En : Enable_NotEn;
  cfg->dma_d_config.rdma.BPYEn = (cfg->Bp) ? Enable_En : Enable_NotEn;
  
  //cfg->dma_d_config.rdma.AOffset = -(cfg->W * sizeof_C * cfg->Tp + sizeof_C * cfg->Lp);
  //cfg->dma_d_config.rdma.LPXOffset=-(cfg->W * sizeof_C * cfg->Tp);
  //cfg->dma_d_config.rdma.RPXOffset=-(cfg->W * sizeof_C * (cfg->Tp-1));
  //cfg->dma_d_config.rdma.TPYOffset= 0 ;
  //cfg->dma_d_config.rdma.BPYOffset = cfg->H * cfg->W * sizeof_C;
  //cfg->dma_d_config.rdma.CntThresholdSha = sizeof_C>128 ? sizeof_C-1 : 0;  // -1 - Accordind To CntThresholdSha Encoding
  
  for(int i=0;i<7;i++)
 
  cfg->dma_d_config.rdma.LPXData=0;
  cfg->dma_d_config.rdma.RPXData=0;
  cfg->dma_d_config.rdma.TPYData=0;
  cfg->dma_d_config.rdma.BPYData=0;
  
  /*
  cfg->dma_d_config.rdma.Bias[6].BiasEn = Enable_NotEn;
  cfg->dma_d_config.rdma.Bias[6].ThreCtrl = 0;
  cfg->dma_d_config.rdma.Bias[6].DecCtrl = 0;
  cfg->dma_d_config.rdma.Bias[6].PXBSEn = Enable_NotEn;
  cfg->dma_d_config.rdma.Bias[6].PYBSEn = Enable_NotEn;
  cfg->dma_d_config.rdma.Bias[6].Bias = 0;
  cfg->dma_d_config.rdma.Bias[6].CntSha = 0;
  cfg->dma_d_config.rdma.Bias[6].CntOffsetEn = 0;
  cfg->dma_d_config.rdma.Bias[6].CntOffset = 0;
  cfg->dma_d_config.rdma.Bias[6].CntThresholdSha = 0;
  cfg->dma_d_config.rdma.Bias[6].CntCmp = 0;
  */
  
  ////////////////////////////
  cfg->dma_w_config.rdma.LPXEn = Enable_NotEn;
  cfg->dma_w_config.rdma.RPXEn = Enable_NotEn;
  cfg->dma_w_config.rdma.TPYEn = Enable_NotEn;
  cfg->dma_w_config.rdma.BPYEn = Enable_NotEn;
  
  //cfg->dma_w_config.rdma.AOffset =0;
  //cfg->dma_w_config.rdma.LPXOffset=0;
  //cfg->dma_w_config.rdma.RPXOffset=0;
  //cfg->dma_w_config.rdma.TPYOffset= 0;
  //cfg->dma_w_config.rdma.BPYOffset= 0;
  //cfg->dma_w_config.rdma.CntThresholdSha = sizeof_C>128 ? sizeof_C-1 : 0;
   
  cfg->dma_w_config.rdma.LPXData=0;
  cfg->dma_w_config.rdma.RPXData=0;
  cfg->dma_w_config.rdma.TPYData=0;
  cfg->dma_w_config.rdma.BPYData=0;
  
  
  return 0;
}

uint32_t nu_mpe_get_warr_offset(void* cmd, MPECmdMetrics* metrics) {
  uint32_t cmd_size;
  uint32_t* ptr;
  uint32_t data;
  uint32_t reg_addr;
  
  cmd_size = metrics->s / 8;
  
  ptr = (uint32_t*) cmd;
  for(int i=0;i<cmd_size;i++) {
    data = *ptr;
    ptr++;
    reg_addr = *ptr;
    ptr++;
    if(reg_addr == 0x000000006) {
      return data * (1024/8);
    }
  }
  
  return 0;
}

void nu_mpe_rdma_setup(uintptr_t base, ConfigRDDMAMPE* cfg) {
  uint32_t temp_BiasCtrl;
  uint32_t temp_ThreCtrl;
  uint32_t temp_DecCtrl;
  uint32_t temp_PadCtrl;
  uint32_t temp_BiasBase;
  temp_BiasCtrl = 0;
  temp_ThreCtrl = cfg->ThreCtrl;
  temp_DecCtrl = 0;
  temp_PadCtrl = 0;
  temp_BiasBase = 0; // Offset Of The First Bias Couple
  for(int i=0;i<7;i++) {
      // Separate Regs
    iowrite32(cfg->Bias[i].Bias   , base + temp_BiasBase + Bias1Sha_MSha);
    iowrite32(cfg->Bias[i].CntSha , base + temp_BiasBase + RD_Bias1CntSha_MSha);

    if (i<6) {
      iowrite32( 
               (cfg->Bias[i].CntOffsetEn<<8) | (cfg->Bias[i].CntOffset<<0),
               base + temp_BiasBase +  Bias1CntOffset_MSha
      );
      iowrite32(cfg->Bias[i].CntThresholdSha, base + temp_BiasBase + Bias1CntThresholdSha_MSha);
    }
    
    if (i==6) {
      temp_BiasBase -= 0x4;
    }
    iowrite32(cfg->Bias[i].CntCmp , base + temp_BiasBase + RD_Bias1CntCmp_MSha);
    
      // Accumulate Common Regs
    temp_BiasCtrl = temp_BiasCtrl | (cfg->Bias[i].BiasEn << i);

    if (i<6) {
      temp_ThreCtrl = temp_ThreCtrl | (
        ( (uint32_t)cfg->Bias[i].ThreCtrl ) << ((i+1)*3)
      ); // *3 :-( May Be Done By Looped+ // +1 - Because The lsb Position is For cfg->ThreCtrl

      temp_DecCtrl  = temp_DecCtrl  | (
        ( (uint32_t)cfg->Bias[i].DecCtrl ) << (i*3)
      );
    }

    temp_PadCtrl  = temp_PadCtrl  | (cfg->Bias[i].PXBSEn << (i+4)) | (cfg->Bias[i].PYBSEn << (i+12));
    
      // Point At The Next Bias Couple
    temp_BiasBase += Bias2Sha_MSha-Bias1Sha_MSha; 
  }
  iowrite32(temp_BiasCtrl,base + BIASCtrl_MSha);
  iowrite32(temp_ThreCtrl,base + RD_THRECtrl_MSha);
  iowrite32(temp_DecCtrl ,base + RD_DECCtrl_MSha);
  
  temp_PadCtrl = temp_PadCtrl | 
    (cfg->LPXEn << 0) | (cfg->RPXEn << 1) | (cfg->TPYEn << 2) | (cfg->BPYEn << 3) ;
  iowrite32(temp_PadCtrl,base + PADCtrl_MSha);
  
  iowrite32(cfg->BFCA           ,base + BFCA_MSha);
  iowrite32(cfg->AOffset        ,base + AOffset_MSha);
  iowrite32(cfg->LPXOffset      ,base + LPXOffset_MSha);
  iowrite32(cfg->RPXOffset      ,base + RPXOffset_MSha);
  iowrite32(cfg->TPYOffset      ,base + TPadYOffset_MSha);
  iowrite32(cfg->BPYOffset      ,base + BPadYOffset_MSha);
  iowrite32(cfg->CntSha         ,base + CntSha_MSha);
  iowrite32(cfg->CntThresholdSha,base + CntThresholdSha_MSha);
  iowrite32(cfg->LPXData        ,base + LPXDR_MSha);
  iowrite32(cfg->RPXData        ,base + RPXDR_MSha);
  iowrite32(cfg->TPYData        ,base + TPYDR_MSha);
  iowrite32(cfg->BPYData        ,base + BPYDR_MSha);
}
void nu_mpe_wdma_setup(uintptr_t base, ConfigWRDMAMPE* cfg) {
  uint32_t temp_BiasBase;
  uint32_t temp;
  temp_BiasBase = 0;
  for(int i=0;i<6;i++) {
    iowrite32(cfg->Bias[i].CntSha,base + temp_BiasBase + WR_Bias1CntSha_MSha);
    iowrite32(cfg->Bias[i].CntCmp,base + temp_BiasBase + WR_Bias1CntCmp_MSha);
    
    temp_BiasBase += WR_Bias2CntSha_MSha-WR_Bias1CntSha_MSha;
  }
  temp = 0 | (cfg->BStrideX << 0) | (cfg->BConvX << 4) | (cfg->Batch << 8);
  iowrite32(temp,base + X_Cfg_MSha);
  
  temp = 0 | (cfg->Thre_PLC << 0) | (cfg->Thre_VLC << 3) | (cfg->Thre_CXC << 6);
  iowrite32(temp,base + WR_THRECtrl_MSha);
  
  temp = 0 | (cfg->Dec_PLC << 0);
  iowrite32(temp,base + WR_DECCtrl_MSha);
  
  temp = 0 | (cfg->USED << 0) | (cfg->ADR_PROL << 1) | (cfg->C_BND << 2) | (cfg->GLUE_EN << 3);
  iowrite32(temp,base + CHCfg_MSha);
  
  iowrite32(cfg->BADR, base + BADR_MSha);
  iowrite32(cfg->LADR, base + LADR_MSha);
  iowrite32(cfg->PLC_CntSha, base + PLC_CntSha_MSha);
  iowrite32(cfg->PLC_ThreSha,base + PLC_ThreSha_MSha);
  iowrite32(cfg->VLC_CntSha, base + VLC_CntSha_MSha);
  iowrite32(cfg->VLC_ThreSha,base + VLC_ThreSha_MSha);
}


void nu_mpe_dma_setup(uintptr_t base, ConfigDMAMPE* cfg) {
  nu_mpe_rdma_setup(base /*Left Unchanged*/, & cfg->rdma);
  nu_mpe_wdma_setup(base /*Left Unchanged*/, & cfg->wdma);
  iowrite32(cfg->MAINCNT,base + MAINCNT_Sha);
}

void nu_mpe_ma_setup(uintptr_t base, ConfigMPE* cfg) {
  uint32_t temp;
  
  iowrite32(cfg->ma_config. ADD_CountI0 ,base + MPE_ADD_CountI0);
  iowrite32(cfg->ma_config. CMP_CountI0 ,base + MPE_CMP_CountI0);
  iowrite32(cfg->ma_config. ADD_CountI1 ,base + MPE_ADD_CountI1);
  iowrite32(cfg->ma_config. CMP_CountI1 ,base + MPE_CMP_CountI1);
  iowrite32(cfg->ma_config. ADD_CountJ  ,base + MPE_ADD_CountJ);
  iowrite32(cfg->ma_config. CMP_CountJ  ,base + MPE_CMP_CountJ);
  iowrite32(cfg->ma_config. ADD_CountPPS,base + MPE_ADD_CountPPS);
  iowrite32(cfg->ma_config. CMP_CountPPS,base + MPE_CMP_CountPPS);
  iowrite32(cfg->ma_config. ADD_CountLCZ,base + MPE_ADD_CountLCZ);
  iowrite32(cfg->ma_config. CMP_CountLCZ,base + MPE_CMP_CountLCZ);
  iowrite32(cfg->ma_config. ADD_CountPLC,base + MPE_ADD_CountPLC);
  iowrite32(cfg->ma_config. CMP_CountPLC,base + MPE_CMP_CountPLC);
  iowrite32(cfg->ma_config. ADD_CountX  ,base + MPE_ADD_CountX);
  iowrite32(cfg->ma_config. CMP_CountX  ,base + MPE_CMP_CountX);
  iowrite32(cfg->ma_config. ADD_CountY  ,base + MPE_ADD_CountY);
  iowrite32(cfg->ma_config. CMP_CountY  ,base + MPE_CMP_CountY);
  iowrite32(cfg->ma_config. ADD_CountM  ,base + MPE_ADD_CountM);
  iowrite32(cfg->ma_config. CMP_CountM  ,base + MPE_CMP_CountM);
  iowrite32(cfg->ma_config. BRDR        ,base + MPE_COMMON_IN_BUF_CFG);
  iowrite32(cfg->ma_config. WA          ,base + MPE_COMMON_WA);
  iowrite32(cfg->ma_config. DA          ,base + MPE_COMMON_DA);
  iowrite32(cfg->ma_config. VRA         ,base + MPE_COMMON_VRA);
  iowrite32(cfg->ma_config. NR          ,base + MPE_COMMON_NR);
  iowrite32(cfg->ma_config. D_BIAS      ,base + MPE_COMMON_D_BIAS);
  iowrite32(cfg->ma_config. D_NULL      ,base + MPE_NULL);
  
  temp = cfg->dt == DataType_Fp16 ? 3 :
         cfg->dt == DataType_Int16? 2 :
       /*cfg->dt == DataType_Int8*/  0 ;
  iowrite32(temp ,base + MPE_COMMON_IN_FORMAT);
  
  temp = (cfg->rnd_size << 0) | (cfg->sat_en << 8) | (cfg->rnd_mode << 16);
  iowrite32(temp, base + MPE_COMMON_NORM_PARAM);
}


void nu_mpe_setup(uintptr_t base, ConfigMPE* cfg) {
  rumboot_printf("Configuring MPE..\n");

  nu_mpe_ma_setup(base + MPE_MA_BASE, cfg);
  nu_mpe_dma_setup(base + MPE_RDMA_D_BASE, & cfg->dma_d_config);
  nu_mpe_dma_setup(base + MPE_RDMA_W_BASE, & cfg->dma_w_config);
}

void nu_mpe_wait_ready(uintptr_t base) {
  while(ioread32(base + MPE_RDMA_D_BASE + DMA_STS) != 0) {}
  while(ioread32(base + MPE_RDMA_W_BASE + DMA_STS) != 0) {}
}

void nu_mpe_run(uintptr_t mpe_base, ConfigMPE* cfg) {
  rumboot_printf("Start MPE...\n");
  iowrite32(cfg->dma_d_config.depend_mask | (1<<0), mpe_base + MPE_RDMA_D_BASE + DMA_START);
  iowrite32(cfg->dma_w_config.depend_mask | (1<<0), mpe_base + MPE_RDMA_W_BASE + DMA_START);
  iowrite32(0,mpe_base + MPE_MA_BASE + MPE_CMD_ICMW);
}

void nu_mpe_run_dmas_only(uintptr_t mpe_base, ConfigMPE* cfg) {
  rumboot_printf("Start MPE_DMAs...\n");
  iowrite32(cfg->dma_d_config.depend_mask | (1<<0), mpe_base + MPE_RDMA_D_BASE + DMA_START);
  iowrite32(cfg->dma_w_config.depend_mask | (1<<0), mpe_base + MPE_RDMA_W_BASE + DMA_START);
}

void nu_mpe_run_ma_only(uintptr_t mpe_base, ConfigMPE* cfg) {
  rumboot_printf("Start MPE_MA...\n");
  iowrite32(0,mpe_base + MPE_MA_BASE + MPE_CMD_ICMW);
}

int  nu_mpe_run_cmd(uintptr_t base, void* cmd, MPECmdMetrics* cmd_metrics) {
  uint32_t offset;
  uint32_t data;
  int num_cmds;
  uint32_t* ptr;

  ptr = (uint32_t*) cmd;
  num_cmds = cmd_metrics->s / 8;
  for(int i=0;i<num_cmds;i++) {
    data = *ptr;
    ptr++;
    offset = *ptr;
    ptr++;
    rumboot_printf("Writing data=%x addr = %x\n", data, offset);
    iowrite32(data,base + (offset*4));
  }
  return 0;
}

void nu_mpe_wait(uintptr_t mpe_base, ConfigDMAMPE* cfg_dma) {

  // while (ioread32(mpe_base + NU_MPE_MYREG)) != ...) {}
}

void nu_mpe_load_buf(uintptr_t base, void* data, int size) {
  int size32words;
  uint32_t* data_ptr;
  uintptr_t buf_ptr;
  uint32_t buf_size_mi1;
  uint32_t buf_append_size;

  size32words = size >> 2;
  data_ptr = (uint32_t*) data;
  buf_ptr = base;

  for(int i=0;i<size32words;i++) {
    iowrite32(*data_ptr,buf_ptr);
    data_ptr++;buf_ptr=buf_ptr+sizeof(uint32_t);
  }

    // Append Zeroes For An Entire Buffer Partition
  buf_size_mi1 = (NU_MPE_BUF01) - 1; // Hmm, The Size Of The Partition Is An Offset Of 2nd Partition
  buf_append_size = buf_ptr & buf_size_mi1;
  if(buf_append_size != 0) {
    size32words = buf_append_size >> 2;
    for(int i=0;i<size32words;i++) {
      iowrite32(0,buf_ptr);
      buf_ptr=buf_ptr+sizeof(uint32_t);
    }
  }
    //

}

void nu_mpe_config_wr_main_channel(uintptr_t dma_base, void *addr, int size){
  nu_cpdmac_rcv512_config(dma_base,addr,size);
}

void nu_mpe_run_wr_main_channel(uintptr_t dma_base) {
  nu_cpdmac_rcv512_run(dma_base);
}

void nu_mpe_wait_wr_main_channel_complete(uintptr_t dma_base) {
  nu_cpdmac_rcv512_wait_complete(dma_base);
}

void  nu_ppe_decide_dma_config_trivial(ConfigPPE* cfg, CubeMetrics* out_cube_metrics, ConfigREGPPE* cfg_reg) {

  uint32_t dt   = cfg->dt   ;
  uint32_t meth = cfg->meth ;

  int32_t Hi  = cfg->H;
  int32_t Wi  = cfg->W;
  int32_t Ci  = cfg->C;
  int32_t Sh  = cfg->Sh;
  int32_t Kh  = cfg->Kh;
  int32_t Sw  = cfg->Sw;
  int32_t Kw  = cfg->Kw;

  int32_t Lp  = cfg->Lp;
  int32_t Rp  = cfg->Rp;

  int32_t nan_to_zero_input = cfg->nan_to_zero_input;

  int32_t Ho  = out_cube_metrics->H;
  int32_t Wo  = out_cube_metrics->W;
  int32_t Co  = out_cube_metrics->C;

  uint32_t fm = (cfg_reg->wOpM >> 8)&0x7;

  // OP_MODE[29] reserved bit may be used here
  uint32_t mr = (cfg_reg->wOpM >> 29)&0x1 ?
                (cfg_reg->wOpM >> 28)&0x1 :
                fm&0x1 && dt || !(fm&0x1) && (dt&&(Ci>8) || Ci>16) ? 0x1 : 0x0
  ;

  int el_s      = dt ? 0x2 : 0x1; // element size in bytes
  int rwdma_thr = 0x10;           // rdma throughput in bytes; wdma_thr is the same
  int rdctn     = mr + 1;         // reduction

  int frgs  = rwdma_thr * rdctn;  // dma transfer quantum in bytes; fragment size

  int Ci_s  = Ci * el_s;
  int Co_s  = Co * el_s;

  int frgli = Ci_s % frgs; // input fragment last size
  int frglo = Co_s % frgs; // output fragment last size

  int Kh_d_Sh, rndup_Kh_d_Sh;

  int Buffer_md = 2048;
  int HWC_MAX = 0x1fff;

  if (Sh>0 && Kh>0) Kh_d_Sh = (Kh-1)%Sh == 0 ? (Kh-1)/Sh : (Kh-1)/Sh + 1; // Kh/Sh
  else rumboot_printf("Error:nu_ppe_decide_dma_config_trivial: cfg->Sh<=0 || cfg->Kh<=0\n");

  rndup_Kh_d_Sh = Kh_d_Sh == 0 || Kh_d_Sh == 1 ? 1 :
                  Kh_d_Sh == 2 ? 2 :
                  Kh_d_Sh == 3 || Kh_d_Sh == 4 ? 4 :
                  Kh_d_Sh > 4 ? 8 : 8
  ;

  int Wout_splt, Win_splt, Wout_splt_st, Win_splt_st, Win_offset;

  if (fm==0x4 || fm==0x5) {
    Wout_splt = Buffer_md/(rndup_Kh_d_Sh*rdctn);

    Win_splt = Kw>Sw ? (Wout_splt - 1)*Sw + Kw : Wout_splt*Sw;

    Wout_splt_st = (Wo % Wout_splt == 0) || (Wi + Lp)%Win_splt >= Kw ? Wout_splt : Wout_splt/2;

    Win_splt_st = Kw>Sw ? (Wout_splt_st - 1)*Sw + Kw - Lp : Wout_splt_st * Sw - Lp;

    Win_offset = Kw>Sw ? Kw - Sw : 0x0;
  }

  int Win_box_st = 128;
  int Win_box = 128;
  int Wout_box_st, Wout_box, rem_Wout_box_st, rem_Wout_box;

  if (fm&0x2) {
    Wout_box_st = Wi>Win_box ? (Win_box_st + Lp - Kw)/Sw + 1 :
    (Win_box_st + Lp + Rp - Kw)/Sw + 1;

    rem_Wout_box_st = Kw>Sw ? Kw-Sw + (Win_box_st + Lp - Kw)%Sw : (Win_box_st + Lp)%Sw;

    Wout_box = Wi <= Win_box_st+Win_box ? (rem_Wout_box_st + Win_box + Rp - Kw)/Sw+1 :
                                          (rem_Wout_box_st + Win_box - Kw)/Sw+1
    ;
  }

  if (!(fm&0x1)) {  // MEMtoPPE
    //cfg_reg->rAXIp  =
    cfg_reg->rBrdX  = (Wi-1) * Ci_s;
    cfg_reg->rBrdY  = (Hi-1) * Wi * Ci_s;
    cfg_reg->rBrdZ  = frgli>0 ? (int)(Ci_s/frgs)*frgs : ((int)(Ci_s/frgs) - 1)*frgs;

    cfg_reg->rStrX  = Ci_s;
    cfg_reg->rStrY  = Wi * Ci_s;
    cfg_reg->rStrZ  = frgs;

    cfg_reg->rFrgs  = frgs;

    cfg_reg->rFrgl  = frgli>0 ? frgli : frgs;

    cfg_reg->rXYZd  = DmaXYZDirection_Z;

    if (!(fm&0x2) && !(fm&0x4)) { // linear
      cfg_reg->rBstX  = Wi-1;
      cfg_reg->rBstY  = Hi-1;
      cfg_reg->rBstZ  = 0x0;
    }
    else if (fm&0x2 && !(fm&0x4)) { // boxed
      cfg_reg->rBstX  = 0x80 - 1; // 128 element box
      cfg_reg->rBstY  = 0x0     ; // 1 element
      cfg_reg->rBstZ  = 0x8 - 1 ; // 128 channels/frgs = 128*(1B|2B)/(16B|32B)
    }

    if (fm&0x4) {  // split
      if (Win_splt_st - Win_offset - 1 > HWC_MAX) cfg_reg->rBstX = HWC_MAX;
      else cfg_reg->rBstX = Win_splt_st - Win_offset - 1;

      if (Win_splt - Win_offset - 1 > HWC_MAX) cfg_reg->rBxtX = HWC_MAX;
      else cfg_reg->rBxtX = Win_splt - Win_offset - 1;

      cfg_reg->rBffX = Win_offset;

      cfg_reg->rBstY = Hi-1;
      cfg_reg->rBstZ = 0x0;
    }
    else {
      cfg_reg->rBxtX  = cfg_reg->rBstX;
      cfg_reg->rBffX  = 0x0;
    }

    cfg_reg->rBxtY  = cfg_reg->rBstY;
    cfg_reg->rBxtZ  = cfg_reg->rBstZ;

    cfg_reg->rBffY  = 0x0;
    cfg_reg->rBffZ  = 0x0;
  }
  else {
    cfg_reg->rBALi = 0x0;
    cfg_reg->rBrdX = 0x0;
    cfg_reg->rBrdY = 0x0;
    cfg_reg->rBrdZ = 0x0;
    cfg_reg->rStrX = 0x0;
    cfg_reg->rStrY = 0x0;
    cfg_reg->rStrZ = 0x0;
    cfg_reg->rFrgs = 0x0;
    cfg_reg->rFrgl = 0x0;
    cfg_reg->rXYZd = 0x0;
    cfg_reg->rBstX = 0x0;
    cfg_reg->rBstY = 0x0;
    cfg_reg->rBstZ = 0x0;
    cfg_reg->rBxtX = 0x0;
    cfg_reg->rBxtY = 0x0;
    cfg_reg->rBxtZ = 0x0;
    cfg_reg->rBffX = 0x0;
    cfg_reg->rBffY = 0x0;
    cfg_reg->rBffZ = 0x0;
  }

  //cfg_reg->wAXIp  =
  cfg_reg->wBrdX  = (Wo-1) * Co_s;
  cfg_reg->wBrdY  = (Ho-1) * Wo * Co_s;
  cfg_reg->wBrdZ  = frglo>0 ? (int)(Co_s/frgs)*frgs : ((int)(Co_s/frgs) - 1)*frgs;

  cfg_reg->wStrX  = Co_s;
  cfg_reg->wStrY  = Wo * Co_s;
  cfg_reg->wStrZ  = frgs;

  cfg_reg->wFrgs  = frgs;
  cfg_reg->wFrgl  = frglo>0 ? frglo : frgs;
  cfg_reg->wXYZd  = DmaXYZDirection_Z;

  if (!(fm&0x2) && !(fm&0x4)) { // linear
    cfg_reg->wBstX  = Wo-1;
    cfg_reg->wBxtX  = Wo-1;

    cfg_reg->wBstY  = Ho-1;
    cfg_reg->wBstZ  = 0x0;

    cfg_reg->wIstX  = 0x0;
    cfg_reg->wIxtX  = 0x0;
    cfg_reg->wIffX  = 0x0;

    cfg_reg->wBxtY  = Ho-1;
    cfg_reg->wBxtZ  = 0x0;

    cfg_reg->wBffX  = 0x0;
    cfg_reg->wBffY  = 0x0;
    cfg_reg->wBffZ  = 0x0;
  }
  else if (fm&0x2 && !(fm&0x4)) { // boxed limitated
    cfg_reg->wIstX  = 0x80-1;
    cfg_reg->wIxtX  = 0x80-1;
    cfg_reg->wIffX  = 0x0   ;

    cfg_reg->wBstX  = Wout_box_st - 1;
    cfg_reg->wBstY  = 0x0   ;
    cfg_reg->wBstZ  = 0x8-1 ;
 
    cfg_reg->wBxtX  = Wout_box - 1;  
    cfg_reg->wBxtY  = 0x0;
    cfg_reg->wBxtZ  = 0x8-1;
    
    cfg_reg->wBffX  = 0x0;
    cfg_reg->wBffY  = 0x0;
    cfg_reg->wBffZ  = 0x0;
  }

  if (fm&0x4) {  // split
    cfg_reg->wBstX  = Wout_splt_st-1;
    cfg_reg->wBstY  = Ho-1;
    cfg_reg->wBstZ  = 0x0;

    cfg_reg->wIstX  = cfg_reg->rBstX;
    cfg_reg->wIxtX  = cfg_reg->rBxtX;
    cfg_reg->wIffX  = cfg_reg->rBffX; 

    cfg_reg->wBxtX  = Wout_splt-1;
    cfg_reg->wBxtY  = Ho-1;
    cfg_reg->wBxtZ  = 0x0;

    cfg_reg->wBffX  = 0x0;
    cfg_reg->wBffY  = 0x0;
    cfg_reg->wBffZ  = 0x0;
  }

  cfg_reg->wWi  = Wi - 1;
  cfg_reg->wHi  = Hi - 1;
  cfg_reg->wCi  = Ci - 1;
  cfg_reg->wWo  = Wo - 1;
  cfg_reg->wHo  = Ho - 1;
  cfg_reg->wCo  = Co - 1;

  cfg_reg->wOpM = mr<<28 | nan_to_zero_input<<24 | dt<<16 | fm<<8 | meth;

  cfg_reg->wK   = (cfg->Sh-1)<<20 | (cfg->Sw-1)<<16 | (cfg->Kh-1)<<8 | (cfg->Kw-1);

  cfg_reg->wKWHr = cfg->Kwh_r;

  cfg_reg->wP   = cfg->Bp<<12 | cfg->Rp<<8 | cfg->Tp<<4 | cfg->Lp;
  cfg_reg->wPV1 = cfg->pv[0];
  cfg_reg->wPV2 = cfg->pv[1];
  cfg_reg->wPV3 = cfg->pv[2];
  cfg_reg->wPV4 = cfg->pv[3];
  cfg_reg->wPV5 = cfg->pv[4];
  cfg_reg->wPV6 = cfg->pv[5];
  cfg_reg->wPV7 = cfg->pv[6];
}

void nu_ppe_setup_reg(uintptr_t rbase, uintptr_t wbase, ConfigREGPPE* cfg) {

  rumboot_printf("nu_ppe_setup_reg\n");

  //iowrite32(0x0       , rbase + NU_PPE_OP_ENABLE          );
  //iowrite32(cfg->rAXIp, rbase + NU_PPE_RDMA_AXI_PARAM     );
  iowrite32(cfg->rBALi, rbase + NU_PPE_RDMA_BASE_ADDR     );
  iowrite32(cfg->rBrdX, rbase + NU_PPE_RDMA_BORDER_X      );
  iowrite32(cfg->rBrdY, rbase + NU_PPE_RDMA_BORDER_Y      );
  iowrite32(cfg->rBrdZ, rbase + NU_PPE_RDMA_BORDER_Z      );
  iowrite32(cfg->rStrX, rbase + NU_PPE_RDMA_STRIDE_X      );
  iowrite32(cfg->rStrY, rbase + NU_PPE_RDMA_STRIDE_Y      );
  iowrite32(cfg->rStrZ, rbase + NU_PPE_RDMA_STRIDE_Z      );
  iowrite32(cfg->rFrgs, rbase + NU_PPE_RDMA_FRAG_SIZE     );
  iowrite32(cfg->rFrgl, rbase + NU_PPE_RDMA_FRAG_LAST_SIZE);
  iowrite32(cfg->rXYZd, rbase + NU_PPE_RDMA_XYZ_DRCT      );
  iowrite32(cfg->rBstX, rbase + NU_PPE_RDMA_BOX_ST_SIZE_X );
  iowrite32(cfg->rBstY, rbase + NU_PPE_RDMA_BOX_ST_SIZE_Y );
  iowrite32(cfg->rBstZ, rbase + NU_PPE_RDMA_BOX_ST_SIZE_Z );
  iowrite32(cfg->rBxtX, rbase + NU_PPE_RDMA_BOX_SIZE_X    );
  iowrite32(cfg->rBxtY, rbase + NU_PPE_RDMA_BOX_SIZE_Y    );
  iowrite32(cfg->rBxtZ, rbase + NU_PPE_RDMA_BOX_SIZE_Z    );
  iowrite32(cfg->rBffX, rbase + NU_PPE_RDMA_BOX_OFFSET_X  );
  iowrite32(cfg->rBffY, rbase + NU_PPE_RDMA_BOX_OFFSET_Y  );
  iowrite32(cfg->rBffZ, rbase + NU_PPE_RDMA_BOX_OFFSET_Z  );
  
  //iowrite32(0x0       , wbase + NU_PPE_OP_ENABLE          );
  //iowrite32(cfg->wAXIp, wbase + NU_PPE_RDMA_AXI_PARAM     );
  iowrite32(cfg->wBALo, wbase + NU_PPE_WDMA_BASE_ADDR     );
  iowrite32(cfg->wBrdX, wbase + NU_PPE_WDMA_BORDER_X      );
  iowrite32(cfg->wBrdY, wbase + NU_PPE_WDMA_BORDER_Y      );
  iowrite32(cfg->wBrdZ, wbase + NU_PPE_WDMA_BORDER_Z      );
  iowrite32(cfg->wStrX, wbase + NU_PPE_WDMA_STRIDE_X      );
  iowrite32(cfg->wStrY, wbase + NU_PPE_WDMA_STRIDE_Y      );
  iowrite32(cfg->wStrZ, wbase + NU_PPE_WDMA_STRIDE_Z      );
  iowrite32(cfg->wFrgs, wbase + NU_PPE_WDMA_FRAG_SIZE     );
  iowrite32(cfg->wFrgl, wbase + NU_PPE_WDMA_FRAG_LAST_SIZE);
  iowrite32(cfg->wXYZd, wbase + NU_PPE_WDMA_XYZ_DRCT      );
  iowrite32(cfg->wBstX, wbase + NU_PPE_WDMA_BOX_ST_SIZE_X );
  iowrite32(cfg->wBstY, wbase + NU_PPE_WDMA_BOX_ST_SIZE_Y );
  iowrite32(cfg->wBstZ, wbase + NU_PPE_WDMA_BOX_ST_SIZE_Z );
  iowrite32(cfg->wBxtX, wbase + NU_PPE_WDMA_BOX_SIZE_X    );
  iowrite32(cfg->wBxtY, wbase + NU_PPE_WDMA_BOX_SIZE_Y    );
  iowrite32(cfg->wBxtZ, wbase + NU_PPE_WDMA_BOX_SIZE_Z    );
  iowrite32(cfg->wBffX, wbase + NU_PPE_WDMA_BOX_OFFSET_X  );
  iowrite32(cfg->wBffY, wbase + NU_PPE_WDMA_BOX_OFFSET_Y  );
  iowrite32(cfg->wBffZ, wbase + NU_PPE_WDMA_BOX_OFFSET_Z  );
  iowrite32(cfg->wIstX, wbase + NU_PPE_INP_BOX_ST_SIZE_X  );
  iowrite32(cfg->wIxtX, wbase + NU_PPE_INP_BOX_SIZE_X     );
  iowrite32(cfg->wIffX, wbase + NU_PPE_INP_BOX_OFFSET_X   );
  iowrite32(cfg->wWi  , wbase + NU_PPE_DATA_W_IN          );
  iowrite32(cfg->wHi  , wbase + NU_PPE_DATA_H_IN          );
  iowrite32(cfg->wCi  , wbase + NU_PPE_DATA_C_IN          );
  iowrite32(cfg->wWo  , wbase + NU_PPE_DATA_W_OUT         );
  iowrite32(cfg->wHo  , wbase + NU_PPE_DATA_H_OUT         );
  iowrite32(cfg->wOpM , wbase + NU_PPE_OP_MODE            );
  iowrite32(cfg->wK   , wbase + NU_PPE_KERNEL             );

  iowrite32(cfg->wKWHr , wbase + NU_PPE_RECIP_KERNEL_WH   );

  iowrite32(cfg->wP   , wbase + NU_PPE_PADDING            );
  iowrite32(cfg->wPV1 , wbase + NU_PPE_PADDING_VALUE_1    );
  iowrite32(cfg->wPV2 , wbase + NU_PPE_PADDING_VALUE_2    );
  iowrite32(cfg->wPV3 , wbase + NU_PPE_PADDING_VALUE_3    );
  iowrite32(cfg->wPV4 , wbase + NU_PPE_PADDING_VALUE_4    );
  iowrite32(cfg->wPV5 , wbase + NU_PPE_PADDING_VALUE_5    );
  iowrite32(cfg->wPV6 , wbase + NU_PPE_PADDING_VALUE_6    );
  iowrite32(cfg->wPV7 , wbase + NU_PPE_PADDING_VALUE_7    );
}

void nu_ppe_page_cmpl_mask (uintptr_t base) {
  //WDMA_PAGE_CMPL 0

  iowrite32(ioread32(base + NU_PPE_WDMA_INT_MASK) | 0x1, base + NU_PPE_WDMA_INT_MASK);
}

uint32_t nu_ppe_page_cmpl_status(uintptr_t base) {
  //WDMA_PAGE_CMPL 0

  return ioread32(base + NU_PPE_WDMA_INT_STATUS) & 0x1;
}

void nu_ppe_page_cmpl_reset(uintptr_t base) {
  //WDMA_PAGE_CMPL 0

  iowrite32(ioread32(base + NU_PPE_WDMA_INT_RESET) | 0x1, base + NU_PPE_WDMA_INT_RESET);
}

void nu_ppe_marked_page_cmpl_mask (uintptr_t base) {
  //WDMA_MARKED_PAGE_CMPL 1

  iowrite32(ioread32(base + NU_PPE_WDMA_INT_MASK) | 0x1<<1, base + NU_PPE_WDMA_INT_MASK);
}

uint32_t nu_ppe_marked_page_cmpl_status(uintptr_t base) {
  //WDMA_MARKED_PAGE_CMPL 1

  return (ioread32(base + NU_PPE_WDMA_INT_STATUS) & 0x1<<1) >> 1;
}

void nu_ppe_marked_page_cmpl_reset (uintptr_t base) {
  //WDMA_MARKED_PAGE_CMPL 1

  iowrite32(ioread32(base + NU_PPE_WDMA_INT_RESET) | 0x1<<1, base + NU_PPE_WDMA_INT_RESET);
}

void nu_ppe_wdma_err_mask (uintptr_t base) {
  //WDMA_IDERR 6 WDMA_SLVERR 5 WDMA_DECERR 4

  iowrite32(ioread32(base + NU_PPE_WDMA_INT_MASK) | 0x1<<6 | 0x1<<5 | 0x1<<4, base + NU_PPE_WDMA_INT_MASK);
}

uint32_t nu_ppe_wdma_err_status(uintptr_t base) {
  //WDMA_IDERR  6 WDMA_SLVERR 5 WDMA_DECERR 4

  uint32_t iderr  = (ioread32(base + NU_PPE_WDMA_INT_STATUS) & 0x1<<6) >> 6;
  uint32_t slverr = (ioread32(base + NU_PPE_WDMA_INT_STATUS) & 0x1<<5) >> 5;
  uint32_t decerr = (ioread32(base + NU_PPE_WDMA_INT_STATUS) & 0x1<<4) >> 4;

  if (iderr ) rumboot_printf ("WDMA_IDERR \n");
  if (slverr) rumboot_printf ("WDMA_SLVERR\n");
  if (decerr) rumboot_printf ("WDMA_DECERR\n");

  return iderr | slverr | decerr;
}

void nu_ppe_wdma_err_reset (uintptr_t base) {
  //WDMA_IDERR 6 WDMA_SLVERR 5 WDMA_DECERR 4

  iowrite32(ioread32(base + NU_PPE_WDMA_INT_RESET) | 0x1<<6 | 0x1<<5 | 0x1<<4, base + NU_PPE_WDMA_INT_RESET);

}

void nu_ppe_rdma_err_mask (uintptr_t base) {
  //RDMA_IDERR 6 RDMA_SLVERR 5 RDMA_DECERR 4

  iowrite32(ioread32(base + NU_PPE_RDMA_INT_MASK) | 0x1<<6 | 0x1<<5 | 0x1<<4, base + NU_PPE_RDMA_INT_MASK);
}

uint32_t nu_ppe_rdma_err_status(uintptr_t base) {
  //RDMA_IDERR 6 RDMA_SLVERR 5 RDMA_DECERR 4

  uint32_t iderr  = (ioread32(base + NU_PPE_RDMA_INT_STATUS) & 0x1<<6) >> 6;
  uint32_t slverr = (ioread32(base + NU_PPE_RDMA_INT_STATUS) & 0x1<<5) >> 5;
  uint32_t decerr = (ioread32(base + NU_PPE_RDMA_INT_STATUS) & 0x1<<4) >> 4;

  if (iderr ) rumboot_printf ("RDMA_IDERR \n");
  if (slverr) rumboot_printf ("RDMA_SLVERR\n");
  if (decerr) rumboot_printf ("RDMA_DECERR\n");

  return iderr | slverr | decerr;
}

void nu_ppe_rdma_err_reset (uintptr_t base) {
  //RDMA_IDERR 6 RDMA_SLVERR  5 RDMA_DECERR 4

  iowrite32(ioread32(base + NU_PPE_RDMA_INT_RESET) | 0x1<<6 | 0x1<<5 | 0x1<<4, base + NU_PPE_RDMA_INT_RESET);

}

// rdma
void nu_ppe_rdma_run(uintptr_t rbase, ConfigREGPPE* cfg) {
  rumboot_printf("Start PPE RDMA...\n");
  iowrite32(cfg->rOpEn,   rbase + NU_PPE_OP_ENABLE);
}
void nu_ppe_rdma_wait_complete(uintptr_t rbase){
  rumboot_printf("Wait PPE RDMA...\n");
  while(ioread32(rbase + NU_PPE_STATUS)) {}
  rumboot_printf("Done PPE RDMA...\n");
}

uint32_t nu_ppe_status_done (uintptr_t wbase) {
  return ioread32(wbase + NU_PPE_STATUS_DONE);
}

// ppe wdma run
void nu_ppe_wdma_run(uintptr_t wbase, ConfigREGPPE* cfg) {
  rumboot_printf("Start PPE WDMA...\n");
  iowrite32(cfg->wOpEn,   wbase + NU_PPE_OP_ENABLE);
}
void nu_ppe_wait_complete(uintptr_t wbase){
  rumboot_printf("Wait PPE WDMA...\n");
  while(ioread32(wbase + NU_PPE_STATUS)) {}
  rumboot_printf("Done PPE WDMA...\n");
}

void nu_ppe_dma_wait_ready_and_run(uintptr_t base) {
  while(ioread32(base + NU_PPE_STATUS)) {}
  iowrite32(1,base + NU_PPE_OP_ENABLE);
}

void nu_ppe_config_rd_main_channel(uintptr_t dma_base, void *addr, int size) {
  nu_cpdmac_trn256_config(dma_base,addr,size);
}

void nu_ppe_run_rd_main_channel(uintptr_t dma_base) {
  nu_cpdmac_trn256_run(dma_base);
}

void nu_ppe_wait_rd_main_channel_complete(uintptr_t dma_base){
  nu_cpdmac_trn256_wait_complete(dma_base);
}

void nu_vpe_config_rd_main_channel(uintptr_t dma_base, void *addr, int size) {
  nu_cpdmac_trn512_config(dma_base,addr,size);
}
void nu_vpe_run_rd_main_channel(uintptr_t dma_base) {
  nu_cpdmac_trn512_run(dma_base);
}
void nu_vpe_wait_rd_main_channel_complete(uintptr_t dma_base){
  nu_cpdmac_trn512_wait_complete(dma_base);
}

void nu_vpe_run(uintptr_t vpe_base, ConfigVPE* cfg){ // ?????????   ConfigVPE* cfg  
  // Напиши сюда 3апуск DMA
  uint32_t temp;
  do {
    temp = ioread32(vpe_base + NU_VPE + NU_VPE_NEXT_CNTX);
  }while((temp & 1) !=0);
  
  rumboot_printf("Start VPE...\n");
  
  temp = (temp & 0xE0FFFFFF) | cfg->depend_mask; // Manipulate With DPND Fields
  
  if(cfg->mark)
   {temp = temp | 0x00000003; }//MARKED_CNTX=1 & NEXT_CNTX=1 
  else 
    temp = (temp & (~(1<<1)) ) | (1<<0);  //MARKED_CNTX=0 | NEXT_CNTX=1
  
  iowrite32 (temp, vpe_base + NU_VPE + NU_VPE_NEXT_CNTX);
}

void nu_vpe_wait(uintptr_t vpe_base, ConfigVPE* cfg){
  if(! cfg->mark) {
    rumboot_printf("Wait VPE WDMA...\n");
    while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 8) & 1) !=1) {}
    iowrite32((1<<8),vpe_base + NU_VPE + NU_VPE_INT_RESET);
  }
  else {
    rumboot_printf("Wait VPE complete...\n");
    while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 12) & 1) !=1) {}
    iowrite32((1<<12),vpe_base + NU_VPE + NU_VPE_INT_RESET);
  }
  // rumboot_printf("Done VPE wait.\n");
}
void nu_vpe_set_int_mask(uintptr_t vpe_base){
	rumboot_printf("Read [%x]=%x\n",ioread32(vpe_base + NU_VPE + NU_VPE_NEXT_CNTX));
    if (( (ioread32(vpe_base + NU_VPE + NU_VPE_NEXT_CNTX) >> 12) & 1) ==0 )
    iowrite32((1<<12) | (1<<8)| (1<<0) | (1<<4) |(1<<5) | (1<<6) | (1<<7) | (1<<16),vpe_base + NU_VPE + NU_VPE_INT_MASK);  //IRQ_DEV_ON  IRQ_CUBE_CMPL IRQ_DEV_OFF 
	rumboot_printf("nu_vpe_set_int_mask: Writing [0x%x]=0x%x\n",vpe_base + NU_VPE + NU_VPE_INT_MASK,ioread32(vpe_base + NU_VPE + NU_VPE_INT_MASK));
}
void nu_npe_vpe_set_int_mask(uintptr_t npe_base){

	if (( (ioread32(npe_base  + NA_CU_REGS_BASE + NA_UNITS_MODE) >> 1) & 1) ==0 )
    iowrite32((1<<11) | (1<<12)| (1<<13) | (1<<14) |(1<<15) | (1<<16) | (1<<17) | (1<<18) | (1<<19),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK);  //IRQ_DEV_ON  IRQ_CUBE_CMPL IRQ_DEV_OFF 
	rumboot_printf("nu_npe_vpe_set_int_mask: Writing [0x%x]=0x%x\n",npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK,ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK));
}		
void nu_vpe_wait_cntx_appl(uintptr_t vpe_base){
    rumboot_printf("Wait VPE context got...\n");
    while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 0) & 1) !=1) {}
    iowrite32((1<<0),vpe_base + NU_VPE + NU_VPE_INT_RESET); 
	rumboot_printf("Done VPE context\n");
}
void nu_vpe_wait_int_dev_off(uintptr_t vpe_base){
	rumboot_printf("Wait VPE dev off got\n");
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 4) & 1) !=1) {}
    iowrite32( (1<<4),vpe_base + NU_VPE + NU_VPE_INT_RESET); 
	rumboot_printf("Done VPE dev off\n");
}
void nu_vpe_wait_int_pause_cntx_appl(uintptr_t vpe_base){
	rumboot_printf("Wait VPE context got\n");
    while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 7) & 1) !=1) {}
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 16) & 1) !=1) {}
    iowrite32(((1<<7) | (1<<16)),vpe_base + NU_VPE + NU_VPE_INT_RESET); 
	rumboot_printf("Done VPE context\n");
}

void nu_na_vpe_wait_int_pause_cntx_appl(uintptr_t npe_base){
	rumboot_printf("Wait_ NA_VPE context got\n");
    while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 12) & 1) !=1) {}
	iowrite32( (1<<12),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 14 ) & 1) !=1) {}
	iowrite32( (1<<14),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_VPE context\n");
}
void nu_vpe_wait_marked_cube_complete(uintptr_t vpe_base){
	rumboot_printf("Wait marked cube complete...\n");
	while (( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS)>> 12) & 1) !=1) {}
	{iowrite32( (1<<12),vpe_base + NU_VPE + NU_VPE_INT_RESET);
	rumboot_printf("Done VPE marked cube complete...\n");}
}
void nu_na_ppe_wait_int_pause(uintptr_t npe_base){
	rumboot_printf("Wait_ NA_PE context got\n");
    while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 22) & 1) !=1) {}
	while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 24 ) & 1) !=1) {}   	
	while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 30 ) & 1) !=1) {} 
	iowrite32(((1<<21) | (1<<22) |  (1<<23) | (1<<24) |  (1<<2) |  (1<<30) ),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_PPE context\n");
}
void nu_na_ppe_rdma_wait_complete(uintptr_t npe_base){
	rumboot_printf("Wait NA PPE ...\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS)>> 27) & 1) !=1) {}
	{iowrite32( (1<<27),npe_base + NA_CU_REGS_BASE   + NA_INT_UNITS_STATUS);
	rumboot_printf("Done PPE RDMA...\n");}
}
void nu_na_ppe_wait_complete(uintptr_t npe_base){
	rumboot_printf("Wait PPE WDMA...\n");
	while (( (ioread32(npe_base + NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS)>> 28) & 1) !=1) {}
	{iowrite32( (1<<28),npe_base + NA_CU_REGS_BASE   + NA_INT_UNITS_STATUS);
	rumboot_printf("Done PPE WDMA...\n");}
}

void nu_na_ppe_dev_resume(uintptr_t npe_base){
    rumboot_printf(" NA_PPE finished cube handle ...\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 31) & 1) !=1) {}
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 30) & 1) !=1) {}	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 21) & 1) !=1) {}	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 22) & 1) !=1) {}
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 28) & 1) !=1) {}	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 27) & 1) !=1) {}	
	iowrite32(((1<<31) | (1<<30) | (1<<21)  | (1<<22) |  (1<<28) |  (1<<27)),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_PPE end \n");
}

void nu_vpe_wait_int_cntx_appl(uintptr_t vpe_base){
	rumboot_printf("Wait VPE context got\n");
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 8) & 1) !=1) {}
    iowrite32( (1<<8),vpe_base + NU_VPE + NU_VPE_INT_RESET); 
	rumboot_printf("Done VPE context\n");
}
void nu_na_vpe_wait_int_dev_off(uintptr_t npe_base){
	rumboot_printf("Wait NA_VPE dev off got\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 12) & 1) !=1) {}
    iowrite32( (1<<12),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 
	rumboot_printf("Done VPE dev off\n");
}
 
void nu_na_wait_int(uintptr_t npe_base){
	rumboot_printf("Wait NA_context got\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_STATUS)) & 1) !=1) {}	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_STATUS)>> 1) & 1) !=1) {}	
    iowrite32( (1<<0)|(1<<1),npe_base + NA_CU_REGS_BASE + NA_INT_RESET);
	
	rumboot_printf("Done NA context\n");		
}

void nu_na_ppe_wait_int_dev_off(uintptr_t npe_base){
	rumboot_printf("Wait PPE dev off got\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 22) & 1) !=1) {}
    iowrite32( (1<<22),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 
	rumboot_printf("Done PPE dev off\n");
}

void nu_vpe_pause_next_cntx(uintptr_t vpe_base){
  uint32_t temp;
    rumboot_printf("Stop VPE begin...\n");
	while(( (ioread32(vpe_base +  NU_VPE  + NU_VPE_INT_STATUS) >> 0) & 1) !=1) {}
	iowrite32((0x00000001),vpe_base + NU_VPE + NU_VPE_INT_RESET);
	while(( (ioread32(vpe_base +  NU_VPE  + NU_VPE_INT_STATUS) >> 6) & 1) !=1) {}
	iowrite32((1<<6),vpe_base + NU_VPE + NU_VPE_INT_RESET);
	
	while(( (ioread32(vpe_base +  NU_VPE  + NU_VPE_INT_STATUS) >> 4) & 1) !=1) {}
	iowrite32((1<<4),vpe_base + NU_VPE + NU_VPE_INT_RESET);	
	
	//rumboot_printf("Stop check...\n");
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 8) & 1) !=1) {}
	temp = ioread32(vpe_base + NU_VPE + NU_VPE_NEXT_CNTX);
	iowrite32(temp | (1<<16),vpe_base + NU_VPE + NU_VPE_NEXT_CNTX);
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 5) & 1) !=1) {}
	iowrite32((1<<5),vpe_base + NU_VPE + NU_VPE_INT_RESET);
	rumboot_printf("Done VPE stop\n");
	iowrite32((0x00000100),vpe_base + NU_VPE + NU_VPE_INT_RESET);
}

void nu_na_vpe_pause(uintptr_t npe_base ){
  uint32_t temp;
    rumboot_printf("Stop NA_VPE  begin...\n");	
	temp = ioread32(npe_base + NA_CU_REGS_BASE + NA_PAUSE);
	iowrite32(temp | (1<<1),npe_base + NA_CU_REGS_BASE + NA_PAUSE); //vpe_pause
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_PAUSE) >> 17) & 1) !=1) {}	
	rumboot_printf("Done NA_VPE stop\n");
	iowrite32((0x00002800),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
}

void nu_vpe_dev_resume(uintptr_t vpe_base){
    rumboot_printf(" VPE finished cube handle ...\n");
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 4) & 1) !=1) {}	
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 6) & 1) !=1) {}	
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 0) & 1) !=1) {}
	iowrite32(((1<<0) | (1<<4) | (1<<6)),vpe_base + NU_VPE + NU_VPE_INT_RESET);
	rumboot_printf("Done VPE end \n");
}

void nu_na_vpe_dev_resume(uintptr_t npe_base){
    rumboot_printf(" NA_VPE finished cube handle ...\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 11) & 1) !=1) {}
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 19) & 1) !=1) {}	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 12) & 1) !=1) {}	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 17) & 1) !=1) {}	
	iowrite32(((1<<11) | (1<<12) | (1<<17)  | (1<<19)),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_VPE end \n");
}
void nu_vpe_dev_pause_resume(uintptr_t vpe_base){
    rumboot_printf("Start after stop VPE  begin...\n");
	iowrite32( (0<<16),vpe_base + NU_VPE + NU_VPE_NEXT_CNTX);
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 7) & 1) !=1) {}
	rumboot_printf("Done VPE start after stop\n");
}
void nu_na_vpe_dev_pause_resume(uintptr_t npe_base){
    rumboot_printf("Start after stop NA_VPE begin...\n");
	iowrite32( (0<<1),npe_base + NA_CU_REGS_BASE  + NA_PAUSE);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 19) & 1) !=1) {}
	iowrite32((1<<19) ,npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 
	rumboot_printf("Done VPE start after stop\n");
}
void nu_vpe_dev_pause_norst_resume(uintptr_t vpe_base){
    rumboot_printf("Start after stop VPE  begin...\n");
	iowrite32((0<<16),vpe_base + NU_VPE + NU_VPE_NEXT_CNTX);
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 7) & 1) !=1) {}
	iowrite32((1<<7) ,vpe_base + NU_VPE + NU_VPE_INT_RESET); 
	rumboot_printf("Done VPE start after stop\n");
}
void nu_vpe_wait_int_pause_norst_cntx_appl(uintptr_t vpe_base){
	rumboot_printf("Wait VPE context \n");
    while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 4) & 1) !=1) {}
    iowrite32((1<<4),vpe_base + NU_VPE + NU_VPE_INT_RESET); 
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 8) & 1) !=1) {}
    iowrite32(((1<<8) ),vpe_base + NU_VPE + NU_VPE_INT_RESET); 
	rumboot_printf("Done VPE context\n");
}
void nu_vpe_soft_reset(uintptr_t vpe_base){
    rumboot_printf("Soft reset VPE ...\n");	
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_NEXT_CNTX) >> 20) & 1) !=1) {}
	iowrite32( (1<<0),vpe_base + NU_VPE + NU_VPE_SOFT_RESET);
	rumboot_printf("Soft reset done ...\n");	
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 16) & 1) !=1) {}
	rumboot_printf("Done VPE soft reset passed\n");	
}

void nu_na_vpe_wait_complete(uintptr_t npe_base){
	rumboot_printf("Wait VPE WDMA...\n");
	while (( (ioread32(npe_base + NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS)>> 17) & 1) !=1) {}
	{iowrite32( (1<<17),npe_base + NA_CU_REGS_BASE   + NA_INT_UNITS_RESET);
	rumboot_printf("Done VPE WDMA...\n");}
}

void nu_na_vpe_wait_marked_cube_complete(uintptr_t npe_base){
	rumboot_printf("Wait marked cube complete...\n");
	while (( (ioread32(npe_base + NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS)>> 18) & 1) !=1) {}
	{iowrite32(0x00040000,npe_base + NA_CU_REGS_BASE   + NA_INT_UNITS_RESET);
	rumboot_printf("Done VPE marked cube complete...\n");}
}
void nu_na_ppe_wait_marked_cube_complete(uintptr_t npe_base){
	rumboot_printf("Wait marked cube complete...\n");
	while (( (ioread32(npe_base + NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS)>> 29) & 1) !=1) {}
	{iowrite32( (1<<29),npe_base + NA_CU_REGS_BASE   + NA_INT_UNITS_RESET);
	rumboot_printf("Done PPE marked cube complete...\n");}
}
void nu_na_mpe_wait_int_dev_off(uintptr_t npe_base){
	rumboot_printf("Wait NA_MPE dev off\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 1) & 1) !=1) {}
    iowrite32( (1<<1),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 
	rumboot_printf("Done NA_MPE dev off\n");
}

void nu_na_mpe_wait_complete(uintptr_t npe_base){
  	 rumboot_printf("Wait MPE WDMA...\n");
  	 while (( (ioread32(npe_base + NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS)>> 7) & 1) !=1) {}
  	 {iowrite32( (1<<7),npe_base + NA_CU_REGS_BASE   + NA_INT_UNITS_RESET);
	 rumboot_printf("Done MPE WDMA...\n");}
}
void nu_npe_mpe_set_int_mask(uintptr_t npe_base){
	if (( (ioread32(npe_base  + NA_CU_REGS_BASE + NA_UNITS_MODE) >> 0) & 1) ==0 )
	iowrite32((1<<0) | (1<<1)| (1<<2) | (1<<3) |(1<<4) | (1<<5) | (1<<6) | (1<<7) | (1<<8)	|(1<<9)	|(1<<10)
		| (1<<11) | (1<<12)| (1<<13) |(1<<14) | (1<<15) | (1<<16) | (1<<17) | (1<<18)	|(1<<19), //????
	npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK);  //IRQ_DEV_ON  IRQ_CUBE_CMPL IRQ_DEV_OFF 
	rumboot_printf("nu_npe_mpe_set_int_mask: Writing [0x%x]=0x%x\n",npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK,ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK));
}
void nu_na_mpe_soft_reset(uintptr_t npe_base){
    rumboot_printf("Soft reset NA_MPE ...\n");	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_PAUSE) >> 16) & 1) !=1) {}
	iowrite32( (1<<0),npe_base + NA_CU_REGS_BASE + NA_MPE_SOFT_RESET);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 4) & 1) !=1) {} //IRQ_MPE_RESET_COMPL
	rumboot_printf("Done NA_MPE soft reset\n");	 
}
void nu_na_mpe_pause(uintptr_t npe_base){
  uint32_t temp;
    rumboot_printf("Stop NA_MPE  begin...\n");	
	temp = ioread32(npe_base + NA_CU_REGS_BASE + NA_PAUSE);
	iowrite32(temp | (1<<0),npe_base + NA_CU_REGS_BASE + NA_PAUSE); //mpe_pause
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_PAUSE) >> 16) & 1) !=1) {}	
	rumboot_printf("Done NA_MPE stop\n");
	iowrite32((0x00000405),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
}

void nu_na_mpe_dev_pause_resume(uintptr_t npe_base){
    rumboot_printf("Start after stop NA_MPE  begin...\n");
	iowrite32( (0<<0),npe_base + NA_CU_REGS_BASE  + NA_PAUSE);
	rumboot_printf("Done MPE start after stop\n");
}
void nu_na_mpe_dev_pause_norst_resume(uintptr_t npe_base){
    rumboot_printf("Start after stop NA_MPE  begin...\n");
	iowrite32((0<<2),npe_base + NA_CU_REGS_BASE + NA_PAUSE);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE+ NA_INT_UNITS_STATUS) >> 8) & 1) !=1) {}
	iowrite32((1<<8) ,npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE+ NA_INT_UNITS_STATUS) >> 9) & 1) !=1) {}
	iowrite32((1<<9) ,npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 
	rumboot_printf("Done NA_MPE start after stop\n");
	
    while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 3) & 1) !=1) {} // IRQ_PPE_WDMA_CMPL 
	iowrite32((1<<3),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 	
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 14)& 1) !=1) {} // IRQ_PPE_RDMA_CMPL
	rumboot_printf("Wait NA_MPE context \n");   
	iowrite32((1<<14),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 	
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 17) & 1) !=1) {} // IRQ_PPE_RDMA_CMPL
 	rumboot_printf("NA_MPE context got\n");   
	iowrite32((1<<17),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);  
			
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 7) & 1) !=1) {} // IRQ_PPE_RDMA_CMPL 
	iowrite32((1<<7 ),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);	
 
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 6) & 1) !=1) {} // IRQ_PPE_RDMA_CMPL
 	rumboot_printf("Wait NA_MPE context \n");   
	iowrite32((1<<6 ),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);	
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 1 & 1) !=1)	){} // IRQ_PPE_RDMA_CMPL
 	rumboot_printf("NA_MPE context got\n");   
	iowrite32((1<<1 ),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);

	rumboot_printf("Done NA_MPE context\n");

}
void nu_na_mpe_wait_int_pause(uintptr_t npe_base){
	rumboot_printf("Wait NA_MPE context\n");
    while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 1) & 1) !=1) {}
	while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 3 ) & 1) !=1) {}   
	while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 4 ) & 1) !=1) {}
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 14) & 1) !=1) {} //MPE+VPE
	iowrite32(((1<<0) | (1<<1) |  (1<<2) | (1<<3) |  (1<<4)  |  (1<<10)  |  (1<<11) |  (1<<14) ),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_MPE context\n");
}

void nu_na_mpe_dev_resume(uintptr_t npe_base){
    rumboot_printf(" NA_MPE finished cube handle ...\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 0) & 1) !=1) {}
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 10) & 1) !=1) {}	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 9) & 1) !=1) {}
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 8) & 1) !=1) {}
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 1) & 1) !=1) {}	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 7) & 1) !=1) {}
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 6) & 1) !=1) {}
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 19) & 1) !=1) {} //MPE+VPE
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 17) & 1) !=1) {} //MPE+VPE

	iowrite32(((1<<0) | (1<<1) | (1<<6)  | (1<<7)  | (1<<8)  | (1<<9)  | (1<<10) | (1<<11) |(1<<17)| (1<<19)  ),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_MPE_VPE end \n");
}	
	
void nu_na_ppe_soft_reset(uintptr_t npe_base){
    rumboot_printf("Soft reset NA_PPE ...\n");	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_PAUSE) >> 18) & 1) !=1) {}
	iowrite32( (1<<0),npe_base + NA_CU_REGS_BASE + NA_PPE_SOFT_RESET);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 25) & 1) !=1) {} //IRQ_PPE_RESET_COMPL
	iowrite32((1<<25) ,npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_PPE soft reset passed\n");	 
}
void nu_na_ppe_pause(uintptr_t npe_base ){
  uint32_t temp;
    rumboot_printf("Stop NA_PPE  begin...\n");	
	temp = ioread32(npe_base + NA_CU_REGS_BASE + NA_PAUSE);
	iowrite32(temp | (1<<2),npe_base + NA_CU_REGS_BASE + NA_PAUSE); //ppe_pause
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_PAUSE) >> 18) & 1) !=1) {}	
	rumboot_printf("Done NA_PPE stop\n");
	
	while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 31 ) & 1) !=1) {}
	iowrite32( (1<<31),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);

	while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 21 ) & 1) !=1) {}
	iowrite32( (1<<21),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);

	while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 23 ) & 1) !=1) {}
	iowrite32( (1<<23),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);

}

void nu_na_ppe_pause_set(uintptr_t base) {
  // 2 PPE_PAUSE

  iowrite32(ioread32(base+NA_PAUSE) | 0x1<<2, base+NA_PAUSE);
}

uint32_t nu_na_ppe_pause_status(uintptr_t base) {
  // 18 PPE_PAUSE_STAT

  return (ioread32(base + NA_PAUSE) >> 18)&0x1;
}

void nu_na_ppe_pause_clr(uintptr_t base) {
  // 2 PPE_PAUSE

  iowrite32(ioread32(base+NA_PAUSE) & ~mask_N_M(2,2), base+NA_PAUSE);
}

void nu_na_int_units_ppe_mask (uintptr_t base) {
  // 25 IRQ_PPE_RESET_CMPL

  iowrite32(ioread32(base+NA_INT_UNITS_MASK) | 0x1<<25, base+NA_INT_UNITS_MASK);
}

void nu_na_ppe_soft_reset_set (uintptr_t base) {
  // 0 PPE_SOFT_RESET

  iowrite32(ioread32(base+NA_PPE_SOFT_RESET) | 0x1<<0, base+NA_PPE_SOFT_RESET);  
}

uint32_t nu_na_ppe_soft_reset_status (uintptr_t base) {
  // 0 PPE_SOFT_RESET

  return ioread32(base+NA_PPE_SOFT_RESET) & 0x1;  
}

void nu_na_vpe_soft_reset(uintptr_t npe_base){
    rumboot_printf("Soft reset NA_VPE ...\n");	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_PAUSE) >> 17) & 1) !=1) {}
	iowrite32( (1<<0),npe_base + NA_CU_REGS_BASE + NA_VPE_SOFT_RESET);	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 15) & 1) !=1) {} //IRQ_VPE_RESET_COMPL
	iowrite32((1<<15) ,npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_VPE soft reset\n");
}

uint32_t nu_vpe_busy(uintptr_t vpe_base) {
	return ioread32(vpe_base + NU_VPE + NU_VPE_NEXT_CNTX) & (1<<12); // DEV_BUSY
}

void nu_vpe_config_wr_main_channel(uintptr_t dma_base, void *addr, int size){
  nu_cpdmac_rcv256_config(dma_base,addr,size);
}

void nu_vpe_run_wr_main_channel(uintptr_t dma_base) {
  nu_cpdmac_rcv256_run(dma_base);
}
void nu_vpe_wait_wr_main_channel_complete(uintptr_t dma_base) {
  nu_cpdmac_rcv256_wait_complete(dma_base);
}

// Type conversion for VPE
bool nu_vpe_mode_to_bool (Mode in_mode){
  bool res;
  if (in_mode == Mode_Unitary) res = 0;
  else                         res = 1;
  return res;
}

void nu_ppe_set_wdma_int_mask(uintptr_t ppe_base){
    if (( (ioread32(ppe_base + NU_PPE_WDMA_INT_STATUS) >> 12) & 1) ==0 )
    iowrite32((1<<12) | (1<<8)| (1<<4) | (1<<5) |(1<<6) | (1<<0) | (1<<1),ppe_base + NU_VPE + NU_VPE_INT_MASK);  //IRQ_DEV_ON  IRQ_CUBE_CMPL IRQ_DEV_OFF 
	rumboot_printf("nu_ppe_set_wdma_int_mask: Writing [0x%x]=0x%x\n",ppe_base + NU_VPE + NU_VPE_INT_MASK,ioread32(ppe_base  + NU_PPE_WDMA_INT_MASK));
}
void nu_npe_ppe_set_int_mask(uintptr_t npe_base){
	if (( (ioread32(npe_base + NA_CU_REGS_BASE + NA_UNITS_MODE) >> 1) & 1) ==0 )
    iowrite32((1<<31) | (1<<30)| (1<<29) | (1<<28) |(1<<27) | (1<<26) | (1<<25) | (1<<24) | (1<<23) | (1<<22) |  (1<<21),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK);  //IRQ_DEV_ON  IRQ_CUBE_CMPL IRQ_DEV_OFF 
	rumboot_printf("nu_npe_ppe_set_int_mask: Writing [0x%x]=0x%x\n",npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK,ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK));
}



void nu_na_ppe_dev_pause_norst_resume(uintptr_t npe_base){
    rumboot_printf("Start after stop PPE  begin...\n");
	iowrite32((0<<2),npe_base + NA_CU_REGS_BASE + NA_PAUSE);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE+ NA_INT_UNITS_STATUS) >> 30) & 1) !=1) {}
	iowrite32((1<<30) ,npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE+ NA_INT_UNITS_STATUS) >> 24) & 1) !=1) {}
	iowrite32((1<<24) ,npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 
	rumboot_printf("Done NA_PPE start after stop\n");
	
    while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 28) & 1) !=1) {} // IRQ_PPE_WDMA_CMPL 
	iowrite32((1<<28),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 	
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 27) & 1) !=1) {} // IRQ_PPE_RDMA_CMPL
 	rumboot_printf("Wait NA_PPE context \n");   
	iowrite32(((1<<27) ),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);	
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 22) & 1) !=1) {} // IRQ_PPE_RDMA_CMPL
 	rumboot_printf("NA_PPE context got\n");   
	iowrite32(((1<<22) ),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);	    
	
	rumboot_printf("Done NA_PPE context\n");


}

void nu_na_ppe_dev_pause_resume(uintptr_t npe_base){
    rumboot_printf("Start after stop NA_PPE  begin...\n");
	iowrite32( (0<<2),npe_base + NA_CU_REGS_BASE  + NA_PAUSE);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 24) & 1) !=1) {}
	iowrite32( (0<<24),npe_base + NA_CU_REGS_BASE  + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_PPE start after stop after soft reset\n");
}
void nu_na_vpe_wait_int_pause(uintptr_t npe_base){
	rumboot_printf("Wait_ NA_VPE context \n");
    while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 23) & 1) !=1) {}
	while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 24 ) & 1) !=1) {}
    while(( (ioread32(npe_base +  NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 25 ) & 1) !=1) {}
    iowrite32(((1<<11) | (1<<12) |  (1<<13) | (1<<14) |  (1<<15) |  (1<<19) ),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_VPE context\n");
}
void nu_na_vpe_wait(uintptr_t npe_base){

    rumboot_printf("Wait NA_VPE complete...\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE +  NA_INT_UNITS_STATUS) >> 12) & 1) !=1) {}
	iowrite32((1<<12),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf(" NA_VPE wait is completed\n");
}
void nu_na_vpe_dev_pause_norst_resume(uintptr_t npe_base){
    rumboot_printf("Start after stop NA_VPE	begin...\n");
	iowrite32((0<<1),npe_base + NA_CU_REGS_BASE + NA_PAUSE);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE+ NA_INT_UNITS_STATUS) >> 19) & 1) !=1) {}
	iowrite32((1<<19) ,npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE+ NA_INT_UNITS_STATUS) >>14) & 1) !=1) {}
	iowrite32((1<<14) ,npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 
	rumboot_printf("Done NA_VPE start after stop\n");
	
    while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 17) & 1) !=1) {} // IRQ_PPE_WDMA_CMPL 
	iowrite32((1<<17),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 	
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 12) & 1) !=1) {} // IRQ_PPE_RDMA_CMPL
 	rumboot_printf("NA_PPE context got\n");   
	iowrite32(((1<<12) ),npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);	    	
	rumboot_printf("Done NA_VPE context\n");

}

void nu_na_soft_reset(uintptr_t npe_base){
    rumboot_printf("NA soft reset ...\n");	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_PAUSE) >> 20) & 1) !=1) {}
	iowrite32( (1<<0),npe_base + NA_CU_REGS_BASE + NA_SOFT_RESET);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_STATUS) >> 4) & 1) !=1) {} //IRQ_PPE_RESET_COMPL
	rumboot_printf("NA soft soft ...\n");	
	iowrite32((1<<4) ,npe_base + NA_CU_REGS_BASE + NA_INT_RESET);
	rumboot_printf("Done NA soft reset passed\n");	 
}

void na_cu_set_units_direct_mode(uintptr_t base, uint32_t mask) {
  uint32_t temp;
  temp = ioread32(base + NA_UNITS_MODE);
  temp = temp | mask;
  rumboot_printf("na_cu_set_units_direct_mode: Writing [0x%x]=0x%x\n",base + NA_UNITS_MODE,temp);
  iowrite32(temp,base + NA_UNITS_MODE);
}

void na_cu_timer_reset_and_start(uintptr_t base, bool timer_vpe, bool timer_ppe) {
  iowrite32(1<<1,base + NA_TIMER_CTRL);
  char *dev;
  if (timer_vpe) dev = "VPE WDMA";
  if (timer_ppe) dev = "PPE WDMA";
  if (timer_vpe & timer_ppe) dev = "VPE WDMA or PPE WDMA";
  rumboot_printf("NA_CU_TIMER was reset and will enabled");
  if (timer_vpe | timer_ppe)
    rumboot_printf(" while %s won't stop it", dev);
  rumboot_printf("\n");
  uint32_t temp;
  temp = (timer_ppe<<3) | (timer_vpe<<2) | (1<<0);
  iowrite32(temp,base + NA_TIMER_CTRL);
}

uint64_t na_cu_timer_read(uintptr_t base) {
   return 0x0000000100000000*ioread32(base + NA_TIMER_H) + ioread32(base + NA_TIMER_L);
}

NPEReg* nu_mpe_add_diff_reg_map(NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr) {
  rumboot_printf("MPE diff reg map..\n");
  // MPE_RDMA_D
  cfg_diff_ptr = nu_mpe_dma_add_diff_reg_map(NA_MPE_BASE, MPE_RDMA_D_BASE, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  // MPE_RDMA_W
  cfg_diff_ptr = nu_mpe_dma_add_diff_reg_map(NA_MPE_BASE, MPE_RDMA_W_BASE, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  // MPE_MA
  cfg_diff_ptr = nu_mpe_ma_add_diff_reg_map(NA_MPE_BASE, MPE_MA_BASE, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  return cfg_diff_ptr;
}

NPEReg* nu_mpe_add_diff_start(NPEReg* cfg_diff_ptr, ConfigMPE* cfg) {
    cfg_diff_ptr = nu_add_diff_start(cfg_diff_ptr, NA_MPE_BASE + MPE_RDMA_D_BASE + DMA_START, cfg->dma_d_config.depend_mask);
    cfg_diff_ptr = nu_add_diff_start(cfg_diff_ptr, NA_MPE_BASE + MPE_RDMA_W_BASE + DMA_START, cfg->dma_w_config.depend_mask);
    cfg_diff_ptr = nu_add_diff_start(cfg_diff_ptr, NA_MPE_BASE + MPE_MA_BASE + MPE_CMD_ICMW, 0);
    return cfg_diff_ptr;
}

NPEReg* nu_add_diff_start(NPEReg* cfg_diff_ptr, uintptr_t start, uint32_t depend_start) {
  cfg_diff_ptr->address = start;
  cfg_diff_ptr->value   = depend_start | (1<<0);
  cfg_diff_ptr++;
  return cfg_diff_ptr;
}

NPEReg* nu_mpe_ma_add_diff_reg_map(uintptr_t base, uintptr_t mpe_ma_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr) {
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_ma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, MPE_COMMON_WA,  MPE_COMMON_D_BIAS);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_ma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, MPE_ADD_CountM, MPE_NULL);
  return cfg_diff_ptr;
}

NPEReg* nu_mpe_dma_add_diff_reg_map(uintptr_t base, uintptr_t mpe_dma_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr) {
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, MAINCNT_Sha        , MAINCNT_Sha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, X_Cfg_MSha         , PLC_ThreSha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, VLC_CntSha_MSha    , VLC_ThreSha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, WR_Bias1CntSha_MSha, WR_Bias1CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, WR_Bias2CntSha_MSha, WR_Bias2CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, WR_Bias3CntSha_MSha, WR_Bias3CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, WR_Bias4CntSha_MSha, WR_Bias4CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, WR_Bias5CntSha_MSha, WR_Bias5CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, WR_Bias6CntSha_MSha, WR_Bias6CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, BIASCtrl_MSha      , AOffset_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, LPXOffset_MSha     , RPXOffset_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, TPadYOffset_MSha   , BPadYOffset_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, CntSha_MSha        , CntThresholdSha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, Bias1Sha_MSha      , Bias1Sha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, RD_Bias1CntSha_MSha, RD_Bias1CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, Bias2Sha_MSha      , Bias2Sha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, RD_Bias2CntSha_MSha, RD_Bias2CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, Bias3Sha_MSha      , Bias3Sha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, RD_Bias3CntSha_MSha, RD_Bias3CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, Bias4Sha_MSha      , Bias4Sha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, RD_Bias4CntSha_MSha, RD_Bias4CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, Bias5Sha_MSha      , Bias5Sha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, RD_Bias5CntSha_MSha, RD_Bias5CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, Bias6Sha_MSha      , Bias6Sha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, Bias6CntSha_MSha   , Bias6CntCmp_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, Bias7Sha_MSha      , Bias7Sha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, Bias7CntSha_MSha   , Bias7CntSha_MSha);
  cfg_diff_ptr = nu_add_diff_reg_map(base, mpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, Bias7BCnt          , BPYDR_MSha);
  return cfg_diff_ptr;
}

NPEReg* nu_vpe_add_diff_reg_map(NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr, Enable lut_en) {
  rumboot_printf("VPE diff reg map..\n");
  cfg_diff_ptr = nu_add_diff_reg_map(NA_VPE_BASE, NU_VPE, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_VPE + NU_VPE_CUBE_SIZE         , NU_VPE + NU_VPE_CUBE_SIZE);
  cfg_diff_ptr = nu_add_diff_reg_map(NA_VPE_BASE, NU_VPE, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_VPE + NU_VPE_CUBE_SIZE_C       , NU_VPE + NU_VPE_OP_MODE);
  cfg_diff_ptr = nu_add_diff_reg_map(NA_VPE_BASE, NU_VPE, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL, NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL);

  cfg_diff_ptr = nu_vpe_op01_add_diff_reg_map(NA_VPE_BASE, NU_VPE                                  , cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  cfg_diff_ptr = nu_vpe_op01_add_diff_reg_map(NA_VPE_BASE, NU_VPE + (NU_VPE_OP1_CFG-NU_VPE_OP0_CFG), cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  cfg_diff_ptr = nu_vpe_op2_add_diff_reg_map( NA_VPE_BASE, NU_VPE                                  , cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, lut_en);
  cfg_diff_ptr = nu_vpe_dma_add_diff_reg_map( NA_VPE_BASE, NU_VPE_DST_WDMA                         , cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  cfg_diff_ptr = nu_vpe_dma_add_diff_reg_map( NA_VPE_BASE, NU_VPE_SRC_RDMA                         , cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  cfg_diff_ptr = nu_vpe_dma_add_diff_reg_map( NA_VPE_BASE, NU_VPE_OP0_RDMA                         , cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  cfg_diff_ptr = nu_vpe_dma_add_diff_reg_map( NA_VPE_BASE, NU_VPE_OP1_RDMA                         , cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  cfg_diff_ptr = nu_vpe_dma_add_diff_reg_map( NA_VPE_BASE, NU_VPE_OP2_RDMA                         , cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  return cfg_diff_ptr;
}

NPEReg* nu_vpe_op01_add_diff_reg_map(uintptr_t base, uintptr_t vpe_op01_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr) {
  cfg_diff_ptr = nu_add_diff_reg_map(base, vpe_op01_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_VPE_OP0_CFG, NU_VPE_OP0_NORM_PARAM);
  return cfg_diff_ptr;
}

NPEReg* nu_vpe_op2_add_diff_reg_map(uintptr_t base, uintptr_t vpe_op2_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr, Enable lut_en) {
  cfg_diff_ptr = nu_add_diff_reg_map(base, vpe_op2_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_VPE_OP2_CFG    , NU_VPE_OP2_ALU_CVT_TRUNC_VAL);
  cfg_diff_ptr = nu_add_diff_reg_map(base, vpe_op2_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_VPE_OP2_MUL_CFG, NU_VPE_OP2_NORM_PARAM);

  if(lut_en) {
    cfg_diff_ptr = nu_vpe_lut_add_diff_reg_map(base, vpe_op2_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr);
  }
  return cfg_diff_ptr;
}

NPEReg* nu_vpe_lut_add_diff_reg_map(uintptr_t base, uintptr_t vpe_op2_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr) {
  cfg_diff_ptr = nu_add_diff_reg_map(base, vpe_op2_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_VPE_LUT_CFG, NU_VPE_LUT_TAB1_SLOPE_SHIFT);
  return cfg_diff_ptr;
}

NPEReg* nu_vpe_dma_add_diff_reg_map(uintptr_t base, uintptr_t vpe_dma_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr) {
  cfg_diff_ptr = nu_add_diff_reg_map(base, vpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_VPE_DMA_CFG          , NU_VPE_DMA_CFG);
  cfg_diff_ptr = nu_add_diff_reg_map(base, vpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_VPE_DMA_AXI_PARAM - 4, NU_VPE_DMA_AXI_PARAM);
  cfg_diff_ptr = nu_add_diff_reg_map(base, vpe_dma_base, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_VPE_DMA_BASE         , NU_VPE_DMA_BATCH_STRIDE);
  return cfg_diff_ptr;
}

NPEReg* nu_vpe_add_diff_start(NPEReg* cfg_diff_ptr, ConfigVPE* cfg) {
  uint32_t temp;

  temp = (temp & 0xE0FFFFFF) | cfg->depend_mask; // Manipulate With DPND Fields

  if(cfg->mark)
   {temp = temp | 0x00000003; }//MARKED_CNTX=1 & NEXT_CNTX=1
  else
    temp = (temp & (~(1<<1)) ) | (1<<0);  //MARKED_CNTX=0 | NEXT_CNTX=1
  cfg_diff_ptr = nu_add_diff_start(cfg_diff_ptr, NA_VPE_BASE + NU_VPE + NU_VPE_NEXT_CNTX, temp);
  return cfg_diff_ptr;
}

NPEReg* nu_ppe_rdma_add_diff_reg_map(NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr) {
  rumboot_printf("PPE RDMA diff reg map..\n");
  cfg_diff_ptr = nu_add_diff_reg_map(NA_PPE_RDMA_BASE, (uintptr_t)NULL, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_PPE_RDMA_BASE_ADDR, NU_PPE_RDMA_BASE_ADDR);
  cfg_diff_ptr = nu_add_diff_reg_map(NA_PPE_RDMA_BASE, (uintptr_t)NULL, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_PPE_RDMA_BORDER_X , NU_PPE_RDMA_BOX_OFFSET_Z);
  return cfg_diff_ptr;
}

NPEReg* nu_ppe_wdma_add_diff_reg_map(NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr) {
  rumboot_printf("PPE WDMA diff reg map..\n");
  cfg_diff_ptr = nu_add_diff_reg_map(NA_PPE_WDMA_BASE, (uintptr_t)NULL, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_PPE_WDMA_BASE_ADDR, NU_PPE_WDMA_BASE_ADDR);
  cfg_diff_ptr = nu_add_diff_reg_map(NA_PPE_WDMA_BASE, (uintptr_t)NULL, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_PPE_WDMA_BORDER_X , NU_PPE_DATA_H_OUT);
  cfg_diff_ptr = nu_add_diff_reg_map(NA_PPE_WDMA_BASE, (uintptr_t)NULL, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_PPE_OP_MODE       , NU_PPE_RECIP_KERNEL_WH);
  cfg_diff_ptr = nu_add_diff_reg_map(NA_PPE_WDMA_BASE, (uintptr_t)NULL, cfg_diff_ptr, curr_cfg_ptr, next_cfg_ptr, NU_PPE_PADDING       , NU_PPE_PADDING_VALUE_7);
  return cfg_diff_ptr;
}

NPEReg* nu_ppe_wdma_add_diff_start(NPEReg* cfg_diff_ptr, ConfigREGPPE* cfg) {
  cfg_diff_ptr = nu_add_diff_start(cfg_diff_ptr, NA_PPE_WDMA_BASE + NU_PPE_OP_ENABLE, cfg->wOpEn);
  return cfg_diff_ptr;
}

NPEReg* nu_add_diff_reg_map(uintptr_t base, uintptr_t device_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr, uint32_t start_shift, uint32_t end_shift) {
  uint32_t curr_reg_value;
  uint32_t next_reg_value;

  curr_cfg_ptr = (uint32_t*)((char*)curr_cfg_ptr + device_base + start_shift);
  next_cfg_ptr = (uint32_t*)((char*)next_cfg_ptr + device_base + start_shift);
  for (uint32_t curr_shift = start_shift; curr_shift <= end_shift;curr_shift+=0x4) {
    curr_reg_value = *curr_cfg_ptr;
    next_reg_value = *next_cfg_ptr;
    if (curr_reg_value != next_reg_value || (uint32_t)curr_cfg_ptr == device_base + curr_shift) {
      //rumboot_printf("addr %x, value %x\n", device_base + curr_shift, next_reg_value);
      cfg_diff_ptr->address = base + device_base + curr_shift;
      cfg_diff_ptr->value   = next_reg_value;
      cfg_diff_ptr++;
    }
    curr_cfg_ptr++;
    next_cfg_ptr++;
  }
  return cfg_diff_ptr;
}

void nu_print_associative_regs_dump(NPEReg* start_ptr, NPEReg* end_ptr) {
#ifndef NU_NO_PRINT
  rumboot_printf("Print associative regs dump\n");
  rumboot_printf("  address = %x\n", start_ptr);
  rumboot_printf("  size= %x\n", (uint32_t)(end_ptr - start_ptr));

  for (NPEReg* curr_ptr = start_ptr; curr_ptr < end_ptr; curr_ptr++) {
    rumboot_printf("    Reg: address = %x, value = %x\n", curr_ptr->address, curr_ptr->value);
  }
#endif // NU_NO_PRINT
}

void nu_mpe_print_reg_map(uint32_t* mpe_reg_map_base) {
#ifndef NU_NO_PRINT
  // MPE_RDMA_D
  rumboot_printf("MPE RDMA_D print reg memory map..\n");
  nu_mpe_dma_print_reg_map((uint32_t*)((char*)mpe_reg_map_base + MPE_RDMA_D_BASE));
  // MPE_RDMA_W
  rumboot_printf("MPE RDMA_W print reg memory map..\n");
  nu_mpe_dma_print_reg_map((uint32_t*)((char*)mpe_reg_map_base + MPE_RDMA_W_BASE));
  // MPE_MA
  rumboot_printf("MPE MPE_MA print reg memory map..\n");
  nu_mpe_ma_print_reg_map((uint32_t*)((char*)mpe_reg_map_base + MPE_MA_BASE));
#endif // NU_NO_PRINT
}

void nu_mpe_dma_print_reg_map(uint32_t* mpe_dma_reg_map_base) {
  print_part_of_memory(mpe_dma_reg_map_base, MAINCNT_Sha,         MAINCNT_Sha);
  print_part_of_memory(mpe_dma_reg_map_base, X_Cfg_MSha,          PLC_ThreSha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, VLC_CntSha_MSha,     VLC_ThreSha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, WR_Bias1CntSha_MSha, WR_Bias1CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, WR_Bias2CntSha_MSha, WR_Bias2CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, WR_Bias3CntSha_MSha, WR_Bias3CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, WR_Bias4CntSha_MSha, WR_Bias4CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, WR_Bias5CntSha_MSha, WR_Bias5CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, WR_Bias6CntSha_MSha, WR_Bias6CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, BIASCtrl_MSha,       AOffset_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, LPXOffset_MSha,      RPXOffset_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, TPadYOffset_MSha,    BPadYOffset_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, CntSha_MSha,         CntThresholdSha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, Bias1Sha_MSha,       Bias1Sha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, RD_Bias1CntSha_MSha, RD_Bias1CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, Bias2Sha_MSha,       Bias2Sha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, RD_Bias2CntSha_MSha, RD_Bias2CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, Bias3Sha_MSha,       Bias3Sha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, RD_Bias3CntSha_MSha, RD_Bias3CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, Bias4Sha_MSha,       Bias4Sha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, RD_Bias4CntSha_MSha, RD_Bias4CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, Bias5Sha_MSha,       Bias5Sha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, RD_Bias5CntSha_MSha, RD_Bias5CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, Bias6Sha_MSha,       Bias6Sha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, Bias6CntSha_MSha,    Bias6CntCmp_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, Bias7Sha_MSha,       Bias7Sha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, Bias7CntSha_MSha,    Bias7CntSha_MSha);
  print_part_of_memory(mpe_dma_reg_map_base, Bias7BCnt,           BPYDR_MSha);
}

void nu_mpe_ma_print_reg_map(uint32_t* mpe_ma_reg_map_base) {
  print_part_of_memory(mpe_ma_reg_map_base, MPE_COMMON_WA,  MPE_COMMON_D_BIAS);
  print_part_of_memory(mpe_ma_reg_map_base, MPE_ADD_CountM, MPE_NULL);
}

void nu_vpe_print_reg_map(uint32_t* vpe_reg_map_base, Enable lut_en) {
#ifndef NU_NO_PRINT
  rumboot_printf("VPE print reg memory map..\n");
  print_part_of_memory(vpe_reg_map_base, NU_VPE + NU_VPE_CUBE_SIZE         , NU_VPE + NU_VPE_CUBE_SIZE);
  print_part_of_memory(vpe_reg_map_base, NU_VPE + NU_VPE_CUBE_SIZE_C       , NU_VPE + NU_VPE_OP_MODE);
  print_part_of_memory(vpe_reg_map_base, NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL, NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL);

  rumboot_printf("VPE op0 print reg memory map..\n");
  nu_vpe_op01_print_reg_map((uint32_t*)((char*)vpe_reg_map_base + NU_VPE));

  rumboot_printf("VPE op1 print reg memory map..\n");
  nu_vpe_op01_print_reg_map((uint32_t*)((char*)vpe_reg_map_base + NU_VPE + (NU_VPE_OP1_CFG-NU_VPE_OP0_CFG)));

  rumboot_printf("VPE op2 print reg memory map..\n");
  nu_vpe_op2_print_reg_map( (uint32_t*)((char*)vpe_reg_map_base + NU_VPE), lut_en);

  rumboot_printf("VPE DST_WDMA print reg memory map..\n");
  nu_vpe_dma_print_reg_map( (uint32_t*)((char*)vpe_reg_map_base + NU_VPE_DST_WDMA));

  rumboot_printf("VPE SRC_RDMA print reg memory map..\n");
  nu_vpe_dma_print_reg_map( (uint32_t*)((char*)vpe_reg_map_base + NU_VPE_SRC_RDMA));

  rumboot_printf("VPE OP0_RDMA print reg memory map..\n");
  nu_vpe_dma_print_reg_map( (uint32_t*)((char*)vpe_reg_map_base + NU_VPE_OP0_RDMA));

  rumboot_printf("VPE OP1_RDMA print reg memory map..\n");
  nu_vpe_dma_print_reg_map( (uint32_t*)((char*)vpe_reg_map_base + NU_VPE_OP1_RDMA));

  rumboot_printf("VPE OP2_RDMA print reg memory map..\n");
  nu_vpe_dma_print_reg_map( (uint32_t*)((char*)vpe_reg_map_base + NU_VPE_OP2_RDMA));
#endif // NU_NO_PRINT
}

void nu_vpe_op01_print_reg_map(uint32_t* vpe_op01_reg_map_base) {
  print_part_of_memory(vpe_op01_reg_map_base, NU_VPE_OP0_CFG, NU_VPE_OP0_NORM_PARAM);
}

void nu_vpe_op2_print_reg_map(uint32_t* vpe_op2_reg_map_base, Enable lut_en) {
  print_part_of_memory(vpe_op2_reg_map_base, NU_VPE_OP2_CFG    , NU_VPE_OP2_ALU_CVT_TRUNC_VAL);
  print_part_of_memory(vpe_op2_reg_map_base, NU_VPE_OP2_MUL_CFG, NU_VPE_OP2_NORM_PARAM);

  if(lut_en) {
    rumboot_printf("VPE LUT print reg memory map..\n");
    nu_vpe_lut_print_reg_map(vpe_op2_reg_map_base);
  }
}

void nu_vpe_lut_print_reg_map(uint32_t* vpe_lut_reg_map_base) {
  print_part_of_memory(vpe_lut_reg_map_base, NU_VPE_LUT_CFG, NU_VPE_LUT_TAB1_SLOPE_SHIFT);
}

void nu_vpe_dma_print_reg_map(uint32_t* vpe_dma_reg_map_base) {
  print_part_of_memory(vpe_dma_reg_map_base, NU_VPE_DMA_CFG          , NU_VPE_DMA_CFG);
  print_part_of_memory(vpe_dma_reg_map_base, NU_VPE_DMA_AXI_PARAM - 4, NU_VPE_DMA_AXI_PARAM);
  print_part_of_memory(vpe_dma_reg_map_base, NU_VPE_DMA_BASE         , NU_VPE_DMA_BATCH_STRIDE);
}

void nu_ppe_rdma_print_reg_map(uint32_t* ppe_base) {
#ifndef NU_NO_PRINT
  rumboot_printf("PPE RDMA print reg memory map..\n");
  print_part_of_memory(ppe_base, NU_PPE_RDMA_BASE_ADDR, NU_PPE_RDMA_BASE_ADDR);
  print_part_of_memory(ppe_base, NU_PPE_RDMA_BORDER_X , NU_PPE_RDMA_BOX_OFFSET_Z);
#endif // NU_NO_PRINT
}

void nu_ppe_wdma_print_reg_map(uint32_t* ppe_base) {
#ifndef NU_NO_PRINT
  rumboot_printf("PPE WDMA print reg memory map..\n");
  print_part_of_memory(ppe_base, NU_PPE_WDMA_BASE_ADDR, NU_PPE_WDMA_BASE_ADDR);
  print_part_of_memory(ppe_base, NU_PPE_WDMA_BORDER_X , NU_PPE_DATA_H_OUT);
  print_part_of_memory(ppe_base, NU_PPE_OP_MODE       , NU_PPE_RECIP_KERNEL_WH);
  print_part_of_memory(ppe_base, NU_PPE_PADDING       , NU_PPE_PADDING_VALUE_7);
#endif // NU_NO_PRINT
}

void print_part_of_memory(uint32_t* base, uint32_t start_shift, uint32_t end_shift) {
  uint32_t* start_ptr = (uint32_t*)((char*)base + start_shift);
  uint32_t* end_ptr = (uint32_t*)((char*)base + end_shift);
  for (uint32_t* cur_mem_ptr = start_ptr; cur_mem_ptr <= end_ptr;cur_mem_ptr++) {
    rumboot_printf("  Address = %x, Value = %x\n", (cur_mem_ptr - base)<<2, *cur_mem_ptr);
  }
}

void nu_npe_run(uintptr_t npe_base, NPEReg* cfg_diff_start_ptr, NPEReg* cfg_diff_end_ptr) {
  rumboot_printf("NA run..\n");
  for (NPEReg* cfg_diff_curr_ptr = cfg_diff_start_ptr; cfg_diff_curr_ptr < cfg_diff_end_ptr; cfg_diff_curr_ptr++)
    iowrite32(cfg_diff_curr_ptr->value, npe_base + cfg_diff_curr_ptr->address);
}

void nu_npe_cmd_dma_setup(uintptr_t npe_base, uint32_t cmd_dma_base_addr, uint32_t cmd_dma_page_size) {
    rumboot_printf("NPE cmd dma setup..\n");
    iowrite32((1<<8), npe_base + NA_CU_REGS_BASE + NA_UNITS_MODE);
    iowrite32(cmd_dma_base_addr, npe_base + NA_CU_REGS_BASE + CMD_DMA_BASE_ADDR);
    iowrite32(cmd_dma_page_size, npe_base + NA_CU_REGS_BASE + CMD_DMA_PAGE_SIZE);
    iowrite32(0xF              , npe_base + NA_CU_REGS_BASE + CMD_DMA_AXI_PARAM); // For perfomance
}

void nu_npe_cmd_dma_run(uintptr_t npe_base) {
    rumboot_printf("NPE cmd dma run..\n");
    iowrite32((1<<0), npe_base + NA_CU_REGS_BASE + CMD_DMA_CFG);
}

void nu_npe_cmd_dma_wait_page_complete(uintptr_t npe_base) {
    rumboot_printf("NPE cmd dma wait page complete..\n");
    while ((ioread32(npe_base + NA_CU_REGS_BASE + CMD_DMA_INT_STATUS) & (1<<0)) != 1) {};
    rumboot_printf("NPE cmd dma page complete done\n");
}

void nu_npe_set_int_mask(uintptr_t npe_base){

	if (( (ioread32(npe_base  + NA_CU_REGS_BASE + NA_UNITS_MODE) >> 0) & 1) ==0 )
	iowrite32((1<<0) | (1<<1)| (1<<2) | (1<<3) |(1<<4) | (1<<5) | (1<<6) | (1<<7) | (1<<8)	|(1<<9)	|(1<<10)
		| (1<<11) | (1<<12)| (1<<13) |(1<<14) | (1<<15) | (1<<16) | (1<<17) | (1<<18)	|(1<<19), //????
	npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK);  //IRQ_DEV_ON  IRQ_CUBE_CMPL IRQ_DEV_OFF 
	rumboot_printf("nu_npe_set_int_mask: Writing [0x%x]=0x%x\n",ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK));
}
void na_rst(uintptr_t base){
	
uint32_t busy;
      busy = ((ioread32(base + NA_CU_REGS_BASE + NA_STAT)>> 19) & 1); 
      if (busy  !=0)
      {iowrite32(0xf,(base    +  NA_CU_REGS_BASE +  NA_PAUSE));
	  while (( (ioread32(base +  NA_CU_REGS_BASE +  NA_PAUSE) >> 20) & 1) !=1) {}    
	  while (( (ioread32(base +  NA_CU_REGS_BASE +  NA_INT_STATUS) >> 2) & 1) !=1) {}  //stopped		    
	  iowrite32(0x1,(base 	  +  NA_CU_REGS_BASE +  NA_SOFT_RESET));
	  while (( (ioread32(base +  NA_CU_REGS_BASE +  NA_INT_STATUS) >> 4) & 1) !=1) {} 
	  iowrite32(0x0,(base     +  NA_CU_REGS_BASE +  NA_PAUSE));	
	  while (( (ioread32(base +  NA_CU_REGS_BASE +  NA_INT_STATUS) >> 0) & 1) !=1) {}
	  }
	   else
	  {iowrite32(0x1,(base 	   + NA_CU_REGS_BASE + NA_SOFT_RESET));		
	   while (( (ioread32(base + NA_CU_REGS_BASE + NA_INT_STATUS) >> 4) & 1) !=1) {}
	   rumboot_printf("NA_RESET passed\n");
	  }
      iowrite32(0xFFEFFFFF,base + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
      iowrite32(0x7F      ,base + NA_CU_REGS_BASE + NA_INT_RESET);
 
      iowrite32(0x00000000,base + NA_CU_REGS_BASE + NA_INT_UNITS_MASK);
      iowrite32(0x00000000,base + NA_CU_REGS_BASE + NA_INT_MASK);
	  
	  iowrite32(0x7FFFFFF ,base + NA_CU_REGS_BASE + NA_INT_AXI_RESET);
	  iowrite32(0x00000000,base + NA_CU_REGS_BASE + NA_INT_AXI_MASK);
	  
	  iowrite32(0x10171	  ,base + NA_CU_REGS_BASE + CMD_DMA_INT_RESET);
	  iowrite32(0x00000000,base + NA_CU_REGS_BASE + CMD_DMA_INT_MASK);
	  
	  iowrite32(0x311F1   ,base + NA_VPE_BASE + NU_VPE + NU_VPE_INT_RESET);
	  iowrite32(0x00000000,base + NA_VPE_BASE + NU_VPE + NU_VPE_INT_MASK);
	  
	  iowrite32(0x1F1     ,base + NA_VPE_BASE + NU_VPE_DST_WDMA + NU_VPE_INT_RESET);
	  iowrite32(0x00000000,base + NA_VPE_BASE + NU_VPE_DST_WDMA + NU_VPE_INT_MASK);
	  
	  iowrite32(0x1F1     ,base + NA_VPE_BASE + NU_VPE_SRC_RDMA + NU_VPE_INT_RESET);
	  iowrite32(0x00000000,base + NA_VPE_BASE + NU_VPE_SRC_RDMA + NU_VPE_INT_MASK);
	  
	  iowrite32(0x1F1     ,base + NA_VPE_BASE + NU_VPE_OP0_RDMA + NU_VPE_INT_RESET);
	  iowrite32(0x00000000,base + NA_VPE_BASE + NU_VPE_OP0_RDMA + NU_VPE_INT_MASK);
	  
	  iowrite32(0x1F1     ,base + NA_VPE_BASE + NU_VPE_OP1_RDMA + NU_VPE_INT_RESET);
	  iowrite32(0x00000000,base + NA_VPE_BASE + NU_VPE_OP1_RDMA + NU_VPE_INT_MASK);
	  
	  iowrite32(0x1F1	  ,base + NA_VPE_BASE + NU_VPE_OP1_RDMA + NU_VPE_INT_RESET);
	  iowrite32(0x00000000,base + NA_VPE_BASE + NU_VPE_OP1_RDMA + NU_VPE_INT_MASK);	  
	  
	  
	  
 }