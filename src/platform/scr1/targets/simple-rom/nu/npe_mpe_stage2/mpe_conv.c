
#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices/nu_lib.h>
#include <platform/devices/nu_test_lib.h>
#include <regs/regs_na.h>
#include <platform/devices.h>

void nu_run_mpe_cmd(uintptr_t base, void* cmd, MPECmdMetrics* cmd_metrics, uintptr_t mpe_dma_r_baddr, uintptr_t mpe_dma_w_baddr) {
  uint32_t offset;
  uint32_t data;
  int num_cmds;
  uint32_t* ptr;
  uint32_t result;
  ptr = (uint32_t*) cmd;
  num_cmds = (cmd_metrics->s / 8);
  for(int i=0;i<num_cmds;i++) {
    data = *ptr;
    ptr++;
    offset = *ptr;

    ptr++;
    rumboot_printf("Writing data=%x addr = %x  num_cmds=%x\n", data, offset, num_cmds);
    if(offset == MPE_RDMA_D_BASE+BFCA_MSha) {data = mpe_dma_r_baddr;}
    if(offset == MPE_RDMA_W_BASE+BFCA_MSha) {data = mpe_dma_w_baddr;}
    iowrite32(data,base + offset);
  }
}

void vpe_config_and_start(uint16_t* vpe_ptr, DataTypeExt in_data_type, DataType out_data_type, uint32_t res_size) {
  ConfigVPE* cfg;
  CubeMetrics* metrics;

  cfg = rumboot_malloc_from_heap(0,sizeof(ConfigVPE));
  metrics = rumboot_malloc_from_heap(0,sizeof(CubeMetrics));

  cfg->in_data_type                         = in_data_type                  ; // DataTypeExt_Int32 or DataTypeExt_Fp32
  cfg->out_data_type                        = out_data_type                 ; // DataType_Int8 or DataType_Int16 or DataType_Fp16
  cfg->src_flying                           = Enable_En                     ;
  cfg->dst_flying                           = Enable_NotEn                  ;
  cfg->op0_en                               = Enable_NotEn                  ;
  cfg->op1_en                               = Enable_NotEn                  ;
  cfg->op2_en                               = Enable_NotEn                  ;
  cfg->c3_offset                            = 0                             ; // set it as you wish
  cfg->c3_scale                             = 1                             ; // set it as you wish
#ifdef TRUNC16
  cfg->c3_trunc                             = 16                            ; // High 16 bits
#else
  cfg->c3_trunc                             = 0                             ; // Low 16 bits
#endif
  cfg->c3_satur_en                          = Enable_NotEn                  ; // set it as you wish
  cfg->c3_round_mode                        = RoundMode_Down                ; // set it as you wish
  cfg->nan_to_zero_input                    = Enable_NotEn                  ; // set it as you wish
  cfg->nan_to_zero_output                   = Enable_NotEn                  ; // set it as you wish
  cfg->op0_config.coef_type                 = DataType_Int8                 ;
  cfg->op0_config.alu_en                    = Enable_NotEn                  ;
  cfg->op0_config.mux_en                    = Enable_NotEn                  ;
  cfg->op0_config.relu_en                   = Enable_NotEn                  ;
  cfg->op0_config.prelu_en                  = Enable_NotEn                  ;
  cfg->op0_config.alu_operation             = AluOperationSwitch_Sum        ;
  cfg->op0_config.lshift_value              = 0                             ;
  cfg->op0_config.alu_mode                  = Mode_Unitary                  ;
  cfg->op0_config.alu_value                 = 0                             ;
  cfg->op0_config.mux_mode                  = Mode_Unitary                  ;
  cfg->op0_config.mux_value                 = 0                             ;
  cfg->op0_config.norm_round_mode           = RoundMode_HalfAwayFromZero    ;
  cfg->op0_config.norm_saturation_en        = Enable_NotEn                  ;
  cfg->op0_config.norm_round_size           = 0                             ;
  cfg->op1_config.coef_type                 = DataType_Int8                 ;
  cfg->op1_config.alu_en                    = Enable_NotEn                  ;
  cfg->op1_config.mux_en                    = Enable_NotEn                  ;
  cfg->op1_config.relu_en                   = Enable_NotEn                  ;
  cfg->op1_config.prelu_en                  = Enable_NotEn                  ;
  cfg->op1_config.alu_operation             = AluOperationSwitch_Sum        ;
  cfg->op1_config.lshift_value              = 0                             ;
  cfg->op1_config.alu_mode                  = Mode_Unitary                  ;
  cfg->op1_config.alu_value                 = 0                             ;
  cfg->op1_config.mux_mode                  = Mode_Unitary                  ;
  cfg->op1_config.mux_value                 = 0                             ;
  cfg->op1_config.norm_round_mode           = RoundMode_HalfAwayFromZero    ;
  cfg->op1_config.norm_saturation_en        = Enable_NotEn                  ;
  cfg->op1_config.norm_round_size           = 0                             ;
  cfg->op2_config.coef_type                 = DataType_Int8                 ;
  cfg->op2_config.alu_en                    = Enable_NotEn                  ;
  cfg->op2_config.mux_en                    = Enable_NotEn                  ;
  cfg->op2_config.lut_en                    = Enable_NotEn                  ;
  cfg->op2_config.prelu_en                  = Enable_NotEn                  ;
  cfg->op2_config.alu_operation             = AluOperationExtSwitch_Sum     ;
  cfg->op2_config.c1_en                     = Enable_NotEn                  ;
  cfg->op2_config.c2_en                     = Enable_NotEn                  ;
  cfg->op2_config.alu_mode                  = Mode_Unitary                  ;
  cfg->op2_config.alu_value                 = 0                             ;
  cfg->op2_config.mux_mode                  = Mode_Unitary                  ;
  cfg->op2_config.mux_value                 = 0                             ;
  cfg->op2_config.norm_round_mode           = RoundMode_HalfAwayFromZero    ;
  cfg->op2_config.norm_saturation_en        = Enable_NotEn                  ;
  cfg->op2_config.norm_round_size           = 0                             ;
  cfg->op2_config.c1_offset                 = 0                             ;
  cfg->op2_config.c1_scale                  = 0                             ;
  cfg->op2_config.c1_trunc                  = 0                             ;
  cfg->op2_config.c2_offset                 = 0                             ;
  cfg->op2_config.c2_scale                  = 0                             ;
  cfg->op2_config.c2_trunc                  = 0                             ;
  cfg->op2_config.lut_sel                   = 0                             ;
  cfg->op2_config.lut_right_priority        = 0                             ;
  cfg->op2_config.lut_left_priority         = 0                             ;
  cfg->op2_config.lut_tab1_x_start          = 0                             ;
  cfg->op2_config.lut_tab1_x_end            = 0                             ;
  cfg->op2_config.lut_tab2_x_start          = 0                             ;
  cfg->op2_config.lut_tab2_x_end            = 0                             ;
  cfg->op2_config.lut_tab1_slope_ovrf_scale = 0                             ;
  cfg->op2_config.lut_tab1_slope_undf_scale = 0                             ;
  cfg->op2_config.lut_tab2_slope_ovrf_scale = 0                             ;
  cfg->op2_config.lut_tab2_slope_undf_scale = 0                             ;
  cfg->op2_config.lut_tab1_slope_ovrf_shift = 0                             ;
  cfg->op2_config.lut_tab1_slope_undf_shift = 0                             ;
  cfg->op2_config.lut_tab2_slope_ovrf_shift = 0                             ;
  cfg->op2_config.lut_tab2_slope_undf_shift = 0                             ;

  cfg->src_rdma_config.dma_en               = Enable_NotEn                  ;
  cfg->src_rdma_config.dma_ram_type         = DmaRamType_CV                 ;
  cfg->src_rdma_config.dma_data_mode        = Mode_Unitary                  ;
  cfg->src_rdma_config.dma_data_size        = DmaDSize_One_Byte             ;
  cfg->src_rdma_config.dma_data_use         = DmaDUse_Off                   ;
  cfg->src_rdma_config.dma_axi_len          = 0                             ;
  cfg->src_rdma_config.dma_baddr            = 0                             ;
  cfg->src_rdma_config.dma_cube_size_c      = 0                             ;
  cfg->src_rdma_config.dma_cube_size_w      = 0                             ;
  cfg->src_rdma_config.dma_cube_size_h      = 0                             ;
  cfg->src_rdma_config.dma_border_x         = 0                             ;
  cfg->src_rdma_config.dma_border_y         = 0                             ;
  cfg->src_rdma_config.dma_border_z         = 0                             ;
  cfg->src_rdma_config.dma_stride_x         = 0                             ;
  cfg->src_rdma_config.dma_stride_y         = 0                             ;
  cfg->src_rdma_config.dma_stride_z         = 0                             ;
  cfg->src_rdma_config.dma_frag_last_size   = 0                             ;
  cfg->src_rdma_config.dma_frag_size        = 0                             ;
  cfg->src_rdma_config.dma_xyz_drct         = 0                             ;
  cfg->src_rdma_config.dma_box_st_size_x    = 0                             ;
  cfg->src_rdma_config.dma_box_st_size_y    = 0                             ;
  cfg->src_rdma_config.dma_box_st_size_z    = 0                             ;
  cfg->src_rdma_config.dma_box_size_x       = 0                             ;
  cfg->src_rdma_config.dma_box_size_y       = 0                             ;
  cfg->src_rdma_config.dma_box_size_z       = 0                             ;
  cfg->src_rdma_config.dma_box_offset_x     = 0                             ;
  cfg->src_rdma_config.dma_box_offset_y     = 0                             ;
  cfg->src_rdma_config.dma_box_offset_z     = 0                             ;

  cfg->op0_rdma_config.dma_en               = Enable_NotEn                  ;
  cfg->op0_rdma_config.dma_ram_type         = DmaRamType_CV                 ;
  cfg->op0_rdma_config.dma_data_mode        = Mode_Unitary                  ;
  cfg->op0_rdma_config.dma_data_size        = DmaDSize_One_Byte             ;
  cfg->op0_rdma_config.dma_data_use         = DmaDUse_Off                   ;
  cfg->op0_rdma_config.dma_axi_len          = 0                             ;
  cfg->op0_rdma_config.dma_baddr            = 0                             ;
  cfg->op0_rdma_config.dma_cube_size_c      = 0                             ;
  cfg->op0_rdma_config.dma_cube_size_w      = 0                             ;
  cfg->op0_rdma_config.dma_cube_size_h      = 0                             ;
  cfg->op0_rdma_config.dma_border_x         = 0                             ;
  cfg->op0_rdma_config.dma_border_y         = 0                             ;
  cfg->op0_rdma_config.dma_border_z         = 0                             ;
  cfg->op0_rdma_config.dma_stride_x         = 0                             ;
  cfg->op0_rdma_config.dma_stride_y         = 0                             ;
  cfg->op0_rdma_config.dma_stride_z         = 0                             ;
  cfg->op0_rdma_config.dma_frag_last_size   = 0                             ;
  cfg->op0_rdma_config.dma_frag_size        = 0                             ;
  cfg->op0_rdma_config.dma_xyz_drct         = 0                             ;
  cfg->op0_rdma_config.dma_box_st_size_x    = 0                             ;
  cfg->op0_rdma_config.dma_box_st_size_y    = 0                             ;
  cfg->op0_rdma_config.dma_box_st_size_z    = 0                             ;
  cfg->op0_rdma_config.dma_box_size_x       = 0                             ;
  cfg->op0_rdma_config.dma_box_size_y       = 0                             ;
  cfg->op0_rdma_config.dma_box_size_z       = 0                             ;
  cfg->op0_rdma_config.dma_box_offset_x     = 0                             ;
  cfg->op0_rdma_config.dma_box_offset_y     = 0                             ;
  cfg->op0_rdma_config.dma_box_offset_z     = 0                             ;

  cfg->op1_rdma_config.dma_en               = Enable_NotEn                  ;
  cfg->op1_rdma_config.dma_ram_type         = DmaRamType_CV                 ;
  cfg->op1_rdma_config.dma_data_mode        = Mode_Unitary                  ;
  cfg->op1_rdma_config.dma_data_size        = DmaDSize_One_Byte             ;
  cfg->op1_rdma_config.dma_data_use         = DmaDUse_Off                   ;
  cfg->op1_rdma_config.dma_axi_len          = 0                             ;
  cfg->op1_rdma_config.dma_baddr            = 0                             ;
  cfg->op1_rdma_config.dma_cube_size_c      = 0                             ;
  cfg->op1_rdma_config.dma_cube_size_w      = 0                             ;
  cfg->op1_rdma_config.dma_cube_size_h      = 0                             ;
  cfg->op1_rdma_config.dma_border_x         = 0                             ;
  cfg->op1_rdma_config.dma_border_y         = 0                             ;
  cfg->op1_rdma_config.dma_border_z         = 0                             ;
  cfg->op1_rdma_config.dma_stride_x         = 0                             ;
  cfg->op1_rdma_config.dma_stride_y         = 0                             ;
  cfg->op1_rdma_config.dma_stride_z         = 0                             ;
  cfg->op1_rdma_config.dma_frag_last_size   = 0                             ;
  cfg->op1_rdma_config.dma_frag_size        = 0                             ;
  cfg->op1_rdma_config.dma_xyz_drct         = 0                             ;
  cfg->op1_rdma_config.dma_box_st_size_x    = 0                             ;
  cfg->op1_rdma_config.dma_box_st_size_y    = 0                             ;
  cfg->op1_rdma_config.dma_box_st_size_z    = 0                             ;
  cfg->op1_rdma_config.dma_box_size_x       = 0                             ;
  cfg->op1_rdma_config.dma_box_size_y       = 0                             ;
  cfg->op1_rdma_config.dma_box_size_z       = 0                             ;
  cfg->op1_rdma_config.dma_box_offset_x     = 0                             ;
  cfg->op1_rdma_config.dma_box_offset_y     = 0                             ;
  cfg->op1_rdma_config.dma_box_offset_z     = 0                             ;

  cfg->op2_rdma_config.dma_en               = Enable_NotEn                  ;
  cfg->op2_rdma_config.dma_ram_type         = DmaRamType_CV                 ;
  cfg->op2_rdma_config.dma_data_mode        = Mode_Unitary                  ;
  cfg->op2_rdma_config.dma_data_size        = DmaDSize_One_Byte             ;
  cfg->op2_rdma_config.dma_data_use         = DmaDUse_Off                   ;
  cfg->op2_rdma_config.dma_axi_len          = 0                             ;
  cfg->op2_rdma_config.dma_baddr            = 0                             ;
  cfg->op2_rdma_config.dma_cube_size_c      = 0                             ;
  cfg->op2_rdma_config.dma_cube_size_w      = 0                             ;
  cfg->op2_rdma_config.dma_cube_size_h      = 0                             ;
  cfg->op2_rdma_config.dma_border_x         = 0                             ;
  cfg->op2_rdma_config.dma_border_y         = 0                             ;
  cfg->op2_rdma_config.dma_border_z         = 0                             ;
  cfg->op2_rdma_config.dma_stride_x         = 0                             ;
  cfg->op2_rdma_config.dma_stride_y         = 0                             ;
  cfg->op2_rdma_config.dma_stride_z         = 0                             ;
  cfg->op2_rdma_config.dma_frag_last_size   = 0                             ;
  cfg->op2_rdma_config.dma_frag_size        = 0                             ;
  cfg->op2_rdma_config.dma_xyz_drct         = 0                             ;
  cfg->op2_rdma_config.dma_box_st_size_x    = 0                             ;
  cfg->op2_rdma_config.dma_box_st_size_y    = 0                             ;
  cfg->op2_rdma_config.dma_box_st_size_z    = 0                             ;
  cfg->op2_rdma_config.dma_box_size_x       = 0                             ;
  cfg->op2_rdma_config.dma_box_size_y       = 0                             ;
  cfg->op2_rdma_config.dma_box_size_z       = 0                             ;
  cfg->op2_rdma_config.dma_box_offset_x     = 0                             ;
  cfg->op2_rdma_config.dma_box_offset_y     = 0                             ;
  cfg->op2_rdma_config.dma_box_offset_z     = 0                             ;

  //-----------------------------------------------------------------------------------------------------
  uint32_t elem_size;

  if   (cfg->dst_flying == Enable_En) cfg->wdma_config.dma_en = Enable_NotEn;
  else                                cfg->wdma_config.dma_en = Enable_En   ;

  // dma_data_size definition------------------- 
  if (cfg->out_data_type == DataType_Int8) cfg->wdma_config.dma_data_size = DmaDSize_One_Byte;
  else                                     cfg->wdma_config.dma_data_size = DmaDSize_Two_Byte;
  
  // elem_size definition-------------------
  if   (cfg->wdma_config.dma_data_size == DmaDSize_Two_Byte) elem_size = 2;
  else                                                       elem_size = 1;

  metrics->C = res_size/elem_size;
  metrics->W = 1;
  metrics->H = 1;
  
  // CUBE_SIZE in vectors
  cfg->cube_size = (metrics->C/16 + ((metrics->C%16) != 0 ? 1 : 0)) * metrics->W * metrics->H - 1;
  
  cfg->wdma_config.dma_en                   = Enable_En                                                    ;
  cfg->wdma_config.dma_ram_type             = DmaRamType_CV                                                ;
  cfg->wdma_config.dma_data_mode            = Mode_Element                                                 ;
//cfg->wdma_config.dma_data_size            = DmaDSize_Two_Byte                                            ; // or DmaDSize_One_Byte. It depends from cfg->out_data_type value.
  cfg->wdma_config.dma_data_use             = DmaDUse_Mux                                                  ;
  cfg->wdma_config.dma_axi_len              = 3                                                            ; // or 0 or 1 or 2
  cfg->wdma_config.dma_baddr                = (uint32_t)vpe_ptr                                            ; // set it according with malloc address
  cfg->wdma_config.dma_cube_size_c          = metrics->C                                                   ;
  cfg->wdma_config.dma_cube_size_w          = metrics->W                                                   ;
  cfg->wdma_config.dma_cube_size_h          = metrics->H                                                   ;
  cfg->wdma_config.dma_border_x             = (metrics->C/4096 + ((metrics->C%4096) != 0 ? 1 : 0) - 1)*(4096*elem_size);                                                     ;
  cfg->wdma_config.dma_border_y             = 0                                                            ;
  cfg->wdma_config.dma_border_z             = 0                                                            ;
  cfg->wdma_config.dma_stride_x             = 4096*elem_size                                               ;
  cfg->wdma_config.dma_stride_y             = 0                                                            ;
  cfg->wdma_config.dma_stride_z             = 0                                                            ;
  cfg->wdma_config.dma_frag_last_size       = (metrics->C%4096 + ((metrics->C%4096) == 0)*4096) * elem_size;
  cfg->wdma_config.dma_frag_size            = 4096*elem_size                                               ;
  cfg->wdma_config.dma_xyz_drct             = 0                                                            ;
  cfg->wdma_config.dma_box_st_size_x        = 0                                                            ;
  cfg->wdma_config.dma_box_st_size_y        = 0                                                            ;
  cfg->wdma_config.dma_box_st_size_z        = 0                                                            ;
  cfg->wdma_config.dma_box_size_x           = 0                                                            ;
  cfg->wdma_config.dma_box_size_y           = 0                                                            ;
  cfg->wdma_config.dma_box_size_z           = 0                                                            ;
  cfg->wdma_config.dma_box_offset_x         = 0                                                            ;
  cfg->wdma_config.dma_box_offset_y         = 0                                                            ;
  cfg->wdma_config.dma_box_offset_z         = 0                                                            ;

  nu_vpe_setup(NPE_BASE+NA_VPE_BASE,cfg);
  nu_vpe_run(NPE_BASE+NA_VPE_BASE,cfg);
}

