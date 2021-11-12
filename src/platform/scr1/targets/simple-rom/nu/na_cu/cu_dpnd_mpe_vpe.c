
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

void mpe_dma_config(uintptr_t base, uint16_t* source_ptr, uint32_t buf_vecs, uint32_t base_in_buf) {
  rumboot_printf("Configuring MPE_DMA\n");
  // Read Channel
  iowrite32(0x00000001,base+BIASCtrl_MSha);
  iowrite32(0x00000000,base+RD_THRECtrl_MSha);
  iowrite32(0x00000000,base+RD_DECCtrl_MSha);
  iowrite32(0x00000000,base+PADCtrl_MSha);
  iowrite32((uint32_t)source_ptr,base+BFCA_MSha);
  iowrite32(0x00000000,base+AOffset_MSha);
  iowrite32(0x0000007F,base+CntSha_MSha);
  iowrite32(0x00000000,base+CntThresholdSha_MSha);
  iowrite32(0x00000080,base+Bias1Sha_MSha);
  iowrite32(buf_vecs-1,base+RD_Bias1CntSha_MSha);
  iowrite32(0x00000000,base+Bias1CntOffset_MSha);
  iowrite32(0x00000000,base+Bias1CntThresholdSha_MSha);
  iowrite32(0x00000000,base+RD_Bias1CntCmp_MSha);
  // Write Channel
  iowrite32(0x00000011,base+X_Cfg_MSha);
  iowrite32(0x0000000D,base+CHCfg_MSha);
  iowrite32(base_in_buf,base+BADR_MSha);
  iowrite32(base_in_buf+(buf_vecs/16+((buf_vecs%16)>0))*16,base+LADR_MSha);

  if (buf_vecs > 128) {
    iowrite32(0x0000007F,base+PLC_CntSha_MSha);
    iowrite32(0x00000001,base+WR_THRECtrl_MSha);
    iowrite32(buf_vecs-1,base+PLC_ThreSha_MSha);
    int div = buf_vecs/128;
    int mod = buf_vecs%128;
    iowrite32(div+(mod>0)-1,base+WR_Bias1CntSha_MSha);
  }
  else {
    iowrite32(buf_vecs-1,base+PLC_CntSha_MSha);
    iowrite32(0x00000000,base+WR_Bias1CntSha_MSha);
  }
  iowrite32(0x00000000,base+VLC_CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias1CntCmp_MSha);
  iowrite32(0x00000000,base+WR_Bias2CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias2CntCmp_MSha);
  iowrite32(0x00000000,base+WR_Bias3CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias3CntCmp_MSha);
  iowrite32(0x00000000,base+WR_Bias4CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias4CntCmp_MSha);
  iowrite32(0x00000000,base+WR_Bias5CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias5CntCmp_MSha);
  iowrite32(0x00000000,base+WR_Bias6CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias6CntCmp_MSha);
}

void mpe_dma_start(uintptr_t base, uint32_t dpnd) {
  rumboot_printf("Starting MPE_DMA\n");
  iowrite32(0x00000000,base+MAINCNT_Sha);
  iowrite32(0x00000001|(dpnd<<24),base+DMA_START);
}


