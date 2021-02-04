/*
 *  ZedBoard / Zynq-7000
 *  UART info could be found in
 *      B.33 UART Controller (UART) / Page 1626 in
 *         Zynq-7000
 *         All Programmable SoC
 *         Technical Reference Manual
 *
 *      http://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf
 */

#include <stdarg.h>
#include <stdint.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <devices/gic.h>
#include <unistd.h>
#include <rumboot/rumboot.h>
#include <rumboot/printf.h>
// Register Description as found in
//    B.33 UART Controller (UART) p.1626

#define XUART_CONTROL_REG0       0
#define XUART_MODE_REG0          4
#define XUART_INTRPT_EN_REG0     8
#define XUART_INTRPT_DIS_REG0    12
#define XUART_INTRPT_MASK_REG0   16
#define XUART_CHNL_INT_STS_REG0  20
#define XUART_BAUD_RATE_GEN      24
#define XUART_RCVR_TIMEOUT_REG0  28
#define XUART_RCVR_FIFO_TRIGGER_LEVEL0       32
#define XUART_MODEM_CTRL_REG0       36
#define XUART_MODEM_STS_REG0       40
#define XUART_CHANNEL_STS_REG0     44
#define XUART_TX_RX_FIFO       48
#define XUART_BAUD_RATE_DIVIDER       52
#define XUART_FLOW_DELAY_REG0       56
#define XUART_TX_FIFO_TRIGGER_LEVEL       6


/*
 *  Page 496
 *  Simplifyed Table 19-1 UART Parameter Value Examples
 *  Parameter Value Examples
 *  Clock                            Baud  BRGR-CD   BDIV-CD                                    Actual Baud Rate
 *  UART Ref clock            600   10417     7
 *  UART Ref clock          9,600     651     7
 *  UART Ref clock     28,800     347     4
 *  UART Ref clock    115,200      62     6
 *  UART Ref clock    230,400      31     6
 */

/*Baudrates assuming input clock speed is 3125000L */
/*Baud_rate_gen_reg0*/
#define XUARTPS_BRGR_CD_115200       62 /*Baud Rate Clock Divisor*/

/*Register Baud_rate_divider_reg0 Details*/
#define XUARTPS_BDIV_CD_115200       6  /*Baud Rate Clock Divisor*/

/* Bits defined in the Register Channel_sts_reg0 */
#define UART_STS_TXFULL 1 << 4 /* Transmitter FIFO Full continuous status:
                                * 0: Tx FIFO is not full
                                * 1: Tx FIFO is full*/

/*Register Control_reg0 BitMask */
#define XUARTPS_CR_STOPBRK (1 << 8)     /* Stop transmitter break */
#define XUARTPS_CR_STTBRK  (1 << 7)     /* Start transmitter break */
#define XUARTPS_CR_RSTTO   (1 << 6)     /* Restart receiver timeout counter */
#define XUARTPS_CR_TXDIS   (1 << 5)     /* Transmit disable */
#define XUARTPS_CR_TXEN    (1 << 4)     /* Transmit enable */
#define XUARTPS_CR_RXDIS   (1 << 3)     /* Receive disable */
#define XUARTPS_CR_RXEN    (1 << 2)     /* Receive enable */
#define XUARTPS_CR_TXRES   (1 << 1)     /* Software reset for Tx data path */
#define XUARTPS_CR_RXRES   (1 << 0)     /* Software reset for Rx data path */

/*Register Control mode_reg0 BitMask*/
#define XUARTPS_MR_PAR                  /*Parity type select [5:3]*/
#define XUARTPS_MR_PAR_EVEN     0       /* 000: even parity
                                         * 001: odd parity
                                         * 010: forced to 0 parity (space)
                                         * 011: forced to 1 parity (mark)*/
#define XUARTPS_MR_PAR_NONE (1 << 5)    /* 1xx: no parity*/
#define XUARTPS_MR_CHRL                 /* Character length select:
                                         *   11: 6 bits
                                         *   10: 7 bits*/
#define XUARTPS_MR_CHRL_8               /*   0x: 8 bits*/

#define XUARTPS_MR_CLKS                 /* Clock source select:*/
#define XUARTPS_MR_CLKS_REF_CLK 0       /*  0: clock source is uart_ref_clk*/


#ifndef CMAKE_BUILD_TYPE_DEBUG
void  rumboot_platform_putchar(uint8_t c)
{
        while ((ioread32(UART1_BASE + XUART_CHANNEL_STS_REG0) & UART_STS_TXFULL) > 0);
        iowrite32(c, UART1_BASE + XUART_TX_RX_FIFO);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
        return 0;
}
#endif

#define SCUTIMER_CONTROL_PRESCALER_MASK	0x0000FF00 /* Prescaler */
#define SCUTIMER_CONTROL_PRESCALER_SHIFT	8
#define SCUTIMER_CONTROL_AUTO_RELOAD_MASK	0x00000002 /* Auto-reload */
#define SCUTIMER_CONTROL_ENABLE_MASK		0x00000001 /* Timer enable */

#define TIMER_LOAD_VAL 0xFFFFFFFF
#define TIMER_PRESCALE 255


#define ZYNQ_SCUTIMER_LOAD     0 /* Timer Load Register */
#define ZYNQ_SCUTIMER_COUNTER  4 /* Timer Counter Register */
#define ZYNQ_SCUTIMER_CONTROL  8 /* Timer Control Register */

#define CONFIG_SYS_HZ 1000

uint32_t rumboot_platform_get_uptime()
{
        return (0xFFFFFFFF - ioread32(ZYNQ_SCUTIMER_BASE + ZYNQ_SCUTIMER_COUNTER));
}

extern char rumboot_heap_start;
extern char rumboot_heap_end;
extern char rumboot_default_irq_vectors;
void rumboot_platform_setup()
{

        /* Reset uptime */
        iowrite32(0xFFFFFFFF, ZYNQ_SCUTIMER_BASE + ZYNQ_SCUTIMER_LOAD);
	arm_vbar_set((uint32_t)&rumboot_default_irq_vectors);
	rumboot_irq_register_gic();
        rumboot_malloc_register_heap("IM0",
                                     &rumboot_heap_start, &rumboot_heap_end);
	rumboot_printf("\n\n");
}
