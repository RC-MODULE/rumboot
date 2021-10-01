
#include <stdint.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <regs/regs_na.h>
//#include <regs/regs_nu_mpe.h>
//#include <regs/regs_nu_vpe.h>
//#include <regs/regs_nu_ppe.h>

//#include <platform/devices.h> 
#include <platform/devices/nu_cpdmac_lib.h>
#include <platform/devices/nu_lib.h>

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

}

void nu_vpe_load_status_regs(VPEStatusRegs* status_regs, void* status_regs_bin) {
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

  cfg-> H        =*ptr;ptr++;
  cfg-> W        =*ptr;ptr++;
  cfg-> C        =*ptr;ptr++;
  cfg-> Tp       =*ptr;ptr++;
  cfg-> Bp       =*ptr;ptr++;
  cfg-> Lp       =*ptr;ptr++;
  cfg-> Rp       =*ptr;ptr++;
  cfg-> R        =*ptr;ptr++;
  cfg-> S        =*ptr;ptr++;
  cfg-> Ds       =*ptr;ptr++;
  cfg-> Dr       =*ptr;ptr++;
  cfg-> Sw       =*ptr;ptr++;
  cfg-> Sh       =*ptr;ptr++;
  cfg-> K        =*ptr;ptr++;
  cfg-> dt       =*ptr;ptr++;
  cfg-> rnd_mode =*ptr;ptr++;
  cfg-> sat_en   =*ptr;ptr++;
  cfg-> rnd_size =*ptr;ptr++;
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

    cfg-> Kh_r  =*ptr;ptr++;
    cfg-> Kw_r  =*ptr;ptr++;

    cfg-> pv[0] =*ptr;ptr++;
    cfg-> pv[1] =*ptr;ptr++;
    cfg-> pv[2] =*ptr;ptr++;
    cfg-> pv[3] =*ptr;ptr++;
    cfg-> pv[4] =*ptr;ptr++;
    cfg-> pv[5] =*ptr;ptr++;
    cfg-> pv[6] =*ptr;ptr++;

    cfg-> meth  =*ptr;ptr++;
    cfg-> dt    =*ptr;ptr++;
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
    cfg_reg->wKWr   = *ptr; ptr++;
    cfg_reg->wKHr   = *ptr; ptr++;
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
      "HalfAwayFromZero",
      "Nearest",
      "Down",
      "Up"
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
  nu_vpe_print_DataTypeExt(cfg->in_data_type,"in_data_type");
  nu_vpe_print_DataType(cfg->out_data_type,"out_data_type");
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

#endif // NU_NO_PRINT
}

void nu_vpe_print_config_dma(ConfigDMAVPE* cfg) {
#ifndef NU_NO_PRINT
  rumboot_printf("ConfigDMAVPE:\n");
  nu_vpe_print_Enable(cfg->dma_src_en,"dma_src_en");
  nu_vpe_print_Enable(cfg->dma_op0_en,"dma_op0_en");
  nu_vpe_print_Enable(cfg->dma_op1_en,"dma_op1_en");
  nu_vpe_print_Enable(cfg->dma_op2_en,"dma_op2_en");
  nu_vpe_print_Enable(cfg->dma_dst_en,"dma_dst_en");
#endif
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
#endif // NU_NO_PRINT
  
}

