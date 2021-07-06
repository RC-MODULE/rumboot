 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>
#include <platform/o32t/platform/devices.h>
#include <platform/regs/sctl.h>
#include <rumboot/testsuite.h>
#include <rumboot/rumboot.h>



static bool
check_sctl_regs_ro(uint32_t base_addr)
{
    rumboot_printf("Checking sctl registers default value...\n");
    struct regpoker_checker check_array[] =
     {
     { "KMBIST_CHAIN_SF_0",     REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0,       0,                          SCTL_KMBIST_CHAIN_SF_mask },
     { "KMBIST_CHAIN_SF_1",     REGPOKER_READ_DCR, KMBIST_CHAIN_SF_1,       0,                          SCTL_KMBIST_CHAIN_SF_mask },
     { "KMBIST_CHAIN_SF_2",     REGPOKER_READ_DCR, KMBIST_CHAIN_SF_2,       0,                          SCTL_KMBIST_CHAIN_SF_mask },
     { "KMBIST_CHAIN_SF_3",     REGPOKER_READ_DCR, KMBIST_CHAIN_SF_3,       0,                          SCTL_KMBIST_CHAIN_SF_mask },
     { "KMBIST_CHAIN_SF_4",     REGPOKER_READ_DCR, KMBIST_CHAIN_SF_4,       0,                          SCTL_KMBIST_CHAIN_SF_mask },
     { "KMBIST_CHAIN_SF_5",     REGPOKER_READ_DCR, KMBIST_CHAIN_SF_5,       0,                          SCTL_KMBIST_CHAIN_SF_mask },
     { "KMBIST_CHAIN_SF_6",     REGPOKER_READ_DCR, KMBIST_CHAIN_SF_6,       0,                          SCTL_KMBIST_CHAIN_SF_mask },
     { "KMBIST_CHAIN_SF_7",     REGPOKER_READ_DCR, KMBIST_CHAIN_SF_7,       0,                          SCTL_KMBIST_CHAIN_SF_mask },
     { "KMBIST_CHAIN_SF_8",     REGPOKER_READ_DCR, KMBIST_CHAIN_SF_8,       0,                          SCTL_KMBIST_CHAIN_SF_mask },
     { "KMBIST_CHAIN_SF_9",     REGPOKER_READ_DCR, KMBIST_CHAIN_SF_9,       0,                          SCTL_KMBIST_CHAIN_SF_mask },
     { "KMBIST_CHAIN_SF_10",    REGPOKER_READ_DCR, KMBIST_CHAIN_SF_10,      0,                          SCTL_KMBIST_CHAIN_SF_mask },

     { "PPC_SYS_CONF",          REGPOKER_READ_DCR, SCTL_PPC_SYS_CONF,       SCTL_PPC_SYS_CONF_dflt,     SCTL_PPC_SYS_CONF_mask },
     { "SCTL_PPC_SLP_CTRL",     REGPOKER_READ_DCR, SCTL_PPC_SLP_CTRL,       0,                          SCTL_PPC_SLP_CTRL_mask },
     { "SCTL_PPC_PMU_CTRL",     REGPOKER_READ_DCR, SCTL_PPC_PMU_CTRL,       0,                          SCTL_PPC_PMU_CTRL_mask },
     
     //{ "SCTL_IFSYS_ETH_CTRL",   REGPOKER_READ_DCR, SCTL_IFSYS_ETH_CTRL,     SCTL_IFSYS_ETH_CTRL_dflt,   SCTL_IFSYS_ETH_CTRL_mask },
     { "SCTL_IFSYS_ETH_HPROT",  REGPOKER_READ_DCR, SCTL_IFSYS_ETH_HPROT,    SCTL_IFSYS_ETH_HPROT_dflt,  SCTL_IFSYS_ETH_HPROT_mask },
     {  }
        };
      if( rumboot_regpoker_check_array( check_array, base_addr ) == 0 )
      {
          rumboot_printf( "OK\n" );
          return true;
      }
      rumboot_printf( "ERROR\n" );
      return false;
}

