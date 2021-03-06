#ifndef PPC_476FP_LIB_C_H
#define PPC_476FP_LIB_C_H


#include <stdint.h>
#include <stdbool.h>
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

#define icbt( CT, addr )\
    asm volatile (\
        "icbt %0, 0, %1\n\t"\
        ::  "i"(CT), "r"(addr)\
    )

#define icblc( CT, addr )\
    asm volatile (\
        "icblc %0, 0, %1\n\t"\
        ::  "i"(CT), "r"(addr)\
    )

#define icbtls( CT, addr )\
    asm volatile (\
        "icbtls %0, 0, %1\n\t"\
        ::  "i"(CT), "r"(addr)\
    )

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

#define dcbt( TH, addr )\
    asm volatile (\
        "dcbt %0, 0, %1\n\t"\
        ::  "i"(TH), "r"(addr)\
    )

#define dcbtst( TH, addr )\
    asm volatile (\
        "dcbtst %0, 0, %1\n\t"\
        ::  "i"(TH), "r"(addr)\
    )

static inline __attribute__((always_inline)) void dcba( void* const addr ) {
    asm volatile (
        "dcba 0, %0\n\t"
        ::"r"(addr)
    );
}

#define dcblc( CT, addr )\
    asm volatile (\
        "dcblc %0, 0, %1\n\t"\
        ::  "i"(CT), "r"(addr)\
    )

#define dcbtls( CT, addr )\
    asm volatile (\
        "dcbtls %0, 0, %1\n\t"\
        ::  "i"(CT), "r"(addr)\
    )

#define dcbtstls( CT, addr )\
    asm volatile (\
        "dcbtstls %0, 0, %1\n\t"\
        ::  "i"(CT), "r"(addr)\
    )

static inline __attribute__((always_inline))
uint32_t stwcx(uint32_t wval, uintptr_t wptr) {
    uint32_t rval = 0;
    asm volatile (
            "stwcx. %1, 0, %2       \n\t"
            "mfcr   %0              \n\t"
            "andi.  %0, %0, 0x0F    \n\t"
        :   "=r"(rval)
        :   "r"(wval), "r"(wptr)
        :   "cr0", "memory"
    );
    return rval;
}

static inline __attribute__((always_inline))
uint32_t lwarx(uintptr_t wptr) {
    uint32_t rval = 0;
    asm volatile (
        "lwarx %0, 0, %1 \n\t"
        :   "=r"(rval)
        :   "r"(wptr)
        :   "memory"
    );
    return rval;
}

static inline __attribute__((always_inline)) bool parity32( uint32_t const val ) {
    bool rval;
    asm volatile (
        "popcntb    %0, %1\n\t"
        "prtyw      %0, %0\n\t"
        :   "=r"(rval)
        :   "r"(val)
    );
    return !rval;
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

#define tlbsx( RB ) ({\
    int32_t RT;\
    uint32_t RB_ = RB;\
    asm volatile (\
        "tlbsx. %0, 0, %1\n\t"          /* RT[33:34] = tlbentry[way], RT[40:47] = tlbentry[index] */\
        "crnot  2, 2\n\t"               /* CR[CR0[2]] = !CR[CR0[2]] */\
        "mfcr   %1\n\t"\
        "rlwimi %0, %1, 2, 0, 0\n\t"    /* RT[32] = CR[CR0[2]] */\
        :   "=r"(RT), "+r"(RB_)\
        ::  "cr0"\
    );\
    RT;                                 /* if RT >= 0 then tlbentry is found */\
})

#define tlbre( RA, WS ) ({\
    uint32_t RT;\
    asm volatile (\
        "tlbre %0, %1, %2\n\t"\
        :   "=r"(RT)\
        :   "r"(RA), "i"(WS)\
    );\
    RT;\
})

#define tlbwe( RS, RA, WS )\
    asm volatile (\
        "tlbwe %0, %1, %2\n\t"\
        ::  "r"(RS), "r"(RA), "i"(WS)\
    )

#define tlbivax( RB )\
    asm volatile (\
        "tlbivax 0, %0\n\t"\
        ::  "r"(RB)\
    )


#endif  /* PPC_476FP_LIB_C_H*/
