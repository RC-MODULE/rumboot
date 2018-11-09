/*
 * test_oi10_em2_108.c
 *
 *  Created on: Jul 26, 2018
 *      Author: a.tarasov
 */


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>

typedef enum
{
    wr_rd = 1,
    rd_o = 0
} flush_state;

#define SRAM0_OFFSET_WT 0x3000
#define SRAM0_OFFSET_WB 0x4000
#define NUM_ELEM 		0x100

void check_sram0_8 (uint32_t addr, flush_state state)
{
    uint8_t check_arr8[] =
    {
        [0] = 0x00,
        [1] = 0x11,
        [2] = 0x22,
        [3] = 0x33,
        [4] = 0x44,
        [5] = 0x55,
        [6] = 0x66,
        [7] = 0x77,
        [8] = 0x88,
        [9] = 0x99,
        [10] = 0xaa,
        [11] = 0xbb,
        [12] = 0xcc,
        [13] = 0xdd,
        [14] = 0xee,
        [15] = 0xff,
        [16] = 0x00,
        [17] = 0x11,
        [18] = 0x22,
        [19] = 0x33,
        [20] = 0x44,
        [21] = 0x55,
        [22] = 0x66,
        [23] = 0x77,
        [24] = 0x88,
        [25] = 0x99,
        [26] = 0xaa,
        [27] = 0xbb,
        [28] = 0xcc,
        [29] = 0xdd,
        [30] = 0xee,
        [31] = 0xff,
    };
    uint32_t i = 0;
    if (state){
        //write
        for (i = 0; i < 32; i++)
        {
            iowrite8 (check_arr8[i], addr + i);
        }
        //read
        for (i = 0; i < 32; i++)
        {
            TEST_ASSERT (ioread8(addr+i) == check_arr8[i],"ERROR: read value is wrong off flush");
        }
    }

    else {
        //read
        for (i = 0; i < 32; i++)
        {
            TEST_ASSERT (ioread8(addr+i) == check_arr8[i],"ERROR: read value is wrong");
        }
    }

}

void check_sram0_16 (uint32_t addr, flush_state state)
{
    uint16_t check_arr16[] =
    {
        [0] = 0x0000,
        [1] = 0x1111,
        [2] = 0x2222,
        [3] = 0x3333,
        [4] = 0x4444,
        [5] = 0x5555,
        [6] = 0x6666,
        [7] = 0x7777,
        [8] = 0x8888,
        [9] = 0x9999,
        [10] = 0xaaaa,
        [11] = 0xbbbb,
        [12] = 0xcccc,
        [13] = 0xdddd,
        [14] = 0xeeee,
        [15] = 0xffff
    };
    uint32_t i = 0;
    if (state){
                //write
                for (i = 0; i < 16; i++)
                {
                    iowrite16 (check_arr16[i], addr + i*2);
                }
                //read
                for (i = 0; i < 16; i++)
                {
                    TEST_ASSERT (ioread16(addr + i*2) == check_arr16[i],"ERROR: read value is wrong off flush");
                }
    }

    else {
        //read
        for (i = 0; i < 16; i++)
        {
            TEST_ASSERT (ioread16(addr + i*2) == check_arr16[i],"ERROR: read value is wrong");
        }
    }

}

void check_sram0_32 (uint32_t addr, flush_state state)
{
    uint32_t check_arr32[] =
    {
        [0] = 0x00000000,
        [1] = 0x11111111,
        [2] = 0x22222222,
        [3] = 0x33333333,
        [4] = 0x44444444,
        [5] = 0x55555555,
        [6] = 0x66666666,
        [7] = 0x77777777
    };
    uint32_t i = 0;
    if (state){
                //write
                for (i = 0; i < 8; i++)
                {
                    iowrite32 (check_arr32[i], addr + i*4);
                }
                //read
                for (i = 0; i < 8; i++)
                {
                    TEST_ASSERT (ioread32(addr + i*4) == check_arr32[i],"ERROR: read value is wrong off flush");
                }
    }

    else {
        //read
        for (i = 0; i < 8; i++)
        {
            TEST_ASSERT (ioread32(addr + i*4) == check_arr32[i],"ERROR: read value is wrong");
        }
    }

}