static bool
check_sctl_regs_rw(uint32_t base_addr)
{
    rumboot_printf("Checking sctl registers for rw value...\n");
    struct regpoker_checker check_array[] = {
    { "KMBIST_CHAIN_SF_0",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_0,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "KMBIST_CHAIN_SF_1",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_1,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "KMBIST_CHAIN_SF_2",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_2,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "KMBIST_CHAIN_SF_3",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_3,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "KMBIST_CHAIN_SF_4",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_4,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "KMBIST_CHAIN_SF_5",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_5,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "KMBIST_CHAIN_SF_6",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_6,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "KMBIST_CHAIN_SF_7",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_7,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "KMBIST_CHAIN_SF_8",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_8,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "KMBIST_CHAIN_SF_9",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_9,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "KMBIST_CHAIN_SF_10",  REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_10,      0,  SCTL_KMBIST_CHAIN_SF_TM_mask },
    { "PPC_SLP_CTRL",       REGPOKER_WRITE_DCR, SCTL_PPC_SLP_CTRL,      0,  SCTL_SLP_CTRL_TYPE_mask },
    { "PPC_PMU_CTRL",       REGPOKER_WRITE_DCR, SCTL_PPC_PMU_CTRL,      0,  SCTL_PPC_PMU_CTRL_mask },
    
    { "IFSYS_ETH_HPROT",    REGPOKER_WRITE_DCR, SCTL_IFSYS_ETH_HPROT,   SCTL_IFSYS_ETH_HPROT_dflt, SCTL_IFSYS_ETH_HPROT_mask },
    { "IFSYS_ETH_HPROT",    REGPOKER_WRITE_DCR, SCTL_IFSYS_ETH_HPROT,   0xFFFFFFFF, 0 },
    { "IFSYS_ETH_HPROT",    REGPOKER_READ_DCR,  SCTL_IFSYS_ETH_HPROT,   0xFFFFFFFF, SCTL_IFSYS_ETH_HPROT_mask },
    { "IFSYS_ETH_HPROT",    REGPOKER_WRITE_DCR, SCTL_IFSYS_ETH_HPROT,   0x33333333, 0 },
    { "IFSYS_ETH_HPROT",    REGPOKER_READ_DCR,  SCTL_IFSYS_ETH_HPROT,   0x33333333, SCTL_IFSYS_ETH_HPROT_mask },
    {  }
    };
    if( rumboot_regpoker_check_array( check_array, base_addr ) == 0 ) {
           rumboot_printf( "OK\n" );
           return true;
       }
    rumboot_printf( "ERROR\n" );
    return false;
}

// This is a sepparate check, because reg PPC_SYS_CONF
// cant be checked with rumboot_regpoker_check_array
// because there are forbidden values of bits [4:3]
static bool
check_ppc_sys_conf_reg_rw(uint32_t base_addr)
{
    int fails = 0;
    int i; 
    uint32_t const address = base_addr + SCTL_PPC_SYS_CONF; 
    uint32_t mask = 0x19;
    if( mask != 0 ) { 

        uint32_t v, readback; 
        
        v = 0x9;
        dcr_write(address, v ); 
        readback = dcr_read( address ); 
        if( ( readback & mask ) != v ) { 
            fails++; 
            rumboot_printf( "Write register %s (0x%x): wrote: 0x%x got: 0x%x mask: 0x%xn", "PPC_SYS_CONF", address, v, readback, mask ); 
        } 
        
        v = 0x18;
        dcr_write(address, v ); 
        readback = dcr_read( address ); 
        if( ( readback & mask ) != v ) { 
            fails++; 
            rumboot_printf( "Write register %s (0x%x): wrote: 0x%x got: 0x%x mask: 0x%xn", "PPC_SYS_CONF", address, v, readback, mask ); 
        } 
    } 
    dcr_write( address , (uint32_t)SCTL_PPC_SYS_CONF_dflt);     
    
    if( fails == 0 ) {
           rumboot_printf( "OK\n" );
           return true;
       }
    rumboot_printf( "ERROR\n" );
    return false;
}



TEST_SUITE_BEGIN(sctl_testlist, "SCTL TEST")
TEST_ENTRY("SCTL reg read default", check_sctl_regs_ro, DCR_SCTL_BASE),
TEST_ENTRY("SCTL reg rw check", check_sctl_regs_rw, DCR_SCTL_BASE),
TEST_ENTRY("SCTL ppc_sys_conf reg rw check", check_ppc_sys_conf_reg_rw, DCR_SCTL_BASE),

TEST_SUITE_END();

int main(void)
{
   register int result;

   result = test_suite_run(NULL,&sctl_testlist);
   return result;
}
