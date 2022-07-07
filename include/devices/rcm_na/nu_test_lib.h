
#ifndef NU_TEST_LIB_H
#define NU_TEST_LIB_H

#include <stdint.h>
#include <regs/regs_na.h>
#include "devices/rcm_na/nu_lib.h"

#define NU_MPE_REG_MAP_SIZE (MPE_MA_BASE + MPE_CMD_ICMW)  // 0xF00
#define NU_VPE_REG_MAP_SIZE (NU_VPE_OP2_RDMA + NU_VPE_DMA_BATCH_STRIDE) // 0x1DF
#define NU_PPE_RDMA_REG_MAP_SIZE NU_PPE_RDMA_BOX_OFFSET_Z // 0x074
#define NU_PPE_WDMA_REG_MAP_SIZE NU_PPE_PADDING_VALUE_7 // 0x0С8

#define NU_RDMA_NUMBER 7
#define NU_WDMA_NUMBER 2

#define NU_DEPEND_MASK_MPE_RDMA_D (0x1<<24)
#define NU_DEPEND_MASK_MPE_RDMA_W (0x1<<25)
#define NU_DEPEND_MASK_VPE_WDMA (0x1<<26)
#define NU_DEPEND_MASK_PPE_RDMA (0x1<<27)
#define NU_DEPEND_MASK_PPE_WDMA (0x1<<28)

int nu_get_heap_id();
int nu_get_add_heap_id();
uintptr_t nu_virt_to_dma(volatile const void *addr);
uint32_t nu_get_uptime_ns();
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

uint32_t mask_N_M (int N, int M);

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
void nu_vpe_print_op_array_desc(OpArrayDescriptor* desc);
int nu_vpe_load_arrays_of_op_metrics(
  int heap_id_op0,int heap_id_op1,int heap_id_op2,int heap_id_lut,
  OpArrayDescriptor* op0_array_desc,
  OpArrayDescriptor* op1_array_desc,
  OpArrayDescriptor* op2_array_desc,
  ConfigVPE* array_of_cfg, 
  int num
);
int nu_vpe_load_array_of_op_cubes(int heap_id, OpArrayDescriptor* op_array_desc,char* file_tag);
int nu_vpe_load_array_of_op_vecs(int heap_id, OpArrayDescriptor* op_array_desc,char* file_tag);
int nu_vpe_load_arrays_of_ops(
  int heap_id_op0,int heap_id_op1, int heap_id_op2, int heap_id_lut,
  OpArrayDescriptor* op0_array_desc,
  OpArrayDescriptor* op1_array_desc,
  OpArrayDescriptor* op2_array_desc
);
StatusRegs* nu_load_array_of_status_regs(int heap_id,int num);

int nu_vpe_load_status_regs_by_tag(int heap_id, StatusRegs* status_regs, char* status_regs_tag);
int nu_bitwise_compare(void* res_data, void* etalon, int size);
float fp16_to_fp32(const short in);
void nu_vpe_interpret_mismatch(ConfigVPE *cfg,void* res_data,void* etalon,void* in_data,void* op0,void* op1,void* op2,int s,int C);

int nu_half_compare_eps(void* res_data, void* etalon, int size);

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

  StatusRegs* array_of_status_regs_etalon;
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

  StatusRegs* status_regs_etalon;

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

  StatusRegs* array_of_status_regs_etalon;
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

  StatusRegs* status_regs_etalon;

} PPEIterationDescriptor;

void nu_ppe_init_iteration_desc(PPETestDescriptor* test_desc, PPEIterationDescriptor* iteration_desc);
void nu_ppe_iterate_desc(PPEIterationDescriptor* desc);

int nu_ppe_place_arrays(int heap_id, PPETestDescriptor* test_desc,int iterations);

