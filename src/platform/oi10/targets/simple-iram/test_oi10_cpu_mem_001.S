#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/devices.h>
#include <platform/trace.S.h>


#define INITIAL_DATA_POINTER            0x80040000
#define INITIAL_DATA_POINTER_FMADD      0x40001800


.macro load_double_to_fpr fpr, addr_reg, data_reg, const_high, const_low
    //store operands
    load_const          \data_reg, \const_high
    stw                 \data_reg, 0(\addr_reg)
    load_const          \data_reg, \const_low
    stw                 \data_reg, 4(\addr_reg)
    //load double into fpr0
    lfd \fpr, 0(\addr_reg)
    isync
.endm

.macro store_double_from_fpr fpr, addr_reg
    stfd \fpr,0(\addr_reg)
    //isync
.endm

.macro load_double_to_fpr_back_to_0  fpr, addr_from_reg, addr_to_reg, data_reg, const_high, const_low
    load_double_to_fpr \fpr, \addr_from_reg, \data_reg, \const_high, \const_low
    store_double_from_fpr \fpr, \addr_to_reg
    fsel                \fpr, \fpr, \fpr, \fpr
    load_double_to_fpr \fpr, \addr_from_reg, \data_reg, 0x00000000, 0x00000000
    store_double_from_fpr \fpr, \addr_to_reg
.endm


.macro load_double_to_fpr_back_to_1  fpr, addr_from_reg, addr_to_reg, data_reg, const_high, const_low
    load_double_to_fpr \fpr, \addr_from_reg, \data_reg, \const_high, \const_low
    store_double_from_fpr \fpr, \addr_to_reg
    fsel                \fpr, \fpr, \fpr, \fpr
    load_double_to_fpr \fpr, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    store_double_from_fpr \fpr, \addr_to_reg
.endm

.macro clear_fprs_with_0 addr_reg, reg_low
    load_double_to_fpr f0, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f1, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f2, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f3, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f4, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f5, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f6, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f7, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f8, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f9, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f10, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f11, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f12, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f13, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f14, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f15, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f16, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f17, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f18, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f19, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f20, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f21, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f22, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f23, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f24, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f25, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f26, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f27, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f28, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f29, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f30, \addr_reg, \reg_low, 0x00000000, 0x00000000
    load_double_to_fpr f31, \addr_reg, \reg_low, 0x00000000, 0x00000000
.endm

.macro clear_fprs_with_1 addr_reg, data_reg
    load_double_to_fpr f0, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f1, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f2, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f3, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f4, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f5, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f6, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f7, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f8, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f9, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f10, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f11, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f12, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f13, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f14, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f15, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f16, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f17, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f18, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f19, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f20, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f21, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f22, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f23, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f24, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f25, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f26, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f27, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f28, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f29, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f30, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f31, \addr_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
.endm

.macro test addr_from_reg, addr_to_reg, data_reg
    load_double_to_fpr_back_to_0 f0 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000000
    fsel                f0, f0, f0, f0

    load_double_to_fpr_back_to_0 f0 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000001
    load_double_to_fpr_back_to_0 f1 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000002
    load_double_to_fpr_back_to_0 f2 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000004
    load_double_to_fpr_back_to_0 f3 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000008

    load_double_to_fpr_back_to_0 f4 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000010
    load_double_to_fpr_back_to_0 f5 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000020
    load_double_to_fpr_back_to_0 f6 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000040
    load_double_to_fpr_back_to_0 f7 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000080

    load_double_to_fpr_back_to_0 f8 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000100
    load_double_to_fpr_back_to_0 f9 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000200
    load_double_to_fpr_back_to_0 f10, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000400
    load_double_to_fpr_back_to_0 f11, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00000800

    load_double_to_fpr_back_to_0 f12, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00001000
    load_double_to_fpr_back_to_0 f13, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00002000
    load_double_to_fpr_back_to_0 f14, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00004000
    load_double_to_fpr_back_to_0 f15, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00008000

    load_double_to_fpr_back_to_0 f16, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00010000
    load_double_to_fpr_back_to_0 f17, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00020000
    load_double_to_fpr_back_to_0 f18, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00040000
    load_double_to_fpr_back_to_0 f19, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00080000

    load_double_to_fpr_back_to_0 f20, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00100000
    load_double_to_fpr_back_to_0 f21, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00200000
    load_double_to_fpr_back_to_0 f22, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00400000
    load_double_to_fpr_back_to_0 f23, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x00800000

    load_double_to_fpr_back_to_0 f24, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x01000000
    load_double_to_fpr_back_to_0 f25, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x02000000
    load_double_to_fpr_back_to_0 f26, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x04000000
    load_double_to_fpr_back_to_0 f27, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x08000000

    load_double_to_fpr_back_to_0 f28, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x10000000
    load_double_to_fpr_back_to_0 f29, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x20000000
    load_double_to_fpr_back_to_0 f30, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x40000000
    load_double_to_fpr_back_to_0 f31, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000000, 0x80000000

