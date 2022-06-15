#include <stdint.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <regs/regs_na.h>

#include <devices/rcm_na/nu_cpdmac_lib.h>
#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 

#ifdef RUMBOOT_PLATFORM_SCR1
#include <platform/devices.h>
#endif

int nu_get_heap_id() {
#ifdef RUMBOOT_NATIVE
  return 1; 
#else
  return 1; 
#endif
}
int nu_get_add_heap_id() {
#ifdef RUMBOOT_NATIVE
  return 2; 
#else
  return 2; 
#endif
}

void nu_get_rather_fair_heap_map(NAHeapMap* heap_map) {
  heap_map->in_data            =nu_get_heap_id();
  heap_map->warr               =nu_get_add_heap_id();
  heap_map->op0                =nu_get_heap_id();
  heap_map->op1                =nu_get_add_heap_id();
  heap_map->op2                =nu_get_heap_id();
  heap_map->lut                =nu_get_heap_id();
  heap_map->mpe_cfg            =nu_get_heap_id();
  heap_map->mpe_cfg_lut        =nu_get_heap_id();
  heap_map->vpe_cfg            =nu_get_heap_id();
  heap_map->ppe_cfg            =nu_get_heap_id();
  heap_map->res                =nu_get_add_heap_id();
  heap_map->etalon             =nu_get_heap_id();
  heap_map->metrics_in_data    =nu_get_heap_id();
  heap_map->metrics_warr       =nu_get_heap_id();
  heap_map->metrics_op0        =nu_get_heap_id();
  heap_map->metrics_op1        =nu_get_heap_id();
  heap_map->metrics_op2        =nu_get_heap_id();
  heap_map->metrics_lut        =nu_get_heap_id();
  heap_map->metrics_mpe_cfg    =nu_get_heap_id();
  heap_map->metrics_mpe_cfg_lut=nu_get_heap_id();
  heap_map->metrics_vpe_cfg    =nu_get_heap_id();
  heap_map->metrics_ppe_cfg    =nu_get_heap_id();
  // heap_map->metrics_res        =nu_get_heap_id();
  heap_map->metrics_etalon     =nu_get_heap_id();
}

  // This Should Return Time In Nanoseconds
uint32_t nu_get_uptime_ns() {
  // On SCR1 Platform Is Made By Direct Timer Request (Not Through rumboot_platform_get_uptime)
#ifdef RUMBOOT_PLATFORM_SCR1
  return ioread32(SCR1_TIMER_BASE + 8) * 10;
#else
  // On Other Platforms
  return rumboot_platform_get_uptime() * 1000;
#endif
}

uint32_t mask_N_M (int N, int M) {
  uint32_t mask_n_0   = (uint32_t)(0xFFFFFFFF) >> (32-1)-N;
  uint32_t mask_31_m  = (uint32_t)(0xFFFFFFFF) << M;

  return mask_n_0 & mask_31_m;
}

uintptr_t nu_virt_to_dma(volatile const void *addr) {
  if(addr==NULL)
    return 0;
  else
    return rumboot_virt_to_dma(addr);
}

int nu_vpe_load_cfg(int heap_id, ConfigVPE* cfg) {
  uint32_t *cfg_bin;
  uint32_t size;
  
  size = NU_VPE_CFG_PARAMS_NUM*sizeof(uint32_t);
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file_ex("cfg_file_tag", (uintptr_t)cfg_bin, size);
  
  nu_vpe_load_config(cfg, cfg_bin);  // Move The VPE Settings From Binary To Struct
  rumboot_free((void*) cfg_bin);
  return 0;
}

int nu_vpe_load_array_of_cfg(int heap_id, ConfigVPE* array_of_cfg, int num) {
  uint32_t *cfg_bin;
  uint32_t *cfg_bin_i;
  ConfigVPE* cfg_i;
  uint32_t size;
  
  size = NU_VPE_CFG_PARAMS_NUM*sizeof(uint32_t)*num;
  
  cfg_i = array_of_cfg;
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file_ex("cfg_file_tag", (uintptr_t)cfg_bin, size);
  
  cfg_bin_i = cfg_bin;
  for(int i=0;i<num;i++) {
    nu_vpe_load_config(cfg_i, cfg_bin_i);
    cfg_i += 1;
    cfg_bin_i += NU_VPE_CFG_PARAMS_NUM;
  }
  rumboot_free((void*)cfg_bin);
  return 0;
}

int nu_vpe_load_cfg_by_tag(int heap_id, ConfigVPE* cfg, char* cfg_file_tag) {
  uint32_t *cfg_bin;
  uint32_t size;
  
  size = NU_VPE_CFG_PARAMS_NUM*sizeof(uint32_t);
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file_ex(cfg_file_tag, (uintptr_t)cfg_bin, size);
  
  nu_vpe_load_config(cfg, cfg_bin);  // Move The VPE Settings From Binary To Struct
  rumboot_free((void*) cfg_bin);
  return 0;
}

int nu_mpe_load_cfg(int heap_id, ConfigMPE* cfg) {
  uint32_t* cfg_bin;
  uint32_t size;
  
  size = NU_MPE_CFG_PARAMS_NUM*sizeof(uint32_t);
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file_ex("cfg_mpe_file_tag",(uintptr_t)cfg_bin, size);
  
  nu_mpe_load_config(cfg,cfg_bin);
  rumboot_free((void*) cfg_bin);
  return 0;
}

int nu_mpe_load_cfg_by_tag(int heap_id, ConfigMPE* cfg, char* cfg_file_tag) {
  uint32_t *cfg_bin;
  uint32_t size;
  
  size = NU_MPE_CFG_PARAMS_NUM*sizeof(uint32_t);
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file_ex(cfg_file_tag, (uintptr_t)cfg_bin, size);
  
  nu_mpe_load_config(cfg, cfg_bin);  // Move The MPE Settings From Binary To Struct
  rumboot_free((void*) cfg_bin);
  return 0;
}

void* nu_mpe_load_cfg_lut(int heap_id) { // :-( Returns A Pointer - Bahaves Not Like Other *_load_cfg*
  void* lut;
  uint32_t size;
  
  size = 0;
  rumboot_platform_request_file_ex("metrics_mpe_cfg_lut_tag", (uintptr_t) &size, sizeof (size) ); // Read The Size Of MPE CFG LUT
  if(size==0)
    return NULL;
  
  lut = rumboot_malloc_from_heap_aligned(heap_id,size+sizeof(uint32_t),sizeof(uint32_t));
  if(lut ==NULL)
    return NULL;
  
  memset(lut,0,size+sizeof(uint32_t)); // Init - Because The Table Seeker Searches For Zero As The End Of A Table
  rumboot_platform_request_file_ex("mpe_cfg_lut_file_tag",(uintptr_t)lut,size);
  
  return lut;
}

int nu_mpe_load_array_of_cfg(int heap_id, ConfigMPE* array_of_cfg, int num) {
  uint32_t *cfg_bin;
  uint32_t *cfg_bin_i;
  ConfigMPE* cfg_i;
  uint32_t size;
  
  size = NU_MPE_CFG_PARAMS_NUM*sizeof(uint32_t)*num;
  
  cfg_i = array_of_cfg;
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file_ex("cfg_mpe_file_tag", (uintptr_t)cfg_bin, size);
  
  cfg_bin_i = cfg_bin;
  for(int i=0;i<num;i++) {
    nu_mpe_load_config(cfg_i, cfg_bin_i);
    cfg_i += 1;
    cfg_bin_i += NU_MPE_CFG_PARAMS_NUM;
  }
  rumboot_free((void*)cfg_bin);
  return 0;
}

int nu_ppe_load_cfg(int heap_id, ConfigPPE* cfg) {
  uint32_t* cfg_bin;
  uint32_t size;
  
  size = NU_PPE_CFG_PARAMS_NUM*sizeof(uint32_t);
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));
  if(cfg_bin == NULL)
    return 1;
  rumboot_platform_request_file_ex("cfg_ppe_file_tag",(uintptr_t)cfg_bin,size);
  
  nu_ppe_load_config(cfg,cfg_bin);
  rumboot_free((void*) cfg_bin);
  return 0;
}

int nu_ppe_load_cfg_by_tag(int heap_id, ConfigPPE* cfg, char* cfg_file_tag) {
  uint32_t *cfg_bin;
  uint32_t size;
  
  size = NU_PPE_CFG_PARAMS_NUM*sizeof(uint32_t);
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));

  if(cfg_bin==NULL) return 1;

  rumboot_platform_request_file_ex(cfg_file_tag, (uintptr_t)cfg_bin, size);
  nu_ppe_load_config(cfg, cfg_bin);

  rumboot_free((void*) cfg_bin);
  return 0;
}

int nu_ppe_load_cfg_reg_by_tag(int heap_id, ConfigREGPPE* cfg_reg, char* cfg_reg_file_tag) {
  int res = 0;

  uint32_t* cfg_reg_bin;
  uint32_t size;
  
  size = NU_PPE_REG_CFG_PARAMS_NUM*sizeof(uint32_t);
  
  cfg_reg_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));

  if (cfg_reg_bin==NULL || cfg_reg==NULL || cfg_reg_file_tag==NULL) res = 1;

  if (!res) {
    rumboot_platform_request_file_ex(cfg_reg_file_tag, (uintptr_t)cfg_reg_bin, size);

    res = nu_ppe_reg_load_config(cfg_reg, cfg_reg_bin);
  }

  if (!res) rumboot_free((void*) cfg_reg_bin);

  return res;
}

int nu_ppe_load_array_of_cfg(int heap_id, ConfigPPE* array_of_cfg, int num) {
  uint32_t *cfg_bin;
  uint32_t *cfg_bin_i;
  ConfigPPE* cfg_i;
  uint32_t size;
  
  size = NU_PPE_CFG_PARAMS_NUM*sizeof(uint32_t)*num;
  
  cfg_i = array_of_cfg;
  
  cfg_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));
  if(cfg_bin==NULL)
    return 1;
  rumboot_platform_request_file_ex("cfg_ppe_file_tag", (uintptr_t)cfg_bin, size);
  
  cfg_bin_i = cfg_bin;
  for(int i=0;i<num;i++) {
    nu_ppe_load_config(cfg_i, cfg_bin_i);
    cfg_i += 1;
    cfg_bin_i += NU_PPE_CFG_PARAMS_NUM;
  }
  rumboot_free((void*)cfg_bin);
  return 0;
}

CubeMetrics* nu_load_cube_metrics(int heap_id, char* file_tag) {
  CubeMetrics* m;
  uint32_t size;
  size = sizeof(CubeMetrics);
  m = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));
  if(m == NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t)m,size);
  return m;
}

WarrMetrics* nu_load_warr_metrics(int heap_id, char* file_tag) {
  WarrMetrics* m;
  uint32_t size;
  size = sizeof(WarrMetrics);
  m = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));
  if(m == NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t)m,size);
  return m;
}

VectorMetrics* nu_load_vec_metrics(int heap_id, char* file_tag) {
  VectorMetrics* m;
  uint32_t size;
  size = sizeof(VectorMetrics);
  m = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));
  if(m == NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t)m,size);
  return m;
}

CubeMetrics* nu_load_array_of_cube_metrics(int heap_id, char* file_tag, int num) {
  CubeMetrics* m;
  uint32_t size;
  size = sizeof(CubeMetrics)*num;
  m = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));
  if(m==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t)m,size);
  return m;
}

VectorMetrics* nu_load_array_of_vec_metrics(int heap_id, char* file_tag, int num) {
  VectorMetrics* m;
  uint32_t size;
  size = sizeof(VectorMetrics)*num;
  m = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));
  if(m==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t) m,size);
  return m;
}

WarrMetrics* nu_load_array_of_warr_metrics(int heap_id, char* file_tag, int num) {
  WarrMetrics* m;
  uint32_t size;
  size = sizeof(WarrMetrics)*num;
  m = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));
  if(m==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t)m,size);
  return m;
}

MPECmdMetrics* nu_mpe_load_cmd_metrics(int heap_id) {
  MPECmdMetrics* m;
  uint32_t size;
  size = sizeof(MPECmdMetrics);
  m = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));
  if(m==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex("metrics_mpe_cmd_tag",(uintptr_t)m,size);
  return m;
}

MPECmdMetrics* nu_mpe_load_cmd_metrics_by_tag(int heap_id, char* file_tag) {
  MPECmdMetrics* m;
  uint32_t size;
  size = sizeof(MPECmdMetrics);
  m = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));
  if(m==NULL)
    return NULL;

  rumboot_platform_request_file_ex(file_tag,(uintptr_t)m,size);
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
  uint32_t size;
  size = metrics->s /*size in bytes*/;
  c = rumboot_malloc_from_heap_aligned(heap_id,size,64/*aligned by 64 bytes*/);
  if(c==NULL) 
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag, (uintptr_t)c, size);
  return c;
}

