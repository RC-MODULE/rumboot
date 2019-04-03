#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/trace.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>


#define WORD_ADDR_SHIFT             IBM_BIT_INDEX(32, 29)
#define INSTR_IDX_SHIFT             IBM_BIT_INDEX(32, 26)
#define CACHE_WAY_SHIFT             IBM_BIT_INDEX(32, 18)
#define ICDBTRH_EXT_TAG_ADDR_SHIFT  IBM_BIT_INDEX(32, 18)

#define ICU_DATA_RDBUS_CHECK_DATA_LEN    15

#define DATA_ARR_TEST_VAL0(way, idx, word_addr)     ((way << 16) | ((word_addr & 0x3) << 8) + idx)                    //(way << 16) | (word_add[1:0]<<8) + idx
#define DATA_ARR_TEST_VAL1(way, idx, word_addr)     (((way << 1) + ((word_addr & 0x4) >> 2)) << IBM_BIT_INDEX(32, 7)) //(2*way + word_addr[2] << IBM_BIT_INDEX(32, 7))

.macro check_icu_dataarr_addr_bus way, idx, word_addr
    load_const r3, ((\way << CACHE_WAY_SHIFT) | (\idx << INSTR_IDX_SHIFT) | (\word_addr << WORD_ADDR_SHIFT))
  rumboot_putstring "\ncheck"
  rumboot_puthex r3
  load_const    r4, \way
  rumboot_puthex r4
    //set expected value according test_mpw_cpu_mem_002.svh
    icread r0, r3
    isync

    load_const r8, DATA_ARR_TEST_VAL0(\way, \idx, \word_addr)
    mfspr r7, SPR_ICDBDR0
  //rumboot_putstring "0\n"
  //load_const    r4, \way
  rumboot_puthex r7
  rumboot_puthex r8
//    rumboot_putstring "actual   ICDBDR0: "
//    rumboot_puthex r7
//    rumboot_putstring "expected ICDBDR0: "
//    rumboot_puthex r8
    cmp cr7, 0, r7, r8
    TEST_ASSERT(eq, cr7, "ICDBDR0 ICU dataarr abus err. I=\idx  W=\way   WA=\word_addr")

    load_const r8, DATA_ARR_TEST_VAL1(\way, \idx, \word_addr)
    mfspr r7, SPR_ICDBDR1
  //rumboot_putstring "1\n"
  //load_const    r4, \way
  rumboot_puthex r7
  rumboot_puthex r8
//    rumboot_putstring "actual   ICDBDR1: "
//    rumboot_puthex r7
//    rumboot_putstring "expected ICDBDR1: "
//    rumboot_puthex r8
    cmp cr7, 0, r7, r8
    TEST_ASSERT(eq, cr7, "ICDBDR1 ICU dataarr abus err. I=\idx  W=\way   WA=\word_addr")
.endm

.macro check_icu_dataarr_addr_bus_bank way, word_addr, msg
    rumboot_putstring "\msg\n"
//                                  way        idx         word_addr
    check_icu_dataarr_addr_bus      \way,      0x00,       \word_addr
    check_icu_dataarr_addr_bus      \way,      0xFF,       \word_addr
    check_icu_dataarr_addr_bus      \way,      0xF0,       \word_addr
    check_icu_dataarr_addr_bus      \way,      0x0F,       \word_addr
    check_icu_dataarr_addr_bus      \way,      0xCC,       \word_addr
    check_icu_dataarr_addr_bus      \way,      0x33,       \word_addr
    check_icu_dataarr_addr_bus      \way,      0xAA,       \word_addr
    check_icu_dataarr_addr_bus      \way,      0x55,       \word_addr
.endm

.macro check_icu_data_cell rS0, rS1, way, idx, word_addr, msg
    //read data [61 : 31] or [30 : 0]
    lwz r6, 0(\rS0) //load expected values
    lwz r7, 0(\rS1) //load expected values
    //isync
    load_const r3, ((\way << CACHE_WAY_SHIFT) | (\word_addr << WORD_ADDR_SHIFT))
    or r3, r3, \idx
    icread r0, r3
    isync
    mfspr r4, SPR_ICDBDR0
//    rumboot_putstring "actual   ICDBDR0: "
//    rumboot_puthex r4
//    rumboot_putstring "expected ICDBDR0: "
//    rumboot_puthex r6
    cmp cr7, 0, r4, r6
    TEST_ASSERT(eq, cr7, "\msg (ICDBDR0)")

    //lwz r6, 0(\rS1) //load expected values
    //isync
    mfspr r4, SPR_ICDBDR1
//    rumboot_putstring "actual   ICDBDR1: "
//    rumboot_puthex r4
//    rumboot_putstring "expected ICDBDR1: "
//    rumboot_puthex r7
    cmp cr7, 0, r4, r7
    TEST_ASSERT(eq, cr7, "\msg (ICDBDR1)")
