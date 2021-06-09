#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <arch/ppc_476fp_config.h>
#include <arch/io.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <platform/test_event_c.h>
#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/irq.h>
#include <devices/mpic128.h>
#include <rumboot/hexdump.h>
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


struct rumboot_bootheader test_hdr;
#ifdef EMI_INIT
static emi_bank_cfg bank_cfg[6];
#endif


int add_tlb_entry( uint32_t e_base, uint32_t r_base, uint32_t ERPN, unsigned DSIZ, unsigned cache_mode, unsigned is_little,
                   unsigned valid, unsigned ra, unsigned xwr, unsigned mmucr )
{
    register uint32_t
    w0 = ( e_base & 0xfffff000 ) | ( valid << 11 ) | ( DSIZ << 4 ),
    w1 = ( r_base & 0xfffff000 ) | ERPN,
    w2;

    switch ( cache_mode )
    {
        case 0:
            w2 = 0x30400; // IL1I, IL1D, IL2
            break;

        case 2:
            w2 = 0x00000; // Cache Write-back
            break;

        case 3:
            w2 = 0x00800; // Cache Write-through
            break;

        case 4:
            w2 = 0x10000;   // IL1D
            break;

        case 5:
            w2 = 0x20000;   // IL1I
            break;

        case 6:             // Cache WB, Guarded
            w2 = 0x00100;
            break;

        case 7:             // IL1I, IL1D, IL2, Guarded
            w2 = 0x30500;
            break;

        default :
            w2 = 0x30500;
            break;
    }

    w2 |= xwr;

    if ( is_little )
    {
        w2 |= 0x80;
    }

    asm(
        // "mtspr  0x3b2,  %0\n"
        "mtspr %5, %0\n"
        "tlbwe  %1, %4, 0\n"
        "tlbwe  %2, %4, 1\n"
        "tlbwe  %3, %4, 2\n"
        :
        :"r"( mmucr ), "r"( w0 ), "r"( w1 ), "r"( w2 ), "r"( ra ),
        "I"( SPR_MMUCR )
    );
    return 0;
}


int main( void )
{
    rumboot_platform_runtime_info->silent = false;

#ifdef EMI_INIT
    printf( "Init EMI ...\n" );
    prepare_default_emi_cfg( bank_cfg );
    //bank0 - SRAM0
    ( bank_cfg + 0 )->ssx_cfg.T_CYC  = TCYC_3;
    ( bank_cfg + 0 )->ssx_cfg.T_DEL  = TDEL_1;
    emi_init_ext_cfg( DCR_EM2_EMI_BASE, bank_cfg );
    printf( "Init EMI - Done.\n" );
#endif

#ifdef ADD_TLB
    // SRAM (Размер 4 Мбайт, 2 микросхемы по 2 Мбайт)
    printf( "Add TLB entries ...\n" );
    // No Cache
    add_tlb_entry( 0x000000, 0x000000, 0x0, MMU_TLBE_DSIZ_1MB, 0, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0x100000, 0x100000, 0x0, MMU_TLBE_DSIZ_1MB, 0, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0x200000, 0x200000, 0x0, MMU_TLBE_DSIZ_1MB, 0, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0x300000, 0x300000, 0x0, MMU_TLBE_DSIZ_1MB, 0, 0, 1, 0x00000000, 0x3f, 0x0 );
    // Write-Through (L1d = on, L1i = on)
    add_tlb_entry( 0x400000, 0x000000, 0x0, MMU_TLBE_DSIZ_1MB, 3, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0x500000, 0x100000, 0x0, MMU_TLBE_DSIZ_1MB, 3, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0x600000, 0x200000, 0x0, MMU_TLBE_DSIZ_1MB, 3, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0x700000, 0x300000, 0x0, MMU_TLBE_DSIZ_1MB, 3, 0, 1, 0x00000000, 0x3f, 0x0 );
    // Write-Back (L1d = on, L1i = on)
    add_tlb_entry( 0x800000, 0x000000, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0x900000, 0x100000, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0xA00000, 0x200000, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0xB00000, 0x300000, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0x00000000, 0x3f, 0x0 );

    // Очищаем первые 64 Кбайт (чтобы убрать X на модели)
    // Если нужна инициализация других областей - это должен сделать загружаемый тест
    printf( "Memset\n" );
    memset( ( void * )0x0, 0, 0x10000 );
    printf( "Add TLB entries - Done.\n" );
#endif

    printf( "Copy test to SRAM\n" );
    rumboot_platform_request_file( "MBIN", M_BASE );

    printf( "Read header from SRAM:\n" );
    memcpy( ( void * )&test_hdr, ( void * )M_BASE, sizeof( test_hdr ) );
    printf( "magic:          0x%x\n", test_hdr.magic );
    printf( "datalen:        %d\n", test_hdr.datalen );
    printf( "entry_point[0]: 0x%x\n", test_hdr.entry_point[0] );

    printf( "spr_read(SPR_DCESR) = 0x%x\n", spr_read( SPR_DCESR ) );

    printf( "Starting test code from another memory\n" );
    return rumboot_bootimage_execute_ep( ( void * ) test_hdr.entry_point[0] );
}
