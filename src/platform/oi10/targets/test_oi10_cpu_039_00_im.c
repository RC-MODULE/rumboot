/*
 * test_oi10_cpu_039_00.c
 *
 *  Created on: Dec 21, 2018
 *      Author: m.smolina
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/platform.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>
#include <arch/ppc_476fp_mmu.h>
#include <devices/ugly/emi.h>

typedef uint8_t func();

#define TLB_ENTRY_WT    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

static const tlb_entry l2c_tlb_entries[] = {
//MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,             IL1I,            IL1D,           W,         I,    M,    G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,           V   )
{MMU_TLB_ENTRY(  0x020,  0xC0000,    0xC0000,    MMU_TLBE_DSIZ_64KB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
{MMU_TLB_ENTRY(  0x020,  0xC0010,    0xC0010,    MMU_TLBE_DSIZ_64KB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
{MMU_TLB_ENTRY(  0x020,  0xC0040,    0xC0020,    MMU_TLBE_DSIZ_64KB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
{MMU_TLB_ENTRY(  0x020,  0xC0050,    0xC0030,    MMU_TLBE_DSIZ_64KB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
{MMU_TLB_ENTRY(  0x000,  0x00040,    0xC0040,    MMU_TLBE_DSIZ_64KB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
{MMU_TLB_ENTRY(  0x000,  0x00050,    0xC0050,    MMU_TLBE_DSIZ_64KB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
{MMU_TLB_ENTRY(  0x000,  0x00060,    0xC0060,    MMU_TLBE_DSIZ_64KB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
{MMU_TLB_ENTRY(  0x000,  0x00070,    0xC0070,    MMU_TLBE_DSIZ_64KB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
};

void sync_for_test() {
 asm volatile (
  "isync                  \n\t"  // 
  "msync                  \n\t"  // 
  :::
  );
};

void init_ppc_for_test() {
 asm volatile (
// initialize MMUCR
  ".set  r0, 0               \n\t"
//  "addis r0, r0, 0x0000   \n\t"  // 0x0000
//  "ori   r0, r0, 0x0000   \n\t"  // 0x0000
//  "mtspr 946, r0          \n\t"  // SPR_MMUCR, 946
//  "mtspr 48,  r0          \n\t"  // SPR_PID,    48
// Set up TLB search priority
  "addis r0, r0, 0x1234   \n\t"  // 0x1234
  "ori   r0, r0, 0x5678   \n\t"  // 0x5678  
  "mtspr 830, r0          \n\t"  // SSPCR, 830
  "mtspr 831, r0          \n\t"  // USPCR, 831
  "mtspr 829, r0          \n\t"  // ISPCR, 829
  "isync                  \n\t"  // 
  "msync                  \n\t"  // 
  :::"r0"
  );
};

uint32_t invalidate_tlb_entry2 (uint32_t eaddr) {
// 1. tlbsx RT, RA, RB Rc
// EA = (RA) + (RB)
// if RA = 0, EA = 0 + (RB)
// if Rc = 1
//  CR[CR0[0]] <- 0
//  CR[CR0[1]] <- 0
//  CR[CR0[3]] <- XER[S0]
// if (a valid entry)
//  RT[33:34] <- Way Hit, [1:2]
//  RT[40:47] <- Index  , [8:15]
//  if Rc = 1, CR[CR0[2]] <- 1
// else
//  (RT) <- undefined
//  if Rc = 1, CR[CR0[2]] <- 0
// 2. tlbwe RS, RA, WS
// if TLB[RA(36)] = 1, tlb way to be written is '0' // RA(4)
// else if TLB[RA(36)] = 0 and TLB[RA(32)] = 1, tlb entry way is specified by TLB[RA(33:34)] // RA(4), RA(0), RA(1:2)
// else provided by the way counter corresponding to the index address

uint32_t res;
asm volatile (
 ".set   r0, 0               \n\t"
 ".set   r3, 3               \n\t"
 ".set   r4, 4               \n\t"

 "addis  %0, r0, 0x0000  \n\t"  // res  = 0x00000000
 "addis  r3, r0, eaddr@h    \n\t"  // [r3] = 0x4000_0000, (%2)
 "ori    r3, r3, eaddr@l    \n\t"  // [r3] = 0x4000_03F0, (%1) /
 "tlbsx. r4, r0, r3      \n\t"  // [r3] - EA, [r4] - Way, Index
 "bne ends               \n\t"  // branch if not found
 "oris   r4, r4, 0x8000  \n\t"  // r4[0]=1, r4[4]=0
 "tlbwe  r3, r4, 0       \n\t"  // [r3] - EA[0:19], V[20], [r4]- Way[1:2], Index[8:15], index is NU
 "ends:or     %0, r4, r4 \n\t"

 "isync                  \n\t"  // 
 "msync                  \n\t"  // 
 : "=r"(res) 
 : "r"(eaddr)
 : "r0", "r3", "r4"
 );

 return res;
};

int main(void)
{
  uint32_t res=0;

    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Init mem... ");
    memset((void*)(SRAM0_BASE), 0x00, 0x28000);
    rumboot_printf("done\n");

    rumboot_printf("Load bin from 0x%x\n", M_BASE);
    rumboot_platform_request_file("MBIN", M_BASE);

    rumboot_printf("Init tlb for code (set WT/WB)\n");
    static const tlb_entry sram0_tlb_entry_wt = {TLB_ENTRY_WT};
    write_tlb_entries(&sram0_tlb_entry_wt,1);
// remove the old TLB entry
    init_ppc_for_test();
    res = res | invalidate_tlb_entry2(0xC0000000); res = res << 1;
    res = res | invalidate_tlb_entry2(0xC0010000); res = res << 1;
    res = res | invalidate_tlb_entry2(0xC0020000); res = res << 1;
    res = res | invalidate_tlb_entry2(0xC0030000); res = res << 1;
    res = res | invalidate_tlb_entry2(0xC0040000); res = res << 1;
    res = res | invalidate_tlb_entry2(0xC0050000); res = res << 1;
    res = res | invalidate_tlb_entry2(0xC0060000); res = res << 1;
    res = res | invalidate_tlb_entry2(0xC0070000); res = res << 1;
    rumboot_printf("Invalidate res 0x%x\n", res);
//
    rumboot_printf("Init tlb for data (set WT/WB)\n");   
    write_tlb_entries(l2c_tlb_entries, ARRAY_SIZE(l2c_tlb_entries));
    sync_for_test();

    rumboot_printf("Starting test code from another memory\n");
    func* f = (func*)(M_BASE);
    uint8_t result = f();

    if (result == 0)
      rumboot_printf("TEST_OK\n");
    else
      rumboot_printf("TEST_ERROR\n");

    return result;
}
