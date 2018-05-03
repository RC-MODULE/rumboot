
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_mmu.S.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <regs/regs_mpic.h>
// #include <mivem_mem_map.h>



// #include "test_mpw_cpu_008_0.h"

// AG: Compatibility to old definitions
#define PLB6_BC_BASE DCR_PLB6_BC_BASE

#define PLB6_BC_CR0_ADDR			PLB6_BC_BASE + 0x00
#define PLB6_BC_PRI_ADDR			PLB6_BC_BASE + 0x01  //0-15
#define PLB6_BC_TSNOOP_ADDR			PLB6_BC_BASE + 0x02
#define PLB6_BC_PAAM_WIN_EXT_ADDR	PLB6_BC_BASE + 0x03
#define PLB6_BC_SGD1_ADDR			PLB6_BC_BASE + 0x04
#define PLB6_BC_SGD2_ADDR			PLB6_BC_BASE + 0x05
#define PLB6_BC_SGD3_ADDR			PLB6_BC_BASE + 0x06
#define PLB6_BC_SGD4_ADDR			PLB6_BC_BASE + 0x07
#define PLB6_BC_SGD5_ADDR			PLB6_BC_BASE + 0x08
#define PLB6_BC_SGD6_ADDR			PLB6_BC_BASE + 0x09
#define PLB6_BC_SGD7_ADDR			PLB6_BC_BASE + 0x0A
#define PLB6_BC_ERR_ADDR			PLB6_BC_BASE + 0x0B
#define PLB6_BC_MSRSP_ADDR			PLB6_BC_BASE + 0x0C
#define PLB6_BC_HCPP_ADDR			PLB6_BC_BASE + 0x0D //6-31
#define PLB6_BC_HD_ADDR				PLB6_BC_BASE + 0x0E
#define PLB6_BC_QD_ADDR				PLB6_BC_BASE + 0x0F
#define PLB6_BC_SHD_ADDR			PLB6_BC_BASE + 0x10
#define PLB6_BC_REVID_ADDR			PLB6_BC_BASE + 0x11

#define PLB6_BC_CR0_VALUE			0x00000000
#define PLB6_BC_PRI_VALUE			0x00000000
#define PLB6_BC_TSNOOP_VALUE		reg_field(3,0x8) //may be changed?
#define PLB6_BC_PAAM_WIN_EXT_VALUE	0xC0000000
#define PLB6_BC_SGD1_VALUE			0x10000000
#define PLB6_BC_SGD2_VALUE			0x20000000
#define PLB6_BC_SGD3_VALUE			0x30000000
#define PLB6_BC_SGD4_VALUE			0x40000000
#define PLB6_BC_SGD5_VALUE			0x50000000
#define PLB6_BC_SGD6_VALUE			0x60000000
#define PLB6_BC_SGD7_VALUE			0x70000000
#define PLB6_BC_ERR_VALUE			0x00000000
#define PLB6_BC_MSRSP_VALUE			0x00000000
#define PLB6_BC_HCPP_VALUE			0x03FFFFFF
#define PLB6_BC_HD_VALUE			0x00000000
#define PLB6_BC_QD_VALUE			0x00000000
#define PLB6_BC_SHD_VALUE			0x00000000
#define PLB6_BC_REVID_VALUE         reg_field(31,0x02) | reg_field(23, 0x01) | reg_field(11, 0x000)


#define SGD_NON_SYNTH_VALUE         0xFFFFFFF0

//DCR register value checker
.macro sgd_wr_check rE,rA,rR,raddress,reg_name
    //load address
    load_const \rA, \raddress
    //load value
    load_const \rE, SGD_NON_SYNTH_VALUE
    //write value to SGDx
    mtdcrx \rA, \rE
    //read
    mfdcrx \rR, \rA
    cmp cr7,0, \rR,\rE
    TEST_ASSERT(eq,cr7,"DCR error: In \reg_name was expected value \exp_value (address \raddress) ")
#ifdef DEBUG_DCR_TEST
    rumboot_putstring "\reg_name OK\n"
#endif
.endm

// #include <mpic128/mpic128_regs.h>
#include <platform/oi10/platform/ppc470s/mmu.S.h>

