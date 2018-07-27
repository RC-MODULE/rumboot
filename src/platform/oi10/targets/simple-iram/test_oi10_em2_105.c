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
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>

int main ()
{
    emi_init ();

    rumboot_printf ("WRITE32 NOR\n");
    iowrite32_NOR ( 0x00000000, (NOR_BASE + 0x0));
    iowrite32_NOR ( 0x10101010, (NOR_BASE + 0x4));
    iowrite32_NOR ( 0x11111111, (NOR_BASE + 0x8));
    iowrite32_NOR ( 0xBABADEDA, (NOR_BASE + 0xC));
    iowrite32_NOR ( 0xDEDABABA, (NOR_BASE + 0x10));
    iowrite32_NOR ( 0xAAAAAAAA, (NOR_BASE + 0x14));
    iowrite32_NOR ( 0x55555555, (NOR_BASE + 0x18));
    iowrite32_NOR ( 0xCCCCCCCC, (NOR_BASE + 0x1C));

    iowrite32_NOR ( 0x00000000, ((uint32_t) NOR_BASE + NOR_SIZE - 0x20));
    iowrite32_NOR ( 0x10101010, ((uint32_t) NOR_BASE + NOR_SIZE - 0x1C));
    iowrite32_NOR ( 0x11111111, ((uint32_t) NOR_BASE + NOR_SIZE - 0x18));
    iowrite32_NOR ( 0xBABADEDA, ((uint32_t) NOR_BASE + NOR_SIZE - 0x14));
    iowrite32_NOR ( 0xDEDABABA, ((uint32_t) NOR_BASE + NOR_SIZE - 0x10));
    iowrite32_NOR ( 0xAAAAAAAA, ((uint32_t) NOR_BASE + NOR_SIZE - 0x0C));
    iowrite32_NOR ( 0x55555555, ((uint32_t) NOR_BASE + NOR_SIZE - 0x08));
    iowrite32_NOR ( 0xCCCCCCCC, ((uint32_t) NOR_BASE + NOR_SIZE - 0x04));

    rumboot_printf ("READ32 NOR\n");
    rumboot_printf( "0x00 = %x\n",ioread32 (NOR_BASE + 0x0));
    TEST_ASSERT (ioread32 (NOR_BASE + 0x0) == 0x00000000,"TEST ERROR: (0x00) - read value is wrong");
    rumboot_printf( "0x04 = %x\n",ioread32 (NOR_BASE + 0x4));
    TEST_ASSERT (ioread32 (NOR_BASE + 0x4) == 0x10101010,"TEST ERROR: (0x04) - read value is wrong");
    rumboot_printf( "0x08 = %x\n",ioread32 (NOR_BASE + 0x8));
    TEST_ASSERT (ioread32 (NOR_BASE + 0x8) == 0x11111111,"TEST ERROR: (0x08) - read value is wrong");
    rumboot_printf( "0x0C = %x\n",ioread32 (NOR_BASE + 0xC));
    TEST_ASSERT (ioread32 (NOR_BASE + 0xC) == 0xBABADEDA,"TEST ERROR: (0x0C) - read value is wrong");
    rumboot_printf( "0x10 = %x\n",ioread32 (NOR_BASE + 0x10));
    TEST_ASSERT (ioread32 (NOR_BASE + 0x10) == 0xDEDABABA,"TEST ERROR: (0x10) - read value is wrong");
    rumboot_printf( "0x14 = %x\n",ioread32 (NOR_BASE + 0x14));
    TEST_ASSERT (ioread32 (NOR_BASE + 0x14) == 0xAAAAAAAA,"TEST ERROR: (0x15) - read value is wrong");
    rumboot_printf( "0x18 = %x\n",ioread32 (NOR_BASE + 0x18));
    TEST_ASSERT (ioread32 (NOR_BASE + 0x18) == 0x55555555,"TEST ERROR: (0x18) - read value is wrong");
    rumboot_printf( "0x1C = %x\n",ioread32 (NOR_BASE + 0x1C));
    TEST_ASSERT (ioread32 (NOR_BASE + 0x1C) == 0xCCCCCCCC,"TEST ERROR: (0x1C) - read value is wrong");

    rumboot_printf( "0xFFFFFE0 = %x\n",ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x20));
    TEST_ASSERT (ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x20) == 0x00000000,"TEST ERROR: (0xFFFFFE0) - read value is wrong");
    rumboot_printf( "0xFFFFFE4 = %x\n",ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x1C));
    TEST_ASSERT (ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x1C) == 0x10101010,"TEST ERROR: (0xFFFFFE4) - read value is wrong");
    rumboot_printf( "0xFFFFFE8 = %x\n",ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x18));
    TEST_ASSERT (ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x18) == 0x11111111,"TEST ERROR: (0xFFFFFE8) - read value is wrong");
    rumboot_printf( "0xFFFFFEC = %x\n",ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x14));
    TEST_ASSERT (ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x14) == 0xBABADEDA,"TEST ERROR: (0xFFFFFEC) - read value is wrong");
    rumboot_printf( "0xFFFFFF0 = %x\n",ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x10));
    TEST_ASSERT (ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x10) == 0xDEDABABA,"TEST ERROR: (0xFFFFFF0) - read value is wrong");
    rumboot_printf( "0xFFFFFF4 = %x\n",ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x0C));
    TEST_ASSERT (ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x0C) == 0xAAAAAAAA,"TEST ERROR: (0xFFFFFF4) - read value is wrong");
    rumboot_printf( "0xFFFFFF8 = %x\n",ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x08));
    TEST_ASSERT (ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x08) == 0x55555555,"TEST ERROR: (0xFFFFFF8) - read value is wrong");
    rumboot_printf( "0xFFFFFFC = %x\n",ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x04));
    TEST_ASSERT (ioread32 ((uint32_t) NOR_BASE + NOR_SIZE - 0x04) == 0xCCCCCCCC,"TEST ERROR: (0xFFFFFFC) - read value is wrong");

    rumboot_printf ("TEST OK\n");
    return 0;
}
