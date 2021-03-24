
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/io.h>
#include <platform/devices/ndma_simple.h>
#include <platform/regs/regs_ndma.h>


int ndma_simple_run(uintptr_t base, ndma_cfg_t * cfg)
{
  rumboot_printf( "NDMAC  simple run, regs init \n" );
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

void clear_ndma_status(uintptr_t base)
{
    iowrite32(0, base+NDMA_Control);
}
    

int ndma_simple_wait_error(uintptr_t base)
{
  uint32_t res;
  do{ res=ioread32(base+NDMA_Control); } while(! ( res&(NDMA_CONTROL_CPL|NDMA_CONTROL_ES) ) );
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


int ndma_apb_check(uintptr_t base, ndma_cfg_t * cfg)
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
  iowrite32(NDMA_CONTROL_DFLT  ,base+NDMA_Control);
  
  if (ioread32(base+NDMARD_Address) != cfg->RD_Address)
  {
      rumboot_printf("NDMARD_Address read fail\n");
      return 1;
  }
  if (ioread32(base+NDMARD_Bias) != cfg->RD_Bias)
  {
      rumboot_printf("NDMARD_Bias read fail\n");
      return 1;
  }
  if (ioread32(base+NDMARD_RowCounter) != cfg->RD_RowCounter)
  {
      rumboot_printf("NDMARD_RowCounter read fail\n");
      return 1;   
  }      
  if (ioread32(base+NDMARD_AddressMode) != cfg->RD_AddressMode)
  {
      rumboot_printf("NDMARD_AddressMode read fail\n");
      return 1;
  }

  if (ioread32(base+NDMAWR_Address) != cfg->WR_Address)
  {
      rumboot_printf("NDMAWR_Address read fail\n");
      return 1;
  }
  if (ioread32(base+NDMAWR_Bias) != cfg->WR_Bias)
  {
      rumboot_printf("NDMAWR_Bias read fail\n");
      return 1;
  }
  if (ioread32(base+NDMAWR_RowCounter) != cfg->WR_RowCounter)
  {
      rumboot_printf("NDMAWR_RowCounter read fail\n");
      return 1;    
  }
  if (ioread32(base+NDMAWR_AddressMode) != cfg->WR_AddressMode)
  {
      rumboot_printf("NDMAWR_AddressMode read fail\n");
      return 1;
  }
  if (ioread32(base+NDMA_MainCounter) != cfg->MainCounter)
  {
      rumboot_printf("NDMA_MainCounter read fail\n");
      return 1;
  }
  if (ioread32(base+NDMA_InterruptMask) != cfg->InterruptMask)
  {
      rumboot_printf("NDMA_InterruptMask read fail\n");
      return 1;
  }
  if (ioread32(base+NDMA_Control) != NDMA_CONTROL_DFLT)
  {
      rumboot_printf("NDMA_Control read fail\n");
      return 1;
  }
  if (ioread32(base+NDMA_State) != NDMA_STATE_DFLT)
  {
      rumboot_printf("NDMA_State read fail\n");
      return 1;
  }
  return 0;
}

int ndma_apb_running_one(uintptr_t base)
{
    uint32_t data = 1;
    for (int i=0; i<32; i++)
    {
        iowrite32(data    ,base+NDMARD_Address     );
        if (ioread32(base+NDMARD_Address) != data)
        {
            rumboot_printf("NDMAC apb running 1 fail, data %x", data);
            return 1;
        }
        data = data << 1;
    }
    return 0;
}
