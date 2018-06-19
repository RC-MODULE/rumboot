#ifndef DEVICES_H
#define DEVICES_H



/**
 * \defgroup platform_oi10_register_map Register Map
 * \ingroup platform_oi10
 *
 * \defgroup platform_oi10_register_map_virt MMIO registers
 * \ingroup platform_oi10_register_map
 * \defgroup platform_oi10_register_map_dcr DCR registers
 * \ingroup platform_oi10_register_map
 *
 * \addtogroup platform_oi10_register_map_virt
 * @{
 *
 * This section contains PPC CPU addresses of devices present in SoC
 * as configured by bootrom TLB.
 *
 * For developers: Please, keep the list sorted by device address
 * and formatted properly.
 *
 * WARNING: DO NOT ADD INDIVIDUAL REGISTER OFFSETS HERE
 */

#define EM2_BASE                0x00000000
#define IM0_BASE                0x80000000
#define IM1_BASE                0xС0000000
#define BOOTROM_BASE            0xFFFF0000

/**
 *
 * @}
 * \addtogroup platform_oi10_register_map_dcr
 * @{
 * This section contains addresses of devices on the DCR bus
 *
 * For developers: Please, keep the list sorted by device address
 * and formatted properly, do not forget DCR_ prefix
 *
 * WARNING: DO NOT ADD INDIVIDUAL REGISTER OFFSETS HERE
 *
 *
 */

#define DCR_PLB6_BC_BASE            0x80000200
#define DCR_PLB6PLB4_0_BASE         0x80000300
/**
 * @}
 */

#endif /* end of include guard: DEVICES_H */
