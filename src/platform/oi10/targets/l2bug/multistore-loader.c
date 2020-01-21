#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <arch/ppc_476fp_config.h>
#include <arch/io.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/test_event_c.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/irq.h>
#include <platform/devices/mpic128.h>

#define printf rumboot_printf


extern void rumboot_itrpt_hdr_base();
extern void rumboot_CI_hdr();
extern void rumboot_MC_hdr();
extern void rumboot_DS_hdr();
extern void rumboot_IS_hdr();
extern void rumboot_EI_hdr();
extern void rumboot_A_hdr();
extern void rumboot_P_hdr();
extern void rumboot_FPU_hdr();
extern void rumboot_SC_hdr();
extern void rumboot_APU_hdr();
extern void rumboot_DEC_hdr();
extern void rumboot_FIT_hdr();
extern void rumboot_WDT_hdr();
extern void rumboot_DTLBE_hdr();
extern void rumboot_ITLBE_hdr();
extern void rumboot_D_hdr();


struct rumboot_bootheader gdbmon_hdr;

#ifdef EMI_INIT
static emi_bank_cfg bank_cfg =
{
    //SS0
    {
        BTYP_SRAM,
        PTYP_NO_PAGES,
        SRDY_EXT_RDY_NOT_USE,
        TWR_0,
        SST_Flow_Through,
        TSSOE_1,
        TSOE_1,
        TCYC_3,
        0, //T_RDY
        TDEL_1
    },
    //SD0
    {
        CSP_256,
        SDS_2M,
        CL_3,
        TRDL_1,
        SI_EXT_INIT,
        TRCD_5,
        TRAS_9
    }
};
#endif


int add_tlb_entry( uint32_t e_base, uint32_t r_base, uint32_t ERPN, unsigned DSIZ, unsigned cache_mode, unsigned is_little )
{
    register uint32_t
    w0 = ( e_base & 0xfffff000 ) | ( 0x800 ) | ( DSIZ << 4 ),
    w1 = ( r_base & 0xfffff000 ) | ERPN,
    w2;

    switch ( cache_mode )
    {
        case 0:
            w2 = 0x3043f; // IL1I, IL1D, IL2
            break;

        case 2:
            w2 = 0x0003f; // Cache Write-back
            break;

        case 3:
            w2 = 0x0083f; // Cache Write-through
            break;

        case 4:
            w2 = 0x1003f;   // IL1D
            break;

        case 5:
            w2 = 0x2003f;   // IL1I
            break;

        case 6:             // Cache WB, Guarded
            w2 = 0x0013f;
            break;

        default :
            w2 = 0x3053f;
            break;
    }

    if ( is_little )
    {
        w2 |= 0x80;
    }

    asm(
        // "mtspr  0x3b2,  %0\n"
        "mtspr %4, %0\n"
        "tlbwe  %1, %0, 0\n"
        "tlbwe  %2, %0, 1\n"
        "tlbwe  %3, %0, 2\n"
        :
        :"r"( 0 ), "r"( w0 ), "r"( w1 ), "r"( w2 ),
        "I"( SPR_MMUCR )
    );
    return 0;
}


#define  PPC476FP_L1_CACHELINE_SIZE   32u   // Длина кэш строки L1 в байтах
#define  PPC476FP_L2_CACHELINE_SIZE  128u   // Длина кэш строки L2 в байтах

/*------------------------------------------------------------------------------
|       Сброс кэшей инструкций и данных в память
|-------------------------------------------------------------------------------
|   Параметры:
|       start       Указатель на начало кэшируемой области
|       sizeInBytes Размер области в байтах
|-------------------------------------------------------------------------------
*/
void  flush_cache_range ( void * start, unsigned long sizeInBytes )
{
    unsigned long    range_start;
    unsigned long    range_end;
    unsigned long    addr;
    unsigned long    mask_line_base;
    unsigned long    start_addr;
    unsigned long    end_addr;

    start_addr = ( unsigned long )start;
    end_addr = start_addr + sizeInBytes;

    // Устанавливаем адрес на первое слово в строке (это необязательно делать)
    mask_line_base = ( ~( PPC476FP_L2_CACHELINE_SIZE - 1 ) );
    range_start = start_addr & mask_line_base;
    range_end = ( end_addr & mask_line_base ) + PPC476FP_L2_CACHELINE_SIZE;

    // Инвалидируем данные из диапазона из кэша данных, чтобы при следующем
    // использовании данные были загружены из памяти
    // dcbf сбрасывает 4 строки DL1 (и соответственно одну строку L2)
    for ( addr = range_start; addr < range_end; addr += PPC476FP_L2_CACHELINE_SIZE )
    {
        asm volatile (
            "dcbf 0, %0\n"
            "msync\n"
            :
            :"r"( addr )
        );
    }

    // Инвалидируем данные из диапазона из кэша инструкций, чтобы при следующем
    // использовании инструкции были загружены из памяти
    mask_line_base = ( ~( PPC476FP_L1_CACHELINE_SIZE - 1 ) );
    range_start = start_addr & mask_line_base;
    range_end = ( end_addr & mask_line_base ) + PPC476FP_L1_CACHELINE_SIZE;

    for ( addr = range_start; addr < range_end; addr += PPC476FP_L1_CACHELINE_SIZE )
    {
        asm volatile (
            "icbi 0, %0\n"
            "isync\n"
            :
            :"r"( addr )
        );
    }
}