void* nu_load_cube_misaligned(int heap_id,char* file_tag,CubeMetrics* metrics, int misalign) {
  void* c;
  uint32_t size;
  size = metrics->s;
  c = rumboot_malloc_from_heap_misaligned(heap_id,size,64,misalign);
  if(c==NULL) 
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag, (uintptr_t)c, size);
  return c;
}

void* nu_load_warr(int heap_id,char* file_tag,WarrMetrics* metrics) {
  void* w;
  uint32_t size;
  size = metrics->s;
  w = rumboot_malloc_from_heap_aligned(heap_id,size,64); // CHECK
  if(w==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t)w,size);
  return w;
}

void* nu_load_vec(int heap_id,char* file_tag,VectorMetrics* metrics) {
  void* v;
  uint32_t size;
  size = metrics->s;
  v = rumboot_malloc_from_heap_aligned(heap_id,size,64); // CHECK
  if(v==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t)v,size);
  return v;
}

void* nu_load_vec_misaligned(int heap_id,char* file_tag,VectorMetrics* metrics, int misalign) {
  void* v;
  uint32_t size;
  size = metrics->s;
  v = rumboot_malloc_from_heap_misaligned(heap_id,size,64,misalign); // CHECK
  if(v==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t)v,size);
  return v;
}

void* nu_mpe_load_cmd(int heap_id,MPECmdMetrics* metrics) {
  void* c;
  uint32_t size;
  size = metrics->s;
  c = rumboot_malloc_from_heap_aligned(heap_id,size,8);
  if(c==NULL)
    return c;
  
  rumboot_platform_request_file_ex("mpe_cmd_file_tag",(uintptr_t)c,size);
  return c;
}

void* nu_mpe_load_cmd_by_tag(int heap_id, char* file_tag, MPECmdMetrics* metrics) {
  void* c;
  uint32_t size;
  size = metrics->s;
  c = rumboot_malloc_from_heap_aligned(heap_id,size,8);
  if(c==NULL)
    return c;

  rumboot_platform_request_file_ex(file_tag,(uintptr_t)c,size);
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


void* nu_malloc_array_of_cubes(int heap_id,CubeMetrics* array_of_metrics,int num,int* size) {
  void* aoc;
  
  *size=0;
  for(int i=0;i<num;i++) {
    *size += array_of_metrics[i].s;
  }
  
  aoc = rumboot_malloc_from_heap(heap_id,*size);
  //aoc = rumboot_malloc_from_heap_aligned(heap_id,*size,16/*Dont Know*/);
  if(aoc==NULL)
    return NULL;
  
  return aoc;
}

void* nu_malloc_array_of_vecs(int heap_id,VectorMetrics* array_of_metrics,int num,int* size) {
  void* aov;
  
  *size=0;
  for(int i=0;i<num;i++) {
    *size += array_of_metrics[i].s;
  }
  
  aov = rumboot_malloc_from_heap_aligned(heap_id,*size,16);
  if(aov == NULL)
    return NULL;
  
  return aov;
}

void* nu_malloc_array_of_warr(int heap_id,WarrMetrics* array_of_metrics,int num,int* size) {
  void* aow;
  
  *size=0;
  for(int i=0;i<num;i++) {
      //rumboot_printf("*size += array_of_metrics[%d].s\n", i);
    *size += array_of_metrics[i].s;
  }
  //rumboot_printf("rumboot_malloc_from_heap_aligned\n");
  aow = rumboot_malloc_from_heap_aligned(heap_id,*size,16);
  if(aow==NULL)
    return NULL;
  
  return aow;
}

void* nu_malloc_array_of_res(int heap_id,CubeMetrics* array_of_metrics,int num) {
  void* aor;
  int size;
  
  aor=nu_malloc_array_of_cubes(heap_id,array_of_metrics,num,&size);
  if(aor==NULL)
    return NULL;
  
  memset(aor,0xA5,size);
  return aor;
}

void* nu_load_array_of_cubes(int heap_id,char* file_tag,CubeMetrics* array_of_metrics,int num) {
  void* aoc;
  int size;

  aoc = nu_malloc_array_of_cubes(heap_id,array_of_metrics,num,&size);
  if(aoc==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t)aoc,size);
  return aoc;
}

void* nu_load_array_of_vecs(int heap_id,char* file_tag,VectorMetrics* array_of_metrics,int num) {
  void* aov;
  int size;
  
  aov = nu_malloc_array_of_vecs(heap_id,array_of_metrics,num,&size);
  if(aov==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t) aov,size);
  return aov;
}

void* nu_load_array_of_warr(int heap_id,char* file_tag,WarrMetrics* array_of_metrics,int num) {
  void* aow;
  int size;
  
  aow = nu_malloc_array_of_warr(heap_id,array_of_metrics,num,&size);
  if(aow==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex(file_tag,(uintptr_t) aow,size);
  return aow;
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
    rumboot_platform_request_file_ex(index?"metrics_op1_cube_tag":"metrics_op0_cube_tag",(uintptr_t)cube_metrics,sizeof(CubeMetrics));
    op = rumboot_malloc_from_heap_aligned(heap_id,cube_metrics->s,64);
    rumboot_platform_request_file_ex(index?"op1_cube_file_tag":"op0_cube_file_tag",(uintptr_t)op,cube_metrics->s);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op0 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file_ex(index?"metrics_op1_vec_tag":"metrics_op0_vec_tag",(uintptr_t)vec_metrics,sizeof(VectorMetrics));
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file_ex(index?"op1_vec_file_tag":"op0_vec_file_tag",(uintptr_t)op,vec_metrics->s);
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
    rumboot_platform_request_file_ex(metrics_cube_tag,(uintptr_t)cube_metrics,sizeof(CubeMetrics));
    op = rumboot_malloc_from_heap_aligned(heap_id,cube_metrics->s,64);
    rumboot_platform_request_file_ex(cube_file_tag,(uintptr_t)op,cube_metrics->s);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op0 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file_ex(metrics_vec_tag,(uintptr_t)vec_metrics,sizeof(VectorMetrics));
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file_ex(vec_file_tag,(uintptr_t)op,vec_metrics->s);
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
    rumboot_platform_request_file_ex(metrics_cube_tag,(uintptr_t)cube_metrics,sizeof(CubeMetrics));
    op = rumboot_malloc_from_heap_misaligned(heap_id,cube_metrics->s,64,misalign);
    rumboot_platform_request_file_ex(cube_file_tag,(uintptr_t)op,cube_metrics->s);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op0 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file_ex(metrics_vec_tag,(uintptr_t)vec_metrics,sizeof(VectorMetrics));
    op = rumboot_malloc_from_heap_misaligned(heap_id,vec_metrics->s,64,misalign);
    rumboot_platform_request_file_ex(vec_file_tag,(uintptr_t)op,vec_metrics->s);
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
    rumboot_platform_request_file_ex("metrics_op2_cube_tag",(uintptr_t)cube_metrics,sizeof(CubeMetrics));
    op = rumboot_malloc_from_heap_aligned(heap_id,cube_metrics->s,64);
    rumboot_platform_request_file_ex("op2_cube_file_tag",(uintptr_t)op,cube_metrics->s);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op2 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file_ex("metrics_op2_vec_tag",(uintptr_t)vec_metrics,sizeof(VectorMetrics));
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file_ex("op2_vec_file_tag",(uintptr_t)op,vec_metrics->s);
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
    rumboot_platform_request_file_ex(metrics_cube_tag,(uintptr_t)cube_metrics,sizeof(CubeMetrics));
    op = rumboot_malloc_from_heap_aligned(heap_id,cube_metrics->s,64);
    rumboot_platform_request_file_ex(cube_file_tag,(uintptr_t)op,cube_metrics->s);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op2 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file_ex(metrics_vec_tag,(uintptr_t)vec_metrics,sizeof(VectorMetrics));
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file_ex(vec_file_tag,(uintptr_t)op,vec_metrics->s);
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
    rumboot_platform_request_file_ex(metrics_cube_tag,(uintptr_t)cube_metrics,sizeof(CubeMetrics));
    op = rumboot_malloc_from_heap_misaligned(heap_id,cube_metrics->s,64,misalign);
    rumboot_platform_request_file_ex(cube_file_tag,(uintptr_t)op,cube_metrics->s);
    rumboot_free((void*) cube_metrics);
    return op;
  } 
    
    // Try If Op2 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file_ex(metrics_vec_tag,(uintptr_t)vec_metrics,sizeof(VectorMetrics));
    op = rumboot_malloc_from_heap_misaligned(heap_id,vec_metrics->s,64,misalign);
    rumboot_platform_request_file_ex(vec_file_tag,(uintptr_t)op,vec_metrics->s);
    rumboot_free((void*) vec_metrics);
    return op;
  } 
  
  return NULL;
}

