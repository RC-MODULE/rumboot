#include <stdint.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>
#include <stdio.h>

#include <devices/rcm_na/nu_lib.h> 

int nu_get_heap_id() {
  return 1;  // CHECK // fix this by define when compile under other platform than scr1
}

int nu_vpe_load_cfg(int heap_id, ConfigVPE* cfg) {
  uint32_t *cfg_bin;
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,NU_VPE_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file("cfg_file_tag", (uintptr_t)cfg_bin);
  
  nu_vpe_load_config(cfg, cfg_bin);  // Move The VPE Settings From Binary To Struct
  rumboot_free((void*) cfg_bin);
  return 0;
}

int nu_vpe_load_cfg_by_tag(int heap_id, ConfigVPE* cfg, char* cfg_file_tag) {
  uint32_t *cfg_bin;
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,NU_VPE_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file(cfg_file_tag, (uintptr_t)cfg_bin);
  
  nu_vpe_load_config(cfg, cfg_bin);  // Move The VPE Settings From Binary To Struct
  rumboot_free((void*) cfg_bin);
  return 0;
}

int nu_mpe_load_cfg(int heap_id, ConfigMPE* cfg) {
  uint32_t* cfg_bin;
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,NU_MPE_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file("cfg_mpe_file_tag",(uintptr_t)cfg_bin);
  
  nu_mpe_load_config(cfg,cfg_bin);
  rumboot_free((void*) cfg_bin);
  return 0;
}

int nu_mpe_load_cfg_by_tag(int heap_id, ConfigMPE* cfg, char* cfg_file_tag) {
  uint32_t *cfg_bin;
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,NU_MPE_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file(cfg_file_tag, (uintptr_t)cfg_bin);
  
  nu_mpe_load_config(cfg, cfg_bin);  // Move The MPE Settings From Binary To Struct
  rumboot_free((void*) cfg_bin);
  return 0;
}

void* nu_mpe_load_cfg_lut(int heap_id) { // :-( Returns A Pointer - Bahaves Not Like Other *_load_cfg*
  void* lut;
  
  lut = rumboot_malloc_from_heap_aligned(heap_id,NU_MPE_DMA_PARAM_TABLE_SIZE,sizeof(uint32_t));
  if(lut ==NULL)
    return NULL;
  
  memset(lut,0,NU_MPE_DMA_PARAM_TABLE_SIZE); // Init - Because The Table Seeker Searches For Zero As The End Of A Table
  rumboot_platform_request_file("mpe_cfg_lut_file_tag",(uintptr_t)lut);
  
  return lut;
}

int nu_ppe_load_cfg(int heap_id, ConfigPPE* cfg) {
  uint32_t* cfg_bin;
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,NU_PPE_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));
  if(cfg_bin == NULL)
    return 1;
  rumboot_platform_request_file("cfg_ppe_file_tag",(uintptr_t)cfg_bin);
  
  nu_ppe_load_config(cfg,cfg_bin);
  rumboot_free((void*) cfg_bin);
  return 0;
}

int nu_ppe_load_cfg_by_tag(int heap_id, ConfigPPE* cfg, char* cfg_file_tag) {
  uint32_t *cfg_bin = NULL;
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,NU_PPE_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));

  if(cfg_bin==NULL) return 1;

  rumboot_platform_request_file(cfg_file_tag, (uintptr_t)cfg_bin);
  nu_ppe_load_config(cfg, cfg_bin);

  rumboot_free((void*) cfg_bin);
  return 0;
}

