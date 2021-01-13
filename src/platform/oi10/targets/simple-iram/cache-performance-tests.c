#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <arch/ppc_476fp_config.h>
#include <arch/mmu.h>
#include <arch/io.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <platform/test_event_c.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/irq.h>
#include <devices/mpic128.h>
#include <rumboot/hexdump.h>
#define printf rumboot_printf

#define PPC476FP_L1_CACHELINE_SIZE 32u  // Длина кэш строки L1 в байтах
#define PPC476FP_L2_CACHELINE_SIZE 128u // Длина кэш строки L2 в байтах

/*------------------------------------------------------------------------------
|       Сброс кэшей инструкций и данных в память
|-------------------------------------------------------------------------------
|   Параметры:
|       start       Указатель на начало кэшируемой области
|       sizeInBytes Размер области в байтах
|-------------------------------------------------------------------------------
*/

enum caheop_mask
{
    CAHEOP_DATA = (1 << 0),
    CAHEOP_INSTR = (1 << 1),
};

void cache_flush_range(void *start, unsigned long sizeInBytes)
{
    unsigned long range_start;
    unsigned long range_end;
    unsigned long addr;
    unsigned long mask_line_base;
    unsigned long start_addr;
    unsigned long end_addr;

    start_addr = (unsigned long)start;
    end_addr = start_addr + sizeInBytes;

    // Устанавливаем адрес на первое слово в строке (это необязательно делать)
    mask_line_base = (~(PPC476FP_L2_CACHELINE_SIZE - 1));
    range_start = start_addr & mask_line_base;
    range_end = (end_addr & mask_line_base) + PPC476FP_L2_CACHELINE_SIZE;

    // Инвалидируем данные из диапазона из кэша данных, чтобы при следующем
    // использовании данные были загружены из памяти
    // dcbf сбрасывает 4 строки DL1 (и соответственно одну строку L2)

    for (addr = range_start; addr < range_end; addr += PPC476FP_L2_CACHELINE_SIZE)
    {
        asm volatile(
            "dcbf 0, %0\n"
            :
            : "r"(addr));
    }
    asm("msync\n");
}

void cache_inval_range(void *start, unsigned long sizeInBytes, int what)
{
    unsigned long range_start;
    unsigned long range_end;
    unsigned long addr;
    unsigned long mask_line_base;
    unsigned long start_addr;
    unsigned long end_addr;

    start_addr = (unsigned long)start;
    end_addr = start_addr + sizeInBytes;

    // Устанавливаем адрес на первое слово в строке (это необязательно делать)
    mask_line_base = (~(PPC476FP_L2_CACHELINE_SIZE - 1));
    range_start = start_addr & mask_line_base;
    range_end = (end_addr & mask_line_base) + PPC476FP_L2_CACHELINE_SIZE;

    // Инвалидируем данные из диапазона из кэша данных, чтобы при следующем
    // использовании данные были загружены из памяти
    // dcbf сбрасывает 4 строки DL1 (и соответственно одну строку L2)

    if (what & CAHEOP_DATA)
    {
        for (addr = range_start; addr < range_end; addr += PPC476FP_L2_CACHELINE_SIZE)
        {
            asm volatile(
                "dcbi 0, %0\n"
                :
                : "r"(addr));
        }
        asm("msync\n");
    }

    // Инвалидируем данные из диапазона из кэша инструкций, чтобы при следующем
    // использовании инструкции были загружены из памяти
    mask_line_base = (~(PPC476FP_L1_CACHELINE_SIZE - 1));
    range_start = start_addr & mask_line_base;
    range_end = (end_addr & mask_line_base) + PPC476FP_L1_CACHELINE_SIZE;
    if (what & CAHEOP_INSTR)
    {
        for (addr = range_start; addr < range_end; addr += PPC476FP_L1_CACHELINE_SIZE)
        {
            asm volatile(
                "dcbst 0, %0\n"
                "msync\n"
                "icbi 0, %0\n"
                "isync\n"
                :
                : "r"(addr));
        }
    }
}


