/*
 * test_oi10_cpu_039_00_helper.c
 *
 *  Created on: Dec 21, 2018
 *      Author: m.smolina
 */


#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>

#define TEST_ERROR 1
#define TEST_OK 0

int __attribute__((section(".text.test"))) check_cpu_ppc_039(void)
{
    const uint32_t num_elem = TEST_OI10_CPU_039_ARRAY_SIZE / 4;
    const uint32_t val_A = 0x0;
    const uint32_t data_base_address = SRAM0_BASE + 0x8000;

    int32_t i;
    uint32_t register R1;
    uint32_t register R2;

    /*stage 1*/
    rumboot_printf("Stage 1, write offsets\n");

    for(i = 0, R1 = val_A; i < num_elem; i++, R1+=4)
    {
        iowrite32( R1, data_base_address + 4*i);
        msync();
    }

    /*stage 2*/
    rumboot_printf("Stage 2, read and compare offsets\n");
    for(i = (num_elem - 1),  R1 = (val_A + 4 * (num_elem - 1)); i >= 0; i--, R1-=4)
    {
        R2 = ioread32(data_base_address + 4*i);
        msync();
        if( R1 != R2 )
        {
            rumboot_printf("Error!  R1 == 0x%x and R2 == 0x%x do not coincide at virtual address == 0x%x!\n", R1, R2, data_base_address + 4*i);
            return TEST_ERROR;
        }
        //R1 -= 4;
    }

    /*stage 3*/
    rumboot_printf("Stage 3, write inverted offset\n");
    R1 = val_A;
    for(i = 0; i < num_elem; i++)
    {
        iowrite32( ~R1, data_base_address + 4*i);
        R1 +=4;
    }
    msync();

    /*stage 4*/
    rumboot_printf("Stage 4, read and compare inverted offset\n");
    R1 = val_A + 4 * (num_elem - 1);
    for(i = num_elem -1 ; i >= 0; i--)
    {
        R2 = ioread32(data_base_address + 4*i);
        if( (~R1) != (R2) )
        {
            rumboot_printf("Error!  ~R1 == 0x%x and R2 == 0x%x do not coincide at virtual address == 0x%x!\n", ~R1, R2, data_base_address + 4*i);
            return TEST_ERROR;
        }
        R1 -= 4;
    }

    return TEST_OK;
}