void vpe_config(uint16_t* vpe_src_ptr, uint16_t* vpe_dst_ptr, DataTypeExt in_data_type, DataType out_data_type, uint32_t cube_size) {
  ConfigVPE* cfg;
  CubeMetrics* metrics;

  cfg = rumboot_malloc_from_heap(0,sizeof(ConfigVPE));
  metrics = rumboot_malloc_from_heap(0,sizeof(CubeMetrics));

  cfg->in_data_type                         = in_data_type                  ; // DataTypeExt_Int32 or DataTypeExt_Fp32
  cfg->out_data_type                        = out_data_type                 ; // DataType_Int8 or DataType_Int16 or DataType_Fp16
  cfg->src_flying                           = Enable_NotEn                  ;
  cfg->dst_flying                           = Enable_NotEn                  ;
  cfg->op0_en                               = Enable_NotEn                  ;
  cfg->op1_en                               = Enable_NotEn                  ;
  cfg->op2_en                               = Enable_NotEn                  ;
  cfg->c3_offset                            = 0                             ; // set it as you wish
  cfg->c3_scale                             = 1                             ; // set it as you wish
  cfg->c3_trunc                             = 0                             ; // Low 16 bits
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

  uint32_t elem_size;

  // dma_data_size definition------------------- 
  if (cfg->in_data_type == DataType_Int8) cfg->src_rdma_config.dma_data_size = DmaDSize_One_Byte;
  else                                    cfg->src_rdma_config.dma_data_size = DmaDSize_Two_Byte;

  if (cfg->out_data_type == DataType_Int8) cfg->wdma_config.dma_data_size = DmaDSize_One_Byte;
  else                                     cfg->wdma_config.dma_data_size = DmaDSize_Two_Byte;
  
  // elem_size definition-------------------
  if   (cfg->wdma_config.dma_data_size == DmaDSize_Two_Byte) elem_size = 2;
  else                                                       elem_size = 1;

  metrics->C = cube_size/elem_size;
  metrics->W = 1;
  metrics->H = 1;
  
  // CUBE_SIZE in vectors
  cfg->cube_size = (metrics->C/16 + ((metrics->C%16) != 0 ? 1 : 0)) * metrics->W * metrics->H - 1;
  
  cfg->src_rdma_config.dma_en               = Enable_En                                                    ;
  cfg->src_rdma_config.dma_ram_type         = DmaRamType_CV                                                ;
  cfg->src_rdma_config.dma_data_mode        = Mode_Unitary                                                 ;
  //cfg->src_rdma_config.dma_data_size        = DmaDSize_Two_Byte                                            ;
  cfg->src_rdma_config.dma_data_use         = DmaDUse_Mux                                                  ;
  cfg->src_rdma_config.dma_axi_len          = 3                                                            ; // or 0 or 1 or 
  cfg->src_rdma_config.dma_baddr            = (uint32_t)vpe_src_ptr                                        ; // set it accord
  cfg->src_rdma_config.dma_cube_size_c      = metrics->C                                                   ;
  cfg->src_rdma_config.dma_cube_size_w      = metrics->W                                                   ;
  cfg->src_rdma_config.dma_cube_size_h      = metrics->H                                                   ;
  cfg->src_rdma_config.dma_border_x         = (metrics->C/4096 + ((metrics->C%4096) != 0 ? 1 : 0) - 1)*(4096*elem_size);
  cfg->src_rdma_config.dma_border_y         = 0                                                            ;
  cfg->src_rdma_config.dma_border_z         = 0                                                            ;
  cfg->src_rdma_config.dma_stride_x         = 4096*elem_size                                               ;
  cfg->src_rdma_config.dma_stride_y         = 0                                                            ;
  cfg->src_rdma_config.dma_stride_z         = 0                                                            ;
  cfg->src_rdma_config.dma_frag_last_size   = (metrics->C%4096 + ((metrics->C%4096) == 0)*4096) * elem_size;
  cfg->src_rdma_config.dma_frag_size        = 4096*elem_size                                               ;
  cfg->src_rdma_config.dma_xyz_drct         = 0                                                            ;
  cfg->src_rdma_config.dma_box_st_size_x    = 0                                                            ;
  cfg->src_rdma_config.dma_box_st_size_y    = 0                                                            ;
  cfg->src_rdma_config.dma_box_st_size_z    = 0                                                            ;
  cfg->src_rdma_config.dma_box_size_x       = 0                                                            ;
  cfg->src_rdma_config.dma_box_size_y       = 0                                                            ;
  cfg->src_rdma_config.dma_box_size_z       = 0                                                            ;
  cfg->src_rdma_config.dma_box_offset_x     = 0                                                            ;
  cfg->src_rdma_config.dma_box_offset_y     = 0                                                            ;
  cfg->src_rdma_config.dma_box_offset_z     = 0                                                            ;

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

  cfg->wdma_config.dma_en                   = Enable_En                                                    ;
  cfg->wdma_config.dma_ram_type             = DmaRamType_CV                                                ;
  cfg->wdma_config.dma_data_mode            = Mode_Element                                                 ;
//cfg->wdma_config.dma_data_size            = DmaDSize_Two_Byte                                            ; // or DmaDSize_One_Byte. It depends from cfg->out_data_type value.
  cfg->wdma_config.dma_data_use             = DmaDUse_Mux                                                  ;
  cfg->wdma_config.dma_axi_len              = 3                                                            ; // or 0 or 1 or 2
  cfg->wdma_config.dma_baddr                = (uint32_t)vpe_dst_ptr                                        ; // set it according with malloc address
  cfg->wdma_config.dma_cube_size_c          = metrics->C                                                   ;
  cfg->wdma_config.dma_cube_size_w          = metrics->W                                                   ;
  cfg->wdma_config.dma_cube_size_h          = metrics->H                                                   ;
  cfg->wdma_config.dma_border_x             = (metrics->C/4096 + ((metrics->C%4096) != 0 ? 1 : 0) - 1)*(4096*elem_size);
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
}