int nu_ppe_load_cfg_reg_by_tag(int heap_id, ConfigREGPPE* cfg_reg, char* cfg_reg_file_tag) {
  int res = 0;

  uint32_t* cfg_reg_bin = NULL;
  
  cfg_reg_bin = rumboot_malloc_from_heap_aligned(heap_id,NU_PPE_REG_CFG_PARAMS_NUM*sizeof(uint32_t),sizeof(uint32_t));

  if (cfg_reg_bin==NULL || cfg_reg==NULL || cfg_reg_file_tag==NULL) res = 1;

  if (!res) {
    rumboot_platform_request_file(cfg_reg_file_tag, (uintptr_t)cfg_reg_bin);

    res = nu_ppe_reg_load_config(cfg_reg, cfg_reg_bin);
  }

  if (!res) rumboot_free((void*) cfg_reg_bin);

  return res;
}

CubeMetrics* nu_load_cube_metrics(int heap_id, char* file_tag) {
  CubeMetrics* m;
  m = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
  if(m == NULL)
    return NULL;
  
  rumboot_platform_request_file(file_tag,(uintptr_t)m);
  return m;
}

WarrMetrics* nu_load_warr_metrics(int heap_id, char* file_tag) {
  WarrMetrics* m;
  m = rumboot_malloc_from_heap_aligned(heap_id,sizeof(WarrMetrics),sizeof(int32_t));
  if(m == NULL)
    return NULL;
  
  rumboot_platform_request_file(file_tag,(uintptr_t)m);
  return m;
}

VectorMetrics* nu_load_vec_metrics(int heap_id, char* file_tag) {
  VectorMetrics* m;
  m = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
  if(m == NULL)
    return NULL;
  
  rumboot_platform_request_file(file_tag,(uintptr_t)m);
  return m;
}

MPECmdMetrics* nu_mpe_load_cmd_metrics(int heap_id) {
  MPECmdMetrics* m;
  m = rumboot_malloc_from_heap_aligned(heap_id,sizeof(MPECmdMetrics),sizeof(int32_t));
  if(m==NULL)
    return NULL;
  
  rumboot_platform_request_file("metrics_mpe_cmd_tag",(uintptr_t)m);
  return m;
}

MPECmdMetrics* nu_mpe_load_cmd_metrics_by_tag(int heap_id, char* file_tag) {
  MPECmdMetrics* m;
  m = rumboot_malloc_from_heap_aligned(heap_id,sizeof(MPECmdMetrics),sizeof(int32_t));
  if(m==NULL)
    return NULL;

  rumboot_platform_request_file(file_tag,(uintptr_t)m);
  return m;
}

CubeMetrics* nu_vpe_load_in_metrics(int heap_id) {
  return nu_load_cube_metrics(heap_id,"metrics_in_tag");
}

CubeMetrics* nu_mpe_load_in_metrics(int heap_id) {
  return nu_load_cube_metrics(heap_id,"metrics_in_tag"); // Yeh! Identical
}

CubeMetrics* nu_ppe_load_in_metrics(int heap_id) {
  return nu_load_cube_metrics(heap_id,"metrics_in_tag"); // One More
}

CubeMetrics* nu_vpe_load_res_metrics(int heap_id) {
  return nu_load_cube_metrics(heap_id,"metrics_etalon_tag");
}

CubeMetrics* nu_mpe_load_res_metrics(int heap_id) {
  return nu_load_cube_metrics(heap_id,"metrics_etalon_tag"); // Identical Once More
}

CubeMetrics* nu_ppe_load_res_metrics(int heap_id) {
  return nu_load_cube_metrics(heap_id,"metrics_etalon_tag"); //
}

WarrMetrics* nu_mpe_load_warr_metrics(int heap_id) {
  return nu_load_warr_metrics(heap_id,"metrics_warr_tag");
}

void* nu_load_cube(int heap_id,char* file_tag,CubeMetrics* metrics) {
  void* c;
  c = rumboot_malloc_from_heap_aligned(heap_id,metrics->s /*size in bytes*/,64/*aligned by 64 bytes*/);
  if(c==NULL) 
    return NULL;
  
  rumboot_platform_request_file(file_tag, (uintptr_t)c);
  return c;
}

