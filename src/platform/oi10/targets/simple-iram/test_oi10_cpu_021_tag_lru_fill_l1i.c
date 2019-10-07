/*
 * test_oi10_cpu_021_tag_lru_fill_l1i.c
 *
 *  Created on: JAN 10, 2019
 *      Author: s.chernousov
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
#include <platform/devices/l2c.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_l1c_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>


#define START_ADDR      (SRAM0_BASE+L1I_SIZE)   // 32 byte aligned
#define L1C_LINES_N     (L1C_INDEXES_N*L1C_WAYS_N)
#define CODE_BLR_INSTR  0x4e800020


asm (   // this 4 words will be copied in EM2
ASM_TEXT(   .section ".text","ax",@progbits                             )
ASM_TEXT(   .align 4                                                    )
ASM_TEXT(   icread_test_code_template:                                  )
ASM_TEXT(       mfctr           r3                                      )   // get current L1I line addr
ASM_TEXT(       addi            r3, r3, 32                              )
ASM_TEXT(       mtctr           r3                                      )
ASM_TEXT(       bctr                                                    )   // goto next L1I line
);
void icread_test_code_template(void);

static void init_test_data( uint32_t const template_code_addr, uint32_t test_code_addr ) {
    rumboot_printf( "Init data...\n" );
    rumboot_memfill8_modelling( (void*)START_ADDR,  L1I_SIZE, 0x00, 0x00 );
    for( uint32_t ind = 0; ind < L1C_LINES_N; ind++, test_code_addr += L1C_LINE_SIZE ) {
        //1 counter pceudocode
        iowrite32( ind,                                             test_code_addr );
#define COUNTER_SIZE    sizeof(uint32_t)
        //4 template words code
        iowrite64( ioread64(template_code_addr),                    test_code_addr+COUNTER_SIZE );
        iowrite64( ioread64(template_code_addr+sizeof(uint64_t)),   test_code_addr+COUNTER_SIZE+sizeof(uint64_t) );
    }
#define TEST_CODE_SIZE  (COUNTER_SIZE + 2*sizeof(uint64_t))
    iowrite32( CODE_BLR_INSTR, test_code_addr - ((L1C_LINE_SIZE-TEST_CODE_SIZE)+sizeof(uint32_t)) ); // replace last bctr with blr
    msync();
    rumboot_printf( "Finished\n" );
}

static bool check_with_icread( uint32_t test_code_addr ) {
    rumboot_printf( "Check with icread. Number of elements = %d\n", L1C_LINES_N );
    uint32_t icdbdr0, icdbtrl, icdbtrh;
    for( uint32_t ind = 0; ind < L1C_LINES_N; ind++, test_code_addr += L1C_LINE_SIZE ) {
        icread((void*)test_code_addr);
        isync();

        icdbtrh = spr_read(SPR_ICDBTRH);
        rumboot_printf( "ind: %d, addr: 0x%x\n", ind, test_code_addr );
        if( !(icdbtrh & XCDBTRH_VALID_mask) ) {
            rumboot_printf( "ERROR: Valid bit in ICDBTRH not expected\n" );
            return false;
        }
        if( (icdbtrh & XCDBTRH_TAG_ADDR_mask) != (test_code_addr & XCDBTRH_TAG_ADDR_mask) ) {
            rumboot_printf( "ERROR: Invalid tag in ICDBTRH\n" );
            return false;
        }
        icdbdr0 = spr_read(SPR_ICDBDR0);
        if( ind != icdbdr0 ) {
            rumboot_printf( "ERROR: Invalid instruction code in ICDBDR0\n" );
            return false;
        }
    }
    icdbtrl = spr_read(SPR_ICDBTRL);
    if ( (icdbtrl & ICDBTRL_LRUV_mask) != ICDBTRL_LRUV_mask ) {
        rumboot_printf( "ERROR: Invalid LRUV in ICDBTRL\n" );
        return false;
    }
    return true;
}


int main( void ) {
    rumboot_printf( "Emi init\n" );
    emi_init(DCR_EM2_EMI_BASE);
//*******************************************
    init_test_data( (uint32_t)&icread_test_code_template, START_ADDR );
//*******************************************
    rumboot_printf( "Set tlb (l1, l2 cache on)\n" );
    static const tlb_entry sram0_tlb_entry[] = {
//       MMU_TLB_ENTRY( ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY( 0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        {MMU_TLB_ENTRY( 0x000,  0x00008,    0x00008,    MMU_TLBE_DSIZ_16KB,     0b0,    0b0,    0b0,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY( 0x000,  0x0000C,    0x0000C,    MMU_TLBE_DSIZ_16KB,     0b0,    0b0,    0b0,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    write_tlb_entries( sram0_tlb_entry, ARRAY_SIZE(sram0_tlb_entry) );
//*******************************************
    rumboot_printf( "Call test func from SRAM0_BASE\n" );
    typedef void func();
    asm volatile
    (
        "mtctr %0\n\t"
        ::"r"(START_ADDR+COUNTER_SIZE)
    );
    ((func*)(START_ADDR+COUNTER_SIZE))();
//*******************************************
    if( !check_with_icread( START_ADDR ) ) {
        return 1;
    }

    return 0;
}