.section ".init.text","ax",@progbits

rumboot_putstring_unexpected_itrpt:
    rumboot_putstring   "Unexpected interrupt: "
    blr

.align 4
unexpected_itrpt_hdr_base:
unexpected_CI_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Critical input\n"
    b           unexpected_error

.align 4
unexpected_MC_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Machine check\n"
    b           unexpected_error

.align 4
unexpected_DS_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Data storage\n"
    b           unexpected_error

.align 4
unexpected_IS_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Instruction storage\n"
    b           unexpected_error

.align 4
unexpected_E_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "External input\n"
    b           unexpected_error

.align 4
unexpected_A_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Alignment\n"
    b           unexpected_error

.align 4
unexpected_P_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Program\n"
    b           unexpected_error

.align 4
unexpected_FPU_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Floating point unavailable\n"
    b           unexpected_error
  .align 4
unexpected_SC_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "System call\n"
    b           unexpected_error
.align 4
unexpected_APU_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Auxiliary processor unavailable\n"
    b           unexpected_error

.align 4
unexpected_DEC_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Decrementer\n"
    b           unexpected_error

.align 4
unexpected_FITI_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Fixed interval timer\n"
    b           unexpected_error

.align 4
unexpected_WTI_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Watchdog timer\n"
    b           unexpected_error

.align 4
unexpected_DTLBE_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Data TLB error\n"
    b           unexpected_error

.align 4
unexpected_ITLBE_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Instruction TLB error\n"
    b           unexpected_error

 .align 4
unexpected_D_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Debug\n"
    b           unexpected_error



.global cpu_008_entry_point
cpu_008_entry_point:
    load_const  r0, 0x00000000  // make r0 "all zeros" source
    load_const  r1, 0xFFFFFFFF  // make r1 "all ones" source

    mtspr       SPR_USPGR0, r0

    lis         r3, unexpected_itrpt_hdr_base@h
    mtspr       SPR_IVPR, r3
    li          r3, unexpected_CI_hdr@l
    mtspr       SPR_IVOR0, r3
    li          r3, unexpected_MC_hdr@l
    mtspr       SPR_IVOR1, r3
    li          r3, unexpected_DS_hdr@l
    mtspr       SPR_IVOR2, r3
    li          r3, unexpected_IS_hdr@l
    mtspr       SPR_IVOR3, r3
    li          r3, unexpected_E_hdr@l
    mtspr       SPR_IVOR4, r3
    li          r3, unexpected_A_hdr@l
    mtspr       SPR_IVOR5, r3
    li          r3, unexpected_P_hdr@l
    mtspr       SPR_IVOR6, r3
    li          r3, unexpected_FPU_hdr@l
    mtspr       SPR_IVOR7, r3
    li          r3, unexpected_SC_hdr@l
    mtspr       SPR_IVOR8, r3
    li          r3, unexpected_APU_hdr@l
    mtspr       SPR_IVOR9, r3
    li          r3, unexpected_DEC_hdr@l
    mtspr       SPR_IVOR10, r3
    li          r3, unexpected_FITI_hdr@l
    mtspr       SPR_IVOR11, r3
    li          r3, unexpected_WTI_hdr@l
    mtspr       SPR_IVOR12, r3
    li          r3, unexpected_DTLBE_hdr@l
    mtspr       SPR_IVOR13, r3
    li          r3, unexpected_ITLBE_hdr@l
    mtspr       SPR_IVOR14, r3
    li          r3, unexpected_D_hdr@l
    mtspr       SPR_IVOR15, r3

    load_const  r3, MPICx_BASE+MPIC128x_WAI_PR
    mfdcrx      r3, r3                          //read Who Am I Register
    cmpwi       r3, PPC0_CORE_ID                //if Processor ID Register is 0
    beq         ppc0_core_start                 //execute code for core 0
    cmpwi       r3, PPC1_CORE_ID                //if Processor ID Register is 1
    // AG // beq         PPC1_CORE_START                 //execute code for core 1
    // AG // b           PPC_UNEXPECTED_CORE_START       //something wrong in configuration? unexpected core number? Execute code on the unexpected core

ppc0_core_start:
goto_ppc0_core_start:
// AG //    b           PPC0_CORE_START