void* nu_load_cube_misaligned(int heap_id,char* file_tag,CubeMetrics* metrics, int misalign) {
  void* c;
  c = rumboot_malloc_from_heap_misaligned(heap_id,metrics->s,64,misalign);
  if(c==NULL) 
    return NULL;
  
  rumboot_platform_request_file(file_tag, (uintptr_t)c);
  return c;
}

void* nu_load_warr(int heap_id,char* file_tag,WarrMetrics* metrics) {
  void* w;
  w = rumboot_malloc_from_heap_aligned(heap_id,metrics->s,64); // CHECK
  if(w==NULL)
    return NULL;
  
  rumboot_platform_request_file(file_tag,(uintptr_t)w);
  return w;
}

void* nu_load_vec(int heap_id,char* file_tag,VectorMetrics* metrics) {
  void* v;
  v = rumboot_malloc_from_heap_aligned(heap_id,metrics->s,64); // CHECK
  if(v==NULL)
    return NULL;
  
  rumboot_platform_request_file(file_tag,(uintptr_t)v);
  return v;
}

void* nu_load_vec_misaligned(int heap_id,char* file_tag,VectorMetrics* metrics, int misalign) {
  void* v;
  v = rumboot_malloc_from_heap_misaligned(heap_id,metrics->s,64,misalign); // CHECK
  if(v==NULL)
    return NULL;
  
  rumboot_platform_request_file(file_tag,(uintptr_t)v);
  return v;
}

void* nu_mpe_load_cmd(int heap_id,MPECmdMetrics* metrics) {
  void* c;
  c = rumboot_malloc_from_heap_aligned(heap_id,metrics->s,8);
  if(c==NULL)
    return c;
  
  rumboot_platform_request_file("mpe_cmd_file_tag",(uintptr_t)c);
  return c;
}

void* nu_mpe_load_cmd_by_tag(int heap_id, char* file_tag, MPECmdMetrics* metrics) {
  void* c;
  c = rumboot_malloc_from_heap_aligned(heap_id,metrics->s,8);
  if(c==NULL)
    return c;

  rumboot_platform_request_file(file_tag,(uintptr_t)c);
  return c;
}


void* nu_vpe_load_in_data(int heap_id,CubeMetrics* metrics) {
  return nu_load_cube(heap_id,"in_file_tag",metrics);
}

void* nu_mpe_load_in_data(int heap_id,CubeMetrics* metrics) {
  return nu_load_cube(heap_id,"in_file_tag",metrics); //
}

void* nu_ppe_load_in_data(int heap_id,CubeMetrics* metrics) {
  return nu_load_cube(heap_id,"in_file_tag",metrics); // ))
}

void* nu_mpe_load_warr(int heap_id,WarrMetrics* metrics) {
  return nu_load_warr(heap_id,"warr_file_tag",metrics);
}

void* nu_vpe_malloc_res(int heap_id,CubeMetrics* metrics) {
  void* res;
  res = rumboot_malloc_from_heap_aligned(heap_id,metrics->s , 64); // CHECK
  if(res == NULL)
    return NULL;
  
  memset(res,0xA5,metrics->s);
  return res;
}

void* nu_ppe_malloc_res(int heap_id,CubeMetrics* metrics) {
  void* res;
  res = rumboot_malloc_from_heap_aligned(heap_id,metrics->s,64); // CHECK
  if(res==NULL)
    return NULL;
  
  memset(res,0xA5,metrics->s);
  return res;
}

void* nu_mpe_malloc_res(int heap_id,CubeMetrics* metrics) {
  void* res;
  res = rumboot_malloc_from_heap_aligned(heap_id,metrics->s, 64); // CHECK
  if(res == NULL)
    return NULL;
  
  memset(res,0xA5,metrics->s);
  return res;
}

void* nu_vpe_load_etalon(int heap_id,CubeMetrics* metrics) {
  return nu_load_cube(heap_id,"etalon_file_tag",metrics);
}

