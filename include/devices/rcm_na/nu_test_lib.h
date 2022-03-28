
#ifndef NU_TEST_LIB_H
#define NU_TEST_LIB_H

#include <stdint.h>
#include "devices/rcm_na/nu_lib.h"

#define NU_MPE_REG_MAP_SIZE 0x21C
#define NU_VPE_REG_MAP_SIZE 0x1DC
#define NU_PPE_REG_MAP_SIZE 0x32

int nu_get_heap_id();
int nu_get_add_heap_id();
uintptr_t nu_virt_to_dma(volatile const void *addr);
int nu_vpe_load_cfg(int heap_id, ConfigVPE* cfg);
int nu_vpe_load_array_of_cfg(int heap_id, ConfigVPE* array_of_cfg, int num) ;
int nu_vpe_load_cfg_by_tag(int heap_id, ConfigVPE* cfg, char* cfg_file_tag);
int nu_mpe_load_cfg(int heap_id, ConfigMPE* cfg);
int nu_mpe_load_cfg_by_tag(int heap_id, ConfigMPE* cfg, char* cfg_file_tag);
void* nu_mpe_load_cfg_lut(int heap_id);
int nu_mpe_load_array_of_cfg(int heap_id, ConfigMPE* array_of_cfg, int num);

int nu_ppe_load_cfg(int heap_id, ConfigPPE* cfg);
int nu_ppe_load_cfg_by_tag(int heap_id, ConfigPPE* cfg, char* cfg_file_tag);
int nu_ppe_load_cfg_reg_by_tag(int heap_id, ConfigREGPPE* cfg_reg, char* cfg_reg_file_tag);
int nu_ppe_load_array_of_cfg(int heap_id, ConfigPPE* array_of_cfg, int num) ;

CubeMetrics* nu_load_cube_metrics(int heap_id, char* file_tag);
WarrMetrics* nu_load_warr_metrics(int heap_id, char* file_tag);
VectorMetrics* nu_load_vec_metrics(int heap_id, char* file_tag);
CubeMetrics* nu_load_array_of_cube_metrics(int heap_id, char* file_tag, int num);
VectorMetrics* nu_load_array_of_vec_metrics(int heap_id, char* file_tag, int num);
WarrMetrics* nu_load_array_of_warr_metrics(int heap_id, char* file_tag, int num);
MPECmdMetrics* nu_mpe_load_cmd_metrics(int heap_id);
MPECmdMetrics* nu_mpe_load_cmd_metrics_by_tag(int heap_id, char* file_tag);
CubeMetrics* nu_vpe_load_in_metrics(int heap_id);
CubeMetrics* nu_mpe_load_in_metrics(int heap_id);
CubeMetrics* nu_ppe_load_in_metrics(int heap_id);
CubeMetrics* nu_vpe_load_res_metrics(int heap_id);
CubeMetrics* nu_mpe_load_res_metrics(int heap_id);
CubeMetrics* nu_ppe_load_res_metrics(int heap_id);
WarrMetrics* nu_mpe_load_warr_metrics(int heap_id);
void* nu_load_cube(int heap_id,char* file_tag,CubeMetrics* metrics);
void* nu_load_array_of_cubes(int heap_id,char* file_tag,CubeMetrics* array_of_metrics,int num);
void* nu_load_array_of_vecs(int heap_id,char* file_tag,VectorMetrics* array_of_metrics,int num);
void* nu_load_array_of_warr(int heap_id,char* file_tag,WarrMetrics* array_of_metrics,int num);
void* nu_load_cube_misaligned(int heap_id,char* file_tag,CubeMetrics* metrics, int misalign);
void* nu_load_warr(int heap_id,char* file_tag,WarrMetrics* metrics);
void* nu_load_vec(int heap_id,char* file_tag,VectorMetrics* metrics);
void* nu_vpe_load_in_data(int heap_id,CubeMetrics* metrics);
void* nu_mpe_load_in_data(int heap_id,CubeMetrics* metrics);
void* nu_ppe_load_in_data(int heap_id,CubeMetrics* metrics);
void* nu_mpe_load_warr(int heap_id,WarrMetrics* metrics);
void* nu_mpe_load_cmd(int heap_id,MPECmdMetrics* metrics);
void* nu_mpe_load_cmd_by_tag(int heap_id, char* file_tag, MPECmdMetrics* metrics);
void* nu_vpe_malloc_res(int heap_id,CubeMetrics* metrics);
void* nu_mpe_malloc_res(int heap_id,CubeMetrics* metrics);
void* nu_ppe_malloc_res(int heap_id,CubeMetrics* metrics);
void* nu_malloc_array_of_cubes(int heap_id,CubeMetrics* array_of_metrics,int num,int* size);
void* nu_malloc_array_of_vecs(int heap_id,VectorMetrics* array_of_metrics,int num,int* size);
void* nu_malloc_array_of_warr(int heap_id,WarrMetrics* array_of_metrics,int num,int* size);
void* nu_vpe_load_etalon(int heap_id,CubeMetrics* metrics);
void* nu_mpe_load_etalon(int heap_id,CubeMetrics* metrics);
void* nu_ppe_load_etalon(int heap_id,CubeMetrics* metrics);
void* nu_vpe_load_op01(int heap_id, ConfigOp01* cfg, int index) ;
void* nu_vpe_load_op01_by_tags(int heap_id, ConfigOp01* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag);
void* nu_vpe_load_op01_misaligned_by_tags(int heap_id, ConfigOp01* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag, int misalign);
void* nu_vpe_load_op2(int heap_id, ConfigOp2* cfg);
void* nu_vpe_load_op2_by_tags(int heap_id, ConfigOp2* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag);
void* nu_vpe_load_op2_misaligned_by_tags(int heap_id, ConfigOp2* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag, int misalign);

