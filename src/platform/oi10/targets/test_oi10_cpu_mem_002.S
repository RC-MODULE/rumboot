#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/trace.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>


#define WORD_ADDR_SHIFT                            IBM_BIT_INDEX(32, 29)
#define INSTR_IDX_SHIFT                            IBM_BIT_INDEX(32, 26)
#define CACHE_WAY_SHIFT                            IBM_BIT_INDEX(32, 18)
#define ICDBTRH_EXT_TAG_ADDR_SHIFT                 IBM_BIT_INDEX(32, 18)

#define ICU_TAG_RDBUS_CHECK_DATA_LEN               125
#define TAG_ARR_TEST_VAL(way, idx, half_AB)        ((((way << 1) + half_AB) << 7) + idx)    //((2*way+half_AB)<<7)+idx

#define TAG_ARR_TEST_VAL0(way, idx, word_addr)     (((word_addr & 0x3)  << 8) + idx)    //(word_add[1:0]<<8) + idx
#define TAG_ARR_TEST_VAL1(way, idx, word_addr)     (((way << 1) + ((word_addr & 0x4) >> 2)) << IBM_BIT_INDEX(32, 7)) //(2*way + word_addr[2] << IBM_BIT_INDEX(32, 7))

.macro check_icu_tag_addr_bus way, idx, idx_base
    //set cell addr which need to read
    //Table 5-3. EA Format icread (Core user manual)
    load_const r3, ((\way << CACHE_WAY_SHIFT) | ((\idx + (\idx_base << 7)) << INSTR_IDX_SHIFT))

    //set expected value according test_mpw_cpu_mem_002_0.svh
    load_const r5, TAG_ARR_TEST_VAL(\way, \idx, \idx_base)
    icread r0, r3
    isync
    mfspr r4, SPR_ICDBTRH
    andi. r4, r4, 0x3FF
    cmp cr7, 0, r4, r5
    TEST_ASSERT(eq, cr7, "ICU tag addr bus error. idx=\idx  way=\way")
.endm

.macro check_icu_tag_addr_bus_bank way, half_AB, msg
    rumboot_putstring "\msg\n"
//                          WAY        INDEX       half_AB
    check_icu_tag_addr_bus  \way,      0x00,       \half_AB
    check_icu_tag_addr_bus  \way,      0x01,       \half_AB
    check_icu_tag_addr_bus  \way,      0x02,       \half_AB
    check_icu_tag_addr_bus  \way,      0x04,       \half_AB
    check_icu_tag_addr_bus  \way,      0x08,       \half_AB
    check_icu_tag_addr_bus  \way,      0x10,       \half_AB
    check_icu_tag_addr_bus  \way,      0x20,       \half_AB
    check_icu_tag_addr_bus  \way,      0x40,       \half_AB
    check_icu_tag_addr_bus  \way,      0x7F,       \half_AB
    check_icu_tag_addr_bus  \way,      0x7E,       \half_AB
    check_icu_tag_addr_bus  \way,      0x7D,       \half_AB
    check_icu_tag_addr_bus  \way,      0x77,       \half_AB
    check_icu_tag_addr_bus  \way,      0x6F,       \half_AB
    check_icu_tag_addr_bus  \way,      0x5F,       \half_AB
    check_icu_tag_addr_bus  \way,      0x3F,       \half_AB
.endm

.macro check_icu_tag_cell rS, way, idx, msg
    //read data [61 : 31] or [30 : 0]
    lwz r6, 0(\rS) //load expected values
    isync
    load_const r3, (\way << CACHE_WAY_SHIFT)
    or r3, r3, \idx
    icread r0, r3
    isync
    mfspr r4, SPR_ICDBTRH
    cmp cr7, 0, r4, r6
    TEST_ASSERT(eq, cr7, "\msg")
 .endm

 .macro check_icu_tag_rdata_bus way, idx_base, msg
 /*
     r0 - 0
     r1 - icu_tag_array_data_a pointer
     r2 - icu_tag_array_data_b pointer
     r3 - icread argument
     r4 - use for copy from SPR_ICDBTRH
     r6 - value from icu_tag_array_data_a
     r7 - value from icu_tag_array_data_b
     r8 - loop counter
     r9 - loop finish index
     r10 - index of icu_tag_array (use for icread)

 */
     rumboot_putstring "\msg\n"
     //load expected data addr for [61:31] bus
     lis r1, icu_tag_array_data_a@h     //load hi part of addr
     ori r1, r1, icu_tag_array_data_a@l //load lo part of addr

     //load expected data addr for [30:0]
     lis r2, icu_tag_array_data_b@h     //load hi part of addr
     ori r2, r2, icu_tag_array_data_b@l //load lo part of addr

     load_const r8, 0   //loop conter
     load_const r9, ICU_TAG_RDBUS_CHECK_DATA_LEN //loop finish index
     loop_dbus_check_\way\idx_base:
         slwi r10, r8, INSTR_IDX_SHIFT
         addi r10, r10, ((\idx_base << 7) << INSTR_IDX_SHIFT)
         check_icu_tag_cell r1,\way,r10,"data bus [61:31] error." //read data [61:31] from cell and compare with expected
         check_icu_tag_cell r2,(\way+1),r10,"data bus [30:0] error."//read data [30:0] from cell and compare with expected

         cmp cr0, 0, r8, r9 //compare loop_index and loop_finish_index
         beq dbus_check_finish_\way\idx_base
         addi r8, r8, 1 //increment loop index
         addi r1, r1, 4 //increment data [61 : 31] pointer
         addi r2, r2, 4 //increment data [61 : 31] pointer
     b loop_dbus_check_\way\idx_base
     dbus_check_finish_\way\idx_base:
