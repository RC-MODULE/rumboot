#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/trace.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>


.macro ICREAD_WRAPPER regA, regB, regC, regD, regE, regF
    icread \regA,\regB
    isync
    mfspr \regC, SPR_ICDBDR0
    mfspr \regD, SPR_ICDBDR1
    mfspr \regE, SPR_ICDBTRH
    mfspr \regF, SPR_ICDBTRL
.endm

.macro DCREAD_WRAPPER regT, regA, regB, regD, regE
    msync
    dcread \regT, \regA, \regB
    isync
    mfspr \regD, SPR_DCDBTRH
    mfspr \regE, SPR_DCDBTRL
.endm

.macro MOD regRES, regDIVIDENT, regDIVISOR
    divwu \regRES, \regDIVIDENT, \regDIVISOR
    mullw \regRES, \regRES, \regDIVISOR
    subf \regRES, \regRES, \regDIVIDENT
.endm

.macro TEST_ICU_LRU_ABUS way, index
    load_const r0, 0x00
    load_const r1, \way
    load_const r2, \index
    load_const r17, 31
    slwi r5, r1, IBM_BIT_INDEX(32, 18)    //way
    slwi r6, r2, IBM_BIT_INDEX(32, 26)    //index
    or r5, r5 ,r6
    ICREAD_WRAPPER r0, r5, r25, r26, r27, r28

    andis. r28, r28, 0x0FC0
    srwi r28, r28, 22

    cmpi cr7, 0, r2, 0x80
    blt cr7, 1f
    subi r2, r2, 0x80
    MOD r18, r2, r17
    ori r18, r18, 0x20
    b 2f
1:
    MOD r18, r2, r17
2:
    cmp cr7, 0, r28, r18
    TEST_ASSERT(eq, cr7, "Wrong value in icu lru array for way \way and index \index")
.endm

.macro TEST_DCU_LRU_ABUS way, index
    load_const r0, 0x00
    load_const r1, \way
    load_const r2, \index
    load_const r17, 31
    load_const r19, 2
    slwi r5, r1, IBM_BIT_INDEX(32, 18) //way
    slwi r6, r2, IBM_BIT_INDEX(32, 26) //index
    or r5, r5 ,r6
    DCREAD_WRAPPER r25, r0, r5, r26, r27

    andis. r27, r27, 0x0FC0
    srwi r27, r27, 22

    MOD r20, r2, r19

    cmpi cr7, 0, r2, 0x80
    blt cr7, 1f
    subi r2, r2, 0x80
    MOD r18, r2, r17
    bgt cr7, 2f
    ori r18, r18, 0x20
    b 3f
1:
    MOD r18, r2, r17
    cmpi cr7, 0, r20, 0x00
    beq cr7, 3f
    ori r18, r18, 0x20
    b 3f
2:
    cmpi cr7, 0, r20, 0x01
    beq cr7, 3f
    ori r18, r18, 0x20
3:
    cmp cr7, 0 , r27, r18
    TEST_ASSERT(eq, cr7, "Wrong value in dcu lru array for way \way and index \index")
.endm

.macro TEST_ICU_LRU_DBUS way, addr
    load_const r0, 0x00
    load_const r1, \way
    load_const r2, 0x00 //current index
    lis r17, \addr@h
    ori r17, r17, \addr@l
6:
    slwi r5, r1, IBM_BIT_INDEX(32, 18) //way
    slwi r6, r2, IBM_BIT_INDEX(32, 26) //index
    or r5, r5 ,r6
    ICREAD_WRAPPER r0, r5, r25, r26, r27, r28

    lwz r18, 0(r17)
    cmp cr7, 0, r28, r18
    TEST_ASSERT(eq, cr7, "Wrong value in icu lru array for way \way")

    addi r17, r17, 4
    addi r2, r2, 1
    cmpi cr7, 0, r2, 10
    blt cr7, 6b
.endm

.macro TEST_DCU_LRU_DBUS way, addr
    load_const r0, 0x00
    load_const r1, \way
    load_const r2, 0x00 //current index
    load_addr  r17, \addr
6:
    slwi r5, r1, IBM_BIT_INDEX(32, 18) //way
    slwi r6, r2, IBM_BIT_INDEX(32, 26) //index
    or r5, r5 ,r6
    DCREAD_WRAPPER r25, r0, r5, r26, r27

    //Workaround to discard "x" in data parity.
    load_const r28, 0xFFFFFFF0
    and r27, r27, r28


    lwz r18, 0(r17)