void nu_vpe_print_status_regs_etalon(VPEStatusRegs* status_regs) {
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
    rumboot_printf("  ThreCtrl = %d \n", bias->ThreCtrl);
    rumboot_printf("  DecCtrl = %d \n", bias->DecCtrl);
    nu_vpe_print_Enable(bias->PBSEn,"PBSEn");
    rumboot_printf("  Bias = 0x%x \n",bias->Bias);
    rumboot_printf("  AOffset = 0x%x \n",bias->AOffset );
    rumboot_printf("  CntSha = %d \n",bias->CntSha);
    nu_vpe_print_Enable(bias->CntOffsetEn,"CntOffsetEn");
    rumboot_printf("  CntOffset = %d \n",bias->CntOffset);
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
    rumboot_printf("  Ds       = %d \n" , cfg->Ds);
    rumboot_printf("  Dr       = %d \n" , cfg->Dr);
    rumboot_printf("  Sw       = %d \n" , cfg->Sw);
    rumboot_printf("  Sh       = %d \n" , cfg->Sh);
    rumboot_printf("  K        = %d \n" , cfg->K);
    nu_vpe_print_DataType(cfg->dt,"dt      ");
    nu_vpe_print_RoundMode(cfg->rnd_mode,"rnd_mode");
    nu_vpe_print_Enable(cfg->sat_en, "sat_en  ");
    rumboot_printf("  rnd_size = %d \n" , cfg->rnd_size);

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
    rumboot_printf("  Kh_r   = 0x%x \n" , cfg->Kh_r);
    rumboot_printf("  Kw_r   = 0x%x \n" , cfg->Kw_r);
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
  rumboot_printf ("rBALi = %x\n", cfg_reg->rBALi);
  rumboot_printf ("rBrdX = %x\n", cfg_reg->rBrdX);
  rumboot_printf ("rBrdY = %x\n", cfg_reg->rBrdY);
  rumboot_printf ("rBrdZ = %x\n", cfg_reg->rBrdZ);
  rumboot_printf ("rStrX = %x\n", cfg_reg->rStrX);
  rumboot_printf ("rStrY = %x\n", cfg_reg->rStrY);
  rumboot_printf ("rStrZ = %x\n", cfg_reg->rStrZ);
  rumboot_printf ("rFrgs = %x\n", cfg_reg->rFrgs);
  rumboot_printf ("rFrgl = %x\n", cfg_reg->rFrgl);
  rumboot_printf ("rXYZd = %x\n", cfg_reg->rXYZd);
  rumboot_printf ("rBstX = %x\n", cfg_reg->rBstX);
  rumboot_printf ("rBstY = %x\n", cfg_reg->rBstY);
  rumboot_printf ("rBstZ = %x\n", cfg_reg->rBstZ);
  rumboot_printf ("rBxtX = %x\n", cfg_reg->rBxtX);
  rumboot_printf ("rBxtY = %x\n", cfg_reg->rBxtY);
  rumboot_printf ("rBxtZ = %x\n", cfg_reg->rBxtZ);
  rumboot_printf ("rBffX = %x\n", cfg_reg->rBffX);
  rumboot_printf ("rBffY = %x\n", cfg_reg->rBffY);
  rumboot_printf ("rBffZ = %x\n", cfg_reg->rBffZ);
  
  //rumboot_printf ("wOpEn = %x\n", cfg_reg->wOpEn);
  //rumboot_printf ("wAXIp = %x\n", cfg_reg->wAXIp);
  rumboot_printf ("wBALo = %x\n", cfg_reg->wBALo);
  rumboot_printf ("wBrdX = %x\n", cfg_reg->wBrdX);
  rumboot_printf ("wBrdY = %x\n", cfg_reg->wBrdY);
  rumboot_printf ("wBrdZ = %x\n", cfg_reg->wBrdZ);
  rumboot_printf ("wStrX = %x\n", cfg_reg->wStrX);
  rumboot_printf ("wStrY = %x\n", cfg_reg->wStrY);
  rumboot_printf ("wStrZ = %x\n", cfg_reg->wStrZ);
  rumboot_printf ("wFrgs = %x\n", cfg_reg->wFrgs);
  rumboot_printf ("wFrgl = %x\n", cfg_reg->wFrgl);
  rumboot_printf ("wXYZd = %x\n", cfg_reg->wXYZd);
  rumboot_printf ("wBstX = %x\n", cfg_reg->wBstX);
  rumboot_printf ("wBstY = %x\n", cfg_reg->wBstY);
  rumboot_printf ("wBstZ = %x\n", cfg_reg->wBstZ);
  rumboot_printf ("wBxtX = %x\n", cfg_reg->wBxtX);
  rumboot_printf ("wBxtY = %x\n", cfg_reg->wBxtY);
  rumboot_printf ("wBxtZ = %x\n", cfg_reg->wBxtZ);
  rumboot_printf ("wBffX = %x\n", cfg_reg->wBffX);
  rumboot_printf ("wBffY = %x\n", cfg_reg->wBffY);
  rumboot_printf ("wBffZ = %x\n", cfg_reg->wBffZ);
  rumboot_printf ("wIstX = %x\n", cfg_reg->wIstX);
  rumboot_printf ("wIxtX = %x\n", cfg_reg->wIxtX);
  rumboot_printf ("wIffX = %x\n", cfg_reg->wIffX);
  rumboot_printf ("wWi   = %x\n", cfg_reg->wWi  );
  rumboot_printf ("wHi   = %x\n", cfg_reg->wHi  );
  rumboot_printf ("wCi   = %x\n", cfg_reg->wCi  );
  rumboot_printf ("wWo   = %x\n", cfg_reg->wWo  );
  rumboot_printf ("wHo   = %x\n", cfg_reg->wHo  );
  rumboot_printf ("wCo   = %x\n", cfg_reg->wCo  );
  rumboot_printf ("wOpM  = %x\n", cfg_reg->wOpM );
  rumboot_printf ("wK    = %x\n", cfg_reg->wK   );
  rumboot_printf ("wKWr  = %x\n", cfg_reg->wKWr );
  rumboot_printf ("wKHr  = %x\n", cfg_reg->wKHr );
  rumboot_printf ("wP    = %x\n", cfg_reg->wP   );
  rumboot_printf ("wPV1  = %x\n", cfg_reg->wPV1 );
  rumboot_printf ("wPV2  = %x\n", cfg_reg->wPV2 );
  rumboot_printf ("wPV3  = %x\n", cfg_reg->wPV3 );
  rumboot_printf ("wPV4  = %x\n", cfg_reg->wPV4 );
  rumboot_printf ("wPV5  = %x\n", cfg_reg->wPV5 );
  rumboot_printf ("wPV6  = %x\n", cfg_reg->wPV6 );
  rumboot_printf ("wPV7  = %x\n", cfg_reg->wPV7 );
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
  
  temp_ui = *((uint32_t*) &a); // Take A Bit Couple
  
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
  uint32_t lut1_size = 257; // Predefined By HW
  uint32_t lut2_size = 65;
  
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

void nu_vpe_setup(uintptr_t base, ConfigVPE* cfg) {
  rumboot_printf("Configuring VPE..\n");

  // iowrite32(cfg->MYFIELD, base + NU_VPE_MYREG);

  int32_t     tmp_data;
  DataType    tmp_type;

  // Configuration of VPE -------------------------------------------------------------------------
  // CUBE_SIZE 
  iowrite32(cfg->cube_size, base + NU_VPE + NU_VPE_CUBE_SIZE );
  iowrite32((cfg->wdma_config.dma_cube_size_c-1)/16 +1, base + NU_VPE + NU_VPE_CUBE_SIZE_C );  // size in vectors!!!!!!!  // TAKES IT FROM DMA struct!!!! NOT GOOD!!!
  iowrite32(cfg->wdma_config.dma_cube_size_w,           base + NU_VPE + NU_VPE_CUBE_SIZE_W );                             // TAKES IT FROM DMA struct!!!! NOT GOOD!!!
  iowrite32(cfg->wdma_config.dma_cube_size_h,           base + NU_VPE + NU_VPE_CUBE_SIZE_H );                             // TAKES IT FROM DMA struct!!!! NOT GOOD!!!
  
  if      (cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Int16) tmp_type = DataType_Int16 ;
  else if (cfg->in_data_type == DataTypeExt_Fp32  || cfg->in_data_type == DataTypeExt_Fp16)  tmp_type = DataType_Fp16  ;
  else                                                                                      tmp_type = DataType_Int8  ;
  tmp_data = (1<<30) | (1<<29) | (1<<28) |   // All Counters On
             (cfg->out_data_type << 16) | 
             ((cfg->op2_config.coef_type>>1) << 15) | ((cfg->op2_rdma_config.dma_data_mode&0x1)<<14) |   // TAKES IT FROM DMA struct!!!! NOT GOOD!!!
             ((cfg->op1_config.coef_type>>1) << 13) | ((cfg->op1_rdma_config.dma_data_mode&0x1)<<12) |   // TAKES IT FROM DMA struct!!!! NOT GOOD!!!
             ((cfg->op0_config.coef_type>>1) << 11) | ((cfg->op0_rdma_config.dma_data_mode&0x1)<<10) |   // TAKES IT FROM DMA struct!!!! NOT GOOD!!!
             (                 (tmp_type>>1) <<  9) | (/*(cfg->dst_flying == Enable_NotEn)*/cfg->trace_mode<<8) |           // that 8bit definition is RIGHT only here !!!!!!!!!!!
             (cfg->nan_to_zero_input << 4) | (cfg->dst_flying << 1) | (cfg->src_flying << 0) ;
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP_MODE);
  
  // Configuration OUT --------------------------------------------------
  iowrite32(cfg->c3_offset, base + NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL);
  iowrite32(cfg->c3_scale , base + NU_VPE + NU_VPE_OUT_CVT_SCALE_VAL );
  
  tmp_data = (cfg->c3_round_mode << 12) | (cfg->c3_satur_en << 8) | (cfg->c3_trunc << 0) ;
  iowrite32(tmp_data , base + NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL );

  // OP0 ----------------------------------------------------------------
  tmp_data = (!cfg->op0_config.relu_en << 6) | (cfg->op0_config.prelu_en << 5) | (!cfg->op0_config.mux_en << 4) | (cfg->op0_config.alu_operation << 2) | (!cfg->op0_config.alu_en << 1) | (!cfg->op0_en << 0);
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP0_CFG);
  
  tmp_data = (cfg->op0_config.lshift_value << 8) | ((cfg->op0_config.alu_mode>>1) << 0) ; // (nu_vpe_mode_to_bool(cfg->op0_config.alu_mode) = (cfg->op0_config.alu_mode>>1)
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP0_ALU_CFG);
  
  iowrite32(cfg->op0_config.alu_value, base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL);
  
  iowrite32((cfg->op0_config.mux_mode>>1) , base + NU_VPE + NU_VPE_OP0_MUL_CFG); //???????????????????
  iowrite32(cfg->op0_config.mux_value, base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL);

  tmp_data = (cfg->op0_config.norm_round_mode << 16) | (cfg->op0_config.norm_saturation_en << 8) | (cfg->op0_config.norm_round_size << 0);
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP0_NORM_PARAM);  
  
  // OP1 ----------------------------------------------------------------
  tmp_data = (!cfg->op1_config.relu_en << 6) | (cfg->op1_config.prelu_en << 5) | (!cfg->op1_config.mux_en << 4) | (cfg->op1_config.alu_operation << 2) | (!cfg->op1_config.alu_en << 1) | (!cfg->op1_en << 0);
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP1_CFG);  
  
  tmp_data = (cfg->op1_config.lshift_value << 8) | ((cfg->op1_config.alu_mode>>1) << 0) ; // ???????????????????????????????
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP1_ALU_CFG);

  iowrite32(cfg->op1_config.alu_value, base + NU_VPE + NU_VPE_OP1_ALU_SRC_VAL);

  iowrite32((cfg->op1_config.mux_mode>>1) , base + NU_VPE + NU_VPE_OP1_MUL_CFG);
  iowrite32(cfg->op1_config.mux_value, base + NU_VPE + NU_VPE_OP1_MUL_SRC_VAL);

  tmp_data = (cfg->op1_config.norm_round_mode << 16) | (cfg->op1_config.norm_saturation_en << 8) | (cfg->op1_config.norm_round_size << 0);
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP1_NORM_PARAM);    
  
  // OP2 ----------------------------------------------------------------
  if (cfg->op2_config.alu_operation == AluOperationExtSwitch_Eql) {
      tmp_data = (0x1 << 7) | (!cfg->op2_config.lut_en << 6) | (cfg->op2_config.prelu_en << 5) | (!cfg->op2_config.mux_en << 4) | (                            0 << 2) | (!cfg->op2_config.alu_en << 1) | (!cfg->op2_en << 0);
  }
  else {  
      tmp_data =              (!cfg->op2_config.lut_en << 6) | (cfg->op2_config.prelu_en << 5) | (!cfg->op2_config.mux_en << 4) | (cfg->op2_config.alu_operation << 2) | (!cfg->op2_config.alu_en << 1) | (!cfg->op2_en << 0);
  }
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP2_CFG);   
  
  tmp_data = (!cfg->op2_config.c2_en << 1) | ((cfg->op2_config.alu_mode>>1) << 0) ; // ???????????????????????????????
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP2_ALU_CFG);  

  iowrite32(cfg->op2_config.alu_value, base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL);

  iowrite32(cfg->op2_config.c2_offset, base + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL);
  iowrite32(cfg->op2_config.c2_scale , base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL );
  iowrite32(cfg->op2_config.c2_trunc , base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL );


  tmp_data = (!cfg->op2_config.c1_en << 1) | ((cfg->op2_config.mux_mode>>1) << 0) ; // ???????????????????????????????
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP2_MUL_CFG);  

  iowrite32(cfg->op2_config.mux_value, base + NU_VPE + NU_VPE_OP2_MUL_SRC_VAL);
  
  iowrite32(cfg->op2_config.c1_offset, base + NU_VPE + NU_VPE_OP2_MUL_CVT_OFFSET_VAL);
  iowrite32(cfg->op2_config.c1_scale , base + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL );
  iowrite32(cfg->op2_config.c1_trunc , base + NU_VPE + NU_VPE_OP2_MUL_CVT_TRUNC_VAL );  
  
  tmp_data = (cfg->op2_config.norm_round_mode << 16) | (cfg->op2_config.norm_saturation_en << 8) | (cfg->op2_config.norm_round_size << 0);
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP2_NORM_PARAM);   
  
  if(cfg->op2_config.lut_en) {
    int32_t shift_i1;
    int32_t shift_i2;
    
    tmp_data = 0x000000000 | (cfg->op2_config.lut_sel << 2) | (cfg->op2_config.lut_right_priority << 1) | (cfg->op2_config.lut_left_priority << 0) |
                             (cfg->op2_config.lut_mrr_type << 4) | (cfg->op2_config.lut_mrr_en << 3);
    iowrite32(tmp_data, base + NU_VPE + NU_VPE_LUT_CFG);
    
    iowrite32(cfg->op2_config.lut_tab1_x_start, base + NU_VPE + NU_VPE_LUT_TAB0_XMIN);
    iowrite32(cfg->op2_config.lut_tab1_x_end  , base + NU_VPE + NU_VPE_LUT_TAB0_XMAX  );
    iowrite32(cfg->op2_config.lut_tab2_x_start, base + NU_VPE + NU_VPE_LUT_TAB1_XMIN);
    iowrite32(cfg->op2_config.lut_tab2_x_end  , base + NU_VPE + NU_VPE_LUT_TAB1_XMAX  );
    
    tmp_data = 0x000000000 | ((cfg->op2_config.lut_tab1_slope_ovrf_scale & 0x0000FFFF) << 16) |
                             ((cfg->op2_config.lut_tab1_slope_undf_scale & 0x0000FFFF)      );
    iowrite32(tmp_data, base + NU_VPE + NU_VPE_LUT_TAB0_SLOPE_SCALE);
    
    tmp_data = 0x000000000 | ((cfg->op2_config.lut_tab1_slope_ovrf_shift & 0x1F) << 5) |
                             ((cfg->op2_config.lut_tab1_slope_undf_shift & 0x1F)     );
    iowrite32(tmp_data, base + NU_VPE + NU_VPE_LUT_TAB0_SLOPE_SHIFT);
    
    tmp_data = 0x000000000 | ((cfg->op2_config.lut_tab2_slope_ovrf_scale & 0x0000FFFF) << 16) |
                             ((cfg->op2_config.lut_tab2_slope_undf_scale & 0x0000FFFF)      );
    iowrite32(tmp_data, base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE);
    
    tmp_data = 0x000000000 | ((cfg->op2_config.lut_tab2_slope_ovrf_shift & 0x1F) << 5) |
                             ((cfg->op2_config.lut_tab2_slope_undf_shift & 0x1F)     );
    iowrite32(tmp_data, base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT);
    
    iowrite32(cfg->op2_config.lut_xoffset, base + NU_VPE + NU_VPE_LUT_XOFFSET);
    tmp_data = 0x000000000 | cfg->op2_config.lut_yoffset;
    iowrite32(tmp_data, base + NU_VPE + NU_VPE_LUT_YOFFSET);
    
    
    if(cfg->in_data_type == DataTypeExt_Fp16 || cfg->in_data_type == DataTypeExt_Fp32) { // Floating Point LUT
      float start_f;
      float end_f;
      float diff_f;
      uint32_t temp_ui;
      
      temp_ui = cfg->op2_config.lut_tab1_x_end  ;end_f   = *((float*) &(temp_ui));// Read The Couple Of Bits 
      temp_ui = cfg->op2_config.lut_tab1_x_start;start_f = *((float*) &(temp_ui));//  And Store It In A float Variable
      diff_f = end_f  - start_f;
      shift_i1 = nu_lut_log2_f(diff_f) - 8;
      
      temp_ui = cfg->op2_config.lut_tab2_x_end  ;end_f   = *((float*) &(temp_ui));// Read The Couple Of Bits 
      temp_ui = cfg->op2_config.lut_tab2_x_start;start_f = *((float*) &(temp_ui));//  And Store It In A float Variable
      diff_f = end_f  - start_f;
      shift_i2 = nu_lut_log2_f(diff_f) - 6;
    }
    else { // Integer LUT
      shift_i1 = nu_lut_log2_i(cfg->op2_config.lut_tab1_x_end - cfg->op2_config.lut_tab1_x_start) - 8;
      shift_i2 = nu_lut_log2_i(cfg->op2_config.lut_tab2_x_end - cfg->op2_config.lut_tab2_x_start) - 6;
    }
    
    rumboot_printf("LUTs: shift_i1 = %x, shift_i2 = %x\n",shift_i1,shift_i2);
    iowrite32(shift_i1,base + NU_VPE + NU_VPE_LUT_TAB0_XSHIFT);
    iowrite32(shift_i2,base + NU_VPE + NU_VPE_LUT_TAB1_XSHIFT);
  }

  // Configuration of WDMA -------------------------------------------------------------------------
  //iowrite32(cfg->wdma_config.dma_en , base + NU_VPE_DST_WDMA + NU_VPE_DMA_CFG );
  
  tmp_data = (cfg->wdma_config.dma_ram_type << 8) | (cfg->wdma_config.dma_data_size << 6) | (cfg->wdma_config.dma_en << 0);
  iowrite32(tmp_data, base + NU_VPE_DST_WDMA + NU_VPE_DMA_CFG);

  iowrite32((0x00020000 | (uint32_t) cfg->wdma_config.dma_axi_len), base + NU_VPE_DST_WDMA + NU_VPE_DMA_AXI_PARAM );

  iowrite32(cfg->wdma_config.dma_baddr           , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BASE                ) ;
  iowrite32(cfg->wdma_config.dma_cube_size_c     , base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_C         ) ;  // ?????
  iowrite32(cfg->wdma_config.dma_cube_size_w     , base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_W         ) ;  // ?????
  iowrite32(cfg->wdma_config.dma_cube_size_h     , base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_H         ) ;  // ?????                                                                                          
  iowrite32(cfg->wdma_config.dma_border_x        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_X            ) ;
  iowrite32(cfg->wdma_config.dma_border_y        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_Y            ) ;
  iowrite32(cfg->wdma_config.dma_border_z        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_Z            ) ;
  iowrite32(cfg->wdma_config.dma_stride_x        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_X            ) ;
  iowrite32(cfg->wdma_config.dma_stride_y        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_Y            ) ;
  iowrite32(cfg->wdma_config.dma_stride_z        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_Z            ) ; 
  iowrite32(cfg->wdma_config.dma_frag_last_size  , base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR ) ;
  iowrite32(cfg->wdma_config.dma_frag_size       , base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_SIZE_ADDR      ) ;
  iowrite32(cfg->wdma_config.dma_xyz_drct        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_XYZ_DRCT_ADDR       ) ;  // 0 = x; 1 = y; 2 = z
  iowrite32(cfg->wdma_config.dma_box_st_size_x   , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_ST_SIZE_X       ) ;
  iowrite32(cfg->wdma_config.dma_box_st_size_y   , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_ST_SIZE_Y       ) ;
  iowrite32(cfg->wdma_config.dma_box_st_size_z   , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_ST_SIZE_Z       ) ;
  iowrite32(cfg->wdma_config.dma_box_size_x      , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_SIZE_X          ) ;
  iowrite32(cfg->wdma_config.dma_box_size_y      , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_SIZE_Y          ) ;
  iowrite32(cfg->wdma_config.dma_box_size_z      , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_SIZE_Z          ) ; 
  iowrite32(cfg->wdma_config.dma_box_offset_x    , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_X   ) ;
  iowrite32(cfg->wdma_config.dma_box_offset_y    , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y   ) ;
  iowrite32(cfg->wdma_config.dma_box_offset_z    , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Z   ) ;  

  // Configuration SRC_RDMA ------------------------------------------------------
  
  //iowrite32(cfg->src_rdma_config.dma_en , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CFG );
  
  tmp_data = (cfg->src_rdma_config.dma_ram_type << 8) | ((cfg->src_rdma_config.dma_data_mode >> 1) << 7) | (cfg->src_rdma_config.dma_data_size << 6) | (cfg->src_rdma_config.dma_data_use << 4) | (cfg->src_rdma_config.dma_en << 0);
  iowrite32(tmp_data, base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CFG );

  iowrite32((0x00020000 | (uint32_t) cfg->src_rdma_config.dma_axi_len), base + NU_VPE_SRC_RDMA + NU_VPE_DMA_AXI_PARAM );

  iowrite32(cfg->src_rdma_config.dma_baddr           , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BASE                ) ;
  iowrite32(cfg->src_rdma_config.dma_cube_size_c     , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CUBE_SIZE_C         ) ;  // ?????
  iowrite32(cfg->src_rdma_config.dma_cube_size_w     , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CUBE_SIZE_W         ) ;  // ?????
  iowrite32(cfg->src_rdma_config.dma_cube_size_h     , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CUBE_SIZE_H         ) ;  // ?????                                                                                          
  iowrite32(cfg->src_rdma_config.dma_border_x        , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_X            ) ;
  iowrite32(cfg->src_rdma_config.dma_border_y        , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_Y            ) ;
  iowrite32(cfg->src_rdma_config.dma_border_z        , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_Z            ) ;
  iowrite32(cfg->src_rdma_config.dma_stride_x        , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_STRIDE_X            ) ;
  iowrite32(cfg->src_rdma_config.dma_stride_y        , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_STRIDE_Y            ) ;
  iowrite32(cfg->src_rdma_config.dma_stride_z        , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_STRIDE_Z            ) ;
  iowrite32(cfg->src_rdma_config.dma_frag_last_size  , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR ) ;
  iowrite32(cfg->src_rdma_config.dma_frag_size       , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_FRAG_SIZE_ADDR      ) ;
  iowrite32(cfg->src_rdma_config.dma_xyz_drct        , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_XYZ_DRCT_ADDR       ) ;  // 0 = x; 1 = y; 2 = z
  iowrite32(cfg->src_rdma_config.dma_box_st_size_x   , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_ST_SIZE_X       ) ;
  iowrite32(cfg->src_rdma_config.dma_box_st_size_y   , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Y       ) ;
  iowrite32(cfg->src_rdma_config.dma_box_st_size_z   , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Z       ) ;
  iowrite32(cfg->src_rdma_config.dma_box_size_x      , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_SIZE_X          ) ;
  iowrite32(cfg->src_rdma_config.dma_box_size_y      , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_SIZE_Y          ) ;
  iowrite32(cfg->src_rdma_config.dma_box_size_z      , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_SIZE_Z          ) ;
  iowrite32(cfg->src_rdma_config.dma_box_offset_x    , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_X   ) ;
  iowrite32(cfg->src_rdma_config.dma_box_offset_y    , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y   ) ;
  iowrite32(cfg->src_rdma_config.dma_box_offset_z    , base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Z   ) ;
  

  // Configuration OP0_RDMA ------------------------------------------------------
  //iowrite32(cfg->op0_rdma_config.dma_en , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_CFG );

  tmp_data = (cfg->op0_rdma_config.dma_ram_type << 8) | ((cfg->op0_rdma_config.dma_data_mode >> 1) << 7) | (cfg->op0_rdma_config.dma_data_size << 6) | (cfg->op0_rdma_config.dma_data_use << 4) | (cfg->op0_rdma_config.dma_en << 0);
  iowrite32(tmp_data, base + NU_VPE_OP0_RDMA + NU_VPE_DMA_CFG );
  
  iowrite32((0x00020000 | (uint32_t) cfg->op0_rdma_config.dma_axi_len), base + NU_VPE_OP0_RDMA + NU_VPE_DMA_AXI_PARAM );
  
  iowrite32(cfg->op0_rdma_config.dma_baddr           , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BASE                ) ;
  iowrite32(cfg->op0_rdma_config.dma_cube_size_c     , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_CUBE_SIZE_C         ) ;  // ?????
  iowrite32(cfg->op0_rdma_config.dma_cube_size_w     , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_CUBE_SIZE_W         ) ;  // ?????
  iowrite32(cfg->op0_rdma_config.dma_cube_size_h     , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_CUBE_SIZE_H         ) ;  // ?????                                                                                      
  iowrite32(cfg->op0_rdma_config.dma_border_x        , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BORDER_X            ) ;
  iowrite32(cfg->op0_rdma_config.dma_border_y        , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BORDER_Y            ) ;
  iowrite32(cfg->op0_rdma_config.dma_border_z        , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BORDER_Z            ) ;
  iowrite32(cfg->op0_rdma_config.dma_stride_x        , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_STRIDE_X            ) ;
  iowrite32(cfg->op0_rdma_config.dma_stride_y        , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_STRIDE_Y            ) ;
  iowrite32(cfg->op0_rdma_config.dma_stride_z        , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_STRIDE_Z            ) ;
  iowrite32(cfg->op0_rdma_config.dma_frag_last_size  , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR ) ;
  iowrite32(cfg->op0_rdma_config.dma_frag_size       , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_FRAG_SIZE_ADDR      ) ;
  iowrite32(cfg->op0_rdma_config.dma_xyz_drct        , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_XYZ_DRCT_ADDR       ) ;  // 0 = x; 1 = y; 2 = z
  iowrite32(cfg->op0_rdma_config.dma_box_st_size_x   , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BOX_ST_SIZE_X       ) ;
  iowrite32(cfg->op0_rdma_config.dma_box_st_size_y   , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Y       ) ;
  iowrite32(cfg->op0_rdma_config.dma_box_st_size_z   , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Z       ) ;
  iowrite32(cfg->op0_rdma_config.dma_box_size_x      , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BOX_SIZE_X          ) ;
  iowrite32(cfg->op0_rdma_config.dma_box_size_y      , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BOX_SIZE_Y          ) ;
  iowrite32(cfg->op0_rdma_config.dma_box_size_z      , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BOX_SIZE_Z          ) ;
  iowrite32(cfg->op0_rdma_config.dma_box_offset_x    , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_X   ) ;
  iowrite32(cfg->op0_rdma_config.dma_box_offset_y    , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y   ) ;
  iowrite32(cfg->op0_rdma_config.dma_box_offset_z    , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Z   ) ;

  
  // Configuration OP1_RDMA ------------------------------------------------------
  //iowrite32(cfg->op1_rdma_config.dma_en , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_CFG );
  
  tmp_data = (cfg->op1_rdma_config.dma_ram_type << 8) | ((cfg->op1_rdma_config.dma_data_mode >> 1) << 7) | (cfg->op1_rdma_config.dma_data_size << 6) | (cfg->op1_rdma_config.dma_data_use << 4) | (cfg->op1_rdma_config.dma_en << 0);
  iowrite32(tmp_data, base + NU_VPE_OP1_RDMA + NU_VPE_DMA_CFG );
  
  iowrite32((0x00020000 | (uint32_t) cfg->op1_rdma_config.dma_axi_len), base + NU_VPE_OP1_RDMA + NU_VPE_DMA_AXI_PARAM );
  
  iowrite32(cfg->op1_rdma_config.dma_baddr           , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BASE                ) ;
  iowrite32(cfg->op1_rdma_config.dma_cube_size_c     , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_CUBE_SIZE_C         ) ;  // ?????
  iowrite32(cfg->op1_rdma_config.dma_cube_size_w     , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_CUBE_SIZE_W         ) ;  // ?????
  iowrite32(cfg->op1_rdma_config.dma_cube_size_h     , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_CUBE_SIZE_H         ) ;  // ?????                                                                                          
  iowrite32(cfg->op1_rdma_config.dma_border_x        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BORDER_X            ) ;
  iowrite32(cfg->op1_rdma_config.dma_border_y        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BORDER_Y            ) ;
  iowrite32(cfg->op1_rdma_config.dma_border_z        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BORDER_Z            ) ;
  iowrite32(cfg->op1_rdma_config.dma_stride_x        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_STRIDE_X            ) ;
  iowrite32(cfg->op1_rdma_config.dma_stride_y        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_STRIDE_Y            ) ;
  iowrite32(cfg->op1_rdma_config.dma_stride_z        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_STRIDE_Z            ) ;
  iowrite32(cfg->op1_rdma_config.dma_frag_last_size  , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR ) ;
  iowrite32(cfg->op1_rdma_config.dma_frag_size       , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_FRAG_SIZE_ADDR      ) ;
  iowrite32(cfg->op1_rdma_config.dma_xyz_drct        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_XYZ_DRCT_ADDR       ) ;  // 0 = x; 1 = y; 2 = z
  iowrite32(cfg->op1_rdma_config.dma_box_st_size_x   , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BOX_ST_SIZE_X       ) ;
  iowrite32(cfg->op1_rdma_config.dma_box_st_size_y   , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Y       ) ;
  iowrite32(cfg->op1_rdma_config.dma_box_st_size_z   , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Z       ) ;
  iowrite32(cfg->op1_rdma_config.dma_box_size_x      , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BOX_SIZE_X          ) ;
  iowrite32(cfg->op1_rdma_config.dma_box_size_y      , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BOX_SIZE_Y          ) ;
  iowrite32(cfg->op1_rdma_config.dma_box_size_z      , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BOX_SIZE_Z          ) ;
  iowrite32(cfg->op1_rdma_config.dma_box_offset_x    , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_X   ) ;
  iowrite32(cfg->op1_rdma_config.dma_box_offset_y    , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y   ) ;
  iowrite32(cfg->op1_rdma_config.dma_box_offset_z    , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Z   ) ;
  
  // Configuration OP2_RDMA ------------------------------------------------------
  //iowrite32(cfg->op2_rdma_config.dma_en , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_CFG );
  
  tmp_data = (cfg->op2_rdma_config.dma_ram_type << 8) | ((cfg->op2_rdma_config.dma_data_mode >> 1) << 7) | (cfg->op2_rdma_config.dma_data_size << 6) | (cfg->op2_rdma_config.dma_data_use << 4) | (cfg->op2_rdma_config.dma_en << 0);
  iowrite32(tmp_data, base + NU_VPE_OP2_RDMA + NU_VPE_DMA_CFG );

  iowrite32((0x00020000 | (uint32_t) cfg->op2_rdma_config.dma_axi_len), base + NU_VPE_OP2_RDMA + NU_VPE_DMA_AXI_PARAM );

  iowrite32(cfg->op2_rdma_config.dma_baddr           , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BASE                ) ;
  iowrite32(cfg->op2_rdma_config.dma_cube_size_c     , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_CUBE_SIZE_C         ) ;  // ?????
  iowrite32(cfg->op2_rdma_config.dma_cube_size_w     , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_CUBE_SIZE_W         ) ;  // ?????
  iowrite32(cfg->op2_rdma_config.dma_cube_size_h     , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_CUBE_SIZE_H         ) ;  // ?????                                                                                          
  iowrite32(cfg->op2_rdma_config.dma_border_x        , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BORDER_X            ) ;
  iowrite32(cfg->op2_rdma_config.dma_border_y        , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BORDER_Y            ) ;
  iowrite32(cfg->op2_rdma_config.dma_border_z        , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BORDER_Z            ) ;
  iowrite32(cfg->op2_rdma_config.dma_stride_x        , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_STRIDE_X            ) ;
  iowrite32(cfg->op2_rdma_config.dma_stride_y        , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_STRIDE_Y            ) ;
  iowrite32(cfg->op2_rdma_config.dma_stride_z        , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_STRIDE_Z            ) ;
  iowrite32(cfg->op2_rdma_config.dma_frag_last_size  , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR ) ;
  iowrite32(cfg->op2_rdma_config.dma_frag_size       , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_FRAG_SIZE_ADDR      ) ;
  iowrite32(cfg->op2_rdma_config.dma_xyz_drct        , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_XYZ_DRCT_ADDR       ) ;  // 0 = x; 1 = y; 2 = z
  iowrite32(cfg->op2_rdma_config.dma_box_st_size_x   , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BOX_ST_SIZE_X       ) ;
  iowrite32(cfg->op2_rdma_config.dma_box_st_size_y   , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Y       ) ;
  iowrite32(cfg->op2_rdma_config.dma_box_st_size_z   , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Z       ) ;
  iowrite32(cfg->op2_rdma_config.dma_box_size_x      , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BOX_SIZE_X          ) ;
  iowrite32(cfg->op2_rdma_config.dma_box_size_y      , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BOX_SIZE_Y          ) ;
  iowrite32(cfg->op2_rdma_config.dma_box_size_z      , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BOX_SIZE_Z          ) ;
  iowrite32(cfg->op2_rdma_config.dma_box_offset_x    , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_X   ) ;
  iowrite32(cfg->op2_rdma_config.dma_box_offset_y    , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y   ) ;
  iowrite32(cfg->op2_rdma_config.dma_box_offset_z    , base + NU_VPE_OP2_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Z   ) ;

}

