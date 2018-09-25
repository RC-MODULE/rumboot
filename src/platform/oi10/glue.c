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
#include <rumboot/boot.h>


int64_t rumboot_virt_to_phys(volatile void *addr)
{
    return get_physical_addr( (uint32_t) addr, 0);
}

uint32_t rumboot_virt_to_dma(volatile void *addr)
{
    uint64_t ret = get_physical_addr( (uint32_t)  addr, 0);
    return (uint32_t) (ret & 0xFFFFFFFF);
}

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime() {
    #define TIMER_TICKS_PER_US  200 /* TODO check if PPC_TMR_CLK is 5ns */

    return spr_read( SPR_TBL_R ) / TIMER_TICKS_PER_US;
}

void unexpected_itrpt_hdr_base();
void unexpected_CI_hdr();
void unexpected_MC_hdr();
void unexpected_DS_hdr();
void unexpected_IS_hdr();
//void unexpected_E_hdr();
void rumboot_irq_hdr();
void unexpected_A_hdr();
void unexpected_P_hdr();
void unexpected_FPU_hdr();
void unexpected_SC_hdr();
void unexpected_APU_hdr();
void unexpected_DEC_hdr();
void unexpected_FITI_hdr();
void unexpected_WTI_hdr();
void unexpected_DTLBE_hdr();
void unexpected_ITLBE_hdr();
void unexpected_D_hdr();

void rumboot_platform_setup() {
    /* Disable interrupts on the PPC core */
    uint32_t const msr_old_value = msr_read();
    msr_write( msr_old_value & ~((0b1 << ITRPT_XSR_CE_i)       /* MSR[CE] - Critical interrupt. */
                               | (0b1 << ITRPT_XSR_EE_i)       /* MSR[EE] - External interrupt. */
                               | (0b1 << ITRPT_XSR_ME_i)       /* MSR[ME] - Machine check. */
                               | (0b1 << ITRPT_XSR_DE_i)));    /* MSR[DE] - Debug interrupt. */

    rumboot_irq_register_mpic128();

    spr_write( SPR_IVPR,    (uint32_t)&unexpected_itrpt_hdr_base & 0xFFFF0000 );    /* link irq handlers mirror */
    spr_write( SPR_IVOR0,   (uint32_t)&unexpected_CI_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR1,   (uint32_t)&unexpected_MC_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR2,   (uint32_t)&unexpected_DS_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR3,   (uint32_t)&unexpected_IS_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR4,   (uint32_t)&rumboot_irq_hdr & 0x0000FFFF );              /* link rumboot irq dispatcher */
    spr_write( SPR_IVOR5,   (uint32_t)&unexpected_A_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR6,   (uint32_t)&unexpected_P_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR7,   (uint32_t)&unexpected_FPU_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR8,   (uint32_t)&unexpected_SC_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR9,   (uint32_t)&unexpected_APU_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR10,  (uint32_t)&unexpected_DEC_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR11,  (uint32_t)&unexpected_FITI_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR12,  (uint32_t)&unexpected_WTI_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR13,  (uint32_t)&unexpected_DTLBE_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR14,  (uint32_t)&unexpected_ITLBE_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR15,  (uint32_t)&unexpected_D_hdr & 0x0000FFFF );

    msr_write( msr_old_value );

    extern char rumboot_im0_heap_start;
    extern char rumboot_im0_heap_end;
    rumboot_malloc_register_heap( "IM0", &rumboot_im0_heap_start, &rumboot_im0_heap_end );

    extern char rumboot_im1_heap_start;
    extern char rumboot_im1_heap_end;
    rumboot_malloc_register_heap( "IM1", &rumboot_im1_heap_start, &rumboot_im1_heap_end );
}