int nu_vpe_load_arrays_of_op_metrics(
  int heap_id_op0, int heap_id_op1, int heap_id_op2, int heap_id_lut,
  OpArrayDescriptor* op0_array_desc,
  OpArrayDescriptor* op1_array_desc,
  OpArrayDescriptor* op2_array_desc,
  ConfigVPE* array_of_cfg, 
  int num
) {
  uint32_t size_op0_cubes;
  uint32_t size_op1_cubes;
  uint32_t size_op2_cubes;
  uint32_t size_op0_vecs;
  uint32_t size_op1_vecs;
  uint32_t size_op2_vecs;
  uint32_t size_lut1;
  uint32_t size_lut2;
  
  op0_array_desc->num_cubes=0;
  op0_array_desc->num_vecs=0;
  op0_array_desc->num_luts=0;
  op1_array_desc->num_cubes=0;
  op1_array_desc->num_vecs=0;
  op1_array_desc->num_luts=0;
  op2_array_desc->num_cubes=0;
  op2_array_desc->num_vecs=0;
  op2_array_desc->num_luts=0;
  
  
  
    // For Each Configuration
  for(int i=0;i<num;i++) {
      
      
      // Calculate The Size Of OP0 Metrics Arrays
    if(array_of_cfg[i].op0_en==Enable_En) {
      
      if( (array_of_cfg[i].op0_config.alu_en==Enable_En && array_of_cfg[i].op0_config.alu_mode==Mode_Element) ||
          (array_of_cfg[i].op0_config.mux_en==Enable_En && array_of_cfg[i].op0_config.mux_mode==Mode_Element) )  {
            op0_array_desc->num_cubes += (array_of_cfg[i].src_rdma_config.dma_bsize+1);
      }
      if( (array_of_cfg[i].op0_config.alu_en==Enable_En && array_of_cfg[i].op0_config.alu_mode==Mode_Channel) ||
          (array_of_cfg[i].op0_config.mux_en==Enable_En && array_of_cfg[i].op0_config.mux_mode==Mode_Channel) )  {
            op0_array_desc->num_vecs++;
      }
    }
      
      // Calculate The Size Of OP1 Metrics Arrays
    if(array_of_cfg[i].op1_en==Enable_En) {
      if( (array_of_cfg[i].op1_config.alu_en==Enable_En && array_of_cfg[i].op1_config.alu_mode==Mode_Element) ||
          (array_of_cfg[i].op1_config.mux_en==Enable_En && array_of_cfg[i].op1_config.mux_mode==Mode_Element) )  {
             op1_array_desc->num_cubes += (array_of_cfg[i].src_rdma_config.dma_bsize+1);
      }
      if( (array_of_cfg[i].op1_config.alu_en==Enable_En && array_of_cfg[i].op1_config.alu_mode==Mode_Channel) ||
          (array_of_cfg[i].op1_config.mux_en==Enable_En && array_of_cfg[i].op1_config.mux_mode==Mode_Channel) )  {
            op1_array_desc->num_vecs++;
      }
    }
      
      // Calculate The Size Of OP2 Metrics Arrays
    if(array_of_cfg[i].op2_en==Enable_En) {
      if( (array_of_cfg[i].op2_config.alu_en==Enable_En && array_of_cfg[i].op2_config.alu_mode==Mode_Element) ||
          (array_of_cfg[i].op2_config.mux_en==Enable_En && array_of_cfg[i].op2_config.mux_mode==Mode_Element) )  {
              op2_array_desc->num_cubes += (array_of_cfg[i].src_rdma_config.dma_bsize+1);
      }
      if( (array_of_cfg[i].op2_config.alu_en==Enable_En && array_of_cfg[i].op2_config.alu_mode==Mode_Channel) ||
          (array_of_cfg[i].op2_config.mux_en==Enable_En && array_of_cfg[i].op2_config.mux_mode==Mode_Channel) )  {
            op2_array_desc->num_vecs++;
      }
      
      if( array_of_cfg[i].op2_config.lut_en==Enable_En ) {
        op2_array_desc->num_luts++;
      }
    }
    
  }
  
    
    // Allocate The OPx Metrics Arrays
  if(op0_array_desc->num_cubes != 0) {
    size_op0_cubes = op0_array_desc->num_cubes * sizeof(CubeMetrics);
    op0_array_desc->array_of_cube_metrics = rumboot_malloc_from_heap_aligned(heap_id_op0, size_op0_cubes, sizeof(uint32_t));
    if(op0_array_desc->array_of_cube_metrics==NULL) return -1;
  }
  if(op1_array_desc->num_cubes != 0) {
    size_op1_cubes = op1_array_desc->num_cubes * sizeof(CubeMetrics);
    op1_array_desc->array_of_cube_metrics = rumboot_malloc_from_heap_aligned(heap_id_op1, size_op1_cubes, sizeof(uint32_t));
    if(op1_array_desc->array_of_cube_metrics==NULL) return -1;
  }
  if(op2_array_desc->num_cubes != 0) {
    size_op2_cubes = op2_array_desc->num_cubes * sizeof(CubeMetrics);
    op2_array_desc->array_of_cube_metrics = rumboot_malloc_from_heap_aligned(heap_id_op2, size_op2_cubes, sizeof(uint32_t));
    if(op2_array_desc->array_of_cube_metrics==NULL) return -1;
  }
  
  if(op0_array_desc->num_vecs != 0) {
    size_op0_vecs = op0_array_desc->num_vecs * sizeof(VectorMetrics);
    op0_array_desc->array_of_vec_metrics = rumboot_malloc_from_heap_aligned(heap_id_op0, size_op0_vecs, sizeof(uint32_t));
    if(op0_array_desc->array_of_vec_metrics==NULL) return -1;
  }
  if(op1_array_desc->num_vecs != 0) {
    size_op1_vecs = op1_array_desc->num_vecs * sizeof(VectorMetrics);
    op1_array_desc->array_of_vec_metrics = rumboot_malloc_from_heap_aligned(heap_id_op1, size_op1_vecs, sizeof(uint32_t));
    if(op1_array_desc->array_of_vec_metrics==NULL) return -1;
  }
  if(op2_array_desc->num_vecs != 0) {
    size_op2_vecs = op2_array_desc->num_vecs * sizeof(VectorMetrics);
    op2_array_desc->array_of_vec_metrics = rumboot_malloc_from_heap_aligned(heap_id_op2, size_op2_vecs, sizeof(uint32_t));
    if(op2_array_desc->array_of_vec_metrics==NULL) return -1;
  }
  
  if(op2_array_desc->num_luts != 0) {
    size_lut1 = op2_array_desc->num_luts * sizeof(VectorMetrics);
    size_lut2 = op2_array_desc->num_luts * sizeof(VectorMetrics);
    op2_array_desc->array_of_lut1_metrics= rumboot_malloc_from_heap_aligned(heap_id_lut, size_lut1, sizeof(uint32_t));
    op2_array_desc->array_of_lut2_metrics= rumboot_malloc_from_heap_aligned(heap_id_lut, size_lut2, sizeof(uint32_t));
    if(op2_array_desc->array_of_lut1_metrics==NULL || 
       op2_array_desc->array_of_lut2_metrics==NULL) return -1;
  }
  
    // Read Files
  if(op0_array_desc->num_cubes != 0) 
    rumboot_platform_request_file_ex("metrics_op0_cube_tag",(uintptr_t)op0_array_desc->array_of_cube_metrics,size_op0_cubes);
  if(op1_array_desc->num_cubes != 0) 
    rumboot_platform_request_file_ex("metrics_op1_cube_tag",(uintptr_t)op1_array_desc->array_of_cube_metrics,size_op1_cubes);
  if(op2_array_desc->num_cubes != 0) 
    rumboot_platform_request_file_ex("metrics_op2_cube_tag",(uintptr_t)op2_array_desc->array_of_cube_metrics,size_op2_cubes);
  
  if(op0_array_desc->num_vecs != 0) 
    rumboot_platform_request_file_ex("metrics_op0_vec_tag",(uintptr_t)op0_array_desc->array_of_vec_metrics,size_op0_vecs);
  if(op1_array_desc->num_vecs != 0) 
    rumboot_platform_request_file_ex("metrics_op1_vec_tag",(uintptr_t)op1_array_desc->array_of_vec_metrics,size_op1_vecs);
  if(op2_array_desc->num_vecs != 0) 
    rumboot_platform_request_file_ex("metrics_op2_vec_tag",(uintptr_t)op2_array_desc->array_of_vec_metrics,size_op2_vecs);
  
   if(op2_array_desc->num_luts != 0) {
     rumboot_platform_request_file_ex("metrics_lut1_file_tag",(uintptr_t)op2_array_desc->array_of_lut1_metrics,size_lut1);
     rumboot_platform_request_file_ex("metrics_lut2_file_tag",(uintptr_t)op2_array_desc->array_of_lut2_metrics,size_lut2);
   }
  
  return 0;
}

int nu_vpe_load_array_of_op_cubes(int heap_id, OpArrayDescriptor* op_array_desc,char* file_tag)  {
  CubeMetrics* cm;
  int size;
  
  cm = op_array_desc->array_of_cube_metrics;
  size=0;
  for(int i=0;i<op_array_desc->num_cubes;i++) {
    size += cm[i].s;
  }
  
  if(size!=0) {
    op_array_desc->array_of_cubes = rumboot_malloc_from_heap_aligned(heap_id,size,16);
    if(op_array_desc->array_of_cubes == NULL)
      return -1;
    
    rumboot_platform_request_file_ex(file_tag,(uintptr_t) (op_array_desc->array_of_cubes),size);
  }
  
  return 0;
}

int nu_vpe_load_array_of_op_vecs(int heap_id, OpArrayDescriptor* op_array_desc,char* file_tag){
  VectorMetrics* vm;
  int size;
  
  vm = op_array_desc->array_of_vec_metrics;
  size=0;
  for(int i=0;i<op_array_desc->num_vecs;i++) {
    size += vm[i].s;
  }
  
  if(size!=0) {
    op_array_desc->array_of_vecs = rumboot_malloc_from_heap_aligned(heap_id,size,16);
    if(op_array_desc->array_of_vecs == NULL)
      return -1;
    
    rumboot_platform_request_file_ex(file_tag,(uintptr_t) (op_array_desc->array_of_vecs),size );
  }
  
  return 0;
}

int nu_vpe_load_arrays_of_ops(
  int heap_id_op0,int heap_id_op1, int heap_id_op2, int heap_id_lut, 
  OpArrayDescriptor* op0_array_desc,
  OpArrayDescriptor* op1_array_desc,
  OpArrayDescriptor* op2_array_desc
){
  
  if(nu_vpe_load_array_of_op_cubes(heap_id_op0,op0_array_desc,"op0_cube_file_tag") != 0) return -1;
  if(nu_vpe_load_array_of_op_cubes(heap_id_op1,op1_array_desc,"op1_cube_file_tag") != 0) return -1;
  if(nu_vpe_load_array_of_op_cubes(heap_id_op2,op2_array_desc,"op2_cube_file_tag") != 0) return -1;
  
  if(nu_vpe_load_array_of_op_vecs(heap_id_op0,op0_array_desc,"op0_vec_file_tag") != 0) return -1;
  if(nu_vpe_load_array_of_op_vecs(heap_id_op1,op1_array_desc,"op1_vec_file_tag") != 0) return -1;
  if(nu_vpe_load_array_of_op_vecs(heap_id_op2,op2_array_desc,"op2_vec_file_tag") != 0) return -1;
  
  if(op2_array_desc->num_luts !=0 ) {
    op2_array_desc->array_of_lut1=nu_load_array_of_vecs(heap_id_lut,"lut1_file_tag",op2_array_desc->array_of_lut1_metrics,op2_array_desc->num_luts);
    if(op2_array_desc->array_of_lut1==NULL) return -1;
    op2_array_desc->array_of_lut2=nu_load_array_of_vecs(heap_id_lut,"lut2_file_tag",op2_array_desc->array_of_lut2_metrics,op2_array_desc->num_luts);
    if(op2_array_desc->array_of_lut2==NULL) return -1;
  }
  
  return 0;
}

StatusRegs* nu_load_array_of_status_regs(int heap_id,int num) {
  StatusRegs* r;
  uint32_t size;
  
  size = sizeof(StatusRegs)*num;
  
  r = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(uint32_t));
  if(r==NULL)
    return NULL;
  
  rumboot_platform_request_file_ex("status_regs_file_tag",(uintptr_t)r,size);
  return r;
}

int nu_vpe_load_status_regs_by_tag(int heap_id, StatusRegs* status_regs, char* status_regs_tag){
  uint32_t* r_bin;
  uint32_t size;
  
  size = SIZEOF_StatusRegs_BIN;
  
  r_bin = rumboot_malloc_from_heap_aligned(heap_id,size,sizeof(int32_t));
  if(r_bin == NULL)
    return 1;
  
  rumboot_platform_request_file_ex(status_regs_tag,(uintptr_t)r_bin,size);
  
  nu_vpe_load_status_regs(status_regs, r_bin);
  rumboot_free((void*)r_bin);
  return 0;
}

int nu_bitwise_compare(void* res_data, void* etalon, int size) {
  return memcmp((char*)res_data,(char*)etalon,size)  ;
}

float fp16_to_fp32(const short in){
    signed int t1, t2, t3;
    float out = 0;

    t1 = ((in & 0x7fff) << 13) + 0x38000000; 
    t2 = (in & 0x8000) << 16;
    t3 = in & 0x7c00;
    
    if(t3==0x7c00) // NaN or inf
      if(in & 0x3FF) // NaN
        t1 = t2 | 0x7FFFFFFF;
      else // inf
        t1 = t2 | 0x7F800000;
    else {
      t1 = (t3==0 ? 0 : t1); // Denorm Correction
      t1 |= t2;
    }
  
    out = *( (float*) &t1);
//    memcpy(&out, &t1, sizeof(out));
    
    return out;
}