//    rumboot_puthex    r27
    cmp cr7, 0 , r27, r18
    TEST_ASSERT(eq, cr7, "Wrong value in dcu lru array for way \way")

    addi r17, r17, 4
    addi r2, r2, 1
    cmpi cr7, 0, r2, 10
    blt cr7, 6b
.endm

.macro TEST_DCU_LRU_DBUS_MASK addr, index
    load_const r0, 0x00
    load_const r1, 0x00
    load_const r2, \index //current index
    load_const r17, \addr //current index
    slwi r5, r1, IBM_BIT_INDEX(32, 18) //way
    slwi r6, r2, IBM_BIT_INDEX(32, 26) //index
    or r5, r5 ,r6
    DCREAD_WRAPPER r25, r0, r5, r26, r27

    //Workaround to discard "x" in data parity.
    load_const r28, 0xFFFFFFF0
    and r27, r27, r28


//    lwz r18, 0(r17)
//    rumboot_puthex    r27
    cmp cr7, 0 , r27, r17
    TEST_ASSERT(eq, cr7, "Wrong value in dcu lru array for way \way")

.endm

BEGIN_ENUM( test_event_code )
DECLARE_ENUM_VAL( TEC_INIT_LRU_ARRAYS_ABUS_CHECK,   TEST_EVENT_CODE_MIN+0 )
DECLARE_ENUM_VAL( TEC_INIT_LRU_ARRAYS_DBUS_CHECK,   TEST_EVENT_CODE_MIN+1 )
DECLARE_ENUM_VAL( TEC_DCU_WBYTESEL0_01_BIT_FORCE,   TEST_EVENT_CODE_MIN+2 )
DECLARE_ENUM_VAL( TEC_DCU_WBYTESEL0_10_BIT_FORCE,   TEST_EVENT_CODE_MIN+3 )
DECLARE_ENUM_VAL( TEC_DCU_WBYTESEL0_11_BIT_FORCE,   TEST_EVENT_CODE_MIN+4 )
END_ENUM( test_event_code )


.section ".text.rom","ax",@progbits