core_unexpected:
    test_event  EVENT_PROCESSOR_ID_UNEXPECTED


.section ".init.text","ax",@progbits

.global infinite_loop_008
infinite_loop_008:
    b infinite_loop_008

.global boot_init

boot_init:
    load_const  r2, 0x00000000
    load_const  r3, 0x00000000
    load_const  r4, 0x00000000
    load_const  r5, 0x00000000
    load_const  r6, 0x00000000
    load_const  r7, 0x00000000
    load_const  r8, 0x00000000
    load_const  r9, 0x00000000
    load_const  r10,0x00000000
    load_const  r11,0x00000000
    load_const  r12,0x00000000
    load_const  r13,0x00000000
    load_const  r14,0x00000000
    load_const  r15,0x00000000
    load_const  r16,0x00000000
    load_const  r17,0x00000000
    load_const  r18,0x00000000
    load_const  r19,0x00000000
    load_const  r20,0x00000000
    load_const  r21,0x00000000
    load_const  r22,0x00000000
    load_const  r23,0x00000000
    load_const  r24,0x00000000
    load_const  r25,0x00000000
    load_const  r26,0x00000000
    load_const  r27,0x00000000
    load_const  r28,0x00000000
    load_const  r29,0x00000000
    load_const  r30,0x00000000
    load_const  r31,0x00000000

init_spr:
//according to User's Manual ch. 9.4 Initialization Software Requirements
    mtspr       SPR_DBCR0, r0       // disable all debug events

    mtspr       SPR_DBSR_RC, r1     // clear all debug interrupts

    load_const  r3, (0b0 << CTRL_CCR0_ITE_i)\
                  | (0b1 << CTRL_CCR0_PRE_i)\
                  | (0b0 << CTRL_CCR0_CRPE_i)\
                  | (CTRL_CCR0_ICS_32byte << CTRL_CCR0_ICS_i)\
                  | (0b0 << CTRL_CCR0_DAPUIB_i)\
                  | (0b0000 << CTRL_CCR0_ICWRIDX_i)\
                  | (0b0 << CTRL_CCR0_DTB_i)\
                  | (0b0 << CTRL_CCR0_FLSTA_i)\
                  | (CTRL_CCR0_DQWPM_No_prediction << CTRL_CCR0_DQWPM_i)\
                  | (CTRL_CCR0_IQWPM_Use_EA << CTRL_CCR0_IQWPM_i)
    mtspr       SPR_CCR0, r3

    load_const  r3, (0b00 << CTRL_CCR1_GPRPEI_i)\
                  | (0b00 << CTRL_CCR1_FPRPEI_i)\
                  | (0b00 << CTRL_CCR1_ICDPEI_i)\
                  | (0b00 << CTRL_CCR1_ICLPEI_i)\
                  | (0b00 << CTRL_CCR1_ICTPEI_i)\
                  | (0b00 << CTRL_CCR1_DCDPEI_i)\
                  | (0b00 << CTRL_CCR1_DCLPEI_i)\
                  | (0b00 << CTRL_CCR1_DCTPEI_i)\
                  | (0b0 << CTRL_CCR1_MMUTPEI_i)\
                  | (0b0 << CTRL_CCR1_MMUDPEI_i)\
                  | (CTRL_CCR1_TSS_CPU_clock << CTRL_CCR1_TSS_i)\
                  | (0b0 << CTRL_CCR1_DPC_i)\
                  | (CTRL_CCR1_TCS_div1 << CTRL_CCR1_TCS_i)
    mtspr       SPR_CCR1, r3

    load_const  r3, (CTRL_CCR2_DSTG_enabled << CTRL_CCR2_DSTG_i)\
                  | (0b0 << CTRL_CCR2_DLFPD_i)\
                  | (0b0 << CTRL_CCR2_DSTI_i)\
                  | (0b0 << CTRL_CCR2_PMUD_i)\
                  | (0b0 << CTRL_CCR2_DCSTGW_i)\
                  | (0 << CTRL_CCR2_STGCTR_i)\
                  | (0b0 << CTRL_CCR2_DISTG_i)\
                  | (0b0 << CTRL_CCR2_SPC5C1_i)\
                  | (0b0 << CTRL_CCR2_MCDTO_i)
    mtspr       SPR_CCR2, r3

    load_const  r3, (0b0 << MMU_MMUBE0_VBE0_i)\
                  | (0b0 << MMU_MMUBE0_VBE1_i)\
                  | (0b0 << MMU_MMUBE0_VBE2_i)
    mtspr       SPR_MMUBE0, r3

    load_const  r3, (0b0 << MMU_MMUBE1_VBE3_i)\
                  | (0b0 << MMU_MMUBE1_VBE4_i)\
                  | (0b0 << MMU_MMUBE1_VBE5_i)
    mtspr       SPR_MMUBE1, r3

    load_const  r3, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_SSPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB) << MMU_SSPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_END << MMU_SSPCR_ORD3_i)
    mtspr       SPR_SSPCR, r3

    load_const  r3, (MMU_XSPCR_ORD_64KB << MMU_ISPCR_ORD1_i)\
                  | (MMU_XSPCR_ORD_4KB << MMU_ISPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_END << MMU_ISPCR_ORD3_i)
    mtspr       SPR_ISPCR, r3

    load_const  r3, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_USPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB) << MMU_USPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_END << MMU_USPCR_ORD3_i)
    mtspr       SPR_USPCR, r3

