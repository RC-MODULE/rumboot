
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

ConfigVPE* cfg_vpe;
CubeMetrics* metrics_vpe;

CubeMetrics* in_metrics;
void *in_data;

WarrMetrics* warr_metrics;
void *warr;

CubeMetrics* op0_metrics;
void *op0_data;

MPECmdMetrics* cmd_metrics;
void *cmd;

CubeMetrics* res_metrics;
uint16_t* res_data;
uint16_t* etalon;


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

void vpe_config_reset(ConfigVPE* cfg, DataTypeExt in_data_type, DataType coef_data_type, DataType out_data_type) {
  DmaDSizeType coef_data_size = (coef_data_type == DataType_Int8) ? DmaDSize_One_Byte : DmaDSize_Two_Byte ;
  DmaDSizeType out_data_size  = (out_data_type  == DataType_Int8) ? DmaDSize_One_Byte : DmaDSize_Two_Byte ;
  
  cfg->in_data_type                         = in_data_type                  ; // DataTypeExt_Int32 or DataTypeExt_Fp32
  cfg->out_data_type                        = out_data_type                 ; // DataType_Int8 or DataType_Int16 or DataType_Fp16
  cfg->src_flying                           = Enable_En                     ;
  cfg->dst_flying                           = Enable_NotEn                  ;
  cfg->op0_en                               = Enable_En                     ;
  cfg->op1_en                               = Enable_NotEn                  ;
  cfg->op2_en                               = Enable_NotEn                  ;
  cfg->c3_offset                            = 0                             ; // set it as you wish
  cfg->c3_scale                             = 1                             ; // set it as you wish
  cfg->c3_trunc                             = 0                             ; // Low 16 bit
  cfg->c3_satur_en                          = Enable_NotEn                  ; // set it as you wish
  cfg->c3_round_mode                        = RoundMode_Down                ; // set it as you wish
  cfg->nan_to_zero_input                    = Enable_NotEn                  ; // set it as you wish
  cfg->nan_to_zero_output                   = Enable_NotEn                  ; // set it as you wish
  
  cfg->op0_config.coef_type                 = coef_data_type                ;
  cfg->op0_config.alu_en                    = Enable_En                     ;
  cfg->op0_config.mux_en                    = Enable_En                     ;
  cfg->op0_config.relu_en                   = Enable_NotEn                  ;
  cfg->op0_config.prelu_en                  = Enable_NotEn                  ;
  cfg->op0_config.alu_operation             = AluOperationSwitch_Sum        ;
  cfg->op0_config.lshift_value              = 0                             ;
  cfg->op0_config.alu_mode                  = Mode_Channel                  ;
  cfg->op0_config.alu_value                 = 0                             ;
  cfg->op0_config.mux_mode                  = Mode_Channel                  ;
  cfg->op0_config.mux_value                 = 0                             ;
  cfg->op0_config.norm_round_mode           = RoundMode_HalfAwayFromZero    ;
  cfg->op0_config.norm_saturation_en        = Enable_En                     ;
  cfg->op0_config.norm_round_size           = 0                             ;
  
  cfg->op1_config.coef_type                 = coef_data_type                ;
  cfg->op1_config.alu_en                    = Enable_NotEn                  ;
  cfg->op1_config.mux_en                    = Enable_NotEn                  ;
  cfg->op1_config.relu_en                   = Enable_NotEn                  ;
  cfg->op1_config.prelu_en                  = Enable_NotEn                  ;
  cfg->op1_config.alu_operation             = AluOperationSwitch_Sum        ;
  cfg->op1_config.lshift_value              = 0                             ;
  cfg->op1_config.alu_mode                  = Mode_Element                  ;
  cfg->op1_config.alu_value                 = 0                             ;
  cfg->op1_config.mux_mode                  = Mode_Unitary                  ;
  cfg->op1_config.mux_value                 = 0                             ;
  cfg->op1_config.norm_round_mode           = RoundMode_HalfAwayFromZero    ;
  cfg->op1_config.norm_saturation_en        = Enable_En                     ;
  cfg->op1_config.norm_round_size           = 0                             ;
  
  cfg->op2_config.coef_type                 = coef_data_type                ;
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

  cfg->op0_rdma_config.dma_en               = Enable_En                     ;
  cfg->op0_rdma_config.dma_ram_type         = DmaRamType_CV                 ;
  cfg->op0_rdma_config.dma_data_mode        = Mode_Channel                  ;
  cfg->op0_rdma_config.dma_data_size        = coef_data_size                ;
  cfg->op0_rdma_config.dma_data_use         = DmaDUse_Both                  ;
  cfg->op0_rdma_config.dma_axi_len          = 15                            ;
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
  cfg->op0_rdma_config.dma_xyz_drct         = DmaXYZDirection_X             ;
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
  cfg->op1_rdma_config.dma_data_mode        = Mode_Element                  ;
  cfg->op1_rdma_config.dma_data_size        = coef_data_size                ;
  cfg->op1_rdma_config.dma_data_use         = DmaDUse_Alu                   ;
  cfg->op1_rdma_config.dma_axi_len          = 15                            ;
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
  cfg->op1_rdma_config.dma_xyz_drct         = DmaXYZDirection_Z             ;
  cfg->op1_rdma_config.dma_box_st_size_x    = 128    - 1                    ;
  cfg->op1_rdma_config.dma_box_st_size_y    = 1      - 1                    ;
  cfg->op1_rdma_config.dma_box_st_size_z    = 128/16 - 1                    ;
  cfg->op1_rdma_config.dma_box_size_x       = 128    - 1                    ;
  cfg->op1_rdma_config.dma_box_size_y       = 1      - 1                    ;
  cfg->op1_rdma_config.dma_box_size_z       = 128/16 - 1                    ;
  cfg->op1_rdma_config.dma_box_offset_x     = 0                             ;
  cfg->op1_rdma_config.dma_box_offset_y     = 0                             ;
  cfg->op1_rdma_config.dma_box_offset_z     = 0                             ;

  cfg->op2_rdma_config.dma_en               = Enable_NotEn                  ;
  cfg->op2_rdma_config.dma_ram_type         = DmaRamType_CV                 ;
  cfg->op2_rdma_config.dma_data_mode        = Mode_Unitary                  ;
  cfg->op2_rdma_config.dma_data_size        = coef_data_size                ;
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
  
  cfg->wdma_config.dma_en                   = Enable_NotEn                  ;
  cfg->wdma_config.dma_ram_type             = DmaRamType_CV                 ;
  cfg->wdma_config.dma_data_mode            = Mode_Element                  ;
  cfg->wdma_config.dma_data_size            = out_data_size                 ;
  cfg->wdma_config.dma_data_use             = DmaDUse_Mux                   ;
  cfg->wdma_config.dma_axi_len              = 15                            ;
  cfg->wdma_config.dma_baddr                = 0                             ;
  cfg->wdma_config.dma_cube_size_c          = 0                             ;
  cfg->wdma_config.dma_cube_size_w          = 0                             ;
  cfg->wdma_config.dma_cube_size_h          = 0                             ;
  cfg->wdma_config.dma_border_x             = 0                             ;
  cfg->wdma_config.dma_border_y             = 0                             ;
  cfg->wdma_config.dma_border_z             = 0                             ;
  cfg->wdma_config.dma_stride_x             = 0                             ;
  cfg->wdma_config.dma_stride_y             = 0                             ;
  cfg->wdma_config.dma_stride_z             = 0                             ;
  cfg->wdma_config.dma_frag_last_size       = 0                             ;
  cfg->wdma_config.dma_frag_size            = 0                             ;
  cfg->wdma_config.dma_xyz_drct             = DmaXYZDirection_Z             ;
  cfg->wdma_config.dma_box_st_size_x        = 128    - 1                    ;
  cfg->wdma_config.dma_box_st_size_y        = 1      - 1                    ;
  cfg->wdma_config.dma_box_st_size_z        = 128/16 - 1                    ;
  cfg->wdma_config.dma_box_size_x           = 128    - 1                    ;
  cfg->wdma_config.dma_box_size_y           = 1      - 1                    ;
  cfg->wdma_config.dma_box_size_z           = 128/16 - 1                    ;
  cfg->wdma_config.dma_box_offset_x         = 0                             ;
  cfg->wdma_config.dma_box_offset_y         = 0                             ;
  cfg->wdma_config.dma_box_offset_z         = 0                             ;
  
  nu_vpe_setup(NPE_BASE+NA_VPE_BASE,cfg);
}


