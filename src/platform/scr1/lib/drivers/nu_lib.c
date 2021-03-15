

#include <stdint.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices/nu_cpdmac_lib.h> 
#include <platform/devices/nu_lib.h> 

void nu_vpe_load_config(ConfigVPE* cfg, void* cfg_bin) {
  int32_t* ptr;
  
  ptr = (int32_t*) cfg_bin;
  
  cfg->data_type=*ptr;ptr++;
  cfg->flying=*ptr;ptr++;
  cfg->op0_en=*ptr;ptr++;
  cfg->op1_en=*ptr;ptr++;
  cfg->op2_en=*ptr;ptr++;
  cfg->c3_offset=*ptr;ptr++;
  cfg->c3_scale=*ptr;ptr++;
  cfg->c3_trunc=*ptr;ptr++;
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
      "Int32",
      "Fp32"
    };

    rumboot_printf("  %s = %s \n", name, DataTypeExtNames[(int) data_type] );
  }


void nu_vpe_print_config(ConfigVPE* cfg){
  rumboot_printf("ConfigVPE:\n");
  nu_vpe_print_DataType(cfg->data_type,"data_type");
  nu_vpe_print_Enable(cfg->flying,"flying");
  nu_vpe_print_Enable(cfg->op0_en,"op0_en");
  nu_vpe_print_Enable(cfg->op1_en,"op1_en");
  nu_vpe_print_Enable(cfg->op2_en,"op2_en");
  rumboot_printf("  c3_offset = %d\n" , cfg->c3_offset);
  rumboot_printf("  c3_scale = %d\n" , cfg->c3_scale);
  rumboot_printf("  c3_trunc = %d\n" , cfg->c3_trunc);
  
  rumboot_printf("ConfigOp01 (0):\n");
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

void nu_vpe_setup(uintptr_t base, ConfigVPE* cfg) {
  rumboot_printf("Configuring VPE..\n");
  
  // iowrite32(cfg->MYFIELD, base + MYREG);
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

void nu_vpe_config_wr_main_channel(uintptr_t dma_base, void *addr, int size){
  nu_cpdmac_rcv256_config(dma_base,addr,size);
}

void nu_vpe_run_wr_main_channel(uintptr_t dma_base) {
  nu_cpdmac_rcv256_run(dma_base);
}
void nu_vpe_wait_wr_main_channel_complete(uintptr_t dma_base) {
  nu_cpdmac_rcv256_wait_complete(dma_base);
}


