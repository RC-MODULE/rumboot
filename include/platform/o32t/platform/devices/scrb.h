#ifndef SCRB_H
#define SCRB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/devices.h>

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


/**
 *  \brief Set scrubber timer timer
 *
 *  \param [in] timer_value New timer value, should be devisible by 8
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *
 *  \details Set new value for scrubber timer - register SCRB_REG_TM
 */
void scrb_set_timer(uint32_t base_addr, uint32_t timer_value);


/**
 *  \brief Set ALLCNT reg value
 *
 *  \param [in] allcnt_value New ALLCNT value
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *
 *  \details Set new value for ALLCNT reg - sum of amount of errors of both im1 and im2 after which there is an interrupt
 */
void scrb_set_allcnt(uint32_t base_addr, uint32_t allcnt_value);


/**
 *  \brief Set FREF reg value
 *
 *  \param [in] fref_value New FREF value
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *
 *  \details Set new value for FREF reg - amount of errors for either im1 and im2 after which there is an interrupt
 */
void scrb_set_fref(uint32_t base_addr, uint32_t fref_value);


/**
 *  \brief Set IMR reg value
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *  \param [in] imr_value New IMR value
 *
 *  \details Set new value for IMR reg - IMR[0] masks interrupt from FREF, IMR[1] - from ALLCNT
 */
void scrb_set_imr(uint32_t base_addr, uint32_t imr_value);


/**
 *  \brief Set work mode of scrubber
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *  \param [in] mode_value Work mode value
 *
 *  \details Config work mode - SCRB_CR[3]
 */
void scrb_set_work_mode(uint32_t base_addr, uint32_t mode_value);


/**
 *  \brief Set priority of scrubber
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *  \param [in] priority_value Priority value
 *
 *  \details Config priority - SCRB_CR[4]
 */
void scrb_set_priority(uint32_t base_addr, uint32_t priority_value);


/**
 *  \brief Start scrubber
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *  \param [in] start_im2_im1_bits SCRB_CR[1:0] value
 *
 *  \details Start scrubber - SCRB_CR[2] start for im3, SCRB_CR[1] start for im2, SCRB_CR[0] - for im1
 */
void scrb_start_scrubber(uint32_t base_addr, uint32_t start_im3_im2_im1_bits);



/**
 *  \brief Stop scrubber for im1
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *
 *  \details Stop scrubber for im1
 */
void scrb_stop_scrubbing_im1(uint32_t base_addr);

/**
 *  \brief Stop scrubber for im2
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *
 *  \details Stop scrubber for im2
 */
void scrb_stop_scrubbing_im2(uint32_t base_addr);

/**
 *  \brief Stop scrubber for im3
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *
 *  \details Stop scrubber for im3
 */
void scrb_stop_scrubbing_im3(uint32_t base_addr);


/**
 *  \brief Clear interrupt SCRB_REG_IRR[0]
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *  \details Clear interrupt SCRB_REG_IRR[0] - from FREF
 */
void scrb_clear_interrupt_0(uint32_t base_addr);


/**
 *  \brief Clear interrupt SCRB_REG_IRR[1]
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *  \details Clear interrupt SCRB_REG_IRR[1] - from ALLCNT
 */
void scrb_clear_interrupt_1(uint32_t base_addr);




/**
 *  \brief Get number of errors at im1
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *  \details Get number of errors at im1 at the end of scrubbing, reg FCNT1
 */
int scrb_get_number_im1_errors(uint32_t base_addr);


/**
 *  \brief Get number of errors at im2
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *  \details Get number of errors at im2 at the end of scrubbing, reg FCNT2
 */
int scrb_get_number_im2_errors(uint32_t base_addr);


/**
 *  \brief Get number of errors at im3
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *  \details Get number of errors at im2 at the end of scrubbing, reg FCNT3
 */
int scrb_get_number_im3_errors(uint32_t base_addr);



/**
 *  \brief Get number of errors at both mem banks
 *
 *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
 *  \details Get number of errors at both mem banks at the end of scrubbing, reg ALLCNT
 */
int scrb_get_number_total_errors_left_in_reg(uint32_t base_addr);



///**
// *  \brief Check if scrubbing is done
// *
// *  \param [in] base_addr Set base addr from test, cuz you cant get it from lib for some reason
// *  \param [in] im1_enable Set 1 if scrubing is enabled for im1
// *  \param [in] im2_enable Set 1 if scrubing is enabled for im2
// *  \param [in] im3_enable Set 1 if scrubing is enabled for im3
// *  \details Check values of FCNT reg and return if scrubbing proccess is finished
// */
//uint32_t scrb_scrubbing_is_finished(uint32_t base_addr, int im1_enable, int im2_enable, int im3_enable);
//
///**
// * @}
// */


#endif