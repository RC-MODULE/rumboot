#ifndef RUMBOOT_IO_H
#define RUMBOOT_IO_H


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
 * An more complex example is provided below:
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

 /**
  * Read a 64-bit value from memory location base_addr
  * @param  base_addr
  * @return value
  */
  static inline uint64_t ioread64(uint32_t const base_addr)
  {
      return *((volatile uint64_t*)(base_addr));
  }

 /**
  * Write a 64-bit value to memory location base_addr
  * @param value     value
  * @param base_addr base_addr
  */
  static inline void iowrite64(uint64_t const value, uint32_t const base_addr)
  {
      *((volatile uint64_t*)(base_addr)) = value;
  }


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
  * Read a 8-bit value from memory location base_addr
  * @param  base_addr
  * @return value
  */
 static inline uint8_t ioread8(uint32_t const base_addr)
 {
     return *((volatile uint8_t*)(base_addr));
 }

 /**
  * Write a 8-bit value to memory location base_addr
  * @param value     value
  * @param base_addr base_addr
  */
 static inline void iowrite8(uint8_t const value, uint32_t const base_addr)
 {
     *((volatile uint8_t*)(base_addr)) = value;
 }

/**
 * @}
 */
#endif /* end of include guard: RUMBOOT_IO_H */