typedef struct NAHeapMap {
  int in_data;
  int warr;
  int op0;
  int op1;
  int op2;
  int lut;
  int mpe_cfg;
  int mpe_cfg_lut;
  int vpe_cfg;
  int ppe_cfg;
  int res;
  int etalon;
  
  int metrics_in_data;
  int metrics_warr;
  int metrics_op0;
  int metrics_op1;
  int metrics_op2;
  int metrics_lut;
  int metrics_mpe_cfg;
  int metrics_mpe_cfg_lut;
  int metrics_vpe_cfg;
  int metrics_ppe_cfg;
  // int metrics_res;
  int metrics_etalon;
  
  int control;
} NAHeapMap;

void nu_get_rather_fair_heap_map(NAHeapMap* heap_map);
void nu_get_rather_fair_heap_map_perf_do(NAHeapMap* heap_map);

typedef struct NARegDump {
  uint32_t mpe[NU_MPE_REG_MAP_SIZE/sizeof(uint32_t)];
  uint32_t vpe[NU_VPE_REG_MAP_SIZE/sizeof(uint32_t)];
  uint32_t ppe_rdma[NU_PPE_RDMA_REG_MAP_SIZE/sizeof(uint32_t)];
  uint32_t ppe_wdma[NU_PPE_WDMA_REG_MAP_SIZE/sizeof(uint32_t)];
} NARegDump;

typedef enum NA_RDMA {
  MPE_RDMA_D=0,
  MPE_RDMA_W=1,
  VPE_RDMA=2,
  VPE_RDMA_OP0=3,
  VPE_RDMA_OP1=4,
  VPE_RDMA_OP2=5,
  PPE_RDMA=6
} NA_RDMA;

typedef enum NA_WDMA {
  VPE_WDMA=0,
  PPE_WDMA=1
} NA_WDMA;

typedef struct NADependTable {
  int16_t read_after_write[NU_WDMA_NUMBER][NU_RDMA_NUMBER];
  int16_t write_after_read[NU_WDMA_NUMBER][NU_RDMA_NUMBER];
} NADependTable;

typedef struct NAIterationConfigMap {
  int num_mpe_cfgs;
  int num_vpe_cfgs;
  int num_ppe_cfgs;
  ConfigMPE** cfg_mpe;
  ConfigVPE** cfg_vpe;
  ConfigPPE** cfg_ppe;
  ConfigREGPPE** cfg_reg_ppe;
} NAIterationConfigMap;

///
typedef struct NPETestDescriptor {
  Enable MPE_ENABLED;
  Enable VPE_ENABLED;
  Enable PPE_ENABLED;
  
  ConfigMPE* array_of_cfg_mpe;
  ConfigVPE* array_of_cfg_vpe;
  ConfigPPE* array_of_cfg_ppe;
  
  ConfigREGPPE* array_of_cfg_reg_ppe;

  NPEReg* associative_regs_dump_start_ptr;
  NPEReg* associative_regs_dump_end_ptr;

  NADependTable* array_of_depend_table;
  NAIterationConfigMap iteration_cfg_map;
  
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
  int invocations;
  //~ StatusRegs* array_of_status_regs_etalon;
}NPETestDescriptor;

void nu_npe_init_test_desc(NPETestDescriptor* test_desc);
void nu_npe_print_test_desc(NPETestDescriptor* test_desc, int iterations);


typedef struct NPEIterationDescriptor {
  Enable MPE_ENABLED;
  Enable VPE_ENABLED;
  Enable PPE_ENABLED;
  
  ConfigMPE* cfg_mpe;
  ConfigVPE* cfg_vpe;
  ConfigPPE* cfg_ppe;

  ConfigREGPPE*  cfg_reg_ppe;

  NARegDump* curr_regs_dump;
  NARegDump* next_regs_dump;

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

  //~ StatusRegs* status_regs_etalon;
} NPEIterationDescriptor;