int nu_half_compare_eps(void* res_data, void* etalon, int size) {
  
  int16_t* r_ptr;
  int16_t* e_ptr;
  int16_t ri;
  int16_t ei;
  //~ int16_t diff;
  
  r_ptr = (int16_t*) res_data;
  e_ptr = (int16_t*) etalon;
  for(int i=0; i< size/sizeof(int16_t); i++) {
    ri = *r_ptr;
    ei = *e_ptr;
    
    if(ri != ei) {
      rumboot_printf("inacc == %x vs %x ===\n",ri,ei);
      
        // Buggy Crutch
      //~ if(ri > ei) diff = ri - ei;
      //~ else        diff = ei - ri;
      
      //~ if(diff > eps) {
        float rf;
        float ef;
        float abs_rf;
        float abs_ef;
        const float small_epsilon = 0.0009765625f; // Exponent = -10 
        int mis;
        
        rf = fp16_to_fp32(ri);
        ef = fp16_to_fp32(ei);
        abs_rf = fabs(rf);
        abs_ef = fabs(ef);
        
        mis=0;
        if(abs_rf < small_epsilon || abs_ef < small_epsilon) {  // Also Negative Zero
          if(abs_rf > 0.125f || abs_ef > 0.125f) // Was Decided By v.gordeev As OK
            mis=1;
        }
        else {
          if( (ri & 0x8000) ^ (ei & 0x8000) )  // Different Signs
            mis=1;
          else {
            float max;
            float min;
            
            if(abs_rf > abs_ef) {
              max = abs_rf;
              min = abs_ef;
            }
            else {
              max = abs_ef;
              min = abs_rf;
            }
            
            if( max - min > 0.0125f * max)
              mis=1;
          }
        }
        
        if(mis) {
          rumboot_printf("ERROR: Data Mismatch Address: %x, Read Data: %x, Address: %x, Read Data: %x\n", (uint32_t)r_ptr, ri, (uint32_t)e_ptr , ei);
          return 1;
        }
      //~ }
    }
    
    r_ptr++;
    e_ptr++;
  }
  
  return 0;
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



void nu_vpe_init_op_array_desc(OpArrayDescriptor* desc) {
  desc-> array_of_cube_metrics=NULL;
  desc-> array_of_vec_metrics=NULL;
  desc-> array_of_lut1_metrics=NULL;
  desc-> array_of_lut2_metrics=NULL;
  desc-> array_of_cubes=NULL;
  desc-> array_of_vecs=NULL;
  desc-> array_of_lut1=NULL;
  desc-> array_of_lut2=NULL;
  desc-> num_cubes=0;
  desc-> num_vecs=0;
  desc-> num_luts=0;
}

void nu_vpe_init_test_desc(VPETestDescriptor* test_desc) {
  test_desc-> array_of_cfg=NULL;
  
  test_desc-> array_of_in_metrics=NULL;
  test_desc-> array_of_res_metrics=NULL;
  
  test_desc-> array_of_in_data=NULL;
  test_desc-> array_of_etalon=NULL;
  test_desc-> array_of_res_data=NULL;
  
  nu_vpe_init_op_array_desc(&(test_desc->op0_array_desc));
  nu_vpe_init_op_array_desc(&(test_desc->op1_array_desc));
  nu_vpe_init_op_array_desc(&(test_desc->op2_array_desc));

  test_desc-> array_of_status_regs_etalon=NULL;
  
}

void nu_vpe_init_iteration_desc(VPETestDescriptor* test_desc, VPEIterationDescriptor* iteration_desc) {
  iteration_desc->cfg      = test_desc->array_of_cfg;
  iteration_desc->in_data  = test_desc->array_of_in_data;
  iteration_desc->etalon   = test_desc->array_of_etalon;
  iteration_desc->res_data = test_desc->array_of_res_data;
  
  iteration_desc->op0_cube = test_desc->op0_array_desc.array_of_cubes;
  iteration_desc->op1_cube = test_desc->op1_array_desc.array_of_cubes;
  iteration_desc->op2_cube = test_desc->op2_array_desc.array_of_cubes;
  iteration_desc->op0_vec  = test_desc->op0_array_desc.array_of_vecs;
  iteration_desc->op1_vec  = test_desc->op1_array_desc.array_of_vecs;
  iteration_desc->op2_vec  = test_desc->op2_array_desc.array_of_vecs;
  iteration_desc->lut1     = test_desc->op2_array_desc.array_of_lut1;
  iteration_desc->lut2     = test_desc->op2_array_desc.array_of_lut2;
  iteration_desc->op0_cube_metrics = test_desc->op0_array_desc.array_of_cube_metrics;
  iteration_desc->op0_vec_metrics  = test_desc->op0_array_desc.array_of_vec_metrics;
  iteration_desc->op1_cube_metrics = test_desc->op1_array_desc.array_of_cube_metrics;
  iteration_desc->op1_vec_metrics  = test_desc->op1_array_desc.array_of_vec_metrics;
  iteration_desc->op2_cube_metrics = test_desc->op2_array_desc.array_of_cube_metrics;
  iteration_desc->op2_vec_metrics  = test_desc->op2_array_desc.array_of_vec_metrics;
  iteration_desc->lut1_metrics     = test_desc->op2_array_desc.array_of_lut1_metrics;
  iteration_desc->lut2_metrics     = test_desc->op2_array_desc.array_of_lut2_metrics;
  
  iteration_desc->status_regs_etalon = test_desc->array_of_status_regs_etalon;
  
  iteration_desc->in_metrics         = test_desc->array_of_in_metrics;
  iteration_desc->res_metrics        = test_desc->array_of_res_metrics;
}

void nu_vpe_print_iteration_desc(VPEIterationDescriptor* iteration_desc) {
  rumboot_printf("VPEIterationDescriptor:\n");
  rumboot_printf("cfg = 0x%x\n",(uintptr_t)(iteration_desc->cfg));

  rumboot_printf("in_data = 0x%x \n",(uintptr_t)(iteration_desc->in_data));
  rumboot_printf("etalon = 0x%x \n",(uintptr_t)(iteration_desc->etalon));
  rumboot_printf("res_data = 0x%x \n",(uintptr_t)(iteration_desc->res_data));
  rumboot_printf("op0_cube = 0x%x \n",(uintptr_t)(iteration_desc->op0_cube));
  rumboot_printf("op1_cube = 0x%x \n",(uintptr_t)(iteration_desc->op1_cube));
  rumboot_printf("op2_cube = 0x%x \n",(uintptr_t)(iteration_desc->op2_cube));
  rumboot_printf("op0_vec = 0x%x \n",(uintptr_t)(iteration_desc->op0_vec));
  rumboot_printf("op1_vec = 0x%x \n",(uintptr_t)(iteration_desc->op1_vec));
  rumboot_printf("op2_vec = 0x%x \n",(uintptr_t)(iteration_desc->op2_vec));
  rumboot_printf("op0 = 0x%x \n",(uintptr_t)(iteration_desc->op0));
  rumboot_printf("op1 = 0x%x \n",(uintptr_t)(iteration_desc->op1));
  rumboot_printf("op2 = 0x%x \n",(uintptr_t)(iteration_desc->op2));
  rumboot_printf("lut1 = 0x%x \n",(uintptr_t)(iteration_desc->lut1));
  rumboot_printf("lut2 = 0x%x \n",(uintptr_t)(iteration_desc->lut2));

  rumboot_printf("in_metrics = 0x%x \n",(uintptr_t)(iteration_desc->in_metrics));
  rumboot_printf("res_metrics = 0x%x \n",(uintptr_t)(iteration_desc->res_metrics));
  
  rumboot_printf("op0_cube_metrics = 0x%x \n",(uintptr_t)(iteration_desc->op0_cube_metrics));
  rumboot_printf("op0_vec_metrics = 0x%x \n",(uintptr_t)(iteration_desc->op0_vec_metrics));
  rumboot_printf("op1_cube_metrics = 0x%x \n",(uintptr_t)(iteration_desc->op1_cube_metrics));
  rumboot_printf("op1_vec_metrics = 0x%x \n",(uintptr_t)(iteration_desc->op1_vec_metrics));
  rumboot_printf("op2_cube_metrics = 0x%x \n",(uintptr_t)(iteration_desc->op2_cube_metrics));
  rumboot_printf("op2_vec_metrics = 0x%x \n",(uintptr_t)(iteration_desc->op2_vec_metrics));

  rumboot_printf("lut1_metrics = 0x%x \n",(uintptr_t)(iteration_desc->lut1_metrics));
  rumboot_printf("lut2_metrics = 0x%x \n",(uintptr_t)(iteration_desc->lut2_metrics));

  rumboot_printf("status_regs_etalon = 0x%x \n",(uintptr_t)(iteration_desc->status_regs_etalon));
  
}

void nu_vpe_iteration_start(VPEIterationDescriptor* iteration_desc) {
  
    // Chooses opx From opx_vec And opx_cube
  
  if(iteration_desc->cfg->op0_en) 
    if( (iteration_desc->cfg->op0_config.alu_en && iteration_desc->cfg->op0_config.alu_mode==Mode_Channel) ||
        (iteration_desc->cfg->op0_config.mux_en && iteration_desc->cfg->op0_config.mux_mode==Mode_Channel) )
      iteration_desc->op0 = iteration_desc->op0_vec;
    else
      iteration_desc->op0 = iteration_desc->op0_cube;
  else
    iteration_desc->op0=NULL;
  
  if(iteration_desc->cfg->op1_en) 
    if( (iteration_desc->cfg->op1_config.alu_en && iteration_desc->cfg->op1_config.alu_mode==Mode_Channel) ||
        (iteration_desc->cfg->op1_config.mux_en && iteration_desc->cfg->op1_config.mux_mode==Mode_Channel) )
      iteration_desc->op1 = iteration_desc->op1_vec;
    else
      iteration_desc->op1 = iteration_desc->op1_cube;
  else
    iteration_desc->op1=NULL;
  
  if(iteration_desc->cfg->op2_en) 
    if( (iteration_desc->cfg->op2_config.alu_en && iteration_desc->cfg->op2_config.alu_mode==Mode_Channel) ||
        (iteration_desc->cfg->op2_config.mux_en && iteration_desc->cfg->op2_config.mux_mode==Mode_Channel) )
      iteration_desc->op2 = iteration_desc->op2_vec;
    else
      iteration_desc->op2 = iteration_desc->op2_cube;
  else
    iteration_desc->op2=NULL;
}

void nu_vpe_iterate_desc(VPEIterationDescriptor* desc) {
  desc->in_data = (void*) ( (char*)(desc->in_data) + (desc->cfg->src_rdma_config.dma_bsize+1) * (desc->in_metrics->s) );
  desc->etalon  = (void*) ( (char*)(desc->etalon ) + (desc->cfg->    wdma_config.dma_bsize+1) * (desc->res_metrics->s));
  desc->res_data= (void*) ( (char*)(desc->res_data)+ (desc->cfg->    wdma_config.dma_bsize+1) * (desc->res_metrics->s));
  if    (desc->cfg->op0_en==Enable_En) {
    if( (desc->cfg->op0_config.alu_en==Enable_En   &&   desc->cfg->op0_config.alu_mode==Mode_Element) ||
        (desc->cfg->op0_config.mux_en==Enable_En   &&   desc->cfg->op0_config.mux_mode==Mode_Element) ) {
             desc->op0_cube = (void*) ( (char*)(desc->op0_cube) + 
                                                (desc->cfg->op0_rdma_config.dma_bsize+1) * (desc->op0_cube_metrics->s) );
             desc->op0_cube_metrics += (desc->cfg->op0_rdma_config.dma_bsize+1);
    }
    if( (desc->cfg->op0_config.alu_en==Enable_En   &&   desc->cfg->op0_config.alu_mode==Mode_Channel) ||
        (desc->cfg->op0_config.mux_en==Enable_En   &&   desc->cfg->op0_config.mux_mode==Mode_Channel) ) {
             desc->op0_vec  = (void*) ( (char*)(desc->op0_vec ) + 
                                                desc->op0_vec_metrics->s  );
             desc->op0_vec_metrics  += 1;
    }
  }
  if    (desc->cfg->op1_en==Enable_En) {
    if( (desc->cfg->op1_config.alu_en==Enable_En   &&   desc->cfg->op1_config.alu_mode==Mode_Element) ||
        (desc->cfg->op1_config.mux_en==Enable_En   &&   desc->cfg->op1_config.mux_mode==Mode_Element) ) {
             desc->op1_cube = (void*) ( (char*)(desc->op1_cube) + 
                                                (desc->cfg->op1_rdma_config.dma_bsize+1) * (desc->op1_cube_metrics->s) );
             desc->op1_cube_metrics += (desc->cfg->op1_rdma_config.dma_bsize+1);
    }
    if( (desc->cfg->op1_config.alu_en==Enable_En   &&   desc->cfg->op1_config.alu_mode==Mode_Channel) ||
        (desc->cfg->op1_config.mux_en==Enable_En   &&   desc->cfg->op1_config.mux_mode==Mode_Channel) ) {
             desc->op1_vec  = (void*) ( (char*)(desc->op1_vec ) + 
                                                desc->op1_vec_metrics->s  );
             desc->op1_vec_metrics  += 1;
    }
  }
  if    (desc->cfg->op2_en==Enable_En) {
    if( (desc->cfg->op2_config.alu_en==Enable_En   &&   desc->cfg->op2_config.alu_mode==Mode_Element) ||
        (desc->cfg->op2_config.mux_en==Enable_En   &&   desc->cfg->op2_config.mux_mode==Mode_Element) ) {
             desc->op2_cube = (void*) ( (char*)(desc->op2_cube) + 
                                                (desc->cfg->op2_rdma_config.dma_bsize+1) * (desc->op2_cube_metrics->s) );
             desc->op2_cube_metrics += (desc->cfg->op2_rdma_config.dma_bsize+1);
    }
    if( (desc->cfg->op2_config.alu_en==Enable_En   &&   desc->cfg->op2_config.alu_mode==Mode_Channel) ||
        (desc->cfg->op2_config.mux_en==Enable_En   &&   desc->cfg->op2_config.mux_mode==Mode_Channel) ) {
             desc->op2_vec  = (void*) ( (char*)(desc->op2_vec ) + 
                                                desc->op2_vec_metrics->s  );
             desc->op2_vec_metrics  += 1;
    }
    
    if(desc->cfg->op2_config.lut_en==Enable_En) {
      desc->lut1 = (void*) ( (char*)(desc->lut1) + desc->lut1_metrics->s );
      desc->lut2 = (void*) ( (char*)(desc->lut2) + desc->lut2_metrics->s );
      desc->lut1_metrics       += 1;
      desc->lut2_metrics       += 1;
    }
  }
  
  desc->in_metrics         += (desc->cfg->src_rdma_config.dma_bsize+1);
  desc->res_metrics        += (desc->cfg->    wdma_config.dma_bsize+1);
  desc->status_regs_etalon += (desc->cfg->    wdma_config.dma_bsize+1);
  
  desc->cfg += 1;
}

int nu_vpe_compare_luts(void* lut1_one, void* lut2_one, void* lut1_another, void* lut2_another) {
  uint16_t* one;
  uint16_t* another;
  
  one     = (uint16_t*) lut1_one;
  another = (uint16_t*) lut1_another;
  for(int i=0;i<NU_VPE_LUT1_SIZE;i++) {
    if(*one++ != *another++)
      return 1;
  }
  
  one     = (uint16_t*) lut2_one;
  another = (uint16_t*) lut2_another;
  for(int i=0;i<NU_VPE_LUT2_SIZE;i++) {
    if(*one++ != *another++)
      return 1;
  }
  
  return 0;
}

int nu_vpe_place_arrays(int heap_id, VPETestDescriptor* test_desc,int iterations) {
  test_desc->array_of_in_metrics = nu_load_array_of_cube_metrics(heap_id, "metrics_in_tag", iterations);
  test_desc->array_of_res_metrics= nu_load_array_of_cube_metrics(heap_id, "metrics_etalon_tag", iterations);
  
  
  if(test_desc->array_of_in_metrics  ==NULL ||
     test_desc->array_of_res_metrics ==NULL ) return -1;
  
  test_desc->invocations = nu_vpe_invocations_cnt(test_desc->array_of_in_metrics,iterations);
    
  test_desc->array_of_in_data = nu_load_array_of_cubes(heap_id,"in_file_tag",    test_desc->array_of_in_metrics, iterations);
  test_desc->array_of_etalon  = nu_load_array_of_cubes(heap_id,"etalon_file_tag",test_desc->array_of_res_metrics,iterations);
  test_desc->array_of_res_data= nu_malloc_array_of_res(heap_id,                  test_desc->array_of_res_metrics,iterations);
  
  
  if(test_desc->array_of_in_data ==NULL || 
     test_desc->array_of_etalon  ==NULL || 
     test_desc->array_of_res_data==NULL ) return -1;
    
  test_desc->array_of_cfg = rumboot_malloc_from_heap_aligned(heap_id,sizeof(ConfigVPE)*(test_desc->invocations),sizeof(uint32_t));
  if(test_desc->array_of_cfg==NULL)return -1;
  if(nu_vpe_load_array_of_cfg(heap_id, test_desc->array_of_cfg, test_desc->invocations) !=0) return -1;
  
  nu_vpe_batch_size_stride_cnt(test_desc->array_of_in_metrics,test_desc->array_of_cfg,iterations);  
  
  
  if(nu_vpe_load_arrays_of_op_metrics(
    heap_id,heap_id,heap_id,heap_id,
    &(test_desc->op0_array_desc),
    &(test_desc->op1_array_desc),
    &(test_desc->op2_array_desc),
    test_desc->array_of_cfg,
    test_desc->invocations) !=0) return -1;
  
  nu_vpe_batch_size_stride_of_ops_cnt(
   &test_desc->op0_array_desc,
   &test_desc->op1_array_desc,
   &test_desc->op2_array_desc,
    test_desc->array_of_cfg,
    test_desc->invocations
  );
  
  if(nu_vpe_load_arrays_of_ops(
    heap_id,heap_id,heap_id,heap_id,
    &(test_desc->op0_array_desc),
    &(test_desc->op1_array_desc),
    &(test_desc->op2_array_desc)
    ) !=0) return -1;
  
  test_desc->array_of_status_regs_etalon = nu_load_array_of_status_regs(heap_id,iterations);
  if(test_desc->array_of_status_regs_etalon==NULL)return -1;
  
  return 0;
}

int nu_vpe_invocations_cnt(CubeMetrics* array_of_in_metrics,int num_cubes){
    rumboot_printf("Invocations_cnt start\n");
    int invocations_cnt = 0;
    for (int i=0;i<num_cubes;i++) {
        if (array_of_in_metrics[i].role == CubeRole_LastInBatch){
            //rumboot_printf("This is LastInBatch!\n");
            invocations_cnt += 1;
        }
        else{
            if (array_of_in_metrics[i].role == CubeRole_NotLastInBatch){
                //rumboot_printf("This is NotLastInBatch!\n");
            }
        }
    }
    //rumboot_printf("Invocations_cnt end\n");
    return invocations_cnt;    
}

void nu_vpe_batch_size_stride_cnt(CubeMetrics* array_of_in_metrics,ConfigVPE* array_of_cfg,int num_cubes){
    rumboot_printf("batch_size_cnt start\n");
    int invocations_index = 0;
    uint32_t bstride_wdma_temp;
    uint32_t bstride_rdma_temp;
    uint8_t bsize_temp = 0;
    for (int i=0;i<num_cubes;i++) {
        
        if (array_of_in_metrics[i].role == CubeRole_LastInBatch){
            //rumboot_printf("This is LastInBatch!\n");
            array_of_cfg[invocations_index].src_rdma_config.dma_bsize = bsize_temp;
            array_of_cfg[invocations_index].wdma_config.dma_bsize = bsize_temp;
            bstride_rdma_temp = array_of_in_metrics[i].s;
            bstride_wdma_temp = array_of_in_metrics[i].H*array_of_in_metrics[i].W*array_of_in_metrics[i].C*(array_of_cfg[invocations_index].out_data_type == DataType_Int8 ? 1 : 2);
            array_of_cfg[invocations_index].src_rdma_config.dma_bstride = bstride_rdma_temp;
            array_of_cfg[invocations_index].wdma_config.dma_bstride = bstride_wdma_temp;
            //rumboot_printf("Invocation = %d, batch_size = %d, bstride_rdma = %d, bstride_wdma = %d\n",invocations_index,bsize_temp,bstride_rdma_temp,bstride_wdma_temp);
            invocations_index += 1; // increment 
            bsize_temp = 0; // reset
            
        }
        else{
            if (array_of_in_metrics[i].role == CubeRole_NotLastInBatch){
                //rumboot_printf("This is NotLastInBatch!\n");
                bsize_temp += 1; //
            }
        }
         
    }
}


void nu_vpe_batch_size_stride_of_ops_cnt(
  OpArrayDescriptor* op0_array_desc,
  OpArrayDescriptor* op1_array_desc,
  OpArrayDescriptor* op2_array_desc,
  ConfigVPE* array_of_cfg,
  int invocations
){
    CubeMetrics* op0_cube_metrics;
    CubeMetrics* op1_cube_metrics;
    CubeMetrics* op2_cube_metrics;
    
    //ConfigVPE* cfg;
    
    op0_cube_metrics = op0_array_desc->array_of_cube_metrics;
    op1_cube_metrics = op1_array_desc->array_of_cube_metrics;
    op2_cube_metrics = op2_array_desc->array_of_cube_metrics;
    
 
    for (int i=0; i<invocations;i++){
        //cfg = >array_of_cfg[i];
                
        array_of_cfg[i].op0_rdma_config.dma_bsize = 0;
        array_of_cfg[i].op0_rdma_config.dma_bstride = 0;
        array_of_cfg[i].op1_rdma_config.dma_bsize = 0;
        array_of_cfg[i].op1_rdma_config.dma_bstride = 0;
        array_of_cfg[i].op2_rdma_config.dma_bsize = 0;
        array_of_cfg[i].op2_rdma_config.dma_bstride = 0;
        
        if    (array_of_cfg[i].op0_en==Enable_En) {
            if( (array_of_cfg[i].op0_config.alu_en==Enable_En   &&   array_of_cfg[i].op0_config.alu_mode==Mode_Element) ||
                (array_of_cfg[i].op0_config.mux_en==Enable_En   &&   array_of_cfg[i].op0_config.mux_mode==Mode_Element) ) {
                    array_of_cfg[i].op0_rdma_config.dma_bsize = array_of_cfg[i].src_rdma_config.dma_bsize;
                    array_of_cfg[i].op0_rdma_config.dma_bstride = op0_cube_metrics->s;
                    op0_cube_metrics += array_of_cfg[i].op0_rdma_config.dma_bsize+1;
                }
        }
        
        if    (array_of_cfg[i].op1_en==Enable_En) {
            if( (array_of_cfg[i].op1_config.alu_en==Enable_En   &&   array_of_cfg[i].op1_config.alu_mode==Mode_Element) ||
                (array_of_cfg[i].op1_config.mux_en==Enable_En   &&   array_of_cfg[i].op1_config.mux_mode==Mode_Element) ) {
                    array_of_cfg[i].op1_rdma_config.dma_bsize = array_of_cfg[i].src_rdma_config.dma_bsize;
                    array_of_cfg[i].op1_rdma_config.dma_bstride = op1_cube_metrics->s;
                    op1_cube_metrics += array_of_cfg[i].op1_rdma_config.dma_bsize+1;
                }
        }
        
        if    (array_of_cfg[i].op2_en==Enable_En) {
            if( (array_of_cfg[i].op2_config.alu_en==Enable_En   &&   array_of_cfg[i].op2_config.alu_mode==Mode_Element) ||
                (array_of_cfg[i].op2_config.mux_en==Enable_En   &&   array_of_cfg[i].op2_config.mux_mode==Mode_Element) ) {
                    array_of_cfg[i].op2_rdma_config.dma_bsize = array_of_cfg[i].src_rdma_config.dma_bsize;
                    array_of_cfg[i].op2_rdma_config.dma_bstride = op2_cube_metrics->s;
                    op2_cube_metrics += array_of_cfg[i].op2_rdma_config.dma_bsize+1;
                }
        }
    }
}



void nu_ppe_init_test_desc(PPETestDescriptor* test_desc) {
  test_desc-> array_of_cfg=NULL;
  
  test_desc-> array_of_cfg_reg=NULL;
  
  test_desc-> array_of_in_metrics=NULL;
  test_desc-> array_of_res_metrics=NULL;
  
  test_desc-> array_of_in_data=NULL;
  test_desc-> array_of_etalon=NULL;
  test_desc-> array_of_res_data=NULL;

  test_desc-> array_of_status_regs_etalon=NULL;

  test_desc-> in_file_tag="in_file_tag"; 
}


void nu_ppe_init_iteration_desc(PPETestDescriptor* test_desc, PPEIterationDescriptor* iteration_desc) {
  iteration_desc->  cfg     = test_desc->array_of_cfg;
  iteration_desc->  cfg_reg = test_desc->array_of_cfg_reg;
  
  iteration_desc->  in_metrics  = test_desc->array_of_in_metrics;
  iteration_desc->  res_metrics = test_desc->array_of_res_metrics;

  iteration_desc-> in_data = test_desc->array_of_in_data;
  iteration_desc-> etalon  = test_desc->array_of_etalon;
  iteration_desc-> res_data= test_desc->array_of_res_data;

  iteration_desc->status_regs_etalon = test_desc->array_of_status_regs_etalon;  
}

void nu_ppe_iterate_desc(PPEIterationDescriptor* desc) {
  desc->in_data = (void*) ( (char*)(desc->in_data) + desc->in_metrics->s );
  desc->etalon  = (void*) ( (char*)(desc->etalon ) + desc->res_metrics->s);
  desc->res_data= (void*) ( (char*)(desc->res_data)+ desc->res_metrics->s);
  
  desc->in_metrics         += 1;
  desc->res_metrics        += 1;
  
  desc->cfg += 1;
  desc->cfg_reg += 1;

  desc->status_regs_etalon += 1;
}

int nu_ppe_place_arrays(int heap_id, PPETestDescriptor* test_desc,int iterations){
  test_desc->array_of_cfg     = rumboot_malloc_from_heap_aligned(heap_id,sizeof(ConfigPPE)   *iterations,sizeof(uint32_t));
  test_desc->array_of_cfg_reg = rumboot_malloc_from_heap_aligned(heap_id,sizeof(ConfigREGPPE)*iterations,sizeof(uint32_t));
  if(test_desc->array_of_cfg==NULL || test_desc->array_of_cfg_reg==NULL)return -1;
  if(nu_ppe_load_array_of_cfg(heap_id, test_desc->array_of_cfg, iterations) !=0) return -1;

  test_desc->array_of_in_metrics = nu_load_array_of_cube_metrics(heap_id,"metrics_in_tag", iterations);

  test_desc->array_of_res_metrics= nu_load_array_of_cube_metrics(heap_id,"metrics_etalon_tag", iterations);
  
  if(test_desc->array_of_in_metrics  ==NULL ||
     test_desc->array_of_res_metrics ==NULL ) return -1;
  
  test_desc->array_of_in_data = nu_load_array_of_cubes(heap_id,test_desc->in_file_tag,test_desc->array_of_in_metrics ,iterations);

  test_desc->array_of_etalon  = nu_load_array_of_cubes(heap_id,"etalon_file_tag",test_desc->array_of_res_metrics,iterations);

  test_desc->array_of_res_data= nu_malloc_array_of_res(heap_id,test_desc->array_of_res_metrics,iterations);

  test_desc->array_of_status_regs_etalon = nu_load_array_of_status_regs(heap_id,iterations);

  if(test_desc->array_of_in_data ==NULL || 
     test_desc->array_of_etalon  ==NULL || 
     test_desc->array_of_res_data==NULL ||
     test_desc->array_of_status_regs_etalon==NULL
  ) return -1;
  
  return 0;
}


void nu_npe_init_test_desc(NPETestDescriptor* test_desc) {
  test_desc-> MPE_ENABLED=Enable_En;
  test_desc-> PPE_ENABLED=Enable_En;
  
  test_desc-> array_of_cfg_mpe=NULL;
  test_desc-> array_of_cfg_vpe=NULL;
  test_desc-> array_of_cfg_ppe=NULL;
  
  test_desc-> array_of_cfg_reg_ppe=NULL;
  
  test_desc-> array_of_warr_metrics=NULL;
  test_desc-> array_of_in_metrics=NULL;
  test_desc-> array_of_res_metrics=NULL;
  
  test_desc-> array_of_in_data=NULL;
  test_desc-> array_of_warr=NULL;
  test_desc-> array_of_etalon=NULL;
  test_desc-> array_of_res_data=NULL;
  
  nu_vpe_init_op_array_desc(&(test_desc->op0_array_desc));
  nu_vpe_init_op_array_desc(&(test_desc->op1_array_desc));
  nu_vpe_init_op_array_desc(&(test_desc->op2_array_desc));
  
  test_desc->mpe_cfg_lut=NULL;

  //~ test_desc-> array_of_status_regs_etalon=NULL;
  
  test_desc->associative_regs_dump_start_ptr=NULL;
  test_desc->associative_regs_dump_end_ptr=NULL;

  test_desc->array_of_depend_table=NULL;
}


void nu_npe_init_iteration_desc(NPETestDescriptor* test_desc, NPEIterationDescriptor* iteration_desc) {
  iteration_desc->MPE_ENABLED=test_desc->MPE_ENABLED;
  iteration_desc->PPE_ENABLED=test_desc->PPE_ENABLED;
  
  iteration_desc->cfg_mpe  = test_desc->array_of_cfg_mpe;
  iteration_desc->cfg_vpe  = test_desc->array_of_cfg_vpe;
  iteration_desc->cfg_ppe  = test_desc->array_of_cfg_ppe;
  
  iteration_desc->cfg_reg_ppe = test_desc->array_of_cfg_reg_ppe;
  
  iteration_desc->warr     = test_desc->array_of_warr;
  iteration_desc->in_data  = test_desc->array_of_in_data;
  iteration_desc->etalon   = test_desc->array_of_etalon;
  iteration_desc->res_data = test_desc->array_of_res_data;
  
  iteration_desc->op0_cube = test_desc->op0_array_desc.array_of_cubes;
  iteration_desc->op1_cube = test_desc->op1_array_desc.array_of_cubes;
  iteration_desc->op2_cube = test_desc->op2_array_desc.array_of_cubes;
  iteration_desc->op0_vec  = test_desc->op0_array_desc.array_of_vecs;
  iteration_desc->op1_vec  = test_desc->op1_array_desc.array_of_vecs;
  iteration_desc->op2_vec  = test_desc->op2_array_desc.array_of_vecs;
  iteration_desc->lut1     = test_desc->op2_array_desc.array_of_lut1;
  iteration_desc->lut2     = test_desc->op2_array_desc.array_of_lut2;
  iteration_desc->op0_cube_metrics = test_desc->op0_array_desc.array_of_cube_metrics;
  iteration_desc->op0_vec_metrics  = test_desc->op0_array_desc.array_of_vec_metrics;
  iteration_desc->op1_cube_metrics = test_desc->op1_array_desc.array_of_cube_metrics;
  iteration_desc->op1_vec_metrics  = test_desc->op1_array_desc.array_of_vec_metrics;
  iteration_desc->op2_cube_metrics = test_desc->op2_array_desc.array_of_cube_metrics;
  iteration_desc->op2_vec_metrics  = test_desc->op2_array_desc.array_of_vec_metrics;
  iteration_desc->lut1_metrics     = test_desc->op2_array_desc.array_of_lut1_metrics;
  iteration_desc->lut2_metrics     = test_desc->op2_array_desc.array_of_lut2_metrics;
  
  //~ iteration_desc->status_regs_etalon = test_desc->array_of_status_regs_etalon;
  
  iteration_desc->warr_metrics = test_desc->array_of_warr_metrics;
  iteration_desc->in_metrics   = test_desc->array_of_in_metrics;
  iteration_desc->res_metrics  = test_desc->array_of_res_metrics;
  
  iteration_desc->curr_regs_dump = NULL;
  iteration_desc->next_regs_dump = NULL;
}

void nu_npe_iteration_start(NPEIterationDescriptor* iteration_desc){ // :( Dirty Copypaste
  
    // Chooses opx From opx_vec And opx_cube
  
  if(iteration_desc->cfg_vpe->op0_en) 
    if( (iteration_desc->cfg_vpe->op0_config.alu_en && iteration_desc->cfg_vpe->op0_config.alu_mode==Mode_Channel) ||
        (iteration_desc->cfg_vpe->op0_config.mux_en && iteration_desc->cfg_vpe->op0_config.mux_mode==Mode_Channel) )
      iteration_desc->op0 = iteration_desc->op0_vec;
    else
      iteration_desc->op0 = iteration_desc->op0_cube;
  else
    iteration_desc->op0=NULL;
  
  if(iteration_desc->cfg_vpe->op1_en) 
    if( (iteration_desc->cfg_vpe->op1_config.alu_en && iteration_desc->cfg_vpe->op1_config.alu_mode==Mode_Channel) ||
        (iteration_desc->cfg_vpe->op1_config.mux_en && iteration_desc->cfg_vpe->op1_config.mux_mode==Mode_Channel) )
      iteration_desc->op1 = iteration_desc->op1_vec;
    else
      iteration_desc->op1 = iteration_desc->op1_cube;
  else
    iteration_desc->op1=NULL;
  
  if(iteration_desc->cfg_vpe->op2_en) 
    if( (iteration_desc->cfg_vpe->op2_config.alu_en && iteration_desc->cfg_vpe->op2_config.alu_mode==Mode_Channel) ||
        (iteration_desc->cfg_vpe->op2_config.mux_en && iteration_desc->cfg_vpe->op2_config.mux_mode==Mode_Channel) )
      iteration_desc->op2 = iteration_desc->op2_vec;
    else
      iteration_desc->op2 = iteration_desc->op2_cube;
  else
    iteration_desc->op2=NULL;
}

void nu_npe_iterate_desc(NPEIterationDescriptor* desc) {
  desc->in_data = (void*) ( (char*)(desc->in_data) + (desc->cfg_vpe->src_rdma_config.dma_bsize+1) * (desc->in_metrics->s) );
  if(desc->MPE_ENABLED==Enable_En) {
    desc->warr    = (void*) ( (char*)(desc->warr)    + desc->warr_metrics->s); // ???: with batch * bsize ?
  }
  desc->etalon  = (void*) ( (char*)(desc->etalon ) + (desc->cfg_vpe->    wdma_config.dma_bsize+1) * (desc->res_metrics->s));
  desc->res_data= (void*) ( (char*)(desc->res_data)+ (desc->cfg_vpe->    wdma_config.dma_bsize+1) * (desc->res_metrics->s));
  if    (desc->cfg_vpe->op0_en==Enable_En) {
    if( (desc->cfg_vpe->op0_config.alu_en==Enable_En   &&   desc->cfg_vpe->op0_config.alu_mode==Mode_Element) ||
        (desc->cfg_vpe->op0_config.mux_en==Enable_En   &&   desc->cfg_vpe->op0_config.mux_mode==Mode_Element) ) {
             desc->op0_cube = (void*) ( (char*)(desc->op0_cube) + 
                                                desc->op0_cube_metrics->s );
             desc->op0_cube_metrics += (desc->cfg_vpe->op0_rdma_config.dma_bsize+1);
    }
    if( (desc->cfg_vpe->op0_config.alu_en==Enable_En   &&   desc->cfg_vpe->op0_config.alu_mode==Mode_Channel) ||
        (desc->cfg_vpe->op0_config.mux_en==Enable_En   &&   desc->cfg_vpe->op0_config.mux_mode==Mode_Channel) ) {
             desc->op0_vec  = (void*) ( (char*)(desc->op0_vec ) + 
                                                desc->op0_vec_metrics->s  );
             desc->op0_vec_metrics  += 1;
    }
  }
  if    (desc->cfg_vpe->op1_en==Enable_En) {
    if( (desc->cfg_vpe->op1_config.alu_en==Enable_En   &&   desc->cfg_vpe->op1_config.alu_mode==Mode_Element) ||
        (desc->cfg_vpe->op1_config.mux_en==Enable_En   &&   desc->cfg_vpe->op1_config.mux_mode==Mode_Element) ) {
             desc->op1_cube = (void*) ( (char*)(desc->op1_cube) + 
                                                (desc->cfg_vpe->op1_rdma_config.dma_bsize+1) * (desc->op1_cube_metrics->s) );
             desc->op1_cube_metrics += (desc->cfg_vpe->op1_rdma_config.dma_bsize+1);
    }
    if( (desc->cfg_vpe->op1_config.alu_en==Enable_En   &&   desc->cfg_vpe->op1_config.alu_mode==Mode_Channel) ||
        (desc->cfg_vpe->op1_config.mux_en==Enable_En   &&   desc->cfg_vpe->op1_config.mux_mode==Mode_Channel) ) {
             desc->op1_vec  = (void*) ( (char*)(desc->op1_vec ) + 
                                                desc->op1_vec_metrics->s  );
             desc->op1_vec_metrics  += 1;
    }
  }
  if    (desc->cfg_vpe->op2_en==Enable_En) {
    if( (desc->cfg_vpe->op2_config.alu_en==Enable_En   &&   desc->cfg_vpe->op2_config.alu_mode==Mode_Element) ||
        (desc->cfg_vpe->op2_config.mux_en==Enable_En   &&   desc->cfg_vpe->op2_config.mux_mode==Mode_Element) ) {
             desc->op2_cube = (void*) ( (char*)(desc->op2_cube) + 
                                                (desc->cfg_vpe->op2_rdma_config.dma_bsize+1) * (desc->op2_cube_metrics->s) );
             desc->op2_cube_metrics += (desc->cfg_vpe->op2_rdma_config.dma_bsize+1);
    }
    if( (desc->cfg_vpe->op2_config.alu_en==Enable_En   &&   desc->cfg_vpe->op2_config.alu_mode==Mode_Channel) ||
        (desc->cfg_vpe->op2_config.mux_en==Enable_En   &&   desc->cfg_vpe->op2_config.mux_mode==Mode_Channel) ) {
             desc->op2_vec  = (void*) ( (char*)(desc->op2_vec ) + 
                                                desc->op2_vec_metrics->s  );
             desc->op2_vec_metrics  += 1;
    }
    
    if(desc->cfg_vpe->op2_config.lut_en==Enable_En) {
      desc->lut1 = (void*) ( (char*)(desc->lut1) + desc->lut1_metrics->s );
      desc->lut2 = (void*) ( (char*)(desc->lut2) + desc->lut2_metrics->s );
      desc->lut1_metrics       += 1;
      desc->lut2_metrics       += 1;
    }
  }
  
  if(desc->MPE_ENABLED) {
    desc->warr_metrics       += 1;
  }
  desc->in_metrics         += (desc->cfg_vpe->src_rdma_config.dma_bsize+1);
  desc->res_metrics        += (desc->cfg_vpe->    wdma_config.dma_bsize+1);
  //~ desc->status_regs_etalon += 1;
  
  if(desc->MPE_ENABLED) {
    desc->cfg_mpe += 1;
  }
  desc->cfg_vpe += 1;
  
  if(desc->PPE_ENABLED==Enable_En) {
    desc->cfg_ppe += 1;
    desc->cfg_reg_ppe += 1;
  }

  nu_npe_reg_map_swap(desc);
}

void nu_npe_reg_map_swap(NPEIterationDescriptor* desc) {
    NARegDump* swap_ptr;

    swap_ptr = desc->curr_regs_dump;
    desc->curr_regs_dump = desc->next_regs_dump;
    desc->next_regs_dump = swap_ptr;
}

int nu_npe_place_regs_dump(int heap_id, NPEIterationDescriptor* desc) {
  desc->curr_regs_dump = rumboot_malloc_from_heap_aligned(heap_id, sizeof(NARegDump), sizeof(uint32_t));
  desc->next_regs_dump = rumboot_malloc_from_heap_aligned(heap_id, sizeof(NARegDump), sizeof(uint32_t));
  if(
    desc->curr_regs_dump == NULL ||
    desc->next_regs_dump == NULL
  ) return -1;
  return 0;
}

int nu_npe_place_arrays_by_heap_map(NAHeapMap* heap_map, NPETestDescriptor* test_desc,int iterations) {
  test_desc->array_of_in_metrics = nu_load_array_of_cube_metrics(heap_map->metrics_in_data, "metrics_in_tag", iterations);
  test_desc->array_of_res_metrics= nu_load_array_of_cube_metrics(heap_map->metrics_etalon, "metrics_etalon_tag", iterations);
  
  if(test_desc->array_of_in_metrics  ==NULL || test_desc->array_of_res_metrics ==NULL  ) return -1;
    
  test_desc->invocations = nu_vpe_invocations_cnt(test_desc->array_of_res_metrics,iterations);
  
  if(test_desc->MPE_ENABLED==Enable_En) {
      test_desc->array_of_warr_metrics=nu_load_array_of_warr_metrics(heap_map->metrics_warr, "metrics_warr_tag", test_desc->invocations);
  }
  
  if((test_desc->array_of_warr_metrics==NULL && test_desc->MPE_ENABLED==Enable_En) ) return -1;
  
  if(test_desc->MPE_ENABLED==Enable_En) {
    test_desc->array_of_warr    = nu_load_array_of_warr (heap_map->warr,       "warr_file_tag",test_desc->array_of_warr_metrics,test_desc->invocations);
  }
  test_desc->array_of_in_data = nu_load_array_of_cubes(heap_map->in_data,         "in_file_tag",test_desc->array_of_in_metrics ,iterations);
  test_desc->array_of_etalon  = nu_load_array_of_cubes(heap_map->etalon,      "etalon_file_tag",test_desc->array_of_res_metrics,iterations);
  test_desc->array_of_res_data= nu_malloc_array_of_res(heap_map->res,                           test_desc->array_of_res_metrics,iterations);
    
  if(test_desc->array_of_in_data ==NULL || 
    (test_desc->array_of_warr    ==NULL && test_desc->MPE_ENABLED==Enable_En) ||
     test_desc->array_of_etalon  ==NULL || 
     test_desc->array_of_res_data==NULL ) return -1;
  
  
  if(test_desc->MPE_ENABLED==Enable_En) {
    test_desc->array_of_cfg_mpe = rumboot_malloc_from_heap_aligned(heap_map->mpe_cfg,sizeof(ConfigMPE)*(test_desc->invocations),sizeof(uint32_t));
  }
  test_desc->array_of_cfg_vpe = rumboot_malloc_from_heap_aligned(heap_map->vpe_cfg,sizeof(ConfigVPE)*(test_desc->invocations),sizeof(uint32_t));
  if(test_desc->PPE_ENABLED==Enable_En) {
    test_desc->array_of_cfg_ppe = rumboot_malloc_from_heap_aligned(heap_map->ppe_cfg,sizeof(ConfigPPE)*(test_desc->invocations),sizeof(uint32_t));
    test_desc->array_of_cfg_reg_ppe = rumboot_malloc_from_heap_aligned(heap_map->ppe_cfg,sizeof(ConfigREGPPE)*(test_desc->invocations),sizeof(uint32_t));
  }

  if((test_desc->array_of_cfg_mpe==NULL && test_desc->MPE_ENABLED==Enable_En) ||
     test_desc->array_of_cfg_vpe==NULL ||
   ((test_desc->array_of_cfg_ppe==NULL || test_desc->array_of_cfg_reg_ppe==NULL) && test_desc->PPE_ENABLED==Enable_En) 
    ) return -1;
  
  if(test_desc->MPE_ENABLED==Enable_En) {
    if(nu_mpe_load_array_of_cfg(heap_map->mpe_cfg,test_desc->array_of_cfg_mpe,test_desc->invocations) !=0) return -1;
  }
  if(nu_vpe_load_array_of_cfg(heap_map->vpe_cfg,test_desc->array_of_cfg_vpe,test_desc->invocations) !=0) return -1;
  if(test_desc->PPE_ENABLED==Enable_En)
    if(nu_ppe_load_array_of_cfg(heap_map->ppe_cfg,test_desc->array_of_cfg_ppe,test_desc->invocations) !=0) return -1;
  
  nu_vpe_batch_size_stride_cnt(test_desc->array_of_res_metrics,test_desc->array_of_cfg_vpe,iterations);
  
  if(nu_vpe_load_arrays_of_op_metrics(
    heap_map->metrics_op0,heap_map->metrics_op1,heap_map->metrics_op2,heap_map->metrics_lut,
    &(test_desc->op0_array_desc),
    &(test_desc->op1_array_desc),
    &(test_desc->op2_array_desc),
    test_desc->array_of_cfg_vpe,
    test_desc->invocations) !=0) return -1;
    
  nu_vpe_batch_size_stride_of_ops_cnt(
   &test_desc->op0_array_desc,
   &test_desc->op1_array_desc,
   &test_desc->op2_array_desc,
    test_desc->array_of_cfg_vpe,
    test_desc->invocations
  );
  
  if(nu_vpe_load_arrays_of_ops(
    heap_map->op0,heap_map->op1,heap_map->op2,heap_map->lut,
    &(test_desc->op0_array_desc),
    &(test_desc->op1_array_desc),
    &(test_desc->op2_array_desc)
    ) !=0) return -1;
  
  if(test_desc->MPE_ENABLED==Enable_En) {
    test_desc->mpe_cfg_lut = nu_mpe_load_cfg_lut(heap_map->mpe_cfg_lut);
    if(test_desc->mpe_cfg_lut==NULL)
      return -1;
  }
  
  return 0;

}
int nu_npe_place_arrays(int heap_id, NPETestDescriptor* test_desc,int iterations) {
  NAHeapMap heap_map;
  
  heap_map.in_data=heap_id;
  heap_map.warr=heap_id;
  heap_map.op0=heap_id;
  heap_map.op1=heap_id;
  heap_map.op2=heap_id;
  heap_map.lut=heap_id;
  heap_map.mpe_cfg=heap_id;
  heap_map.mpe_cfg_lut=heap_id;
  heap_map.vpe_cfg=heap_id;
  heap_map.ppe_cfg=heap_id;
  heap_map.res=heap_id;
  heap_map.etalon=heap_id;
  heap_map.metrics_in_data=heap_id;
  heap_map.metrics_warr=heap_id;
  heap_map.metrics_op0=heap_id;
  heap_map.metrics_op1=heap_id;
  heap_map.metrics_op2=heap_id;
  heap_map.metrics_lut=heap_id;
  heap_map.metrics_mpe_cfg=heap_id;
  heap_map.metrics_mpe_cfg_lut=heap_id;
  heap_map.metrics_vpe_cfg=heap_id;
  heap_map.metrics_ppe_cfg=heap_id;
  // heap_map.metrics_res=heap_id;
  heap_map.metrics_etalon=heap_id;
  
  return nu_npe_place_arrays_by_heap_map(&heap_map,test_desc,iterations);
}
  

void nu_vpe_pause_next_cntx_fail_stop(uintptr_t vpe_base, ConfigVPE* cfg){
    rumboot_printf("Prepare for Stop VPE begin...\n");
	while( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) == 0x00000041) !=1) {}
	rumboot_printf("Soft reset VPE ...\n");	
	iowrite32( (1<<0),vpe_base + NU_VPE + NU_VPE_SOFT_RESET);
	rumboot_printf("Soft reset done ...\n");
	while(( (ioread32(vpe_base + NU_VPE + NU_VPE_INT_STATUS) >> 17) & 1) !=1) {}
	rumboot_printf("VPE fail soft reset passed\n");	
	 iowrite32(((1<<17) | (1<<0)|(1<<6)) ,vpe_base + NU_VPE + NU_VPE_INT_RESET); 
	}
