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


#define ITRPT_XSR_FP_i IBM_BIT_INDEX(64, ITRPT_XSR_FP_e)

double FPSCR_OX_1 = 1.1111e64;
double FPSCR_OX_2 = 1.1111e64;

double result_sum;

static void enable_fpu()
{
    msr_write(msr_read() | (1 << ITRPT_XSR_FP_i));
}

int main ()
{

    uint32_t value;
    enable_fpu();

    result_sum = FPSCR_OX_1 + FPSCR_OX_2;
    rumboot_printf("Result sum: %d\n", result_sum);
    asm volatile
            (
                    "mffs %0 \n\t"
                    : "=r" (value)
            );
    rumboot_printf("Result: %b\n", value);
    rumboot_printf("TEST OK\n");
    return 0;
}