////
    load_double_to_fpr_back_to_0 f0 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000001, 0x00000000
    load_double_to_fpr_back_to_0 f1 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000002, 0x00000000
    load_double_to_fpr_back_to_0 f2 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000004, 0x00000000
    load_double_to_fpr_back_to_0 f3 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000008, 0x00000000

    load_double_to_fpr_back_to_0 f4 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000010, 0x00000000
    load_double_to_fpr_back_to_0 f5 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000020, 0x00000000
    load_double_to_fpr_back_to_0 f6 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000040, 0x00000000
    load_double_to_fpr_back_to_0 f7 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000080, 0x00000000

    load_double_to_fpr_back_to_0 f8 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000100, 0x00000000
    load_double_to_fpr_back_to_0 f9 , \addr_from_reg, \addr_to_reg, \data_reg, 0x00000200, 0x00000000
    load_double_to_fpr_back_to_0 f10, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000400, 0x00000000
    load_double_to_fpr_back_to_0 f11, \addr_from_reg, \addr_to_reg, \data_reg, 0x00000800, 0x00000000

    load_double_to_fpr_back_to_0 f12, \addr_from_reg, \addr_to_reg, \data_reg, 0x00001000, 0x00000000
    load_double_to_fpr_back_to_0 f13, \addr_from_reg, \addr_to_reg, \data_reg, 0x00002000, 0x00000000
    load_double_to_fpr_back_to_0 f14, \addr_from_reg, \addr_to_reg, \data_reg, 0x00004000, 0x00000000
    load_double_to_fpr_back_to_0 f15, \addr_from_reg, \addr_to_reg, \data_reg, 0x00008000, 0x00000000

    load_double_to_fpr_back_to_0 f16, \addr_from_reg, \addr_to_reg, \data_reg, 0x00010000, 0x00000000
    load_double_to_fpr_back_to_0 f17, \addr_from_reg, \addr_to_reg, \data_reg, 0x00020000, 0x00000000
    load_double_to_fpr_back_to_0 f18, \addr_from_reg, \addr_to_reg, \data_reg, 0x00040000, 0x00000000
    load_double_to_fpr_back_to_0 f19, \addr_from_reg, \addr_to_reg, \data_reg, 0x00080000, 0x00000000

    load_double_to_fpr_back_to_0 f20, \addr_from_reg, \addr_to_reg, \data_reg, 0x00100000, 0x00000000
    load_double_to_fpr_back_to_0 f21, \addr_from_reg, \addr_to_reg, \data_reg, 0x00200000, 0x00000000
    load_double_to_fpr_back_to_0 f22, \addr_from_reg, \addr_to_reg, \data_reg, 0x00400000, 0x00000000
    load_double_to_fpr_back_to_0 f23, \addr_from_reg, \addr_to_reg, \data_reg, 0x00800000, 0x00000000

    load_double_to_fpr_back_to_0 f24, \addr_from_reg, \addr_to_reg, \data_reg, 0x01000000, 0x00000000
    load_double_to_fpr_back_to_0 f25, \addr_from_reg, \addr_to_reg, \data_reg, 0x02000000, 0x00000000
    load_double_to_fpr_back_to_0 f26, \addr_from_reg, \addr_to_reg, \data_reg, 0x04000000, 0x00000000
    load_double_to_fpr_back_to_0 f27, \addr_from_reg, \addr_to_reg, \data_reg, 0x08000000, 0x00000000

    load_double_to_fpr_back_to_0 f28, \addr_from_reg, \addr_to_reg, \data_reg, 0x10000000, 0x00000000
    load_double_to_fpr_back_to_0 f29, \addr_from_reg, \addr_to_reg, \data_reg, 0x20000000, 0x00000000
    load_double_to_fpr_back_to_0 f30, \addr_from_reg, \addr_to_reg, \data_reg, 0x40000000, 0x00000000
    load_double_to_fpr_back_to_0 f31, \addr_from_reg, \addr_to_reg, \data_reg, 0x80000000, 0x00000000

    clear_fprs_with_1 \addr_from_reg, \data_reg
    fsel              f0, f0, f0, f0

