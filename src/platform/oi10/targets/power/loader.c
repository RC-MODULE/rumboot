#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <rumboot/boot.h>
#include <rumboot/xmodem.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/macros.h>
#include <algo/crc32.h>
#include <rumboot/memtest.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#define IM0_BASE_UNCACHED 0x80000000
#ifdef ENABLE_CACHE
#undef IM0_BASE
#define IM0_BASE 0x80060000
#endif

#define PPC476FP_L1_CACHELINE_SIZE 32u  // Длина кэш строки L1 в байтах
#define PPC476FP_L2_CACHELINE_SIZE 128u // Длина кэш строки L2 в байтах

void flush_cache_range(void *start, unsigned long sizeInBytes)
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

        // Сбрасываем все данные из диапазона из кэша L2 в память
        // The dcbst instruction flushes dirty data from the L2 cache. This
        // instruction is effectively a no-op for the L1 because the L1 cache is
        // write-through only and cannot contain dirty data.
        // for (addr = range_start; addr < range_end; addr += PPC476FP_L2_CACHELINE_SIZE)
        // {
        //     asm volatile (
        //         "dcbst 0, %0\n"
        //         "msync\n"
        //         :
        //         :"r"(addr)
        //     );
        // }

        // Инвалидируем данные из диапазона из кэша данных, чтобы при следующем
        // использовании данные были загружены из памяти
        // dcbf сбрасывает 4 строки DL1 (и соответственно одну строку L2)
        rumboot_printf("Invalidating data cache...\n");
        for (addr = range_start; addr < range_end; addr += PPC476FP_L2_CACHELINE_SIZE)
        {
                asm volatile(
                    "dcbf 0, %0\n"
                    "msync\n"
                    :
                    : "r"(addr));
        }

        // Инвалидируем данные из диапазона из кэша инструкций, чтобы при следующем
        // использовании инструкции были загружены из памяти (важно для установки и
        // снятия точек установа)
        mask_line_base = (~(PPC476FP_L1_CACHELINE_SIZE - 1));
        range_start = start_addr & mask_line_base;
        range_end = (end_addr & mask_line_base) + PPC476FP_L1_CACHELINE_SIZE;

        rumboot_printf("Invalidating instruction cache...\n");
        for (addr = range_start; addr < range_end; addr += PPC476FP_L1_CACHELINE_SIZE)
        {
                asm volatile(
                    "icbi 0, %0\n"
                    "isync\n"
                    :
                    : "r"(addr));
        }
}

// Cache IM0-M:                   5368716 us
// No Cache IM0-M: Iteration took 4283273619 us
// NO Store
// With cache: Iteration took 1006649 us
// No cache    Iteration took 4289934172 us

//#define MIRROR_CACHE
//#define MIRROR_NO_CACHE
//#define MAIN_CACHE
#include <platform/rom_api_r1.h>

//rumboot: micro-trampoline to IM0, self load address at 0x80020000
//Reading header @ 0x0
//Next header @ 0xb420
//Reading header @ 0xb420
//PANIC: No more valid images

static size_t find_my_end(struct rumboot_bootheader *self, void *bsrc, void *pdata)
{
        char tmp[512];
        off_t curpos = 0; 
        while (1) {
                rumboot_printf("Reading header @ 0x%x\n", curpos);
                size_t rd = rom_bootsource_read(bsrc, pdata, tmp, curpos, sizeof(struct rumboot_bootheader));
                struct rumboot_bootheader *hdr = (void *) tmp;
                rumboot_printf("read %d bytes\n", rd);
                rumboot_printf("magic: 0x%x curpos:0x%x datalen:0x%x HDRsz 0x%x\n", hdr->magic, curpos, hdr->datalen, sizeof(*hdr));

                if (hdr->magic != RUMBOOT_HEADER_MAGIC) {
                        rumboot_printf("magic: 0x%x curpos:0x%x datalen:0x%x\n", hdr->magic, curpos, hdr->datalen);
                        rumboot_platform_panic("No more valid images");
                }
                curpos += sizeof(*hdr) + hdr->datalen;

                rumboot_printf("Next header @ 0x%x\n", curpos);

                if (self->data_crc32 == hdr->data_crc32) {
                        rumboot_printf("Next binary starts @ 0x%x\n", curpos);
                        return curpos;
                }
        }
        return 0;
}


