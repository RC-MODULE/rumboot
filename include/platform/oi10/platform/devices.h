#ifndef OI10_DEVICES_H
#define OI10_DEVICES_H



/**
 * \defgroup platform_oi10_register_map Register Map
 * \ingroup platform_oi10
 *
 *
 * This file contains physical addresses of devices present in SoC.
 * For developers: Please, keep the list sorted by device address
 * and formatted properly.
 *
 * WARNING: DO NOT ADD INDIVIDUAL REGISTER OFFSETS HERE
 *
 * \defgroup platform_oi10_register_map_virt MMIO registers
 * \ingroup platform_oi10_register_map
 * \defgroup platform_oi10_register_map_dcr DCR registers
 * \ingroup platform_oi10_register_map
 * \addtogroup platform_oi10_register_map_virt
 * @{
 *
 *
 */

#define EM2_BASE                0x00000000
#define EM0_BASE                0x80000000
#define EM1_BASE                0xA0000000
#define IM0_BASE                0xC0010000
#define IM1_BASE                0xD0000000
#define BOOTROM_BASE            0xFFFF0000

/**
 *
 * @}
 * \addtogroup platform_oi10_register_map_dcr
 * @{
 *
 *
 */

#define DCR_PLB6_BC_BASE            0x80000200
#define DCR_PLB6PLB4_0_BASE         0x80000300
/**
 * @}
 */

#endif /* end of include guard: OI10_DEVICES_H */