void vpe_reconfig_and_start(ConfigVPE* cfg, CubeMetrics* metrics, VPE_Mode vpe_mode, bool ppe_on, uint16_t* bn_ptr, uint16_t* add_ptr, uint16_t* vpe_ptr ) {
  uint32_t elem_size;
  int32_t  tmp_data;
  uint32_t base = NPE_BASE+NA_VPE_BASE;
  
  //-----------------------------------------------------------------------------------------------------
  cfg->dst_flying = (ppe_on) ? Enable_En : Enable_NotEn ;  // PPE ON or OFF

  // OP1 config
  if (vpe_mode == VPE_MODE_BN) {
    cfg->op1_en             = Enable_NotEn  ;
    cfg->op1_config.alu_en  = Enable_NotEn  ;
    cfg->op1_config.relu_en = Enable_NotEn  ;
  }
  else if (vpe_mode == VPE_MODE_BN_RELU) {
    cfg->op1_en              = Enable_En    ;
    cfg->op1_config.alu_en   = Enable_NotEn ;
    cfg->op1_config.relu_en  = Enable_En    ;
  }
  else { //VPE_MODE_BN_ADD_RELU
    cfg->op1_en             = Enable_En     ;
    cfg->op1_config.alu_en  = Enable_En     ;
    cfg->op1_config.relu_en = Enable_En     ;
  }

  // CUBE_SIZE in vectors
  cfg->cube_size = (metrics->C/16 + ((metrics->C%16) != 0 ? 1 : 0)) * metrics->W * metrics->H - 1;

  // OP0_RDMA CONFIG
   if   (cfg->op0_rdma_config.dma_data_size == DmaDSize_Two_Byte) elem_size = 2;
   else                                                           elem_size = 1;
   cfg->op0_rdma_config.dma_baddr          = (uint32_t)bn_ptr           ;
   cfg->op0_rdma_config.dma_frag_last_size = metrics->C * 2 * elem_size ;
   
   // OP1_RDMA_CONFIG
  cfg->op1_rdma_config.dma_en = cfg->op1_config.alu_en ;
  if (cfg->op1_rdma_config.dma_en == Enable_En) {
    if   (cfg->op1_rdma_config.dma_data_size == DmaDSize_Two_Byte) elem_size = 2;
    else                                                           elem_size = 1;
    cfg->op1_rdma_config.dma_baddr          = (uint32_t)add_ptr                                         ; // set it according with malloc address
    cfg->op1_rdma_config.dma_frag_last_size = ((metrics->C%16) + ((metrics->C%16) == 0)*16) * elem_size ;
    cfg->op1_rdma_config.dma_stride_z       = 16                      * elem_size                       ; //coef_z == vector_size * elem_size
    cfg->op1_rdma_config.dma_stride_x       = metrics->C              * elem_size                       ; //coef_x == full_line_z             = full_line_C*elem_size
    cfg->op1_rdma_config.dma_stride_y       = metrics->C * metrics->W * elem_size                       ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
    cfg->op1_rdma_config.dma_border_z       = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size ; //line_size (bytes)               = (Z-1)*elem_size
    cfg->op1_rdma_config.dma_border_x       = (metrics->W - 1) * cfg->op1_rdma_config.dma_stride_x      ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
    cfg->op1_rdma_config.dma_border_y       = (metrics->H - 1) * cfg->op1_rdma_config.dma_stride_y      ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size

    iowrite32(cfg->op1_rdma_config.dma_baddr           , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BASE                ) ;
    iowrite32(cfg->op1_rdma_config.dma_border_x        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BORDER_X            ) ;
    iowrite32(cfg->op1_rdma_config.dma_border_y        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BORDER_Y            ) ;
    iowrite32(cfg->op1_rdma_config.dma_border_z        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_BORDER_Z            ) ;
    iowrite32(cfg->op1_rdma_config.dma_stride_x        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_STRIDE_X            ) ;
    iowrite32(cfg->op1_rdma_config.dma_stride_y        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_STRIDE_Y            ) ;
    iowrite32(cfg->op1_rdma_config.dma_stride_z        , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_STRIDE_Z            ) ;
    iowrite32(cfg->op1_rdma_config.dma_frag_last_size  , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR ) ;
    iowrite32(cfg->op1_rdma_config.dma_frag_size       , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_FRAG_SIZE_ADDR      ) ;
  }
  
  // WDMA_CONFIG
  cfg->wdma_config.dma_en = (ppe_on) ? Enable_NotEn : Enable_En ;
  
  if (cfg->wdma_config.dma_en == Enable_En) {
    if   (cfg->wdma_config.dma_data_size == DmaDSize_Two_Byte) elem_size = 2;
    else                                                       elem_size = 1;
    cfg->wdma_config.dma_baddr          = (uint32_t)vpe_ptr                                         ; // set it according with malloc address
    cfg->wdma_config.dma_frag_size      = 16                      * elem_size                       ; //coef_z == vector_size * elem_size
    cfg->wdma_config.dma_frag_last_size = ((metrics->C%16) + ((metrics->C%16) == 0)*16) * elem_size ;
    cfg->wdma_config.dma_stride_z       = 16                      * elem_size                       ; //coef_z == vector_size * elem_size
    cfg->wdma_config.dma_stride_x       = metrics->C              * elem_size                       ; //coef_x == full_line_z             = full_line_C*elem_size
    cfg->wdma_config.dma_stride_y       = metrics->C * metrics->W * elem_size                       ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
    cfg->wdma_config.dma_border_z       = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size ; //line_size (bytes)               = (Z-1)*elem_size
    cfg->wdma_config.dma_border_x       = (metrics->W - 1) * cfg->wdma_config.dma_stride_x          ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
    cfg->wdma_config.dma_border_y       = (metrics->H - 1) * cfg->wdma_config.dma_stride_y          ; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size

    iowrite32(cfg->wdma_config.dma_baddr           , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BASE                ) ;
    iowrite32(cfg->wdma_config.dma_border_x        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_X            ) ;
    iowrite32(cfg->wdma_config.dma_border_y        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_Y            ) ;
    iowrite32(cfg->wdma_config.dma_border_z        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_Z            ) ;
    iowrite32(cfg->wdma_config.dma_stride_x        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_X            ) ;
    iowrite32(cfg->wdma_config.dma_stride_y        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_Y            ) ;
    iowrite32(cfg->wdma_config.dma_stride_z        , base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_Z            ) ;
    iowrite32(cfg->wdma_config.dma_frag_last_size  , base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR ) ;
    iowrite32(cfg->wdma_config.dma_frag_size       , base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_SIZE_ADDR      ) ;
  
  }

  //VPE regs config
  tmp_data = (!cfg->op1_config.relu_en << 6) | (cfg->op1_config.prelu_en << 5) | (!cfg->op1_config.mux_en << 4) | (cfg->op1_config.alu_operation << 2) | (!cfg->op1_config.alu_en << 1) | (!cfg->op1_en << 0);
  iowrite32(tmp_data, base + NU_VPE + NU_VPE_OP1_CFG);
  
  iowrite32(cfg->op0_rdma_config.dma_baddr         , base + NU_VPE_OP0_RDMA + NU_VPE_DMA_BASE               ) ;
  iowrite32(cfg->op0_rdma_config.dma_frag_last_size, base + NU_VPE_OP0_RDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR) ;

  tmp_data = (cfg->op1_rdma_config.dma_ram_type << 8) | ((cfg->op1_rdma_config.dma_data_mode >> 1) << 7) | (cfg->op1_rdma_config.dma_data_size << 6) | (cfg->op1_rdma_config.dma_data_use << 4) | (cfg->op1_rdma_config.dma_en << 0);
  iowrite32(tmp_data                                 , base + NU_VPE_OP1_RDMA + NU_VPE_DMA_CFG) ;

  tmp_data = (cfg->wdma_config.dma_ram_type << 8) | (cfg->wdma_config.dma_data_size << 6) | (cfg->wdma_config.dma_en << 0);
  iowrite32(tmp_data, base + NU_VPE_DST_WDMA + NU_VPE_DMA_CFG);

//nu_vpe_setup(NPE_BASE+NA_VPE_BASE,cfg);
  nu_vpe_run(NPE_BASE+NA_VPE_BASE,cfg);
}