static void exec_from_cached_area()
{
#ifdef ENABLE_CACHE
        rumboot_printf("Invalidating cache...\n");
        flush_cache_range((void *)IM0_BASE, 64 * 1024);
        asm("msync");
        asm("isync");
        asm("sync");
        asm("msync");
        rumboot_printf("Done, executing...\n");
#endif
        rumboot_platform_exec((void *)IM0_BASE, 0);

}

extern char rumboot_self;
static void chainload_flash()
{
        char pdata[128];

	struct rumboot_bootheader *hdr = (struct rumboot_bootheader *) &(rumboot_self);
	rumboot_printf("rumboot: micro-trampoline to IM0, self load address at 0x%x\n", hdr);
	if (!hdr->device) {
		rom_rumboot_putstring("rumboot: micro-trampoline can't run from host");
		return;
	}
	rom_bootsource_init(hdr->device, pdata);
	struct rumboot_bootheader *dst = (void *) IM0_BASE_UNCACHED;
	size_t offset = find_my_end(hdr, hdr->device, pdata);
        rom_bootsource_read(hdr->device, pdata, dst, offset, 512);
        rom_bootsource_read(hdr->device, pdata, dst, offset, dst->datalen + sizeof(*dst));
//	rom_bootsource_try_single(hdr->device, pdata, dst, 64 * 1024, &offset);
	rom_bootsource_deinit(hdr->device, pdata);
        exec_from_cached_area();
}

static void chainload_direct()
{
        rumboot_platform_request_file("IM0BIN", (uint32_t)IM0_BASE_UNCACHED);

}

int main()
{
        rumboot_printf("=== POWER TEST HOST/SPI PRELOADER ===\n");
        static const tlb_entry im0_tlb_entry[] = {
//          MMU_TLB_ENTRY(    ERPN,   RPN,     EPN,     DSIZ,               IL1I,IL1D,W,   I,   M,   G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#if defined(ENABLE_CACHE)
                {MMU_TLB_ENTRY(0x010, 0x80000, 0x80060, MMU_TLBE_DSIZ_64KB, 0b0, 0b0, 0b1, 0b0, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_UND, MMU_TLBWE_BE_UND, 0b1)},
                {MMU_TLB_ENTRY(0x010, 0x80010, 0x80070, MMU_TLBE_DSIZ_64KB, 0b0, 0b0, 0b1, 0b0, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_UND, MMU_TLBWE_BE_UND, 0b1)},
#else
                {MMU_TLB_ENTRY(0x010, 0x80000, 0x80060, MMU_TLBE_DSIZ_64KB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_UND, MMU_TLBWE_BE_UND, 0b1)},
                {MMU_TLB_ENTRY(0x010, 0x80010, 0x80070, MMU_TLBE_DSIZ_64KB, 0b1, 0b1, 0b0, 0b1, 0b0, 0b0, MMU_TLBE_E_BIG_END, 0b0, 0b0, 0b0, 0b1, 0b1, 0b1, 0b0, 0b0, 0b0, MEM_WINDOW_SHARED, MMU_TLBWE_WAY_UND, MMU_TLBWE_BE_UND, 0b1)},
#endif
        };
        
        write_tlb_entries(im0_tlb_entry, ARRAY_SIZE(im0_tlb_entry));

        /* Disable machine check */
        uint32_t msr = msr_read();
        msr &= ~(1 << 12);
        msr_write(msr);

	struct rumboot_bootheader *hdr = (struct rumboot_bootheader *) &(rumboot_self);
        if (hdr->device) { /* We've booted from flash */
                rumboot_printf("Chain-booting IM0 code from FLASH \n");
                chainload_flash();
        } else {
                rumboot_printf("Chain-booting IM0 code from HOST \n");
                chainload_direct();
        }
        return 0;
}
