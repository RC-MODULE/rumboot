#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <rumboot/testsuite.h>

#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_crg_sys.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>


static uint32_t check_registers_read(uint32_t base_addr)
{
    rumboot_printf("Check the read of the registers:");

    struct regpoker_checker check_default_array[] = {
          {   "CRG_SYS_PLL_STAT",    REGPOKER_READ_DCR,    CRG_SYS_PLL_STAT,         0x00000000,           0x00000011   },
          {   "CRG_SYS_PLL_CTRL",    REGPOKER_READ_DCR,    CRG_SYS_PLL_CTRL,         0x00000000,           0x00000003   },
          {   "CRG_SYS_PLL_LDUR",    REGPOKER_READ_DCR,    CRG_SYS_PLL_LDUR,         0x00000640,           0x000007ff   },
          {   "CRG_SYS_PLL_PRDIV",   REGPOKER_READ_DCR,    CRG_SYS_PLL_PRDIV,        0x00000001,           0x0000001f   },
          {   "CRG_SYS_PLL_FBDIV",   REGPOKER_READ_DCR,    CRG_SYS_PLL_FBDIV,        0x00000020,           0x000007ff   },
          {   "CRG_SYS_PLL_PSDIV",   REGPOKER_READ_DCR,    CRG_SYS_PLL_PSDIV,        0x00000004,           0x00000007   },
          {   "CRG_SYS_WR_LOCK",     REGPOKER_READ_DCR,    CRG_SYS_WR_LOCK,          0x00000001,           0x00000001   },
          {   "CRG_SYS_RST_MON",     REGPOKER_READ_DCR,    CRG_SYS_RST_MON,          0x00000800,           0x00000f07   },
          {   "CRG_SYS_RST_CFG0",    REGPOKER_READ_DCR,    CRG_SYS_RST_CFG0,         0x00000096,           0x00000fff   },
          {   "CRG_SYS_RST_CFG1",    REGPOKER_READ_DCR,    CRG_SYS_RST_CFG1,         0x00320096,           0x0fff0fff   },
          {   "CRG_SYS_RST_CFG2",    REGPOKER_READ_DCR,    CRG_SYS_RST_CFG2,         0x0007000E,           0x0107002f   },
          {   "CRG_SYS_CKUPDATE",    REGPOKER_READ_DCR,    CRG_SYS_CKUPDATE,         0x00000000,           0x00000001   },
          {   "CRG_SYS_INTMASK",     REGPOKER_READ_DCR,    CRG_SYS_INTMASK,          0x00000000,           0x00000001   },
          //{   "CRG_SYS_INTCLR",      REGPOKER_READ_DCR,    CRG_SYS_INTCLR,           0x00000000,           0x00000001   },
          {   "CRG_SYS_CKDIVMODE0",  REGPOKER_READ_DCR,    CRG_SYS_CKDIVMODE0,       0x00000000,           0x0000001f   },
          {   "CRG_SYS_CKDIVMODE1",  REGPOKER_READ_DCR,    CRG_SYS_CKDIVMODE1,       0x00000000,           0x0000001f   },
          {   "CRG_SYS_CKDIVMODE2",  REGPOKER_READ_DCR,    CRG_SYS_CKDIVMODE2,       0x00000001,           0x0000001f   },
          {   "CRG_SYS_CKDIVMODE3",  REGPOKER_READ_DCR,    CRG_SYS_CKDIVMODE3,       0x00000000,           0x0000001f   },
          {   "CRG_SYS_CKDIVMODE4",  REGPOKER_READ_DCR,    CRG_SYS_CKDIVMODE4,       0x00000009,           0x0000001f   },
          { }
      };

    if( rumboot_regpoker_check_array( check_default_array, base_addr ) == 0 )
    {
        rumboot_printf( "OK\n" );
        return 0;
    }

    rumboot_printf( "ERROR\n" );
    return 1;
}

