/*
 * test_oi10_cpu_037.c
 *
 *  Created on: Nov 16, 2018
 *      Author: m.smolina
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/devices.h>
#include <platform/test_assert.h>
#include <rumboot/irq.h>
#include <platform/arch/ppc/ppc_476fp_fpu_fields.h>
#include <platform/arch/ppc/ppc_476fp_fpu_const.h>
#include <platform/test_event_c.h>


typedef union
{
    uint32_t w[2]; // for print
    uint64_t u;
     int64_t i;
    double   d;
} ud64_t;

typedef union
{
    uint32_t u;
     int32_t i;
    float    f;
} uf32_t;

double FPSCR_d = 1.99999e39;
float FPSCR_f = 1.123456789999e-38;
float FPSCR_ZERO = 0.0e-38;
float FPSCR_UND = 1.1e3;
float FPSCR_neg = -1.99777e20;
float FPSCR_pos = +1.99999e20;
volatile double FPSCR_Round = 1.19159013186231589874e+308;
volatile float res_com1;
volatile float res_com2;
volatile float res_com3;
volatile float res_com4;
float result_sum;
ud64_t result_d;
volatile union {
    uint64_t u;
    double d;
} const FPSCR_INF_POS = { .u =  C_D_POS_INF };

volatile union {
    uint32_t u;
    float f;
} const FPSCR_SQRT = { .u = C_S_NEG_INF };

union {
    uint32_t u;
    float f;
} const FPSCR_CONV = { .u = C_S_POS_INF };

ud64_t FPSCR_NaN_D = { .u =  C_D_NEG_NAN_MAX };

volatile uint64_t value = 0x0;

static void enable_fpu()
{

    msr_write(msr_read() | (1 << ITRPT_XSR_FP_i));

}



void read_fpu ()
{
    register double double_value;
    asm volatile
            (
                    "mffs %0 \n\t"
                    "stfd %0, 0(%1)\n\t"
                    : "=r" (double_value)
                    : "r"  (&value)
            );
}

void check_exception ()
{
    if ((value >> FPU_FPSCR_FX_i)     & 1) rumboot_printf("Floating-point exception summary [32] \n");//32
    if ((value >> FPU_FPSCR_FEX_i)    & 1) rumboot_printf("Floating-point enabled exception summary [33]\n");//33
    if ((value >> FPU_FPSCR_VX_i)     & 1) rumboot_printf("Floating-point invalid operation exception summary [34]\n");//34
    if ((value >> FPU_FPSCR_OX_i)     & 1) rumboot_printf("A floating-point overflow exception occurred [35]\n");//35
    if ((value >> FPU_FPSCR_UX_i)     & 1) rumboot_printf("A floating-point underflow exception occurred [36]\n");//36
    if ((value >> FPU_FPSCR_ZX_i)     & 1) rumboot_printf("A floating-point zero divide exception occurred [37]\n");//37
    if ((value >> FPU_FPSCR_XX_i)     & 1) rumboot_printf("A floating-point inexact exception occurred [38]\n");//38
    if ((value >> FPU_FPSCR_VXSNAN_i) & 1) rumboot_printf("A floating-point invalid operation exception (VXSNAN) occurred [39]\n");//39
    if ((value >> FPU_FPSCR_VXISI_i)  & 1) rumboot_printf("A floating-point invalid operation exception (VXISI) occurred [40]\n");//40
    if ((value >> FPU_FPSCR_VXIDI_i)  & 1) rumboot_printf("A floating-point invalid operation exception (VXIDI) occurred [41]\n");//41
    if ((value >> FPU_FPSCR_VXZDZ_i)  & 1) rumboot_printf("A floating-point invalid operation exception (VXZDZ) occurred [42]\n");//42
    if ((value >> FPU_FPSCR_VXIMZ_i)  & 1) rumboot_printf("A floating-point invalid operation exception (VXIMZ) occurred [43]\n");//43
    if ((value >> FPU_FPSCR_VXVC_i)   & 1) rumboot_printf("A floating-point invalid operation exception (VXVC) occurred [44]\n");//44
    if ((value >> FPU_FPSCR_FR_i)     & 1) rumboot_printf("Floating-point fraction rounded [45]\n");//45
    if ((value >> FPU_FPSCR_FI_i)     & 1) rumboot_printf("Floating-point fraction inexact [46]\n");//46
    if ((value >> FPU_FPSCR_FPRF_i)   & 1) rumboot_printf("Floating-point result flag (FPRF) [47]\n");//47
    if ((value >> FPU_FPSCR_FL_i)     & 1) rumboot_printf("Floating-point less than or negative [48]\n");//48
    if ((value >> FPU_FPSCR_FG_i)     & 1) rumboot_printf("Floating-point greater than or positive [49]\n");//49
    if ((value >> FPU_FPSCR_FE_i)     & 1) rumboot_printf("Floating-point equal to zero [50]\n");//50
    if ((value >> FPU_FPSCR_FU_i)     & 1) rumboot_printf("Floating-point unordered or not-a-number (NaN) [51]\n");//51
    if ((value >> FPU_FPSCR_VXSOFT_i) & 1) rumboot_printf("A floating-point invalid operation exception (software request) occurred [53]\n");//53
    if ((value >> FPU_FPSCR_VXSQRT_i) & 1) rumboot_printf("A floating-point invalid operation exception (invalid square root) occurred [54]\n");//54
    if ((value >> FPU_FPSCR_VXCVI_i)  & 1) rumboot_printf("Floating-point invalid operation exception (invalid integer convert) [55]\n");//55
    if ((value >> FPU_FPSCR_VE_i)     & 1) rumboot_printf("=========> Floating-point invalid operation exception enabled [56]\n");//56
    if ((value >> FPU_FPSCR_OE_i)     & 1) rumboot_printf("=========> Floating-point overflow exception enable [57]\n");//57
    if ((value >> FPU_FPSCR_UE_i)     & 1) rumboot_printf("=========> Floating-point underflow exception enable [58]\n");//58
    if ((value >> FPU_FPSCR_ZE_i)     & 1) rumboot_printf("=========> Floating-point zero divide exception enable [59]\n");//59
    if ((value >> FPU_FPSCR_XE_i)     & 1) rumboot_printf("=========> Floating-point inexact exception enable [60]\n");//60
    if (((value >> FPU_FPSCR_RN_i)    & 0b11) == 0b00) rumboot_printf("=========>  Round to nearest [62:63]\n");//62:63
    if (((value >> FPU_FPSCR_RN_i)    & 0b11) == 0b01) rumboot_printf("=========>  Round toward zero [62:63]\n");//62:63
    if (((value >> FPU_FPSCR_RN_i)    & 0b11) == 0b10) rumboot_printf("=========>  Round toward + infinity [62:63]\n");//62:63
    if (((value >> FPU_FPSCR_RN_i)    & 0b11) == 0b11) rumboot_printf("=========>   Round toward - infinity [62:63]\n");//62:63
}

void clearing_bits ()
{
    rumboot_printf("======> Clearing bits \n");
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (0)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (3)
                               );
    asm volatile
                      ( "mtfsb0 %0 \n\t"
                         :: "i"  (4)
                              );
    asm volatile
                      ( "mtfsb0 %0 \n\t"
                         :: "i"  (5)
                              );
    asm volatile
                      ( "mtfsb0 %0 \n\t"
                         :: "i"  (6)
                              );
    asm volatile
                      ( "mtfsb0 %0 \n\t"
                         :: "i"  (7)
                              );
    asm volatile
                      ( "mtfsb0 %0 \n\t"
                         :: "i"  (8)
                              );
    asm volatile
                      ( "mtfsb0 %0 \n\t"
                         :: "i"  (9)
                              );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (10)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (11)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (12)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (13)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (14)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (15)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (16)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (17)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (18)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (19)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (20)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (21)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (22)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (23)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (24)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (25)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (26)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (27)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (28)
                               );

}

void set_one ()
{
    rumboot_printf("======> Exceptions are enabled \n");
    asm volatile
                   ( "mtfsb1 %0 \n\t"
                         :: "i"  (24)
                               );
    asm volatile
                   ( "mtfsb1 %0 \n\t"
                         :: "i"  (25)
                               );
    asm volatile
                   ( "mtfsb1 %0 \n\t"
                         :: "i"  (26)
                               );
    asm volatile
                   ( "mtfsb1 %0 \n\t"
                         :: "i"  (27)
                               );
    asm volatile
                   ( "mtfsb1 %0 \n\t"
                         :: "i"  (28)
                               );
}

void Round_to_Nearest()
{
    rumboot_printf("Round to Nearest\n");
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (30)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (31)
                               );
}

void Round_toward_Zero ()
{
    rumboot_printf("Round toward Zero\n");
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (30)
                               );
    asm volatile
                   ( "mtfsb1 %0 \n\t"
                         :: "i"  (31)
                               );
}

void Round_toward_pos_Infinity ()
{
    rumboot_printf("Round toward + Infinity\n");
    asm volatile
                   ( "mtfsb1 %0 \n\t"
                         :: "i"  (30)
                               );
    asm volatile
                   ( "mtfsb0 %0 \n\t"
                         :: "i"  (31)
                               );
}

void Round_toward_neg_Infinity ()
{
    rumboot_printf("Round toward - Infinity\n");
    asm volatile
                   ( "mtfsb1 %0 \n\t"
                         :: "i"  (30)
                               );
    asm volatile
                   ( "mtfsb1 %0 \n\t"
                         :: "i"  (31)
                               );
}

void check_OX ()
{
    rumboot_printf("TEST OX\n");
    result_sum = FPSCR_d + FPSCR_d;
    read_fpu ();
    check_exception ();
    clearing_bits ();

    set_one ();
    result_sum = FPSCR_d + FPSCR_d;
    read_fpu ();
    check_exception ();
    clearing_bits ();
}

void check_UX ()
{
    rumboot_printf("TEST UX\n");
    result_sum = FPSCR_f/FPSCR_UND;
    read_fpu ();
    check_exception ();
    clearing_bits ();


    set_one ();
    result_sum = FPSCR_f/FPSCR_UND;
    read_fpu ();
    check_exception ();
    clearing_bits ();
}

void check_ZX ()
{
    rumboot_printf("TEST ZX\n");
    result_sum = FPSCR_d/FPSCR_ZERO;
    read_fpu ();
    check_exception ();
    clearing_bits ();

    set_one ();
    result_sum = FPSCR_d/FPSCR_ZERO;
    read_fpu ();
    check_exception ();
    clearing_bits ();
}

void check_XX ()
{
    rumboot_printf("TEST XX\n");
    result_sum = FPSCR_d;
    read_fpu ();
    check_exception ();
    clearing_bits ();

    set_one ();
    result_sum = FPSCR_d;;
    read_fpu ();
    check_exception ();
    clearing_bits ();
}

void check_sqrt ()
{
    set_one ();
    rumboot_printf("TEST SQRT\n");


    volatile float result_sum_sqrt;
    asm volatile
                (
                        "fsqrt %0, %1 \n\t"
                        : "=f" (result_sum_sqrt)
                        : "f"  (FPSCR_SQRT.f)
                );

    read_fpu ();
    check_exception ();
    clearing_bits ();


}

void check_infinity ()
{
    set_one ();
    rumboot_printf("TEST INFINITY division\n");
    result_sum = FPSCR_INF_POS.d/FPSCR_INF_POS.d;
    read_fpu ();
    check_exception ();
    clearing_bits ();
    set_one ();
    rumboot_printf("TEST INFINITY subtraction\n");
    result_sum = FPSCR_INF_POS.d - FPSCR_INF_POS.d;
    read_fpu ();
    check_exception ();
    clearing_bits ();
    set_one ();
    rumboot_printf("TEST infinity multiply zero\n");
    result_sum = FPSCR_INF_POS.d*FPSCR_ZERO;
    read_fpu ();
    check_exception ();
    clearing_bits ();
}

void check_zero_division ()
{
    rumboot_printf("TEST zero division\n");
    result_sum = FPSCR_ZERO / FPSCR_ZERO;
    read_fpu ();
    check_exception ();
    clearing_bits ();

}

void check_comparison ()
{
    rumboot_printf("TEST comparison\n");
    rumboot_printf("TEST negative\n");
    if (FPSCR_neg < FPSCR_ZERO) rumboot_printf("Test negative ok \n");
    read_fpu ();
    check_exception ();
    rumboot_printf("TEST positive\n");
    if (FPSCR_pos > FPSCR_ZERO) rumboot_printf("Test positive ok \n");
    read_fpu ();
    check_exception ();
    rumboot_printf("TEST zero\n");
    if (FPSCR_ZERO == FPSCR_ZERO) rumboot_printf("Test zero ok \n");
    read_fpu ();
    check_exception ();

    rumboot_printf("TEST NaN\n");
    result_sum = FPSCR_ZERO * FPSCR_INF_POS.d;
    read_fpu ();
    check_exception ();


    rumboot_printf("TEST VXVC\n");
    volatile float result_com = FPSCR_ZERO * FPSCR_INF_POS.d;
    volatile float result_com_0 = FPSCR_ZERO + FPSCR_INF_POS.d;
     asm volatile
                 (
                         "fcmpo %0, %1, %2 \n\t"
                         :
                         :"i" (6), "f"  (result_com), "f"  (result_com_0)
                 );
    read_fpu ();
    check_exception ();
    clearing_bits ();
}

void Integer_convert()
{
    rumboot_printf("Integer convert\n");

    volatile ud64_t  result_conv;
    FPSCR_NaN_D.u = C_D_POS_NAN_MIN;
    asm volatile
                (
                     "fctid %0, %1 \n\t"
                     : "=d" (result_conv.d)
                     : "d"  (FPSCR_NaN_D.d)
                     : "memory"
                );

    read_fpu ();
    check_exception ();
    clearing_bits ();
}

void software_defined_condition ()
{
    rumboot_printf("Software-defined condition\n");
      asm volatile
                     ( "mtfsb1 %0 \n\t"
                           :: "i"  (21)
                                 );
      read_fpu ();
      check_exception ();
      clearing_bits ();

      asm volatile
                     ( "mtfsb0 %0 \n\t"
                           :: "i"  (21)
                                 );

    read_fpu ();
    check_exception ();
    clearing_bits ();
}

void check_round ()
{

    Round_to_Nearest();
    res_com1 = FPSCR_Round;

    Round_toward_Zero();
    res_com2 = FPSCR_Round;

    Round_toward_pos_Infinity ();
    res_com3 = FPSCR_Round;

    Round_toward_neg_Infinity ();
    res_com4 = FPSCR_Round;

    if (res_com1 != res_com4) rumboot_printf("Test ok 1vs4\n");
    if (res_com3 != res_com4) rumboot_printf("Test ok 3vs4\n");
    if (res_com2 != res_com3) rumboot_printf("Test ok 2vs3\n");
    if (res_com3 != res_com1) rumboot_printf("Test ok 3vs1\n");

}



int main ()
{
    rumboot_memfill8_modelling((void*)SRAM0_BASE, 0x1000, 0x00, 0x00); //workaround (init 4KB SRAM0)

    enable_fpu();


   rumboot_printf("TEST START\n");

   read_fpu ();
   check_exception ();

    check_OX ();
    check_UX ();
    check_ZX ();
    check_XX ();
    check_sqrt ();
    check_infinity ();
    check_zero_division ();
    check_comparison ();
    Integer_convert();
    software_defined_condition ();
    check_round ();


    read_fpu ();
    check_exception ();

    rumboot_printf("TEST OK\n");
   return 0;
}
