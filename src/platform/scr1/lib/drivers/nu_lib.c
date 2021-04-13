

#include <stdint.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <regs/regs_nu_mpe.h>
#include <regs/regs_nu_vpe.h>
#include <regs/regs_nu_ppe.h>

#include <platform/devices.h>
#include <platform/devices/nu_cpdmac_lib.h>
#include <platform/devices/nu_lib.h>



void nu_vpe_load_config(ConfigVPE* cfg, void* cfg_bin) {
  int32_t* ptr;

  ptr = (int32_t*) cfg_bin;

  cfg->in_data_type=*ptr;ptr++;
  cfg->out_data_type=*ptr;ptr++;
  cfg->flying=*ptr;ptr++;
  cfg->op0_en=*ptr;ptr++;
  cfg->op1_en=*ptr;ptr++;
  cfg->op2_en=*ptr;ptr++;
  cfg->c3_offset=*ptr;ptr++;
  cfg->c3_scale=*ptr;ptr++;
  cfg->c3_trunc=*ptr;ptr++;
  cfg->nan_to_zero=*ptr;ptr++;
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

}
void nu_mpe_load_config(ConfigMPE* cfg, void* cfg_bin) {
  int32_t* ptr;

  ptr = (int32_t*) cfg_bin;

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
  cfg-> RND_MODE =*ptr;ptr++;
  cfg-> SAT      =*ptr;ptr++;
  cfg-> RND_SIZE =*ptr;ptr++;
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
    cfg-> Kh_r  =*ptr;ptr++;
    cfg-> Kw_r  =*ptr;ptr++;
    cfg-> Tp    =*ptr;ptr++;
    cfg-> Bp    =*ptr;ptr++;
    cfg-> Lp    =*ptr;ptr++;
    cfg-> Rp    =*ptr;ptr++;

    cfg-> pv[0] =*ptr;ptr++;
    cfg-> pv[1] =*ptr;ptr++;
    cfg-> pv[2] =*ptr;ptr++;
    cfg-> pv[3] =*ptr;ptr++;
    cfg-> pv[4] =*ptr;ptr++;
    cfg-> pv[5] =*ptr;ptr++;
    cfg-> pv[6] =*ptr;ptr++;

    cfg-> meth   =*ptr;ptr++;
    cfg-> dt     =*ptr;ptr++;
}

  void nu_vpe_print_Mode(Mode mode, char* name) {
    static char* ModeNames[]= {
      "Unitary",
      "Channel",
      "Element"
    };

    rumboot_printf("  %s = %s \n", name, ModeNames[(int) mode] );
  }


  void nu_vpe_print_RoundMode(RoundMode mode, char* name) {
    static char* RoundModeNames[]= {
      "Nearest",
      "Down",
      "Up"
    };

    rumboot_printf("  %s = %s \n", name, RoundModeNames[(int) mode] );
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
      "Sum"
    };

    rumboot_printf("  %s = %s \n", name, AluOperationSwitchNames[(int) operation] );
  }


  void nu_vpe_print_AluOperationExtSwitch(AluOperationExtSwitch operation, char* name) {
    static char* AluOperationExtSwitchNames[]= {
      "Max",
      "Min",
      "Sum",
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


void nu_vpe_print_config(ConfigVPE* cfg){
  rumboot_printf("ConfigVPE:\n");
  nu_vpe_print_DataTypeExt(cfg->in_data_type,"in_data_type");
  nu_vpe_print_DataType(cfg->in_data_type,"out_data_type");
  nu_vpe_print_Enable(cfg->flying,"flying");
  nu_vpe_print_Enable(cfg->op0_en,"op0_en");
  nu_vpe_print_Enable(cfg->op1_en,"op1_en");
  nu_vpe_print_Enable(cfg->op2_en,"op2_en");
  rumboot_printf("  c3_offset = %d\n" , cfg->c3_offset);
  rumboot_printf("  c3_scale = %d\n" , cfg->c3_scale);
  rumboot_printf("  c3_trunc = %d\n" , cfg->c3_trunc);
  nu_vpe_print_Enable(cfg->nan_to_zero,"nan_to_zero");

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
  rumboot_printf("  c2_trunc = %d \n\n" , cfg->op2_config.c2_trunc);

}

void nu_vpe_print_config_dma(ConfigDMAVPE* cfg) {
  rumboot_printf("ConfigDMAVPE:\n");
  nu_vpe_print_Enable(cfg->dma_src_en,"dma_src_en");
  nu_vpe_print_Enable(cfg->dma_op0_en,"dma_op0_en");
  nu_vpe_print_Enable(cfg->dma_op1_en,"dma_op1_en");
  nu_vpe_print_Enable(cfg->dma_op2_en,"dma_op2_en");
  nu_vpe_print_Enable(cfg->dma_dst_en,"dma_dst_en");
}

void nu_mpe_print_config(ConfigMPE* cfg){
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
    rumboot_printf("  RND_MODE = %d \n" , cfg->RND_MODE);
    rumboot_printf("  SAT      = %d \n" , cfg->SAT);
    rumboot_printf("  RND_SIZE = %d \n" , cfg->RND_SIZE);

}

void nu_mpe_print_config_dma(ConfigDMAMPE* cfg) {
  rumboot_printf("ConfigDMAMPE:\n");
    rumboot_printf("  H        = %d \n" , cfg-> H);
    rumboot_printf("  W        = %d \n" , cfg-> W);
    rumboot_printf("  C        = %d \n" , cfg-> C);
    rumboot_printf("  R        = %d \n" , cfg-> R);
    rumboot_printf("  S        = %d \n" , cfg-> S);
    rumboot_printf("  data_buf#= %d \n" , cfg-> in_data_partition);
    rumboot_printf("  warr_buf#= %d \n" , cfg-> warr_partition);
}

void nu_ppe_print_config(ConfigPPE* cfg){
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
}

void nu_ppe_print_config_reg(ConfigREGPPE* cfg_reg){
  rumboot_printf("ConfigREGPPE:\n");
  rumboot_printf("  = RDMA registers:\n");
  // rumboot_printf("  rSt     = %x \n" , cfg_reg->rSt);
  rumboot_printf("  rOpEn   = %x \n" , cfg_reg->rOpEn);
  rumboot_printf("  rPWi    = %x \n" , cfg_reg->rPWi);
  rumboot_printf("  rPHi    = %x \n" , cfg_reg->rPHi);
  rumboot_printf("  rPCi    = %x \n" , cfg_reg->rPCi);
  rumboot_printf("  rBALs   = %x \n" , cfg_reg->rBALs);
  // rumboot_printf("  rBAHs   = %x \n" , cfg_reg->rBAHs);
  rumboot_printf("  rVSs    = %x \n" , cfg_reg->rVSs);
  rumboot_printf("  rLSs    = %x \n" , cfg_reg->rLSs);
  rumboot_printf("  rESs    = %x \n" , cfg_reg->rESs);
  rumboot_printf("  rOpM    = %x \n" , cfg_reg->rOpM);
  rumboot_printf("  rBSWi   = %x \n" , cfg_reg->rBSWi);
  rumboot_printf("  rBSHi   = %x \n" , cfg_reg->rBSHi);
  rumboot_printf("  rBSCi   = %x \n" , cfg_reg->rBSCi);
  rumboot_printf("  rStWi   = %x \n" , cfg_reg->rStWi);
  rumboot_printf("  rOfWi   = %x \n" , cfg_reg->rOfWi);
  // rumboot_printf("  rK      = %x \n" , cfg_reg->rK);
  rumboot_printf("  = PPE+WDMA registers:\n");
  // rumboot_printf("  wSt     = %x \n" , cfg_reg->wSt);
  rumboot_printf("  wOpEn   = %x \n" , cfg_reg->wOpEn);
  rumboot_printf("  wWi     = %x \n" , cfg_reg->wWi);
  rumboot_printf("  wHi     = %x \n" , cfg_reg->wHi);
  rumboot_printf("  wCi     = %x \n" , cfg_reg->wCi);
  rumboot_printf("  wWo     = %x \n" , cfg_reg->wWo);
  rumboot_printf("  wHo     = %x \n" , cfg_reg->wHo);
  rumboot_printf("  wCo     = %x \n" , cfg_reg->wCo);
  rumboot_printf("  wPWo    = %x \n" , cfg_reg->wPWo);
  rumboot_printf("  wPHo    = %x \n" , cfg_reg->wPHo);
  rumboot_printf("  wPCo    = %x \n" , cfg_reg->wPCo);
  rumboot_printf("  wBALd   = %x \n" , cfg_reg->wBALd);
  // rumboot_printf("  wBAHd   = %x \n" , cfg_reg->wBAHd);
  rumboot_printf("  wVSd    = %x \n" , cfg_reg->wVSd);
  rumboot_printf("  wLSd    = %x \n" , cfg_reg->wLSd);
  rumboot_printf("  wESd    = %x \n" , cfg_reg->wESd);
  rumboot_printf("  wOpM    = %x \n" , cfg_reg->wOpM);
  rumboot_printf("  wBSWi   = %x \n" , cfg_reg->wBSWi);
  rumboot_printf("  wBSHi   = %x \n" , cfg_reg->wBSHi);
  rumboot_printf("  wBSCi   = %x \n" , cfg_reg->wBSCi);
  rumboot_printf("  wStWi   = %x \n" , cfg_reg->wStWi);
  rumboot_printf("  wOfWi   = %x \n" , cfg_reg->wOfWi);
  rumboot_printf("  wBSWo   = %x \n" , cfg_reg->wBSWo);
  rumboot_printf("  wBSHo   = %x \n" , cfg_reg->wBSHo);
  rumboot_printf("  wBSCo   = %x \n" , cfg_reg->wBSCo);
  rumboot_printf("  wStWo   = %x \n" , cfg_reg->wStWo);
  rumboot_printf("  wOfWo   = %x \n" , cfg_reg->wOfWo);
  rumboot_printf("  wK      = %x \n" , cfg_reg->wK);
  rumboot_printf("  wKWr    = %x \n" , cfg_reg->wKWr);
  rumboot_printf("  wKHr    = %x \n" , cfg_reg->wKHr);
  rumboot_printf("  wP      = %x \n" , cfg_reg->wP);
  rumboot_printf("  wPV1    = %x \n" , cfg_reg->wPV1);
  rumboot_printf("  wPV2    = %x \n" , cfg_reg->wPV2);
  rumboot_printf("  wPV3    = %x \n" , cfg_reg->wPV3);
  rumboot_printf("  wPV4    = %x \n" , cfg_reg->wPV4);
  rumboot_printf("  wPV5    = %x \n" , cfg_reg->wPV5);
  rumboot_printf("  wPV6    = %x \n" , cfg_reg->wPV6);
  rumboot_printf("  wPV7    = %x \n" , cfg_reg->wPV7);
  // rumboot_printf("  wINi    = %x \n" , cfg_reg->wINi);
  // rumboot_printf("  wNNi    = %x \n" , cfg_reg->wNNi);
  // rumboot_printf("  wNNo    = %x \n" , cfg_reg->wNNo);
}

void nu_vpe_setup(uintptr_t base, ConfigVPE* cfg, ConfigDMAVPE* cfg_dma) {
  rumboot_printf("Configuring VPE..\n");

  // iowrite32(cfg->MYFIELD, base + NU_VPE_MYREG);

  int32_t tmp_data;


  // VPE & WDMA -------------------------------------------------------------------------
  iowrite32(cfg->wrdma_config.dma_op_en , DUT_BASE + NU_VPE + NU_VPE_OP_ENABLE );
  iowrite32(cfg->wrdma_config.dma_W     , DUT_BASE + NU_VPE + NU_VPE_DATA_W_OUT);
  iowrite32(cfg->wrdma_config.dma_H     , DUT_BASE + NU_VPE + NU_VPE_DATA_H_OUT);
  iowrite32(cfg->wrdma_config.dma_C     , DUT_BASE + NU_VPE + NU_VPE_DATA_C_OUT);

  tmp_data = (cfg->wrdma_config.dma_ram_type << 8);
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_WDMA_DST_CFG);

  iowrite32(cfg->wrdma_config.dma_baddr         , DUT_BASE + NU_VPE + NU_VPE_WDMA_DST_BASE_L        );
  iowrite32(cfg->wrdma_config.dma_line_stride   , DUT_BASE + NU_VPE + NU_VPE_WDMA_DST_LINE_STRIDE   );
  iowrite32(cfg->wrdma_config.dma_vector_stride , DUT_BASE + NU_VPE + NU_VPE_WDMA_DST_VECTOR_STRIDE );
  iowrite32(cfg->wrdma_config.dma_elem_stride   , DUT_BASE + NU_VPE + NU_VPE_WDMA_ELEM_STRIDE       );
  iowrite32(cfg->wrdma_config.dma_box_st_size_x , DUT_BASE + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_X     );
  iowrite32(cfg->wrdma_config.dma_box_st_size_y , DUT_BASE + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Y     );
  iowrite32(cfg->wrdma_config.dma_box_st_size_z , DUT_BASE + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Z     );
  iowrite32(cfg->wrdma_config.dma_box_size_x    , DUT_BASE + NU_VPE + NU_VPE_WDMA_BOX_SIZE_X        );
  iowrite32(cfg->wrdma_config.dma_box_size_y    , DUT_BASE + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Y        );
  iowrite32(cfg->wrdma_config.dma_box_size_z    , DUT_BASE + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Z        );
  iowrite32(cfg->wrdma_config.dma_box_offset_x  , DUT_BASE + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_X );
  iowrite32(cfg->wrdma_config.dma_box_offset_y  , DUT_BASE + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Y );
  iowrite32(cfg->wrdma_config.dma_box_offset_z  , DUT_BASE + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Z );


  tmp_data = (cfg->out_data_type << 16) | (cfg->op2_config.coef_type << 14) | (cfg->op1_config.coef_type << 12) | (cfg->op0_config.coef_type << 10) | (cfg->in_data_type << 8) | (cfg->nan_to_zero << 4) | (cfg->flying << 0) ;
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP_MODE);


  // OP0 ----------------------------------------------------------------
  tmp_data = (cfg->op0_config.relu_en << 6) | (cfg->op0_config.prelu_en << 5) | (!cfg->op0_config.mux_en << 4) | (cfg->op0_config.alu_operation << 2) | (!cfg->op0_config.alu_en << 1) | (!cfg->op0_en << 0);
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP0_CFG);

  tmp_data = (cfg->op0_config.lshift_value << 8) | (nu_vpe_mode_to_bool(cfg->op0_config.alu_mode) << 0) ; // ??????????????????????????????? (cfg->op0_config.alu_mode << 0)
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP0_ALU_CFG);

  iowrite32(cfg->op0_config.alu_value, DUT_BASE + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL);

  iowrite32(nu_vpe_mode_to_bool(cfg->op0_config.mux_mode) , DUT_BASE + NU_VPE + NU_VPE_OP0_MUL_CFG); //???????????????????
  iowrite32(cfg->op0_config.mux_value, DUT_BASE + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL);

  tmp_data = (cfg->op0_config.norm_round_mode << 16) | (cfg->op0_config.norm_saturation_en << 8) | (cfg->op0_config.norm_round_size << 0);
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP0_NORM_PARAM);


  // OP1 ----------------------------------------------------------------
  tmp_data = (cfg->op1_config.relu_en << 6) | (cfg->op1_config.prelu_en << 5) | (!cfg->op1_config.mux_en << 4) | (cfg->op1_config.alu_operation << 2) | (!cfg->op1_config.alu_en << 1) | (!cfg->op1_en << 0);
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP1_CFG);

  tmp_data = (cfg->op1_config.lshift_value << 8) | (nu_vpe_mode_to_bool(cfg->op1_config.alu_mode) << 0) ; // ???????????????????????????????
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP1_ALU_CFG);

  iowrite32(cfg->op1_config.alu_value, DUT_BASE + NU_VPE + NU_VPE_OP1_ALU_SRC_VAL);

  iowrite32(nu_vpe_mode_to_bool(cfg->op1_config.mux_mode) , DUT_BASE + NU_VPE + NU_VPE_OP1_MUL_CFG);
  iowrite32(cfg->op1_config.mux_value, DUT_BASE + NU_VPE + NU_VPE_OP1_MUL_SRC_VAL);

  tmp_data = (cfg->op1_config.norm_round_mode << 16) | (cfg->op1_config.norm_saturation_en << 8) | (cfg->op1_config.norm_round_size << 0);
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP1_NORM_PARAM);

  // OP2 ----------------------------------------------------------------
  tmp_data = (!cfg->op2_config.lut_en << 6) | (cfg->op2_config.prelu_en << 5) | (!cfg->op2_config.mux_en << 4) | (cfg->op2_config.alu_operation << 2) | (!cfg->op2_config.alu_en << 1) | (!cfg->op2_en << 0);
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP2_CFG);

  tmp_data = (!cfg->op2_config.c1_en << 8) | (nu_vpe_mode_to_bool(cfg->op2_config.alu_mode) << 0) ; // ???????????????????????????????
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP2_ALU_CFG);

  iowrite32(cfg->op2_config.alu_value, DUT_BASE + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL);

  iowrite32(cfg->op2_config.c1_offset, DUT_BASE + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL);
  iowrite32(cfg->op2_config.c1_scale , DUT_BASE + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL );
  iowrite32(cfg->op2_config.c1_trunc , DUT_BASE + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL );


  tmp_data = (!cfg->op2_config.c2_en << 8) | (nu_vpe_mode_to_bool(cfg->op2_config.mux_mode) << 0) ; // ???????????????????????????????
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP2_MUL_CFG);

  iowrite32(cfg->op2_config.mux_value, DUT_BASE + NU_VPE + NU_VPE_OP2_MUL_SRC_VAL);

  iowrite32(cfg->op2_config.c2_offset, DUT_BASE + NU_VPE + NU_VPE_OP2_MUL_CVT_OFFSET_VAL);
  iowrite32(cfg->op2_config.c2_scale , DUT_BASE + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL );
  iowrite32(cfg->op2_config.c2_trunc , DUT_BASE + NU_VPE + NU_VPE_OP2_MUL_CVT_TRUNC_VAL );

  tmp_data = (cfg->op2_config.norm_round_mode << 16) | (cfg->op2_config.norm_saturation_en << 8) | (cfg->op2_config.norm_round_size << 0);
  iowrite32(tmp_data, DUT_BASE + NU_VPE + NU_VPE_OP2_NORM_PARAM);


  // Configuration SRC_RDMA ------------------------------------------------------
  iowrite32(cfg->src_rdma_config.dma_op_en , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_OP_ENABLE);
  iowrite32(cfg->src_rdma_config.dma_W     , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_W_IN);
  iowrite32(cfg->src_rdma_config.dma_H     , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_H_IN);
  iowrite32(cfg->src_rdma_config.dma_C     , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_C_IN);

  tmp_data = (cfg->src_rdma_config.dma_ram_type << 5) | ((cfg->src_rdma_config.dma_data_mode >> 1) << 4) | (cfg->src_rdma_config.dma_data_size << 3) | (cfg->src_rdma_config.dma_data_use << 1);
  iowrite32(tmp_data, DUT_BASE + NU_VPE_SRC_RDMA+ NU_VPE_RDMA_CFG );

  iowrite32(cfg->src_rdma_config.dma_baddr         , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BASE_L            );
  iowrite32(cfg->src_rdma_config.dma_line_stride   , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_LINE_STRIDE       );
  iowrite32(cfg->src_rdma_config.dma_vector_stride , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_VECTOR_STRIDE     );
  iowrite32(cfg->src_rdma_config.dma_elem_stride   , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_ELEM_STRIDE       );
  iowrite32(cfg->src_rdma_config.dma_box_st_size_x , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X     );
  iowrite32(cfg->src_rdma_config.dma_box_st_size_y , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y     );
  iowrite32(cfg->src_rdma_config.dma_box_st_size_z , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z     );
  iowrite32(cfg->src_rdma_config.dma_box_size_x    , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_X        );
  iowrite32(cfg->src_rdma_config.dma_box_size_y    , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_Y        );
  iowrite32(cfg->src_rdma_config.dma_box_size_z    , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_Z        );
  iowrite32(cfg->src_rdma_config.dma_box_offset_x  , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X );
  iowrite32(cfg->src_rdma_config.dma_box_offset_y  , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y );
  iowrite32(cfg->src_rdma_config.dma_box_offset_z  , DUT_BASE + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z );

  // Configuration OP0_RDMA ------------------------------------------------------
  iowrite32(cfg->op0_rdma_config.dma_op_en , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_OP_ENABLE);
  iowrite32(cfg->op0_rdma_config.dma_W     , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_W_IN);
  iowrite32(cfg->op0_rdma_config.dma_H     , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_H_IN);
  iowrite32(cfg->op0_rdma_config.dma_C     , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_C_IN);

  tmp_data = (cfg->op0_rdma_config.dma_ram_type << 5) | ((cfg->op0_rdma_config.dma_data_mode >> 1) << 4) | (cfg->op0_rdma_config.dma_data_size << 3) | (cfg->op0_rdma_config.dma_data_use << 1);
  iowrite32(tmp_data, DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_CFG);

  iowrite32(cfg->op0_rdma_config.dma_baddr         , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BASE_L            );
  iowrite32(cfg->op0_rdma_config.dma_line_stride   , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_LINE_STRIDE       );
  iowrite32(cfg->op0_rdma_config.dma_vector_stride , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_VECTOR_STRIDE     );
  iowrite32(cfg->op0_rdma_config.dma_elem_stride   , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_ELEM_STRIDE       );
  iowrite32(cfg->op0_rdma_config.dma_box_st_size_x , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X     );
  iowrite32(cfg->op0_rdma_config.dma_box_st_size_y , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y     );
  iowrite32(cfg->op0_rdma_config.dma_box_st_size_z , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z     );
  iowrite32(cfg->op0_rdma_config.dma_box_size_x    , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_X        );
  iowrite32(cfg->op0_rdma_config.dma_box_size_y    , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Y        );
  iowrite32(cfg->op0_rdma_config.dma_box_size_z    , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Z        );
  iowrite32(cfg->op0_rdma_config.dma_box_offset_x  , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X );
  iowrite32(cfg->op0_rdma_config.dma_box_offset_y  , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y );
  iowrite32(cfg->op0_rdma_config.dma_box_offset_z  , DUT_BASE + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z );

  // Configuration OP1_RDMA ------------------------------------------------------
  iowrite32(cfg->op1_rdma_config.dma_op_en , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_OP_ENABLE);
  iowrite32(cfg->op1_rdma_config.dma_W     , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_W_IN);
  iowrite32(cfg->op1_rdma_config.dma_H     , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_H_IN);
  iowrite32(cfg->op1_rdma_config.dma_C     , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_C_IN);

  tmp_data = (cfg->op1_rdma_config.dma_ram_type << 5) | ((cfg->op1_rdma_config.dma_data_mode >> 1) << 4) | (cfg->op1_rdma_config.dma_data_size << 3) | (cfg->op1_rdma_config.dma_data_use << 1);
  iowrite32(tmp_data, DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_CFG);

  iowrite32(cfg->op1_rdma_config.dma_baddr         , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BASE_L            );
  iowrite32(cfg->op1_rdma_config.dma_line_stride   , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_LINE_STRIDE       );
  iowrite32(cfg->op1_rdma_config.dma_vector_stride , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_VECTOR_STRIDE     );
  iowrite32(cfg->op1_rdma_config.dma_elem_stride   , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_ELEM_STRIDE       );
  iowrite32(cfg->op1_rdma_config.dma_box_st_size_x , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X     );
  iowrite32(cfg->op1_rdma_config.dma_box_st_size_y , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y     );
  iowrite32(cfg->op1_rdma_config.dma_box_st_size_z , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z     );
  iowrite32(cfg->op1_rdma_config.dma_box_size_x    , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_X        );
  iowrite32(cfg->op1_rdma_config.dma_box_size_y    , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Y        );
  iowrite32(cfg->op1_rdma_config.dma_box_size_z    , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Z        );
  iowrite32(cfg->op1_rdma_config.dma_box_offset_x  , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X );
  iowrite32(cfg->op1_rdma_config.dma_box_offset_y  , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y );
  iowrite32(cfg->op1_rdma_config.dma_box_offset_z  , DUT_BASE + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z );

  // Configuration OP2_RDMA ------------------------------------------------------
  iowrite32(cfg->op2_rdma_config.dma_op_en , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_OP_ENABLE);
  iowrite32(cfg->op2_rdma_config.dma_W     , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_W_IN);
  iowrite32(cfg->op2_rdma_config.dma_H     , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_H_IN);
  iowrite32(cfg->op2_rdma_config.dma_C     , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_C_IN);

  tmp_data = (cfg->op2_rdma_config.dma_ram_type << 5) | ((cfg->op2_rdma_config.dma_data_mode >> 1) << 4) | (cfg->op2_rdma_config.dma_data_size << 3) | (cfg->op2_rdma_config.dma_data_use << 1);
  iowrite32(tmp_data, DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_CFG);

  iowrite32(cfg->op2_rdma_config.dma_baddr         , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BASE_L            );
  iowrite32(cfg->op2_rdma_config.dma_line_stride   , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_LINE_STRIDE       );
  iowrite32(cfg->op2_rdma_config.dma_vector_stride , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_VECTOR_STRIDE     );
  iowrite32(cfg->op2_rdma_config.dma_elem_stride   , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_ELEM_STRIDE       );
  iowrite32(cfg->op2_rdma_config.dma_box_st_size_x , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X     );
  iowrite32(cfg->op2_rdma_config.dma_box_st_size_y , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y     );
  iowrite32(cfg->op2_rdma_config.dma_box_st_size_z , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z     );
  iowrite32(cfg->op2_rdma_config.dma_box_size_x    , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_X        );
  iowrite32(cfg->op2_rdma_config.dma_box_size_y    , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Y        );
  iowrite32(cfg->op2_rdma_config.dma_box_size_z    , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Z        );
  iowrite32(cfg->op2_rdma_config.dma_box_offset_x  , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X );
  iowrite32(cfg->op2_rdma_config.dma_box_offset_y  , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y );
  iowrite32(cfg->op2_rdma_config.dma_box_offset_z  , DUT_BASE + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z );

}


