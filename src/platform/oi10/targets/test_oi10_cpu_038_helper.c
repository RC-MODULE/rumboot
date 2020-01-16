#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>
#ifdef NOR_WRITE
#include <platform/devices/nor_1636RR4.h>
#endif

#define TEST_ERROR 1
#define TEST_OK 0

const uint32_t __attribute__((section(".text"))) array_size = TEST_OI10_CPU_038_ARRAY_SIZE;
const uint32_t __attribute__((section(".text"))) val_A = 0x0;
uint32_t __attribute__((section(".data.test"))) mass[TEST_OI10_CPU_038_ARRAY_SIZE] = { 0xffffffff };


int __attribute__((section(".text.test"))) check_cpu_ppc_038(void)
{
    uint32_t i;
    uint32_t register R1 = val_A;
    uint32_t register R2;
    /*stage 1*/

    rumboot_printf("Stage 1, write offsets\n");

    for(i = 0; i < array_size; i++)
    {
#ifdef NOR_WRITE
        nor_write32(R1,(uint32_t)&mass[i]);
#else
        iowrite32( R1, (uint32_t)&mass[i]);
#endif
        R1 += 4;
    }
    msync();

    /*stage 2*/
    rumboot_printf("Stage 2, read and compare offsets\n");
    R1 = val_A;
    for(i = 0; i < array_size; i++)
    {
        R2 = ioread32((uint32_t)&mass[i]);
        if( R1 != R2 )
        {
            rumboot_printf("Error!  R1 == 0x%x and R2 == 0x%x do not coincide at virtual address == 0x%x!\n", R1, R2, &mass[i]);
            return TEST_ERROR;
        }
        R1 += 4;
    }

    /*stage 3*/
    rumboot_printf("Stage 3, write inverted offset\n");
    R1 = val_A;
    for(i = 0; i < array_size; i++)
    {
#ifdef NOR_WRITE
        nor_write32(~R1,(uint32_t)&mass[i]);
#else
        iowrite32( ~R1, (uint32_t)&mass[i]);
#endif
        R1 +=4;
    }
    msync();

    /*stage 4*/
    rumboot_printf("Stage 4, read and compare inverted offset\n");
    R1 = val_A;
    for(i = 0; i < array_size; i++)
    {
        R2 = ioread32((uint32_t)&mass[i]);
        if( (~R1) != (R2) )
        {
            rumboot_printf("Error!  ~R1 == 0x%x and R2 == 0x%x do not coincide at virtual address == 0x%x!\n", ~R1, R2, &mass[i]);
            return TEST_ERROR;
        }
        R1 += 4;
    }

    rumboot_printf("TEST OK\n");
    return TEST_OK;
}
