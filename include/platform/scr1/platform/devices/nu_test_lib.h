
#ifndef NU_TEST_LIB_H
#define NU_TEST_LIB_H

#include <stdint.h>
#include "platform/devices/nu_lib.h"

int nu_get_heap_id();
int nu_vpe_load_cfg(int heap_id, ConfigVPE* cfg);
int nu_vpe_load_cfg_by_tag(int heap_id, ConfigVPE* cfg, char* cfg_file_tag);
int nu_mpe_load_cfg(int heap_id, ConfigMPE* cfg);

int nu_ppe_load_cfg(int heap_id, ConfigPPE* cfg);
int nu_ppe_load_cfg_by_tag(int heap_id, ConfigPPE* cfg, char* cfg_file_tag);
int nu_ppe_load_cfg_reg_by_tag(int heap_id, ConfigREGPPE* cfg_reg, char* cfg_reg_file_tag);

CubeMetrics* nu_load_cube_metrics(int heap_id, char* file_tag);
WarrMetrics* nu_load_warr_metrics(int heap_id, char* file_tag);
VectorMetrics* nu_load_vec_metrics(int heap_id, char* file_tag);
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
void* nu_vpe_load_etalon(int heap_id,CubeMetrics* metrics);
void* nu_mpe_load_etalon(int heap_id,CubeMetrics* metrics);
void* nu_ppe_load_etalon(int heap_id,CubeMetrics* metrics);
void* nu_vpe_load_op01(int heap_id, ConfigOp01* cfg, int index) ;
void* nu_vpe_load_op01_by_tags(int heap_id, ConfigOp01* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag);
void* nu_vpe_load_op01_misaligned_by_tags(int heap_id, ConfigOp01* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag, int misalign);
void* nu_vpe_load_op2(int heap_id, ConfigOp2* cfg);
void* nu_vpe_load_op2_by_tags(int heap_id, ConfigOp2* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag);
void* nu_vpe_load_op2_misaligned_by_tags(int heap_id, ConfigOp2* cfg, char* metrics_cube_tag,char* metrics_vec_tag,char* cube_file_tag,char* vec_file_tag, int misalign);
int nu_vpe_load_status_regs_by_tag(int heap_id, VPEStatusRegs* status_regs, char* status_regs_tag);
int nu_bitwise_compare(void* res_data, void* etalon, int size);

#endif