static uint32_t check_registers_write( uint32_t base_addr )
{
    rumboot_printf("Check the write of the registers:");

    struct regpoker_checker check_default_array[] = {
          //{   "CRG_SYS_PLL_STAT",    REGPOKER_WRITE_DCR,    CRG_SYS_PLL_STAT,         0x00000000,           0x00000011   },
          {   "CRG_SYS_PLL_CTRL",    REGPOKER_WRITE_DCR,    CRG_SYS_PLL_CTRL,         0x00000000,           0x00000003   },
          {   "CRG_SYS_PLL_LDUR",    REGPOKER_WRITE_DCR,    CRG_SYS_PLL_LDUR,         0x00000640,           0x000007ff   },
          {   "CRG_SYS_PLL_PRDIV",   REGPOKER_WRITE_DCR,    CRG_SYS_PLL_PRDIV,        0x00000001,           0x0000001f   },
          {   "CRG_SYS_PLL_FBDIV",   REGPOKER_WRITE_DCR,    CRG_SYS_PLL_FBDIV,        0x00000020,           0x000007ff   },
          {   "CRG_SYS_PLL_PSDIV",   REGPOKER_WRITE_DCR,    CRG_SYS_PLL_PSDIV,        0x00000004,           0x00000007   },
          //{   "CRG_SYS_WR_LOCK",     REGPOKER_WRITE_DCR,    CRG_SYS_WR_LOCK,          0x00000001,           0xffffffff   },
          //{   "CRG_SYS_RST_MON",     REGPOKER_WRITE_DCR,    CRG_SYS_RST_MON,          0x00000800,           0x00000f07   },
          {   "CRG_SYS_RST_CFG0",    REGPOKER_WRITE_DCR,    CRG_SYS_RST_CFG0,         0x00000096,           0x00000fff   },
          {   "CRG_SYS_RST_CFG1",    REGPOKER_WRITE_DCR,    CRG_SYS_RST_CFG1,         0x00320096,           0x0fff0fff   },
          {   "CRG_SYS_RST_CFG2",    REGPOKER_WRITE_DCR,    CRG_SYS_RST_CFG2,         0x0007000E,           0x0107002f   },
          {   "CRG_SYS_CKUPDATE",    REGPOKER_WRITE_DCR,    CRG_SYS_CKUPDATE,         0x00000000,           0x00000001   },
          {   "CRG_SYS_INTMASK",     REGPOKER_WRITE_DCR,    CRG_SYS_INTMASK,          0x00000000,           0x00000001   },
          //{   "CRG_SYS_INTCLR",      REGPOKER_WRITE_DCR,    CRG_SYS_INTCLR,           0x00000000,           0x00000001   },
          {   "CRG_SYS_CKDIVMODE0",  REGPOKER_WRITE_DCR,    CRG_SYS_CKDIVMODE0,       0x00000000,           0x0000001f   },
          {   "CRG_SYS_CKDIVMODE1",  REGPOKER_WRITE_DCR,    CRG_SYS_CKDIVMODE1,       0x00000000,           0x0000001f   },
          {   "CRG_SYS_CKDIVMODE2",  REGPOKER_WRITE_DCR,    CRG_SYS_CKDIVMODE2,       0x00000001,           0x0000001f   },
          {   "CRG_SYS_CKDIVMODE3",  REGPOKER_WRITE_DCR,    CRG_SYS_CKDIVMODE3,       0x00000000,           0x0000001f   },
          {   "CRG_SYS_CKDIVMODE4",  REGPOKER_WRITE_DCR,    CRG_SYS_CKDIVMODE4,       0x00000009,           0x0000001f   },
          { }
      };

    if( rumboot_regpoker_check_array( check_default_array, base_addr ) == 0 )
    {
        rumboot_printf( "OK\n" );
        return 0;
    }

    rumboot_printf( "ERROR\n" );
    return 1;
}



int main(void)
{
    register int result;

    const uint32_t CRG_REG_WRITE_ENABLE = 0x1ACCE551;


    rumboot_printf( "CRG registers test has been started\n" );

    result = check_registers_read(DCR_CRG_BASE);

    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, CRG_REG_WRITE_ENABLE);

    result |= check_registers_write(DCR_CRG_BASE);

    dcr_write(DCR_CRG_BASE + CRG_SYS_WR_LOCK, ~CRG_REG_WRITE_ENABLE);


    if(!result)
    {
        rumboot_printf("TEST_OK\n");
        return 0;
    }

    rumboot_printf("TEST_ERROR\n");
    return result;
}
