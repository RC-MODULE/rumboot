#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/memfill.h>
#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <rumboot/rumboot.h>
#include <arch/ppc_476fp_mmu.h>
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
#define TLB_ENTRY_CACHE_OFF   MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
static const tlb_entry em_tlb_entry_cache_on  = {TLB_ENTRY_CACHE_ON };
static const tlb_entry em_tlb_entry_cache_off = {TLB_ENTRY_CACHE_OFF};

extern void working_function(uint32_t** addr, uint32_t length_in_words);

extern void working_function_no_cache(uint32_t** addr, uint32_t length_in_words, uint32_t* test_data_addr);

#define COUNT_AREAS 11
int main()
{
    uint32_t* rdf_buf[COUNT_AREAS];
    uint32_t  result = 0;

    emi_init(DCR_EM2_EMI_BASE);
    rumboot_memfill8_modelling((void*)SRAM0_BASE,0x1000,0,0);
    rumboot_memfill8_modelling((void*)SSRAM_BASE,0x10000,0,0);

    for(uint32_t i = 1; i < COUNT_AREAS; ++i)
    {
        rdf_buf[i] = rumboot_malloc_from_named_heap_aligned("SSRAM", sizeof(test_data), 64);
        if((rdf_buf[i] == NULL))
            return 1;
    }
    rdf_buf[0] = rumboot_malloc_from_named_heap_aligned("IM1", sizeof(test_data), 64);
    if( (rdf_buf[0] == NULL))
        return 1;

    for(uint32_t i = 0; i < COUNT_AREAS ; ++i)
    {
        memcpy(rdf_buf[i],test_data, sizeof(test_data));
    }

    rumboot_printf("Now starting main test code sequence. Wait about 5 ms, the sequence should not be interfered with messages.\n");
    write_tlb_entries(&em_tlb_entry_cache_on, 1);
    msync();
    isync();

    working_function(rdf_buf, (sizeof(test_data) >> 2));
    for(uint32_t addr = 0; addr < sizeof(test_data); addr+=0x20)
        for(uint32_t j = 1; j < COUNT_AREAS; ++j)
            dcbf(addr + rdf_buf[j]);
    ici(0);
    dci(2);
    isync();
    rumboot_printf("Main test code sequence finished. Starting checks. Zero is an ok result.\n");
    for(uint32_t i = 0; i < sizeof(test_data) >> 2; ++i)
        test_data[i] ^= 0xFFFFFFFF;
    for(uint32_t j = 0; j < COUNT_AREAS; ++j)
    {
        result |= memcmp(rdf_buf[j], test_data, sizeof(test_data));
        rumboot_printf("rdf[%d]: result == 0x%x\n", j, result);
    }
    rumboot_printf("Main test code sequence checked. Starting supplementary test code sequence for oi10_tb.u_OI10.u_HLB_CPU.mPPC476L2C0.uPPC476L2C.uPPC476L2C_WDF.uPPC476L2C_WDF_BUFF3_GENERICRF.\n");

    write_tlb_entries(&em_tlb_entry_cache_off, 1);
    msync();
    isync();

    working_function_no_cache(rdf_buf, (sizeof(test_data) >> 2), test_data);
    rumboot_printf("Supplementary test code sequence finished. Starting checks. Zero is an ok result.\n");
    for(uint32_t j = 0; j < COUNT_AREAS; ++j)
    {
        result |= memcmp(rdf_buf[j], test_data, sizeof(test_data));
        rumboot_printf("rdf[%d]: result == 0x%x\n", j, result);
    }

    return ((result & 0xff) | ((result >> 8) & 0xff) | ((result >> 16) & 0xff) | ((result >> 24) & 0xff));
}
