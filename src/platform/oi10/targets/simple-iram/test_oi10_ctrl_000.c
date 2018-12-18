 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <rumboot/testsuite.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>
//#include <rumboot/platform.h>
#include <platform/oi10/platform/regs/regs_sctl.h>
#include <platform/oi10/platform/devices.h>
#include <platform/regs/regs_sctl.h>
//#include <rumboot/include/regs/regs_sctl.h>

#ifdef CHECK_REGS
static uint8_t check_ctrl_regs_ro(uint32_t base_addr)
{   
    rumboot_printf("Checking sctl registers default value...\n");
    struct regpoker_checker check_array[] = {
     { "PPC_SYS_CONF", REGPOKER_READ_DCR, PPC_SYS_CONF, 0x0, 0x1 },
     { "KMBIST_CHAIN_SF_i", REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0, 0b00000, 0b11111 },
     { "KMBIST_CHAIN_SF_i", REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0, 0b000000, 0b111111 },
     { "PPC_SYS_CONF", REGPOKER_READ_DCR, PPC_SYS_CONF, 0b000, 0b111 },
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
    { "PPC_SYS_CONF", REGPOKER_WRITE_DCR, PPC_SYS_CONF, 0b00000, 0b11000 },
    { "PPC_SYS_CONF", REGPOKER_READ_DCR, PPC_SYS_CONF, 0b1000, 0b1111 },
    { "PPC_SYS_CONF", REGPOKER_READ_DCR, PPC_SYS_CONF << 4, 0b000, 0b111 },
    { "PPC_SYS_CONF", REGPOKER_WRITE_DCR, PPC_SYS_CONF << 4, 0b1, 0b1},
    { "PPC_SYS_CONF", REGPOKER_READ_DCR, PPC_SYS_CONF << 5, 0b0, 0b1 },
    { "PPC_SYS_CONF", REGPOKER_WRITE_DCR, PPC_SYS_CONF << 5, 0b1, 0b1},
    };
    if( rumboot_regpoker_check_array( check_array, base_addr ) == 0 ) {
           rumboot_printf( "OK\n" );
           return 0;
       }
    rumboot_printf( "ERROR\n" );
    return 1;
}
#endif

/*
static void handler0( int irq, void *arg )
{
    struct ctrl_instance_i *a = (struct ctrl_instance_i *) arg;
    a->ctrl_irq = a->ctrl_irq + 1;
    rumboot_printf( "IRQ 0 arrived  \n" );
    rumboot_printf( "ctrl_%d PROGRAM PPC INT # %d  \n", a->ctrl_index, a->ctrl_irq );
    //sp805_clrint( a->base_addr);
}
*/

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

TEST_SUITE_BEGIN(sctl_testlist, "SCTL TEST")
#ifdef CHECK_REGS
//TEST_ENTRY("sctl ro", check_ctrl_regs_ro, (uint32_t, &in[0])),
//TEST_ENTRY("sctl rw", check_ctrl_regs_rw, (uint32_t, &in[0])),
TEST_ENTRY("sctl ro", check_ctrl_regs_ro, (uint32_t, DCR_SCTL_BASE)),
TEST_ENTRY("sctl rw", check_ctrl_regs_rw, (uint32_t, DCR_SCTL_BASE)),
#endif
TEST_SUITE_END();

int main(void)
{

   register int result;
   rumboot_printf("Check write/read ctrl register values\n");
/*
   result = check_ctrl_regs_ro(DCR_SCTL_BASE) ||
   check_ctrl_regs_rw(DCR_SCTL_BASE);

   if(!result)
   {
       rumboot_printf("Checked TEST_OK\n");
       return 0;
   }

   rumboot_printf("Checked TEST_ERROR\n");
  // return result;
*/

  //  register uint32_t result;
  //  rumboot_printf( "SCTL test START\n" );

    /* struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_irq_cli();
    rumboot_irq_set_handler( tbl, RUMBOOT_IRQ_PROGRAM, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler0, &in[0]);
    // Activate the table //
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( RUMBOOT_IRQ_PROGRAM);
    rumboot_irq_sei();
    */

    result = test_suite_run(NULL, &sctl_testlist);

    return (!result) ? rumboot_printf("CHECKED TEST_OK\n"), result: rumboot_printf("CHECKED TEST_ERROR\n"), result;
}
