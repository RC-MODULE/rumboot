#ifndef OI10_IRQ_H
#define OI10_IRQ_H


/**
 * \defgroup platform_oi10_irq_map IRQ Map
 * \ingroup platform_oi10
 *
 * \addtogroup platform_oi10_irq_map
 * @{
 * This file contains mpic128 irq numbers present in SoC.
 * For developers: Please, keep the list sorted by irq number
 * and formatted properly.
 *
 * WARNING: DO NOT ADD ANYTHING ELSE HERE, ONLY IRQ NUMBERS
 */

#define RUMBOOT_PLATFORM_NUM_IRQS 136

/**  */
#define L2C0_INTERRUPTOUT           (0)
#define L2C0_MCHKOUT                (1)
#define PMU0_INTERRUPT              (2)
#define DMA2PLB6_DMA_IRQ_0          (3)
#define DMA2PLB6_DMA_IRQ_1          (4)
#define DMA2PLB6_DMA_IRQ_2          (5)
#define DMA2PLB6_DMA_IRQ_3          (6)
#define DMA2PLB6_SLV_ERR_INT        (7)
#define O_SYSTEM_HUNG               (8)
#define PLB6PLB40_O_0_BR6TO4_INTR   (9)
#define PLB6PLB41_O_BR6TO4_INTR     (10)
#define PLB4XAHB1_INTR              (11)
#define ARB_SYSDCRERR               (12)
#define ITRACE_COMPLETE             (13)
#define LTRACE_COMPLETE_0           (14)
/* 15-31 reserved */
#define GPIO0_INT                   (32)
#define GPIO1_INT                   (33)
#define GSPI0_INT                   (34)
#define GSPI1_INT                   (35)
#define UART0_INT                   (36)
#define UART1_INT                   (37)
#define MKIO0_INT                   (38)
#define MKIO1_INT                   (39)
//#define DIT_INT0                    (40)
//#define DIT_INT1                    (41)
#define WDT_INT                     (42)
#define CRG_INT                     (43)
#define SW0_HSCB_INT                (44)
#define SW0_AXI_INT                 (45)
#define SW1_HSCB_INT                (46)
#define SW1_AXI_INT                 (47)
#define SW2_HSCB_INT                (48)
#define SW2_AXI_INT                 (49)
#define SW3_HSCB_INT                (50)
#define SW3_AXI_INT                 (51)
#define ETH0_INT                    (52)
#define ETH1_INT                    (53)
#define SRAM_INT                    (54)
#define NOR_INT                     (55)
//#define EMI_CNTR_INT                ()
//#define PLB6_INT                    ()
#define MCLFIR_REC_INT              (63)
#define MCLFIR_UNREC_INT            (64)
#define MCLFIR_INFST_INT            (65)
#define DIT_INT0                    (119)
#define DIT_INT1                    (118)


/**
 * @}
 */

#endif /* end of include guard: OI10_IRQ_H */
