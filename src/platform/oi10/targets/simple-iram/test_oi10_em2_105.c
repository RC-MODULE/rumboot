/*
 * test_oi10_em2_105.c
 *
 *  Created on: Jul 26, 2018
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/regpoker.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>

int main ()
{
    emi_init (DCR_EM2_EMI_BASE);

    rumboot_printf ("WRITE32 NOR\n");
    nor_write32 ( 0x00000000, (NOR_BASE + 0x0));
    nor_write32 ( 0x10101010, (NOR_BASE + 0x4));
    nor_write32 ( 0x11111111, (NOR_BASE + 0x8));
    nor_write32 ( 0xBABADEDA, (NOR_BASE + 0xC));
    nor_write32 ( 0xDEDABABA, (NOR_BASE + 0x10));
    nor_write32 ( 0xAAAAAAAA, (NOR_BASE + 0x14));
    nor_write32 ( 0x55555555, (NOR_BASE + 0x18));
    nor_write32 ( 0xCCCCCCCC, (NOR_BASE + 0x1C));

    nor_write32 ( 0x00000000, ((uint32_t) NOR_BASE + NOR_SIZE - 0x20));
    nor_write32 ( 0x10101010, ((uint32_t) NOR_BASE + NOR_SIZE - 0x1C));
    nor_write32 ( 0x11111111, ((uint32_t) NOR_BASE + NOR_SIZE - 0x18));
    nor_write32 ( 0xBABADEDA, ((uint32_t) NOR_BASE + NOR_SIZE - 0x14));
    nor_write32 ( 0xDEDABABA, ((uint32_t) NOR_BASE + NOR_SIZE - 0x10));
    nor_write32 ( 0xAAAAAAAA, ((uint32_t) NOR_BASE + NOR_SIZE - 0x0C));
    nor_write32 ( 0x55555555, ((uint32_t) NOR_BASE + NOR_SIZE - 0x08));
    nor_write32 ( 0xCCCCCCCC, ((uint32_t) NOR_BASE + NOR_SIZE - 0x04));

    rumboot_printf ("READ32 NOR\n");

    struct regpoker_checker check_array[] = {
      { "Data0",  REGPOKER_READ32, 0x0, 0x00000000, 0xffffffff },
      { "Data1",  REGPOKER_READ32, 0x4, 0x10101010, 0xffffffff },
      { "Data2",  REGPOKER_READ32, 0x8, 0x11111111, 0xffffffff },
      { "Data3",  REGPOKER_READ32, 0xC, 0xBABADEDA, 0xffffffff },
      { "Data4",  REGPOKER_READ32, 0x10, 0xDEDABABA, 0xffffffff },
      { "Data5",  REGPOKER_READ32, 0x14, 0xAAAAAAAA, 0xffffffff },
      { "Data6",  REGPOKER_READ32, 0x18, 0x55555555, 0xffffffff },
      { "Data7",  REGPOKER_READ32, 0x1C, 0xCCCCCCCC, 0xffffffff },
      { "Data8",  REGPOKER_READ32, (uint32_t) NOR_SIZE - 0x20, 0x00000000, 0xffffffff },
      { "Data9",  REGPOKER_READ32, (uint32_t) NOR_SIZE - 0x1C, 0x10101010, 0xffffffff },
      { "Data10",  REGPOKER_READ32, (uint32_t) NOR_SIZE - 0x18, 0x11111111, 0xffffffff },
      { "Data11",  REGPOKER_READ32, (uint32_t) NOR_SIZE - 0x14, 0xBABADEDA, 0xffffffff },
      { "Data12",  REGPOKER_READ32, (uint32_t) NOR_SIZE - 0x10, 0xDEDABABA, 0xffffffff },
      { "Data13",  REGPOKER_READ32, (uint32_t) NOR_SIZE - 0x0C, 0xAAAAAAAA, 0xffffffff },
      { "Data14",  REGPOKER_READ32, (uint32_t) NOR_SIZE - 0x08, 0x55555555, 0xffffffff },
      { "Data15",  REGPOKER_READ32, (uint32_t) NOR_SIZE - 0x04, 0xCCCCCCCC, 0xffffffff },
      { /* Sentinel */ }
    };

    if (rumboot_regpoker_check_array(check_array, NOR_BASE) == 0)
    {
        rumboot_printf ("TEST OK\n");
        return 0;
    }

    rumboot_printf ("TEST ERROR\n");
    return 1;
}
