#ifndef RUMBOOT_IO_H
#define RUMBOOT_IO_H


/**
 *
 * \defgroup memio Register access functions
 * These functions should be used to access device registers.
 * \code{.c}
 * #include <rumboot/io.h>
 * \endcode
 *
 * \addtogroup memio
 *  @{
 */

/**
 * Read a 32-bit value from memory location base_addr
 * @param  base_addr
 * @return value
 */
 static inline uint32_t ioread32(uint32_t const base_addr)
 {
     return *((volatile uint32_t*)(base_addr));
 }

/**
 * Write a 32-bit value to memory location base_addr
 * @param value     value
 * @param base_addr base_addr
 */
 static inline void iowrite32(uint32_t const value, uint32_t const base_addr)
 {
     *((volatile uint32_t*)(base_addr)) = value;
 }

 /**
  * Read a 16-bit value from memory location base_addr
  * @param  base_addr
  * @return value
  */
 static inline uint16_t ioread16(uint32_t const base_addr)
 {
     return *((volatile uint16_t*)(base_addr));
 }

 /**
  * Write a 16-bit value to memory location base_addr
  * @param value     value
  * @param base_addr base_addr
  */
 static inline void iowrite16(uint16_t const value, uint32_t const base_addr)
 {
     *((volatile uint16_t*)(base_addr)) = value;
 }

/**
 * @}
 */
#endif /* end of include guard: RUMBOOT_IO_H */
