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
uint32_t __attribute__((section(".data.test_oi10_cpu_038"))) mass[TEST_OI10_CPU_038_ARRAY_SIZE] = { 0x0 };

uint32_t r1, r2;

int check_cpu_ppc_038(void)
{
    uint32_t i;
    uint32_t register temp_output;
    uint32_t register val_A = 0x00000000;
    /*stage 1*/

    rumboot_printf("Stage 1, write\n");

    mass[0] = 0x00;
    rumboot_printf("mass[0]: 0x%x\n",(uint32_t)mass[0]);

    r1 = val_A;
    rumboot_printf("r1 value early init: 0x%x\n", (uint32_t)r1);
    /*zapis v massiv chicel s incrementom*/
    for(i = 0; i < array_size; ++i)
    {
    iowrite32(val_A, mass[i]);
    temp_output = (uint32_t)&mass[i];
    rumboot_printf("Write operation...0x%x \n", temp_output);
    rumboot_printf("Address val_A 0x%x \n", (uint32_t)val_A);
    }
    /*stage 2*/
    rumboot_printf("Stage 2, read and compare\n");
    val_A = 0x00000000;
    rumboot_printf("Value init val_A: 0x%x \n", (uint32_t)val_A);
    r1 = val_A; //punkt d
    rumboot_printf("r1 register value by copy val_A in r1: 0x%x \n", r1);
    for(i = 0; i < array_size; ++i)
    {
    r2 = ioread32((uint32_t)&mass[i]);
    rumboot_printf("Result READ operaion r1 is: 0x%x\n", r1);
    rumboot_printf("Result READ operaion r2 is: 0x%x\n", r2);
    rumboot_printf("Read operation r2 is: 0x%x\n", &mass[i]);
    if( r1 != r2 )
    {
        rumboot_printf("Error  r1 != r2 not compared\n");
        return ERROR;
    }
    rumboot_printf("Read Ok!\n");
    mass[i] = val_A << 2;
    }
    /*3 stage*/
    rumboot_printf("Stage 3, write\n");
    mass[0] = 0x00;
    rumboot_printf("mass[0]: 0x%x\n",(uint32_t)mass[0]);
    r1 = val_A;
    rumboot_printf("r1 value early init: 0x%x\n", (uint32_t)r1); //OTLADKA
    for(i = 0; i < array_size; ++i)
    {
    iowrite32(~val_A, mass[i]);
    temp_output = (uint32_t)&mass[i];
    rumboot_printf("Write operation...0x%x \n", temp_output);
    rumboot_printf("Address val_A 0x%x \n", (uint32_t)val_A); //OTLADRKA
    val_A +=4;
    }
    /*4 stage*/
    rumboot_printf("Stage 4, read and compare\n");
    val_A = 0x00000000;
    rumboot_printf("Value init val_A: 0x%x \n", (uint32_t)val_A); //OTLADRKA
    r1 = val_A; //punkt d
    rumboot_printf("r1 register value by copy val_A in r1: 0x%x \n", r1); //OTLADKA
    for(i = 0; i < array_size; ++i)
    {
    r2 = ioread32((uint32_t)&mass[i]);
    rumboot_printf("Result READ operaion r1 is: 0x%x\n", r1);
    rumboot_printf("Result READ operaion r2 is: 0x%x\n", r2);
    rumboot_printf("Read operation r2 is: 0x%x\n", &mass[i]);

    if( r1 != r2 )
    {
        rumboot_printf("Error  r1 != r2 not compared\n");
        return ERROR;
    }
    rumboot_printf("Read Ok!\n");
    mass[i] = val_A << 2;
    }
    return OK;
}