void nu_na_mpe_pause_fail_stop(uintptr_t npe_base){
    //rumboot_printf("Prepare for Stop NA_MPE begin...\n");
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 0) & 1) !=1) {}
	iowrite32((1<<0) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	
	//rumboot_printf("Soft reset NA_MPE ...\n");	
	iowrite32( (1<<0),npe_base + NA_CU_REGS_BASE + NA_MPE_SOFT_RESET);
	//rumboot_printf("NA_MPE soft reset done ...\n");
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 5) & 1) !=1) {}
	iowrite32((1<<5) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 
	rumboot_printf("NA_MPE fail soft reset passed\n");
	

	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 6) & 1) !=1) {}
	iowrite32((1<<6) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);

	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 8) & 1) !=1) {}
	iowrite32((1<<8) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 9) & 1) !=1) {}
	iowrite32((1<<9) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 10) & 1) !=1) {}
	iowrite32((1<<10) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_MPE fail soft reset\n");
	}		
void nu_na_ppe_pause_fail_stop(uintptr_t npe_base){
	//rumboot_printf("Prepare for Stop NA_PPE begin...\n");
	while( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) == 0xC0200000) !=1) {}
	//rumboot_printf("Soft reset NA_PPE ...\n");	
	iowrite32( (1<<0),npe_base + NA_CU_REGS_BASE + NA_PPE_SOFT_RESET);
	//rumboot_printf("Soft reset done ...\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 26) & 1) !=1) {}
	//rumboot_printf("NA_PPE fail soft reset passed\n");	
	iowrite32(((1<<31) | (1<<21)|(1<<26)) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Soft reset done ...\n");
	}		
