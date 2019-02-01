
#ifndef DEVICES_H_
#define DEVICES_H_

#define AMB1_Limit   0x00080000
#define AMB0_Mirror_Base 0x40000000

#define PERIPH_Base  0x40080000

// APBS0 bridge
#define GRETH_Base      (PERIPH_Base+0x00000000)
#define GPIOC_Base      (PERIPH_Base+0x00001000)
#define GPIOD_Base      (PERIPH_Base+0x00002000)
#define GPIOE_Base      (PERIPH_Base+0x00003000)
#define SPI0_Base       (PERIPH_Base+0x00004000)
#define SPI1_Base       (PERIPH_Base+0x00005000)
#define SPI2_Base       (PERIPH_Base+0x00006000)
#define SPI3_Base       (PERIPH_Base+0x00007000)
#define SPI4_Base       (PERIPH_Base+0x00008000)
#define WDT_Base        (PERIPH_Base+0x00009000)
#define CRG_DAC_Base    (PERIPH_Base+0x0000A000)
// APBS1 bridge
#define GPIOA_Base      (PERIPH_Base+0x00010000)
#define GPIOB_Base      (PERIPH_Base+0x00011000)
#define UART0_Base      (PERIPH_Base+0x00012000)
#define UART1_Base      (PERIPH_Base+0x00013000)
#define DIT_Base        (PERIPH_Base+0x00014000)
#define SCTL_Base       (PERIPH_Base+0x00015000)
#define CRG_SYS_Base    (PERIPH_Base+0x00016000)
#define CRG_ADC_Base    (PERIPH_Base+0x00017000)
#define EMI_Base        (PERIPH_Base+0x00018000)

//#define TSNS_Base       (PERIPH_Base+0x00016000)

#define APBIC_TOP_Base  (PERIPH_Base+0x00020000)
#define TMC_ETF_Base    (PERIPH_Base+0x00021000)
#define TMC_ETR_Base    (PERIPH_Base+0x00022000)
#define CTI1_Base       (PERIPH_Base+0x00023000)
#define ATB_REPL_Base   (PERIPH_Base+0x00024000)
#define TPIU_Base       (PERIPH_Base+0x00025000)
#define TSNGEN_Base     (PERIPH_Base+0x00026000)
#define NM0DBG_Base     (PERIPH_Base+0x00027000)
#define NM1DBG_Base     (PERIPH_Base+0x00028000)
#define APBIC_CPU_Base  (PERIPH_Base+0x00030000)
#define ETM_Base        (PERIPH_Base+0x00031000)
#define DEBUG_REG_Base  (PERIPH_Base+0x00032000)
#define PERF_MON_Base   (PERIPH_Base+0x00033000)
#define CTI0_Base       (PERIPH_Base+0x00034000)

#define GICC_Base       (PERIPH_Base+0x00040000)
#define GICD_Base       (PERIPH_Base+0x00042000)
#define MDMAC_Base      (PERIPH_Base+0x00044000)

#define TUBE_Base       (DIT_Base+0x800)

#define DDC0_Base       0x00300000
#define DDC1_Base       0x00310000

#define ITU_Base        0x00320000

#define CORB_Base       0x00340000

#define TRNSMT_Base     0x003C0000


#define GIC_DIST_BASE GICD_Base
#define GIC_CPUIF_BASE GICC_Base
#define EMI_MEM_Base    0x80000000

#endif
