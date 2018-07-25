#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/regs_muart.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <devices/muart.h>
#include <rumboot/rumboot.h>
#include <rumboot/timer.h>
#include <rumboot/testsuite.h>


//     uint32_t base = UART0_BASE;
//     uint32_t base1 = UART1_BASE;
//     struct muart_conf conf;

     uint32_t BASE[] = {UART0_BASE, UART1_BASE};
     char ch[] = {'1','2','3','4','5','6','7','8','9','0', \
                  '1','2','3','4','5','6','7','8','9','0', \
                  '1','2','3','4','5','6','7','8','9','0'};
#define size 30

//#define LIMIT 0x7FF
#define LIMIT 0x700
//#define LIMIT 0x400
//#define LIMIT 0x200

static void set_muart(uint32_t base)
{
    struct muart_conf conf;
    conf.wlen = WL_8;
    conf.stp2 = STP1;
    conf.is_even = 0;
    conf.is_parity = 1;
    conf.mode = RS_232;
    conf.is_loopback = 1;
    conf.baud_rate = 12500000;// bod/s (bdiv=4)
    conf.is_dma = 0;
    muart_soft_reset(base);
    muart_init(base, &conf);

    set_reg(base, MUART_MASK, 0x1FF);
    set_reg(base,MUART_TIMEOUT, 0x16);
    set_reg(base,MUART_TXTIMEOUT, 0x16);
}

static bool test_APB_control_flow(uint32_t uart_N)
{
    uint32_t base = BASE[uart_N];
    int  i, j, w_cnt = 0, is_TTRIS=0;
    int loop = LIMIT/size;
    uint32_t reg, reg_stat=0;

    set_muart(base);
    muart_enable(base);
    muart_dump(base);

    rumboot_printf("\n WRITE ======== loop=%d \n",loop);
    // WRITE ========
    for(i=0; i<loop; i++) {
        for(j=0; j< size; j++){//write
           reg = ioread32(base + MUART_FIFO_STATE);
           while ((reg & 0x7ff0000) >= 0x3FF0000){
               rumboot_printf("write_char fifo state: %x\n",reg);
           }
            iowrite8((uint8_t)ch[j], base + MUART_DTRANS);
           w_cnt++;
           reg = ioread32(base + MUART_STATUS);
           reg_stat |= reg;
           if((reg_stat & (1<<MUART_TTRIS_i)) != 0) {
                 is_TTRIS++;
                 break;
           }
        }
        reg = ioread32(base + MUART_FIFO_STATE);
        rumboot_printf(
            "After %d loop write CNT=%d fifo state=%x stat=%x TTRIS=%x\n",
             i, w_cnt, reg, reg_stat, is_TTRIS);
        if (is_TTRIS != 0) break; //but save reg_stat
        reg_stat=0;
    }
    reg = ioread32(base + MUART_FIFO_STATE);
    if((reg & 0x7FF)>= 0x400){
      if (is_TTRIS != 0){
        rumboot_printf("Mode RS232. Interrupt event of TTRIS has occured.\n");
        rumboot_printf("Data transfer flow control by RTS/CTS is working\n");
        rumboot_printf("TEST PASS \n");
        return true;
      }else{
        rumboot_printf("Mode RS232. Event of TTRIS has occured but\n");
        rumboot_printf("Interrupt TTRIS has not occured.\n");
        rumboot_printf("For last %d writes integrated MUART_STATUS %x\n",
                       j, reg_stat);
        rumboot_printf("TEST FAIL \n");
        return false;
      }
    }else{
       rumboot_printf("Mode RS232. Event of TTRIS has not occured.\n");
       rumboot_printf("MUART_FIFO_STATE=%x Writes count=%d\n",reg,w_cnt);
       rumboot_printf("TEST UNTESTED \n");
       return false;
    }
}
// Declare the testsuite structure
TEST_SUITE_BEGIN(APB_control_flow_test, "APB interface, mode RS-232")
TEST_ENTRY("APB_control_flow_base_0", test_APB_control_flow, 0), //UART0_BASE
TEST_ENTRY("APB_control_flow_base_1", test_APB_control_flow, 1), //UART1_BASE
TEST_SUITE_END();

uint32_t main()
{
    int res = 0;

    rumboot_printf("TEST 1.6: TESTING: Data transfer flow control by " \
                   "RTS/CTS is working.\n");
    rumboot_printf("USING: APB interface, mode RS-232.\n");

    /* Run APB Interface and GIC test suite */
    res = test_suite_run(NULL, &APB_control_flow_test);
    if (res == 0) {
       rumboot_printf("TEST PASS: %d errors happened\n", res);
    }else{
        rumboot_printf("TEST FAIL: %d errors happened\n", res);
    }
    return res;
}