typedef struct OpArrayDescriptor {
  CubeMetrics*   array_of_cube_metrics;
  VectorMetrics* array_of_vec_metrics;
  VectorMetrics* array_of_lut1_metrics;
  VectorMetrics* array_of_lut2_metrics;
  void* array_of_cubes;
  void* array_of_vecs;
  void* array_of_lut1;
  void* array_of_lut2;
  int num_cubes;
  int num_vecs;
  int num_luts;
}OpArrayDescriptor;
void nu_vpe_init_op_array_desc(OpArrayDescriptor* desc);
int nu_vpe_load_arrays_of_op_metrics(int heap_id,OpArrayDescriptor* op0_array_desc,OpArrayDescriptor* op1_array_desc,OpArrayDescriptor* op2_array_desc,ConfigVPE* array_of_cfg, int num);
int nu_vpe_load_array_of_op_cubes(int heap_id, OpArrayDescriptor* op_array_desc,char* file_tag);
int nu_vpe_load_array_of_op_vecs(int heap_id, OpArrayDescriptor* op_array_desc,char* file_tag);
int nu_vpe_load_arrays_of_ops(int heap_id,OpArrayDescriptor* op0_array_desc,OpArrayDescriptor* op1_array_desc,OpArrayDescriptor* op2_array_desc);
VPEStatusRegs* nu_vpe_load_array_of_status_regs(int heap_id,int num);

int nu_vpe_load_status_regs_by_tag(int heap_id, VPEStatusRegs* status_regs, char* status_regs_tag);
int nu_bitwise_compare(void* res_data, void* etalon, int size);
float fp16_to_fp32(const short in);
void nu_vpe_interpret_mismatch(ConfigVPE *cfg,void* res_data,void* etalon,void* in_data,void* op0,void* op1,void* op2,int s,int C);

int nu_half_compare_eps(void* res_data, void* etalon, int size, int eps);

//in case of <string.h> and <stdio.h> are absent;
char* int_to_strdec (char* str, int arg);
int str_length(char* str);
char* str_concat(char* lstr, char* rstr);
char* str_copy (char* dst, char* src);

char* fn_base_it_nmb (char* dst, char* src, int it_nmb);

///////////
// Long Test Iteration Stuff

