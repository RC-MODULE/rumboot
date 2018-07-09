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

#include <platform/devices/mpic128.h>


/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime() {
    #define TIMER_TICKS_PER_US  800

    return spr_read( SPR_TBL_R ) / TIMER_TICKS_PER_US;
}

void rumboot_irq_hdr();

void rumboot_platform_setup() {
    /* Disable interrupts on the PPC core */
    uint32_t const msr_old_value = msr_read();
    msr_write( msr_old_value & ~((0b1 << ITRPT_XSR_CE_i)       /* MSR[CE] - Critical interrupt. */
                               | (0b1 << ITRPT_XSR_EE_i)       /* MSR[EE] - External interrupt. */
                               | (0b1 << ITRPT_XSR_ME_i)       /* MSR[ME] - Machine check. */
                               | (0b1 << ITRPT_XSR_DE_i)));    /* MSR[DE] - Debug interrupt. */

    rumboot_irq_register_mpic128();

    spr_write( SPR_IVPR,    (uint32_t)&rumboot_irq_hdr & 0xFFFF0000 ); /* link irq handlers mirror */
    spr_write( SPR_IVOR4,   (uint32_t)&rumboot_irq_hdr & 0x0000FFFF ); /* link rumboot irq dispatcher */

    msr_write( msr_old_value );

    //extern char rumboot_platform_heap_start;
    //extern char rumboot_platform_heap_end;
    //rumboot_malloc_register_heap( "IM0", &rumboot_platform_heap_start, &rumboot_platform_heap_end );
}
