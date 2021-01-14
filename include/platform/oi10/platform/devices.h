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
#define EM2_BANK0_BASE          0x00000000
#define SRAM0_BASE              EM2_BANK0_BASE
#define EM2_BANK1_BASE          0x20000000
#define SDRAM_BASE              EM2_BANK1_BASE
#define EM2_BANK2_BASE          0x40000000
#define SSRAM_BASE              EM2_BANK2_BASE
#define EM2_BANK3_BASE          0x50000000
#define PIPELINED_BASE          EM2_BANK3_BASE
#define EM2_BANK4_BASE          0x60000000
#define SRAM1_BASE              EM2_BANK4_BASE
#define EM2_BANK5_BASE          0x70000000
#define NOR_BASE                EM2_BANK5_BASE
#define IM0_BASE                0x80000000
#define IM1_BASE                0x80020000
#define IM2_BASE                0x80040000
#define MKIO0_BASE              0xC0020000
#define GPIO_0_BASE             0xC0028000
#define MKIO1_BASE              0xC0030000
#define GPIO_1_BASE             0xC0038000
#define UART0_BASE              0xC0029000
#define UART1_BASE              0xC0039000
#define GRETH_0_BASE            0xC002A000
#define GRETH_1_BASE            0xC003A000
#define GSPI_0_BASE             0xC002B000
#define SDIO_0_BASE             0xC002C000
#define SCRB_BASE               0xC002D000
#define GSPI_1_BASE             0xC003B000
#define SDIO_1_BASE             0xC003C000
#define LSCB0_BASE              0xC0020000
#define LSCB1_BASE              0xC0030000
#define HSCB0_BASE              0xC0300000
#define HSCB1_BASE              0xC0301000
#define HSCB2_BASE              0xC0302000
#define HSCB3_BASE              0xC0303000
#define BOOTROM_BASE            0xFFFF0000
#define AXI32HSIFS_CTRL_BASE    0xC0100000
#define AXI64HSIFS_CTRL_BASE    0xC0200000

/**
 * @}
 *
 * \addtogroup platform_oi10_register_map_dcr
 * @{
 * This section contains addresses of devices on the DCR bus
 *
 * For developers: Please, keep the list sorted by device address
 * and formatted properly, do not forget DCR_ prefix
 *
 * WARNING: DO NOT ADD INDIVIDUAL REGISTER OFFSETS HERE
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
#define DCR_L2C_BASE            0x80000600
#define DCR_ARB_BASE            0x80000700
#define DCR_ITRACE_BASE         0x80000800
#define DCR_LTRACE_BASE         0x80000900
#define DCR_CLDCR_BASE          0x80000A00
#define DCR_EM2_PLB6MCIF2_BASE  0x80050000
#define DCR_EM2_EMI_BASE        0x80060000
#define DCR_EM2_MCLFIR_BASE     0x80070000
#define DCR_CRG_BASE            0x80080000
#define DCR_SCTL_BASE           0x80090000
#define DCR_TIMERS_BASE         0x800A0000
#define DCR_WATCHDOG_BASE       0x800B0000
#define DCR_EM2_AXIMCIF2_BASE   0x800D0000
#define DCR_MPIC128_BASE        0xFFC00000
/**
 * @}
 */


#endif /* end of include guard: DEVICES_H */
