
#ifndef NU_TEST_LIB_H
#define NU_TEST_LIB_H

#include <stdint.h>
#include "platform/devices/nu_lib.h"

int nu_get_heap_id();
int nu_vpe_load_cfg(int heap_id, ConfigVPE* cfg);
CubeMetrics* nu_load_cube_metrics(int heap_id, char* file_tag);
CubeMetrics* nu_vpe_load_in_metrics(int heap_id);
CubeMetrics* nu_vpe_load_res_metrics(int heap_id);
void* nu_load_cube(int heap_id,char* file_tag,CubeMetrics* metrics);
void* nu_vpe_load_in_data(int heap_id,CubeMetrics* metrics);
void* nu_vpe_malloc_res(int heap_id,CubeMetrics* metrics);
void* nu_vpe_load_etalon(int heap_id,CubeMetrics* metrics);
void* nu_vpe_load_op01(int heap_id, ConfigOp01* cfg, int index) ;
void* nu_vpe_load_op2(int heap_id, ConfigOp2* cfg);

#endif