void nu_vpe_decide_dma_config_trivial(ConfigVPE* cfg, CubeMetrics* metrics) {
  // New structur
  cfg->src_rdma_config.dma_cube_size_w = metrics->W;
  cfg->src_rdma_config.dma_cube_size_h = metrics->H;
  cfg->src_rdma_config.dma_cube_size_c = metrics->C;
  
  cfg->op0_rdma_config.dma_cube_size_w = metrics->W;
  cfg->op0_rdma_config.dma_cube_size_h = metrics->H;
  cfg->op0_rdma_config.dma_cube_size_c = metrics->C;
  
  cfg->op1_rdma_config.dma_cube_size_w = metrics->W;
  cfg->op1_rdma_config.dma_cube_size_h = metrics->H;
  cfg->op1_rdma_config.dma_cube_size_c = metrics->C;
  
  cfg->op2_rdma_config.dma_cube_size_w = metrics->W;
  cfg->op2_rdma_config.dma_cube_size_h = metrics->H;
  cfg->op2_rdma_config.dma_cube_size_c = metrics->C;  
  
  cfg->wdma_config.dma_cube_size_w     = metrics->W;
  cfg->wdma_config.dma_cube_size_h     = metrics->H;
  cfg->wdma_config.dma_cube_size_c     = metrics->C;  
  
  cfg->mark = Enable_NotEn;
    
   
  uint32_t elem_size;
  uint32_t tmp_data_use ;
  uint32_t tmp_data_size;   
  
  // CUBE_SIZE in vectors
  cfg->cube_size = (metrics->C/16 + ((metrics->C%16) != 0 ? 1 : 0)) * metrics->W * metrics->H - 1;  
    
  // WDMA --------------------------------------------------------------------------------------------
  ///if(cfg->out_data_type == DataTypeExt_Int32 || cfg->out_data_type == DataTypeExt_Fp32) {
  ///     cfg->wdma_config.dma_en = Enable_NotEn; // new struct ============
  ///}
  ///else cfg->wdma_config.dma_en = Enable_En; // new struct ============
  
  //cfg->wdma_config.dma_en = Enable_En;  // it must be assigned in test main function !!!!!!!!!!!!
  
  if   (cfg->dst_flying == Enable_En) cfg->wdma_config.dma_en = Enable_NotEn; 
  else                                cfg->wdma_config.dma_en = Enable_En   ; 
  
  cfg->wdma_config.dma_ram_type = 0;  // ????????????????????????????????? 

  // dma_data_size definition------------------- 
  if (cfg->out_data_type == DataType_Int8) cfg->wdma_config.dma_data_size = DmaDSize_One_Byte;
  else                                     cfg->wdma_config.dma_data_size = DmaDSize_Two_Byte;
  
  // elem_size definition-------------------
  if   (cfg->wdma_config.dma_data_size == DmaDSize_Two_Byte) tmp_data_size = 2;
  else                                                       tmp_data_size = 1;  
  
  elem_size = tmp_data_size; 

  cfg->wdma_config.dma_frag_size       = 16 * elem_size ;
  cfg->wdma_config.dma_xyz_drct        = DmaXYZDirection_Z;
  
  if (cfg->trace_mode == TraceMode_MPE) { // MPE read mode
     cfg->wdma_config.dma_frag_last_size  = ((metrics->C % 16) + ((metrics->C%16) == 0)*16) * elem_size ;
     cfg->wdma_config.dma_stride_z        = 16                      * elem_size                         ; //coef_z == vector_size * elem_size
     cfg->wdma_config.dma_stride_x        = metrics->C              * elem_size                         ; //coef_x == full_line_z             = full_line_C*elem_size
     cfg->wdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                         ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
     cfg->wdma_config.dma_border_z        = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size   ; //line_size (bytes)               = (Z-1)*elem_size
     cfg->wdma_config.dma_border_x        = (metrics->W - 1) * cfg->wdma_config.dma_stride_x            ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
     cfg->wdma_config.dma_border_y        = (metrics->H - 1) * cfg->wdma_config.dma_stride_y            ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
    
     cfg->wdma_config.dma_box_st_size_x  = 128    - 1 ;
     cfg->wdma_config.dma_box_st_size_y  = 1      - 1 ;
     cfg->wdma_config.dma_box_st_size_z  = 128/16 - 1 ;
     cfg->wdma_config.dma_box_size_x     = 128    - 1 ;
     cfg->wdma_config.dma_box_size_y     = 1      - 1 ;
     cfg->wdma_config.dma_box_size_z     = 128/16 - 1 ;
     cfg->wdma_config.dma_box_offset_x   = 0 ;
     cfg->wdma_config.dma_box_offset_y   = 0 ;
     cfg->wdma_config.dma_box_offset_z   = 0 ;  
  }
  else {  // PPE read mode
     cfg->wdma_config.dma_frag_last_size  = ((metrics->C % 16) + ((metrics->C%16) == 0)*16) * elem_size ;
     cfg->wdma_config.dma_stride_z        = 16                      * elem_size                         ; //coef_z == vector_size * elem_size
     cfg->wdma_config.dma_stride_x        = metrics->C              * elem_size                         ; //coef_x == full_line_z             = full_line_C*elem_size
     cfg->wdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                         ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
     cfg->wdma_config.dma_border_z        = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size   ; //line_size (bytes)               = (Z-1)*elem_size
     cfg->wdma_config.dma_border_x        = (metrics->W - 1) * cfg->wdma_config.dma_stride_x        ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
     cfg->wdma_config.dma_border_y        = (metrics->H - 1) * cfg->wdma_config.dma_stride_y        ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
    
     cfg->wdma_config.dma_box_st_size_x  = metrics->W - 1 ;
     cfg->wdma_config.dma_box_st_size_y  = metrics->H - 1 ;
     cfg->wdma_config.dma_box_st_size_z  = 1 - 1 ;
     cfg->wdma_config.dma_box_size_x     = metrics->W - 1 ;
     cfg->wdma_config.dma_box_size_y     = metrics->H - 1 ;
     cfg->wdma_config.dma_box_size_z     = 1 - 1 ;
     cfg->wdma_config.dma_box_offset_x   = 0 ;
     cfg->wdma_config.dma_box_offset_y   = 0 ;
     cfg->wdma_config.dma_box_offset_z   = 0 ;    
  }


  // SRC_DMA --------------------------------------------------------------------------------------------
  if(cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32) {
    // if 32bit Input Data then Main Channel Works
    cfg->src_rdma_config.dma_en    = Enable_NotEn ; //
    cfg->src_rdma_config.dma_data_use = DmaDUse_Off  ; // 
  }
  else {
    cfg->src_rdma_config.dma_en    = Enable_En    ; //
    cfg->src_rdma_config.dma_data_use = DmaDUse_Mux  ; // 
  }

  cfg->src_rdma_config.dma_ram_type       = 0; // ?????????????????????????????????
  cfg->src_rdma_config.dma_data_mode      = Mode_Element;
  
  if (cfg->in_data_type == DataTypeExt_Int8) cfg->src_rdma_config.dma_data_size = DmaDSize_One_Byte;
  else                                       cfg->src_rdma_config.dma_data_size = DmaDSize_Two_Byte;
  
  
  // elem_size definition-------------------
  if   (cfg->src_rdma_config.dma_data_use == DmaDUse_Both) tmp_data_use  = 2;
  else                                                     tmp_data_use  = 1;

  if   (cfg->src_rdma_config.dma_data_size == DmaDSize_Two_Byte) tmp_data_size = 2;
  else                                                           tmp_data_size = 1;  
  
  elem_size = tmp_data_use * tmp_data_size; 
  
  cfg->src_rdma_config.dma_frag_size       = 16 * elem_size ;
  cfg->src_rdma_config.dma_xyz_drct        = DmaXYZDirection_Z;
  // ----------------
  if (cfg->dst_flying == Enable_En) {
    // for flying testcases only. Linear data read mode. NOT MPE read mode !!!!!!!!!!!! correct only for Cube sizes < 128
    cfg->src_rdma_config.dma_frag_last_size  = metrics->C              * elem_size                 ;
    cfg->src_rdma_config.dma_stride_z        = metrics->C              * elem_size                 ; //coef_z == vector_size * elem_size
    cfg->src_rdma_config.dma_stride_x        = metrics->C              * elem_size                 ; //coef_x == full_line_z             = full_line_C*elem_size
    cfg->src_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                 ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
    cfg->src_rdma_config.dma_border_z        = 0                                                   ; //line_size (bytes)               = (Z-1)*elem_size
    cfg->src_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->src_rdma_config.dma_stride_x ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
    cfg->src_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->src_rdma_config.dma_stride_y ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
  
    cfg->src_rdma_config.dma_box_st_size_x   = metrics->W - 1 ;
    cfg->src_rdma_config.dma_box_st_size_y   = metrics->H - 1 ;
    cfg->src_rdma_config.dma_box_st_size_z   = 1 - 1 ;
    cfg->src_rdma_config.dma_box_size_x      = metrics->W - 1 ;
    cfg->src_rdma_config.dma_box_size_y      = metrics->H - 1 ;
    cfg->src_rdma_config.dma_box_size_z      = 1 - 1 ;
    cfg->src_rdma_config.dma_box_offset_x    = 0 ;
    cfg->src_rdma_config.dma_box_offset_y    = 0 ;
    cfg->src_rdma_config.dma_box_offset_z    = 0 ;
  
  }
  else {  // MPE read mode
     if (cfg->trace_mode == TraceMode_MPE) { // MPE read mode
       cfg->src_rdma_config.dma_frag_last_size  = ((metrics->C % 16) + ((metrics->C%16) == 0)*16) * elem_size ;
       cfg->src_rdma_config.dma_stride_z        = 16                      * elem_size                         ; //coef_z == vector_size * elem_size
       cfg->src_rdma_config.dma_stride_x        = metrics->C              * elem_size                         ; //coef_x == full_line_z             = full_line_C*elem_size
       cfg->src_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                         ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
       cfg->src_rdma_config.dma_border_z        = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size   ; //line_size (bytes)               = (Z-1)*elem_size
       cfg->src_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->src_rdma_config.dma_stride_x         ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
       cfg->src_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->src_rdma_config.dma_stride_y         ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
       cfg->src_rdma_config.dma_box_st_size_x  = 128    - 1 ;
       cfg->src_rdma_config.dma_box_st_size_y  = 1      - 1 ;
       cfg->src_rdma_config.dma_box_st_size_z  = 128/16 - 1 ;
       cfg->src_rdma_config.dma_box_size_x     = 128    - 1 ;
       cfg->src_rdma_config.dma_box_size_y     = 1      - 1 ;
       cfg->src_rdma_config.dma_box_size_z     = 128/16 - 1 ;
       cfg->src_rdma_config.dma_box_offset_x   = 0 ;
       cfg->src_rdma_config.dma_box_offset_y   = 0 ;
       cfg->src_rdma_config.dma_box_offset_z   = 0 ;  
    }
    else {  // PPE read mode
       cfg->src_rdma_config.dma_frag_last_size  = ((metrics->C % 16) + ((metrics->C%16) == 0)*16) * elem_size ;
       cfg->src_rdma_config.dma_stride_z        = 16                      * elem_size                         ; //coef_z == vector_size * elem_size
       cfg->src_rdma_config.dma_stride_x        = metrics->C              * elem_size                         ; //coef_x == full_line_z             = full_line_C*elem_size
       cfg->src_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                         ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
       cfg->src_rdma_config.dma_border_z        = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size   ; //line_size (bytes)               = (Z-1)*elem_size
       cfg->src_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->src_rdma_config.dma_stride_x         ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
       cfg->src_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->src_rdma_config.dma_stride_y         ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
       cfg->src_rdma_config.dma_box_st_size_x  = metrics->W - 1 ;
       cfg->src_rdma_config.dma_box_st_size_y  = metrics->H - 1 ;
       cfg->src_rdma_config.dma_box_st_size_z  = 1 - 1 ;
       cfg->src_rdma_config.dma_box_size_x     = metrics->W - 1 ;
       cfg->src_rdma_config.dma_box_size_y     = metrics->H - 1 ;
       cfg->src_rdma_config.dma_box_size_z     = 1 - 1 ;
       cfg->src_rdma_config.dma_box_offset_x   = 0 ;
       cfg->src_rdma_config.dma_box_offset_y   = 0 ;
       cfg->src_rdma_config.dma_box_offset_z   = 0 ;    
    }
  }
  
  // OP0_RDMA -------------------------------------------------------------------------------------------- 
  if((cfg->op0_config.alu_en == Enable_En && cfg->op0_config.alu_mode != Mode_Unitary) ||
     (cfg->op0_config.mux_en == Enable_En && cfg->op0_config.mux_mode != Mode_Unitary)  ) { // if Some Of Operands Enabled And Not A Single Value
    cfg->op0_rdma_config.dma_en = Enable_En; // new struct ============
  }
  else {
    cfg->op0_rdma_config.dma_en = Enable_NotEn; // new struct ============
  }
  
  cfg->op0_rdma_config.dma_ram_type = 0;  // ?????????????????????????????????
 
  // dma_data_mode parameter definition ----
  if      (cfg->op0_config.alu_en == Enable_En && cfg->op0_config.alu_mode != Mode_Unitary) cfg->op0_rdma_config.dma_data_mode = cfg->op0_config.alu_mode;
  else if (cfg->op0_config.mux_en == Enable_En && cfg->op0_config.mux_mode != Mode_Unitary) cfg->op0_rdma_config.dma_data_mode = cfg->op0_config.mux_mode;    
  else                                                                                      cfg->op0_rdma_config.dma_data_mode = Mode_Unitary;
  
  if ((cfg->op0_config.alu_en == Enable_En && cfg->op0_config.alu_mode != Mode_Unitary) &&    // checking mode differences ????????????????????????????????????????
      (cfg->op0_config.mux_en == Enable_En && cfg->op0_config.mux_mode != Mode_Unitary) &&
      (cfg->op0_config.mux_mode != cfg->op0_config.alu_mode)                                ) {
    rumboot_printf("ERROR! parameters alu_mode & mux_mode must be equal..\n");
  }    
  
  // dma_data_size parameter definition ----
  if (cfg->op0_config.coef_type == DataType_Int8) cfg->op0_rdma_config.dma_data_size = DmaDSize_One_Byte;
  else                                            cfg->op0_rdma_config.dma_data_size = DmaDSize_Two_Byte;  
  
  // dma_data_use parameter definition ----
  if      ((cfg->op0_config.alu_en == Enable_En && cfg->op0_config.alu_mode != Mode_Unitary) &&
           (cfg->op0_config.mux_en == Enable_En && cfg->op0_config.mux_mode != Mode_Unitary) ){
    cfg->op0_rdma_config.dma_data_use = DmaDUse_Both ;
  }
  else if (( cfg->op0_config.alu_en == Enable_En && cfg->op0_config.alu_mode != Mode_Unitary) &&
          (( cfg->op0_config.mux_en == Enable_En && cfg->op0_config.mux_mode == Mode_Unitary) ||
           ( cfg->op0_config.mux_en != Enable_En)                                           ) ){
    cfg->op0_rdma_config.dma_data_use = DmaDUse_Alu ;
  }
  else if (( cfg->op0_config.mux_en == Enable_En && cfg->op0_config.mux_mode != Mode_Unitary) &&
          (( cfg->op0_config.alu_en == Enable_En && cfg->op0_config.alu_mode == Mode_Unitary) ||
           ( cfg->op0_config.alu_en != Enable_En)                                           ) ){
    cfg->op0_rdma_config.dma_data_use = DmaDUse_Mux ;
  }  
  else cfg->op0_rdma_config.dma_data_use = DmaDUse_Off ;

  
  // elem_size definition-------------------
  if   (cfg->op0_rdma_config.dma_data_use  == DmaDUse_Both) tmp_data_use  = 2;
  else                                                      tmp_data_use  = 1;

  if   (cfg->op0_rdma_config.dma_data_size == DmaDSize_Two_Byte) tmp_data_size = 2;
  else                                                           tmp_data_size = 1;  
  
  elem_size = tmp_data_use * tmp_data_size; 
  
  cfg->op0_rdma_config.dma_frag_size = 16 * elem_size ;
  //------------
  cfg->op0_rdma_config.dma_xyz_drct = DmaXYZDirection_Z;
  if (cfg->dst_flying == Enable_En) {
    // for flying testcases only. Linear data read mode. NOT MPE read mode !!!!!!!!!!!! correct only for Cube sizes < 128
    cfg->op0_rdma_config.dma_frag_last_size  = metrics->C              * elem_size                 ;
    cfg->op0_rdma_config.dma_stride_z        = metrics->C              * elem_size                 ; //coef_z == vector_size * elem_size
    cfg->op0_rdma_config.dma_stride_x        = metrics->C              * elem_size                 ; //coef_x == full_line_z             = full_line_C*elem_size
    cfg->op0_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                 ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
    cfg->op0_rdma_config.dma_border_z        = 0                                                   ; //line_size (bytes)               = (Z-1)*elem_size
    cfg->op0_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->op0_rdma_config.dma_stride_x ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
    cfg->op0_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->op0_rdma_config.dma_stride_y ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
    
    cfg->op0_rdma_config.dma_box_st_size_x   = metrics->W - 1 ;
    cfg->op0_rdma_config.dma_box_st_size_y   = metrics->H - 1 ;
    cfg->op0_rdma_config.dma_box_st_size_z   = 1 - 1 ;
    cfg->op0_rdma_config.dma_box_size_x      = metrics->W - 1 ;
    cfg->op0_rdma_config.dma_box_size_y      = metrics->H - 1 ;
    cfg->op0_rdma_config.dma_box_size_z      = 1 - 1 ;
    cfg->op0_rdma_config.dma_box_offset_x    = 0 ;
    cfg->op0_rdma_config.dma_box_offset_y    = 0 ;
    cfg->op0_rdma_config.dma_box_offset_z    = 0 ;  
  }
  else {  
    if (cfg->trace_mode == TraceMode_MPE) { // MPE read mode
      if (cfg->op0_rdma_config.dma_data_mode == Mode_Channel) {
         cfg->op0_rdma_config.dma_xyz_drct = DmaXYZDirection_X;
         cfg->op0_rdma_config.dma_frag_last_size  = metrics->C * elem_size ; 
         cfg->op0_rdma_config.dma_stride_z        = 0                      ; //coef_z == vector_size * elem_size
         cfg->op0_rdma_config.dma_stride_x        = 0                      ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op0_rdma_config.dma_stride_y        = 0                      ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
         cfg->op0_rdma_config.dma_border_z        = 0                      ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op0_rdma_config.dma_border_x        = 0                      ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op0_rdma_config.dma_border_y        = 0                      ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
         
         cfg->op0_rdma_config.dma_box_st_size_x   = 0 ;
         cfg->op0_rdma_config.dma_box_st_size_y   = 0 ;
         cfg->op0_rdma_config.dma_box_st_size_z   = 0 ;
         cfg->op0_rdma_config.dma_box_size_x      = 0 ;
         cfg->op0_rdma_config.dma_box_size_y      = 0 ;
         cfg->op0_rdma_config.dma_box_size_z      = 0 ;
         cfg->op0_rdma_config.dma_box_offset_x    = 0 ;
         cfg->op0_rdma_config.dma_box_offset_y    = 0 ;
         cfg->op0_rdma_config.dma_box_offset_z    = 0 ; 
      
      }
      else {
         cfg->op0_rdma_config.dma_frag_last_size  = ((metrics->C % 16) + ((metrics->C%16) == 0)*16) * elem_size ;
         cfg->op0_rdma_config.dma_stride_z        = 16                      * elem_size                         ; //coef_z == vector_size * elem_size
         cfg->op0_rdma_config.dma_stride_x        = metrics->C              * elem_size                         ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op0_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                         ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
         cfg->op0_rdma_config.dma_border_z        = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size   ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op0_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->op0_rdma_config.dma_stride_x         ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op0_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->op0_rdma_config.dma_stride_y         ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
         cfg->op0_rdma_config.dma_box_st_size_x  = 128    - 1 ;
         cfg->op0_rdma_config.dma_box_st_size_y  = 1      - 1 ;
         cfg->op0_rdma_config.dma_box_st_size_z  = 128/16 - 1 ;
         cfg->op0_rdma_config.dma_box_size_x     = 128    - 1 ;
         cfg->op0_rdma_config.dma_box_size_y     = 1      - 1 ;
         cfg->op0_rdma_config.dma_box_size_z     = 128/16 - 1 ;
         cfg->op0_rdma_config.dma_box_offset_x   = 0 ;
         cfg->op0_rdma_config.dma_box_offset_y   = 0 ;
         cfg->op0_rdma_config.dma_box_offset_z   = 0 ;   
      }     
    }
    else { // PPE read mode
      if (cfg->op0_rdma_config.dma_data_mode == Mode_Channel) {
         cfg->op0_rdma_config.dma_xyz_drct = DmaXYZDirection_X;
         cfg->op0_rdma_config.dma_frag_last_size  = metrics->C * elem_size ; 
         cfg->op0_rdma_config.dma_stride_z        = 0                      ; //coef_z == vector_size * elem_size
         cfg->op0_rdma_config.dma_stride_x        = 0                      ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op0_rdma_config.dma_stride_y        = 0                      ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
         cfg->op0_rdma_config.dma_border_z        = 0                      ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op0_rdma_config.dma_border_x        = 0                      ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op0_rdma_config.dma_border_y        = 0                      ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
         
         cfg->op0_rdma_config.dma_box_st_size_x   = 0 ;
         cfg->op0_rdma_config.dma_box_st_size_y   = 0 ;
         cfg->op0_rdma_config.dma_box_st_size_z   = 0 ;
         cfg->op0_rdma_config.dma_box_size_x      = 0 ;
         cfg->op0_rdma_config.dma_box_size_y      = 0 ;
         cfg->op0_rdma_config.dma_box_size_z      = 0 ;
         cfg->op0_rdma_config.dma_box_offset_x    = 0 ;
         cfg->op0_rdma_config.dma_box_offset_y    = 0 ;
         cfg->op0_rdma_config.dma_box_offset_z    = 0 ; 
      }
      else {
         cfg->op0_rdma_config.dma_frag_last_size  = ((metrics->C % 16) + ((metrics->C%16) == 0)*16) * elem_size ;
         cfg->op0_rdma_config.dma_stride_z        = 16                      * elem_size                         ; //coef_z == vector_size * elem_size
         cfg->op0_rdma_config.dma_stride_x        = metrics->C              * elem_size                         ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op0_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                         ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
         cfg->op0_rdma_config.dma_border_z        = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size   ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op0_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->op0_rdma_config.dma_stride_x         ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op0_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->op0_rdma_config.dma_stride_y         ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
         cfg->op0_rdma_config.dma_box_st_size_x  = metrics->W - 1 ;
         cfg->op0_rdma_config.dma_box_st_size_y  = metrics->H - 1 ;
         cfg->op0_rdma_config.dma_box_st_size_z  = 1 - 1 ;
         cfg->op0_rdma_config.dma_box_size_x     = metrics->W - 1 ;
         cfg->op0_rdma_config.dma_box_size_y     = metrics->H - 1 ;
         cfg->op0_rdma_config.dma_box_size_z     = 1 - 1 ;
         cfg->op0_rdma_config.dma_box_offset_x   = 0 ;
         cfg->op0_rdma_config.dma_box_offset_y   = 0 ;
         cfg->op0_rdma_config.dma_box_offset_z   = 0 ;
      }        
    }
  }

  // OP1_RDMA -------------------------------------------------------------------------------------------- 
  if((cfg->op1_config.alu_en == Enable_En && cfg->op1_config.alu_mode != Mode_Unitary) ||
     (cfg->op1_config.mux_en == Enable_En && cfg->op1_config.mux_mode != Mode_Unitary)  ) { // if Some Of Operands Enabled And Not A Single Value
    cfg->op1_rdma_config.dma_en = Enable_En; // new struct ============
  }
  else {
    cfg->op1_rdma_config.dma_en = Enable_NotEn; // new struct ============
  }
  
  cfg->op1_rdma_config.dma_ram_type = 0;  // ?????????????????????????????????
 
  // dma_data_mode parameter definition ----
  if      (cfg->op1_config.alu_en == Enable_En && cfg->op1_config.alu_mode != Mode_Unitary) cfg->op1_rdma_config.dma_data_mode = cfg->op1_config.alu_mode;
  else if (cfg->op1_config.mux_en == Enable_En && cfg->op1_config.mux_mode != Mode_Unitary) cfg->op1_rdma_config.dma_data_mode = cfg->op1_config.mux_mode; 
  else                                                                                      cfg->op1_rdma_config.dma_data_mode = Mode_Unitary;
  
  if ((cfg->op1_config.alu_en == Enable_En && cfg->op1_config.alu_mode != Mode_Unitary) &&    // checking mode differences ????????????????????????????????????????
      (cfg->op1_config.mux_en == Enable_En && cfg->op1_config.mux_mode != Mode_Unitary) &&
      (cfg->op1_config.mux_mode != cfg->op1_config.alu_mode)                                ) {
    rumboot_printf("ERROR! parameters alu_mode & mux_mode must be equal..\n");
  }    
  
  // dma_data_size parameter definition ----
  if (cfg->op1_config.coef_type == DataType_Int8) cfg->op1_rdma_config.dma_data_size = DmaDSize_One_Byte;
  else                                            cfg->op1_rdma_config.dma_data_size = DmaDSize_Two_Byte;  

  // dma_data_use parameter definition ----
  if      ((cfg->op1_config.alu_en == Enable_En && cfg->op1_config.alu_mode != Mode_Unitary) &&
           (cfg->op1_config.mux_en == Enable_En && cfg->op1_config.mux_mode != Mode_Unitary) ){
    cfg->op1_rdma_config.dma_data_use = DmaDUse_Both ;
  }
  else if (( cfg->op1_config.alu_en == Enable_En && cfg->op1_config.alu_mode != Mode_Unitary) &&
          (( cfg->op1_config.mux_en == Enable_En && cfg->op1_config.mux_mode == Mode_Unitary) ||
           ( cfg->op1_config.mux_en != Enable_En)                                           ) ){
    cfg->op1_rdma_config.dma_data_use = DmaDUse_Alu ;
  }
  else if (( cfg->op1_config.mux_en == Enable_En && cfg->op1_config.mux_mode != Mode_Unitary) &&
          (( cfg->op1_config.alu_en == Enable_En && cfg->op1_config.alu_mode == Mode_Unitary) ||
           ( cfg->op1_config.alu_en != Enable_En)                                           ) ){
    cfg->op1_rdma_config.dma_data_use = DmaDUse_Mux ;
  }  
  else cfg->op1_rdma_config.dma_data_use = DmaDUse_Off ;
  
  // elem_size definition-------------------
  if   (cfg->op1_rdma_config.dma_data_use == DmaDUse_Both) tmp_data_use  = 2;
  else                                                     tmp_data_use  = 1;

  if   (cfg->op1_rdma_config.dma_data_size == DmaDSize_Two_Byte) tmp_data_size = 2;
  else                                                           tmp_data_size = 1;  
  
  elem_size = tmp_data_use * tmp_data_size; 

  cfg->op1_rdma_config.dma_frag_size = 16 * elem_size ;
  //------------
  cfg->op1_rdma_config.dma_xyz_drct = DmaXYZDirection_Z;
  if (cfg->dst_flying == Enable_En) {
    // for flying testcases only. Linear data read mode. NOT MPE read mode !!!!!!!!!!!! correct only for Cube sizes < 128
    cfg->op1_rdma_config.dma_frag_last_size  = metrics->C              * elem_size                 ;
    cfg->op1_rdma_config.dma_stride_z        = metrics->C              * elem_size                 ; //coef_z == vector_size * elem_size
    cfg->op1_rdma_config.dma_stride_x        = metrics->C              * elem_size                 ; //coef_x == full_line_z             = full_line_C*elem_size
    cfg->op1_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                 ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
    cfg->op1_rdma_config.dma_border_z        = 0                                                   ; //line_size (bytes)               = (Z-1)*elem_size
    cfg->op1_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->op1_rdma_config.dma_stride_x ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
    cfg->op1_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->op1_rdma_config.dma_stride_y ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
    
    cfg->op1_rdma_config.dma_box_st_size_x   = metrics->W - 1 ;
    cfg->op1_rdma_config.dma_box_st_size_y   = metrics->H - 1 ;
    cfg->op1_rdma_config.dma_box_st_size_z   = 1 - 1 ;
    cfg->op1_rdma_config.dma_box_size_x      = metrics->W - 1 ;
    cfg->op1_rdma_config.dma_box_size_y      = metrics->H - 1 ;
    cfg->op1_rdma_config.dma_box_size_z      = 1 - 1 ;
    cfg->op1_rdma_config.dma_box_offset_x    = 0 ;
    cfg->op1_rdma_config.dma_box_offset_y    = 0 ;
    cfg->op1_rdma_config.dma_box_offset_z    = 0 ;
  
  }
  else {  
    if (cfg->trace_mode == TraceMode_MPE) { // MPE read mode
      if (cfg->op1_rdma_config.dma_data_mode == Mode_Channel) {
         cfg->op1_rdma_config.dma_xyz_drct = DmaXYZDirection_X;
         cfg->op1_rdma_config.dma_frag_last_size  = metrics->C * elem_size ; 
         cfg->op1_rdma_config.dma_stride_z        = 0                      ; //coef_z == vector_size * elem_size
         cfg->op1_rdma_config.dma_stride_x        = 0                      ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op1_rdma_config.dma_stride_y        = 0                      ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
         cfg->op1_rdma_config.dma_border_z        = 0                      ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op1_rdma_config.dma_border_x        = 0                      ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op1_rdma_config.dma_border_y        = 0                      ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
         
         cfg->op1_rdma_config.dma_box_st_size_x   = 0 ;
         cfg->op1_rdma_config.dma_box_st_size_y   = 0 ;
         cfg->op1_rdma_config.dma_box_st_size_z   = 0 ;
         cfg->op1_rdma_config.dma_box_size_x      = 0 ;
         cfg->op1_rdma_config.dma_box_size_y      = 0 ;
         cfg->op1_rdma_config.dma_box_size_z      = 0 ;
         cfg->op1_rdma_config.dma_box_offset_x    = 0 ;
         cfg->op1_rdma_config.dma_box_offset_y    = 0 ;
         cfg->op1_rdma_config.dma_box_offset_z    = 0 ; 
      
      }
      else {
         cfg->op1_rdma_config.dma_frag_last_size  = ((metrics->C % 16) + ((metrics->C%16) == 0)*16) * elem_size ;
         cfg->op1_rdma_config.dma_stride_z        = 16                      * elem_size                         ; //coef_z == vector_size * elem_size
         cfg->op1_rdma_config.dma_stride_x        = metrics->C              * elem_size                         ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op1_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                         ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
         cfg->op1_rdma_config.dma_border_z        = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size   ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op1_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->op1_rdma_config.dma_stride_x         ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op1_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->op1_rdma_config.dma_stride_y         ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
         cfg->op1_rdma_config.dma_box_st_size_x  = 128    - 1 ;
         cfg->op1_rdma_config.dma_box_st_size_y  = 1      - 1 ;
         cfg->op1_rdma_config.dma_box_st_size_z  = 128/16 - 1 ;
         cfg->op1_rdma_config.dma_box_size_x     = 128    - 1 ;
         cfg->op1_rdma_config.dma_box_size_y     = 1      - 1 ;
         cfg->op1_rdma_config.dma_box_size_z     = 128/16 - 1 ;
         cfg->op1_rdma_config.dma_box_offset_x   = 0 ;
         cfg->op1_rdma_config.dma_box_offset_y   = 0 ;
         cfg->op1_rdma_config.dma_box_offset_z   = 0 ;   
      }     
    }
    else { // PPE read mode
      if (cfg->op1_rdma_config.dma_data_mode == Mode_Channel) {
         cfg->op1_rdma_config.dma_xyz_drct = DmaXYZDirection_X;
         cfg->op1_rdma_config.dma_frag_last_size  = metrics->C * elem_size ; 
         cfg->op1_rdma_config.dma_stride_z        = 0                      ; //coef_z == vector_size * elem_size
         cfg->op1_rdma_config.dma_stride_x        = 0                      ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op1_rdma_config.dma_stride_y        = 0                      ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
         cfg->op1_rdma_config.dma_border_z        = 0                      ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op1_rdma_config.dma_border_x        = 0                      ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op1_rdma_config.dma_border_y        = 0                      ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
         
         cfg->op1_rdma_config.dma_box_st_size_x   = 0 ;
         cfg->op1_rdma_config.dma_box_st_size_y   = 0 ;
         cfg->op1_rdma_config.dma_box_st_size_z   = 0 ;
         cfg->op1_rdma_config.dma_box_size_x      = 0 ;
         cfg->op1_rdma_config.dma_box_size_y      = 0 ;
         cfg->op1_rdma_config.dma_box_size_z      = 0 ;
         cfg->op1_rdma_config.dma_box_offset_x    = 0 ;
         cfg->op1_rdma_config.dma_box_offset_y    = 0 ;
         cfg->op1_rdma_config.dma_box_offset_z    = 0 ; 
      }
      else {
         cfg->op1_rdma_config.dma_frag_last_size  = ((metrics->C % 16) + ((metrics->C%16) == 0)*16) * elem_size ;
         cfg->op1_rdma_config.dma_stride_z        = 16                      * elem_size                         ; //coef_z == vector_size * elem_size
         cfg->op1_rdma_config.dma_stride_x        = metrics->C              * elem_size                         ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op1_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                         ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
         cfg->op1_rdma_config.dma_border_z        = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size   ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op1_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->op1_rdma_config.dma_stride_x         ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op1_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->op1_rdma_config.dma_stride_y         ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
         cfg->op1_rdma_config.dma_box_st_size_x  = metrics->W - 1 ;
         cfg->op1_rdma_config.dma_box_st_size_y  = metrics->H - 1 ;
         cfg->op1_rdma_config.dma_box_st_size_z  = 1 - 1 ;
         cfg->op1_rdma_config.dma_box_size_x     = metrics->W - 1 ;
         cfg->op1_rdma_config.dma_box_size_y     = metrics->H - 1 ;
         cfg->op1_rdma_config.dma_box_size_z     = 1 - 1 ;
         cfg->op1_rdma_config.dma_box_offset_x   = 0 ;
         cfg->op1_rdma_config.dma_box_offset_y   = 0 ;
         cfg->op1_rdma_config.dma_box_offset_z   = 0 ;
      }        
    }
  }

  // OP2_RDMA -------------------------------------------------------------------------------------------- 
  if((cfg->op2_config.alu_en == Enable_En && cfg->op2_config.alu_mode != Mode_Unitary) ||
     (cfg->op2_config.mux_en == Enable_En && cfg->op2_config.mux_mode != Mode_Unitary)  ) { // if Some Of Operands Enabled And Not A Single Value
    cfg->op2_rdma_config.dma_en = Enable_En; // new struct ============
  }
  else {
    cfg->op2_rdma_config.dma_en = Enable_NotEn; // new struct ============
  }
  
  cfg->op2_rdma_config.dma_ram_type = 0;  // ?????????????????????????????????
 
  // dma_data_mode parameter definition ----
  if      (cfg->op2_config.alu_en == Enable_En && cfg->op2_config.alu_mode != Mode_Unitary) cfg->op2_rdma_config.dma_data_mode = cfg->op2_config.alu_mode;
  else if (cfg->op2_config.mux_en == Enable_En && cfg->op2_config.mux_mode != Mode_Unitary) cfg->op2_rdma_config.dma_data_mode = cfg->op2_config.mux_mode;    
  else                                                                                      cfg->op2_rdma_config.dma_data_mode = Mode_Unitary;
  
  if ((cfg->op2_config.alu_en == Enable_En && cfg->op2_config.alu_mode != Mode_Unitary) &&    // checking mode differences ????????????????????????????????????????
      (cfg->op2_config.mux_en == Enable_En && cfg->op2_config.mux_mode != Mode_Unitary) &&
      (cfg->op2_config.mux_mode != cfg->op2_config.alu_mode)                                ) {
    rumboot_printf("ERROR! parameters alu_mode & mux_mode must be equal..\n");
  }    
  
  // dma_data_size parameter definition ----
  if (cfg->op2_config.coef_type == DataType_Int8) cfg->op2_rdma_config.dma_data_size = DmaDSize_One_Byte;
  else                                            cfg->op2_rdma_config.dma_data_size = DmaDSize_Two_Byte;  

  // dma_data_use parameter definition ----
  if      ((cfg->op2_config.alu_en == Enable_En && cfg->op2_config.alu_mode != Mode_Unitary) &&
           (cfg->op2_config.mux_en == Enable_En && cfg->op2_config.mux_mode != Mode_Unitary) ){
    cfg->op2_rdma_config.dma_data_use = DmaDUse_Both ;
  }
  else if (( cfg->op2_config.alu_en == Enable_En && cfg->op2_config.alu_mode != Mode_Unitary) &&
          (( cfg->op2_config.mux_en == Enable_En && cfg->op2_config.mux_mode == Mode_Unitary) ||
           ( cfg->op2_config.mux_en != Enable_En)                                           ) ){
    cfg->op2_rdma_config.dma_data_use = DmaDUse_Alu ;
  }
  else if (( cfg->op2_config.mux_en == Enable_En && cfg->op2_config.mux_mode != Mode_Unitary) &&
          (( cfg->op2_config.alu_en == Enable_En && cfg->op2_config.alu_mode == Mode_Unitary) ||
           ( cfg->op2_config.alu_en != Enable_En)                                           ) ){
    cfg->op2_rdma_config.dma_data_use = DmaDUse_Mux ;
  }  
  else cfg->op2_rdma_config.dma_data_use = DmaDUse_Off ;

  // elem_size definition-------------------
  if   (cfg->op2_rdma_config.dma_data_use == DmaDUse_Both) tmp_data_use  = 2;
  else                                                     tmp_data_use  = 1;

  if   (cfg->op2_rdma_config.dma_data_size == DmaDSize_Two_Byte) tmp_data_size = 2;
  else                                                           tmp_data_size = 1;  
  
  elem_size = tmp_data_use * tmp_data_size; 
  
  cfg->op2_rdma_config.dma_frag_size = 16 * elem_size ;
  //------------
  cfg->op2_rdma_config.dma_xyz_drct = DmaXYZDirection_Z;
  if (cfg->dst_flying == Enable_En) {
    // for flying testcases only. Linear data read mode. NOT MPE read mode !!!!!!!!!!!! correct only for Cube sizes < 128
    cfg->op2_rdma_config.dma_frag_last_size  = metrics->C              * elem_size                 ;
    cfg->op2_rdma_config.dma_stride_z        = metrics->C              * elem_size                 ; //coef_z == vector_size * elem_size
    cfg->op2_rdma_config.dma_stride_x        = metrics->C              * elem_size                 ; //coef_x == full_line_z             = full_line_C*elem_size
    cfg->op2_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                 ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
    cfg->op2_rdma_config.dma_border_z        = 0                                                   ; //line_size (bytes)               = (Z-1)*elem_size
    cfg->op2_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->op2_rdma_config.dma_stride_x ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
    cfg->op2_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->op2_rdma_config.dma_stride_y ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
  
    cfg->op2_rdma_config.dma_box_st_size_x   = metrics->W - 1 ;
    cfg->op2_rdma_config.dma_box_st_size_y   = metrics->H - 1 ;
    cfg->op2_rdma_config.dma_box_st_size_z   = 1 - 1 ;
    cfg->op2_rdma_config.dma_box_size_x      = metrics->W - 1 ;
    cfg->op2_rdma_config.dma_box_size_y      = metrics->H - 1 ;
    cfg->op2_rdma_config.dma_box_size_z      = 1 - 1 ;
    cfg->op2_rdma_config.dma_box_offset_x    = 0 ;
    cfg->op2_rdma_config.dma_box_offset_y    = 0 ;
    cfg->op2_rdma_config.dma_box_offset_z    = 0 ;
  
  }
  else {  
    if (cfg->trace_mode == TraceMode_MPE) { // MPE read mode
      if (cfg->op2_rdma_config.dma_data_mode == Mode_Channel) {
         cfg->op2_rdma_config.dma_xyz_drct = DmaXYZDirection_X;
         cfg->op2_rdma_config.dma_frag_last_size  = metrics->C * elem_size ; 
         cfg->op2_rdma_config.dma_stride_z        = 0                      ; //coef_z == vector_size * elem_size
         cfg->op2_rdma_config.dma_stride_x        = 0                      ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op2_rdma_config.dma_stride_y        = 0                      ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
         cfg->op2_rdma_config.dma_border_z        = 0                      ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op2_rdma_config.dma_border_x        = 0                      ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op2_rdma_config.dma_border_y        = 0                      ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
         
         cfg->op2_rdma_config.dma_box_st_size_x   = 0 ;
         cfg->op2_rdma_config.dma_box_st_size_y   = 0 ;
         cfg->op2_rdma_config.dma_box_st_size_z   = 0 ;
         cfg->op2_rdma_config.dma_box_size_x      = 0 ;
         cfg->op2_rdma_config.dma_box_size_y      = 0 ;
         cfg->op2_rdma_config.dma_box_size_z      = 0 ;
         cfg->op2_rdma_config.dma_box_offset_x    = 0 ;
         cfg->op2_rdma_config.dma_box_offset_y    = 0 ;
         cfg->op2_rdma_config.dma_box_offset_z    = 0 ; 
      
      }
      else {
         cfg->op2_rdma_config.dma_frag_last_size  = ((metrics->C % 16) + ((metrics->C%16) == 0)*16) * elem_size ;
         cfg->op2_rdma_config.dma_stride_z        = 16                      * elem_size                         ; //coef_z == vector_size * elem_size
         cfg->op2_rdma_config.dma_stride_x        = metrics->C              * elem_size                         ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op2_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                         ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
         cfg->op2_rdma_config.dma_border_z        = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size   ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op2_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->op2_rdma_config.dma_stride_x         ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op2_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->op2_rdma_config.dma_stride_y         ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
         cfg->op2_rdma_config.dma_box_st_size_x  = 128    - 1 ;
         cfg->op2_rdma_config.dma_box_st_size_y  = 1      - 1 ;
         cfg->op2_rdma_config.dma_box_st_size_z  = 128/16 - 1 ;
         cfg->op2_rdma_config.dma_box_size_x     = 128    - 1 ;
         cfg->op2_rdma_config.dma_box_size_y     = 1      - 1 ;
         cfg->op2_rdma_config.dma_box_size_z     = 128/16 - 1 ;
         cfg->op2_rdma_config.dma_box_offset_x   = 0 ;
         cfg->op2_rdma_config.dma_box_offset_y   = 0 ;
         cfg->op2_rdma_config.dma_box_offset_z   = 0 ;   
      }     
    }
    else { // PPE read mode
      if (cfg->op2_rdma_config.dma_data_mode == Mode_Channel) {
         cfg->op2_rdma_config.dma_xyz_drct = DmaXYZDirection_X;
         cfg->op2_rdma_config.dma_frag_last_size  = metrics->C * elem_size ; 
         cfg->op2_rdma_config.dma_stride_z        = 0                      ; //coef_z == vector_size * elem_size
         cfg->op2_rdma_config.dma_stride_x        = 0                      ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op2_rdma_config.dma_stride_y        = 0                      ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size   
         cfg->op2_rdma_config.dma_border_z        = 0                      ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op2_rdma_config.dma_border_x        = 0                      ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op2_rdma_config.dma_border_y        = 0                      ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
         
         cfg->op2_rdma_config.dma_box_st_size_x   = 0 ;
         cfg->op2_rdma_config.dma_box_st_size_y   = 0 ;
         cfg->op2_rdma_config.dma_box_st_size_z   = 0 ;
         cfg->op2_rdma_config.dma_box_size_x      = 0 ;
         cfg->op2_rdma_config.dma_box_size_y      = 0 ;
         cfg->op2_rdma_config.dma_box_size_z      = 0 ;
         cfg->op2_rdma_config.dma_box_offset_x    = 0 ;
         cfg->op2_rdma_config.dma_box_offset_y    = 0 ;
         cfg->op2_rdma_config.dma_box_offset_z    = 0 ; 
      }
      else {
         cfg->op2_rdma_config.dma_frag_last_size  = ((metrics->C % 16) + ((metrics->C%16) == 0)*16) * elem_size ;
         cfg->op2_rdma_config.dma_stride_z        = 16                      * elem_size                         ; //coef_z == vector_size * elem_size
         cfg->op2_rdma_config.dma_stride_x        = metrics->C              * elem_size                         ; //coef_x == full_line_z             = full_line_C*elem_size
         cfg->op2_rdma_config.dma_stride_y        = metrics->C * metrics->W * elem_size                         ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
         cfg->op2_rdma_config.dma_border_z        = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size   ; //line_size (bytes)               = (Z-1)*elem_size
         cfg->op2_rdma_config.dma_border_x        = (metrics->W - 1) * cfg->op2_rdma_config.dma_stride_x         ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
         cfg->op2_rdma_config.dma_border_y        = (metrics->H - 1) * cfg->op2_rdma_config.dma_stride_y         ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
      
         cfg->op2_rdma_config.dma_box_st_size_x  = metrics->W - 1 ;
         cfg->op2_rdma_config.dma_box_st_size_y  = metrics->H - 1 ;
         cfg->op2_rdma_config.dma_box_st_size_z  = 1 - 1 ;
         cfg->op2_rdma_config.dma_box_size_x     = metrics->W - 1 ;
         cfg->op2_rdma_config.dma_box_size_y     = metrics->H - 1 ;
         cfg->op2_rdma_config.dma_box_size_z     = 1 - 1 ;
         cfg->op2_rdma_config.dma_box_offset_x   = 0 ;
         cfg->op2_rdma_config.dma_box_offset_y   = 0 ;
         cfg->op2_rdma_config.dma_box_offset_z   = 0 ;
      }        
    }
  } 

  // We Have No Setting That Define If We Run WDMA Or Main Wr Channel
}