CubeMetrics* in_metrics;
void *in_data;

WarrMetrics* warr_metrics;
void *warr;

MPECmdMetrics* cmd_metrics;
void *cmd;

CubeMetrics* res_metrics;
uint16_t* res_data;
uint16_t* etalon;

int main()
{
  int heap_id;

  rumboot_printf("START MPE CONV TEST\n");

  heap_id = nu_get_heap_id();

  cmd_metrics = nu_mpe_load_cmd_metrics(heap_id);
  if(cmd_metrics == NULL) return -1;
  
  cmd = nu_mpe_load_cmd(heap_id,cmd_metrics);
  if(cmd == NULL) return -1;

  in_metrics = nu_mpe_load_in_metrics(heap_id);
  if(in_metrics == NULL) return -1;
  
  in_data = nu_mpe_load_in_data(heap_id,in_metrics);
  if(in_data == NULL) return -1;

  warr_metrics = nu_mpe_load_warr_metrics(heap_id);
  if(warr_metrics == NULL) return -1;
  
  warr = nu_mpe_load_warr(heap_id,warr_metrics);
  if(warr == NULL) return -1;

  res_metrics= nu_mpe_load_res_metrics(heap_id);
  if(res_metrics == NULL) return -1;

  res_data = nu_mpe_malloc_res(heap_id,res_metrics);
  if(res_data == NULL) return -1;

  etalon = nu_mpe_load_etalon(heap_id,res_metrics);
  if(etalon == NULL) return -1;

  // MPE
  nu_run_mpe_cmd(NPE_BASE+NA_MPE_BASE,cmd,cmd_metrics,(uint32_t)in_data,(uint32_t)warr);
  
  // VPE
  DataTypeExt in_data_type = DataTypeExt_Int32;
  DataType out_data_type = DataType_Int16;

  vpe_config_and_start(res_data,in_data_type,out_data_type,res_metrics->s);

  // Wait VPE DMA ends
  while (((ioread32(NPE_BASE+NA_VPE_BASE+NU_VPE_NEXT_CNTX)>>12)&0x1)) {};
  rumboot_printf("End VPE\n");

  rumboot_printf("Comparing...\n");
#ifdef TRUNC16
  // Compare arrays
  if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
    rumboot_printf("Test PASSED\n");
  else {
    rumboot_printf("Test FAILED\n");
    return 1;
  }
#else
  for(int i=0;i<res_metrics->s/2;i++) {
    if ((res_data[i] & 0x7FFF) != (etalon[i] & 0x7FFF)) {
      rumboot_printf("res_data[%d] = 0x%x, etalon[%d] = 0x%x\n",i,res_data[i],i,etalon[i]);
      rumboot_printf("Test FAILED\n");
      return 1;
    };
  }
  rumboot_printf("Test PASSED\n");
#endif

  return 0;
}