void* nu_mpe_load_etalon(int heap_id,CubeMetrics* metrics) {
  return nu_load_cube(heap_id,"etalon_file_tag",metrics);   // Identical
}

void* nu_ppe_load_etalon(int heap_id,CubeMetrics* metrics) {
  return nu_load_cube(heap_id,"etalon_file_tag",metrics); //
}

void* nu_vpe_load_op01(int heap_id, ConfigOp01* cfg, int index) {
  void* op;
  CubeMetrics* cube_metrics;
  VectorMetrics* vec_metrics;
  
    // Try If OP0 Is A Cube
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Element) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Element) ) {
    cube_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
    rumboot_platform_request_file(index?"metrics_op1_cube_tag":"metrics_op0_cube_tag",(uintptr_t)cube_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,cube_metrics->s,64);
    rumboot_platform_request_file(index?"op1_cube_file_tag":"op0_cube_file_tag",(uintptr_t)op);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op0 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file(index?"metrics_op1_vec_tag":"metrics_op0_vec_tag",(uintptr_t)vec_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file(index?"op1_vec_file_tag":"op0_vec_file_tag",(uintptr_t)op);
    rumboot_free((void*) vec_metrics);
    return op;
  } 
  
  return NULL;
}
void* nu_vpe_load_op01_by_tags(int heap_id, ConfigOp01* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag) {
  void* op;
  CubeMetrics* cube_metrics;
  VectorMetrics* vec_metrics;
  
    // Try If OP0 Is A Cube
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Element) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Element) ) {
    cube_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
    rumboot_platform_request_file(metrics_cube_tag,(uintptr_t)cube_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,cube_metrics->s,64);
    rumboot_platform_request_file(cube_file_tag,(uintptr_t)op);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op0 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file(metrics_vec_tag,(uintptr_t)vec_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file(vec_file_tag,(uintptr_t)op);
    rumboot_free((void*) vec_metrics);
    return op;
  } 
  
  return NULL;
}
void* nu_vpe_load_op01_misaligned_by_tags(int heap_id, ConfigOp01* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag, int misalign) {
  void* op;
  CubeMetrics* cube_metrics;
  VectorMetrics* vec_metrics;
  
    // Try If OP0 Is A Cube
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Element) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Element) ) {
    cube_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
    rumboot_platform_request_file(metrics_cube_tag,(uintptr_t)cube_metrics);
    op = rumboot_malloc_from_heap_misaligned(heap_id,cube_metrics->s,64,misalign);
    rumboot_platform_request_file(cube_file_tag,(uintptr_t)op);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op0 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file(metrics_vec_tag,(uintptr_t)vec_metrics);
    op = rumboot_malloc_from_heap_misaligned(heap_id,vec_metrics->s,64,misalign);
    rumboot_platform_request_file(vec_file_tag,(uintptr_t)op);
    rumboot_free((void*) vec_metrics);
    return op;
  } 
  
  return NULL;
}
void* nu_vpe_load_op2(int heap_id, ConfigOp2* cfg) {
  void* op;
  CubeMetrics* cube_metrics;
  VectorMetrics* vec_metrics;
  
    // Try If OP2 Is A Cube
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Element) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Element) ) {
    cube_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
    rumboot_platform_request_file("metrics_op2_cube_tag",(uintptr_t)cube_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,cube_metrics->s,64);
    rumboot_platform_request_file("op2_cube_file_tag",(uintptr_t)op);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op2 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file("metrics_op2_vec_tag",(uintptr_t)vec_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file("op2_vec_file_tag",(uintptr_t)op);
    rumboot_free((void*) vec_metrics);
    return op;
  } 
  
  return NULL;
}

