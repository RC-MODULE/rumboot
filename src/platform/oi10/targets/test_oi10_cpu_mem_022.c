#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/memfill.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <rumboot/rumboot.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/test_event_c.h>

static uint32_t test_data[] =
{
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,

0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,

0x00000000,0x00000000,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000000,

0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,
0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,

0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,
0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,

0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,
0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,

0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,
0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,

0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,
0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,

0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,
0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,

0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,
0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,
};

/*                            MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
#define TLB_ENTRY_CACHE_ON    MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
static const tlb_entry em_tlb_entry_cache_on = {TLB_ENTRY_CACHE_ON};

extern void working_function(uint32_t** addr, uint32_t length_in_words);
//{
//    for(int i = 0; i < 10; ++i)
//        msync();
//    return;
//}
#define COUNT_AREAS 11
int main()
{
    uint32_t* rdf_buf[COUNT_AREAS];
    uint32_t* wdf_buf;
//    register uint32_t current_word;
//    register uint32_t rdf0_word;
//    register uint32_t rdf1_word;
//    register uint32_t rdf2_word;
//    register uint32_t rdf3_word;
//    register uint32_t rdf4_word;
//    register uint32_t rdf5_word;
//    register uint32_t rdf6_word;
//    register uint32_t rdf7_word;
//    register uint32_t rdf0_addr;
    uint32_t  result = 0;
//    uint32_t    count_nops = 0;
    emi_init(DCR_EM2_EMI_BASE);
    rumboot_memfill8_modelling((void*)SRAM0_BASE,0x1000,0,0);
    rumboot_memfill8_modelling((void*)SSRAM_BASE,0x10000,0,0);
    rumboot_memfill32((void*)IM1_BASE, 0x1000,0,1);

    for(uint32_t i = 1; i < COUNT_AREAS; ++i)
    {
        rdf_buf[i] = rumboot_malloc_from_named_heap_aligned("SSRAM", sizeof(test_data), 64);
        if((rdf_buf[i] == NULL))
            return 1;
    }
        rdf_buf[0] = rumboot_malloc_from_named_heap_aligned("IM1", sizeof(test_data), 64);
//    rdf0_addr = (uint32_t)rdf_buf[0];
    wdf_buf = rumboot_malloc_from_named_heap_aligned("SSRAM", sizeof(test_data), 64);
    if( (wdf_buf == NULL))
        return 1;

    for(uint32_t i = 0; i < COUNT_AREAS ; ++i)
    {
        rumboot_memfill8(rdf_buf[i], sizeof(test_data), (uint8_t)(i & 0xFF), 0);
        memcpy(rdf_buf[i],test_data, sizeof(test_data));
    }
//    rdf_buf[COUNT_AREAS - 1] = test_data;
    write_tlb_entries(&em_tlb_entry_cache_on, 1);
    msync();
    isync();

//    for(int i = 0; i < 20; ++i)
//    {
//        switch(i % 11)
//        {
//            case 10:    ++count_nops;
//            case 9:     ++count_nops;
//            case 8:     ++count_nops;
//            case 7:     ++count_nops;
//            case 6:     ++count_nops;
//            case 5:     ++count_nops;
//            case 4:     ++count_nops;
//            case 3:     ++count_nops;
//            case 2:     ++count_nops;
//            case 1:     ++count_nops;
//        }
        working_function(rdf_buf, (sizeof(test_data) >> 2));
        for(uint32_t addr = 0; addr < sizeof(test_data); addr+=0x20)
            for(uint32_t j = 1; j < COUNT_AREAS; ++j)
                dcbf(addr + rdf_buf[j]);
        ici(0);
        dci(2);
        isync();
//        rumboot_putdump((uint32_t)test_data, sizeof(test_data));
        for(uint32_t j = 0; j < COUNT_AREAS; ++j)
        {
//            rumboot_putdump((uint32_t)rdf_buf[j], sizeof(test_data));
            result |= memcmp(rdf_buf[j], test_data, sizeof(test_data));
            result |= (ioread32((uint32_t)rdf_buf[j] + sizeof(test_data)) == ((j & 0xff) | ((j & 0xff) << 8) | ((j & 0xff) << 16) | ((j & 0xff) << 24)));
            rumboot_printf("rdf[%d]: result == 0x%x\n", j, result);
        }
//        buf += count_nops;

//    }
//    for(uint32_t addr = 0; addr < sizeof(test_data); addr += 4)
//    {
//        current_word = ioread32((uint32_t)(test_data + addr));
////        asm(
//////                ".align 16\n\t"
////                "lwzux %0, %1, %2 \n\t"
////                : "=r"(rdf0_word)
////                : "r"(addr),
////                  "r"(rdf0_addr)
////        );
//        asm(
//                ".align 4\n\t"
//                "lwzux %0, %8, %9 \n\t"
//                "lwzux %1, %8, %10\n\t"
//                "lwzux %2, %8, %11\n\t"
//                "lwzux %3, %8, %12\n\t"
//                "lwzux %4, %8, %13\n\t"
//                "lwzux %5, %8, %14\n\t"
//                "lwzux %6, %8, %15\n\t"
//                "lwzux %7, %8, %16\n\t"
//                : "=r"(rdf0_word),
//                  "=r"(rdf1_word),
//                  "=r"(rdf2_word),
//                  "=r"(rdf3_word),
//                  "=r"(rdf4_word),
//                  "=r"(rdf5_word),
//                  "=r"(rdf6_word),
//                  "=r"(rdf7_word)
//                : "r"(addr),
//                  "r"(rdf_buf[0]),
//                  "r"(rdf_buf[1]),
//                  "r"(rdf_buf[2]),
//                  "r"(rdf_buf[3]),
//                  "r"(rdf_buf[4]),
//                  "r"(rdf_buf[5]),
//                  "r"(rdf_buf[6]),
//                  "r"(rdf_buf[7])
//        );
//        if(current_word != rdf0_word)
//        {
//            result = 1;
//            rumboot_printf("rdf: data mismatch: test_data[0x%x] == 0x%x, rdf_buf[0][0x%x] == 0x%x",
//                    addr,current_word, addr, rdf0_word);
//        }
//        if(current_word != rdf1_word)
//        {
//            result = 1;
//            rumboot_printf("rdf: data mismatch: test_data[0x%x] == 0x%x, rdf_buf[1][0x%x] == 0x%x",
//                    addr,current_word, addr, rdf1_word);
//        }
//        if(current_word != rdf2_word)
//        {
//            result = 1;
//            rumboot_printf("rdf: data mismatch: test_data[0x%x] == 0x%x, rdf_buf[2][0x%x] == 0x%x",
//                    addr,current_word, addr, rdf2_word);
//        }
//        if(current_word != rdf3_word)
//        {
//            result = 1;
//            rumboot_printf("rdf: data mismatch: test_data[0x%x] == 0x%x, rdf_buf[3][0x%x] == 0x%x",
//                    addr,current_word, addr, rdf3_word);
//        }
//        if(current_word != rdf4_word)
//        {
//            result = 1;
//            rumboot_printf("rdf: data mismatch: test_data[0x%x] == 0x%x, rdf_buf[4][0x%x] == 0x%x",
//                    addr,current_word, addr, rdf4_word);
//        }
//        if(current_word != rdf5_word)
//        {
//            result = 1;
//            rumboot_printf("rdf: data mismatch: test_data[0x%x] == 0x%x, rdf_buf[5][0x%x] == 0x%x",
//                    addr,current_word, addr, rdf5_word);
//        }
//        if(current_word != rdf6_word)
//        {
//            result = 1;
//            rumboot_printf("rdf: data mismatch: test_data[0x%x] == 0x%x, rdf_buf[6][0x%x] == 0x%x",
//                    addr,current_word, addr, rdf6_word);
//        }
//        if(current_word != rdf7_word)
//        {
//            result = 1;
//            rumboot_printf("rdf: data mismatch: test_data[0x%x] == 0x%x, rdf_buf[7][0x%x] == 0x%x",
//                    addr,current_word, addr, rdf7_word);
//        }
//    }

    return ((result & 0xff) | ((result >> 8) & 0xff) | ((result >> 16) & 0xff) | ((result >> 24) & 0xff));
}