void nu_vpe_decide_dma_config_trivial(ConfigVPE* cfg, CubeMetrics* metrics, ConfigDMAVPE* cfg_dma) {
  cfg_dma->H = metrics->H;
  cfg_dma->W = metrics->W;
  cfg_dma->C = metrics->C;

  if(cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32) {
    cfg_dma->dma_src_en =  Enable_NotEn ;  // if 32bit Input Data then Main Channel Works
    cfg->src_rdma_config.dma_op_en = Enable_NotEn; // new struct ============
  }
  else {
    cfg_dma->dma_src_en = Enable_En;
    cfg->src_rdma_config.dma_op_en = Enable_En; // new struct ============
  }

  if((cfg->op0_config.alu_en == Enable_En && cfg->op0_config.alu_mode != Mode_Unitary) ||
     (cfg->op0_config.mux_en == Enable_En && cfg->op0_config.mux_mode != Mode_Unitary)  ) { // if Some Of Operands Enabled And Not A Single Value
    cfg_dma->dma_op0_en = Enable_En;
    cfg->op0_rdma_config.dma_op_en = Enable_En; // new struct ============
  }
  else {
    cfg_dma->dma_op0_en = Enable_NotEn;
    cfg->op0_rdma_config.dma_op_en = Enable_NotEn; // new struct ============
  }


  if((cfg->op1_config.alu_en == Enable_En && cfg->op1_config.alu_mode != Mode_Unitary) ||
     (cfg->op1_config.mux_en == Enable_En && cfg->op1_config.mux_mode != Mode_Unitary)  ) { // if Some Of Operands Enabled And Not A Single Value
    cfg_dma->dma_op1_en = Enable_En;
    cfg->op1_rdma_config.dma_op_en = Enable_En; // new struct ============
  }
  else {
    cfg_dma->dma_op1_en = Enable_NotEn;
    cfg->op1_rdma_config.dma_op_en = Enable_NotEn; // new struct ============
  }


  if((cfg->op2_config.alu_en == Enable_En && cfg->op2_config.alu_mode != Mode_Unitary) ||
     (cfg->op2_config.mux_en == Enable_En && cfg->op2_config.mux_mode != Mode_Unitary)  ) { // if Some Of Operands Enabled And Not A Single Value
    cfg_dma->dma_op2_en = Enable_En;
    cfg->op2_rdma_config.dma_op_en = Enable_En; // new struct ============
  }
  else {
    cfg_dma->dma_op2_en = Enable_NotEn;
    cfg->op2_rdma_config.dma_op_en = Enable_NotEn; // new struct ============
  }


  // We Have No Setting That Define If We Run WDMA Or Main Wr Channel
}

