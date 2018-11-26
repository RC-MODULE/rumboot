#ifndef PPC_476FP_LIB_C_H
#define PPC_476FP_LIB_C_H


#include <stdint.h>
#include <rumboot/io.h>


/*MSR access*/
static inline __attribute__((always_inline)) void msr_write( uint32_t const wval ) {
    asm volatile (
        "mtmsr %0 \n\t"
        ::  "r"(wval)
        :   "memory"
    );
}

static inline __attribute__((always_inline)) uint32_t msr_read() {
    uint32_t rval = 0;
    asm volatile (
        "mfmsr %0 \n\t"
        :   "=r"(rval)
        ::  "memory"
    );
    return rval;
}

/*SPR access*/
#define spr_write( spr_reg, value )\
    asm volatile (\
        "mtspr %1, %0 \n\t"\
        ::"r"(value), "i"(spr_reg)\
    )

#define spr_read( spr_reg ) ({\
    uint32_t rval = 0;\
    asm volatile (\
        "mfspr %0, %1 \n\t"\
        :"=r"(rval)\
        :"i"(spr_reg)\
    );\
    rval;\
})


#define dcread( addr ) ({\
    uint32_t rval = 0;\
    asm volatile (\
        "dcread %0, 0, %1 \n\t"\
        :"=r"(rval)\
        :"r"(addr)\
    );\
    rval;\
})

static inline __attribute__((always_inline)) void icbi( void* const addr ) {
    asm volatile (
        "icbi 0, %0\n\t"
        ::  "r"(addr)
    );
}

static inline __attribute__((always_inline)) void icbt( void* const addr ) {
    asm volatile (
        "icbt 0, 0, %0\n\t"
        ::  "r"(addr)
    );
}

static inline __attribute__((always_inline)) void icread( void* const addr ) {
    asm volatile (
        "icread 0, %0\n\t"
        ::  "r"(addr)
    );
}

static inline __attribute__((always_inline)) void dcbf( void* const addr ) {
    asm volatile (
        "dcbf 0, %0\n\t"
        ::  "r"(addr)
    );
}

static inline __attribute__((always_inline)) void dcbi( void* const addr ) {
    asm volatile (
        "dcbi 0, %0\n\t"
        ::  "r"(addr)
    );
}

static inline __attribute__((always_inline)) void dcbz( void* const addr ) {
    asm volatile (
        "dcbz 0, %0\n\t"
        ::  "r"(addr)
    );
}

static inline __attribute__((always_inline)) void dcbst( void* const addr ) {
    asm volatile (
        "dcbst 0, %0\n\t"
        ::"r"(addr)
    );
}

static inline __attribute__((always_inline)) void dcbt( void* const addr ) {
    asm volatile (
        "dcbt 0, 0, %0\n\t"
        ::"r"(addr)
    );
}
/* These instruction calls must be placed in the same function
 * where called.
 * It cannot be wrapped within an inline function, because
 * inlining function is not a must-do but only recommendation
 * to the compiler*/

#define rfi()\
    asm volatile (\
        "rfi \n\t"\
    )

#define rfci()\
    asm volatile (\
        "rfci \n\t"\
    )

#define rfmci()\
    asm volatile (\
        "rfmci \n\t"\
    )

#define isync()\
    asm volatile (\
        "isync \n\t"\
    )

#define ici( CT )\
    asm volatile (\
        "ici %0 \n\t"\
        ::  "i"(CT)\
    )

#define dci( CT )\
    asm volatile (\
        "dci %0 \n\t"\
        ::  "i"(CT)\
    )

#define nop()\
    asm volatile (\
        "nop \n\t"\
    )

#define sc()\
    asm volatile (\
        "sc \n\t"\
    )

#define trap()\
    asm volatile (\
        "trap \n\t"\
    )

#define tlbsync()\
    asm volatile (\
        "tlbsync \n\t"\
    )

#endif  /* PPC_476FP_LIB_C_H*/
