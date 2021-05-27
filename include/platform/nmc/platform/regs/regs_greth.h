#ifndef REGS_GRETH_H
#define REGS_GRETH_H

/**
 * \defgroup regs_greth Gaisler GRETH
 * \ingroup regs
 *
 * Register offsets and constants
 * \code{.c}
 * #include <regs/regs_greth.h>
 * \endcode
 *
 * @{
 */

#define CTRL                                0x00
#define STATUS                              0x04
#define MAC_MSB                             0x08
#define MAC_LSB                             0x0C
#define MDIO_CTRL                           0x10
#define TRANSMIT_DESCR_PTR                  0x14
#define RECEIVER_DESCR_PTR                  0x18
#define EDCL_IP                             0x1C
#define HASH_TABLE_MSB                      0x20
#define HASH_TABLE_LSB                      0x24
#define EDCL_MAC_MSB                        0x28
#define EDCL_MAC_LSB                        0x2C

#define TRANSMIT_BUF_DEBUG_ACC_START        0x10000
#define TRANSMIT_BUF_DEBUG_ACC_FINISH       0x107FC
#define RECEIVER_BUF_DEBUG_ACC_START        0x20000
#define RECEIVER_BUF_DEBUG_ACC_FINISH       0x207FC
#define EDCL_BUF_DEBUG_ACC_START            0x30000
#define EDCL_BUF_DEBUG_ACC_FINISH           0x3FFFC


#define GRETH_CTRL_MASK                               0xFE007FFF

#define GRETH_MDIO_CTRL_MASK                          0xFFFFFFDF

#define GRETH_EDCL_IP_MASK                            0xFFFFFFFF

#define GRETH_EDCL_MAC_MSB_MASK                       0x0000FFFF
#define GRETH_EDCL_MAC_LSB_MASK                       0xFFFFFFFF

#define GRETH_CTRL_DEFAULT                            0x9A000090

#define GRETH_MDIO_CTRL_DEFAULT                       0x01E10140

#define GRETH0_EDCL_IP_DEFAULT                         0xc0a80321
#define GRETH1_EDCL_IP_DEFAULT                         0xc0a80322

#define GRETH0_EDCL_MAC_MSB_DEFAULT                    0x0000EC17
#define GRETH0_EDCL_MAC_LSB_DEFAULT                    0x66edc101

#define GRETH1_EDCL_MAC_MSB_DEFAULT                    0x0000EC17
#define GRETH1_EDCL_MAC_LSB_DEFAULT                    0x66edc102




/**
* @}
*/

 /**
  *
  * \defgroup greth_bit_positions Bit_positions
  * \ingroup greth
  *
  * \addtogroup greth_bit_positions
  * @{
  */

/**
 * CTRL register BITS and fields
 */
#define GRETH_CTRL_EA  31
#define GRETH_CTRL_GA  27
#define GRETH_CTRL_ME  26
#define GRETH_CTRL_MC  25
//24:15 reserved
#define GRETH_CTRL_ED  14
#define GRETH_CTRL_RD  13
#define GRETH_CTRL_DD  12
#define GRETH_CTRL_MCE 11
#define GRETH_CTRL_PI  10
//9:8 reserved
#define GRETH_CTRL_SP  7
#define GRETH_CTRL_RS  6
#define GRETH_CTRL_PM  5
#define GRETH_CTRL_FD  4
#define GRETH_CTRL_RI  3
#define GRETH_CTRL_TI  2
#define GRETH_CTRL_RE  1
#define GRETH_CTRL_TE  0

/**
 * Status register BITS
 */
#define GRETH_STATUS_PS  8
#define GRETH_STATUS_IA  7
#define GRETH_STATUS_TS  6
#define GRETH_STATUS_TA  5
#define GRETH_STATUS_RA  4
#define GRETH_STATUS_TI  3
#define GRETH_STATUS_RI  2
#define GRETH_STATUS_TE  1
#define GRETH_STATUS_RE  0


/**
 * MDIO control/status BITS
 */
#define GRETH_MDIO_WR  0
#define GRETH_MDIO_RD  1
#define GRETH_MDIO_LF  2
#define GRETH_MDIO_BU  3
#define GRETH_MDIO_NV  4
//5:RESERVED
#define GRETH_MDIO_REGADDR  6
#define GRETH_MDIO_PHYADDR  11
#define GRETH_MDIO_DATA     16

/**
 * Masks for read/write data by MDIO interface
 */
#define GRETH_MDIO_REGADDR_MASK  ((uint32_t)(0b11111 << GRETH_MDIO_REGADDR))
#define GRETH_MDIO_PHYADDR_MASK  ((uint32_t)(0b11111 << GRETH_MDIO_PHYADDR))
#define GRETH_MDIO_DATA_MASK     ((uint32_t)0xFFFF << GRETH_MDIO_DATA)

/**
* @}
*/











#endif /* end of include guard: REGS_GRETH_H */
