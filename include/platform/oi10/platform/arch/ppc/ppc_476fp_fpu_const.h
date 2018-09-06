/*
 * ppc_476fp_fpu_const.h
 *
 *  Created on: Jul 13, 2018
 *      Author: d.arsentiev
 */

/*

        Single IEEE754 ranges:
        +---------------+----
        | +NaN max      |   0x7FFFFFFF
        +---------------+----
        | +NAN min      |   0x7F800001
        +---------------+----
        | +Inf          |   0x7F800000
        +---------------+----
        | +normal max   |   0x7F7FFFFF  (3.40282347E+38)
        +---------------+----
        | +normal min   |   0x00800000  (1.17549435E-38)
        +---------------+----
        | +denorm max   |   0x007FFFFF  (1.17549421E-38)
        +---------------+----
        | +denorm min   |   0x00000001  (1.40129846E-45)
        +---------------+----
        | +0            |   0x00000000
    ----+---------------+----------------
        | -0            |   0x80000000
        +---------------+----
        | -denorm max   |   0x80000001  (-1.40129846E-45)
        +---------------+----
        | -denorm min   |   0x807FFFFF  (-1.17549421E-38)
        +---------------+----
        | -normal max   |   0x80800000  (-1.17549435E-38)
        +---------------+----
        | -normal min   |   0xFF7FFFFF  (-3.40282347E+38)
        +---------------+----
        | -Inf          |   0xFF800000
        +---------------+----
        | -NaN max      |   0xFF800001
        +---------------+----
        | -NaN min      |   0xFFFFFFFF
        +---------------+----

        Double IEEE754 ranges:
        +---------------+----
        | +NaN max      |   0x7FFFFFFFFFFFFFFF
        +---------------+----
        | +NAN min      |   0x7FF0000000000001
        +---------------+----
        | +Inf          |   0x7FF0000000000000
        +---------------+----
        | +normal max   |   0x7FEFFFFFFFFFFFFF  (1.797693134862315708E+308)
        +---------------+----
        | +normal min   |   0x0010000000000000  (2.225073858507201338E-308)
        +---------------+----
        | +denorm max   |   0x000FFFFFFFFFFFFF  (2.225073858507200889E-308)
        +---------------+----
        | +denorm min   |   0x0000000000000001  (4.940656458412465441E-324)
        +---------------+----
        | +0            |   0x0000000000000000
    ----+---------------+----------------
        | -0            |   0x8000000000000000
        +---------------+----
        | -denorm max   |   0x8000000000000001  (-4.940656458412465441E-324)
        +---------------+----
        | -denorm min   |   0x800FFFFFFFFFFFFF  (-2.225073858507200889E-308)
        +---------------+----
        | -normal max   |   0x8010000000000000  (-2.225073858507201338E-308)
        +---------------+----
        | -normal min   |   0xFFEFFFFFFFFFFFFF  (-1.797693134862315708E+308)
        +---------------+----
        | -Inf          |   0xFFF0000000000000
        +---------------+----
        | -NaN max      |   0xFFF0000000000001
        +---------------+----
        | -NaN min      |   0xFFFFFFFFFFFFFFFF
        +---------------+----
*/

#ifndef PPC_476FP_FPU_CONST_H_
#define PPC_476FP_FPU_CONST_H_

/* Math constants */
#define M_E             2.7182818284590452354
#define M_LOG2E         1.4426950408889634074
#define M_LOG10E        0.43429448190325182765
#define M_LN2           0.693147180559945309417
#define M_LN10          2.30258509299404568402
#define M_PI            3.14159265358979323846
#define M_PI_2          1.57079632679489661923
#define M_PI_4          0.78539816339744830962
#define M_1_PI          0.31830988618379067154
#define M_2_PI          0.63661977236758134308
#define M_SQRT2         1.41421356237309504880
#define M_SQRT1_2       0.70710678118654752440
#define M_TWOPI         6.283185307179586476925286
#define M_SQRTPI        1.77245385090551602792981
#define M_LN2LO         1.9082149292705877000E-10
#define M_LN2HI         6.9314718036912381649E-1
#define M_SQRT3         1.73205080756887719000

/* Denormal single positive */
#define C_S_DEN_P_MIN   0x00000001
#define C_S_DEN_P_01    (0xFFA5A5A5 & 0x007FFFFF)
#define C_S_DEN_P_10    (0xFF5A5A5A & 0x007FFFFF)
#define C_S_DEN_P_MAX   0x007FFFFF
/* Denormal single negative */
#define C_S_DEN_N_MIN   0x807FFFFF
#define C_S_DEN_N_01    (0xFFA5A5A5 & 0x807FFFFF)
#define C_S_DEN_N_10    (0xFF5A5A5A & 0x807FFFFF)
#define C_S_DEN_N_MAX   0x80000001
/* Denormal double positive */
#define C_D_DEN_P_MIN   0x0000000000000001
#define C_D_DEN_P_01    0x0005A5A5A5A5A5A5
#define C_D_DEN_P_10    0x000A5A5A5A5A5A5A
#define C_D_DEN_P_MAX   0x000FFFFFFFFFFFFF
/* Denormal double negative */
#define C_D_DEN_N_MIN   0x800FFFFFFFFFFFFF
#define C_D_DEN_N_01    0x8005A5A5A5A5A5A5
#define C_D_DEN_N_10    0x800A5A5A5A5A5A5A
#define C_D_DEN_N_MAX   0x8000000000000001

/* +0, -0, +Inf, -Inf, +NaN, -NaN single */
#define C_S_POS_ZERO    0x00000000
#define C_S_NEG_ZERO    0x80000000
#define C_S_POS_INF     0x7F800000
#define C_S_NEG_INF     0xFF800000
#define C_S_POS_NAN_MIN 0x7F800001
#define C_S_POS_NAN_MAX 0x7FFFFFFF
#define C_S_NEG_NAN_MIN 0xFF800001
#define C_S_NEG_NAN_MAX 0xFFFFFFFF

/* +0, -0, +Inf, -Inf, +NaN, -NaN double */
#define C_D_POS_ZERO    0x0000000000000000
#define C_D_NEG_ZERO    0x8000000000000000
#define C_D_POS_INF     0x7FF0000000000000
#define C_D_NEG_INF     0xFFF0000000000000
#define C_D_POS_NAN_MIN 0x7FF0000000000001
#define C_D_POS_NAN_MAX 0x7FFFFFFFFFFFFFFF
#define C_D_NEG_NAN_MIN 0xFFF0000000000001
#define C_D_NEG_NAN_MAX 0xFFFFFFFFFFFFFFFF


#endif /* PPC_476FP_FPU_CONST_H_ */