//Before editing TLB let's initialize all SPR registers to default values by the spec
//Most of them have X-state after reset and therefore CPU model cannot work properly
    mtspr       SPR_CTR, r0
    mtcr        r0
    mtspr       SPR_LR, r0
    mtspr       SPR_DCESR, r0
    mtspr       SPR_CSRR0, r0
    mtspr       SPR_CSRR1, r0
    mtspr       SPR_MCSRR0, r0
    mtspr       SPR_MCSRR1, r0
    mtspr       SPR_RMPD, r0

    mtspr       SPR_IAC1, r0
    mtspr       SPR_IAC2, r0
    mtspr       SPR_IAC3, r0
    mtspr       SPR_IAC4, r0
    mtspr       SPR_DAC1, r0
    mtspr       SPR_DAC2, r0
    mtspr       SPR_DVC1, r0
    mtspr       SPR_DVC2, r0
    mtspr       SPR_DBCR1, r0
    mtspr       SPR_DBCR2, r0
    mtspr       SPR_DBDR, r0
    mtspr       SPR_MCSR_C, r1
    mtspr       SPR_DEAR, r0
    mtspr       SPR_TCR, r0
    mtspr       SPR_DEC, r0
    mtspr       SPR_DECAR, r0
    mtspr       SPR_TSR_RC, r1
    mtspr       SPR_SRR0, r0
    mtspr       SPR_SRR1, r0
    mtspr       SPR_TBL_W, r0
    mtspr       SPR_TBU_W, r0
    mtspr       SPR_XER, r0
    mtspr       SPR_SPRG0, r0
    mtspr       SPR_SPRG1, r0
    mtspr       SPR_SPRG2, r0
    mtspr       SPR_SPRG3, r0
    mtspr       SPR_SPRG4, r0
    mtspr       SPR_SPRG5, r0
    mtspr       SPR_SPRG6, r0
    mtspr       SPR_SPRG7, r0
    mtspr       SPR_SPRG8, r0
    mtspr       SPR_PID, r0

    b           setup_tlb

tlb_entries:
// TLB entries to remap 64KB pages BOOTROM from 0xffffffff_fffc0000-0xffffffff_ffffffff to 0xfffc0000-0xffffffff
    .long   MMU_TLB_ENTRY(  0x3ff,  0xfffc0,    0xfffc0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x3ff,  0xfffd0,    0xfffd0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x3ff,  0xfffe0,    0xfffe0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x3ff,  0xffff0,    0xffff0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,   0b1 )
// Invalidate initial TLB entry
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xfffff,    MMU_TLBE_DSIZ_4KB,      0b0,    0b0,    0b0,    0b0,    0b0,    00,     MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
.set TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE

setup_tlb:
    load_addr   r3, tlb_entries
    load_const  r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries

    b test_mpw_cpu_008

.section ".text","ax",@progbits

