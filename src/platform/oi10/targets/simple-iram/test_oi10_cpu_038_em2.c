/*
 * test_oi10_cpu_038_em2.c
 *
 *  Created on: Nov 29, 2018
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
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>


#define DATA_SIZE   (124 * 1024) //124 KB
#define BASE_ADDR    SRAM0_BASE

int main (void)
{
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Stage 1, write offsets\n");
    for (uint32_t i = 0; i < (DATA_SIZE / 4); i+=4)
        iowrite32(i, BASE_ADDR + i);
    msync();

    rumboot_printf("Stage 2, read and compare offsets\n");
    for (uint32_t i = 0; i < (DATA_SIZE / 4); i+=4)
        TEST_ASSERT(ioread32(BASE_ADDR + i) == i, "TEST_ERROR: invalid read value");
    msync();

    rumboot_printf("Stage 3, write inverted offset\n");
    for (uint32_t i = 0; i < (DATA_SIZE / 4); i+=4)
        iowrite32(~i, BASE_ADDR + i);
    msync();

    rumboot_printf("Stage 4, read and compare inverted offset\n");
    for (uint32_t i = 0; i < (DATA_SIZE / 4); i+=4)
        TEST_ASSERT(ioread32(BASE_ADDR + i) == ~i, "TEST_ERROR: invalid read value");
    msync();

    rumboot_printf("TEST_OK\n");
    return 0;
}
