#ifndef DEVICES_SCRB_H
#define DEVICES_SCRB_H

/**
 * \defgroup devices_scrb_regs Registers
 * \ingroup devices_scrb
 *
 * \addtogroup devices_scrb_regs
 *
 * @{
 */


#define SCRB_REG_CR        0x000
#define SCRB_REG_TM        0x008

#define SCRB_REG_ECNT1     0x010
#define SCRB_REG_ECNT2     0x018
#define SCRB_REG_ECNT3     0x020

#define SCRB_REG_ALLCNT    0x030

#define SCRB_REG_FCNT1     0x040
#define SCRB_REG_FCNT2     0x048
#define SCRB_REG_FCNT3     0x050

#define SCRB_REG_FREF      0x060

#define SCRB_REG_IMR       0x070
#define SCRB_REG_IRR       0x078


/**
* @}
*/

#endif /* end of include guard: DEVICES_SCRB_H */
