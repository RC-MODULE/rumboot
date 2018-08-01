
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <rumboot/regpoker.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/regs_nic400.h>
#include <platform/devices.h>

static uint8_t AXIx_check_ID_regs(uint32_t base_addr)
{   
    rumboot_printf("Checking ID registers default value...\n");
    //peripheral_id//
      struct regpoker_checker check_array[] = {
      {" NIC400_PIDR4", REGPOKER_READ32, NIC400_PIDR4, 0x04, 0xff },
      { "NIC400_PIDR5", REGPOKER_READ32, NIC400_PIDR5, 0x00, 0xff },
      { "NIC400_PIDR6", REGPOKER_READ32, NIC400_PIDR6, 0x00, 0xff },
      { "NIC400_PIDR7", REGPOKER_READ32, NIC400_PIDR7, 0x00, 0xff },
      { "NIC400_PIDR0", REGPOKER_READ32, NIC400_PIDR0, 0x00, 0xff },
      { "NIC400_PIDR1", REGPOKER_READ32, NIC400_PIDR1, 0xB4, 0xff },
      { "NIC400_PIDR2", REGPOKER_READ32, NIC400_PIDR2, 0x5B, 0xff },
      { "NIC400_PIDR3", REGPOKER_READ32, NIC400_PIDR3, 0x00, 0xff },
      { "NIC400_CIDR0", REGPOKER_READ32, NIC400_CIDR0, 0x0D, 0xff },
      { "NIC400_CIDR1", REGPOKER_READ32, NIC400_CIDR1, 0xF0, 0xff },
      { "NIC400_CIDR2", REGPOKER_READ32, NIC400_CIDR2, 0x05, 0xff },
      { "NIC400_CIDR3", REGPOKER_READ32, NIC400_CIDR3, 0xB1, 0xff },
      {  }
    };
      if( rumboot_regpoker_check_array( check_array, base_addr ) == 0 ) {
              rumboot_printf( "OK\n" );
              return 0;
          }
          rumboot_printf( "ERROR\n" );
          return 1;
}

static int AXI_check_regs32(uint32_t base_addr)
{
    rumboot_printf("Checking ID registers random value...\n");

    struct regpoker_checker check_array[] = {
    { "NIC400_ARM_S_RQOS", REGPOKER_READ32, NIC400_ARM_S_RQOS, 0x0, 0xff},
    { "NIC400_ARM_S_WQOS", REGPOKER_READ32, NIC400_ARM_S_WQOS, 0x0, 0xff},
    { "NIC400_ARM_S_FN_MOD", REGPOKER_READ32, NIC400_ARM_S_FN_MOD, 0x0, 0xff},

    { "NIC400_PCIE_S_RQOS", REGPOKER_READ32, NIC400_PCIE_S_RQOS, 0x0, 0xff},
    { "NIC400_PCIE_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_WQOS, 0x0, 0xff},
    { "NIC400_PCIE_S_FN_MOD", REGPOKER_READ32, NIC400_PCIE_S_FN_MOD, 0x0, 0xff},

    { "NIC400_DMA0_S_RQOS", REGPOKER_READ32, NIC400_PCIE_S_RQOS, 0x0, 0xff},
    { "NIC400_DMA0_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_WQOS, 0x0, 0xff},
    { "NIC400_DMA0_S_FN_MOD", REGPOKER_READ32, NIC400_PCIE_S_FN_MOD, 0x0, 0xff},

    { "NIC400_DMA1_S_RQOS", REGPOKER_READ32, NIC400_PCIE_S_RQOS, 0x0, 0xff},
    { "NIC400_DMA1_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_WQOS, 0x0, 0xff},
    { "NIC400_DMA1_S_FN_MOD", REGPOKER_READ32, NIC400_PCIE_S_FN_MOD, 0x0, 0xff},

    { "NIC400_DMA2_S_RQOS", REGPOKER_READ32, NIC400_PCIE_S_RQOS, 0x0, 0xff},
    { "NIC400_DMA2_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_WQOS, 0x0, 0xff},
    { "NIC400_DMA2_S_FN_MOD", REGPOKER_READ32, NIC400_PCIE_S_FN_MOD, 0x0, 0xff},

    { "NIC400_DMA3_S_RQOS", REGPOKER_READ32, NIC400_PCIE_S_RQOS, 0x0, 0xff},
    { "NIC400_DMA3_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_WQOS, 0x0, 0xff},
    { "NIC400_DMA3_S_FN_MOD", REGPOKER_READ32, NIC400_PCIE_S_FN_MOD, 0x0, 0xff},
    {}
    };
    if( rumboot_regpoker_check_array( check_array, base_addr ) == 0 ) {
           rumboot_printf( "OK\n" );
           return 0;
       }

       rumboot_printf( "ERROR\n" );
       return 1;
}

static int AXI_check_regs64(uint32_t base_addr)
{
    rumboot_printf("Checking ID registers random value...\n");

    struct regpoker_checker check_array[] = {
    { "NIC400_ARM_S_RQOS", REGPOKER_READ32, NIC400_ARM_S_RQOS, 0x00, 0xff },
    { "NIC400_ARM_S_WQOS", REGPOKER_READ32, NIC400_ARM_S_WQOS, 0x00, 0xff },
    { "NIC400_ARM_S_FN_MOD", REGPOKER_READ32, NIC400_ARM_S_FN_MOD, 0x00, 0xff },
    { "NIC400_PCIE_S_RQOS", REGPOKER_READ32, NIC400_PCIE_S_RQOS, 0x00, 0xff },
    { "NIC400_PCIE_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_WQOS, 0x00, 0xff },
    { "NIC400_PCIE_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_FN_MOD, 0x00, 0xff },
    { "NIC400_DMA0_S_RQOS", REGPOKER_READ32, NIC400_DMA0_S_RQOS, 0x00, 0xff },
    { "NIC400_DMA0_S_WQOS", REGPOKER_READ32, NIC400_DMA0_S_WQOS, 0x00, 0xff },
    { "NIC400_DMA0_S_FN_MOD", REGPOKER_READ32, NIC400_DMA0_S_FN_MOD, 0x00, 0xff },
    { "NIC400_DMA1_S_RQOS", REGPOKER_READ32, NIC400_DMA1_S_RQOS, 0x00, 0xff },
    { "NIC400_DMA1_S_WQOS", REGPOKER_READ32, NIC400_DMA1_S_WQOS, 0x00, 0xff },
    { "NIC400_DMA1_S_FN_MOD", REGPOKER_READ32, NIC400_DMA1_S_FN_MOD, 0x00, 0xff },
    {}
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
   rumboot_printf("Check write/read register values\n");

   result = AXIx_check_ID_regs(AXI32HSIFS_CTRL_BASE) ||
           AXIx_check_ID_regs(AXI64HSIFS_CTRL_BASE) ||
           AXI_check_regs32(AXI32HSIFS_CTRL_BASE) ||
           AXI_check_regs64(AXI64HSIFS_CTRL_BASE);
   if(result)
   {
       rumboot_printf("Checked TEST_OK\n");
       return 0;
   }
       return result;
}