int nu_mpe_get_size_in_partitions(int size_in_bytes) {
  int res;
  res = size_in_bytes / (NU_MPE_BUF01 - NU_MPE_BUF00);
  if( (size_in_bytes % (NU_MPE_BUF01 - NU_MPE_BUF00)) != 0 )
    res++;

  return res;
}

int nu_mpe_decide_dma_config_trivial(ConfigMPE* cfg, CubeMetrics* cube_metrics, WarrMetrics* warr_metrics, ConfigDMAMPE* cfg_dma) {
  int warr_size_in_partitions;

  cfg_dma->H = cube_metrics->H;
  cfg_dma->W = cube_metrics->W;
  cfg_dma->C = cube_metrics->C;

  cfg_dma->R = warr_metrics->H;
  cfg_dma->S = warr_metrics->W; // CHECK The Accurate WarrMetrics Field Meanings

  cfg_dma->in_data_partition=0; // Data Is Always In 0th Partition
  cfg_dma->warr_partition = nu_mpe_get_size_in_partitions(cube_metrics->s); // Warr - In Next Partition After Data
  warr_size_in_partitions = nu_mpe_get_size_in_partitions(warr_metrics->s);
  if(cfg_dma->warr_partition+warr_size_in_partitions >16) return -1;
  return 0;
}

