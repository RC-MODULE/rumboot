#ifndef DIT_LIB_H
#define DIT_LIB_H

/**
 * \defgroup devices_sp805 SP804 'WD' WATCHDOG
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
 * \addtogroup devices_sp805
 *
 * @{
 */

//#define DIT_CTRL_ENABLE     (1<<7)
#define WD_CTRL_PERIODIC   (1<<6)

#define WD_CTRL_DIV1       (1<<3)
#define WD_CTRL_DIV0       (1<<2)

#define WD_CTRL_SIZE32     (1<<1)
#define WD_CTRL_INTEN           (1<<5)
#define WD_CTRL_RESEN           (1<<1)

enum sp805_mode
{
    FREERUN,
    TEST
};



/**
 *  sp805_conf         : Structure contains configuration parameters
 *  interrupt_enable   - Interrupts enabled
 *  width              - Width of the counter - 32 or 16
 *  load               - Load value to count from (won't be writen to corresponding reg if zero )
 *  bgload             - Background Load value to count from (won't be writen to corresponding reg if zero )
 */
struct sp805_conf
{
    int mode;
    int  interrupt_enable;
    int  clock_division;
    int  width;
    int  load;
//    int  bgload;
};

/**
 *  \brief Start watchdog
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp805 unit by setting its base address
 *
 *  \details Manualy sets ENABLE bit in CONTROL register of chosen timer.
 */
void sp805_enable( uint32_t base_addr, int index);

/**
 *  \brief Stop watchdog
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp805 unit by setting its base address
 *
 *  \details Manualy clears ENABLE bit in CONTROL register of chosen timer.
 */
void sp805_stop( uint32_t base_addr, int index);

/**
 *  \brief Get watchdog value
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp805 unit by setting its base address
 *  \return Return current counter value
 *
 *  \details
 */
int sp805_get_value( uint32_t base_addr, int index);

/**
 *  \brief Clear watchdog interrupt
 *
 *  \param [in] index Choose one of two identical timers
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *  \return Return nothing
 *
 *  \details Manualy writes 1 in CLEARINT register of the chosen timer.
 */
void sp805_clrint( uint32_t base_addr, int index);

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
void sp805_config( uint32_t base_addr, const struct sp805_conf * config, int index);

/**
 * @}
 */


#endif
