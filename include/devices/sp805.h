#ifndef WD_LIB_H
#define WD_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/devices.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>

/**
 * \defgroup devices_sp805 'WD' WATCHDOG
 * \ingroup devices
 *
 *  Watchdog block function libriary.
 *  Contains functions such as:
 *  - Start watchdog
 *  - Stop watchdog
 *  - Get watchdog value
 *  - Clear watchdog interrupt
 *  - Config watchdog
 *
 * \code{.c}
 * #include <devices/dit_lib.h>
 * \endcode
 *
 *  Usage example:
 * \code{.c}
 *
 *  static void handler0(int irq, int base_addr)
 *  {
 *  	rumboot_printf("IRQ 0 arrived  \n");
 *
 *      sp805_clrint(base_addr,0);
 *  }
 *    int c = 0;
 *    uint32_t base_addr = BASE_ADDR;
 *
 *    struct sp805_conf config_0 = {
 *      .mode             = FREERUN,
 *      .interrupt_enable = 1      ,
 *      .clock_division   = 1      ,
 *      .width            = 32     ,
 *      .load             = 100    ,
 *      };
 * \endcode
 *
 * \addtogroup devices_sp805
 *
 * @{
 */

#define WD_CTRL_INTEN      1
#define WD_CTRL_RESEN      (1<<1)

enum sp805_mode
{
    FREERUN
};

struct sp805_conf
{
    enum sp805_mode mode;
    int  interrupt_enable;
    int  clock_division;
    int  width;
    int  load;
};

/**
 *  \brief unlock r/w for all regs
 *  \
 *  \param [in] base_addr Choose exact sp805 unit by setting its base address
 *
*/
void sp805_unlock_access(uint32_t base_addr);

/**
 *  \brief Start watchdog
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp805 unit by setting its base address
 *
 *  \details Manualy sets ENABLE bit in CONTROL register of chosen timer.
 */
void sp805_enable( uint32_t base_addr);

/**
 *  \brief Stop watchdog
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp805 unit by setting its base address
 *
 *  \details Manualy clears ENABLE bit in CONTROL register of chosen timer.
 */
void sp805_stop( uint32_t base_addr);

/**
 *  \brief Get watchdog value
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp805 unit by setting its base address
 *  \return Return current counter value
 *
 *  \details
 */
int sp805_get_value( uint32_t base_addr);

/**
 *  \brief Clear watchdog interrupt
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *  \return Return nothing
 *
 *  \details Manualy writes 1 in CLEARINT register of the chosen timer.
 */
void sp805_clrint( uint32_t base_addr);

/**
 *  \brief Config watchdog
 *
 *  \param [in] config Input structure, contains config options
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp805 unit by setting its base address
 *  \return Return nothing
 *
 *  \details Sets CONTROl register of the chosen watchdog due to input structure values,
 *   sets LOAD and BgLoad values if they are non-zero.
 */
void sp805_config( uint32_t base_addr, const struct sp805_conf * config);

/**
 * @}
 */


#endif