void nu_mpe_setup(uintptr_t base, ConfigMPE* cfg, ConfigDMAMPE* cfg_dma) {
  rumboot_printf("Configuring MPE..\n");

  // iowrite32(cfg->MYFIELD, base + NU_MPE_MYREG);
}

void nu_mpe_run(uintptr_t mpe_base, ConfigDMAMPE* cfg_dma) {

  // iowrite32(cfg->MYFIELD, mpe_base + NU_MPE_MYREG);
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

void nu_ppe_setup(uintptr_t base, ConfigPPE* cfg, ConfigREGPPE* cfg_reg) {
  rumboot_printf("Configuring PPE..\n");

  // Перегнатb параметры и3 cfg в cfg_reg и 3апуститb nu_ppe_setup_reg

  // iowrite32(cfg->MYFIELD, base + NU_MPE_MYREG);
}

int  nu_ppe_decide_dma_config_trivial(ConfigPPE* cfg, CubeMetrics* in_cube_metrics, CubeMetrics* out_cube_metrics, ConfigREGPPE* cfg_reg) {
  uint32_t Celem;
  uint32_t Cbyte;
  // rdma
  // cfg_reg->rOpEn  = 0;
  // cfg_reg->rBALs  = 0;
  // cfg_reg->rK     = 0;
  if(cfg->dt == DataType_Int8) {
    cfg_reg->rOpM = 0X00000000;
    Cbyte = 1*cfg->C;
  } else if(cfg->dt == DataType_Int16) {
    cfg_reg->rOpM = 0X00200000;
    Cbyte = 2*cfg->C;
  } else if(cfg->dt == DataType_Fp16) {
    cfg_reg->rOpM = 0X00400000;
    Cbyte = 2*cfg->C;
  } else {
    rumboot_printf("ERROR: Unsupported data type for PPE!\n");
    return -1;
  }
  Celem = Cbyte/32 - (Cbyte%32 == 0);
  cfg_reg->rESs   = 0X00000020; // 32 bytes, always 16x16 or 32x8
  cfg_reg->rVSs   = Cbyte;                        // bytes of full C (now defined for BOX=CUBE)
  cfg_reg->rLSs   = (cfg_reg->rVSs)*(cfg->W);     // bytes of full C*W (now defined for BOX=CUBE)
  cfg_reg->rPCi   = Celem*32;                     // bytes for Celem - 1
  cfg_reg->rPWi   = (cfg_reg->rVSs)*(cfg->W - 1); // bytes for C*(W-1)
  cfg_reg->rPHi   = (cfg_reg->rLSs)*(cfg->H - 1); // bytes for C*W*(H-1)
  cfg_reg->rBSWi  = 0X00001FFF & (cfg->W - 1);    // W elements -1
  cfg_reg->rBSHi  = 0X00001FFF & (cfg->H - 1);    // H elements -1
  cfg_reg->rBSCi  = 0X00000000; // Celem;         // C elements -1 for FLYING_MODE = FLYING_BOXED (not supported yet), else 0
  cfg_reg->rStWi  = 0X00001FFF & (cfg->W - 1);    // W elements -1 in first box
  cfg_reg->rOfWi  = 0X00000000;                   // W elements addon between boxes
  // ppe+wdma
  if(cfg_reg->rOpEn == 0){
    cfg_reg->wOpM = 0X00000010;
  } else {
    cfg_reg->wOpM = 0X00000020;
  }
  // cfg_reg->wOpEn  = 0;
  // cfg_reg->wBALd  = 0;
  if(cfg->dt == DataType_Int8) {
    cfg_reg->wOpM = (0x00000030 & cfg_reg->wOpM) | 0X00000000;
    Cbyte = 1*out_cube_metrics->C;
  } else if(cfg->dt == DataType_Int16) {
    cfg_reg->wOpM = (0x00000030 & cfg_reg->wOpM) | 0X00200000;
    Cbyte = 2*out_cube_metrics->C;
  } else if(cfg->dt == DataType_Fp16) {
    cfg_reg->wOpM = (0x00000030 & cfg_reg->wOpM) | 0X00400000;
    Cbyte = 2*out_cube_metrics->C;
  } else {
    rumboot_printf("ERROR: Unsupported data type for PPE!\n");
    return -1;
  }
  Celem = Cbyte/32 - (Cbyte%32 == 0);
  if(cfg->meth == PoolingOperationSwitch_Avg) {
    cfg_reg->wOpM = (0x00600000 & cfg_reg->wOpM) | 0X00000000;
  } else if(cfg->meth == PoolingOperationSwitch_Max) {
    cfg_reg->wOpM = (0x00600000 & cfg_reg->wOpM) | 0X00000001;
  } else if(cfg->meth == PoolingOperationSwitch_Min) {
    cfg_reg->wOpM = (0x00600000 & cfg_reg->wOpM) | 0X00000002;
  } else {
    rumboot_printf("ERROR: Unsupported pooling method type for PPE!\n");
    return -1;
  }
  cfg_reg->wWi    = 0X00001FFF & (cfg->W - 1);
  cfg_reg->wHi    = 0X00001FFF & (cfg->H - 1);
  cfg_reg->wCi    = 0X00001FFF & (cfg->C - 1);
  cfg_reg->wWo    = 0X00001FFF & (out_cube_metrics->W - 1);
  cfg_reg->wHo    = 0X00001FFF & (out_cube_metrics->H - 1);
  cfg_reg->wCo    = 0X00001FFF & (out_cube_metrics->C - 1);
  cfg_reg->wESd   = 0X00000020;                                 // 32 bytes, always 16x16 or 32x8
  cfg_reg->wVSd   = Cbyte;                                      // bytes of full C (now defined for BOX=CUBE)
  cfg_reg->wLSd   = (cfg_reg->wVSd)*(out_cube_metrics->W);      // bytes of full C*W (now defined for BOX=CUBE)
  cfg_reg->wPCo   = Celem*32;                                   // bytes for Celem - 1
  cfg_reg->wPWo   = (cfg_reg->wVSd)*(out_cube_metrics->W - 1);  // bytes for C*(W-1)
  cfg_reg->wPHo   = (cfg_reg->wLSd)*(out_cube_metrics->H - 1);  // bytes for C*W*(H-1)
  cfg_reg->wBSWi  = cfg_reg->rBSWi;
  cfg_reg->wBSHi  = cfg_reg->rBSHi;
  cfg_reg->wBSCi  = cfg_reg->rBSCi;
  cfg_reg->wStWi  = cfg_reg->rStWi;
  cfg_reg->wOfWi  = cfg_reg->rOfWi;
  cfg_reg->wBSWo  = 0X00001FFF & (out_cube_metrics->W - 1);     // W elements -1
  cfg_reg->wBSHo  = 0X00001FFF & (out_cube_metrics->H - 1);     // H elements -1
  cfg_reg->wBSCo  = 0X00000000; // Celem;                       // C elements -1 for FLYING_MODE = FLYING_BOXED (not supported yet), else 0
  cfg_reg->wStWo  = 0X00001FFF & (out_cube_metrics->W - 1);     // W elements -1 in first box
  cfg_reg->wOfWo  = 0X00000000;                                 // W elements addon between boxes
  cfg_reg->wK     = (0X00F00000 & (cfg->Sh << 20)) | (0X000F0000 & (cfg->Sw << 16)) | (0X00000700 & (cfg->Kh << 8)) | (0X00000007 & cfg->Kw);
  cfg_reg->wKWr   = 0X0000FFFF & cfg->Kw_r;
  cfg_reg->wKHr   = 0X0000FFFF & cfg->Kh_r;
  cfg_reg->wP     = (0X00007000 & (cfg->Bp << 12)) | (0X00000700 & (cfg->Rp << 8)) | (0X00000070 & (cfg->Tp << 4)) | (0X00000007 & cfg->Lp);
  cfg_reg->wPV1   = 0X0007FFFF & cfg->pv[0];
  cfg_reg->wPV2   = 0X0007FFFF & cfg->pv[1];
  cfg_reg->wPV3   = 0X0007FFFF & cfg->pv[2];
  cfg_reg->wPV4   = 0X0007FFFF & cfg->pv[3];
  cfg_reg->wPV5   = 0X0007FFFF & cfg->pv[4];
  cfg_reg->wPV6   = 0X0007FFFF & cfg->pv[5];
  cfg_reg->wPV7   = 0X0007FFFF & cfg->pv[6];
  return 0;
}

void nu_ppe_setup_reg(uintptr_t rbase, uintptr_t wbase, ConfigREGPPE* cfg) {
  rumboot_printf("Configuring PPE regs..\n");
  // rdma
  // iowrite32(cfg->rSt,      rbase + NU_PPE_RDMA_STATUS);
  iowrite32(0X00000000,   rbase + NU_PPE_RDMA_OP_ENABLE);
  iowrite32(cfg->rPWi,    rbase + NU_PPE_RDMA_PLANE_W_IN);
  iowrite32(cfg->rPHi,    rbase + NU_PPE_RDMA_PLANE_H_IN);
  iowrite32(cfg->rPCi,    rbase + NU_PPE_RDMA_PLANE_C_IN);
  iowrite32(cfg->rBALs,   rbase + NU_PPE_RDMA_SRC_BASE_ADDR_L);
  // iowrite32(cfg->rBAHs,    rbase + NU_PPE_RDMA_SRC_BASE_ADDR_H);
  iowrite32(cfg->rVSs,    rbase + NU_PPE_RDMA_SRC_VECTOR_STRIDE);
  iowrite32(cfg->rLSs,    rbase + NU_PPE_RDMA_SRC_LINE_STRIDE);
  iowrite32(cfg->rESs,    rbase + NU_PPE_RDMA_SRC_ELEMENT_STRIDE);
  iowrite32(cfg->rOpM,    rbase + NU_PPE_RDMA_OP_MODE);
  iowrite32(cfg->rBSWi,   rbase + NU_PPE_RDMA_BOX_SIZE_W_IN);
  iowrite32(cfg->rBSHi,   rbase + NU_PPE_RDMA_BOX_SIZE_H_IN);
  iowrite32(cfg->rBSCi,   rbase + NU_PPE_RDMA_BOX_SIZE_C_IN);
  iowrite32(cfg->rStWi,   rbase + NU_PPE_RDMA_BOX_START_W_IN);
  iowrite32(cfg->rOfWi,   rbase + NU_PPE_RDMA_BOX_OFFSET_W_IN);
  // iowrite32(cfg->rK,      rbase + NU_PPE_RDMA_KERNEL);
  // ppe + wdma
  // iowrite32(cfg->wSt,      wbase + NU_PPE_STATUS);
  iowrite32(0X00000000,   wbase + NU_PPE_OP_ENABLE);
  iowrite32(cfg->wWi,     wbase + NU_PPE_DATA_W_IN);
  iowrite32(cfg->wHi,     wbase + NU_PPE_DATA_H_IN);
  iowrite32(cfg->wCi,     wbase + NU_PPE_DATA_C_IN);
  iowrite32(cfg->wWo,     wbase + NU_PPE_DATA_W_OUT);
  iowrite32(cfg->wHo,     wbase + NU_PPE_DATA_H_OUT);
  iowrite32(cfg->wCo,     wbase + NU_PPE_DATA_C_OUT);
  iowrite32(cfg->wPWo,    wbase + NU_PPE_PLANE_W_OUT);
  iowrite32(cfg->wPHo,    wbase + NU_PPE_PLANE_H_OUT);
  iowrite32(cfg->wPCo,    wbase + NU_PPE_PLANE_C_OUT);
  iowrite32(cfg->wBALd,   wbase + NU_PPE_DST_BASE_ADDR_L);
  // iowrite32(cfg->wBAHd,    wbase + NU_PPE_DST_BASE_ADDR_H);
  iowrite32(cfg->wVSd,    wbase + NU_PPE_DST_VECTOR_STRIDE);
  iowrite32(cfg->wLSd,    wbase + NU_PPE_DST_LINE_STRIDE);
  iowrite32(cfg->wESd,    wbase + NU_PPE_DST_ELEMENT_STRIDE);
  iowrite32(cfg->wOpM,    wbase + NU_PPE_OP_MODE);
  iowrite32(cfg->wBSWi,   wbase + NU_PPE_BOX_SIZE_W_IN);
  iowrite32(cfg->wBSHi,   wbase + NU_PPE_BOX_SIZE_H_IN);
  iowrite32(cfg->wBSCi,   wbase + NU_PPE_BOX_SIZE_C_IN);
  iowrite32(cfg->wStWi,   wbase + NU_PPE_BOX_START_W_IN);
  iowrite32(cfg->wOfWi,   wbase + NU_PPE_BOX_OFFSET_W_IN);
  iowrite32(cfg->wBSWo,   wbase + NU_PPE_BOX_SIZE_W_OUT);
  iowrite32(cfg->wBSHo,   wbase + NU_PPE_BOX_SIZE_H_OUT);
  iowrite32(cfg->wBSCo,   wbase + NU_PPE_BOX_SIZE_C_OUT);
  iowrite32(cfg->wStWo,   wbase + NU_PPE_BOX_START_W_OUT);
  iowrite32(cfg->wOfWo,   wbase + NU_PPE_BOX_OFFSET_W_OUT);
  iowrite32(cfg->wK,      wbase + NU_PPE_KERNEL);
  iowrite32(cfg->wKWr,    wbase + NU_PPE_RECIP_KERNEL_W);
  iowrite32(cfg->wKHr,    wbase + NU_PPE_RECIP_KERNEL_H);
  iowrite32(cfg->wP,      wbase + NU_PPE_PADDING);
  iowrite32(cfg->wPV1,    wbase + NU_PPE_PADDING_VALUE_1);
  iowrite32(cfg->wPV2,    wbase + NU_PPE_PADDING_VALUE_2);
  iowrite32(cfg->wPV3,    wbase + NU_PPE_PADDING_VALUE_3);
  iowrite32(cfg->wPV4,    wbase + NU_PPE_PADDING_VALUE_4);
  iowrite32(cfg->wPV5,    wbase + NU_PPE_PADDING_VALUE_5);
  iowrite32(cfg->wPV6,    wbase + NU_PPE_PADDING_VALUE_6);
  iowrite32(cfg->wPV7,    wbase + NU_PPE_PADDING_VALUE_7);
  // iowrite32(cfg->wINi,     wbase + NU_PPE_INF_NUM_IN);
  // iowrite32(cfg->wNNi,     wbase + NU_PPE_NAN_NUM_IN);
  // iowrite32(cfg->wNNo,     wbase + NU_PPE_NAN_NUM_OUT);
}
// rdma
void nu_ppe_rdma_run(uintptr_t rbase, ConfigREGPPE* cfg) {
  rumboot_printf("Start PPE RDMA...\n");
  iowrite32(cfg->rOpEn,   rbase + NU_PPE_RDMA_OP_ENABLE);
}
void nu_ppe_rdma_wait_complete(uintptr_t rbase){
  rumboot_printf("Wait PPE RDMA...\n");
  while(ioread32(rbase + NU_PPE_RDMA_STATUS) !=0) {}
  rumboot_printf("Done PPE RDMA...\n");
}
// ppe + wdma
void nu_ppe_run(uintptr_t wbase, ConfigREGPPE* cfg) {
  rumboot_printf("Start PPE + WDMA...\n");
  iowrite32(cfg->wOpEn,   wbase + NU_PPE_OP_ENABLE);
}
void nu_ppe_wait_complete(uintptr_t wbase){
  rumboot_printf("Wait PPE + WDMA...\n");
  while(ioread32(wbase + NU_PPE_STATUS) !=0) {}
  rumboot_printf("Done PPE + WDMA...\n");
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

void nu_vpe_run(uintptr_t vpe_base, ConfigDMAVPE* cfg) {
  // Напиши сюда 3апуск DMA
}

void nu_vpe_wait(uintptr_t vpe_base, ConfigDMAVPE* cfg){
  // Напиши сюда 3авершение DMA
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

void nu_mpe_config_wr_main_channel(uintptr_t dma_base, void *addr, int size){
  nu_cpdmac_rcv512_config(dma_base,addr,size);
}

void nu_mpe_run_wr_main_channel(uintptr_t dma_base) {
  nu_cpdmac_rcv512_run(dma_base);
}
void nu_mpe_wait_wr_main_channel_complete(uintptr_t dma_base) {
  nu_cpdmac_rcv512_wait_complete(dma_base);
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


// Type conversion for VPE
bool nu_vpe_mode_to_bool (Mode in_mode){
  bool res;
  if (in_mode == Mode_Unitary) res = 0;
  else                         res = 1;
  return res;
}