// void nu_calc_mpe2vpe_cube_metrics(CubeMetrics* mpe2vpe_metrics,CubeMetrics* cube_metrics,WarrMetrics* warr_metrics) {
//   mpe2vpe_metrics->H = ...;
// }

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

int nu_vpe_check_status_regs(uintptr_t base, VPEStatusRegs* status_regs) {
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
  cfg->rdma.LPXEn=               *ptr;ptr++;
  cfg->rdma.RPXEn=               *ptr;ptr++;
  cfg->rdma.TPYEn=               *ptr;ptr++;
  cfg->rdma.BPYEn=               *ptr;ptr++;
//
  for(int i=0;i<7;i++){
    cfg->rdma.Bias[i].BiasEn=                    *ptr;ptr++;
    cfg->rdma.Bias[i].ThreCtrl=(uint8_t)         *ptr;ptr++;
    cfg->rdma.Bias[i].DecCtrl=(uint8_t)          *ptr;ptr++;
    cfg->rdma.Bias[i].PBSEn=                     *ptr;ptr++;
    cfg->rdma.Bias[i].Bias=                      *ptr;ptr++;
//                                                      ptr++; // Skipped AOffset
    cfg->rdma.Bias[i].AOffset=                   *ptr;ptr++;
    cfg->rdma.Bias[i].CntSha=(uint16_t)          *ptr;ptr++;
    cfg->rdma.Bias[i].CntOffsetEn=               *ptr;ptr++;
    cfg->rdma.Bias[i].CntOffset=(uint8_t)        *ptr;ptr++;
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
    } while(in_param_match && cnt<15);
    
    row_ptr = row_ptr + NU_MPE_DMA_PARAM_TABLE_ROW_SIZE;
  }
  
  if(in_param_match) {
    nu_mpe_load_ma_config_from_table_row(& (cfg->ma_config),&tab_ptr);
    nu_mpe_load_dma_config_from_table_row(& (cfg->dma_d_config),&tab_ptr);
    nu_mpe_load_dma_config_from_table_row(& (cfg->dma_w_config),&tab_ptr);
    res=0;
  }
  
  return res;
}