void nu_npe_init_iteration_desc(NPETestDescriptor* test_desc, NPEIterationDescriptor* iteration_desc);
void nu_npe_iteration_start(NPEIterationDescriptor* iteration_desc);
void nu_npe_iteration_with_dep_start(NPEIterationDescriptor* iteration_desc,NAIterationConfigMap* iteration_cfg_map,int i);
void nu_npe_iterate_desc(NPEIterationDescriptor* desc) ;
void nu_npe_iterate_desc_with_dep(NPETestDescriptor* test_desc, NPEIterationDescriptor* iteration_desc, int i);
int nu_npe_place_arrays_by_heap_map(NAHeapMap* heap_map, NPETestDescriptor* test_desc,int iterations);
int nu_npe_place_arrays(int heap_id, NPETestDescriptor* test_desc,int iterations);
int nu_malloc_iteration_cfg_map(int heap_id, NAIterationConfigMap* iteration_cfg_map, int iterations);
bool nu_mpe_dep_table_requires_mpe_run(NADependTable* dep_table);
bool nu_vpe_dep_table_requires_vpe_run(NADependTable* dep_table);
bool nu_ppe_dep_table_requires_ppe_run(NADependTable* dep_table);
void nu_npe_count_cfgs_by_dep_table(NAIterationConfigMap* iteration_cfg_map,NADependTable* array_of_depend_table,int iterations);
void nu_fill_iteration_cfg_map(
  NAIterationConfigMap* iteration_cfg_map, 
  NADependTable* array_of_depend_table, 
  ConfigMPE* array_of_cfg_mpe,
  ConfigVPE* array_of_cfg_vpe,
  ConfigPPE* array_of_cfg_ppe,
  ConfigREGPPE* array_of_cfg_reg_ppe,
  int iterations
);
int nu_npe_place_arrays_with_dep_by_heap_map(NAHeapMap* heap_map, NPETestDescriptor* test_desc,int iterations);
void nu_npe_erase_unused_in_data(NPEIterationDescriptor* iteration_desc, NADependTable* dep_table, int curr_i);

void nu_vpe_pause_next_cntx_fail_stop(uintptr_t vpe_base, ConfigVPE* cfg);
int nu_vpe_regs_check(uintptr_t base, int num, int iteration);
int nu_regs_check(uintptr_t base,int num, int iteration);
int nu_ppe_regs_check(uintptr_t base,int num, int iteration);
void nu_na_mpe_pause_fail_stop(uintptr_t npe_base);
void nu_na_vpe_pause_fail_stop(uintptr_t npe_base);
void nu_na_vpe_fail_mpe_srst_stop(uintptr_t npe_base);
void nu_na_ppe_pause_fail_stop(uintptr_t npe_base); 
int nu_mpe_regs_check(uintptr_t base, int num, int iteration);

void nu_npe_add_depend_rd_after_wr(NPETestDescriptor* desc,int curr_i);
void nu_npe_add_depend_wr_after_rd(
  NPETestDescriptor* desc,
  int curr_i
);

void nu_npe_add_depend_rd_channel_cfg(
  NPETestDescriptor* curr_desc,
  int curr_i,
  int read_channel,
  uint32_t address,
  uint32_t start_bit
);

void nu_npe_write_channel_depend_cfg(
  NPETestDescriptor* curr_desc,
  int curr_i,
  int write_channel,
  uint32_t address,
  uint32_t start_bit
);

void nu_npe_reg_map_swap(NPEIterationDescriptor* desc);
int nu_npe_place_associative_regs_dump(int heap_id, NPETestDescriptor* test_desc,int iterations);
int nu_npe_place_regs_dump(int heap_id, NPEIterationDescriptor* desc);
int nu_npe_place_array_of_depend_table(int heap_id, NPETestDescriptor* test_desc,int iterations);
NADependTable* nu_load_array_of_depend_table(int heap_id, char* file_tag, char* metrics_file_tag, int iterations);
void nu_print_array_of_depend_table(NPETestDescriptor* test_desc, int iterations);

int nu_ppe_rdma_regs_swrst_check (uintptr_t base);
int nu_ppe_regs_swrst_check (uintptr_t base);

int get_nmb_10_clk_t();

#endif
