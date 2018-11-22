#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/ppc470s/mmu.h>
#include <platform/devices/mpic128.h>
#include <platform/devices.h>
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

void rumboot_itrpt_hdr_base();
void rumboot_CI_hdr();
void rumboot_MC_hdr();
void rumboot_DS_hdr();
void rumboot_IS_hdr();
void rumboot_EI_hdr();
void rumboot_A_hdr();
void rumboot_P_hdr();
void rumboot_FPU_hdr();
void rumboot_SC_hdr();
void rumboot_APU_hdr();
void rumboot_DEC_hdr();
void rumboot_FIT_hdr();
void rumboot_WDT_hdr();
void rumboot_DTLBE_hdr();
void rumboot_ITLBE_hdr();
void rumboot_D_hdr();

void rumboot_platform_setup() {
    /* Disable interrupts on the PPC core */
    uint32_t const msr_old_value = msr_read();
    msr_write( msr_old_value & ~((0b1 << ITRPT_XSR_CE_i)       /* MSR[CE] - Critical interrupt. */
                               | (0b1 << ITRPT_XSR_EE_i)       /* MSR[EE] - External interrupt. */
                               | (0b1 << ITRPT_XSR_ME_i)       /* MSR[ME] - Machine check. */
                               | (0b1 << ITRPT_XSR_DE_i)));    /* MSR[DE] - Debug interrupt. */

    rumboot_irq_register_mpic128();

    spr_write( SPR_IVPR,    (uint32_t)&rumboot_itrpt_hdr_base & 0xFFFF0000 );    /* link irq handlers mirror */
    spr_write( SPR_IVOR0,   (uint32_t)&rumboot_CI_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR1,   (uint32_t)&rumboot_MC_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR2,   (uint32_t)&rumboot_DS_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR3,   (uint32_t)&rumboot_IS_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR4,   (uint32_t)&rumboot_EI_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR5,   (uint32_t)&rumboot_A_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR6,   (uint32_t)&rumboot_P_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR7,   (uint32_t)&rumboot_FPU_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR8,   (uint32_t)&rumboot_SC_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR9,   (uint32_t)&rumboot_APU_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR10,  (uint32_t)&rumboot_DEC_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR11,  (uint32_t)&rumboot_FIT_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR12,  (uint32_t)&rumboot_WDT_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR13,  (uint32_t)&rumboot_DTLBE_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR14,  (uint32_t)&rumboot_ITLBE_hdr & 0x0000FFFF );
    spr_write( SPR_IVOR15,  (uint32_t)&rumboot_D_hdr & 0x0000FFFF );

    msr_write( msr_old_value );

    extern char rumboot_im0_heap_start;
    extern char rumboot_im0_heap_end;
    rumboot_malloc_register_heap( "IM0", &rumboot_im0_heap_start, &rumboot_im0_heap_end );

    extern char rumboot_im1_heap_start;
    extern char rumboot_im1_heap_end;
    rumboot_malloc_register_heap( "IM1", &rumboot_im1_heap_start, &rumboot_im1_heap_end );

    extern char rumboot_im2_heap_start;
    extern char rumboot_im2_heap_end;
    rumboot_malloc_register_heap( "IM2", &rumboot_im2_heap_start, &rumboot_im2_heap_end );

    extern char rumboot_sram0_heap_start;
    extern char rumboot_sram0_heap_end;
    rumboot_malloc_register_heap("SRAM0", &rumboot_sram0_heap_start, &rumboot_sram0_heap_end);

    extern char rumboot_sdram_heap_start;
    extern char rumboot_sdram_heap_end;
    rumboot_malloc_register_heap("SDRAM", &rumboot_sdram_heap_start, &rumboot_sdram_heap_end);

    extern char rumboot_ssram_heap_start;
    extern char rumboot_ssram_heap_end;
    rumboot_malloc_register_heap("SSRAM", &rumboot_ssram_heap_start, &rumboot_ssram_heap_end);

    extern char rumboot_plram_heap_start;
    extern char rumboot_plram_heap_end;
    rumboot_malloc_register_heap("PLRAM", &rumboot_plram_heap_start, &rumboot_plram_heap_end);

    extern char rumboot_sram1_heap_start;
    extern char rumboot_sram1_heap_end;
    rumboot_malloc_register_heap("SRAM1", &rumboot_sram1_heap_start, &rumboot_sram1_heap_end);

    extern char rumboot_nor_heap_start;
    extern char rumboot_nor_heap_end;
    rumboot_malloc_register_heap("NOR", &rumboot_nor_heap_start, &rumboot_nor_heap_end);
}