//////////////////////////////////////////////
    load_double_to_fpr_back_to_1 f0 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFE
    load_double_to_fpr_back_to_1 f1 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFD
    load_double_to_fpr_back_to_1 f2 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFB
    load_double_to_fpr_back_to_1 f3 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFF7

    load_double_to_fpr_back_to_1 f4 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFEF
    load_double_to_fpr_back_to_1 f5 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFDF
    load_double_to_fpr_back_to_1 f6 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFBF
    load_double_to_fpr_back_to_1 f7 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFF7F

    load_double_to_fpr_back_to_1 f8 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFEFF
    load_double_to_fpr_back_to_1 f9 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFDFF
    load_double_to_fpr_back_to_1 f10, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFBFF
    load_double_to_fpr_back_to_1 f11, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFF7FF

    load_double_to_fpr_back_to_1 f12, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFEFFF
    load_double_to_fpr_back_to_1 f13, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFDFFF
    load_double_to_fpr_back_to_1 f14, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFFBFFF
    load_double_to_fpr_back_to_1 f15, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFF7FFF

    load_double_to_fpr_back_to_1 f16, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFEFFFF
    load_double_to_fpr_back_to_1 f17, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFDFFFF
    load_double_to_fpr_back_to_1 f18, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFFBFFFF
    load_double_to_fpr_back_to_1 f19, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFF7FFFF

    load_double_to_fpr_back_to_1 f20, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFEFFFFF
    load_double_to_fpr_back_to_1 f21, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFDFFFFF
    load_double_to_fpr_back_to_1 f22, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFFBFFFFF
    load_double_to_fpr_back_to_1 f23, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFF7FFFFF

    load_double_to_fpr_back_to_1 f24, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFEFFFFFF
    load_double_to_fpr_back_to_1 f25, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFDFFFFFF
    load_double_to_fpr_back_to_1 f26, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xFBFFFFFF
    load_double_to_fpr_back_to_1 f27, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xF7FFFFFF

    load_double_to_fpr_back_to_1 f28, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xEFFFFFFF
    load_double_to_fpr_back_to_1 f29, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xDFFFFFFF
    load_double_to_fpr_back_to_1 f30, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0xBFFFFFFF
    load_double_to_fpr_back_to_1 f31, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFF, 0x7FFFFFFF