int nu_mpe_decide_dma_config_trivial(ConfigMPE* cfg, CubeMetrics* cube_metrics, WarrMetrics* warr_metrics) {
  
  uint32_t sizeof_C;
  uint32_t sizeof_C_div128;
  
  uint32_t Hp;
  uint32_t Wp;
  uint32_t Rd;
  uint32_t Sd;
  uint32_t Ho;
  uint32_t Wo;
  
  Hp  = cfg->H+cfg->Tp+cfg->Bp; // H after null padding
  Wp  = cfg->W+cfg->Lp+cfg->Rp; // W after null padding

  Rd  = (cfg->R-1)*cfg->Dr + 1; // R kernel size after dilation
  Sd  = (cfg->S-1)*cfg->Ds + 1; // S kernel size after dilation

  Ho  = (Hp-Rd)/cfg->Sh + 1; // Hout with respect to striding, dilation, null padding, conv
  Wo  = (Wp-Sd)/cfg->Sw + 1; // Wout with respect to striding, dilation, null padding, conv

  
  
    // [C/128] - it is a size in bytes, division with rounding up
  sizeof_C = cfg->C << (cfg->dt == DataType_Int16 || cfg->dt == DataType_Fp16 ? 1 : 0);

  if (sizeof_C > 127) {
    if ((sizeof_C & 0x7F) > 0)  sizeof_C_div128 = (sizeof_C >> 7) + 1;
    else                        sizeof_C_div128 = sizeof_C >> 7;
  }
  else sizeof_C_div128 = 1;


  /*
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
  
  //////////////////////////////
  cfg->dma_d_config.rdma.LPXEn = (cfg->Lp) ? Enable_En : Enable_NotEn;
  cfg->dma_d_config.rdma.RPXEn = (cfg->Rp) ? Enable_En : Enable_NotEn;
  cfg->dma_d_config.rdma.TPYEn = (cfg->Tp) ? Enable_En : Enable_NotEn;
  cfg->dma_d_config.rdma.BPYEn = (cfg->Bp) ? Enable_En : Enable_NotEn;
  
  cfg->dma_d_config.rdma.AOffset = -(cfg->W * sizeof_C * cfg->Tp + sizeof_C * cfg->Lp);
  cfg->dma_d_config.rdma.LPXOffset=-(cfg->W * sizeof_C * cfg->Tp);
  cfg->dma_d_config.rdma.RPXOffset=-(cfg->W * sizeof_C * (cfg->Tp-1));
  cfg->dma_d_config.rdma.TPYOffset= 0 ;
  cfg->dma_d_config.rdma.BPYOffset = cfg->H * cfg->W * sizeof_C;
  cfg->dma_d_config.rdma.CntThresholdSha = sizeof_C>128 ? sizeof_C : 0;
  
  for(int i=0;i<7;i++)
    cfg->dma_d_config.rdma.Bias[i].AOffset = 0;
  */
  
  cfg->dma_d_config.rdma.LPXData=0;
  cfg->dma_d_config.rdma.RPXData=0;
  cfg->dma_d_config.rdma.TPYData=0;
  cfg->dma_d_config.rdma.BPYData=0;
  
  /*
  cfg->dma_d_config.rdma.Bias[6].BiasEn = Enable_NotEn;
  cfg->dma_d_config.rdma.Bias[6].ThreCtrl = 0;
  cfg->dma_d_config.rdma.Bias[6].DecCtrl = 0;
  cfg->dma_d_config.rdma.Bias[6].PBSEn = Enable_En;
  cfg->dma_d_config.rdma.Bias[6].Bias = 0;
  cfg->dma_d_config.rdma.Bias[6].CntSha = 0;
  cfg->dma_d_config.rdma.Bias[6].CntOffsetEn = 0;
  cfg->dma_d_config.rdma.Bias[6].CntOffset = 0;
  cfg->dma_d_config.rdma.Bias[6].CntThresholdSha = 0;
  cfg->dma_d_config.rdma.Bias[6].CntCmp = 0;
  
  ////////////////////////////
  cfg->dma_w_config.rdma.LPXEn = Enable_NotEn;
  cfg->dma_w_config.rdma.RPXEn = Enable_NotEn;
  cfg->dma_w_config.rdma.TPYEn = Enable_NotEn;
  cfg->dma_w_config.rdma.BPYEn = Enable_NotEn;
  
  cfg->dma_w_config.rdma.AOffset =0;
  cfg->dma_w_config.rdma.LPXOffset=0;
  cfg->dma_w_config.rdma.RPXOffset=0;
  cfg->dma_w_config.rdma.TPYOffset= 0;
  cfg->dma_w_config.rdma.BPYOffset= 0;
  cfg->dma_w_config.rdma.CntThresholdSha = sizeof_C>128 ? sizeof_C : 0;
  
  for(int i=0;i<7;i++)
    cfg->dma_w_config.rdma.Bias[i].AOffset = 0;
  */
  
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
  temp_ThreCtrl = 0;
  temp_DecCtrl = 0;
  temp_PadCtrl = 0;
  temp_BiasBase = 0; // Offset Of The First Bias Couple
  for(int i=0;i<7;i++) {
      // Separate Regs
    iowrite32(cfg->Bias[i].Bias   , base + temp_BiasBase + Bias1Sha_MSha);
    iowrite32(cfg->Bias[i].AOffset, base + temp_BiasBase + Bias1AOffset_MSha);
    iowrite32(cfg->Bias[i].CntSha , base + temp_BiasBase + RD_Bias1CntSha_MSha);
    iowrite32( 
             (cfg->Bias[i].CntOffsetEn<<8) | (cfg->Bias[i].CntOffset<<0),
             base + temp_BiasBase +  Bias1CntOffset_MSha
    );
    iowrite32(cfg->Bias[i].CntThresholdSha, base + temp_BiasBase + Bias1CntThresholdSha_MSha);
    iowrite32(cfg->Bias[i].CntCmp , base + temp_BiasBase + RD_Bias1CntCmp_MSha);
    
      // Accumulate Common Regs
    temp_BiasCtrl = temp_BiasCtrl | (cfg->Bias[i].BiasEn << i);
    temp_ThreCtrl = temp_ThreCtrl | (cfg->Bias[i].ThreCtrl << (i*3)); // *3 :-( May Be Done By Looped+
    temp_DecCtrl  = temp_DecCtrl  | (cfg->Bias[i].DecCtrl << (i*3));
    temp_PadCtrl  = temp_PadCtrl  | (cfg->Bias[i].PBSEn << (i+4));
    
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
  temp = 0 | (cfg->BStrideX << 0) | (cfg->BConvX << 4);
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
  
  iowrite32(cfg->dt ,base + MPE_COMMON_IN_FORMAT);
  
  temp = (cfg->rnd_size << 0) | (cfg->sat_en << 8) | (cfg->rnd_mode << 16);
  iowrite32(temp, base + MPE_COMMON_NORM_PARAM);
  // iowrite32(0xFF, base + MPE_NULL);
  
}


void nu_mpe_setup(uintptr_t base, ConfigMPE* cfg) {
  rumboot_printf("Configuring MPE..\n");

  nu_mpe_ma_setup(base + MPE_MA_BASE, cfg);
  nu_mpe_dma_setup(base + MPE_RDMA_D_BASE, & cfg->dma_d_config);
  nu_mpe_dma_setup(base + MPE_RDMA_W_BASE, & cfg->dma_w_config);
}

void nu_mpe_run(uintptr_t mpe_base, ConfigMPE* cfg) {
  rumboot_printf("Start MPE...\n");
  iowrite32(0,mpe_base + MPE_RDMA_D_BASE + DMA_START);
  iowrite32(0,mpe_base + MPE_RDMA_W_BASE + DMA_START);
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

  int32_t Ho  = out_cube_metrics->H;
  int32_t Wo  = out_cube_metrics->W;
  int32_t Co  = out_cube_metrics->C;

  uint32_t fm = cfg_reg->wOpM >> 8;

  uint32_t mr = fm&0x1 && dt || ~(fm&0x1) && (dt&&(Ci>8) || Ci>16) ? 0x1 : 0x0;

  int el_s    = dt ? 0x2 : 0x1; // element size in bytes
  int rdma_thr= 0x10;   // rdma throughput in bytes; wdma_thr is the same
  int rdctn   = mr + 1; // reduction

  int frgs  = rdma_thr * rdctn; // dma transfer quantum in bytes; fragment size

  int Ci_s  = Ci * el_s;
  int Co_s  = Co * el_s;

  int frgli = Ci_s % frgs; // fragment last size if frgl>0
  int frglo = Co_s % frgs; // fragment last size if frgl>0

  if (~(fm&0x1)) {  // MEMtoPPE
    //cfg_reg->rAXIp  =
    cfg_reg->rBrdX  = (Wi-1) * Ci_s;
    cfg_reg->rBrdY  = (Hi-1) * Wi * Ci_s;
    cfg_reg->rBrdZ  = frgli>0 ? (int)(Ci_s/frgs)*frgs : ((int)(Ci_s/frgs) - 1)*frgs;

    cfg_reg->rStrX  = Ci_s;
    cfg_reg->rStrY  = Wi * Ci_s;
    cfg_reg->rStrZ  = frgs;

    cfg_reg->rFrgs  = frgs;
    cfg_reg->rFrgl  = frgli>0 ? frgli : frgs;
    //cfg_reg->rXYZd  = 0x2;
    cfg_reg->rXYZd  = DmaXYZDirection_Z;

    if (!(fm&0x2)) {  // linear
      cfg_reg->rBstX  = Wi-1;
      cfg_reg->rBstY  = Hi-1;
      cfg_reg->rBstZ  = 0x0;
    }
    else if (fm&0x2) {  // boxed
      cfg_reg->rBstX  = 0x80 - 1; // 128 element box
      cfg_reg->rBstY  = 0x0     ; // 1 element
      cfg_reg->rBstZ  = 0x8 - 1 ; // 128 channels/frgs = 128*(1B|2B)/(16B|32B)
    }
    //else if (fm&0x4) {} // split

    cfg_reg->rBxtX  = cfg_reg->rBstX;
    cfg_reg->rBxtY  = cfg_reg->rBstY;
    cfg_reg->rBxtZ  = cfg_reg->rBstZ;

    cfg_reg->rBffX  = 0x0;
    cfg_reg->rBffY  = 0x0;
    cfg_reg->rBffZ  = 0x0;
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
  cfg_reg->wXYZd  = 0x2;

  if (!(fm&0x2)) {  // linear
    cfg_reg->wBstX  = Wo-1;
    cfg_reg->wBxtX  = Wo-1;

    cfg_reg->wBstY  = Ho-1;
    cfg_reg->wBstZ  = 0x0;

    cfg_reg->wIstX  = 0x0;
    cfg_reg->wIxtX  = 0x0;
    cfg_reg->wIffX  = 0x0;
  }
  else if (fm&0x2) {  // boxed
    //cfg_reg->wBstX  = ; // excel
    //cfg_reg->wBxtX  = ; // excel

    cfg_reg->wBstY  = 0x0     ;
    cfg_reg->wBstZ  = 0x8 - 1 ;

    cfg_reg->wIstX  = Wi-1; // the same as rBstX
    cfg_reg->wIxtX  = Wi-1; // the same as rBxtX
    cfg_reg->wIffX  = 0x0;  // the same as rBffX
  }
  //else if (fm&0x4) {} // split

  cfg_reg->wBxtY  = cfg_reg->wBstY;
  cfg_reg->wBxtZ  = cfg_reg->wBstZ;

  cfg_reg->wBffX  = 0x0;
  cfg_reg->wBffY  = 0x0;
  cfg_reg->wBffZ  = 0x0;

  cfg_reg->wWi  = Wi - 1;
  cfg_reg->wHi  = Hi - 1;
  cfg_reg->wCi  = Ci - 1;
  cfg_reg->wWo  = Wo - 1;
  cfg_reg->wHo  = Ho - 1;
  cfg_reg->wCo  = Co - 1;
  cfg_reg->wOpM = mr<<28 | dt<<16 | fm<<8 | meth;
  cfg_reg->wK   = (cfg->Sh-1)<<20 | (cfg->Sw-1)<<16 | (cfg->Kh-1)<<8 | (cfg->Kw-1);
  cfg_reg->wKWr = cfg->Kw_r;
  cfg_reg->wKHr = cfg->Kh_r;
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
  iowrite32(cfg->wCo  , wbase + NU_PPE_DATA_C_OUT         );
  iowrite32(cfg->wOpM , wbase + NU_PPE_OP_MODE            );
  iowrite32(cfg->wK   , wbase + NU_PPE_KERNEL             );
  iowrite32(cfg->wKWr , wbase + NU_PPE_RECIP_KERNEL_W     );
  iowrite32(cfg->wKHr , wbase + NU_PPE_RECIP_KERNEL_H     );
  iowrite32(cfg->wP   , wbase + NU_PPE_PADDING            );
  iowrite32(cfg->wPV1 , wbase + NU_PPE_PADDING_VALUE_1    );
  iowrite32(cfg->wPV2 , wbase + NU_PPE_PADDING_VALUE_2    );
  iowrite32(cfg->wPV3 , wbase + NU_PPE_PADDING_VALUE_3    );
  iowrite32(cfg->wPV4 , wbase + NU_PPE_PADDING_VALUE_4    );
  iowrite32(cfg->wPV5 , wbase + NU_PPE_PADDING_VALUE_5    );
  iowrite32(cfg->wPV6 , wbase + NU_PPE_PADDING_VALUE_6    );
  iowrite32(cfg->wPV7 , wbase + NU_PPE_PADDING_VALUE_7    );
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

//void nu_vpe_run(uintptr_t vpe_base, ConfigDMAVPE* cfg) {
void nu_vpe_run(uintptr_t vpe_base, ConfigVPE* cfg){ // ?????????   ConfigVPE* cfg  
  // Напиши сюда 3апуск DMA
  uint32_t temp;
  do {
    temp = ioread32(vpe_base + NU_VPE + NU_VPE_NEXT_CNTX);
  }while((temp & 1) !=0);
  
  rumboot_printf("Start VPE...\n");
  
  if(cfg->mark)
    temp = temp | (1<<1) | (1<<0);  //MARKED_CNTX=1 | NEXT_CNTX=1
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

void nu_vpe_wait_cntx_appl(uintptr_t vpe_base, ConfigVPE* cfg){
    rumboot_printf("Wait VPE context got...\n");
    while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 0) & 1) !=1) {}
    iowrite32((1<<0),vpe_base + NU_VPE + NU_VPE_INT_RESET);
  rumboot_printf("Done VPE context got.\n");
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

void na_cu_set_units_direct_mode(uintptr_t base, uint32_t mask) {
  uint32_t temp;
  temp = ioread32(base + NA_UNITS_MODE);
  temp = temp | mask;
  rumboot_printf("Writing [0x%x]=0x%x\n",base + NA_UNITS_MODE,temp);
  iowrite32(temp,base + NA_UNITS_MODE);
}

