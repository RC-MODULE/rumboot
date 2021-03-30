
#ifndef NU_TEST_LIB_H
#define NU_TEST_LIB_H

#include <stdint.h>
#include "platform/devices/nu_lib.h"


void* nu_vpe_load_op01(int heap_id, ConfigOp01* cfg, int index) ;
void* nu_vpe_load_op2(int heap_id, ConfigOp2* cfg);

#endif