//========================================================================================
// MAIN FUNCTION
//========================================================================================

int main()
{
  int heap_id;

  rumboot_printf("START RESNET TEST\n");

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

  op0_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(op0_metrics),128);
  rumboot_platform_request_file("metrics_op0_cube_tag",(uintptr_t)op0_metrics);

  op0_data = rumboot_malloc_from_heap_aligned(heap_id,op0_metrics->s,128);
  rumboot_platform_request_file("op0_cube_file_tag", (uintptr_t)op0_data);

  res_metrics= nu_mpe_load_res_metrics(heap_id);
  if(res_metrics == NULL) return -1;

  res_data = nu_mpe_malloc_res(heap_id,res_metrics);
  if(res_data == NULL) return -1;

  etalon = nu_mpe_load_etalon(heap_id,res_metrics);
  if(etalon == NULL) return -1;

  // VPE INITIAL CONFIG
  DataTypeExt in_data_type   = DataTypeExt_Fp32;
  DataType    coef_data_type = DataType_Fp16;
  DataType    out_data_type  = DataType_Fp16;
  
  cfg_vpe = rumboot_malloc_from_heap(0,sizeof(ConfigVPE));
  metrics_vpe = rumboot_malloc_from_heap(0,sizeof(CubeMetrics));
  vpe_config_reset(cfg_vpe, in_data_type, coef_data_type, out_data_type);

  // MPE 1st
  rumboot_printf("Configuring MPE..\n");
  nu_run_mpe_cmd(NPE_BASE+NA_MPE_BASE,cmd,cmd_metrics,(uint32_t)in_data,(uint32_t)warr);
  
  // VPE 1st
  metrics_vpe->C = 64;
  metrics_vpe->W = 112;
  metrics_vpe->H = 112;
  vpe_reconfig_and_start(cfg_vpe, metrics_vpe, VPE_MODE_BN_RELU, 1, (uint16_t*)op0_data, 0, (uint16_t*)res_data);
  
  // PPE 1st

  // Wait VPE DMA ends
  while (((ioread32(NPE_BASE+NA_VPE_BASE+NU_VPE_NEXT_CNTX)>>12)&0x1)) {};
  rumboot_printf("End VPE\n");

  rumboot_printf("Comparing...\n");
  
  // Compare arrays
  if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
    rumboot_printf("Test PASSED\n");
  else {
    rumboot_printf("Test FAILED\n");
    return 1;
  }

  return 0;
}