void nu_na_vpe_fail_mpe_srst_stop(uintptr_t npe_base){
    rumboot_printf("Prepare for Stop NA_VPE begin...\n");
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 19) & 1) !=1) {}
	iowrite32((1<<19) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 17) & 1) !=1) {}
	iowrite32((1<<17) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 11) & 1) !=1) {}
	iowrite32((1<<11) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);	
	}

void nu_na_vpe_pause_fail_stop(uintptr_t npe_base){
    rumboot_printf("Prepare for Stop NA_VPE begin...\n");
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 11) & 1) !=1) {}
	iowrite32((1<<11) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	
	rumboot_printf("Soft reset NA_VPE ...\n");	
	iowrite32( (1<<0),npe_base + NA_CU_REGS_BASE + NA_VPE_SOFT_RESET);
	rumboot_printf("NA_VPE soft reset done ...\n");
	
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 16) & 1) !=1) {}
	iowrite32((1<<16) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET); 
	rumboot_printf("NA_MPE fail soft reset passed\n");
	while(( (ioread32(npe_base + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) >> 19) & 1) !=1) {}
	iowrite32((1<<19) ,npe_base +  NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	rumboot_printf("Done NA_VPE fail soft reset\n");
	}				
	
int nu_vpe_regs_check(uintptr_t base, int num, int iteration) {
	 int res;
	res = ioread32(base + 4*num);
	for( int i =num; i< iteration;i++) {	
	if ((i != 38) & (i != 4) & (i != 7) )
		{res = ioread32(base + 4*i);}
		if(((res != 0x00000000) & (i !=12) & (i !=18)& (i !=24) & (i !=25) & (i !=31)& (i != 17) & (i != 23) &  (i != 36) & ( i != 54))|
		((res != 0x00000053) &( (i==12)| (i ==18)| (i ==24))) |
		((res != 0x00000002) &( (i==25)| (i ==31))) | 
		(((res >> 16)  != 0x00000007) & ( (i ==17) | (i ==23) | (i ==36))) | 
		((((res >> 19)  != 0x00000000) & (i == 54)))  ) {
		rumboot_printf("res_invalid =%x\n",res);
		rumboot_printf("addr =%x\n",(base + 4*i));
		return  -1;
		}
  }
return 0;  
}
int nu_regs_check(uintptr_t base, int num, int iteration) {
	  int res;
	res = ioread32(base);	  
	for( int i =0; i< iteration;i++) {	
		if ((i != 2)) //& (i != 1)
		{res = ioread32(base + 4*i);}
		if(((res != 0x000000000) & (i !=3) & (i !=20)) | ((res != 0x000020000) & (i ==3)) |
		((res != 0x000000002) & (i ==20) & (num==1)) | ((res != 0x000000000) & (i ==20) & (num==0)) ){
		rumboot_printf("res_invalid =%x\n",res);
		rumboot_printf("addr =%x\n",(base + 4*i));
		rumboot_printf("i =%x\n",i);
		return  -1;
    }
  }
  return 0;
}
int nu_ppe_regs_check(uintptr_t base, int num, int iteration) {
	 int res_ppe;
	res_ppe = ioread32(base + 4*num);
	for( int i =num; i< iteration;i++) {	
	if ((i != 38) & (i != 9) & (i != 10)& (i != 11) )
		{res_ppe = ioread32(base + 4*i);}
		if(((res_ppe != 0x00000000) & (i !=3) & (i !=20))|
		((res_ppe != 0x00000002) & (i==20) ) 	
		  ) {
		rumboot_printf("res_ppe_invalid =%x\n",res_ppe);
		rumboot_printf("addr =%x\n",(base + 4*i));
		rumboot_printf("i =%x\n",i);
		return  -1;
		}
  }
return 0;  
}