.global main
.align 8
.long 0
.align 8
.long 0
.align 8
.long 0
.align 8
.long 0
.align 8
.long 0
.align 8
.long 0
.align 8
main:
test_oi10_cpu_mem_007:

    test_event_send_test_id "test_oi10_cpu_mem_007"

    test_event TEC_INIT_LRU_ARRAYS_ABUS_CHECK

    rumboot_putstring "TEST_ICU_LRU_ABUS 0 1\n"
    TEST_ICU_LRU_ABUS 0, 0x00
    TEST_ICU_LRU_ABUS 0, 0x01
    TEST_ICU_LRU_ABUS 0, 0x02
    TEST_ICU_LRU_ABUS 0, 0x04
    TEST_ICU_LRU_ABUS 0, 0x08
    TEST_ICU_LRU_ABUS 0, 0x10
    TEST_ICU_LRU_ABUS 0, 0x20
    TEST_ICU_LRU_ABUS 0, 0x40
    TEST_ICU_LRU_ABUS 0, 0x80

    rumboot_putstring "TEST_ICU_LRU_ABUS 1 1\n"
    TEST_ICU_LRU_ABUS 1, 0x00
    TEST_ICU_LRU_ABUS 1, 0x01
    TEST_ICU_LRU_ABUS 1, 0x02
    TEST_ICU_LRU_ABUS 1, 0x04
    TEST_ICU_LRU_ABUS 1, 0x08
    TEST_ICU_LRU_ABUS 1, 0x10
    TEST_ICU_LRU_ABUS 1, 0x20
    TEST_ICU_LRU_ABUS 1, 0x40
    TEST_ICU_LRU_ABUS 1, 0x80

    rumboot_putstring "TEST_ICU_LRU_ABUS 2 1\n"
    TEST_ICU_LRU_ABUS 2, 0x00
    TEST_ICU_LRU_ABUS 2, 0x01
    TEST_ICU_LRU_ABUS 2, 0x02
    TEST_ICU_LRU_ABUS 2, 0x04
    TEST_ICU_LRU_ABUS 2, 0x08
    TEST_ICU_LRU_ABUS 2, 0x10
    TEST_ICU_LRU_ABUS 2, 0x20
    TEST_ICU_LRU_ABUS 2, 0x40
    TEST_ICU_LRU_ABUS 2, 0x80

    rumboot_putstring "TEST_ICU_LRU_ABUS 3 1\n"
    TEST_ICU_LRU_ABUS 3, 0x00
    TEST_ICU_LRU_ABUS 3, 0x01
    TEST_ICU_LRU_ABUS 3, 0x02
    TEST_ICU_LRU_ABUS 3, 0x04
    TEST_ICU_LRU_ABUS 3, 0x08
    TEST_ICU_LRU_ABUS 3, 0x10
    TEST_ICU_LRU_ABUS 3, 0x20
    TEST_ICU_LRU_ABUS 3, 0x40
    TEST_ICU_LRU_ABUS 3, 0x80

    rumboot_putstring "TEST_ICU_LRU_ABUS 0 0\n"
    TEST_ICU_LRU_ABUS 0, 0xFE
    TEST_ICU_LRU_ABUS 0, 0xFD
    TEST_ICU_LRU_ABUS 0, 0xFB
    TEST_ICU_LRU_ABUS 0, 0xF7
    TEST_ICU_LRU_ABUS 0, 0xEF
    TEST_ICU_LRU_ABUS 0, 0xDF
    TEST_ICU_LRU_ABUS 0, 0xBF
    TEST_ICU_LRU_ABUS 0, 0x7F

    rumboot_putstring "TEST_ICU_LRU_ABUS 1 0\n"
    TEST_ICU_LRU_ABUS 1, 0xFE
    TEST_ICU_LRU_ABUS 1, 0xFD
    TEST_ICU_LRU_ABUS 1, 0xFB
    TEST_ICU_LRU_ABUS 1, 0xF7
    TEST_ICU_LRU_ABUS 1, 0xEF
    TEST_ICU_LRU_ABUS 1, 0xDF
    TEST_ICU_LRU_ABUS 1, 0xBF
    TEST_ICU_LRU_ABUS 1, 0x7F

    rumboot_putstring "TEST_ICU_LRU_ABUS 2 0\n"
    TEST_ICU_LRU_ABUS 2, 0xFE
    TEST_ICU_LRU_ABUS 2, 0xFD
    TEST_ICU_LRU_ABUS 2, 0xFB
    TEST_ICU_LRU_ABUS 2, 0xF7
    TEST_ICU_LRU_ABUS 2, 0xEF
    TEST_ICU_LRU_ABUS 2, 0xDF
    TEST_ICU_LRU_ABUS 2, 0xBF
    TEST_ICU_LRU_ABUS 2, 0x7F

    rumboot_putstring "TEST_ICU_LRU_ABUS 3 0\n"
    TEST_ICU_LRU_ABUS 3, 0xFE
    TEST_ICU_LRU_ABUS 3, 0xFD
    TEST_ICU_LRU_ABUS 3, 0xFB
    TEST_ICU_LRU_ABUS 3, 0xF7
    TEST_ICU_LRU_ABUS 3, 0xEF
    TEST_ICU_LRU_ABUS 3, 0xDF
    TEST_ICU_LRU_ABUS 3, 0xBF
    TEST_ICU_LRU_ABUS 3, 0x7F


    rumboot_putstring "TEST_DCU_LRU_ABUS 0 1\n"
    TEST_DCU_LRU_ABUS 0, 0x00
    TEST_DCU_LRU_ABUS 0, 0x01
    TEST_DCU_LRU_ABUS 0, 0x02
    TEST_DCU_LRU_ABUS 0, 0x04
    TEST_DCU_LRU_ABUS 0, 0x08
    TEST_DCU_LRU_ABUS 0, 0x10
    TEST_DCU_LRU_ABUS 0, 0x20
    TEST_DCU_LRU_ABUS 0, 0x40
    TEST_DCU_LRU_ABUS 0, 0x80

    rumboot_putstring "TEST_DCU_LRU_ABUS 1 1\n"
    TEST_DCU_LRU_ABUS 1, 0x00
    TEST_DCU_LRU_ABUS 1, 0x01
    TEST_DCU_LRU_ABUS 1, 0x02
    TEST_DCU_LRU_ABUS 1, 0x04
    TEST_DCU_LRU_ABUS 1, 0x08
    TEST_DCU_LRU_ABUS 1, 0x10
    TEST_DCU_LRU_ABUS 1, 0x20
    TEST_DCU_LRU_ABUS 1, 0x40
    TEST_DCU_LRU_ABUS 1, 0x80

    rumboot_putstring "TEST_DCU_LRU_ABUS 2 1\n"
    TEST_DCU_LRU_ABUS 2, 0x00
    TEST_DCU_LRU_ABUS 2, 0x01
    TEST_DCU_LRU_ABUS 2, 0x02
    TEST_DCU_LRU_ABUS 2, 0x04
    TEST_DCU_LRU_ABUS 2, 0x08
    TEST_DCU_LRU_ABUS 2, 0x10
    TEST_DCU_LRU_ABUS 2, 0x20
    TEST_DCU_LRU_ABUS 2, 0x40
    TEST_DCU_LRU_ABUS 2, 0x80

    rumboot_putstring "TEST_DCU_LRU_ABUS 3 1\n"
    TEST_DCU_LRU_ABUS 3, 0x00
    TEST_DCU_LRU_ABUS 3, 0x01
    TEST_DCU_LRU_ABUS 3, 0x02
    TEST_DCU_LRU_ABUS 3, 0x04
    TEST_DCU_LRU_ABUS 3, 0x08
    TEST_DCU_LRU_ABUS 3, 0x10
    TEST_DCU_LRU_ABUS 3, 0x20
    TEST_DCU_LRU_ABUS 3, 0x40
    TEST_DCU_LRU_ABUS 3, 0x80

    rumboot_putstring "TEST_DCU_LRU_ABUS 0 0\n"
    TEST_DCU_LRU_ABUS 0, 0xFE
    TEST_DCU_LRU_ABUS 0, 0xFD
    TEST_DCU_LRU_ABUS 0, 0xFB
    TEST_DCU_LRU_ABUS 0, 0xF7
    TEST_DCU_LRU_ABUS 0, 0xEF
    TEST_DCU_LRU_ABUS 0, 0xDF
    TEST_DCU_LRU_ABUS 0, 0xBF
    TEST_DCU_LRU_ABUS 0, 0x7F

    rumboot_putstring "TEST_DCU_LRU_ABUS 1 0\n"
    TEST_DCU_LRU_ABUS 1, 0xFE
    TEST_DCU_LRU_ABUS 1, 0xFD
    TEST_DCU_LRU_ABUS 1, 0xFB
    TEST_DCU_LRU_ABUS 1, 0xF7
    TEST_DCU_LRU_ABUS 1, 0xEF
    TEST_DCU_LRU_ABUS 1, 0xDF
    TEST_DCU_LRU_ABUS 1, 0xBF
    TEST_DCU_LRU_ABUS 1, 0x7F

    rumboot_putstring "TEST_DCU_LRU_ABUS 2 0\n"
    TEST_DCU_LRU_ABUS 2, 0xFE
    TEST_DCU_LRU_ABUS 2, 0xFD
    TEST_DCU_LRU_ABUS 2, 0xFB
    TEST_DCU_LRU_ABUS 2, 0xF7
    TEST_DCU_LRU_ABUS 2, 0xEF
    TEST_DCU_LRU_ABUS 2, 0xDF
    TEST_DCU_LRU_ABUS 2, 0xBF
    TEST_DCU_LRU_ABUS 2, 0x7F

    rumboot_putstring "TEST_DCU_LRU_ABUS 3 0\n"
    TEST_DCU_LRU_ABUS 3, 0xFE
    TEST_DCU_LRU_ABUS 3, 0xFD
    TEST_DCU_LRU_ABUS 3, 0xFB
    TEST_DCU_LRU_ABUS 3, 0xF7
    TEST_DCU_LRU_ABUS 3, 0xEF
    TEST_DCU_LRU_ABUS 3, 0xDF
    TEST_DCU_LRU_ABUS 3, 0xBF
    TEST_DCU_LRU_ABUS 3, 0x7F


    test_event TEC_INIT_LRU_ARRAYS_DBUS_CHECK

    rumboot_putstring "TEST_ICU_LRU_DBUS icu_lru_array_data\n"
    TEST_ICU_LRU_DBUS 0, icu_lru_array_data
    TEST_ICU_LRU_DBUS 1, icu_lru_array_data
    TEST_ICU_LRU_DBUS 2, icu_lru_array_data
    TEST_ICU_LRU_DBUS 3, icu_lru_array_data

    rumboot_putstring "TEST_DCU_LRU_DBUS dcu_lru_array_data\n"

    .align 7
    TEST_DCU_LRU_DBUS 0, dcu_lru_array_data
    TEST_DCU_LRU_DBUS 1, dcu_lru_array_data
    TEST_DCU_LRU_DBUS 2, dcu_lru_array_data
    TEST_DCU_LRU_DBUS 3, dcu_lru_array_data

    rumboot_putstring "TEC_DCU_WBYTESEL0_01_BIT_FORCE dcu_lru_array_data\n"

    test_event TEC_DCU_WBYTESEL0_01_BIT_FORCE
    .align 7
    TEST_DCU_LRU_DBUS_MASK 0x80008000, 0x00
    TEST_DCU_LRU_DBUS_MASK 0x08448000, 0x3f
    TEST_DCU_LRU_DBUS_MASK 0x80800000, 0x40
    TEST_DCU_LRU_DBUS_MASK 0x88c44000, 0x7f
    TEST_DCU_LRU_DBUS_MASK 0x80008000, 0x80
    TEST_DCU_LRU_DBUS_MASK 0x00404000, 0xbf
    TEST_DCU_LRU_DBUS_MASK 0x8884c000, 0xc0
    TEST_DCU_LRU_DBUS_MASK 0x80c08000, 0xff

    rumboot_putstring "TEC_DCU_WBYTESEL0_10_BIT_FORCE dcu_lru_array_data\n"

    ici 0
    msync

    test_event TEC_DCU_WBYTESEL0_10_BIT_FORCE
    .align 7
    TEST_DCU_LRU_DBUS_MASK 0x89400000, 0x00
    TEST_DCU_LRU_DBUS_MASK 0x0fc48000, 0x3f
    TEST_DCU_LRU_DBUS_MASK 0x80008000, 0x40
    TEST_DCU_LRU_DBUS_MASK 0x8fc4c000, 0x7f
    TEST_DCU_LRU_DBUS_MASK 0x8fc08000, 0x80
    TEST_DCU_LRU_DBUS_MASK 0x0fc0c000, 0xbf
    TEST_DCU_LRU_DBUS_MASK 0x8004c000, 0xc0
    TEST_DCU_LRU_DBUS_MASK 0x8fc08000, 0xff

    rumboot_putstring "TEC_DCU_WBYTESEL0_11_BIT_FORCE dcu_lru_array_data\n"

    ici 0
    msync

    test_event TEC_DCU_WBYTESEL0_11_BIT_FORCE
    .align 7
    TEST_DCU_LRU_DBUS_MASK 0x02c04000, 0x00
    TEST_DCU_LRU_DBUS_MASK 0x8004c000, 0x3f
    TEST_DCU_LRU_DBUS_MASK 0x80400000, 0x40
    TEST_DCU_LRU_DBUS_MASK 0x0000c000, 0x7f
    TEST_DCU_LRU_DBUS_MASK 0x00440000, 0x80
    TEST_DCU_LRU_DBUS_MASK 0x80008000, 0xbf
    TEST_DCU_LRU_DBUS_MASK 0x80444000, 0xc0
    TEST_DCU_LRU_DBUS_MASK 0x00048000, 0xff

test_ok:
    rumboot_putstring "TEST_OK\n"
    exit 0x00

test_error:
    rumboot_putstring "TEST_ERROR\n"
    exit 0x01

icu_lru_array_data:
//Array with LRU parity bits
.long 0x00000000
.long 0xefc04000
.long 0x1f3c4000
.long 0x3ccc0000
.long 0xaaa8a008
.long 0x55548000
.long 0xc3302008
.long 0xe0c06008
.long 0x103c6008
.long 0xfffc2008


dcu_lru_array_data:
//Array with LRU parity bits
.long 0x0000c000
.long 0x0fc0c000
.long 0xff208000
.long 0x9ce4c000
.long 0x5a944000
.long 0xa5684000
.long 0x6318c000
.long 0x00dc8000
.long 0xf03cc000
.long 0xfffcc000