.endm



.section ".text","ax",@progbits

.global main

main:
test_oi10_cpu_mem_002:

    test_event_send_test_id "test_oi10_cpu_mem_002"

check_icu_tag_array_addr_bus:
    test_event EVENT_INIT_ICU_TAG_ARRAY_ABUS_CHECK
    load_const r0, 0
//                                  WAY    half_AB
    check_icu_tag_addr_bus_bank     0,     0,       "Check tagArray0.A[6:0]"
    check_icu_tag_addr_bus_bank     1,     1,       "Check tagArray1.A[6:0]"
    check_icu_tag_addr_bus_bank     2,     0,       "Check tagArray2.A[6:0]"
    check_icu_tag_addr_bus_bank     3,     1,       "Check tagArray3.A[6:0]"

check_icu_tag_arrays_rdata_bus:
    .global icu_tag_array_data_a
    .global icu_tag_array_data_b
    test_event EVENT_INIT_ICU_TAG_ARRAY_DBUS_CHECK
    load_const r0, 0

    check_icu_tag_rdata_bus 0, 0, "Check tagArray0.DOUT"
    check_icu_tag_rdata_bus 0, 1, "Check tagArray1.DOUT"
    check_icu_tag_rdata_bus 2, 0, "Check tagArray2.DOUT"
    check_icu_tag_rdata_bus 2, 1, "Check tagArray3.DOUT"

test_ok:
    rumboot_putstring "TEST_OK\n"
    exit 0x00

test_error:
    rumboot_putstring "TEST_ERROR\n"
    exit 0x01



.global icu_tag_array_data_a
icu_tag_array_data_a:
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000400
.long 0x00000800
.long 0x00002000
.long 0x00004000
.long 0x00008000
.long 0x00010000
.long 0x00020000
.long 0x00040000
.long 0x00080000
.long 0x00100000
.long 0x00200000
.long 0x00400000
.long 0x00800000
.long 0x01000000
.long 0x02000000
.long 0x04000000
.long 0x08000000
.long 0x10000000
.long 0x20000000
.long 0x40000000
.long 0x80000000
.long 0x00000001
.long 0x00000002
.long 0x00000004
.long 0x00000008
.long 0x00000010
.long 0x00000020
.long 0x00000040
.long 0x00000080
.long 0x00000100
.long 0x00000200
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEBFF
.long 0xFFFFE7FF
.long 0xFFFFCFFF
.long 0xFFFFAFFF
.long 0xFFFF6FFF
.long 0xFFFEEFFF
.long 0xFFFDEFFF
.long 0xFFFBEFFF
.long 0xFFF7EFFF
.long 0xFFEFEFFF
.long 0xFFDFEFFF
.long 0xFFBFEFFF
.long 0xFF7FEFFF
.long 0xFEFFEFFF
.long 0xFDFFEFFF
.long 0xFBFFEFFF
.long 0xF7FFEFFF
.long 0xEFFFEFFF
.long 0xDFFFEFFF
.long 0xBFFFEFFF
.long 0x7FFFEFFF
.long 0xFFFFEFFE
.long 0xFFFFEFFD
.long 0xFFFFEFFB
.long 0xFFFFEFF7
.long 0xFFFFEFEF
.long 0xFFFFEFDF
.long 0xFFFFEFBF
.long 0xFFFFEF7F
.long 0xFFFFEEFF
.long 0xFFFFEDFF

.global icu_tag_array_data_b
icu_tag_array_data_b:
.long 0x00000000
.long 0x00000400
.long 0x00000800
.long 0x00002000
.long 0x00004000
.long 0x00008000
.long 0x00010000
.long 0x00020000
.long 0x00040000
.long 0x00080000
.long 0x00100000
.long 0x00200000
.long 0x00400000
.long 0x00800000
.long 0x01000000
.long 0x02000000
.long 0x04000000
.long 0x08000000
.long 0x10000000
.long 0x20000000
.long 0x40000000
.long 0x80000000
.long 0x00000001
.long 0x00000002
.long 0x00000004
.long 0x00000008
.long 0x00000010
.long 0x00000020
.long 0x00000040
.long 0x00000080
.long 0x00000100
.long 0x00000200
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0xFFFFEFFF
.long 0xFFFFEBFF
.long 0xFFFFE7FF
.long 0xFFFFCFFF
.long 0xFFFFAFFF
.long 0xFFFF6FFF
.long 0xFFFEEFFF
.long 0xFFFDEFFF
.long 0xFFFBEFFF
.long 0xFFF7EFFF
.long 0xFFEFEFFF
.long 0xFFDFEFFF
.long 0xFFBFEFFF
.long 0xFF7FEFFF
.long 0xFEFFEFFF
.long 0xFDFFEFFF
.long 0xFBFFEFFF
.long 0xF7FFEFFF
.long 0xEFFFEFFF
.long 0xDFFFEFFF
.long 0xBFFFEFFF
.long 0x7FFFEFFF
.long 0xFFFFEFFE
.long 0xFFFFEFFD
.long 0xFFFFEFFB
.long 0xFFFFEFF7
.long 0xFFFFEFEF
.long 0xFFFFEFDF
.long 0xFFFFEFBF
.long 0xFFFFEF7F
.long 0xFFFFEEFF
.long 0xFFFFEDFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
.long 0xFFFFEFFF