void vpe_start(uint32_t dpnd) {
  iowrite32 ((dpnd<<24)|1, NPE_BASE + NA_VPE_BASE + NU_VPE + NU_VPE_NEXT_CNTX);
}

void vpe_start_with_context_num(uint32_t cntx_num) {
  iowrite32 ((cntx_num<<4)|1, NPE_BASE + NA_VPE_BASE + NU_VPE + NU_VPE_NEXT_CNTX);
}

//void vpe_start(ConfigVPE* cfg, uint32_t dpnd) {
//  nu_vpe_setup(NPE_BASE+NA_VPE_BASE,cfg);
//  iowrite32 ((dpnd<<24)|1, NPE_BASE + NA_VPE_BASE + NU_VPE + NU_VPE_NEXT_CNTX);
//}

void vpe_start_with_new_addr(uint32_t src_base, uint32_t dst_base, uint32_t dpnd) {
  iowrite32(dst_base,NPE_BASE + NA_VPE_BASE + NU_VPE + NU_VPE_DST_WDMA + NU_VPE_DMA_BASE);
  iowrite32(src_base,NPE_BASE + NA_VPE_BASE + NU_VPE + NU_VPE_SRC_RDMA + NU_VPE_DMA_BASE);
  iowrite32 ((dpnd<<24)|1, NPE_BASE + NA_VPE_BASE + NU_VPE + NU_VPE_NEXT_CNTX);
}

CubeMetrics* metrics_in_data;
CubeMetrics* metrics_etalon_data; // reference
uint16_t *in_data;
uint16_t *etalon_data;

// in_data:
//-------------------------------------------------
// DATA FOR MPE_W (256 VECTORS BY 128 BYTES: 0-255)
//
//
//
//
//
//
// WEIGHTS FOR MPE_MA (VECTORS 240-255)
//-------------------------------------------------
// DATA FOR MPE_D (128 VECTORS BY 128 BYTES: 0-127)
//
//
// DATA FOR MPE_MA (VECTORS 112-127)
// ------------------------------------------------
// DUMMY 128 VECTORS
//
//
//
//-------------------------------------------------

