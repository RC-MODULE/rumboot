/*
 * test_oi10_cpu_037.c
 *
 *  Created on: Nov 16, 2018
 *      Author: m.smolina
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/ppc470s/mmu.h>
#include <platform/devices/mpic128.h>
#include <devices/gpio_pl061.h>
#include <platform/devices.h>
#include <platform/regs/sctl.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsrc/physmap.h>
#include <rumboot/bootsrc/sdio.h>
#include <rumboot/bootsrc/boilerplate.h>
#include <platform/bootm.h>
#include <regs/regs_gpio_pl061.h>
#include <regs/regs_uart_pl011.h>
#include <platform/devices/emi.h>
#include <platform/regs/fields/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/devices/plb6mcif2.h>
#include <devices/uart_pl011.h>
#include <rumboot/irq.h>
#include <platform/arch/ppc/ppc_476fp_fpu_fields.h>
#include <platform/arch/ppc/ppc_476fp_fpu_const.h>

#define ITRPT_XSR_FP_i IBM_BIT_INDEX(64, ITRPT_XSR_FP_e)

float FPSCR_1 = 9.989e-37;
float FPSCR_2 = 9.979e-37;
float FPSCR_3 = 1.123455e-34;
float FPSCR_4 = 1.123456e-34;
float FPSCR_5 = 0.0e-37;
float FPSCR_6 = 0.121e+37;
float result_sum;


static void enable_fpu()
{
    msr_write(msr_read() | (1 << ITRPT_XSR_FP_i));
}

uint64_t  value;

void read_fpu ()
{
    asm volatile
            (
                    "mffs %0 \n\t"
                    : "=r" (value)
            );
}

void check_exception ()
{
    if ((value >> FPU_FPSCR_OX_e)     & 1) rumboot_printf("A floating-point overflow exception occurred\n");
    if ((value >> FPU_FPSCR_UX_e)     & 1) rumboot_printf("A floating-point underflow exception occurred\n");
    if ((value >> FPU_FPSCR_ZX_e)     & 1) rumboot_printf("A floating-point zero divide exception occurred\n");
    if ((value >> FPU_FPSCR_XX_e)     & 1) rumboot_printf("A floating-point inexact exception occurred\n");
    if ((value >> FPU_FPSCR_VXSNAN_e) & 1) rumboot_printf("A floating-point invalid operation exception (VXSNAN) occurred\n");
    if ((value >> FPU_FPSCR_VXISI_e)  & 1) rumboot_printf("A floating-point invalid operation exception (VXISI) occurred\n");
    if ((value >> FPU_FPSCR_VXIDI_e)  & 1) rumboot_printf("A floating-point invalid operation exception (VXIDI) occurred\n");
    if ((value >> FPU_FPSCR_VXZDZ_e)  & 1) rumboot_printf("A floating-point invalid operation exception (VXZDZ) occurred\n");
    if ((value >> FPU_FPSCR_VXSOFT_e) & 1) rumboot_printf("A floating-point invalid operation exception (software request) occurred\n");
    if ((value >> FPU_FPSCR_VXSQRT_e) & 1) rumboot_printf("A floating-point invalid operation exception (invalid square root) occurred\n");
    if ((value >> FPU_FPSCR_FI_e)     & 1) rumboot_printf("Floating-point fraction inexact \n");
    if ((value >> FPU_FPSCR_FR_e)     & 1) rumboot_printf("Floating-point fraction rounded \n");
    //if ((value >> FPU_FPSCR_FE_e)     & 1) rumboot_printf("Floating-point equal to zero \n");
    if ((value >> FPU_FPSCR_FU_e)     & 1) rumboot_printf("Floating-point unordered or not-a-number (NaN). \n");



}

void check_OX ()
{

    rumboot_printf("TEST OX\n");
    result_sum = FPSCR_1 + FPSCR_2;
    read_fpu ();
    check_exception ();

    result_sum = FPSCR_1 + FPSCR_2 - 192;

    read_fpu ();
    check_exception ();

}

void check_UX ()
{
    rumboot_printf("TEST UX\n");
    result_sum = FPSCR_3 - FPSCR_4;
    if (result_sum == 0.0e-264 ) rumboot_printf("result_sum = 0 \n");
    read_fpu ();
    check_exception ();
}

void check_ZX ()
{
    rumboot_printf("TEST ZX\n");
    result_sum = FPSCR_1/FPSCR_5;
    read_fpu ();
    check_exception ();
}


int main ()
{
    check_exception ();
    //Overflow Exception(OX)
    enable_fpu();
    rumboot_printf("TEST START\n");

    read_fpu ();
    check_exception ();

    //rumboot_printf("TEST OX\n");
    //result_sum = FPSCR_1 + FPSCR_2;

   // rumboot_printf("TEST UX\n");
    //result_sum = FPSCR_3 - FPSCR_4;
    //if (result_sum == 0.0e-264 ) rumboot_printf("result_sum = 0 \n");
    //rumboot_printf("TEST ZX\n");
    //result_sum = FPSCR_1/FPSCR_5;
    //rumboot_printf("result_sum = %d\n", result_sum);
    //rumboot_printf("TEST XX\n");
    //result_sum = FPSCR_3 + FPSCR_6;
    //rumboot_printf("result_sum = %d\n", result_sum);
    //read_fpu ();
    //check_exception ();

    //check_OX ();
    //check_UX ();
    //check_ZX ();


    rumboot_printf("TEST OK\n");
    return 0;
}