typedef struct VPETestDescriptor {
  ConfigVPE* array_of_cfg;
  
  CubeMetrics* array_of_in_metrics;
  CubeMetrics* array_of_res_metrics;
  
  void *array_of_in_data;
  void *array_of_etalon;
  void *array_of_res_data;
  
  OpArrayDescriptor op0_array_desc;
  OpArrayDescriptor op1_array_desc;
  OpArrayDescriptor op2_array_desc;

  VPEStatusRegs* array_of_status_regs_etalon;
  int invocations;
}VPETestDescriptor;

void nu_vpe_init_test_desc(VPETestDescriptor* test_desc);

typedef struct VPEIterationDescriptor {
  ConfigVPE* cfg;

  void *in_data;
  void *etalon;
  void *res_data;
  void *op0_cube;
  void *op1_cube;
  void *op2_cube;
  void *op0_vec;
  void *op1_vec;
  void *op2_vec;
  void *op0;
  void *op1;
  void *op2;
  void *lut1;
  void *lut2;

  CubeMetrics* in_metrics;
  CubeMetrics* res_metrics;
  
  CubeMetrics*   op0_cube_metrics;
  VectorMetrics* op0_vec_metrics;
  CubeMetrics*   op1_cube_metrics;
  VectorMetrics* op1_vec_metrics;
  CubeMetrics*   op2_cube_metrics;
  VectorMetrics* op2_vec_metrics;

  VectorMetrics* lut1_metrics;
  VectorMetrics* lut2_metrics;

  VPEStatusRegs* status_regs_etalon;

}VPEIterationDescriptor ;

void nu_vpe_init_iteration_desc(VPETestDescriptor* test_desc, VPEIterationDescriptor* iteration_desc);
void nu_vpe_print_iteration_desc(VPEIterationDescriptor* iteration_desc);
void nu_vpe_iteration_start(VPEIterationDescriptor* iteration_desc);
void nu_vpe_iterate_desc(VPEIterationDescriptor* desc);
int nu_vpe_place_arrays(int heap_id, VPETestDescriptor* test_desc,int iterations);
int nu_vpe_invocations_cnt(CubeMetrics* array_of_in_metrics,int num_cubes);
void nu_vpe_batch_size_stride_cnt(CubeMetrics* array_of_in_metrics,ConfigVPE* array_of_cfg,int num_cubes);
void nu_vpe_batch_size_stride_of_ops_cnt(
  OpArrayDescriptor* op0_array_desc,
  OpArrayDescriptor* op1_array_desc,
  OpArrayDescriptor* op2_array_desc,
  ConfigVPE* array_of_cfg,
  int invocations
);


// LUT Stuff (For "Tight" Tests)
typedef enum LUTLoadDecision /*: unsigned char*/ {
  LUTLoadDecision_DontLoad = 0,
  LUTLoadDecision_LoadNow  = 1,
  LUTLoadDecision_BlockThenLoad = 2
}LUTLoadDecision;

int nu_vpe_compare_luts(void* lut1_one, void* lut2_one, void* lut1_another, void* lut2_another); // Returns 0 If Equal, 1 - If Not

///
typedef struct PPETestDescriptor {
  ConfigPPE*     array_of_cfg;
  ConfigREGPPE*  array_of_cfg_reg;
  
  CubeMetrics* array_of_in_metrics;
  CubeMetrics* array_of_res_metrics;
  
  void *array_of_in_data;
  void *array_of_etalon;
  void *array_of_res_data;
  
  char* in_file_tag; // Crutch For Tests That Need To Overwrite This Parameter
  
} PPETestDescriptor;
  
void nu_ppe_init_test_desc(PPETestDescriptor* test_desc);


typedef struct PPEIterationDescriptor {
  ConfigPPE*     cfg;
  ConfigREGPPE*  cfg_reg;
  
  CubeMetrics*  in_metrics  ;
  CubeMetrics*  res_metrics ;

  void* in_data ;
  void* etalon  ;
  void* res_data;
} PPEIterationDescriptor;

void nu_ppe_init_iteration_desc(PPETestDescriptor* test_desc, PPEIterationDescriptor* iteration_desc);
void nu_ppe_iterate_desc(PPEIterationDescriptor* desc);

int nu_ppe_place_arrays(int heap_id, PPETestDescriptor* test_desc,int iterations);


