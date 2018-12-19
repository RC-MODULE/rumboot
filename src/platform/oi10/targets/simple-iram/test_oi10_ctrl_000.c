 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <rumboot/testsuite.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>
#include <platform/oi10/platform/devices.h>
#include <platform/regs/regs_sctl.h>

#ifdef CHECK_REGS
static uint8_t check_ctrl_regs_ro(uint32_t base_addr)
{   
    rumboot_printf("Checking sctl registers default value...\n");
    struct regpoker_checker check_array[] =
    {
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
    { "PPC_SYS_CONF", REGPOKER_READ_DCR,0b0000 & (4 << PPC_SYS_CONF), 0b0, 0b1 },
    { "PPC_SYS_CONF", REGPOKER_WRITE_DCR, 0b0000 & (4 << PPC_SYS_CONF), 0b0, 0b1},
    { "PPC_SYS_CONF", REGPOKER_READ_DCR, 0b00000 & (5 << PPC_SYS_CONF), 0b0, 0b1 },
    { "PPC_SYS_CONF", REGPOKER_WRITE_DCR, 0b00000 &(5 << PPC_SYS_CONF), 0b0, 0b1},

    { "PPC_SLP_CTRL", REGPOKER_READ_DCR, 0b00 & (2 << PPC_SLP_CTRL), 0b00, 0b11 },
    { "PPC_SLP_CTRL", REGPOKER_WRITE_DCR, 0b00 & (2 << PPC_SLP_CTRL), 0b00, 0b11},

    { "PPC_SLP_CTRL", REGPOKER_READ_DCR, 0b0 & (1 << PPC_SLP_CTRL), 0b00, 0b11 },
    { "PPC_SLP_CTRL", REGPOKER_WRITE_DCR, 0b0 & (1 << PPC_SLP_CTRL), 0b00, 0b11},

//   { "PPC_SLP_CTRL", REGPOKER_READ_DCR, PPC_SLP_CTRL << 1, 0b00, 0b11 },
//    { "PPC_SLP_CTRL", REGPOKER_WRITE_DCR, PPC_SLP_CTRL << 1, 0b11, 0b11},

    { "PPC_PMU_CTRL", REGPOKER_READ_DCR, 0b0000 & (4 << PPC_SLP_CTRL), 0b0, 0b1 },
    { "PPC_PMU_CTRL", REGPOKER_WRITE_DCR, 0b0000 & (4 << PPC_SLP_CTRL), 0b1, 0b1 },

    { "PPC_PMU_CTRL", REGPOKER_READ_DCR, 0b000 & (3 << PPC_SLP_CTRL), 0b0, 0b1 },
    { "PPC_PMU_CTRL", REGPOKER_WRITE_DCR, 0b000 & (3 << PPC_SLP_CTRL), 0b1, 0b1 },

    { "PPC_PMU_CTRL", REGPOKER_READ_DCR, 0b00 & (2 << PPC_SLP_CTRL), 0b0, 0b1 },
    { "PPC_PMU_CTRL", REGPOKER_WRITE_DCR, 0b00 & (2 << PPC_SLP_CTRL), 0b1, 0b1 },

    { "PPC_PMU_CTRL", REGPOKER_READ_DCR, 0b0 & (1 << PPC_SLP_CTRL), 0b0, 0b1 },
    { "PPC_PMU_CTRL", REGPOKER_WRITE_DCR, 0b0 & (1 << PPC_SLP_CTRL), 0b1, 0b1 },

    { "PPC_PMU_CTRL", REGPOKER_READ_DCR, 0 << PPC_SLP_CTRL, 0b0, 0b1 },
    { "PPC_PMU_CTRL", REGPOKER_WRITE_DCR, 0 << PPC_SLP_CTRL, 0b0, 0b1},

    };
    if( rumboot_regpoker_check_array( check_array, base_addr ) == 0 ) {
           rumboot_printf( "OK\n" );
           return 0;
       }
    rumboot_printf( "ERROR\n" );
    return 1;
}

#endif

TEST_SUITE_BEGIN(sctl_testlist, "SCTL TEST")
#ifdef CHECK_REGS
TEST_ENTRY("sctl ro", check_ctrl_regs_ro, (uint32_t, DCR_SCTL_BASE)),
TEST_ENTRY("sctl rw", check_ctrl_regs_rw, (uint32_t, DCR_SCTL_BASE)),
#endif
TEST_SUITE_END();

int main(void)
{

   register int result;
   rumboot_printf("Check write/read ctrl register values\n");
   result = test_suite_run(NULL, &sctl_testlist);
   return (!result) ? rumboot_printf("CHECKED TEST_OK\n"), result: rumboot_printf("CHECKED TEST_ERROR\n"), result;
}
