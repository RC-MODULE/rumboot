#ifndef RUMBOOT_IO_H
#define RUMBOOT_IO_H

#include <stdint.h>

/**
 *
 * \defgroup memio Register access functions
 * \ingroup libraries
 *
 * These functions should be used to access device registers.
 * \code{.c}
 * #include <rumboot/io.h>
 * \endcode
 *
 * A more complex example is provided below:
 *
 * \code{.c}
 * #include <rumboot/io.h>
 *   uint32_t temp_data;  // All the variables should be declared in stack yet
 *   iowrite32(0x000000DA, GPIO1_BASE + GPIO_DATA);
 *   iowrite32(0x00000000, GPIO0_BASE + GPIO_DATA);
 *   iowrite32(0x00000009, GPIO0_BASE + GPIO_DATA);
 *     //
 *   temp_data = ioread32(GPIO1_BASE + GPIO_DATA);
 *
 *   rumboot_printf("GPIO1=0x%x\n",temp_data);
 *
 *   if(temp_data == 0x000000DA)
 *     return 0;
 *   else
 *     return 1;
 *
 * \endcode
 * \addtogroup memio
 *  @{
 */


 /* These forward declarations are needed for additional sanity-checking */
/**
 * Read a 64-bit double value from memory location base_addr
 * @param  base_addr
 * @return value
 */
static inline double ioread64d(uint32_t const base_addr); 
/**
 * Write a 64-bit double value to memory location base_addr
 * @param value     value
 * @param base_addr base_addr
 */
static inline void iowrite64d(double const value, uint32_t const base_addr);
/**
 * Read a 64-bit value from memory location base_addr
 * @param  base_addr
 * @return value
 */
static inline uint64_t ioread64(uint32_t const base_addr);
/**
 * Write a 64-bit value to memory location base_addr
 * @param value     value
 * @param base_addr base_addr
 */
static inline void iowrite64(uint64_t const value, uint32_t const base_addr);
/**
 * Read a 32-bit value from memory location base_addr
 * @param  base_addr
 * @return value
 */
static inline uint32_t ioread32(uint32_t const base_addr);
/**
 * Write a 32-bit value to memory location base_addr
 * @param value     value
 * @param base_addr base_addr
 */
static inline void iowrite32(uint32_t const value, uint32_t const base_addr);
/**
 * Read a 16-bit value from memory location base_addr
 * @param  base_addr
 * @return value
 */
static inline uint16_t ioread16(uint32_t const base_addr);
/**
 * Write a 16-bit value to memory location base_addr
 * @param value     value
 * @param base_addr base_addr
 */
static inline void iowrite16(uint16_t const value, uint32_t const base_addr);
/**
 * Read a 8-bit value from memory location base_addr
 * @param  base_addr
 * @return value
 */
static inline uint8_t ioread8(uint32_t const base_addr);
/**
 * Write a 8-bit value to memory location base_addr
 * @param value     value
 * @param base_addr base_addr
 */
static inline void iowrite8(uint8_t const value, uint32_t const base_addr);

#if defined(__PPC__) || defined(__NM__)
    #include <arch/io.h>
#endif

#ifndef RUMBOOT_ARCH_HAS_IO

static __attribute__((no_instrument_function)) inline double ioread64d(uint32_t const base_addr)
{
    return *((volatile double*)(base_addr));
}

static __attribute__((no_instrument_function)) inline void iowrite64d(double const value, uint32_t const base_addr)
{
    *((volatile double*)(base_addr)) = value;
}

static __attribute__((no_instrument_function)) inline uint64_t ioread64(uint32_t const base_addr)
{
    return *((volatile uint64_t*)(base_addr));
}

static __attribute__((no_instrument_function)) inline void iowrite64(uint64_t const value, uint32_t const base_addr)
{
    *((volatile uint64_t*)(base_addr)) = value;
}

static __attribute__((no_instrument_function)) inline uint32_t ioread32(uint32_t const base_addr)
{
    return *((volatile uint32_t*)(base_addr));
}

static __attribute__((no_instrument_function)) inline void iowrite32(uint32_t const value, uint32_t const base_addr)
{
    *((volatile uint32_t*)(base_addr)) = value;
}

static __attribute__((no_instrument_function)) inline uint16_t ioread16(uint32_t const base_addr)
{
    return *((volatile uint16_t*)(base_addr));
}

static __attribute__((no_instrument_function)) inline void iowrite16(uint16_t const value, uint32_t const base_addr)
{
    *((volatile uint16_t*)(base_addr)) = value;
}

static __attribute__((no_instrument_function)) inline uint8_t ioread8(uint32_t const base_addr)
{
    return *((volatile uint8_t*)(base_addr));
}

static __attribute__((no_instrument_function)) inline void iowrite8(uint8_t const value, uint32_t const base_addr)
{
    *((volatile uint8_t*)(base_addr)) = value;
}

#endif

/**
 * @}
 */
#endif /* end of include guard: RUMBOOT_IO_H */