////
    load_double_to_fpr_back_to_1 f0 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFE, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f1 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFD, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f2 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFFB, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f3 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFF7, 0xFFFFFFFF

    load_double_to_fpr_back_to_1 f4 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFEF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f5 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFDF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f6 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFFBF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f7 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFF7F, 0xFFFFFFFF

    load_double_to_fpr_back_to_1 f8 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFEFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f9 , \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFDFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f10, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFFBFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f11, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFF7FF, 0xFFFFFFFF

    load_double_to_fpr_back_to_1 f12, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFEFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f13, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFDFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f14, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFFBFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f15, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFF7FFF, 0xFFFFFFFF

    load_double_to_fpr_back_to_1 f16, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFEFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f17, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFDFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f18, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFFBFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f19, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFF7FFFF, 0xFFFFFFFF

    load_double_to_fpr_back_to_1 f20, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFEFFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f21, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFDFFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f22, \addr_from_reg, \addr_to_reg, \data_reg, 0xFFBFFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f23, \addr_from_reg, \addr_to_reg, \data_reg, 0xFF7FFFFF, 0xFFFFFFFF

    load_double_to_fpr_back_to_1 f24, \addr_from_reg, \addr_to_reg, \data_reg, 0xFEFFFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f25, \addr_from_reg, \addr_to_reg, \data_reg, 0xFDFFFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f26, \addr_from_reg, \addr_to_reg, \data_reg, 0xFBFFFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f27, \addr_from_reg, \addr_to_reg, \data_reg, 0xF7FFFFFF, 0xFFFFFFFF

    load_double_to_fpr_back_to_1 f28, \addr_from_reg, \addr_to_reg, \data_reg, 0xEFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f29, \addr_from_reg, \addr_to_reg, \data_reg, 0xDFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f30, \addr_from_reg, \addr_to_reg, \data_reg, 0xBFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr_back_to_1 f31, \addr_from_reg, \addr_to_reg, \data_reg, 0x7FFFFFFF, 0xFFFFFFFF


    .align 4
    load_double_to_fpr f0 , \addr_from_reg, \data_reg, 0x00000000, 0x00000000
    load_double_to_fpr f1 , \addr_from_reg, \data_reg, 0x00000000, 0x00000001
    load_double_to_fpr f0 , \addr_from_reg, \data_reg, 0x00000000, 0x00000000
    load_double_to_fpr f2 , \addr_from_reg, \data_reg, 0x00000000, 0x00000002
    load_double_to_fpr f0 , \addr_from_reg, \data_reg, 0x00000000, 0x00000000
    load_double_to_fpr f4 , \addr_from_reg, \data_reg, 0x00000000, 0x00000004
    load_double_to_fpr f0 , \addr_from_reg, \data_reg, 0x00000000, 0x00000000
    load_double_to_fpr f8 , \addr_from_reg, \data_reg, 0x00000000, 0x00000008
    load_double_to_fpr f0 , \addr_from_reg, \data_reg, 0x00000000, 0x00000000
    load_double_to_fpr f16, \addr_from_reg, \data_reg, 0x00000000, 0x00000010
    load_double_to_fpr f0 , \addr_from_reg, \data_reg, 0x00000000, 0x00000000
    load_double_to_fpr f31, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f30, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFE
    load_double_to_fpr f31, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f29, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFD
    load_double_to_fpr f31, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f27, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFB
    load_double_to_fpr f31, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f23, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFF7
    load_double_to_fpr f31, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    load_double_to_fpr f15, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFEF
    load_double_to_fpr f31, \addr_from_reg, \data_reg, 0xFFFFFFFF, 0xFFFFFFFF
    isync
    msync
    .align 4
    fsel f0 , f0 , f0 , f0
    fsel f8 , f4 , f2 , f1
    fsel f0 , f0 , f0 , f0
    fsel f8 , f4 , f2 , f1
    fsel f0 , f0 , f0 , f0
    fsel f16, f8 , f4 , f2
    fsel f0 , f0 , f0 , f0
    fsel f16, f8 , f4 , f2
    fsel f0 , f0 , f0 , f0
    fsel f1 , f16, f8 , f4
    fsel f0 , f0 , f0 , f0
    fsel f1 , f16, f8 , f4
    fsel f0 , f0 , f0 , f0
    fsel f2 , f1 , f16, f8
    fsel f0 , f0 , f0 , f0
    fsel f2 , f1 , f16, f8
    fsel f0 , f0 , f0 , f0
    fsel f4 , f2 , f1 , f16
    fsel f0 , f0 , f0 , f0
    fsel f4 , f2 , f1 , f16
    fsel f0 , f0 , f0 , f0
    fsel f31, f31, f31, f31
    fsel f31, f31, f31, f31
    fsel f23, f27, f29, f30
    fsel f31, f31, f31, f31
    fsel f23, f27, f29, f30
    fsel f31, f31, f31, f31
    fsel f15, f23, f27, f29
    fsel f31, f31, f31, f31
    fsel f15, f23, f27, f29
    fsel f31, f31, f31, f31
    fsel f30, f15, f23, f27
    fsel f31, f31, f31, f31
    fsel f30, f15, f23, f27
    fsel f31, f31, f31, f31
    fsel f29, f30, f15, f23
    fsel f31, f31, f31, f31
    fsel f29, f30, f15, f23
    fsel f31, f31, f31, f31
    fsel f27, f29, f30, f15
    fsel f31, f31, f31, f31
    fsel f27, f29, f30, f15
    fsel f31, f31, f31, f31
.endm


tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )


.global main
.section ".text","ax",@progbits
main:
test_oi10_cpu_mem_001:

    test_event_send_test_id "test_oi10_cpu_mem_001"

fpu_enable:
    mfmsr r0
    ori r0, r0, reg_field(50, 0b1) | reg_field(52, 0b1) | reg_field(55, 0b1)
    mtmsr r0