test_mpw_cpu_008:
    //check all default values
    check_value r0,r1,r2, PLB6_BC_CR0_ADDR, PLB6_BC_CR0_VALUE, "PLB6_BC_CR0"
    check_value r0,r1,r2, PLB6_BC_PRI_ADDR, PLB6_BC_PRI_VALUE, "PLB6_BC_PRI"
    check_value r0,r1,r2, PLB6_BC_TSNOOP_ADDR, PLB6_BC_TSNOOP_VALUE, "PLB6_BC_TSNOOP"
    check_value r0,r1,r2, PLB6_BC_PAAM_WIN_EXT_ADDR,PLB6_BC_PAAM_WIN_EXT_VALUE, "PLB6_BC_PAAM_WIN_EXT"
    //non-used segments mest be written with value 0xFFFFFFF0.
    check_value r0,r1,r2, PLB6_BC_SGD1_ADDR ,PLB6_BC_SGD1_VALUE, "PLB6_BC_SGD1"
    check_value r0,r1,r2, PLB6_BC_SGD2_ADDR ,PLB6_BC_SGD2_VALUE, "PLB6_BC_SGD2"
    sgd_wr_check r0,r1,r2, PLB6_BC_SGD3_ADDR , "PLB6_BC_SGD3"
    sgd_wr_check r0,r1,r2, PLB6_BC_SGD4_ADDR , "PLB6_BC_SGD4"
    sgd_wr_check r0,r1,r2, PLB6_BC_SGD5_ADDR , "PLB6_BC_SGD5"
    sgd_wr_check r0,r1,r2, PLB6_BC_SGD6_ADDR , "PLB6_BC_SGD6"
    sgd_wr_check r0,r1,r2, PLB6_BC_SGD7_ADDR , "PLB6_BC_SGD7"
    check_value r0,r1,r2, PLB6_BC_ERR_ADDR ,PLB6_BC_ERR_VALUE, "PLB6_BC_ERR"
    check_value r0,r1,r2, PLB6_BC_MSRSP_ADDR ,PLB6_BC_MSRSP_VALUE, "PLB6_BC_MSRSP"
    check_value r0,r1,r2, PLB6_BC_HCPP_ADDR ,PLB6_BC_HCPP_VALUE, "PLB6_BC_HCPP"
    check_value r0,r1,r2, PLB6_BC_HD_ADDR ,PLB6_BC_HD_VALUE, "PLB6_BC_HD"
    check_value r0,r1,r2, PLB6_BC_QD_ADDR ,PLB6_BC_QD_VALUE, "PLB6_BC_QD"
    check_value r0,r1,r2, PLB6_BC_SHD_ADDR ,PLB6_BC_SHD_VALUE, "PLB6_BC_SHD"
    check_value r0,r1,r2, PLB6_BC_REVID_ADDR ,PLB6_BC_REVID_VALUE, "PLB6_BC_REVID"
    //ADDR bus now is checked

    //check DATA I/O buses
    rumboot_putstring "Sliding '1' and '0' ..."
running_1_low:
    load_const r0, PLB6_BC_SGD1_ADDR   //pointer
    load_const r1, 0x10                  //begin val
    load_const r3, 1                    //shift
    load_const r4, 0x80000000           //max val for compare
loop_1_low:
        mtdcrx r0, r1
        mfdcrx r2, r0
        cmp cr7, 0, r2,r1
        TEST_ASSERT(eq, cr7, "PLB6 DCRDATA: Running one error")
        cmp 0, 0, r1, r4
        //beq running_1_hi
        beq running_0_low
        slw r1, r1, r3
        b loop_1_low

running_0_low:
    load_const r1, 0xFFFFFFF0
    load_const r0, PLB6_BC_SGD1_ADDR
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running null error")

    load_const r1, 0xFFFFFFE0       //begin val
    load_const r3, 1                //shift
    load_const r4, 0x7FFFFFF0       //finish val
loop_0_low:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running null error")
    cmp 0, 0, r1, r4
    //beq running_0_hi
    beq test_ok
    slw r1, r1, r3
    ori r1, r1, 0x10
    b loop_0_low

test_ok:
    test_event EVENT_FINISHED
stop:
    b stop

unexpected_error:
    test_event  EVENT_ERROR

