#define FORCE_VPE_WDMA_EN

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

typedef struct Property2d_s {
    uint16_t w;
    uint16_t h;
} Property2d_s;

Property2d_s* nu_vpe_significant_start_coords(int heap_id, Property2d_s* stride, Property2d_s* pad) {
  Property2d_s* start_coords;
  start_coords = rumboot_malloc_from_heap_aligned(heap_id, sizeof(Property2d_s), sizeof(uint16_t));
  start_coords->h = (pad->h > 0) ? start_coords->h = stride->h-1 : 0;
  start_coords->w = (pad->w > 0) ? start_coords->w = stride->w-1 : 0;
  return start_coords;
}

void nu_vpe_decide_dma_config (
  ConfigVPE* cfg,
  CubeMetrics* in_metrics,
  uint8_t axi_len,
  void*in_data,
  void*op0,
  void*op1,
  void*op2,
  CubeMetrics*res_metrics,
  void*res_data
) {
   rumboot_printf("nu_vpe_decide_dma_config \n");

  cfg->src_flying = cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32 ? Enable_En : Enable_NotEn;
#ifdef FORCE_VPE_WDMA_EN
  cfg->dst_flying = Enable_NotEn;
#else
  cfg->dst_flying = cfg->out_data_type == DataType_Int8 ? Enable_NotEn : Enable_En;
#endif

#ifdef VPE_TraceMode_MPE_DW
  cfg->trace_mode = TraceMode_MPE_DW;
#else
  cfg->trace_mode = TraceMode_MPE;
#endif
  
  cfg->src_rdma_config.dma_bsize=0;
  cfg->src_rdma_config.dma_bstride=0;
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

  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);

  cfg->src_rdma_config.dma_baddr = nu_virt_to_dma(in_data);
  cfg->op0_rdma_config.dma_baddr = nu_virt_to_dma(op0);
  cfg->op1_rdma_config.dma_baddr = nu_virt_to_dma(op1);
  cfg->op2_rdma_config.dma_baddr = nu_virt_to_dma(op2);
  cfg->wdma_config.dma_baddr     = nu_virt_to_dma(res_data);

  cfg->src_rdma_config.dma_axi_len = 15;
  cfg->op0_rdma_config.dma_axi_len = 15;
  cfg->op1_rdma_config.dma_axi_len = 15;
  cfg->op2_rdma_config.dma_axi_len = 15;
  cfg->wdma_config.dma_axi_len     = 15;
}

