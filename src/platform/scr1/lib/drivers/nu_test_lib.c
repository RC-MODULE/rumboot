#include <stdint.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices/nu_lib.h> 

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