///
typedef struct NPETestDescriptor {
  Enable MPE_ENABLED;
  Enable PPE_ENABLED;
  
  ConfigMPE* array_of_cfg_mpe;
  ConfigVPE* array_of_cfg_vpe;
  ConfigPPE* array_of_cfg_ppe;
  
  ConfigREGPPE* array_of_cfg_reg_ppe;

  NPEReg* array_of_cfg_diff_mpe;
  uint32_t* array_of_cfg_diff_mpe_size;

  NPEReg* array_of_cfg_diff_vpe;
  uint32_t* array_of_cfg_diff_vpe_size;

  NPEReg* array_of_cfg_diff_ppe;
  uint32_t* array_of_cfg_diff_ppe_size;
  
  WarrMetrics* array_of_warr_metrics;
  CubeMetrics* array_of_in_metrics;
  CubeMetrics* array_of_res_metrics;
  
  void *array_of_in_data;
  void *array_of_warr;
  void *array_of_etalon;
  void *array_of_res_data;
  
  OpArrayDescriptor op0_array_desc;
  OpArrayDescriptor op1_array_desc;
  OpArrayDescriptor op2_array_desc;
  
  void* mpe_cfg_lut;

  //~ VPEStatusRegs* array_of_status_regs_etalon;
}NPETestDescriptor;

void nu_npe_init_test_desc(NPETestDescriptor* test_desc);


typedef struct NPEIterationDescriptor {
  Enable MPE_ENABLED;
  Enable PPE_ENABLED;
  
  ConfigMPE* cfg_mpe;
  ConfigVPE* cfg_vpe;
  ConfigPPE* cfg_ppe;

  ConfigREGPPE*  cfg_reg_ppe;

  NPEReg* cfg_diff_mpe;
  uint32_t* cfg_diff_mpe_size;

  NPEReg* cfg_diff_vpe;
  uint32_t* cfg_diff_vpe_size;

  NPEReg* cfg_diff_ppe;
  uint32_t* cfg_diff_ppe_size;

  void *warr;
  void *in_data;
  void *etalon;
  void *res_data;
  void *op0_cube;
  void *op1_cube;
  void *op2_cube;
  void *op0_vec;
  void *op1_vec;
  void *op2_vec;
  void *op0;
  void *op1;
  void *op2;
  void *lut1;
  void *lut2;

  WarrMetrics* warr_metrics;
  CubeMetrics* in_metrics;
  CubeMetrics* res_metrics;
  
  CubeMetrics  mpe_out_metrics; // Not A Pointer!
  
  CubeMetrics*   op0_cube_metrics;
  VectorMetrics* op0_vec_metrics;
  CubeMetrics*   op1_cube_metrics;
  VectorMetrics* op1_vec_metrics;
  CubeMetrics*   op2_cube_metrics;
  VectorMetrics* op2_vec_metrics;

  VectorMetrics* lut1_metrics;
  VectorMetrics* lut2_metrics;

  //~ VPEStatusRegs* status_regs_etalon;
} NPEIterationDescriptor;

void nu_npe_init_iteration_desc(NPETestDescriptor* test_desc, NPEIterationDescriptor* iteration_desc);
void nu_npe_iteration_start(NPEIterationDescriptor* iteration_desc);
void nu_npe_iterate_desc(NPEIterationDescriptor* desc) ;
int nu_npe_place_arrays(int heap_id, NPETestDescriptor* test_desc,int iterations);
void nu_vpe_pause_next_cntx_fail_stop(uintptr_t vpe_base, ConfigVPE* cfg);
int nu_vpe_regs_check(uintptr_t base, int num, int iteration);
int nu_regs_check(uintptr_t base,int num, int iteration);
int nu_ppe_regs_check(uintptr_t base,int num, int iteration);
void nu_na_mpe_pause_fail_stop(uintptr_t npe_base);
void nu_na_vpe_pause_fail_stop(uintptr_t npe_base);
void nu_na_vpe_fail_mpe_srst_stop(uintptr_t npe_base);
void nu_na_ppe_pause_fail_stop(uintptr_t npe_base); 
int nu_mpe_regs_check(uintptr_t base, int num, int iteration);
#endif