void* nu_vpe_load_op2_by_tags(int heap_id, ConfigOp2* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag) {
  void* op;
  CubeMetrics* cube_metrics;
  VectorMetrics* vec_metrics;
  
    // Try If OP2 Is A Cube
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Element) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Element) ) {
    cube_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
    rumboot_platform_request_file(metrics_cube_tag,(uintptr_t)cube_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,cube_metrics->s,64);
    rumboot_platform_request_file(cube_file_tag,(uintptr_t)op);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op2 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file(metrics_vec_tag,(uintptr_t)vec_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file(vec_file_tag,(uintptr_t)op);
    rumboot_free((void*) vec_metrics);
    return op;
  } 
  
  return NULL;
}

void* nu_vpe_load_op2_misaligned_by_tags(int heap_id, ConfigOp2* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag, int misalign) {
  void* op;
  CubeMetrics* cube_metrics;
  VectorMetrics* vec_metrics;
  
    // Try If OP2 Is A Cube
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Element) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Element) ) {
    cube_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(CubeMetrics),sizeof(int32_t));
    rumboot_platform_request_file(metrics_cube_tag,(uintptr_t)cube_metrics);
    op = rumboot_malloc_from_heap_misaligned(heap_id,cube_metrics->s,64,misalign);
    rumboot_platform_request_file(cube_file_tag,(uintptr_t)op);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op2 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file(metrics_vec_tag,(uintptr_t)vec_metrics);
    op = rumboot_malloc_from_heap_misaligned(heap_id,vec_metrics->s,64,misalign);
    rumboot_platform_request_file(vec_file_tag,(uintptr_t)op);
    rumboot_free((void*) vec_metrics);
    return op;
  } 
  
  return NULL;
}

int nu_vpe_load_status_regs_by_tag(int heap_id, VPEStatusRegs* status_regs, char* status_regs_tag){
  uint32_t* r_bin;
  
  r_bin = rumboot_malloc_from_heap_aligned(heap_id,SIZEOF_VPEStatusRegs_BIN,sizeof(int32_t));
  if(r_bin == NULL)
    return 1;
  
  rumboot_platform_request_file(status_regs_tag,(uintptr_t)r_bin);
  
  nu_vpe_load_status_regs(status_regs, r_bin);
  rumboot_free((void*)r_bin);
  return 0;
}

int nu_bitwise_compare(void* res_data, void* etalon, int size) {
  return memcmp((char*)res_data,(char*)etalon,size)  ;
}

int nu_half_compare_eps(void* res_data, void* etalon, int size, int eps) {
  int res = 0;

  int i, ri, ei, eps_cmp;

  for (i=0; i < size>>1 && !res; i=i+2) {
  //for (i=0; i < 4 && !res; i=i+2) {

    ri = (*((int*)res_data + i + 1)&0xF) << 8 | *((int*)res_data + i)&0xF;
    ei = (*((int*)etalon + i + 1)&0xF) << 8 | *((int*)etalon + i)&0xF;

    if (ri > ei)  eps_cmp = ri - ei;
    else          eps_cmp = ei - ri;

    if (eps_cmp > eps) {
      res = 1;

      rumboot_printf("ERROR: Data Mismatch Address: %08x, Read Data: %08x, Address: %08x, Read Data: %08x\n", *((int*)res_data+i), ri, *((int*)etalon+i), ei);
    }
  }

  return res;
}

void nu_vpe_interpret_mismatch_print_op01(ConfigOp01 *op_config,void* op,uint32_t offset_out,int C) {
  uint32_t offset_op;
  if((op_config->alu_mode == Mode_Channel && op_config->alu_en == Enable_En) ||
     (op_config->mux_mode == Mode_Channel && op_config->mux_en == Enable_En))
    offset_op = offset_out % C;
  else 
    offset_op = offset_out;
  
  if(op_config->alu_mode == Mode_Channel && op_config->alu_en == Enable_En &&
     op_config->mux_mode == Mode_Channel && op_config->mux_en == Enable_En ) { // sengvitch :)
    offset_op = offset_op << 1; 
    if(op_config->coef_type == DataType_Int8) {
      uint8_t* op8 = (uint8_t*) op;
      rumboot_printf("Op: 0x%x 0x%x\n",op8[offset_op],op8[offset_op+1]);
    } else { // 16bit
      uint16_t* op16 = (uint16_t*) op;
      rumboot_printf("Op: 0x%x 0x%x\n",op16[offset_op],op16[offset_op+1]);
    }
  } 
  else {
    if(op_config->coef_type == DataType_Int8) {
      uint8_t* op8 = (uint8_t*) op;
      rumboot_printf("Op: 0x%x\n",op8[offset_op]);
    } else { // 16bit
      uint16_t* op16 = (uint16_t*) op;
      rumboot_printf("Op: 0x%x\n",op16[offset_op]);
    }
  }
}