void nu_vpe_wdma_dw_transpose_trace_mode_cube_config(ConfigDMA* dma_cfg, TraceMode trace_mode, CubeMetrics* metrics, Property2d_s* stride, Property2d_s* pad) {
  uint32_t elem_size     ;
  uint32_t tmp_data_use  ;
  uint32_t tmp_data_size ;

  int32_t metrics_W_abc;
  
  dma_cfg->dma_cube_size_w = metrics->W ;
  dma_cfg->dma_cube_size_h = metrics->H ;
  dma_cfg->dma_cube_size_c = metrics->C ;

  metrics_W_abc = metrics->W + (stride->w - 1)*(metrics->W + 2*pad->w - 1);

  if   (dma_cfg->dma_data_use  == DmaDUse_Both) tmp_data_use = 2 ;
  else                                          tmp_data_use = 1 ;
    
  if   (dma_cfg->dma_data_size == DmaDSize_Two_Byte) tmp_data_size = 2 ;
  else                                               tmp_data_size = 1 ;
    
  elem_size = tmp_data_use * tmp_data_size; 

  //------------
  dma_cfg->dma_frag_size = 16 * elem_size ;
  dma_cfg->dma_xyz_drct = DmaXYZDirection_Z;
  dma_cfg->dma_frag_last_size = ((metrics->C%16) + ((metrics->C%16) == 0)*16) * elem_size ;
  dma_cfg->dma_stride_z       = 16                                            * elem_size ; //coef_z == vector_size * elem_size
  dma_cfg->dma_stride_x       = stride->w * metrics->C                        * elem_size ; //coef_x == full_line_z             = full_line_C*elem_size
  dma_cfg->dma_stride_y       = stride->h * metrics_W_abc * metrics->C        * elem_size ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
  dma_cfg->dma_border_z       = (metrics->C/16 - ((metrics->C%16) == 0)) * 16 * elem_size ; //line_size (bytes)               = (Z-1)*elem_size
  dma_cfg->dma_border_x       = (metrics_W_abc - 1) * metrics->C              * elem_size ; //plane_size - last line (bytes)  = (X-1)*full_line_z*elem_size
  dma_cfg->dma_border_y       = (metrics->H - 1) * dma_cfg->dma_stride_y; //cube_size  - last plane (bytes) = (Y-1)*full_line_z*full_line_x*elem_size
  rumboot_printf("dma_cfg->dma_stride_x %d\n", dma_cfg->dma_stride_x);
  rumboot_printf("dma_cfg->dma_border_x %d\n", dma_cfg->dma_border_x);
  rumboot_printf("dma_cfg->dma_stride_y %d\n", dma_cfg->dma_stride_y);
  rumboot_printf("dma_cfg->dma_border_y %d\n", dma_cfg->dma_border_y);
  if (trace_mode == TraceMode_MPE) { // MPE read mode
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
}

void nu_vpe_data_gen_enable(ConfigVPE* cfg_vpe) {
  int32_t     tmp_data;
  DataType    tmp_type;
  if      (cfg_vpe->in_data_type == DataTypeExt_Int32 || cfg_vpe->in_data_type == DataTypeExt_Int16) tmp_type = DataType_Int16 ;
  else if (cfg_vpe->in_data_type == DataTypeExt_Fp32  || cfg_vpe->in_data_type == DataTypeExt_Fp16)  tmp_type = DataType_Fp16  ;
  else                                                                                               tmp_type = DataType_Int8  ;
  tmp_data = ( 1 << 2)| 
             ( (cfg_vpe->ninf_cnt_en&1) << 30) | 
             ( (cfg_vpe->ovrf_cnt_en&1) << 29) | 
             ( (cfg_vpe->lut_cnt_en&1)  << 28) |
             ((cfg_vpe->wdma_config.dma_bsize) << 20) |   // why wdma&&&& maybe rdma????  -1
             (cfg_vpe->out_data_type << 16) | 
             ((cfg_vpe->op2_config.coef_type>>1) << 15) | ((cfg_vpe->op2_rdma_config.dma_data_mode&0x1)<<14) |
             ((cfg_vpe->op1_config.coef_type>>1) << 13) | ((cfg_vpe->op1_rdma_config.dma_data_mode&0x1)<<12) |
             ((cfg_vpe->op0_config.coef_type>>1) << 11) | ((cfg_vpe->op0_rdma_config.dma_data_mode&0x1)<<10) |
             (                 (tmp_type>>1) <<  9) | ( ((cfg_vpe->trace_mode==TraceMode_MPE  || cfg_vpe->trace_mode==TraceMode_MPE_DW)?1:0)<<8) |
             (cfg_vpe->nan_to_zero_input << 4) | (cfg_vpe->dst_flying << 1) | (cfg_vpe->src_flying << 0) ;
  iowrite32(tmp_data, NPE_BASE+NA_VPE_BASE+NU_VPE+NU_VPE_OP_MODE);
}


void nu_run_mpe_cmd(uintptr_t base, void* cmd, MPECmdMetrics* cmd_metrics, uintptr_t mpe_dma_r_baddr, uintptr_t mpe_dma_w_baddr) {
  uint32_t offset;
  uint32_t data;
  int num_cmds;
  uint32_t* ptr;
  uint32_t result;
  ptr = (uint32_t*) cmd;
  num_cmds = (cmd_metrics->s / 8);
  rumboot_printf("Configuring and starting MPE..\n");
  for(int i=0;i<num_cmds;i++) {
    data = *ptr;
    ptr++;
    offset = *ptr;

    ptr++;
    if(offset == MPE_RDMA_D_BASE+BFCA_MSha) {data = nu_virt_to_dma(mpe_dma_r_baddr);}
    if(offset == MPE_RDMA_W_BASE+BFCA_MSha) {data = nu_virt_to_dma(mpe_dma_w_baddr);}
    //rumboot_printf("Writing data=0x%x, offset = 0x%x\n", data, offset);
    iowrite32(data,base + offset);
  }
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

  rumboot_printf("START NKBVS TEST\n");

  heap_id = nu_get_heap_id();
  
  ConfigVPE cfg_vpe;
  char* fn_cfg_base = "cfg_file_tag"   ;
  char  fn_cfg[32];

  if(nu_vpe_load_cfg_by_tag(heap_id, &cfg_vpe, fn_cfg_base) != 0) return -1;


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
  //nu_run_mpe_cmd(NPE_BASE+NA_MPE_BASE,cmd,cmd_metrics,(uint32_t)in_data,(uint32_t)warr);
  
  // VPE
  DataTypeExt in_data_type = DataTypeExt_Int32;
  DataType out_data_type = DataType_Int16;

  CubeMetrics* vpe_in_metrics;
  uint32_t size;
  size = sizeof(CubeMetrics);

  vpe_in_metrics = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));

  vpe_in_metrics->C = vpe_in_c;
  vpe_in_metrics->W = vpe_in_w;
  vpe_in_metrics->H = vpe_in_h;
  vpe_in_metrics->s = vpe_in_metrics->C * vpe_in_metrics->W * vpe_in_metrics->H * 2;
  
