#include <rumboot/printf.h>
#include <rumboot/macros.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <unistd.h>
#include <platform/devices.h>
#include <rumboot/memtest.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>


//#define TEST_NO_CACHE
int main()
{



    rumboot_printf( "Init ext_clk \n");

    uint32_t read_data;

    dcr_write(0x8008003c,0x1ACCE551);

    read_data = dcr_read(0x8008003c);

    rumboot_printf( "read_data 0x80080130 = %x\n", read_data);

    dcr_write(0x80080130,0x7);

    read_data = dcr_read(0x80080130);

    rumboot_printf( "read_data 0x80080130 = %x\n", read_data);

    dcr_write(0x80080060,0x1);

    read_data = dcr_read(0x80080060);

    rumboot_printf( "read_data 0x80080060= %x\n", read_data);

    read_data = dcr_read(0x80080130);

    rumboot_printf( "read_data 0x80080130 = %x\n", read_data);

    rumboot_printf("Writing TLB entry for SSRAM\n");

#ifndef TEST_NO_CACHE
    static const tlb_entry sram0_tlb_entry[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
#else
    static const tlb_entry sram0_tlb_entry[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
#endif

    write_tlb_entries(sram0_tlb_entry, ARRAY_SIZE(sram0_tlb_entry));
    emi_init(DCR_EM2_EMI_BASE);
    rumboot_printf("Testing cached access\n");

    int err = memtest(SRAM0_BASE, 256 * 1024);
    rumboot_printf("%d errors detected\n", err);


    return err ? 1 : 0;
}
