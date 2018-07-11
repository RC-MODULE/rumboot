#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#define TEST_ERROR 1
#define TEST_OK 0
#define ERROR 1
#define OK 0

const uint32_t __attribute__((section(".text"))) array_size = TEST_OI10_CPU_038_ARRAY_SIZE;
const uint32_t __attribute__((section(".text"))) val_A = 0x0;
uint32_t __attribute__((section(".data.test_oi10_cpu_038"))) mass[TEST_OI10_CPU_038_ARRAY_SIZE] = { 0xffffffff };


int check_cpu_ppc_038(void)
{
    uint32_t i;
    uint32_t register R1 = val_A;
    uint32_t register R2;
    /*stage 1*/

    rumboot_printf("Stage 1, write offset\n");

    rumboot_printf("mass[0]: 0x%x\n",(uint32_t)mass[0]);

    for(i = 0; i < array_size; ++i)
    {
        mass[i] = R1;
        R1 += 4;
    }
    /*stage 2*/
    rumboot_printf("Stage 2, read and compare offsets\n");
    R1 = val_A;
    for(i = 0; i < array_size; ++i)
    {
        R2 = mass[i];
//        rumboot_printf("Result READ operaion r1 is: 0x%x\n", r1);
//        rumboot_printf("Result READ operaion r2 is: 0x%x\n", r2);
//        rumboot_printf("Read operation r2 is: 0x%x\n", &mass[i]);
        if( R1 != R2 )
        {
            rumboot_printf("Error!  R1 == 0x%x and R2 == 0x%x do not coincide at virtual address == 0x%x!", R1, R2, &mass[i]);
            return ERROR;
        }
        R1 += 4;
    }
    /*stage 3*/
    rumboot_printf("Stage 3, write inverted offset\n");
    R1 = val_A;
    for(i = 0; i < array_size; ++i)
    {
        mass[i] = R1;
        R1 +=4;
    }
    /*stage 4*/
    rumboot_printf("Stage 4, read and compare inverted offset\n");
    R1 = val_A;
    for(i = 0; i < array_size; ++i)
    {
        R2 = mass[i];
//        rumboot_printf("Result READ operaion r1 is: 0x%x\n", r1);
//        rumboot_printf("Result READ operaion r2 is: 0x%x\n", r2);
//        rumboot_printf("Read operation r2 is: 0x%x\n", &mass[i]);

        if( (~R1) != (R2) )
        {
            rumboot_printf("Error!  R1 == 0x%x and R2 == 0x%x do not coincide at virtual address == 0x%x!", R1, R2, &mass[i]);
            return ERROR;
        }
        R1 += 4;
    }
    return OK;
}

