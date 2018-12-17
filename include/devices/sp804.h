#ifndef DIT_LIB_H
#define DIT_LIB_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/devices.h>
#include <regs/regs_sp804.h>
#include <devices/sp804.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
/**
 * \defgroup devices_sp804 SP804 'DIT' Timer
 * \ingroup devices
 *
 *  Double Timer block function libriary.
 *  Contains functions such as:
 *  - Start timer
 *  - Stop timer
 *  - Get timer value
 *  - Clear timer interrupt
 *  - Config timer
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
 *      sp804_clrint(base_addr,0);
 *  }
 *    int c = 0;
 *    uint32_t base_addr = BASE_ADDR;
 *
 *    struct sp804_conf config_0 = {
 *      .mode             = ONESHOT,
 *      .interrupt_enable = 1      ,
 *      .clock_division   = 1      ,
 *      .width            = 32     ,
 *      .load             = 100    ,
 *      .bgload           = 0
 *      };
 *
 *    for (int i =0; i<10; i++)
 *    {
 *        sp804_config(base_addr,&config_0,0);
 *        sp804_enable(base_addr,0);
 *        while(sp804_get_value(base_addr,0)){};
 *        c++;
 *    }
 * \endcode
 *
 * \addtogroup devices_sp804
 *
 * @{
 */


#define DIT_CTRL_ENABLE     (1<<7)
#define DIT_CTRL_PERIODIC   (1<<6)
#define DIT_CTRL_INTEN      (1<<5)
#define DIT_CTRL_DIV1       (1<<3)
#define DIT_CTRL_DIV0       (1<<2)
#define DIT_CTRL_SIZE32     (1<<1)
#define DIT_CTRL_ONESHOT    1//static uint32_t config (int control, int inten, int clkdiv, int size32)                         ;


enum sp804_mode{
    ONESHOT,
    PERIODIC,
    FREERUN
};

/**
 *  sp804_conf         : Structure contains configuration parameters
 *  sp804_mode         - Chose counting mode - Oneshot, Periodic or Freerun
 *  interrupt_enable   - Interrupts enabled
 *  width              - Width of the counter - 32 or 16
 *  load               - Load value to count from (won't be writen to corresponding reg if zero )
 *  bgload             - Background Load value to count from (won't be writen to corresponding reg if zero )
 */
struct sp804_conf{
    enum sp804_mode mode;
    int  interrupt_enable;
    int  clock_division;
    int  width;
    int  load;
    int  bgload;

};

/**
 *  \brief Start timer
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *
 *  \details Manualy sets ENABLE bit in CONTROL register of chosen timer.
 */
void sp804_enable( uint32_t base_addr, int index);


/**
 *  \brief Stop timer
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *
 *  \details Manualy clears ENABLE bit in CONTROL register of chosen timer.
 */
void sp804_stop( uint32_t base_addr, int index);


/**
 *  \brief Get timer value
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *  \return Return current counter value
 *
 *  \details
 */
int sp804_get_value( uint32_t base_addr, int index);


/**
 *  \brief Clear timer interrupt
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *  \return Return nothing
 *
 *  \details Manualy writes 1 in CLEARINT register of the chosen timer.
 */
void sp804_clrint( uint32_t base_addr, int index);

/**
 *  \brief Config timer
 *
 *  \param [in] config Input structure, contains config options
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *  \return Return nothing
 *
 *  \details Sets CONTROl register of the chosen timer due to input structure values,
 *   sets LOAD and BgLoad values if they are non-zero.
 */
void sp804_config( uint32_t base_addr, const struct sp804_conf * config, int index);


/**
 * @}
 */


#endif
