
#include <stdint.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices/nu_lib.h> 


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
    return op;
  } 
    
    // Try If Op0 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file(index?"metrics_op1_vec_tag":"metrics_op0_vec_tag",(uintptr_t)vec_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file(index?"op1_vec_file_tag":"op0_vec_file_tag",(uintptr_t)op);
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
    return op;
  } 
    
    // Try If Op2 Is A Vector
  if((cfg->alu_en==Enable_En && cfg->alu_mode==Mode_Channel) || 
     (cfg->mux_en==Enable_En && cfg->mux_mode==Mode_Channel) ) {
    vec_metrics = rumboot_malloc_from_heap_aligned(heap_id,sizeof(VectorMetrics),sizeof(int32_t));
    rumboot_platform_request_file("metrics_op2_vec_tag",(uintptr_t)vec_metrics);
    op = rumboot_malloc_from_heap_aligned(heap_id,vec_metrics->s,64);
    rumboot_platform_request_file("op2_vec_file_tag",(uintptr_t)op);
    return op;
  } 
  
  return NULL;
}

