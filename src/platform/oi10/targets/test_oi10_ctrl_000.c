 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>
#include <platform/oi10/platform/devices.h>
#include <platform/regs/sctl.h>
#include <rumboot/testsuite.h>
#include <rumboot/rumboot.h>
#include <platform/test_event_c.h>
#include <platform/test_assert.h>

DECLARE_CONST(TEST_OI10_CTRL_000_CHECK_50MHz, TEST_EVENT_CODE_MIN + 0)
DECLARE_CONST(TEST_OI10_CTRL_000_CHECK_100MHz, TEST_EVENT_CODE_MIN + 1)
DECLARE_CONST(TEST_OI10_CTRL_000_CHECK_INJECT_ERR, TEST_EVENT_CODE_MIN + 2)

DECLARE_CONST(TEST_OI10_CTRL_000_MBIST_TIMEOUT, 0xffffffff)

static bool check_sctl_regs_ro(uint32_t base_addr)
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
     { "PPC_SYS_CONF",          REGPOKER_READ_DCR, SCTL_PPC_SYS_CONF,       SCTL_PPC_SYS_CONF_dflt,     SCTL_PPC_SYS_CONF_mask },
     { "SCTL_PPC_SLP_CTRL",     REGPOKER_READ_DCR, SCTL_PPC_SLP_CTRL,       0,                          SCTL_PPC_SLP_CTRL_mask },
     { "SCTL_PPC_PMU_CTRL",     REGPOKER_READ_DCR, SCTL_PPC_PMU_CTRL,       0,                          SCTL_PPC_PMU_CTRL_mask },
     { "SCTL_IFSYS_ETH_CTRL",   REGPOKER_READ_DCR, SCTL_IFSYS_ETH_CTRL,     SCTL_IFSYS_ETH_CTRL_dflt,   SCTL_IFSYS_ETH_CTRL_mask },
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

static bool check_sctl_regs_rw(uint32_t base_addr)
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
    { "PPC_SLP_CTRL",       REGPOKER_WRITE_DCR, SCTL_PPC_SLP_CTRL,      0,  SCTL_SLP_CTRL_TYPE_mask },
    { "PPC_PMU_CTRL",       REGPOKER_WRITE_DCR, SCTL_PPC_PMU_CTRL,      0,  SCTL_PPC_PMU_CTRL_mask },
    { "IFSYS_ETH_CTRL",     REGPOKER_WRITE_DCR, SCTL_IFSYS_ETH_CTRL,    SCTL_IFSYS_ETH_CTRL_dflt,
                                                                            SCTL_IFSYS_ETH_CTRL_mask },
    { "IFSYS_ETH_HPROT",    REGPOKER_WRITE_DCR, SCTL_IFSYS_ETH_HPROT,   SCTL_IFSYS_ETH_HPROT_dflt,
                                                                            SCTL_IFSYS_ETH_HPROT_mask },
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

DECLARE_CONST(COUNT_MEMORY_AREAS, 8)

static bool check_sctl_plb4_clk_management(const uint32_t base_addr)
{
    bool result = true;
    uint32_t ppc_sys_conf_value = dcr_read(DCR_SCTL_BASE + SCTL_PPC_SYS_CONF);
    test_event(TEST_OI10_CTRL_000_CHECK_50MHz);
    if(SCTL_PPC_SYS_CONF_PLB4_DIVMODE_50MHz != (
                    (ppc_sys_conf_value & SCTL_PPC_SYS_CONF_PLB4_DIVMODE_mask)
                        >> SCTL_PPC_SYS_CONF_PLB4_DIVMODE_i))
    {
        result = false;
        rumboot_printf("Illegal value of PLB4_DIVMODE field in SCTL_PPC_SYS_CONF register: must be 1 for 50MHz");
    }
    dcr_write(DCR_SCTL_BASE + SCTL_PPC_SYS_CONF,
            (ppc_sys_conf_value
                    & (~SCTL_PPC_SYS_CONF_PLB4_DIVMODE_mask))
                    | (SCTL_PPC_SYS_CONF_PLB4_DIVMODE_100MHz << SCTL_PPC_SYS_CONF_PLB4_DIVMODE_i));
    ppc_sys_conf_value = dcr_read(DCR_SCTL_BASE + SCTL_PPC_SYS_CONF);
    test_event(TEST_OI10_CTRL_000_CHECK_100MHz);
    if(SCTL_PPC_SYS_CONF_PLB4_DIVMODE_100MHz != (
                    (ppc_sys_conf_value & SCTL_PPC_SYS_CONF_PLB4_DIVMODE_mask)
                        >> SCTL_PPC_SYS_CONF_PLB4_DIVMODE_i))
    {
        result = false;
        rumboot_printf("Illegal value of PLB4_DIVMODE field in SCTL_PPC_SYS_CONF register: must be 1 for 100MHz");
    }
    dcr_write(DCR_SCTL_BASE + SCTL_PPC_SYS_CONF,
            (ppc_sys_conf_value
                    & (~SCTL_PPC_SYS_CONF_PLB4_DIVMODE_mask))
                    | (SCTL_PPC_SYS_CONF_PLB4_DIVMODE_50MHz << SCTL_PPC_SYS_CONF_PLB4_DIVMODE_i));
    return result;
}

