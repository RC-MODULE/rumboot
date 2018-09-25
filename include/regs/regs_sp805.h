#ifndef DEVICES_WD_H
#define DEVICES_WD_H

/**
 * \defgroup devices_sp805_regs Registers
 * \ingroup devices_sp805
 *
 * \addtogroup devices_sp805_regs
 *
 * @{
 */
#define WD_REG_LOAD             0x00
#define WD_REG_VALUE            0x04
#define WD_REG_CONTROL          0x08
#define WD_REG_INTCLR           0x0C
#define WD_REG_RIS              0x10
#define WD_REG_MIS              0x14
#define WD_REG_LOCK             0xC0
#define WD_REG_ITCR             0xF0
#define WD_REG_ITOP             0xF04

#define WD_REG_PERIPHID0        0xFE0
#define WD_REG_PERIPHID1        0xFE4
#define WD_REG_PERIPHID2        0xFE8
#define WD_REG_PERIPHID3        0xFEC
#define WD_REG_PCELLID0         0xFF0
#define WD_REG_PCELLID1         0xFF4
#define WD_REG_PCELLID2         0xFF8
#define WD_REG_PCELLID3         0xFFC


struct sp805_instance
{
    uint32_t base_addr;
    int wd_index;
};



/**
* @}
*/

#endif /* end of include guard: DEVICES_WD_H */
