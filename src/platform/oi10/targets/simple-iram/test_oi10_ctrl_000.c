 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>
#include <platform/oi10/platform/devices.h>
#include <platform/regs/sctl.h>

static uint8_t check_ctrl_regs_ro(uint32_t base_addr)
{   
    rumboot_printf("Checking sctl registers default value...\n");
    struct regpoker_checker check_array[] =
     {
     { "PPC_SYS_CONF", REGPOKER_READ_DCR, SCTL_PPC_SYS_CONF, 0x0, 0x1 },
     { "KMBIST_CHAIN_SF_i", REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0, 0b00000, 0b11111 },
     { "KMBIST_CHAIN_SF_i", REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0, 0b000000, 0b111111 },
     { "PPC_SYS_CONF", REGPOKER_READ_DCR, SCTL_PPC_SYS_CONF, 0b000, 0b111 },
     {  }
    };
      if( rumboot_regpoker_check_array( check_array, base_addr ) == 0 )
      {
          rumboot_printf( "OK\n" );
          return 0;
      }
      rumboot_printf( "ERROR\n" );
      return 1;
}

static int check_ctrl_regs_rw(uint32_t base_addr)
{
    rumboot_printf("Checking sctl registers for rw value...\n");
    struct regpoker_checker check_array[] = {
    { "KMBIST_CHAIN_SF_i", REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0, 0b0000, 0b1111 },
    { "KMBIST_CHAIN_SF_I", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_0, 0b000,   0b1 },
    { "KMBIST_CHAIN_SF_i", REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0, 0b0000, 0b1111 },
    { "KMBIST_CHAIN_SF_i", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_0, 0b000,   0b111 },
    { "PPC_SYS_CONF", REGPOKER_READ_DCR, SCTL_PPC_SYS_CONF, 0b11000, 0b11111 },
    { "PPC_SYS_CONF", REGPOKER_READ_DCR, SCTL_PPC_SYS_CONF, 0b0, 0b1 },
    { "PPC_SLP_CTRL", REGPOKER_READ_DCR, SCTL_PPC_SLP_CTRL, 0b0000, 0b1111 },
    { "PPC_PMU_CTRL", REGPOKER_READ_DCR, SCTL_PPC_PMU_CTRL, 0b00000, 0b11111 },
    { "IFSYS_ETH_CTRL", REGPOKER_READ_DCR, SCTL_IFSYS_ETH_CTRL, 0b0000, 0b1111 },
    { "IFSYS_ETH_CTRL", REGPOKER_READ_DCR, SCTL_IFSYS_ETH_CTRL, 0x10000, 0x11111 },
    { "IFSYS_ETH_HPROT", REGPOKER_READ_DCR, SCTL_IFSYS_ETH_HPROT, 0x33333333, 0xffffffff },
    {  }
    };
    if( rumboot_regpoker_check_array( check_array, base_addr ) == 0 ) {
           rumboot_printf( "OK\n" );
           return 0;
       }
    rumboot_printf( "ERROR\n" );
    return 1;
}

int main(void)
{
   register int result;
   rumboot_printf("Check write/read ctrl register values\n");

   result = check_ctrl_regs_ro(DCR_SCTL_BASE)
           || check_ctrl_regs_rw(DCR_SCTL_BASE);
   return result;
}
