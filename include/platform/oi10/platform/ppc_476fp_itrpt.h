#ifndef __PPC_476FP_ITRPT_H__
#define __PPC_476FP_ITRPT_H__


#include <platform/oi10/platform/ppc_476fp_config.h>
#include <common_macros/common_macros.h>
#include <arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/oi10/platform/boost/preprocessor/tuple/elem.hpp>
#include <platform/oi10/platform/boost/preprocessor/stringize.hpp>


#define ITRPT_N             16

#define ITRPT_TUPLE_SIZE    2
#define                               ITRPT_TUPLE_IVO_i   0
#define                                             ITRPT_TUPLE_RET_i   1
#define ITRPT_CRITICAL_INPUT        ( SPR_IVOR0,    rfci )
#define ITRPT_MACHINE_CHECK         ( SPR_IVOR1,    rfmci )
#define ITRPT_DATA_STORAGE          ( SPR_IVOR2,    rfi )
#define ITRPT_INSTRUCTION_STORAGE   ( SPR_IVOR3,    rfi )
#define ITRPT_EXTERNAL_INPUT        ( SPR_IVOR4,    rfi )
#define ITRPT_ALIGNMENT             ( SPR_IVOR5,    rfi )
#define ITRPT_PROGRAM               ( SPR_IVOR6,    rfi )
#define ITRPT_FP_UNAVAILABLE        ( SPR_IVOR7,    rfi )
#define ITRPT_SYSTEM_CALL           ( SPR_IVOR8,    rfi )
#define ITRPT_AP_UNAVAILABLE        ( SPR_IVOR9,    rfi )
#define ITRPT_DECREMENTER           ( SPR_IVOR10,   rfi )
#define ITRPT_FIXED_INTERVAL_TIMER  ( SPR_IVOR11,   rfi )
#define ITRPT_WATCHDOG_TIMER        ( SPR_IVOR12,   rfmci )
#define ITRPT_DATA_TLB_ERROR        ( SPR_IVOR13,   rfi )
#define ITRPT_INSTRUCTION_TLB_ERROR ( SPR_IVOR14,   rfi )
#define ITRPT_DEBUG                 ( SPR_IVOR15,   rfci )


#define SET_INTERRUPT_HANDLER( interrupt, handler )\
asm volatile (\
    "b      2f\n\t"\
    ".align 4\n\t"\
    "1:\n\t"\
    "stwu   "BOOST_PP_STRINGIZE(r1)",-160("BOOST_PP_STRINGIZE(r1)")\n\t"\
    "stw    "BOOST_PP_STRINGIZE(r0)", 156("BOOST_PP_STRINGIZE(r1)")\n\t"\
    "stmw   "BOOST_PP_STRINGIZE(r2)", 36("BOOST_PP_STRINGIZE(r1)")\n\t"\
    \
    "mflr   "BOOST_PP_STRINGIZE(r0)"\n\t"\
    "stw    "BOOST_PP_STRINGIZE(r0)", 32("BOOST_PP_STRINGIZE(r1)")\n\t"\
    \
    "mfcr   "BOOST_PP_STRINGIZE(r0)"\n\t"\
    "stw    "BOOST_PP_STRINGIZE(r0)", 28("BOOST_PP_STRINGIZE(r1)")\n\t"\
    \
    "mfctr  "BOOST_PP_STRINGIZE(r0)"\n\t"\
    "stw    "BOOST_PP_STRINGIZE(r0)", 24("BOOST_PP_STRINGIZE(r1)")\n\t"\
    \
    "mfxer  "BOOST_PP_STRINGIZE(r0)"\n\t"\
    "stw    "BOOST_PP_STRINGIZE(r0)", 20("BOOST_PP_STRINGIZE(r1)")\n\t"\
    \
    "lis    "BOOST_PP_STRINGIZE(r0)", %0@h\n\t"\
    "ori    "BOOST_PP_STRINGIZE(r0)", "BOOST_PP_STRINGIZE(r0)", %0@l\n\t"\
    "mtctr  "BOOST_PP_STRINGIZE(r0)"\n\t"\
    "bctrl\n\t"\
    \
    "lwz    "BOOST_PP_STRINGIZE(r0)", 20("BOOST_PP_STRINGIZE(r1)")\n\t"\
    "mtxer  "BOOST_PP_STRINGIZE(r0)"\n\t"\
    \
    "lwz    "BOOST_PP_STRINGIZE(r0)", 24("BOOST_PP_STRINGIZE(r1)")\n\t"\
    "mtctr  "BOOST_PP_STRINGIZE(r0)"\n\t"\
    \
    "lwz    "BOOST_PP_STRINGIZE(r0)", 28("BOOST_PP_STRINGIZE(r1)")\n\t"\
    "mtcr   "BOOST_PP_STRINGIZE(r0)"\n\t"\
    \
    "lwz    "BOOST_PP_STRINGIZE(r0)", 32("BOOST_PP_STRINGIZE(r1)")\n\t"\
    "mtlr   "BOOST_PP_STRINGIZE(r0)"\n\t"\
    \
    "lmw    "BOOST_PP_STRINGIZE(r2)", 36("BOOST_PP_STRINGIZE(r1)")\n\t"\
    "lwz    "BOOST_PP_STRINGIZE(r0)", 156("BOOST_PP_STRINGIZE(r1)")\n\t"\
    "addi   "BOOST_PP_STRINGIZE(r1)","BOOST_PP_STRINGIZE(r1)",160\n\t"\
    \
    BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM( ITRPT_TUPLE_SIZE, ITRPT_TUPLE_RET_i, interrupt ))"\n\t"\
    "2:\n\t"\
    "lis    "BOOST_PP_STRINGIZE(r0)", 1b@h\n\t"\
    "mtspr  "BOOST_PP_STRINGIZE(SPR_IVPR)", "BOOST_PP_STRINGIZE(r0)"\n\t"\
    "li     "BOOST_PP_STRINGIZE(r0)", 1b@l\n\t"\
    "mtspr  "BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM( ITRPT_TUPLE_SIZE, ITRPT_TUPLE_IVO_i, interrupt ))", "BOOST_PP_STRINGIZE(r0)"\n\t"\
    ::"i"(handler)\
    :BOOST_PP_STRINGIZE(r0)\
)


#endif //__PPC_476FP_ITRPT_H__