#define L2C_SIZE 0x40000
void flush_L2C ( void * start )
{
    unsigned long    range_start;
    unsigned long    range_end;
    unsigned long    addr;
    unsigned long    mask_line_base;
    unsigned long    start_addr;
    unsigned long    end_addr;

    start_addr = ( unsigned long )start;
    end_addr = start_addr + (L2C_SIZE - 1);

    // Устанавливаем адрес на первое слово в строке (это необязательно делать)
    mask_line_base = ( ~( PPC476FP_L2_CACHELINE_SIZE - 1 ) );
    range_start = start_addr & mask_line_base;
    range_end = ( end_addr & mask_line_base ) + PPC476FP_L2_CACHELINE_SIZE;

    // Сбрасываем все данные из диапазона из кэша L2 в память
    // The dcbst instruction flushes dirty data from the L2 cache. This
    // instruction is effectively a no-op for the L1 because the L1 cache is
    // write-through only and cannot contain dirty data.
    for (addr = range_start; addr < range_end; addr += PPC476FP_L2_CACHELINE_SIZE)
    {
        asm volatile (
            "dcbst 0, %0\n"
            :
            :"r"(addr)
        );
    }
    asm volatile ("msync \n\t");
    asm volatile ("isync \n\t");
}

int main(void)
{
    // uint32_t result = 0;  

    rumboot_platform_runtime_info->silent = false;
        uint32_t const msr_old_value = msr_read();
    msr_write( msr_old_value & ~((0b1 << ITRPT_XSR_CE_i)       /* MSR[CE] - Critical interrupt. */
                               | (0b1 << ITRPT_XSR_EE_i)       /* MSR[EE] - External interrupt. */
                               | (0b1 << ITRPT_XSR_ME_i)       /* MSR[ME] - Machine check. */
                               | (0b1 << ITRPT_XSR_DE_i)));    /* MSR[DE] - Debug interrupt. */

    rumboot_irq_register_mpic128();

    spr_write( SPR_IVPR,    (uint32_t)&rumboot_itrpt_hdr_base & ITRPT_IVPR_ADDR_mask );    /* link irq handlers mirror */
    spr_write( SPR_IVOR0,   (uint32_t)&rumboot_CI_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR1,   (uint32_t)&rumboot_MC_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR2,   (uint32_t)&rumboot_DS_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR3,   (uint32_t)&rumboot_IS_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR4,   (uint32_t)&rumboot_EI_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR5,   (uint32_t)&rumboot_A_hdr        & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR6,   (uint32_t)&rumboot_P_hdr        & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR7,   (uint32_t)&rumboot_FPU_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR8,   (uint32_t)&rumboot_SC_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR9,   (uint32_t)&rumboot_APU_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR10,  (uint32_t)&rumboot_DEC_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR11,  (uint32_t)&rumboot_FIT_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR12,  (uint32_t)&rumboot_WDT_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR13,  (uint32_t)&rumboot_DTLBE_hdr    & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR14,  (uint32_t)&rumboot_ITLBE_hdr    & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR15,  (uint32_t)&rumboot_D_hdr        & ITRPT_IVORn_OFFSET_mask );

    msr_write( msr_old_value );

    rumboot_irq_set_exception_handler(rumboot_arch_exception);

    printf("Setup cache\n");
    spr_write(SPR_CCR2, (CTRL_CCR2_DSTG_disabled << CTRL_CCR2_DSTG_i)\
                        | (0b0 << CTRL_CCR2_DLFPD_i)    \
                        | (0b0 << CTRL_CCR2_DSTI_i)     \
                        | (0b0 << CTRL_CCR2_PMUD_i)     \
                        | (0b0 << CTRL_CCR2_DCSTGW_i)   \
                        | (0   << CTRL_CCR2_STGCTR_i)   \
                        | (0b0 << CTRL_CCR2_DISTG_i)    \
                        | (0b0 << CTRL_CCR2_SPC5C1_i)   \
                        | (0b0 << CTRL_CCR2_MCDTO_i     )
            );

    dcr_write(0x80000600, 0xC10);
    dcr_write(0x80000604, 0x1);

#ifdef EMI_INIT
    printf("Init EMI...\n");
    emi_init_ext_cfg(DCR_EM2_EMI_BASE, &bank_cfg);
    printf("Init done.\n");
#endif

#ifdef ADD_TLB
    printf("Add TLB entries\n");
    add_tlb_entry( 0x0, 0x0, 0x0, MMU_TLBE_DSIZ_1MB, 6, 0 );
    rumboot_memfill8_modelling((void*)SRAM0_BASE, 0x10000, 0x00, 0x00); //workaround (init first 64KB in SRAM0)
//    memset((void *)0x0, 0x0, 0x4000);
    flush_L2C(0x0);
    printf("Done.\n");
#endif

    dcr_write(0x80000600, 0xC10);
    printf("L2WACCFG(0xC10) = 0x%x\n", dcr_read(0x80000604));

    printf("Copy test to SRAM\n");
    rumboot_platform_request_file("MBIN", M_BASE);
    printf("Drop l2\n");
    flush_L2C(M_BASE);

    printf("Read header:\n");
    memcpy((void *)&gdbmon_hdr, (void *)M_BASE, sizeof(gdbmon_hdr));
    printf("magic:          %lx\n", gdbmon_hdr.magic);
    printf("datalen:        %ld\n", gdbmon_hdr.datalen);
    printf("entry_point[0]: %ld\n", gdbmon_hdr.entry_point[0]);

    rumboot_printf("Starting test code from another memory\n");

    return rumboot_bootimage_execute_ep((void *) gdbmon_hdr.entry_point[0]);
}
