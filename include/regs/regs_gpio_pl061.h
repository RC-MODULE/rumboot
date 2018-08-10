#ifndef REGS_GPIO_PL061_H
#define REGS_GPIO_PL061_H


/**
 * \defgroup regs_gpio-pl061 GPIO PL061
 * \ingroup regs
 *
 * Register offsets and constants
 * \code{.c}
 * #include <regs/regs_gpio_pl061.h>
 * \endcode
 *
 * @{
 */
#define GPIO_DATA                    0x000
#define GPIO_DIR                     0x400
#define GPIO_IS                      0x404
#define GPIO_IBE                     0x408
#define GPIO_IEV                     0x40C
#define GPIO_IE                      0x410
#define GPIO_RIS                     0x414
#define GPIO_MIS                     0x418
#define GPIO_IC                      0x41C
#define GPIO_AFSEL                   0x420
#define GPIO_PeriphID0               0xFE0
#define GPIO_PeriphID1               0xFE4
#define GPIO_PeriphID2               0xFE8
#define GPIO_PeriphID3               0xFEC
#define GPIO_CellID0                 0xFF0
#define GPIO_CellID1                 0xFF4
#define GPIO_CellID2                 0xFF8
#define GPIO_CellID3                 0xFFC
/**
 * @}
 */

//default values
#define GPIO_DATA_DEFAULT            0x00
#define GPIO_DIR_DEFAULT             0x00
#define GPIO_IS_DEFAULT              0x00
#define GPIO_IBE_DEFAULT             0x00
#define GPIO_IEV_DEFAULT             0x00
#define GPIO_IE_DEFAULT              0x00
#define GPIO_RIS_DEFAULT             0x00
#define GPIO_MIS_DEFAULT             0x00
#define GPIO_IC_DEFAULT              0x00
#define GPIO_AFSEL_DEFAULT           0x00
#define GPIO_PeriphID0_DEFAULT       0x61
#define GPIO_PeriphID1_DEFAULT       0x10
#define GPIO_PeriphID2_DEFAULT       0x04
#define GPIO_PeriphID3_DEFAULT       0x00
#define GPIO_CellID0_DEFAULT         0x0D
#define GPIO_CellID1_DEFAULT         0xF0
#define GPIO_CellID2_DEFAULT         0x05
#define GPIO_CellID3_DEFAULT         0xB1
// masks
#define GPIO_REG_MASK                0xFF
#define GPIO_ADDR_MASK               (GPIO_REG_MASK << 2)


#endif /* REGS_GPIO_PL061_H */