//  // Insert zero on x-dim
  CubeMetrics* res_with_null_gaps_metrics;
  Property2d_s* res_data_start_coords;
  Property2d_s stride;
  Property2d_s pad;
  stride.h = stride_h;
  stride.w = stride_w;
  pad.h = pad_h;
  pad.w = pad_w;

  res_data_start_coords = nu_vpe_significant_start_coords(heap_id, &stride, &pad);

  res_with_null_gaps_metrics = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));
  res_with_null_gaps_metrics->C = vpe_in_metrics->C;
  res_with_null_gaps_metrics->W = vpe_in_metrics->W + (stride.w - 1)*(vpe_in_metrics->W + 2*pad.w - 1);
  rumboot_printf("res_with_null_gaps_metrics->W = %d\n", res_with_null_gaps_metrics->W);
  rumboot_printf("pad.w = %d\n", pad.w);
  res_with_null_gaps_metrics->H = vpe_in_metrics->H + (stride.h - 1)*(vpe_in_metrics->H + 2*pad.h - 1);
  res_with_null_gaps_metrics->s = 2 * res_with_null_gaps_metrics->C * res_with_null_gaps_metrics->W * res_with_null_gaps_metrics->H; 
  uint32_t res_data_offset = 2 * (res_data_start_coords->h * res_with_null_gaps_metrics->C * res_with_null_gaps_metrics->W + res_data_start_coords->w * res_with_null_gaps_metrics->C);

  rumboot_printf("res_with_null_gaps_metrics->C = %d\n", res_with_null_gaps_metrics->C);
  rumboot_printf("res_with_null_gaps_metrics->W = %d\n", res_with_null_gaps_metrics->W);
  rumboot_printf("res_with_null_gaps_metrics->H = %d\n", res_with_null_gaps_metrics->H);
  rumboot_printf("res_with_null_gaps_metrics->s = %d\n", res_with_null_gaps_metrics->s);
  rumboot_printf("res_data_offset = %d\n", res_data_offset);
  nu_vpe_decide_dma_config(&cfg_vpe,vpe_in_metrics,15,0,0,0,0,res_metrics,(char*)res_data+res_data_offset);
  nu_vpe_wdma_dw_transpose_trace_mode_cube_config(&(cfg_vpe.wdma_config),cfg_vpe.trace_mode,vpe_in_metrics,&stride,&pad);

  uint32_t line_offset = 2 * res_with_null_gaps_metrics->C*res_with_null_gaps_metrics->W;
//  nu_vpe_decide_dma_config(&cfg_vpe,vpe_in_metrics,15,0,0,0,0,res_metrics,(char*)res_data+line_offset+2*vpe_in_metrics->C);
//  nu_vpe_wdma_dilation_trace_mode_cube_config(&(cfg_vpe.wdma_config),cfg_vpe.trace_mode, vpe_in_metrics);
//  nu_vpe_setup(NPE_BASE+NA_VPE_BASE, &cfg_vpe);
//  nu_vpe_data_gen_enable(&cfg_vpe);
//
//  nu_vpe_run(NPE_BASE+NA_VPE_BASE, &cfg_vpe);

  // MPE -> VPE
  memset(res_data, 0, res_metrics->s);
//  cfg_vpe.depend_mask = NU_DEPEND_MASK_VPE_WDMA;
  //cfg_vpe.mark = 1;
  nu_vpe_setup(NPE_BASE+NA_VPE_BASE, &cfg_vpe);

  nu_run_mpe_cmd(NPE_BASE+NA_MPE_BASE,cmd,cmd_metrics,(uint32_t)in_data,(uint32_t)warr);
  nu_vpe_run(NPE_BASE+NA_VPE_BASE, &cfg_vpe);


//  // VPE
//  vpe_in_metrics->W = (vpe_in_metrics->W-1)+(vpe_in_metrics->W-2);
//  vpe_in_metrics->H = vpe_in_metrics->H+1;
//  vpe_in_metrics->s = vpe_in_metrics->C * vpe_in_metrics->W * vpe_in_metrics->H * 2;
//
//  nu_vpe_decide_dma_config(&cfg_vpe,vpe_in_metrics,15,0,0,0,0,res_metrics,res_data);
//  cfg_vpe.wdma_config.dma_stride_y = 2*2 * vpe_in_metrics->C * vpe_in_metrics->W                       ; //coef_y == full_line_z*full_line_x = full_line_C*full_line_W*elem_size 
//  cfg_vpe.wdma_config.dma_border_y = (vpe_in_metrics->H - 1) * cfg_vpe.wdma_config.dma_stride_y;
//  cfg_vpe.depend_mask = NU_DEPEND_MASK_VPE_WDMA;
//  cfg_vpe.mark = 1;
//  nu_vpe_setup(NPE_BASE+NA_VPE_BASE, &cfg_vpe);
//
//  nu_vpe_data_gen_enable(&cfg_vpe);
//  nu_vpe_run(NPE_BASE+NA_VPE_BASE, &cfg_vpe);

  nu_vpe_wait(NPE_BASE+NA_VPE_BASE, &cfg_vpe);
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
