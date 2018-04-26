#ifndef PPC_476FP_LIB_C_H
#define PPC_476FP_LIB_C_H

#include <stdint.h>

#define MSR_EE              48
#define MSR_CE              46


/*DCR bus access*/
static inline void dcr_write( uint32_t const addr, uint32_t const wval )
{
    asm volatile
    (
        "mtdcrx %0, %1 \n\t"
        ::"r"(addr), "r"(wval)
    );
}

static inline uint32_t dcr_read( uint32_t const addr )
{
    uint32_t rval=0;
    asm volatile
    (
        "mfdcrx %0, %1 \n\t"
        :"=r"(rval)
        :"r"(addr)
    );
    return rval;
}

/*MSR access*/
static inline void msr_write( uint32_t const wval )
{
    asm volatile
    (
        "mtmsr %0 \n\t"
        ::"r"(wval)
    );
}

static inline uint32_t msr_read()
{
    uint32_t rval=0;
    asm volatile
    (
        "mfmsr %0 \n\t"
        :"=r"(rval)
    );
    return rval;
}

/*DCR bus access*/
static inline void mtdcrx( uint32_t const addr, uint32_t const wval )
{
    asm volatile
    (
        "mtdcrx %0, %1 \n\t"
        ::"r"(addr), "r"(wval)
    );
}

static inline uint32_t mfdcrx( uint32_t const addr )
{
    uint32_t rval=0;
    asm volatile
    (
        "mfdcrx %0, %1 \n\t"
        :"=r"(rval)
        :"r"(addr)
    );
    return rval;
}

/*MSR access*/
static inline void mtmsr( uint32_t const wval )
{
    asm volatile
    (
        "mtmsr %0 \n\t"
        ::"r"(wval)
    );
}

static inline uint32_t mfmsr()
{
    uint32_t rval=0;
    asm volatile
    (
        "mfmsr %0 \n\t"
        :"=r"(rval)
    );
    return rval;
}

inline static void dcbi( void* const addr )
{
    asm volatile
    (
        "dcbi 0, %0\n\t"
        ::"r"(addr)
    );
}

inline static void dcbz( void* const addr )
{
    asm volatile
    (
        "dcbz 0, %0\n\t"
        ::"r"(addr)
    );
}

inline static void dcbst( void* const addr )
{
    asm volatile
    (
        "dcbst 0, %0\n\t"
        ::"r"(addr)
    );
}

inline static void sc()
{
    asm volatile
    (
        "sc\n\t"
    );
}

inline static void trap()
{
    asm volatile
    (
        "trap\n\t"
    );
}

/* These instruction calls must be placed in the same function
 * where called.
 * It cannot be wrapped within an inline function, because
 * inlining function is not a must-do but only recommendation
 * to the compiler*/
#define rfi()\
    asm volatile\
    (\
        "rfi \n\t"\
    )

#define rfci()\
    asm volatile\
    (\
        "rfci \n\t"\
    )

#define rfmci()\
    asm volatile\
    (\
        "rfmci \n\t"\
    )

#define isync()\
    asm volatile\
    (\
        "isync \n\t"\
    )

#define mbar()\
    asm volatile\
    (\
        "mbar \n\t"\
    )

#define msync()\
    asm volatile\
    (\
        "msync \n\t"\
    )

#define lwsync()\
    asm volatile\
    (\
        "lwsync \n\t"\
    )

#define ici( CT )\
    asm volatile\
    (\
        "ici %0 \n\t"\
        ::"i"(CT)\
    )

#define dci( CT )\
    asm volatile\
    (\
        "dci %0 \n\t"\
        ::"i"(CT)\
    )

#define nop()\
    asm volatile\
    (\
        "nop \n\t"\
    )

#endif  /* PPC_476FP_LIB_C_H*/
