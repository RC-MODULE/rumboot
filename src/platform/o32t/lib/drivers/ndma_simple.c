
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/io.h>
#include <platform/devices/ndma_simple.h>
#include <platform/regs/regs_ndma.h>


int ndma_simple_run(uintptr_t base, ndma_cfg_t * cfg)
{
  iowrite32(cfg->RD_Address    ,base+NDMARD_Address     );
  iowrite32(cfg->RD_Bias       ,base+NDMARD_Bias        );
  iowrite32(cfg->RD_RowCounter ,base+NDMARD_RowCounter  );
  iowrite32(cfg->RD_AddressMode,base+NDMARD_AddressMode );
  iowrite32(cfg->WR_Address    ,base+NDMAWR_Address     );
  iowrite32(cfg->WR_Bias       ,base+NDMAWR_Bias        );
  iowrite32(cfg->WR_RowCounter ,base+NDMAWR_RowCounter  );
  iowrite32(cfg->WR_AddressMode,base+NDMAWR_AddressMode );
  iowrite32(cfg->MainCounter   ,base+NDMA_MainCounter   );
  iowrite32(cfg->InterruptMask ,base+NDMA_InterruptMask );
  
  iowrite32(NDMA_CONTROL_EN,base+NDMA_Control);
  return 0;
}

int ndma_simple_wait_complete(uintptr_t base)
{
  uint32_t res;
  do{ res=ioread32(base+NDMA_Control); } while(! ( res&NDMA_CONTROL_CPL ) );
  return 0;
}

int ndma_simple_memcpy(uintptr_t base, ndma_cfg_t * cfg)
{
  int res;
  
  res = ndma_simple_run(base,cfg);
  if(res) return res;
  res = ndma_simple_wait_complete(base);
  return res;
}
