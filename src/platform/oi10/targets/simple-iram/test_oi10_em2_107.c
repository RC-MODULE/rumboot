/*
 * test_oi10_em2_107.c
 *
 *  Created on: Jul 26, 2018
 *      Author: a.tarasov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <devices/ugly/emi.h>

#define SRAM0_OFFSET_1 0x1000
#define SRAM0_OFFSET_2 0x2000

void check_sram0_8 (uint32_t const addr)
{
    static uint8_t const check_arr8[] =
    {
        [0] = 0x00,
        [1] = 0x11,
        [2] = 0x22,
        [3] = 0x33,
        [4] = 0x44,
        [5] = 0x55,
        [6] = 0x66,
        [7] = 0x77,
        [8] = 0x88,
        [9] = 0x99,
        [10] = 0xaa,
        [11] = 0xbb,
        [12] = 0xcc,
        [13] = 0xdd,
        [14] = 0xee,
        [15] = 0xff,
        [16] = 0x00,
        [17] = 0x11,
        [18] = 0x22,
        [19] = 0x33,
        [20] = 0x44,
        [21] = 0x55,
        [22] = 0x66,
        [23] = 0x77,
        [24] = 0x88,
        [25] = 0x99,
        [26] = 0xaa,
        [27] = 0xbb,
        [28] = 0xcc,
        [29] = 0xdd,
        [30] = 0xee,
        [31] = 0xff,
    };

    //write
    uint32_t i = 0;
    for (i = 0; i < 32; i++)
    {
        iowrite8 (check_arr8[i], addr + i);
    }
    msync();

    //read
    for (i = 0; i < 32; i++)
    {
        TEST_ASSERT (ioread8(addr+i) == check_arr8[i],"ERROR: read value is wrong");
    }
}

void check_sram0_16 (uint32_t const addr)
{
    static uint16_t const check_arr16[] =
    {
        [0] = 0x0000,
        [1] = 0x1111,
        [2] = 0x2222,
        [3] = 0x3333,
        [4] = 0x4444,
        [5] = 0x5555,
        [6] = 0x6666,
        [7] = 0x7777,
        [8] = 0x8888,
        [9] = 0x9999,
        [10] = 0xaaaa,
        [11] = 0xbbbb,
        [12] = 0xcccc,
        [13] = 0xdddd,
        [14] = 0xeeee,
        [15] = 0xffff
    };
    //write
    uint32_t i = 0;
    for (i = 0; i < 16; i++)
    {
        iowrite16 (check_arr16[i], addr + i*2);
    }
    msync();

    //read
    for (i = 0; i < 16; i++)
    {
        TEST_ASSERT (ioread16(addr + i*2) == check_arr16[i],"ERROR: read value is wrong");
    }
}

void check_sram0_32 (uint32_t const addr)
{
    static uint32_t const check_arr32[] =
    {
        [0] = 0x00000000,
        [1] = 0x11111111,
        [2] = 0x22222222,
        [3] = 0x33333333,
        [4] = 0x44444444,
        [5] = 0x55555555,
        [6] = 0x66666666,
        [7] = 0x77777777
    };
    //write
    uint32_t i = 0;
    for (i = 0; i < 8; i++)
    {
        iowrite32 (check_arr32[i], addr + i*4);
    }
    msync();

    //read
    for (i = 0; i < 8; i++)
    {
        TEST_ASSERT (ioread32(addr + i*4) == check_arr32[i],"ERROR: read value is wrong");
    }
}

void check_sram0_64 (uint32_t const addr)
{
    static uint64_t const check_arr64[] =
    {
        [0] = 0x0011223344556677ULL,
        [1] = 0x8899aabbccddeeffULL,
        [2] = 0x0011223344556677ULL,
        [3] = 0x8899aabbccddeeffULL
    };
    //write
    uint32_t i = 0;
    for (i = 0; i < 4; i++)
    {
        iowrite64 (check_arr64[i], addr + i * 8);
    }
    msync();

    //read
    for (i = 0; i < 4; i++)
    {
        TEST_ASSERT (ioread64(addr + i*8) == check_arr64[i],"ERROR: read value is wrong");
    }
}

void check_sram0 (const uint32_t addr)
{
    rumboot_printf ("START CHECK W/R SRAM0\n");
    rumboot_printf ("START ADDR = %x\n", addr);

    rumboot_printf ("SIZE_8\n");
    check_sram0_8(addr);

    rumboot_printf ("SIZE_16\n");
    check_sram0_16(addr + 32);

    rumboot_printf ("SIZE_32\n");
    check_sram0_32(addr + 64);

    rumboot_printf ("SIZE_64\n");
    check_sram0_64(addr + 96);

    rumboot_printf ("CHECK W/R SRAM0 SUCCESSFUL\n");
}


void init_iteration(uint32_t const addr)
{
  uint32_t i;
  uint32_t ptr;
  
  ptr = addr;
  for(i=0; i<=32 ; i++)   // All The Memory Region Is 128 Bytes = 32 32-bit Words  + 1 Word (Because It Is Unaligned)
  {
    iowrite32(0xDEADBEEF,ptr);
    ptr = ptr + 4;
  }
}

void init_test_place()
{
  init_iteration(SRAM0_BASE);
  init_iteration(SRAM0_BASE + SRAM0_OFFSET_1);
  init_iteration(SRAM0_BASE + SRAM0_OFFSET_2);
}

int main ()
{
    emi_init(DCR_EM2_EMI_BASE);

    init_test_place();
    
    rumboot_printf ("CACHE OFF\n");
    check_sram0 (SRAM0_BASE + 0x01);

    rumboot_printf ("CACHE OFF\n");
    check_sram0 (SRAM0_BASE + SRAM0_OFFSET_1 + 2);

    rumboot_printf ("CACHE OFF\n");
    check_sram0 (SRAM0_BASE + SRAM0_OFFSET_2 + 3);

    rumboot_printf ("TEST_OK\n");
    return 0;
}
