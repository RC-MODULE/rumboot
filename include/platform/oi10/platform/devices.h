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
#define IM1_BASE                0x80020000
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

#define DCR_PLB4ARB8M_0_BASE    0x00000010
#define DCR_PLB4ARB8M_1_BASE    0x00000020
#define DCR_SRAMMC2PLB4_0_BASE  0x00000030
#define DCR_SRAMMC2PLB4_1_BASE  0x00000040
#define DCR_PLB4AHB_0_BASE      0x00000050
#define DCR_PLB4ARB8M_2_BASE    0x00000060
#define DCR_DMAPLB6_BASE        0x80000100
#define DCR_PLB6_BC_BASE        0x80000200
#define DCR_PLB6PLB4_0_BASE     0x80000300
#define DCR_PLB6PLB4_1_BASE     0x80000400
#define DCR_PLB4PLB6_0_BASE     0x80000500
#define DCR_L2C_BASE            0x80000600
#define DCR_ARB_BASE            0x80000700
#define DCR_ITRACE_BASE         0x80000800
#define DCR_LTRACE_BASE         0x80000900
#define DCR_CLDCR_BASE          0x80000A00
#define DCR_TIMERS_BASE         0X800A0000
#define DCR_EM2_PLB6MCIF2_BASE  0x80050000
#define DCR_EM2_SRAM_BASE       0x80060000
#define DCR_EM2_MCLFIR_BASE     0x80070000
#define DCR_MPIC128_BASE        0xFFC00000


/**
 * @}
 */

#endif /* end of include guard: DEVICES_H */
