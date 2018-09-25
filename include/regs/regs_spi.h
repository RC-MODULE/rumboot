#ifndef REGS_SPI_H
#define REGS_SPI_H

/**
 *
 * \defgroup regs_SPI
 * \ingroup spi
 *
 * \addtogroup regs_SPI
 * @{
 */
 #define PL022_CPSR  0x10
 #define PL022_CR0 0x0
 #define PL022_IMSC 0x14
 #define PL022_ICR 0x20
 #define PL022_CR1 0x4
 #define PL022_DMACR 0x24
 #define PL022_SR 0xc
 #define PL022_DR 0x8
 #define SSPSR_SOFTCS                    0xCC
/**
  * @}
*/
 /**
  *
  * \defgroup spi_bit_positions Bit_positions
  * \ingroup spi
  *
  * \addtogroup spi_bit_positions
  * @{
  */
#define PL022_SR__RNE_i 2
#define PL022_SR__TNF_i 1
#define PL022_SR__TFE_i 0
#define PL022_CR1__SOD_i 3
#define PL022_CR1__SSE_i 1
/**
 * @}

 * \defgroup spi_constants  Constants
 * \ingroup spi
 *
 * \addtogroup spi_constants
 * @{
 */
#define SPI_READ_CMD_TIMEOUT 500
#define SPI_READ_TIMEOUT 500
/**
 * @}
 */
#endif
