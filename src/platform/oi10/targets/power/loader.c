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

#ifdef ENABLE_CACHE
#undef IM0_BASE
#define IM0_BASE 0x80060000
#endif

#define  PPC476FP_L1_CACHELINE_SIZE   32u   // Длина кэш строки L1 в байтах
#define  PPC476FP_L2_CACHELINE_SIZE  128u   // Длина кэш строки L2 в байтах

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
    // использовании инструкции были загружены из памяти (важно для установки и
    // снятия точек установа)
    mask_line_base = ( ~( PPC476FP_L1_CACHELINE_SIZE - 1 ) );
    range_start = start_addr & mask_line_base;
    range_end = ( end_addr & mask_line_base ) + PPC476FP_L1_CACHELINE_SIZE;

    rumboot_printf("Invalidating instruction cache...\n");
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

#ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
static void hostmode_loop(struct rumboot_config *conf, void *pdata)
{
        size_t maxsize = 256 * 1024;
        struct rumboot_bootheader *hdr = (void *) IM0_BASE;
        dbg_boot(NULL, "Host Mode, please upload SPL to 0x%x", (uintptr_t) hdr);
        rumboot_platform_enter_host_mode(conf);
        dbg_boot(NULL, "Hit 'X' for X-Modem upload");
        void *data;
        int ret;

        while (1) {
                rumboot_platform_sv_event("HOST");
                #ifdef __PPC__
                /* FixMe: Use cross-platform barrier sync functions here */
                asm("msync");
                #endif

                int c = rumboot_getchar(100);
                if (c == 'X') {
                        int ret = xmodem_get((void *) hdr, maxsize);
                        mdelay(250);
                        rumboot_printf("\n\n\n");
                        if (ret > 0) {
                                dbg_boot(NULL, "Received %d bytes, executing in 100ms", ret);
                                mdelay(100);
                        } else {
                                dbg_boot(NULL, "Upload failed, code %d", ret);
                                hdr->magic = -ret;
                        }
                }

                if (c == 'e') {
                        #ifndef RUMBOOT_PLATFORM_EASTER_EGG
                        dbg_boot(NULL, "M'aiq the Liar: There are absolutely no easter eggs in bootrom code.");
                        #else
                        dbg_boot(NULL, RUMBOOT_PLATFORM_EASTER_EGG);
                        #endif
                        rumboot_platform_sv_event("EASTER_EGG");
                }

                if (hdr->device) {
                        hdr->device = NULL;
                }
                ssize_t len = rumboot_bootimage_check_header(hdr, &data);
                if (len == -EBADMAGIC) {
                        continue;
                }
                if (len < 0) {
                        dbg_boot(NULL, "Header error: %s\n", rumboot_strerror(len));
                        hdr->magic = -len;
                        continue;
                }
                
                flush_cache_range(hdr, 64*1024);
                asm("msync");
                asm("isync");                        
                asm("sync");

                rumboot_printf("GOT\n\n");
                if (0 == rumboot_bootimage_check_data(hdr)) {
                        hdr->magic = 0x0;
                        asm("msync");
                        asm("isync");                        
                        asm("sync");
                        rumboot_printf("GOT\n\n");

                        ret = rumboot_platform_exec(hdr, 0);
                        dbg_boot(NULL, "Back in rom, code %d", ret);
                        if (ret > 0) {
                                bootsource_try_by_id(ret - 1, pdata, hdr, maxsize);
                        }
                } else {
                        dbg_boot(NULL, "Data CRC32 mismatch\n");
                        hdr->magic = EBADDATACRC;
                }
        }
}

#endif

// Cache IM0-M:                   5368716 us
// No Cache IM0-M: Iteration took 4283273619 us
// NO Store
// With cache: Iteration took 1006649 us
// No cache    Iteration took 4289934172 us


//#define MIRROR_CACHE
//#define MIRROR_NO_CACHE
//#define MAIN_CACHE
int main()
{
        static const tlb_entry im0_tlb_entry[] = {
//          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#if defined(ENABLE_CACHE)
          { MMU_TLB_ENTRY(  0x010,  0x80000,    0x80060,    MMU_TLBE_DSIZ_64KB,     0b0,    0b0,    0b1,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1)},
          { MMU_TLB_ENTRY(  0x010,  0x80010,    0x80070,    MMU_TLBE_DSIZ_64KB,     0b0,    0b0,    0b1,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1)},
#else
          { MMU_TLB_ENTRY(  0x010,  0x80000,    0x80060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1)},
          { MMU_TLB_ENTRY(  0x010,  0x80010,    0x80070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1)},
#endif
    };

    write_tlb_entries(im0_tlb_entry, ARRAY_SIZE(im0_tlb_entry));


    /* Disable machine check */
    uint32_t msr = msr_read();
    msr &= ~(1 << 12);
    msr_write(msr);

  rumboot_printf("Chain-booting from IM0 \n");



#ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
#define PDATA_SIZE 128
        char pdata[PDATA_SIZE];
        struct rumboot_config conf;
        rumboot_platform_read_config(&conf);
        rumboot_platform_init_loader(&conf);
        rumboot_print_logo();
        hostmode_loop(&conf, pdata);
#else 
        rumboot_platform_request_file("IM0BIN", (uint32_t) IM0_BASE);
#ifdef ENABLE_CACHE
        rumboot_printf("Invalidating cache...\n");
        flush_cache_range((void *) IM0_BASE, 64*1024);
        asm("msync");
        asm("isync");                        
        asm("sync");
        asm ("msync");
        rumboot_printf("Done, executing...\n");
#endif
        rumboot_platform_exec((void *) IM0_BASE, 0);
#endif

    return 0;
}