int nu_ppe_rdma_regs_swrst_check (uintptr_t base) {
  int i, res;

  int last_reg_addr = NU_PPE_RDMA_BOX_OFFSET_Z;

  uint32_t dflt_rdma_axi_param= 0x2 << 16;
  uint32_t dflt_rdma_xyz_drct = 0x2;

  for (i=0, res=0; i<last_reg_addr+4 && !res; i+=4) {

    res = ioread32(base+i);

    if      (i == NU_PPE_RDMA_AXI_PARAM ) res = !(res == dflt_rdma_axi_param);
    else if (i == NU_PPE_RDMA_XYZ_DRCT  ) res = !(res == dflt_rdma_xyz_drct );
    else                                  res = !(res == 0x0);

    if (res) rumboot_printf("ERROR: reg_addr 0x%x reg_val 0x%x\n", i, ioread32(base+i));
  }

  return res;
}

int nu_ppe_regs_swrst_check (uintptr_t base) {
  int i, res;

  int last_reg_addr = NU_PPE_STATUS_DONE;

  uint32_t dflt_wdma_axi_param= 0x2 << 16;
  uint32_t dflt_wdma_xyz_drct = 0x2;

  for (i=0, res=0; i<last_reg_addr+4 && !res; i+=4) {

    res = ioread32(base+i);

    if      (i == NU_PPE_WDMA_AXI_PARAM ) res = !(res == dflt_wdma_axi_param);
    else if (i == NU_PPE_WDMA_XYZ_DRCT  ) res = !(res == dflt_wdma_xyz_drct );
    else                                  res = !(res == 0x0);

    if (res) rumboot_printf("ERROR: reg_addr 0x%x reg_val 0x%x\n", i, ioread32(base+i));
  }

  return res;
}