static const tlb_entry cached_tlb[] =
    {
        /* Drop all shit */
        //       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,    DSIZ, IL1I, IL1D,W,   I,   M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(0x000, 0x00000, 0x00000, MMU_TLBE_DSIZ_1GB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_0, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b0)},
        {MMU_TLB_ENTRY(0x000, 0x40000, 0x40000, MMU_TLBE_DSIZ_1GB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_0, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b0)},
        {MMU_TLB_ENTRY(0x000, 0x80000, 0x00000, MMU_TLBE_DSIZ_1GB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_1, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b0)},
        {MMU_TLB_ENTRY(0x000, 0xc0000, 0x40000, MMU_TLBE_DSIZ_1GB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_1, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b0)},
        {MMU_TLB_ENTRY(0x001, 0x00000, 0x00000, MMU_TLBE_DSIZ_1GB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_2, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b0)},
        {MMU_TLB_ENTRY(0x001, 0x40000, 0x40000, MMU_TLBE_DSIZ_1GB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_2, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b0)},
        {MMU_TLB_ENTRY(0x001, 0x80000, 0x00000, MMU_TLBE_DSIZ_1GB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_3, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b0)},
        {MMU_TLB_ENTRY(0x001, 0xc0000, 0x40000, MMU_TLBE_DSIZ_1GB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_3, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b0)},
        {MMU_TLB_ENTRY(0x020, 0xC0300, 0xC0300, MMU_TLBE_DSIZ_16KB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b1, MMU_TLBE_E_LITTLE_END, 0b0, 0b0, 0b0, 0b0, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_1, MMU_TLBWE_BE_UND, 0b0)},
        {MMU_TLB_ENTRY(0x020, 0xC0300, 0xC0300, MMU_TLBE_DSIZ_16KB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b1, MMU_TLBE_E_LITTLE_END, 0b0, 0b0, 0b0, 0b0, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b1)},
        /* --- insert some new shit */
        {MMU_TLB_ENTRY(0x000, 0x00000, 0x00000, MMU_TLBE_DSIZ_1GB, 0b0, 0b0, 0b0, 0b0, 0b1, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_0, MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND, 0b1)}
//        {MMU_TLB_ENTRY(0x020, 0xC0000, 0x80020, MMU_TLBE_DSIZ_64KB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_LITTLE_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_3, 0b1)},
//        {MMU_TLB_ENTRY(0x020, 0xC0010, 0x80030, MMU_TLBE_DSIZ_64KB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_LITTLE_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_4, 0b1)},
//        {MMU_TLB_ENTRY(0x010, 0x80000, 0x80000, MMU_TLBE_DSIZ_64KB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_LITTLE_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_1, 0b1)},
//        {MMU_TLB_ENTRY(0x010, 0x80010, 0x80010, MMU_TLBE_DSIZ_64KB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_LITTLE_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_0, MMU_TLBWE_BE_2, 0b1)},
};

#define SIZE_TEST 4096

void prepare_data(){
    rumboot_printf("Filling memory with some data\n");
    int i; 
    void *dptr = 0;
    for(i=0;i<SIZE_TEST / 4; i+=4) {
        iowrite32(0xdeadf00d, i);
    }
    asm("msync");
    rumboot_printf("done\n");
}

int main(void)
{
    
    rumboot_printf("Hello, I'll try to benchmark caching\n");
    emi_init( DCR_EM2_EMI_BASE );
    rumboot_platform_perf("TLB Configuration");
    write_tlb_entries(cached_tlb, ARRAY_SIZE(cached_tlb));
    rumboot_platform_perf(NULL);


    prepare_data();
    rumboot_printf("Flushing!\n");
    rumboot_platform_perf("Cache flush");
    cache_flush_range(0, SIZE_TEST);
    rumboot_platform_perf(NULL);

    prepare_data();
    rumboot_printf("Invalidating!\n");
    rumboot_platform_perf("Cache invalidate (I+D)");
    cache_inval_range(0, SIZE_TEST, CAHEOP_DATA | CAHEOP_INSTR);
    rumboot_platform_perf(NULL);

    prepare_data();
    rumboot_printf("Invalidating!\n");
    rumboot_platform_perf("Cache invalidate (I)");
    cache_inval_range(0, SIZE_TEST, CAHEOP_INSTR);
    rumboot_platform_perf(NULL);

    prepare_data();
    rumboot_printf("Invalidating!\n");
    rumboot_platform_perf("Cache invalidate (D)");
    cache_inval_range(0, SIZE_TEST, CAHEOP_DATA);
    rumboot_platform_perf(NULL);

    rumboot_printf("Test complete, have a look at the html report!\n");


    return 0;
}