.endm

.macro check_icu_dataarr_rdata_bus_quater  way, word_addr, err_msg, loop_id
    load_const r8, 0   //loop conter
    load_const r9, ICU_DATA_RDBUS_CHECK_DATA_LEN //loop finish index
    loop_rdbus_check_\loop_id\way:
        slwi r10, r8, INSTR_IDX_SHIFT
        check_icu_data_cell r1, r2, \way, r10, \word_addr, "\err_msg"

        cmp cr0, 0, r8, r9 //compare loop_index and loop_finish_index
        beq rdbus_check_finish_\loop_id\way
        addi r8, r8, 1 //increment loop index
        addi r1, r1, 4 //increment ICDBR0 pointer
        addi r2, r2, 4 //increment ICDBR1 pointer
    b loop_rdbus_check_\loop_id\way
    rdbus_check_finish_\loop_id\way:
.endm

.macro check_icu_dataarr_rdata_bus way, word_addr, msg
    rumboot_putstring "\msg\n"

    rumboot_putstring "check [167:126]\n"
    lis r1, icu_data_array_ICDBR0_w0@h     //load hi part of addr
    ori r1, r1, icu_data_array_ICDBR0_w0@l //load lo part of addr
    lis r2, icu_data_array_ICDBR1_w0@h     //load hi part of addr
    ori r2, r2, icu_data_array_ICDBR1_w0@l //load lo part of addr
    check_icu_dataarr_rdata_bus_quater  \way, \word_addr, "icu rdata bus error [167:126]", "data_167_126_\word_addr"

    rumboot_putstring "check [125:84]\n"
    lis r1, icu_data_array_ICDBR0_w1@h     //load hi part of addr
    ori r1, r1, icu_data_array_ICDBR0_w1@l //load lo part of addr
    lis r2, icu_data_array_ICDBR1_w1@h     //load hi part of addr
    ori r2, r2, icu_data_array_ICDBR1_w1@l //load lo part of addr
    check_icu_dataarr_rdata_bus_quater  \way, (\word_addr+1), "icu rdata bus error [125:84]", "data_125_84_\word_addr"

    rumboot_putstring "check [83:42]\n"
    lis r1, icu_data_array_ICDBR0_w2@h     //load hi part of addr
    ori r1, r1, icu_data_array_ICDBR0_w2@l //load lo part of addr
    lis r2, icu_data_array_ICDBR1_w2@h     //load hi part of addr
    ori r2, r2, icu_data_array_ICDBR1_w2@l //load lo part of addr
    check_icu_dataarr_rdata_bus_quater  \way, (\word_addr+2), "icu rdata bus error [83:42]", "data_83_42_\word_addr"

    rumboot_putstring "check [41:0]\n"
    lis r1, icu_data_array_ICDBR0_w3@h     //load hi part of addr
    ori r1, r1, icu_data_array_ICDBR0_w3@l //load lo part of addr
    lis r2, icu_data_array_ICDBR1_w3@h     //load hi part of addr
    ori r2, r2, icu_data_array_ICDBR1_w3@l //load lo part of addr
    check_icu_dataarr_rdata_bus_quater  \way, (\word_addr+3), "icu rdata bus error [41:0]", "data_41_0\word_addr"
.endm



.section ".text","ax",@progbits

.global main

main:
test_oi10_cpu_mem_004:

    test_event_send_test_id "test_oi10_cpu_mem_004"

check_icu_data_arrays_addr_bus:
    test_event EVENT_INIT_ICU_DATA_ARRAY_ABUS_CHECK
    load_const r0, 0
//                                      WAY     WORD_ADDR
    check_icu_dataarr_addr_bus_bank     0,      0,          "Check dataArray0a.A"
    check_icu_dataarr_addr_bus_bank     0,      4,          "Check dataArray1a.A"
    check_icu_dataarr_addr_bus_bank     1,      1,          "Check dataArray2a.A"
    check_icu_dataarr_addr_bus_bank     1,      5,          "Check dataArray3a.A"
    check_icu_dataarr_addr_bus_bank     2,      2,          "Check dataArray4a.A"
    check_icu_dataarr_addr_bus_bank     2,      6,          "Check dataArray5a.A"
    check_icu_dataarr_addr_bus_bank     3,      3,          "Check dataArray6a.A"
    check_icu_dataarr_addr_bus_bank     3,      7,          "Check dataArray7a.A"

check_icu_data_arrays_rdata_bus:
    test_event EVENT_INIT_ICU_DATA_ARRAY_RDBUS_CHECK
