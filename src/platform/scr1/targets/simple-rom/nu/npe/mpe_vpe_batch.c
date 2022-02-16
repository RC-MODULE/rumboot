
#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <devices/rcm_na/nu_lib.h>
#include <devices/rcm_na/nu_test_lib.h>
#include <regs/regs_na.h>
#include <platform/devices.h>

uint32_t nu_run_mpe_cmd(uintptr_t base, void* cmd, MPECmdMetrics* cmd_metrics, uintptr_t mpe_dma_r_baddr, uintptr_t mpe_dma_w_baddr) {
  uint32_t offset;
  uint32_t data;
  int num_cmds;
  uint32_t* ptr;
  ptr = (uint32_t*) cmd;
  num_cmds = (cmd_metrics->s/8);
  for(int i=0;i<num_cmds;i++) {
    data = *ptr;
    ptr++;
    offset = *ptr;

    ptr++;
    rumboot_printf("Writing data = 0x%X addr = 0x%X\n", data, offset);
    if(offset == MPE_RDMA_D_BASE+BFCA_MSha) {data = mpe_dma_r_baddr;}
    if(offset == MPE_RDMA_W_BASE+BFCA_MSha) {data = mpe_dma_w_baddr;}
    iowrite32(data,base+offset);
    if (offset == MPE_MA_BASE+MPE_CMD_ICMW) break;
  }
  return (uint32_t)ptr;
}

