#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <platform/easynmc/easynmc.h>
#include <stdint.h>
#include <devices/greth_edcl.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/hexdump.h>
#include <rumboot/io.h>
#include <rumboot/platform.h>
#include <rumboot/irq.h>

void overflow() {
        uint32_t tmp = 0xb2ffffff;
        uint32_t tmp2 = 0xbfffffff;
        uint32_t ret = 0;
        asm volatile("%0 = %1 + %1;\n"
                : "=r"(ret)
                : "r"(tmp)
        );
}

static volatile int got_exception = 0;
void the_exception(int id, const char *name) {
        rumboot_printf("Got an expected exception %d: %s\n", id, name);
        got_exception=5;
        rumboot_printf("==> %d\n", got_exception);
}

int main(int argc, char** argv)
{
        rumboot_irq_set_exception_handler(the_exception);
        rumboot_irq_sei();

        /* Check overflow exception handling */
        __nmc_enable_overflow_exception(true);
        RUMBOOT_ATOMIC_BLOCK(){
                rumboot_printf("Triggering overflow\n");
                overflow();
                rumboot_printf("Checking it didn't arrive!\n");
                if (got_exception) {
                        exit(1);
                }
        }
        if (!got_exception) {
                rumboot_printf("Overflow exception did not happen\n");
                rumboot_printf("Psst! Check your toolchain for https://jira.module.ru/jira/browse/NM-1191\n");
                exit(1);
        }
        rumboot_printf("Looking good, yeah\n");
        return 0;
} 