void nu_vpe_interpret_mismatch_print_op2(ConfigOp2 *op_config,void* op,uint32_t offset_out,int C) {
  uint32_t offset_op;
  if((op_config->alu_mode == Mode_Channel && op_config->alu_en == Enable_En) ||
     (op_config->mux_mode == Mode_Channel && op_config->mux_en == Enable_En) )
    offset_op = offset_out % C;
  else 
    offset_op = offset_out;
  
  if(op_config->alu_mode == Mode_Channel && op_config->alu_en == Enable_En &&
     op_config->mux_mode == Mode_Channel && op_config->mux_en == Enable_En ) { // sengvitch :)
    offset_op = offset_op << 1; 
    if(op_config->coef_type == DataType_Int8) {
      uint8_t* op8 = (uint8_t*) op;
      rumboot_printf("Op: 0x%x 0x%x\n",op8[offset_op],op8[offset_op+1]);
    } else { // 16bit
      uint16_t* op16 = (uint16_t*) op;
      rumboot_printf("Op: 0x%x 0x%x\n",op16[offset_op],op16[offset_op+1]);
    }
  } 
  else {
    if(op_config->coef_type == DataType_Int8) {
      uint8_t* op8 = (uint8_t*) op;
      rumboot_printf("Op: 0x%x\n",op8[offset_op]);
    } else { // 16bit
      uint16_t* op16 = (uint16_t*) op;
      rumboot_printf("Op: 0x%x\n",op16[offset_op]);
    }
  }
}

void nu_vpe_interpret_mismatch(ConfigVPE *cfg,void* res_data,void* etalon,void* in_data,void* op0,void* op1,void* op2,int s,int C) {
  uint32_t mismatch_offset;
  uint8_t* r;uint8_t* e;
  int myexit;
  
  mismatch_offset=0; r=(uint8_t*)res_data; e=(uint8_t*)etalon; myexit=0;
  while(mismatch_offset<s && myexit==0) {
    if(*r != *e)
      myexit=1;
    if(myexit==0) {
      r++;e++;
      mismatch_offset++;
    }
  }
  
  if(myexit!=0) {
    uint32_t offset_out;
    if(cfg->out_data_type == DataType_Int8)
      offset_out = mismatch_offset;
    else  // 16bit
      offset_out = mismatch_offset >> 1;
    rumboot_printf("Mismatched Data Index: %d\n",offset_out);
    
    if(cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32) {
      uint32_t* in32;
      in32 = (uint32_t*) in_data;
      rumboot_printf("In Data Item: 0x%x\n",in32[offset_out]);
    } else if(cfg->in_data_type == DataTypeExt_Int16 || cfg->in_data_type == DataTypeExt_Fp16) {
      uint16_t* in16;
      in16 = (uint16_t*) in_data;
      rumboot_printf("In Data Item: 0x%x\n",in16[offset_out]);
    } else {
      uint8_t* in8;
      in8 = (uint8_t*) in_data;
      rumboot_printf("In Data Item: 0x%x\n",in8[offset_out]);
    }
    
    if(op0 != NULL){
      rumboot_printf("Op0: ");
      nu_vpe_interpret_mismatch_print_op01(&(cfg->op0_config),op0,offset_out,C);
    }
    if(op1 != NULL){
      rumboot_printf("Op1: ");
      nu_vpe_interpret_mismatch_print_op01(&(cfg->op1_config),op1,offset_out,C);
    }
    if(op2 != NULL){
      rumboot_printf("Op2: ");
      nu_vpe_interpret_mismatch_print_op2(&(cfg->op2_config),op2,offset_out,C);
    }
    
    if(cfg->out_data_type == DataType_Int8) {
      uint8_t* out8 = (uint8_t*) res_data;
      uint8_t* et8  = (uint8_t*) etalon;
      rumboot_printf("Result Data Item: 0x%x\n", out8[offset_out]);
      rumboot_printf("Etalon Data Item: 0x%x\n",  et8[offset_out]);
    } else { // 16bit
      uint16_t* out16 = (uint16_t*) res_data;
      uint16_t* et16  = (uint16_t*) etalon;
      rumboot_printf("Result Data Item: 0x%x\n", out16[offset_out]);
      rumboot_printf("Etalon Data Item: 0x%x\n",  et16[offset_out]);
    }
      
  }
}

