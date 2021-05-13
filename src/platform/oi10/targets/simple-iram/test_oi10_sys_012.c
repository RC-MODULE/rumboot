/* -*- rumboot-test-labels: oi10only -*- */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <arch/common_macros.h>
#include <rumboot/regpoker.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <platform/regs/regs_nic400.h>
#include <platform/devices.h>

static uint8_t AXIx_check_ID_regs(uint32_t base_addr)
{   
    rumboot_printf("Checking ID registers default value...\n");
      struct regpoker_checker check_array[] = {
      {" NIC400_PERIPHERAL_ID4", REGPOKER_READ32, NIC400_PERIPHERAL_ID4, 0x04, 0xff },
      { "NIC400_PERIPHERAL_ID5", REGPOKER_READ32, NIC400_PERIPHERAL_ID5, 0x00, 0xff },
      { "NIC400_PERIPHERAL_ID6", REGPOKER_READ32, NIC400_PERIPHERAL_ID6, 0x00, 0xff },
      { "NIC400_PERIPHERAL_ID7", REGPOKER_READ32, NIC400_PERIPHERAL_ID7, 0x00, 0xff },
      { "NIC400_PERIPHERAL_ID0", REGPOKER_READ32, NIC400_PERIPHERAL_ID0, 0x00, 0xff },
      { "NIC400_PERIPHERAL_ID1", REGPOKER_READ32, NIC400_PERIPHERAL_ID1, 0xB4, 0xff },
      { "NIC400_PERIPHERAL_ID2", REGPOKER_READ32, NIC400_PERIPHERAL_ID2, 0x5B, 0xff },
      { "NIC400_PERIPHERAL_ID3", REGPOKER_READ32, NIC400_PERIPHERAL_ID3, 0x00, 0xff },
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
    rumboot_printf("Checking ID registers random value regs32...\n");

    struct regpoker_checker check_array[] = {
    { "NIC400_CPU_S_RQOS", REGPOKER_READ32, NIC400_CPU_S_RQOS, 0x0, 0b1111 },
    { "NIC400_CPU_S_RQOS", REGPOKER_WRITE32, NIC400_CPU_S_RQOS, 0x0, 0b1111 },

    { "NIC400_CPU_S_WQOS", REGPOKER_READ32, NIC400_CPU_S_WQOS, 0x0, 0b1111 },
    { "NIC400_CPU_S_WQOS", REGPOKER_WRITE32, NIC400_CPU_S_RQOS, 0x0, 0b1111 },

    { "NIC400_CPU_S_FN_MOD", REGPOKER_READ32, NIC400_CPU_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_CPU_S_FN_MOD", REGPOKER_WRITE32, NIC400_CPU_S_FN_MOD, 0x0, 0b11 },

    { "NIC400_GSPI_1_S_RQOS", REGPOKER_READ32, NIC400_GSPI_1_S_RQOS, 0x0, 0b1111 },
    { "NIC400_GSPI_1_S_RQOS", REGPOKER_WRITE32, NIC400_GSPI_1_S_RQOS, 0x0,0b1111 },

    { "NIC400_GSPI_1_S_WQOS", REGPOKER_READ32, NIC400_GSPI_1_S_WQOS, 0x0, 0b1111 },
    { "NIC400_GSPI_1_S_WQOS", REGPOKER_WRITE32, NIC400_GSPI_1_S_WQOS, 0x0, 0b1111 },

    { "NIC400_GSPI_1_S_FN_MOD", REGPOKER_READ32, NIC400_GSPI_1_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_GSPI_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_GSPI_1_S_FN_MOD, 0x0, 0b11 },

    { "NIC400_ETH_1_S_RQOS", REGPOKER_READ32, NIC400_ETH_1_S_RQOS, 0x0, 0b1111 },
    { "NIC400_ETH_1_S_RQOS", REGPOKER_WRITE32, NIC400_ETH_1_S_RQOS, 0x0, 0b1111 },

    { "NIC400_ETH_1_S_WQOS", REGPOKER_READ32, NIC400_ETH_1_S_WQOS, 0x0, 0b1111 },
    { "NIC400_ETH_1_S_WQOS", REGPOKER_WRITE32, NIC400_ETH_1_S_WQOS, 0x0, 0b1111 },

    { "NIC400_ETH_1_S_FN_MOD", REGPOKER_READ32, NIC400_ETH_1_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_ETH_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_ETH_1_S_FN_MOD, 0x0, 0b11 },

    { "NIC400_SW_0_3_S_RQOS", REGPOKER_READ32, NIC400_SW_0_3_S_RQOS, 0x4, 0b1111 },
    { "NIC400_SW_0_3_S_RQOS", REGPOKER_WRITE32, NIC400_SW_0_3_S_RQOS, 0x4, 0b1111 },

    { "NIC400_SW_0_3_S_WQOS", REGPOKER_READ32, NIC400_SW_0_3_S_WQOS, 0x4, 0b1111 },
    { "NIC400_SW_0_3_S_WQOS", REGPOKER_WRITE32, NIC400_SW_0_3_S_WQOS, 0x4, 0b1111 },

    { "NIC400_SW_0_3_S_FN_MOD", REGPOKER_READ32, NIC400_SW_0_3_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_SW_0_3_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_0_3_S_FN_MOD, 0x0, 0b11 },

    { "NIC400_GSPI_0_S_RQOS", REGPOKER_READ32, NIC400_GSPI_0_S_RQOS, 0x0, 0b1111 },
    { "NIC400_GSPI_0_S_RQOS", REGPOKER_WRITE32, NIC400_GSPI_0_S_RQOS, 0x0, 0b1111 },

    { "NIC400_GSPI_0_S_WQOS", REGPOKER_READ32, NIC400_GSPI_0_S_WQOS, 0x0, 0b1111 },
    { "NIC400_GSPI_0_S_WQOS", REGPOKER_WRITE32, NIC400_GSPI_0_S_WQOS, 0x0, 0b1111 },

    { "NIC400_GSPI_0_S_FN_MOD", REGPOKER_READ32, NIC400_GSPI_0_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_GSPI_0_S_FN_MOD", REGPOKER_WRITE32, NIC400_GSPI_0_S_FN_MOD, 0x0, 0b11 },

    { "NIC400_ETH_0_S_RQOS", REGPOKER_READ32, NIC400_ETH_0_S_RQOS, 0x0, 0b1111 },
    { "NIC400_ETH_0_S_RQOS", REGPOKER_WRITE32, NIC400_ETH_0_S_RQOS, 0x0, 0b1111 },

    { "NIC400_ETH_0_S_WQOS", REGPOKER_READ32, NIC400_ETH_0_S_WQOS, 0x0, 0b1111 },
    { "NIC400_ETH_0_S_WQOS", REGPOKER_WRITE32, NIC400_ETH_0_S_WQOS, 0x0, 0b1111 },

    { "NIC400_ETH_0_S_FN_MOD", REGPOKER_READ32, NIC400_ETH_0_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_ETH_0_S_FN_MOD", REGPOKER_WRITE32, NIC400_ETH_0_S_FN_MOD, 0x0, 0b11 },

    { "NIC400_MKIO_0_S_RQOS", REGPOKER_READ32, NIC400_MKIO_0_S_RQOS, 0x0, 0b1111 },
    { "NIC400_MKIO_0_S_RQOS", REGPOKER_WRITE32, NIC400_MKIO_0_S_RQOS, 0x0, 0b1111 },

    { "NIC400_MKIO_0_S_WQOS", REGPOKER_READ32, NIC400_MKIO_0_S_WQOS, 0x0, 0b1111 },
    { "NIC400_MKIO_0_S_WQOS", REGPOKER_WRITE32, NIC400_MKIO_0_S_WQOS, 0x0, 0b1111 },

    { "NIC400_MKIO_0_S_FN_MOD", REGPOKER_READ32, NIC400_MKIO_0_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_MKIO_0_S_FN_MOD", REGPOKER_WRITE32, NIC400_MKIO_0_S_FN_MOD, 0x0, 0b11 },

    { "NIC400_MKIO_1_S_RQOS", REGPOKER_READ32, NIC400_MKIO_1_S_RQOS, 0x0, 0b1111 },
    { "NIC400_MKIO_1_S_RQOS", REGPOKER_WRITE32, NIC400_MKIO_1_S_RQOS, 0x0, 0b1111 },

    { "NIC400_MKIO_1_S_WQOS", REGPOKER_READ32, NIC400_MKIO_1_S_WQOS, 0x0, 0b1111 },
    { "NIC400_MKIO_1_S_WQOS", REGPOKER_WRITE32, NIC400_MKIO_1_S_WQOS, 0x0, 0b1111 },

    { "NIC400_MKIO_1_S_FN_MOD", REGPOKER_READ32, NIC400_MKIO_1_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_MKIO_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_MKIO_1_S_FN_MOD, 0x0, 0b11 },

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
    rumboot_printf("Checking ID registers random value regs64...\n");

    struct regpoker_checker check_array[] = {
    { "NIC400_SW_0_S_RQOS", REGPOKER_READ32, NIC400_SW_0_S_RQOS, 0x0, 0b1111 },
    { "NIC400_SW_0_S_RQOS", REGPOKER_WRITE32, NIC400_SW_0_S_RQOS, 0x0, 0b1111 },

    { "NIC400_SW_0_S_WQOS", REGPOKER_READ32, NIC400_SW_0_S_WQOS, 0x0, 0b1111 },
    { "NIC400_SW_0_S_WQOS", REGPOKER_WRITE32, NIC400_SW_0_S_WQOS, 0x0, 0b1111 },

    { "NIC400_SW_0_S_FN_MOD", REGPOKER_READ32, NIC400_SW_0_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_SW_0_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_0_S_FN_MOD, 0x0, 0b11 },

    { "NIC400_SW_1_S_RQOS", REGPOKER_READ32, NIC400_SW_1_S_RQOS, 0x0, 0b1111 },
    { "NIC400_SW_1_S_RQOS", REGPOKER_WRITE32, NIC400_SW_1_S_RQOS, 0x0, 0b1111 },

    { "NIC400_SW_1_S_WQOS", REGPOKER_READ32, NIC400_SW_1_S_WQOS, 0x0, 0b1111 },
    { "NIC400_SW_1_S_WQOS", REGPOKER_WRITE32, NIC400_SW_1_S_WQOS, 0x0, 0b1111 },

    { "NIC400_SW_1_S_FN_MOD", REGPOKER_READ32, NIC400_SW_1_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_SW_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_1_S_FN_MOD, 0x0, 0b11 },

    { "NIC400_SW_2_1_S_RQOS", REGPOKER_READ32, NIC400_SW_2_1_S_RQOS, 0x0, 0b1111 },
    { "NIC400_SW_2_1_S_RQOS", REGPOKER_WRITE32, NIC400_SW_2_1_S_RQOS, 0x0, 0b1111 },

    { "NIC400_SW_2_1_S_WQOS", REGPOKER_READ32, NIC400_SW_2_1_S_WQOS, 0x0, 0b1111 },
    { "NIC400_SW_2_1_S_WQOS", REGPOKER_WRITE32, NIC400_SW_2_1_S_WQOS, 0x0, 0b1111 },

    { "NIC400_SW_2_1_S_FN_MOD", REGPOKER_READ32, NIC400_SW_2_1_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_SW_2_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_2_1_S_FN_MOD, 0x0, 0b11 },

    { "NIC400_SW_3_S_RQOS", REGPOKER_READ32, NIC400_SW_3_S_RQOS, 0x0, 0b1111 },
    { "NIC400_SW_3_S_RQOS", REGPOKER_WRITE32, NIC400_SW_3_S_RQOS, 0x0, 0b1111 },

    { "NIC400_SW_3_S_WQOS", REGPOKER_READ32, NIC400_SW_3_S_WQOS, 0x0, 0b1111 },
    { "NIC400_SW_3_S_WQOS", REGPOKER_WRITE32, NIC400_SW_3_S_WQOS, 0x0, 0b1111 },

    { "NIC400_SW_3_S_FN_MOD", REGPOKER_READ32, NIC400_SW_3_S_FN_MOD, 0x0, 0b11},
    { "NIC400_SW_3_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_3_S_FN_MOD, 0x0, 0b11 },
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
   if(!result)
   {
       rumboot_printf("Checked TEST_OK\n");
       return 0;
   }
   rumboot_printf("Checked TEST_ERROR\n");
   return result;
}
