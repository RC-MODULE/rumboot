/*
 * io.h
 *
 *  Created on: Jul 24, 2018
 *      Author: r.galiulin
 */

#ifndef PPC_ARCH_IO_H
#define PPC_ARCH_IO_H


#define RUMBOOT_ARCH_HAS_IO


static inline __attribute__((no_instrument_function)) __attribute__((always_inline)) uint64_t __attribute__((optimize("no-delete-null-pointer-checks"))) ioread64( uint32_t const base_addr ) {
    return *( ( volatile uint64_t* )( base_addr ) );
}

static inline __attribute__((no_instrument_function)) __attribute__((always_inline)) void __attribute__((optimize("no-delete-null-pointer-checks"))) iowrite64( uint64_t const value, uint32_t const base_addr ) {
    *( ( volatile uint64_t* )( base_addr ) ) = value;
}

static inline __attribute__((no_instrument_function)) __attribute__((always_inline)) uint32_t ioread32( uint32_t const base_addr ) {
    uint32_t rval = 0;
    asm volatile (
        "lwzx %0, 0, %1 \n\t"
        :   "=r"(rval)
        :   "r"(base_addr)
    );
    return rval;
}

static inline __attribute__((no_instrument_function)) __attribute__((always_inline)) void iowrite32( uint32_t const value, uint32_t const base_addr ) {
    asm volatile (
        "stwx %0, 0, %1 \n\t"
        ::  "r"(value), "r"(base_addr)
    );
}

static inline __attribute__((no_instrument_function)) __attribute__((always_inline)) uint16_t ioread16( uint32_t const base_addr ) {
    uint16_t rval = 0;
    asm volatile (
        "lhzx %0, 0, %1 \n\t"
        :   "=r"(rval)
        :   "r"(base_addr)
    );
    return rval;
}

static inline __attribute__((no_instrument_function)) __attribute__((always_inline)) void iowrite16( uint16_t const value, uint32_t const base_addr ) {
    asm volatile (
        "sthx %0, 0, %1 \n\t"
        ::  "r"(value), "r"(base_addr)
    );
}

static inline __attribute__((no_instrument_function)) __attribute__((always_inline)) uint8_t ioread8( uint32_t const base_addr ) {
    uint8_t rval = 0;
    asm volatile (
        "lbzx %0, 0, %1 \n\t"
        :   "=r"(rval)
        :   "r"(base_addr)
    );
    return rval;
}

static inline __attribute__((no_instrument_function)) __attribute__((always_inline)) void iowrite8( uint8_t const value, uint32_t const base_addr ) {
    asm volatile (
        "stbx %0, 0, %1 \n\t"
        ::  "r"(value), "r"(base_addr)
    );
}

/*DCR bus access*/
static inline __attribute__((no_instrument_function)) __attribute__((always_inline)) void dcr_write( uint32_t const addr, uint32_t const wval ) {
    asm volatile (
        "mtdcrx %0, %1 \n\t"
        ::  "r"(addr), "r"(wval)
    );
}

static inline __attribute__((no_instrument_function)) __attribute__((always_inline)) uint32_t dcr_read( uint32_t const addr ) {
    uint32_t rval = 0;
    asm volatile (
        "mfdcrx %0, %1 \n\t"
        :   "=r"(rval)
        :   "r"(addr)
    );
    return rval;
}

/*Memory sync*/
#define mbar()\
    asm volatile (\
        "mbar \n\t"\
    )

#define msync()\
    asm volatile (\
        "msync \n\t"\
    )

#define lwsync()\
    asm volatile (\
        "lwsync \n\t"\
    )


#endif /* PPC_ARCH_IO_H */
