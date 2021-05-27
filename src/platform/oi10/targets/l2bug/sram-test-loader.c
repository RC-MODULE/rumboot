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


struct rumboot_bootheader gdbmon_hdr;
struct rumboot_bootheader *p_header = ( struct rumboot_bootheader * )0x0;

#ifdef EMI_INIT
static emi_bank_cfg bank_cfg[6];
#endif


// valid: 0 - disable page; 1 - enable page
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
    end_addr = start_addr + ( L2C_SIZE - 1 );

    // Устанавливаем адрес на первое слово в строке (это необязательно делать)
    mask_line_base = ( ~( PPC476FP_L2_CACHELINE_SIZE - 1 ) );
    range_start = start_addr & mask_line_base;
    range_end = ( end_addr & mask_line_base ) + PPC476FP_L2_CACHELINE_SIZE;

    // Сбрасываем все данные из диапазона из кэша L2 в память
    // The dcbst instruction flushes dirty data from the L2 cache. This
    // instruction is effectively a no-op for the L1 because the L1 cache is
    // write-through only and cannot contain dirty data.
    for ( addr = range_start; addr < range_end; addr += PPC476FP_L2_CACHELINE_SIZE )
    {
        asm volatile (
            "dcbst 0, %0\n"
            :
            :"r"( addr )
        );
    }

    asm volatile ( "msync \n\t" );
    asm volatile ( "isync \n\t" );
}