start_testing:
    load_const r4, INITIAL_DATA_POINTER
    mfspr r9, SPR_PIR
    mulli r9, r9, 0x400
    add  r4,r4,r9
    addi r7,r4,8

    clear_fprs_with_1 r4, r5
    load_const r9, 0x0
    stw r9, 0x0(r4)
    stw r9, 0x4(r4)
    stw r9, 0x8(r4)
    stw r9, 0xC(r4)
    msync
    isync
    test_event EVENT_FPRS_CLEARED_START_CHECK
    clear_fprs_with_0 r4, r5

    test r4, r7, r5
    msync
    isync


test_oi10_cpu_mem_001_fmadd_operations:
    rumboot_putstring "test_oi10_cpu_mem_001: emi_init\n"
    load_addr r3, DCR_EM2_EMI_BASE
    bl        emi_init

    rumboot_putstring "test_oi10_cpu_mem_001: turn on cache\n"
    load_addr   r3, tlb_entries
    load_size   r4, 2
    bl          ppc470s_write_tlb_entries /*(r3, r4) - are parameters*/
    msync
    isync

#ifdef RUMBOOT_PRINTF_ACCEL
    rumboot_putstring "test_oi10_cpu_mem_001: data memory fill\n"
    load_addr  r3, (INITIAL_DATA_POINTER_FMADD)
    load_size  r4, 0x800
    load_const r5, 0x00
    load_const r6, 0x01
    bl         rumboot_memfill8
#endif

    rumboot_putstring "test_oi10_cpu_mem_001: memory allocation\n"
    load_addr r3, mem_label
    load_size r4, (end_of_code - code_touch + 256)
    bl        rumboot_malloc_from_named_heap
    mr r20, r3

#ifdef RUMBOOT_PRINTF_ACCEL
    rumboot_putstring "test_oi10_cpu_mem_001: memory fill\n"
    mr r3, r20
    load_size  r4, (end_of_code - code_touch + 256)
    load_const r5, 0x00
    load_const r6, 0x01
    bl         rumboot_memfill8
#endif

    rumboot_putstring "test_oi10_cpu_mem_001: code copy\n"
    mr r3, r20
    load_addr  r4, code_touch
    load_size  r5, (end_of_code - code_touch)
    bl         memcpy

    msync
    isync

    mtctr r3
    bctr


code_touch:
    load_addr r0, (SDRAM_BASE + (start_testing_fmadd - code_touch))
    load_const r1,0x0
    addi r2, r0, 0xA00
code_touch_loop:
    icbt 0,  r1, r0
    addi r1, r1, 0x80
    cmplw cr3, r1, r2
    ble cr3, code_touch_loop
    msync

.align 7
start_testing_fmadd:
    load_const r4, INITIAL_DATA_POINTER_FMADD
    mfspr r9, SPR_PIR
    mulli r9, r9, 0x400
    add  r4,r4,r9
    addi r7,r4,8

    clear_fprs_with_1 r4, r5
//  test_event EVENT_FPRS_CLEARED_START_CHECK
    clear_fprs_with_0 r4, r5

//    test r4, r7, r5
    load_const          r10, 0x40080000
    stw                 r10, 0x0(r4)
    load_const          r10, 0x0
    stw                 r10, 0x4(r4)

    load_const          r10, 0x0
    stw                 r10, 0x8(r4)
    load_const          r10, 0x00000001
    stw                 r10, 0xC(r4)

    load_const          r10, 0x0
    stw                 r10, 0x10(r4)
    load_const          r10, 0x00000001
    stw                 r10, 0x14(r4)

    //load double into fpr0
    lfd f0, 0x00(r4) //-3.0
    lfd f1, 0x08(r4) //min
    rumboot_putstring "first sequence\n"
    .align 7

    lfd f2, 0x10(r4) //min
    fmadd f3, f0, f1, f2
    stfd f3,0x18(r4)
    fmr f2, f3
//    lfd f2,0x18(r4) //-2*min
    fmadd f4, f0, f2, f3
    stfd f4,0x20(r4)
    fmr f3, f4
//    lfd f3,0x20(r4) //4*min
    fmadd f5, f0, f3, f4
    stfd f5,0x28(r4)
    fmr f4,f5
//    lfd f4,0x28(r4) //-8*min
    fmadd f6, f0, f4, f5
    stfd f6,0x30(r4)
    fmr f5, f6
//    lfd f5,0x30(r4) //16*min
    .align 7
    nop
    .align 7
    rumboot_putstring "second sequence\n"
