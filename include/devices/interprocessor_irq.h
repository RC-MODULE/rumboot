#ifndef INTERPROCESSOR_IRQ_H
#define INTERPROCESSOR_IRQ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/devices.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

/**
 * \defgroup devices_interprocessor_irq Interprocessor IRQ 
 * \ingroup devices
 *
 * Interprocessor IRQ  block function libriary.
 * 
 *  Contains functions such as:
 *  - Start timer
 *  - Stop timer
 *  - Get timer value
 *  - Clear timer interrupt
 *  - Config timer
 *
 * \code{.c}
 * #include <devices/interprocessor_irq_lib.h>
 * \endcode
 *
 *  Usage example:
 * \code{.c}
 *
 * 
 * static void ppc_handler0( int irq, void *arg ) {
 *     interprocessor_irq_clear_interrupt(ppc_base, 0);
 *     interprocessor_irq_clear_confirmation(0);
 * }
 * interprocessor_irq_set_interrupt(ppc_base, test_index);
 * \endcode
 *
 * \addtogroup devices_interprocessor_irq
 *
 * @{
 */





/**
 *  \brief Clears bit irq_id of SYNC reg of base_addr interprocessor_irq
 *
 *  \param [in] base_addr Base addr of interprocessor_irq block; can be PPC interprocessor_irq
 *  or NMC one
 *  \param [in] irq_id ID of a irq bit in SYNC reg
 *
 *  \details Clears bit irq_id of base_addr interprocessor_irq that created relative IRQ
 */
void interprocessor_irq_clear_interrupt(uint32_t base_addr, uint32_t irq_id);

/**
 *  \brief Set bit irq_id of SYNC reg of base_addr interprocessor_irq
 *
 *  \param [in] base_addr Base addr of interprocessor_irq block; can be PPC interprocessor_irq
 *  or NMC one
 *  \param [in] irq_id ID of a irq bit in SYNC reg
 *
 *  \details Set bit irq_id of base_addr interprocessor_irq that then creates relative IRQ
 */
void interprocessor_irq_set_interrupt(uint32_t base_addr, uint32_t irq_id);


/**
 *  \brief Set bit irq_conf_id of GP reg of base_addr interprocessor_irq
 *
 *  \param [in] base_addr Base addr of interprocessor_irq block; can be PPC interprocessor_irq
 *  or NMC one
 *  \param [in] irq_conf_id ID of a irq_conf bit in GP reg
 *
 *  \details Set bit irq_conf_id of base_addr interprocessor_irq that then
 *  sets same irq_conf_id bit of GP reg of connected interprocessor_irq
 */
void interprocessor_irq_set_irq_confirmation(uint32_t base_addr, uint32_t irq_conf_value);

/**
 *  \brief Check bit irq_conf_id of GP reg of base_addr interprocessor_irq
 *
 *  \param [in] base_addr Base addr of interprocessor_irq block; can be PPC interprocessor_irq
 *  or NMC one
 *  \param [in] irq_conf_id ID of a irq_conf bit in GP reg
 *  \param [out] Value of irq_conf_id bit at GP reg
 *
 *  \details Check bit irq_conf_id of base_addr interprocessor_irq that has been
 *  set by the same irq_conf_id bit of GP reg of connected interprocessor_irq
 */
uint32_t interprocessor_irq_get_irq_confirmation(uint32_t base_addr);

/**
 *  \brief Clear bit irq_conf_id of GP reg of base_addr interprocessor_irq
 *
 *  \param [in] base_addr Base addr of interprocessor_irq block; can be PPC interprocessor_irq
 *  or NMC one
 *  \param [in] irq_conf_id ID of a irq_conf bit in GP reg
 *
 *  \details Clear bit irq_conf_id of base_addr interprocessor_irq that has been
 *  set by the same irq_conf_id bit of GP reg of connected interprocessor_irq
 */
void interprocessor_irq_clear_irq_confirmation(uint32_t base_addr);
















#endif