static void send_mbist_inject_event(const uint32_t chain_number)
{
    uint32_t const event_data[] = { TEST_OI10_CTRL_000_CHECK_INJECT_ERR, chain_number };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_data, ARRAY_SIZE(event_data) );
}

static bool check_sctl_kmbist_chains(const uint32_t base_addr, const bool with_injections)
{
    bool result = true;
    uint32_t t = 0;
    uint32_t chain_number = 0;
    for(uint32_t kmbist_chain = KMBIST_CHAIN_SF_0; kmbist_chain < KMBIST_CHAIN_SF_8; kmbist_chain+=4)
    {
        t = TEST_OI10_CTRL_000_MBIST_TIMEOUT;
        chain_number = ((kmbist_chain - KMBIST_CHAIN_SF_0) / 4) + 1;
        if(with_injections)
            send_mbist_inject_event(chain_number);

        dcr_write(base_addr + kmbist_chain, (SCTL_KMBIST_CHAIN_SF_TM_val << SCTL_KMBIST_CHAIN_SF_TM_i)
                | SCTL_KMBIST_CHAIN_SF_START_mask);
        rumboot_printf("Started KMBIST_CHAIN number %d\n", chain_number);
        while((!(dcr_read(base_addr + kmbist_chain) & SCTL_KMBIST_CHAIN_SF_DONE_mask)) && (t))
        {
            --t;
        }
        TEST_ASSERT(((dcr_read(base_addr + kmbist_chain) & SCTL_KMBIST_CHAIN_SF_DONE_mask) && t),
                "Timeout occurred while waiting end of KMBIST_CHAIN!");
        result &= ((bool)(dcr_read(base_addr + kmbist_chain) & SCTL_KMBIST_CHAIN_SF_FAIL_mask)
                == with_injections);
    }
    return result;
}

static bool check_sctl_kmbist_chains_default(const uint32_t base_addr)
{
    return check_sctl_kmbist_chains(base_addr, false);
}

#ifdef MBIST_ENABLED
static bool check_sctl_kmbist_chains_inject(const uint32_t base_addr)
{
    return check_sctl_kmbist_chains(base_addr, true);
}
#endif

TEST_SUITE_BEGIN(sctl_testlist, "SCTL TEST")
TEST_ENTRY("SCTL reg read default", check_sctl_regs_ro, DCR_SCTL_BASE),
TEST_ENTRY("SCTL reg rw check", check_sctl_regs_rw, DCR_SCTL_BASE),
TEST_ENTRY("SCTL check PLB4 frequency change", check_sctl_plb4_clk_management, DCR_SCTL_BASE),
TEST_ENTRY("SCTL run KMBIST chains as is", check_sctl_kmbist_chains_default, DCR_SCTL_BASE),
#ifdef MBIST_ENABLED
TEST_ENTRY("SCTL run KMBIST chains with injections", check_sctl_kmbist_chains_inject, DCR_SCTL_BASE),
#endif
TEST_SUITE_END();

int main(void)
{
   register int result;
   test_event_send_test_id("test_oi10_ctrl_000");
   rumboot_printf("Check write/read ctrl register values\n");

   result = test_suite_run(NULL,&sctl_testlist);
   return result;
}
