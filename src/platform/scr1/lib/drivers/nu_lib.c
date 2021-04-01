

#include <stdint.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <regs/regs_nu_mpe.h>
#include <regs/regs_nu_vpe.h>
#include <regs/regs_nu_ppe.h>

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

    cfg-> H      =*ptr;ptr++;
    cfg-> W      =*ptr;ptr++;
    cfg-> C      =*ptr;ptr++;
    cfg-> Kh     =*ptr;ptr++;
    cfg-> Kw     =*ptr;ptr++;
    cfg-> Sh     =*ptr;ptr++;
    cfg-> Sw     =*ptr;ptr++;
    cfg-> Kh_r   =*ptr;ptr++;
    cfg-> Kw_r   =*ptr;ptr++;
    cfg-> Tp     =*ptr;ptr++;
    cfg-> Bp     =*ptr;ptr++;
    cfg-> Lp     =*ptr;ptr++;
    cfg-> Rp     =*ptr;ptr++;
    cfg-> pv[0]  =*ptr;ptr++;
    cfg-> pv[1]  =*ptr;ptr++;
    cfg-> pv[2]  =*ptr;ptr++;
    cfg-> pv[3]  =*ptr;ptr++;
    cfg-> pv[4]  =*ptr;ptr++;
    cfg-> pv[5]  =*ptr;ptr++;
    cfg-> pv[6]  =*ptr;ptr++;
    cfg-> meth   =*ptr;ptr++;
    cfg-> dt     =*ptr;ptr++;
    cfg-> MC     =*ptr;ptr++;
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
    rumboot_printf("  Kh_r   = %d \n" , cfg->Kh_r);
    rumboot_printf("  Kw_r   = %d \n" , cfg->Kw_r);
    rumboot_printf("  Tp     = %d \n" , cfg->Tp);
    rumboot_printf("  Bp     = %d \n" , cfg->Bp);
    rumboot_printf("  Lp     = %d \n" , cfg->Lp);
    rumboot_printf("  Rp     = %d \n" , cfg->Rp);
    rumboot_printf("  pv     = %d %d %d %d %d %d %d \n" , cfg->pv[0], cfg->pv[1], cfg->pv[2], cfg->pv[3], cfg->pv[4], cfg->pv[5], cfg->pv[6]);
    nu_vpe_print_PoolingOperationSwitch(cfg->meth,"meth    ");
    nu_vpe_print_DataType(cfg->dt,"dt      ");
    rumboot_printf("  MC     = %d \n" , cfg->MC);
}

void nu_vpe_setup(uintptr_t base, ConfigVPE* cfg, ConfigDMAVPE* cfg_dma) {
  rumboot_printf("Configuring VPE..\n");

  // iowrite32(cfg->MYFIELD, base + NU_VPE_MYREG);
}

void nu_vpe_decide_dma_config_trivial(ConfigVPE* cfg, CubeMetrics* metrics, ConfigDMAVPE* cfg_dma) {
  cfg_dma->H = metrics->H;
  cfg_dma->W = metrics->W;
  cfg_dma->C = metrics->C;

  if(cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32)
    cfg_dma->dma_src_en =  Enable_NotEn ;  // if 32bit Input Data then Main Channel Works
  else
    cfg_dma->dma_src_en = Enable_En;

  if((cfg->op0_config.alu_en == Enable_En && cfg->op0_config.alu_mode != Mode_Unitary) ||
     (cfg->op0_config.mux_en == Enable_En && cfg->op0_config.mux_mode != Mode_Unitary)  )// if Some Of Operands Enabled And Not A Single Value
    cfg_dma->dma_op0_en = Enable_En;
  else
    cfg_dma->dma_op0_en = Enable_NotEn;

  if((cfg->op1_config.alu_en == Enable_En && cfg->op1_config.alu_mode != Mode_Unitary) ||
     (cfg->op1_config.mux_en == Enable_En && cfg->op1_config.mux_mode != Mode_Unitary)  )// if Some Of Operands Enabled And Not A Single Value
    cfg_dma->dma_op1_en = Enable_En;
  else
    cfg_dma->dma_op1_en = Enable_NotEn;

  if((cfg->op2_config.alu_en == Enable_En && cfg->op2_config.alu_mode != Mode_Unitary) ||
     (cfg->op2_config.mux_en == Enable_En && cfg->op2_config.mux_mode != Mode_Unitary)  )// if Some Of Operands Enabled And Not A Single Value
    cfg_dma->dma_op2_en = Enable_En;
  else
    cfg_dma->dma_op2_en = Enable_NotEn;


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

void nu_ppe_setup(uintptr_t base, ConfigPPE* cfg) {
  rumboot_printf("Configuring PPE..\n");

  // iowrite32(cfg->MYFIELD, base + NU_MPE_MYREG);
}

nu_ppe_setup_reg(uintptr_t rbase, uintptr_t wbase, ConfigREGPPE* cfg) {
  rumboot_printf("Configuring PPE regs..\n");
  // rdma
  // iowrite32(cfg->rSt,      rbase + NU_PPE_RDMA_STATUS);
  iowrite32(0X00000000,   rbase + NU_PPE_RDMA_OP_ENABLE);
  iowrite32(cfg->rWi,     rbase + NU_PPE_RDMA_DATA_W_IN);
  iowrite32(cfg->rHi,     rbase + NU_PPE_RDMA_DATA_H_IN);
  iowrite32(cfg->rCi,     rbase + NU_PPE_RDMA_DATA_C_IN);
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
  iowrite32(cfg->rK,      rbase + NU_PPE_RDMA_KERNEL);
  // ppe + wdma
  // iowrite32(cfg->wSt,      wbase + NU_PPE_STATUS);
  iowrite32(0X00000000,   wbase + NU_PPE_OP_ENABLE);
  iowrite32(cfg->wWi,     wbase + NU_PPE_DATA_W_IN);
  iowrite32(cfg->wHi,     wbase + NU_PPE_DATA_H_IN);
  iowrite32(cfg->wCi,     wbase + NU_PPE_DATA_C_IN);
  iowrite32(cfg->wWo,     wbase + NU_PPE_DATA_W_OUT);
  iowrite32(cfg->wHo,     wbase + NU_PPE_DATA_H_OUT);
  iowrite32(cfg->wCo,     wbase + NU_PPE_DATA_C_OUT);
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
nu_ppe_rdma_run(uintptr_t rbase, ConfigREGPPE* cfg) {
  rumboot_printf("Start PPE RDMA...\n");
  // rdma
  iowrite32(cfg->rOpEn,   rbase + NU_PPE_RDMA_OP_ENABLE);
}
nu_ppe_run(uintptr_t wbase, ConfigREGPPE* cfg) {
  rumboot_printf("Start PPE + WDMA...\n");
  // ppe + wdma
  iowrite32(cfg->wOpEn,   wbase + NU_PPE_OP_ENABLE);
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