int main()
{
  int heap_id;
  heap_id = nu_get_heap_id();

  metrics_in_data = rumboot_malloc_from_heap_aligned(heap_id,sizeof(metrics_in_data),128);
  rumboot_platform_request_file("metrics_in_arr",(uintptr_t)metrics_in_data);

  in_data = rumboot_malloc_from_heap_aligned(heap_id,metrics_in_data->s,128);
  rumboot_platform_request_file("in_arr", (uintptr_t)in_data);

  //mpe_out_data = rumboot_malloc_from_heap_aligned(heap_id,metrics_pe_data->s,128);
  
  CubeMetrics* metrics_vpe_dma_cube;
  metrics_vpe_dma_cube = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
  metrics_vpe_dma_cube->s = 16*128;
  uint32_t vpe_in_data_addr = (uint32_t)in_data;
  CubeMetrics* metrics_mpe_dma_cube;
  metrics_mpe_dma_cube = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
  metrics_mpe_dma_cube->s = 256*128; // 256 vectors by 1Kbit in bytes (half of inbuff)
  uint32_t mpe_w_in_data_addr = (uint32_t)in_data;
  uint32_t mpe_d_in_data_addr = mpe_w_in_data_addr + metrics_mpe_dma_cube->s;
 
  CubeMetrics* metrics_mpe_ma_cube;
  metrics_mpe_ma_cube = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
  metrics_mpe_ma_cube->s = 16*128; // 16 vectors by 1K in bytes
  uint32_t mpe_ma_w_in_data_addr = metrics_mpe_dma_cube->s - metrics_mpe_ma_cube->s;
  uint32_t mpe_ma_d_in_data_addr = metrics_mpe_dma_cube->s + (metrics_mpe_dma_cube->s)/2 - metrics_mpe_ma_cube->s;
  uint32_t vpe_out_data_addr = (uint32_t)in_data + mpe_ma_d_in_data_addr/* it is mpe! */; // to check that data will rewrite by vpe after mpe reads

  DataTypeExt in_data_type = DataType_Int16;
  DataType out_data_type = DataType_Int16;
  vpe_config((uint16_t*)vpe_in_data_addr,(uint16_t*)vpe_out_data_addr,in_data_type,out_data_type,metrics_mpe_ma_cube->s);

  mpe_dma_config(NPE_BASE+NA_MPE_BASE+MPE_RDMA_W_BASE,(uint16_t*)mpe_w_in_data_addr,(metrics_mpe_dma_cube->s)/128 /* in vectors */, 0);
  mpe_dma_config(NPE_BASE+NA_MPE_BASE+MPE_RDMA_D_BASE,(uint16_t*)mpe_d_in_data_addr,(metrics_mpe_dma_cube->s)/128/2 /* in vectors */, metrics_mpe_dma_cube->s/128);

  mpe_dma_start(NPE_BASE+NA_MPE_BASE+MPE_RDMA_D_BASE,0);
  mpe_dma_start(NPE_BASE+NA_MPE_BASE+MPE_RDMA_W_BASE,0);
  vpe_start(1); // start vpe with dependence on mpe_d finish
  iowrite32((uint32_t)in_data+mpe_ma_w_in_data_addr,NPE_BASE+NA_VPE_BASE+NU_VPE+NU_VPE_DST_WDMA+NU_VPE_DMA_BASE);
  vpe_start(1<<1); // start vpe with dependence on mpe_w finish

  iowrite32(mpe_ma_w_in_data_addr/128,NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+MPE_COMMON_WA);
  iowrite32(mpe_ma_d_in_data_addr/128,NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+MPE_COMMON_DA);
  iowrite32(0,NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+MPE_COMMON_VRA);
  iowrite32(2,NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+MPE_COMMON_IN_FORMAT); // int16
  iowrite32(16-1,NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+MPE_COMMON_NR);
  iowrite32(10,NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+MPE_COMMON_NORM_PARAM);
  iowrite32((1<<31)|(1<<28)|(1<<25),NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+MPE_CMD_IRCW);
  iowrite32((1<<24),NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+MPE_CMD_IRC);
  iowrite32((1<<30)|(1<<29)|(1<<23)|(1<<23)|(0b11111111<<7)|(1<<5)|(1<<3)|(1<<0),NPE_BASE+NA_MPE_BASE+MPE_MA_BASE+MPE_CMD_IRC);

  rumboot_printf("vpe_in_data_addr = 0x%x\n",vpe_in_data_addr);
  rumboot_printf("mpe_w_in_data_addr = 0x%x\n",mpe_w_in_data_addr);
  rumboot_printf("mpe_d_in_data_addr = 0x%x\n",mpe_d_in_data_addr);
  rumboot_printf("vpe_out_data_addr = 0x%x\n",vpe_out_data_addr);
  rumboot_printf("mpe_ma_w_in_data_addr = 0x%x\n",mpe_ma_w_in_data_addr);
  rumboot_printf("mpe_ma_d_in_data_addr = 0x%x\n",mpe_ma_d_in_data_addr);
  rumboot_printf("metrics_mpe_dma_cube->s = 0x%x\n",metrics_mpe_dma_cube->s);
  rumboot_printf("metrics_vpe_dma_cube->s = 0x%x\n",metrics_vpe_dma_cube->s);
  
  uint32_t mpe_rdma_cadr = ioread32(NPE_BASE+NA_MPE_BASE+MPE_RDMA_D_BASE+CURRADR);
  if ((mpe_rdma_cadr/4)>((uint32_t)in_data + mpe_ma_d_in_data_addr)) {
    rumboot_printf("ERROR: MPE_D already read data before VPE starts\n");
    rumboot_printf("mpe_rdma_cadr = 0x%x, mpe_ma_d_in_data_addr = 0x%x\n",mpe_rdma_cadr,mpe_ma_d_in_data_addr);
    return -1;
  }

  mpe_rdma_cadr = ioread32(NPE_BASE+NA_MPE_BASE+MPE_RDMA_W_BASE+CURRADR);
  if ((mpe_rdma_cadr/4)>((uint32_t)in_data + mpe_ma_w_in_data_addr)) {
    rumboot_printf("ERROR: MPE_W already read data before VPE starts\n");
    return -1;
  }

  if ((ioread32(NPE_BASE+NA_MPE_BASE+MPE_RDMA_D_BASE+DMA_STS)&0x3)!=3) {
    rumboot_printf("ERROR: MPE ended works before configuring done!\n");
    return -1;
  };
  
  uint32_t C = 16*16; // in elements!
  uint16_t *mpe_out_data;
  mpe_out_data = rumboot_malloc_from_heap_aligned(heap_id,C*2,128);

  iowrite32((uint32_t)mpe_out_data,NPE_BASE+NA_VPE_BASE+NU_VPE_DST_WDMA+NU_VPE_DMA_BASE);
  iowrite32(0,NPE_BASE+NA_VPE_BASE+NU_VPE_DST_WDMA+NU_VPE_DMA_BORDER_X);
  iowrite32(0,NPE_BASE+NA_VPE_BASE+NU_VPE_DST_WDMA+NU_VPE_DMA_FRAG_SIZE_ADDR);
  iowrite32(0,NPE_BASE+NA_VPE_BASE+NU_VPE_DST_WDMA+NU_VPE_DMA_STRIDE_X);
  iowrite32(C*2,NPE_BASE+NA_VPE_BASE+NU_VPE_DST_WDMA+NU_VPE_DMA_FRAG_LAST_SIZE_ADDR);

  iowrite32(C/16-1,NPE_BASE+NA_VPE_BASE+NU_VPE+NU_VPE_CUBE_SIZE);
  iowrite32(C/16,NPE_BASE+NA_VPE_BASE+NU_VPE_DST_WDMA+NU_VPE_DMA_CUBE_SIZE_C);

  uint32_t tmp = ioread32(NPE_BASE+NA_VPE_BASE+NU_VPE_OP_MODE);
  iowrite32(tmp|1,NPE_BASE+NA_VPE_BASE+NU_VPE_OP_MODE);

  tmp = ioread32(NPE_BASE+NA_VPE_BASE+NU_VPE_SRC_RDMA+NU_VPE_DMA_CFG);
  iowrite32(tmp&(~0x1),NPE_BASE+NA_VPE_BASE+NU_VPE_SRC_RDMA+NU_VPE_DMA_CFG);

  vpe_start_with_context_num(0x3);

  // Wait VPE DMA ends
  while ((ioread32(NPE_BASE+NA_VPE_BASE+NU_VPE_NEXT_CNTX)&((3<<4)|(1<<12)))!=(3<<4)) {};
  rumboot_printf("End VPE\n");

  rumboot_printf("Comparing...\n");

  metrics_etalon_data = rumboot_malloc_from_heap_aligned(heap_id,sizeof(metrics_etalon_data),128);
  rumboot_platform_request_file("metrics_etalon_arr",(uintptr_t)metrics_etalon_data);

  etalon_data = rumboot_malloc_from_heap_aligned(heap_id,metrics_etalon_data->s,128);
  rumboot_platform_request_file("etalon_arr", (uintptr_t)etalon_data);
  
  //for (int j=0; j<(metrics_etalon_data->s)/2; j++) {
  //  rumboot_printf("etalon[%d] = 0x%x, mpe = 0x%x\n",j,etalon_data[j],mpe_out_data[j]);
  //}

  if(nu_bitwise_compare(mpe_out_data,etalon_data,(metrics_etalon_data->s)/2) == 0)
    rumboot_printf("Test PASSED\n");
  else {
    rumboot_printf("Test FAILED\n");
    return 1;
  }

  return 0;
}
