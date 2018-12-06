 
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/common_macros/common_macros.h>
#include <rumboot/regpoker.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/oi10/platform/regs/regs_sctl.h>

static uint8_t dcr_check_ctrl_regs_ro(uint32_t base_addr)
{   
    rumboot_printf("Checking sctl registers default value...\n");

     struct regpoker_checker check_array[] = {
     { "KMBIST_CHAIN_SF_i", REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0 << 6, 0b0, 0b1 },
     { "KMBIST_CHAIN_SF_i", REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0 << 5, 0b0, 0b1 },
     { "PPC_SYS_CONF", REGPOKER_READ_DCR, PPC_SYS_CONF << 3 , 0b0, 0b1 },
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


static int dcr_check_ctrl_regs_rw(uint32_t base_addr)
{
    rumboot_printf("Checking sctl registers for rw value...\n");

    struct regpoker_checker check_array[] = {
    { "KMBIST_CHAIN_SF_i", REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0 << 3, 0b0, 0b1 },
    { "KMBIST_CHAIN_SF_i", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_0 << 3, 0b0, 0b1 },
    { "PPC_SYS_CONF", REGPOKER_READ_DCR, PPC_SYS_CONF << 4, 0b0, 0b1},
    { "PPC_SYS_CONF", REGPOKER_WRITE_DCR, PPC_SYS_CONF << 4, 0b1, 0b1},
    { "PPC_SYS_CONF", REGPOKER_READ_DCR, PPC_SYS_CONF << 5, 0b0, 0b1},
    { "PPC_SYS_CONF", REGPOKER_WRITE_DCR, PPC_SYS_CONF << 5, 0b1, 0b1},
    };
    if( rumboot_regpoker_check_array( check_array, base_addr ) == 0 ) {
           rumboot_printf( "OK\n" );
           return 0;
       }
    rumboot_printf( "ERROR\n" );
    return 1;
}
/*
static int AXI_check_regs64(uint32_t base_addr)
{
    rumboot_printf("Checking ID registers random value...\n");
    rumboot_printf("Checking ID registers random value regs64...\n");

    struct regpoker_checker check_array[] = {
    { "NIC400_SW_0_S_RQOS", REGPOKER_READ32, NIC400_SW_0_S_RQOS, 0x0, 0b1111 },
    { "NIC400_SW_0_S_RQOS", REGPOKER_WRITE32, NIC400_SW_0_S_RQOS, 0x0, 0b1111 },

    { "NIC400_SW_0_S_WQOS", REGPOKER_READ32, NIC400_SW_0_S_WQOS, 0x0, 0b1111 },
    { "NIC400_SW_0_S_WQOS", REGPOKER_WRITE32, NIC400_SW_0_S_WQOS, 0x0, 0b1111 },

    { "NIC400_SW_0_S_FN_MOD", REGPOKER_READ32, NIC400_SW_0_S_FN_MOD, 0x0, 0b11
},
    { "NIC400_SW_0_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_0_S_FN_MOD, 0x0, 0b11
},

    { "NIC400_SW_1_S_RQOS", REGPOKER_READ32, NIC400_SW_1_S_RQOS, 0x0, 0b1111 },
    { "NIC400_SW_1_S_RQOS", REGPOKER_WRITE32, NIC400_SW_1_S_RQOS, 0x0, 0b1111 },

    { "NIC400_SW_1_S_WQOS", REGPOKER_READ32, NIC400_SW_1_S_WQOS, 0x0, 0b1111 },
    { "NIC400_SW_1_S_WQOS", REGPOKER_WRITE32, NIC400_SW_1_S_WQOS, 0x0, 0b1111 },

    { "NIC400_SW_1_S_FN_MOD", REGPOKER_READ32, NIC400_SW_1_S_FN_MOD, 0x0, 0b11
},
    { "NIC400_SW_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_1_S_FN_MOD, 0x0, 0b11
},

    { "NIC400_SW_2_1_S_RQOS", REGPOKER_READ32, NIC400_SW_2_1_S_RQOS, 0x0, 0b1111
},
    { "NIC400_SW_2_1_S_RQOS", REGPOKER_WRITE32, NIC400_SW_2_1_S_RQOS, 0x0,
0b1111 },

    { "NIC400_SW_2_1_S_WQOS", REGPOKER_READ32, NIC400_SW_2_1_S_WQOS, 0x0, 0b1111
},
    { "NIC400_SW_2_1_S_WQOS", REGPOKER_WRITE32, NIC400_SW_2_1_S_WQOS, 0x0,
0b1111 },

    { "NIC400_SW_2_1_S_FN_MOD", REGPOKER_READ32, NIC400_SW_2_1_S_FN_MOD, 0x0,
0b11 },
    { "NIC400_SW_2_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_2_1_S_FN_MOD, 0x0,
0b11 },

    { "NIC400_SW_3_S_RQOS", REGPOKER_READ32, NIC400_SW_3_S_RQOS, 0x0, 0b1111 },
    { "NIC400_SW_3_S_RQOS", REGPOKER_WRITE32, NIC400_SW_3_S_RQOS, 0x0, 0b1111 },

    { "NIC400_SW_3_S_WQOS", REGPOKER_READ32, NIC400_SW_3_S_WQOS, 0x0, 0b1111 },
    { "NIC400_SW_3_S_WQOS", REGPOKER_WRITE32, NIC400_SW_3_S_WQOS, 0x0, 0b1111 },

    { "NIC400_SW_3_S_FN_MOD", REGPOKER_READ32, NIC400_SW_3_S_FN_MOD, 0x0, 0b11},
    { "NIC400_SW_3_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_3_S_FN_MOD, 0x0, 0b11
},
    {}
    };

    if( rumboot_regpoker_check_array( check_array, base_addr ) == 0 ) {
            rumboot_printf( "OK\n" );
            return 0;
        }
    rumboot_printf( "ERROR\n" );
    return 1;
}
*/
int main(void)
{
   register int result = 0;
   rumboot_printf("Check write/read ctrl register values\n");

   result = dcr_check_ctrl_regs_ro(DCR_SCTL_BASE) ||
   dcr_check_ctrl_regs_rw(DCR_SCTL_BASE);

       //AXIx_check_ID_regs(DCR_CTRL_BASE) ||
       //      AXIx_check_ID_regs(AXI64HSIFS_CTRL_BASE) ||
       //      AXI_check_regs32(AXI32HSIFS_CTRL_BASE) ||
       //      AXI_check_regs64(AXI64HSIFS_CTRL_BASE);

   if(!result)
   {
       rumboot_printf("Checked TEST_OK\n");
       return 0;
   }

   rumboot_printf("Checked TEST_ERROR\n");
   return result;
}