int nu_ppe_status_regs_check (uintptr_t base, StatusRegs* regs) {
  int inf_in  ;
  int nan_in  ;
  int nan_out ;

  inf_in  = !(ioread32(base+NU_PPE_INF_NUM_IN ) == regs->ppe_inf_num_in );
  nan_in  = !(ioread32(base+NU_PPE_NAN_NUM_IN ) == regs->ppe_nan_num_in );
  nan_out = !(ioread32(base+NU_PPE_NAN_NUM_OUT) == regs->ppe_nan_num_out);

  if (inf_in  ) rumboot_printf ("ERROR: NU_PPE_INF_NUM_IN: 0x%x inf_num_in: 0x%x\n", ioread32(base+NU_PPE_INF_NUM_IN ), regs->inf_num_in );
  if (nan_in  ) rumboot_printf ("ERROR: NU_PPE_NAN_NUM_IN: 0x%x nan_num_in: 0x%x\n", ioread32(base+NU_PPE_NAN_NUM_IN ), regs->nan_num_in );
  if (nan_out ) rumboot_printf ("ERROR: NU_PPE_NAN_NUM_OUT: 0x%x nan_num_out: 0x%x\n", ioread32(base+NU_PPE_NAN_NUM_OUT), regs->nan_num_out);

  return inf_in | nan_in | nan_out;
}

  int nu_mpe_regs_check(uintptr_t base, int num, int iteration) {
	 int res_mpe;
	res_mpe = ioread32(base + 4*num); 
	
	for( int i = num; i< iteration;i++) {	
	if ( (i !=4) & (i !=10) & (i !=11) & (i !=12)&(i!=13)& (i!=14)& (i !=15) & (i!=16) & (i !=17) 
		 &(i!=18)& (i !=19) &(i!=20)& (i !=21)& (i !=22)& (i !=23)&(i !=24)& (i !=25)&(i!=26)& (i !=30)& (i !=31)	

		) 
		{res_mpe = ioread32(base + 4*i);}
		if(((res_mpe != 0x00000000) & (i !=4) & (i !=5)& (i !=6) & (i !=7) & (i !=10)& (i !=14)
			& (i !=11) & (i !=13)& (i !=15) & (i !=17) & (i !=19) & (i !=21)& (i !=22)& (i !=23)& (i !=24)& (i !=25)
			& (i !=26)& (i !=27) &(i !=28)& (i !=29) & (i !=32)
			& (i !=29) ) |      		
			((res_mpe != 0x00000001) & (i==7))  |
			((res_mpe != 0x00070100) & (i==5) ) |
			((res_mpe != 0x00000100) & (i==6)) 		
		  ) {
		rumboot_printf("res_mpe_invalid =%x\n",ioread32(base + 4*i));
		rumboot_printf("addr =%x\n",(base + 4*i));
		rumboot_printf("i =%d\n",i);
		return  -1;
		}
  }
return 0;
}


int nu_npe_place_associative_regs_dump(int heap_id, NPETestDescriptor* test_desc,int iterations) {
  test_desc->associative_regs_dump_start_ptr = rumboot_malloc_from_heap_aligned(heap_id, sizeof(NARegDump)*iterations*2, sizeof(NPEReg));
  if (test_desc->associative_regs_dump_start_ptr == NULL) return -1;
  return 0;
}

int nu_npe_place_array_of_depend_table(int heap_id, NPETestDescriptor* test_desc,int iterations) {
  test_desc->array_of_depend_table = rumboot_malloc_from_heap_aligned(heap_id, sizeof(NADependTable)*iterations, sizeof(uint32_t));
  if (test_desc->associative_regs_dump_start_ptr == NULL) return -1;
  return 0;
}

NADependTable* nu_load_array_of_depend_table(int heap_id, char* file_tag, int iterations) {
  void* array_of_depend_table;
  int size = sizeof(NADependTable)*iterations;
  array_of_depend_table = rumboot_malloc_from_heap_aligned(heap_id, size, sizeof(NADependTable));
  if(array_of_depend_table==NULL)
    return NULL;

  rumboot_platform_request_file_ex(file_tag,(uintptr_t)array_of_depend_table,size);
  return array_of_depend_table;
}

void nu_print_array_of_depend_table(NPETestDescriptor* test_desc, int iterations) {
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 7; k++) {
                rumboot_printf("rd num %d\n", test_desc->array_of_depend_table[i].read_after_write[j][k]);
                rumboot_printf("wr num %d\n", test_desc->array_of_depend_table[i].write_after_read[j][k]);
            }
        }
    }
}

void nu_npe_add_depend_rd_after_wr(
  NPETestDescriptor* desc,
  int curr_i
) {

  int write_channel;
  int read_channel;
  int depend_i;
  uint32_t address;

  for (write_channel = VPE_WDMA; write_channel <= PPE_WDMA; write_channel++) {  // VPE_WDMA, PPE_WDMA
    for (read_channel = MPE_RDMA_D; read_channel <= PPE_RDMA; read_channel++) { // MPE_RDMA_D, MPE_RDMA_W, VPE_RDMA, VPE_OP0_RDMA, VPE_OP1_RDMA, VPE_OP2_RDMA, PPE_RDMA
      depend_i = desc->array_of_depend_table[curr_i].read_after_write[write_channel][read_channel];
      if (depend_i >= 0) {
        switch(write_channel) {
          case VPE_WDMA: {
            address = desc->array_of_cfg_vpe[depend_i].wdma_config.dma_baddr;
            rumboot_printf("VPE_WDMA memory address = %x\n", address);
            nu_npe_add_depend_rd_channel_cfg(desc, curr_i, read_channel, address, NU_DEPEND_MASK_VPE_WDMA);
            break;
          }
          case PPE_WDMA: {
            if(desc->PPE_ENABLED==Enable_En) {
              address = desc->array_of_cfg_reg_ppe[depend_i].wBALo;
              rumboot_printf("PPE_WDMA memory address = %x\n", address);
              nu_npe_add_depend_rd_channel_cfg(desc, curr_i, read_channel, address, NU_DEPEND_MASK_PPE_WDMA);
            }
            break;
          }
          default: break;
        }
      }
    }
  }
}


void nu_npe_add_depend_wr_after_rd(
  NPETestDescriptor* desc,
  int curr_i
) {

  int write_channel;
  int read_channel;
  int depend_i;
  uint32_t address;

  for (read_channel = MPE_RDMA_D; read_channel <= PPE_RDMA; read_channel++) { // MPE_RDMA_D, MPE_RDMA_W, VPE_RDMA, VPE_OP0_RDMA, VPE_OP1_RDMA, VPE_OP2_RDMA, PPE_RDMA
    for (write_channel = VPE_WDMA; write_channel <= PPE_WDMA; write_channel++) {  // VPE_WDMA, PPE_WDMA
      depend_i = desc->array_of_depend_table[curr_i].write_after_read[write_channel][read_channel];
      if (depend_i >= 0) {
        switch (read_channel) {
          case MPE_RDMA_D: {
            address = desc->array_of_cfg_mpe[depend_i].dma_d_config.rdma.BFCA;
            rumboot_printf("MPE_RDMA_D memory address = %x\n", address);
            nu_npe_write_channel_depend_cfg(desc, curr_i, write_channel, address, NU_DEPEND_MASK_MPE_RDMA_D);
            break;
          }
          case MPE_RDMA_W: {
            address = desc->array_of_cfg_mpe[depend_i].dma_w_config.rdma.BFCA;
            rumboot_printf("MPE_RDMA_W memory address = %x\n", address);
            nu_npe_write_channel_depend_cfg(desc, curr_i, write_channel, address, NU_DEPEND_MASK_MPE_RDMA_W);
            break;
          }
          case VPE_RDMA: {
            address = desc->array_of_cfg_vpe[depend_i].src_rdma_config.dma_baddr;
            rumboot_printf("VPE_RDMA memory address = %x\n", address);
            nu_npe_write_channel_depend_cfg(desc, curr_i, write_channel, address, NU_DEPEND_MASK_VPE_WDMA);
            break;
          }
          case VPE_RDMA_OP0: {
            address = desc->array_of_cfg_vpe[depend_i].op0_rdma_config.dma_baddr;
            rumboot_printf("VPE_RDMA_OP0 memory address = %x\n", address);
            nu_npe_write_channel_depend_cfg(desc, curr_i, write_channel, address, NU_DEPEND_MASK_VPE_WDMA);
            break;
          }
          case VPE_RDMA_OP1: {
            address = desc->array_of_cfg_vpe[depend_i].op1_rdma_config.dma_baddr;
            rumboot_printf("VPE_RDMA_OP1 memory address = %x\n", address);
            nu_npe_write_channel_depend_cfg(desc, curr_i, write_channel, address, NU_DEPEND_MASK_VPE_WDMA);
            break;
          }
          case VPE_RDMA_OP2: {
            address = desc->array_of_cfg_vpe[depend_i].op2_rdma_config.dma_baddr;
            rumboot_printf("VPE_RDMA_OP2 memory address = %x\n", address);
            nu_npe_write_channel_depend_cfg(desc, curr_i, write_channel, address, NU_DEPEND_MASK_VPE_WDMA);
            break;
          }
          case PPE_RDMA: {
            address = desc->array_of_cfg_reg_ppe[depend_i].rBALi;
            rumboot_printf("PPE_RDMA memory address = %x\n", address);
            nu_npe_write_channel_depend_cfg(desc, curr_i, write_channel, address, NU_DEPEND_MASK_PPE_RDMA);
            break;
          }
          default: break;
        }
      }
    }
  }
}


void nu_npe_add_depend_rd_channel_cfg(
  NPETestDescriptor* desc,
  int curr_i,
  int read_channel,
  uint32_t address,
  uint32_t depend_mask
) {

  switch (read_channel) {
    case MPE_RDMA_D: {
      rumboot_printf("MPE_RDMA_D\n");
      desc->array_of_cfg_mpe[curr_i].dma_d_config.rdma.BFCA = address;
      desc->array_of_cfg_mpe[curr_i].dma_d_config.depend_mask |= depend_mask;
      break;
    }
    case MPE_RDMA_W: {
      rumboot_printf("MPE_RDMA_W\n");
      desc->array_of_cfg_mpe[curr_i].dma_w_config.rdma.BFCA = address;
      desc->array_of_cfg_mpe[curr_i].dma_w_config.depend_mask |= depend_mask;
      break;
    }
    case VPE_RDMA: {
      rumboot_printf("VPE_RDMA\n");
      desc->array_of_cfg_vpe[curr_i].src_rdma_config.dma_baddr = address;
      desc->array_of_cfg_vpe[curr_i].depend_mask |= depend_mask;
      break;
    }
    case VPE_RDMA_OP0: {
      rumboot_printf("VPE_OP0_RDMA\n");
      desc->array_of_cfg_vpe[curr_i].op0_rdma_config.dma_baddr = address;
      desc->array_of_cfg_vpe[curr_i].depend_mask |= depend_mask;
      break;
    }
    case VPE_RDMA_OP1: {
      rumboot_printf("VPE_OP1_RDMA\n");
      desc->array_of_cfg_vpe[curr_i].op1_rdma_config.dma_baddr = address;
      desc->array_of_cfg_vpe[curr_i].depend_mask |= depend_mask;
      break;
    }
    case VPE_RDMA_OP2: {
      rumboot_printf("VPE_OP2_RDMA\n");
      desc->array_of_cfg_vpe[curr_i].op2_rdma_config.dma_baddr = address;
      desc->array_of_cfg_vpe[curr_i].depend_mask |= depend_mask;
      break;
    }
    case PPE_RDMA: {
      rumboot_printf("PPE_RDMA\n");
      desc->array_of_cfg_reg_ppe[curr_i].rBALi = address;
      desc->array_of_cfg_reg_ppe[curr_i].rOpEn |= depend_mask;
      break;
    }
    default: break;
  }
}


void nu_npe_write_channel_depend_cfg(
  NPETestDescriptor* desc,
  int curr_i,
  int write_channel,
  uint32_t address,
  uint32_t depend_mask
) {

  switch(write_channel) {
    case VPE_WDMA: {
      rumboot_printf("VPE_WDMA\n");
      desc->array_of_cfg_vpe[curr_i].wdma_config.dma_baddr = address;
      desc->array_of_cfg_vpe[curr_i].depend_mask |= depend_mask;
      break;
    }
    case PPE_WDMA: {
      if(desc->PPE_ENABLED==Enable_En) {
        rumboot_printf("PPE_WDMA\n");
        desc->array_of_cfg_reg_ppe[curr_i].wBALo = address;
        desc->array_of_cfg_reg_ppe[curr_i].wOpEn |= depend_mask;
      }
      break;
    }
    default: break;
  }
}