void vpe_config_and_start(uint16_t* vpe_ptr, DataTypeExt in_data_type, DataType out_data_type, uint32_t C, uint32_t W, uint32_t H, uint32_t bsize) {
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
  cfg->c3_offset                            = 0                             ;
  cfg->c3_scale                             = 1                             ;
#ifdef TRUNC16
  cfg->c3_trunc                             = 16                            ; // High 16 bits
#else
  cfg->c3_trunc                             = 0                             ; // Low 16 bits
#endif
  cfg->c3_satur_en                          = Enable_NotEn                  ;
  cfg->c3_round_mode                        = RoundMode_Down                ;
  cfg->nan_to_zero_input                    = Enable_NotEn                  ;
  cfg->nan_to_zero_output                   = Enable_NotEn                  ;
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

  metrics->C = C;
  metrics->W = W;
  metrics->H = H;
  
  // CUBE_SIZE in vectors
  cfg->cube_size = (metrics->C/16 + ((metrics->C%16) != 0 ? 1 : 0)) * metrics->W * metrics->H - 1;
  
  cfg->trace_mode                           = TraceMode_MPE                                    ;

  cfg->wdma_config.dma_en                   = Enable_En                                        ;
  cfg->wdma_config.dma_ram_type             = DmaRamType_CV                                    ;
  cfg->wdma_config.dma_data_mode            = Mode_Element                                     ;
//cfg->wdma_config.dma_data_size            = DmaDSize_Two_Byte                                ; // or DmaDSize_One_Byte. It depends from cfg->out_data_type value.
  cfg->wdma_config.dma_data_use             = DmaDUse_Mux                                      ;
  cfg->wdma_config.dma_axi_len              = 3                                                ; // or 0 or 1 or 2
  cfg->wdma_config.dma_baddr                = (uint32_t)vpe_ptr                                ;
  cfg->wdma_config.dma_cube_size_c          = metrics->C                                       ;
  cfg->wdma_config.dma_cube_size_w          = metrics->W                                       ;
  cfg->wdma_config.dma_cube_size_h          = metrics->H                                       ;
  cfg->wdma_config.dma_xyz_drct             = 2                                                ;
  
  cfg->wdma_config.dma_bsize                = bsize-1                                          ;
  cfg->wdma_config.dma_bstride              = metrics->C * metrics->W * metrics->H * elem_size ;

  cfg->wdma_config.dma_frag_last_size = ((metrics->C%16) + ((metrics->C%16) == 0)*16) * elem_size ;
  cfg->wdma_config.dma_frag_size      = 16*elem_size                                              ;
  cfg->wdma_config.dma_stride_z       = 16                      * elem_size                       ; //coef_z == vector_size * elem_size
  cfg->wdma_config.dma_stride_x       = metrics->C              * elem_size                       ; //coef_x == full_line_z             = full_line_C*elem_size
  cfg->wdma_config.dma_stride_y       = metrics->C * metrics->W * elem_size                       ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
  cfg->wdma_config.dma_border_z       = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size ; //line_size (bytes)               = (Z-1)*elem_size
  cfg->wdma_config.dma_border_x       = (metrics->W - 1) * cfg->wdma_config.dma_stride_x          ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
  cfg->wdma_config.dma_border_y       = (metrics->H - 1) * cfg->wdma_config.dma_stride_y          ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
  
  cfg->wdma_config.dma_box_st_size_x  = 128/bsize  - 1 ;
  cfg->wdma_config.dma_box_st_size_y  = 1      - 1 ;
  cfg->wdma_config.dma_box_st_size_z  = 128/16 - 1 ;
  cfg->wdma_config.dma_box_size_x     = 128/bsize  - 1 ;
  cfg->wdma_config.dma_box_size_y     = 1      - 1 ;
  cfg->wdma_config.dma_box_size_z     = 128/16 - 1 ;
  cfg->wdma_config.dma_box_offset_x   = 0 ;
  cfg->wdma_config.dma_box_offset_y   = 0 ;
  cfg->wdma_config.dma_box_offset_z   = 0 ;

  cfg->src_rdma_config.dma_bsize          = bsize-1       ;
    
  cfg->src_rdma_config.dma_cube_size_c    = metrics->C    ;
  cfg->src_rdma_config.dma_cube_size_w    = metrics->W    ;
  cfg->src_rdma_config.dma_cube_size_h    = metrics->H    ;
  cfg->src_rdma_config.dma_box_st_size_x  = 128/bsize - 1 ;
  cfg->src_rdma_config.dma_box_st_size_y  = 1      - 1    ;
  cfg->src_rdma_config.dma_box_st_size_z  = 128/16 - 1    ;
  cfg->src_rdma_config.dma_box_size_x     = 128/bsize - 1 ;
  cfg->src_rdma_config.dma_box_size_y     = 1      - 1    ;
  cfg->src_rdma_config.dma_box_size_z     = 128/16 - 1    ;
  cfg->src_rdma_config.dma_box_offset_x   = 0 ;
  cfg->src_rdma_config.dma_box_offset_y   = 0 ;
  cfg->src_rdma_config.dma_box_offset_z   = 0 ;
  
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
uint16_t* res_batch;

int main()
{
  int heap_id;

  rumboot_printf("START MPE+VPE BATCH TEST\n");

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

  res_metrics= nu_mpe_load_res_metrics(heap_id); // Res from VPE 16 bits
  if(res_metrics == NULL) return -1;

  res_data = rumboot_malloc_from_heap_aligned(heap_id,res_metrics->s, 64);
  if(res_data == NULL) return -1;

  res_batch = rumboot_malloc_from_heap_aligned(heap_id,res_metrics->s, 64);
  if(res_batch == NULL) return -1;

  uint32_t batch = dB;
  uint32_t B = dB;
  uint32_t C = dC;
  uint32_t W = dW;
  uint32_t H = dH;
  uint32_t K = dK;
  uint32_t S = dS;
  uint32_t R = dR;

#ifndef TP
  uint32_t TP = 0;
#endif
#ifndef BP
  uint32_t BP = 0;
#endif
#ifndef LP
  uint32_t LP = 0;
#endif
#ifndef RP
  uint32_t RP = 0;
#endif

  uint32_t Wout = (LP+W+RP-S)/SX+1;
  uint32_t Hout = (BP+H+TP-R)/SY+1;

  rumboot_printf("B = %d\n",B);
  rumboot_printf("C = %d\n",C);
  rumboot_printf("W = %d\n",W);
  rumboot_printf("H = %d\n",H);
  rumboot_printf("K = %d\n",K);
  rumboot_printf("S = %d\n",S);
  rumboot_printf("R = %d\n",R);
  rumboot_printf("TP = %d\n",TP);
  rumboot_printf("BP = %d\n",BP);
  rumboot_printf("LP = %d\n",LP);
  rumboot_printf("RP = %d\n",RP);
  rumboot_printf("SX = %d\n",SX);
  rumboot_printf("SY = %d\n",SY);
  rumboot_printf("Wout = %d\n",Wout);
  rumboot_printf("Hout = %d\n",Hout);

  uint32_t elem_size = 2; // 16 bit per element
  if (in_metrics->s!=B*C*W*H*elem_size) {
    rumboot_printf("ERROR: cube.bin contains %d bytes, B*C*W*H*elem_size = %d bytes\n",in_metrics->s,B*C*W*H*elem_size);
    return -1;
  }
  if (warr_metrics->s!=K*C*S*R*elem_size) {
    rumboot_printf("ERROR: warr.bin contains %d bytes, K*C*S*R*elem_size = %d bytes\n",warr_metrics->s,K*C*S*R*elem_size);
    return -1;
  }
  if (res_metrics->s!=B*K*Wout*Hout*elem_size) {
    rumboot_printf("ERROR: res_metrics.bin contains info about %d bytes, B*K*Wout*Hout*elem_size = %d bytes\n",res_metrics->s,B*K*Wout*Hout*elem_size);
    return -1;
  }
  rumboot_printf("Configuration checked\n");

// START WITH BATCH MODE
  // MPE
  uint32_t next_cmd_ptr = nu_run_mpe_cmd(NPE_BASE+NA_MPE_BASE,cmd,cmd_metrics,(uint32_t)in_data,(uint32_t)warr);

  // VPE
  DataTypeExt in_data_type = DataTypeExt_Int32;
  DataType out_data_type = DataType_Int16;
  vpe_config_and_start(res_batch,in_data_type,out_data_type,K,Wout,Hout,batch);

  // Wait VPE DMA ends
  while (((ioread32(NPE_BASE+NA_VPE_BASE+NU_VPE_NEXT_CNTX)>>12)&0x1)) {};
  rumboot_printf("End VPE\n");

// START WO BATCH MODE
  for (uint32_t i=0;i<batch;i++) {
    // MPE
    next_cmd_ptr = nu_run_mpe_cmd(NPE_BASE+NA_MPE_BASE,(void*)next_cmd_ptr,cmd_metrics,(uint32_t)in_data+C*W*H*elem_size*i,(uint32_t)warr);

    // VPE
    vpe_config_and_start((uint16_t*)((uint32_t)res_data+K*Wout*Hout*elem_size*i),in_data_type,out_data_type,K,Wout,Hout,1);

    // Wait VPE DMA ends
    while (((ioread32(NPE_BASE+NA_VPE_BASE+NU_VPE_NEXT_CNTX)>>12)&0x1)) {};
    rumboot_printf("End VPE\n");
  }

  // Compare arrays
  rumboot_printf("Comparing...\n");
  if(nu_bitwise_compare(res_data,res_batch,K*S*R*2*batch) == 0)
    rumboot_printf("Test PASSED\n");
  else {
    rumboot_printf("Test FAILED\n");
    return 1;
  }

  return 0;
}