//                                WAY     WORD_ADDR
    check_icu_dataarr_rdata_bus    0,      0,          "Check dataArray0a.DOUT"
    check_icu_dataarr_rdata_bus    0,      4,          "Check dataArray1a.DOUT"
    check_icu_dataarr_rdata_bus    1,      0,          "Check dataArray2a.DOUT"
    check_icu_dataarr_rdata_bus    1,      4,          "Check dataArray3a.DOUT"
    check_icu_dataarr_rdata_bus    2,      0,          "Check dataArray4a.DOUT"
    check_icu_dataarr_rdata_bus    2,      4,          "Check dataArray5a.DOUT"
    check_icu_dataarr_rdata_bus    3,      0,          "Check dataArray6a.DOUT"
    check_icu_dataarr_rdata_bus    3,      4,          "Check dataArray7a.DOUT"

test_ok:
    rumboot_putstring "TEST_OK\n"
    exit 0x00

test_error:
    rumboot_putstring "TEST_ERROR\n"
    exit 0x01



.global icu_data_array_ICDBR0_w0
icu_data_array_ICDBR0_w0:
.long 0x00000000
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0x00000000
.long 0xFFFFFFFF
.long 0x00000000
.long 0xFFFFF800
.long 0x000007FF
.long 0xFFE007FF
.long 0x001FF800
.long 0xFC1F07E0
.long 0x03E0F81F
.long 0xE398C71C
.long 0x1C6738E3
.long 0x55555555
.long 0xAAAAAAAA

.global icu_data_array_ICDBR1_w0
icu_data_array_ICDBR1_w0:
.long 0x00000000
.long 0xFFC00000
.long 0xFFC00000
.long 0x00000000
.long 0xFFC00000
.long 0x00000000
.long 0x00000000
.long 0xFFC00000
.long 0x00000000
.long 0xFFC00000
.long 0xF0800000
.long 0x0F400000
.long 0x8C800000
.long 0x73400000
.long 0xAA400000
.long 0x55800000

.global icu_data_array_ICDBR0_w1
icu_data_array_ICDBR0_w1:
.long 0x00000000
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0x00000000
.long 0x00000000
.long 0xFFFFFFFF
.long 0xFFFFF800
.long 0x000007FF
.long 0xFFE007FF
.long 0x001FF800
.long 0xFC1F07E0
.long 0x03E0F81F
.long 0xE398C71C
.long 0x1C6738E3
.long 0x55555555
.long 0xAAAAAAAA

.global icu_data_array_ICDBR1_w1
icu_data_array_ICDBR1_w1:
.long 0x00000000
.long 0xFFC00000
.long 0xFFC00000
.long 0x00000000
.long 0x00000000
.long 0xFFC00000
.long 0x00000000
.long 0xFFC00000
.long 0x00000000
.long 0xFFC00000
.long 0xF0800000
.long 0x0F400000
.long 0x8C800000
.long 0x73400000
.long 0xAA400000
.long 0x55800000

.global icu_data_array_ICDBR0_w2
icu_data_array_ICDBR0_w2:
.long 0x00000000
.long 0xFFFFFFFF
.long 0x00000000
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0x00000000
.long 0xFFFFF800
.long 0x000007FF
.long 0xFFE007FF
.long 0x001FF800
.long 0xFC1F07E0
.long 0x03E0F81F
.long 0xE398C71C
.long 0x1C6738E3
.long 0x55555555
.long 0xAAAAAAAA

.global icu_data_array_ICDBR1_w2
icu_data_array_ICDBR1_w2:
.long 0x00000000
.long 0xFFC00000
.long 0x00000000
.long 0xFFC00000
.long 0xFFC00000
.long 0x00000000
.long 0x00000000
.long 0xFFC00000
.long 0x00000000
.long 0xFFC00000
.long 0xF0800000
.long 0x0F400000
.long 0x8C800000
.long 0x73400000
.long 0xAA400000
.long 0x55800000

.global icu_data_array_ICDBR0_w3
icu_data_array_ICDBR0_w3:
.long 0x00000000
.long 0xFFFFFFFF
.long 0x00000000
.long 0xFFFFFFFF
.long 0x00000000
.long 0xFFFFFFFF
.long 0xFFFFF800
.long 0x000007FF
.long 0xFFE007FF
.long 0x001FF800
.long 0xFC1F07E0
.long 0x03E0F81F
.long 0xE398C71C
.long 0x1C6738E3
.long 0x55555555
.long 0xAAAAAAAA

.global icu_data_array_ICDBR1_w3
icu_data_array_ICDBR1_w3:
.long 0x00000000
.long 0xFFC00000
.long 0x01000000
.long 0xFFC00000
.long 0x00000000
.long 0xFFC00000
.long 0x00000000
.long 0xFFC00000
.long 0x00000000
.long 0xFFC00000
.long 0xF0800000
.long 0x0F400000
.long 0x8C800000
.long 0x73400000
.long 0xAA400000
.long 0x55800000