void check_sram0_64 (uint32_t addr, flush_state state)
{
    uint64_t check_arr64[] =
    {
        [0] = 0x0011223344556677ULL,
        [1] = 0x8899aabbccddeeffULL,
        [2] = 0x0011223344556677ULL,
        [3] = 0x8899aabbccddeeffULL
    };
    uint32_t i = 0;
    if (state){
                //write
                for (i = 0; i < 4; i++)
                {
                    iowrite64 (check_arr64[i], addr + i * 8);
                }

                //read
                for (i = 0; i < 4; i++)
                {
                    TEST_ASSERT (ioread64(addr + i*8) == check_arr64[i],"ERROR: read value is wrong off flush");
                }

        }

    else{
        //read
        for (i = 0; i < 4; i++)
        {
            TEST_ASSERT (ioread64(addr + i*8) == check_arr64[i],"ERROR: read value is wrong");
        }
    }

}

void check_sram0 (const uint32_t addr, flush_state state)
{
    rumboot_printf ("START CHECK SRAM0\n");
    rumboot_printf ("START ADDR = %x\n", addr);

    rumboot_printf ("SIZE_8\n");
    check_sram0_8(addr, state);

    rumboot_printf ("SIZE_16\n");
    check_sram0_16(addr + 32, state);

    rumboot_printf ("SIZE_32\n");
    check_sram0_32(addr + 64, state);

    rumboot_printf ("SIZE_64\n");
    check_sram0_64(addr + 96, state);

    rumboot_printf ("CHECK W/R SRAM0 SUCCESSFUL\n");
}


/*                          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
#define TLB_ENTRY_CACHE_WT  MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,     MMU_TLBWE_WAY_3,      MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_WB  MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,     MMU_TLBWE_WAY_3,      MMU_TLBWE_BE_UND,   0b1 )

int main ()
{
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf ("Init data\n");
    uint32_t addr = 0;
    uint32_t i = 0;

    addr = SRAM0_BASE + SRAM0_OFFSET_WT;
    for (i = 0; i < NUM_ELEM; i++)
      iowrite32 (0x00, addr + i*4);

    addr = SRAM0_BASE + SRAM0_OFFSET_WB;
    for (i = 0; i < NUM_ELEM; i++)
      iowrite32 (0x00, addr + i*4);
    dci(2);

    rumboot_printf ("CACHE WT\n");
    static const tlb_entry sram0_tlb_entry_wt = {TLB_ENTRY_CACHE_WT};
    write_tlb_entries(&sram0_tlb_entry_wt,1);

    rumboot_printf ("WRITE and READ \n");
    check_sram0 (SRAM0_BASE +  SRAM0_OFFSET_WT + 1, wr_rd);
    dcbf ((void*)addr); //flush

    rumboot_printf ("READ ONLY \n");
    check_sram0 (SRAM0_BASE +  SRAM0_OFFSET_WT + 1, rd_o);

    dci (2);

    rumboot_printf ("CACHE WB\n");
    static const tlb_entry sram0_tlb_entry_wb = {TLB_ENTRY_CACHE_WB};
    write_tlb_entries(&sram0_tlb_entry_wb,1);

    rumboot_printf ("WRITE and READ \n");
    check_sram0 (SRAM0_BASE +  SRAM0_OFFSET_WB + 2, wr_rd);
    dcbf ((void*)addr);//flush
    dci (2);
    rumboot_printf ("READ ONLY \n");
    check_sram0 (SRAM0_BASE + SRAM0_OFFSET_WB + 2, rd_o);


    rumboot_printf ("TEST_OK\n");
    return 0;
}