char* int_to_strdec (char* str, int arg) {

  int i, msd;
  char d[10];

  d[0] = (arg%10)/1;
  d[1] = (arg%100)/10;
  d[2] = (arg%1000)/100;
  d[3] = (arg%10000)/1000;
  d[4] = (arg%100000)/10000;
  d[5] = (arg%1000000)/100000;
  d[6] = (arg%10000000)/1000000;
  d[7] = (arg%100000000)/10000000;
  d[8] = (arg%1000000000)/100000000;
  d[9] = (arg%10000000000)/1000000000;

  for (msd=0, i=0; i<10; i++) if (d[i]) msd = i;

  for (i=0; i<msd+1; i++) {
    if      (d[i] == 0) *(str+msd-i) = '0';
    else if (d[i] == 1) *(str+msd-i) = '1';
    else if (d[i] == 2) *(str+msd-i) = '2';
    else if (d[i] == 3) *(str+msd-i) = '3';
    else if (d[i] == 4) *(str+msd-i) = '4';
    else if (d[i] == 5) *(str+msd-i) = '5';
    else if (d[i] == 6) *(str+msd-i) = '6';
    else if (d[i] == 7) *(str+msd-i) = '7';
    else if (d[i] == 8) *(str+msd-i) = '8';
    else if (d[i] == 9) *(str+msd-i) = '9';
  }

  *(str+msd+1) = '\0';

  //rumboot_printf ("nmb %d %s\n", arg, str);

  return str;
}

int str_length(char* str) {
  int i, len;

  for (len=0, i=0; !len && i<1023; i++) if (*(str+i) == '\0') len = i;

  if (i > 1023) rumboot_printf ("ERROR:str_length: string is too long\n");

  //rumboot_printf ("%s\n", len);

  return len;
}

char* str_concat(char* lstr, char* rstr) {
  int i, lenl, lenr;

  lenl= str_length(lstr);
  lenr= str_length(rstr);

  for (i=0; i<lenr+1; i++) *(lstr+lenl+i) = *(rstr+i);

  //rumboot_printf ("%s\n", lstr);

  return lstr;
}

char* str_copy (char* dst, char* src) {
  int i, len;

  len = str_length(src);

  for (i=0; i<len+1; i++) *(dst+i) = *(src+i);

  //rumboot_printf ("%s\n", dst);

  return dst;
}

char* fn_base_it_nmb (char* dst, char* src, int it_nmb) {
  char it_nmb_str[8];

  //int_to_strdec(&it_nmb_str, it_nmb);
  //str_copy(dst, src);
  //str_concat(dst, &it_nmb_str);
  //str_concat(dst, "_");

  sprintf (it_nmb_str, "%d", it_nmb);
  strcpy(dst, src);
  strcat(dst, it_nmb_str);
  strcat(dst, "_");

  //rumboot_printf("%s\n", dst);

  return dst;
}