int main( void )
{
    rumboot_platform_runtime_info->silent = false;

#if 0
    printf( "Setup cache\n" );
    // 1. Отключение контроля четности кэш-памяти данных L1D
    spr_write( SPR_CCR1, ( 0b00 << CTRL_CCR1_GPRPEI_i )\
               | ( 0b00 << CTRL_CCR1_FPRPEI_i )\
               | ( 0b00 << CTRL_CCR1_ICDPEI_i )\
               | ( 0b00 << CTRL_CCR1_ICLPEI_i )\
               | ( 0b00 << CTRL_CCR1_ICTPEI_i )\
               | ( 0b00 << CTRL_CCR1_DCDPEI_i )\
               | ( 0b00 << CTRL_CCR1_DCLPEI_i )\
               | ( 0b00 << CTRL_CCR1_DCTPEI_i )\
               | ( 0b0 << CTRL_CCR1_MMUTPEI_i )\
               | ( 0b0 << CTRL_CCR1_MMUDPEI_i )\
               | ( CTRL_CCR1_TSS_CPU_clock << CTRL_CCR1_TSS_i )\
               | ( 0b1 << CTRL_CCR1_DPC_i )\
               | ( CTRL_CCR1_TCS_div1 << CTRL_CCR1_TCS_i )
             );
    /*
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
    */
    // 2. L2CWACCFG.GatheringOff (И используем WB + Non-guarded)
    dcr_write( 0x80000600, 0xC10 );
    dcr_write( 0x80000604, 0x1 );
#endif /* 0 */

#ifdef EMI_INIT
    printf( "Init EMI...\n" );
    prepare_default_emi_cfg( bank_cfg );
    //bank0 - SRAM0
    ( bank_cfg + 0 )->ssx_cfg.T_CYC  = TCYC_3;
    ( bank_cfg + 0 )->ssx_cfg.T_DEL  = TDEL_1;
    // ( bank_cfg + 0 )->ssx_cfg.T_CYC  = TCYC_8;
    // ( bank_cfg + 0 )->ssx_cfg.T_DEL  = TDEL_3;
    //bank5 - NOR
    ( bank_cfg + 5 )->ssx_cfg.T_CYC  = TCYC_12;
    emi_init_ext_cfg( DCR_EM2_EMI_BASE, bank_cfg );
    printf( "Init done.\n" );
#endif

#ifdef ADD_TLB
    printf( "Remove 0x0 TLB entry\n" );
    add_tlb_entry( 0x0, 0x0, 0x0, MMU_TLBE_DSIZ_1GB, 0, 0, 0, 0xE0000000, 0x07, 0x0 );
    printf( "Add TLB entries\n" );
    // SRAM (Размер 4 Мбайт, 2 микросхемы по 2 Мбайт)
    /*
        add_tlb_entry( 0x0, 0x0, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0xE0000000, 0x3f, 0x0 );
        add_tlb_entry( 0x100000, 0x100000, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0xE0000000, 0x3f, 0x0 );
        add_tlb_entry( 0x200000, 0x200000, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0xE0000000, 0x3f, 0x0 );
        add_tlb_entry( 0x300000, 0x300000, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0xE0000000, 0x3f, 0x0 );
    */
    add_tlb_entry( 0x0, 0x0, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0x100000, 0x100000, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0x200000, 0x200000, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0x00000000, 0x3f, 0x0 );
    add_tlb_entry( 0x300000, 0x300000, 0x0, MMU_TLBE_DSIZ_1MB, 2, 0, 1, 0x00000000, 0x3f, 0x0 );

    // Контроллер SW
    add_tlb_entry( 0xC0300000, 0xC0300000, 0x20, MMU_TLBE_DSIZ_16KB, 7, 1, 1, 0x00000000, 0x03, 0x0 );

    // Области внутреннй памяти, ставим в LE для работы со SW
    add_tlb_entry( 0xC0000000, 0xC0000000, 0x20, MMU_TLBE_DSIZ_64KB, 0, 1, 1, 0xE0000000, 0x07, 0x0 );
    add_tlb_entry( 0xC0010000, 0xC0010000, 0x20, MMU_TLBE_DSIZ_64KB, 0, 1, 1, 0xE0000000, 0x07, 0x0 );
    add_tlb_entry( 0xC0040000, 0xC0040000, 0x20, MMU_TLBE_DSIZ_64KB, 0, 1, 1, 0xE0000000, 0x07, 0x0 );
    add_tlb_entry( 0xC0050000, 0xC0050000, 0x20, MMU_TLBE_DSIZ_64KB, 0, 1, 1, 0xE0000000, 0x07, 0x0 );

    // NOR (Размер 128 Мбайт, 2 микросхемы по 512 Мбит)
    // add_tlb_entry( 0x70000000, 0x70000000, 0x0, MMU_TLBE_DSIZ_256MB, 0, 0, 0, 0xE0000000, 0x07, 0x8000 );
    // add_tlb_entry( 0x70000000, 0x70000000, 0x0, MMU_TLBE_DSIZ_16MB, 0, 0, 1, 0xE0000000, 0x3f, 0x0 );

    // printf( "Remove IM0 TLB entries\n" );
    // add_tlb_entry( 0x80000000, 0x80000000, 0x010, MMU_TLBE_DSIZ_64KB, 0, 0, 0, 0x09000000, 0x07, 0x0 );
    // add_tlb_entry( 0x80010000, 0x80010000, 0x010, MMU_TLBE_DSIZ_64KB, 0, 0, 0, 0x0A000000, 0x07, 0x0 );
    // printf( "Add    IM0 TLB entries: write-back\n" );
    // add_tlb_entry( 0x80000000, 0x80000000, 0x010, MMU_TLBE_DSIZ_64KB, 2, 0, 1, 0x00000000, 0x3f, 0x0 );
    // add_tlb_entry( 0x80010000, 0x80010000, 0x010, MMU_TLBE_DSIZ_64KB, 2, 0, 1, 0x00000000, 0x3f, 0x0 );
    // printf( "Add    IM0 TLB entries: no cache\n" );
    // add_tlb_entry( 0x80020000, 0x80000000, 0x010, MMU_TLBE_DSIZ_64KB, 0, 0, 1, 0x00000000, 0x3f, 0x0 );
    // add_tlb_entry( 0x80030000, 0x80010000, 0x010, MMU_TLBE_DSIZ_64KB, 0, 0, 1, 0x00000000, 0x3f, 0x0 );
    printf( "Done.\n" );
#endif

    printf( "write something\n" );
    uint32_t *ptr = ( uint32_t * )0x4;
    uint32_t value;

    for ( int i = 1; i < 12; i += 4 )
    {
        value = ( i << 24 ) | ( ( i + 1 ) << 16 ) | ( ( i + 2 ) << 8 ) | ( i + 3 );
        printf( "0x%.8lx\n", value );
        *ptr++ = value;

    }

    printf( "read something\n" );
    ptr = ( uint32_t * )0x4;

    for ( int i = 1; i < 12; i += 4 )
    {
        printf( "0x%.8lx\n", *ptr++ );
    }

#if 1
    printf( "Copy test to SRAM\n" );
    rumboot_platform_request_file( "MBIN", M_BASE );
#else
    // Загружаем информацию об образе gdbmon из NOR памяти
    printf( "Read header from NOR:\n" );
    memcpy( ( void * )&gdbmon_hdr, ( void * )( NOR_BASE + 8192 ), sizeof( gdbmon_hdr ) );
    printf( "magic:          0x%lx\n", gdbmon_hdr.magic );
    printf( "datalen:        %ld\n", gdbmon_hdr.datalen );
    printf( "entry_point[0]: 0x%lx\n", gdbmon_hdr.entry_point[0] );

    printf( "Copy test to SRAM\n" );
    memcpy( ( void * )M_BASE, ( void * )( NOR_BASE + 8192 ), gdbmon_hdr.datalen );
#endif

    printf( "Drop l2\n" );
    flush_L2C( M_BASE );

    printf( "Read header from SRAM:\n" );
    memcpy( ( void * )&gdbmon_hdr, ( void * )M_BASE, sizeof( gdbmon_hdr ) );
    printf( "magic:          0x%lx\n", gdbmon_hdr.magic );
    printf( "datalen:        %ld\n", gdbmon_hdr.datalen );
    printf( "entry_point[0]: 0x%lx\n", gdbmon_hdr.entry_point[0] );

    // printf( "datalen:        %ld\n", *( ( uint32_t * )( M_BASE + 0xC ) ) );
    // uint32_t entry_point = *( ( uint32_t * )( M_BASE + 0x10 ) );
    // printf( "entry_point[0]: 0x%lx\n", entry_point );

    rumboot_printf( "Starting test code from another memory\n" );

    return rumboot_bootimage_execute_ep( ( void * ) gdbmon_hdr.entry_point[0] );
    // return rumboot_bootimage_execute_ep( ( void * ) entry_point );
}