/*
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
*/
    .align 6
    fmadd f7, f0, f5, f6
    stfd f7,0x38(r4)
    fmr f6, f7
    fmadd f8, f0, f6, f7

    lfd f7,0x10(r4)
    fmadd f9, f0, f7, f8

    lfd f8,0x18(r4)
    fmadd f8, f0, f8, f9

    lfd f20, 0x00(r4)
    lfd f21, 0x08(r4)
    lfd f22, 0x10(r4)

    fmadd f10, f20, f21, f22
    stfd  f10, 0x40(r4)
    lfd f23, 0x10(r4)
    lfd f24, 0x18(r4)
    fmadd f11, f20, f23, f24
    .align 7
    rumboot_putstring "third sequence\n"

    .align 6
    fmadd f7, f0, f5, f6
    stfd f7,0x38(r4)
    fmr f6, f7
    fmadd f8, f0, f6, f7

    lfd f7,0x20(r4)
    fmadd f9, f0, f7, f8

    lfd f8,0x28(r4)
    fmadd f8, f0, f8, f9

    lfd f8, 0x00(r4)

    lfd f8, 0x08(r4)

    lfd f8, 0x10(r4)

    fmadd f8, f20, f21, f22
    stfd  f8, 0x40(r4)
    lfd f23, 0x10(r4)
    lfd f24, 0x18(r4)
    fmadd f11, f20, f23, f24

    rumboot_putstring "fourth sequence\n"

    .align 7
    stfd  f11, 0x48(r4)
    lfd f25, 0x18(r4)
    lfd f26, 0x20(r4)
    fmadd f12, f20, f25, f26
    stfd  f12, 0x50(r4)
    lfd f27, 0x20(r4)
    lfd f28, 0x28(r4)
    fmadd f13, f20, f27, f28
    stfd  f13, 0x58(r4)
    lfd f29, 0x28(r4)
    lfd f30, 0x30(r4)
    
    rumboot_putstring "2 write opareations(due to 2 ports) in 1 fpr\n"
    
    load_const r7, 0x10
    mtctr r7
    loop:
    .align 5
    load_addr r6, new_double_var
     fmadd f17, f1, f4, f7
      lfdux f25, r30, r29
    fmadd f19, f0, f8, f5
      lfdux f27, r30, r29
    fmadd f21, f11, f14, f15
      lfdux f29, r30, r29
    fmadd f16, f7, f10, f13
      lfdux f26, r30, r29
    fmadd f18, f12, f11, f9
      lfdux f31, r30, r29
    fmadd f22, f15, f6, f0
      lfdux f24, r30, r29
    fmadd f20, f1, f8, f3
      lfdux f28, r30, r29
    fmadd f23, f9, f5, f2
      lfdux f30, r30, r29
 

    //fmadd f30, f0, f11, f7 // "write op in 1 fpr (f30)" 
    fmadd f25, f0, f11, f7
      lfdux f17, r30, r29
    fmadd f27, f13, f3, f14
      lfdux f19, r30, r29
    fmadd f29, f10, f8, f15
      lfdux f21, r30, r29
    fmadd f26, f12, f7, f6
      lfdux f16, r30, r29
    fmadd f31, f2, f4, f5
      lfdux f18, r30, r29
    fmadd f24, f10, f9, f15
      lfdux f22, r30, r29
    fmadd f28, f7, f11, f13
      lfdux f20, r30, r29
    fmadd f30, f14, f3, f12
      lfdux f23, r30, r29

      bdnz+ loop // "end of write op in 1 fpr"

    rumboot_putstring "sqrt sequence\n"
    load_addr r4, sqrt_source
    lfd f0, 0x0(r4)
    load_addr r4, sqrt_target
    lfd f2, 0x0(r4)
    fsqrt f1, f0
    fcmpu cr1, f1,f2
    beq cr1, test_ok
    b test_error


    .align 4
    b test_ok

test_error:
    //gtube_init
    rumboot_putstring "TEST ERROR\n"
    test_event EVENT_ERROR

finish:
    b finish

test_ok:
    //gtube_init
    rumboot_putstring "TEST OK\n"
    test_event EVENT_OK
    b finish

end_of_code:



.section ".data","aw",@progbits
.align 4
sqrt_source:
    .double 56.25

.align 4
sqrt_target:
    .double   7.5
    
.align 4
new_double_var:
    .double   17.15

.align 4
mem_label:
    .asciz "SDRAM"
