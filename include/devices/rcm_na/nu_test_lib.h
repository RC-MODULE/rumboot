
#ifndef NU_TEST_LIB_H
#define NU_TEST_LIB_H

#include <stdint.h>
#include "devices/rcm_na/nu_lib.h"

int nu_get_heap_id();
int nu_vpe_load_cfg(int heap_id, ConfigVPE* cfg);
int nu_vpe_load_array_of_cfg(int heap_id, ConfigVPE* array_of_cfg, int num) ;
int nu_vpe_load_cfg_by_tag(int heap_id, ConfigVPE* cfg, char* cfg_file_tag);
int nu_mpe_load_cfg(int heap_id, ConfigMPE* cfg);
int nu_mpe_load_cfg_by_tag(int heap_id, ConfigMPE* cfg, char* cfg_file_tag);
void* nu_mpe_load_cfg_lut(int heap_id);

int nu_ppe_load_cfg(int heap_id, ConfigPPE* cfg);
int nu_ppe_load_cfg_by_tag(int heap_id, ConfigPPE* cfg, char* cfg_file_tag);
int nu_ppe_load_cfg_reg_by_tag(int heap_id, ConfigREGPPE* cfg_reg, char* cfg_reg_file_tag);

CubeMetrics* nu_load_cube_metrics(int heap_id, char* file_tag);
WarrMetrics* nu_load_warr_metrics(int heap_id, char* file_tag);
VectorMetrics* nu_load_vec_metrics(int heap_id, char* file_tag);
CubeMetrics* nu_load_array_of_cube_metrics(int heap_id, char* file_tag, int num);
VectorMetrics* nu_load_array_of_vec_metrics(int heap_id, char* file_tag, int num);
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
void nu_vpe_iteration_start(VPEIterationDescriptor* iteration_desc);
void nu_vpe_iterate_desc(VPEIterationDescriptor* iteration_desc);
int nu_vpe_place_arrays(int heap_id, VPETestDescriptor* test_desc,int iterations);

#endif
