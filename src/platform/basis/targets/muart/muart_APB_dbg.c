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

uint32_t main()
{
//     uint32_t base = UART0_BASE;
//     uint32_t base1 = UART1_BASE;
     struct muart_conf conf;
/*
  dum       MDMA
  wlen =     Длина слова
  str2      Выбор количества бит
  SPS       Выбор фиксированной чётности
  EPS       Выбор чётности
  PEN       Доступность чётности
  RTS_POL   Выбор полярности сигнала RTS, режим RS-485
  CTSen     Устанавливается в 1, если включен режим CTS
  RTSen     Устанавливается в 1, если включен режим RTS
  MDS       Устанавливает режим функционирования сигнала RTS
            0 = режим RS-232,  1 = режим RS-485
  APB_MD    Режим работы с APB
  LBE       Включение режима петли
  MEN       MUART enable


*/  //2047 7FF
     char ch[] = {'1','2','3','4','5','6','7','8','9','0', \
                  '1','2','3','4','5','6','7','8','9','0', \
                  '1','2','3','4','5','6','7','8','9','0'};
    size_t size = 30;
 //   int res = 0;
    char wlen_maska[] = {
        0x1F,
        0x3F,
        0x7F,
        0xFF
    };
    uint32_t BASE[] = {UART0_BASE, UART1_BASE};
    int  b = 1;
    int  e = 0;
    int  p = 1;
    int  m = RS_232;
//    int  m = RS_485;
    int  stp = 0;
    int  w = 3;

    char dd[30];
    int  i, j, w_cnt = 0, is_TTRIS=0;
    int loop;
//#define LIMIT 0x7FF
#define LIMIT 0x600
//#define LIMIT 0x400
//#define LIMIT 0x200
    uint32_t reg, reg_stat=0;

    rumboot_printf("TEST 1.6: Check the MUART connection with " \
                   "the external chip pins. \n");
    rumboot_printf("TESTING: Data transfer flow control by " \
                   "RTS/CTS is working.\n");
    rumboot_printf("USING: APB interface, mode RS-232.\n");
    //set right data for current wlen
    for(i=0; i<size; i++) {
        dd[i] = (ch[i] & wlen_maska[w]);
    }
    loop = LIMIT/size;

    muart_soft_reset(BASE[b]);
    conf.wlen = w;
    conf.stp2 = stp;
    conf.is_even = e;
    conf.is_parity = p;
    conf.mode = m;
    conf.is_loopback = 1;
    conf.baud_rate = 12500000,// bod/s (bdiv=4)
    conf.is_dma = 0;
    muart_init(BASE[b], &conf);

    set_reg(BASE[b], MUART_MASK, 0x1FF);
 //   set_reg(BASE[b], MUART_BDIV, 0x1);
    set_reg(BASE[b],MUART_TIMEOUT, 0x4);

    muart_enable(BASE[b]);
    muart_dump(BASE[b]);
    rumboot_printf("\n WRITE ======== loop=%d \n",loop);
    // WRITE ========
    for(int i=0; i<loop; i++) {
        for(j=0; j<size; j++){//write
           reg = ioread32(BASE[b] + MUART_FIFO_STATE);
           while ((reg & 0x7ff0000) >= 0x3FF0000){
               rumboot_printf("write_char fifo state: %x\n",reg);
           }
            iowrite8((uint8_t)dd[j], BASE[b] + MUART_DTRANS);
           w_cnt++;
           reg = ioread32(BASE[b] + MUART_STATUS);
           reg_stat |= reg;
           if((reg_stat & (1<<MUART_TTRIS_i)) != 0) {
                 is_TTRIS++;
                 break;
           }
        }
        reg = ioread32(BASE[b] + MUART_FIFO_STATE);
        rumboot_printf(
            "After %d loop write CNT=%d fifo state=%x stat=%x TTRIS=%x\n",
             i, w_cnt, reg, reg_stat, is_TTRIS);
        if (is_TTRIS != 0) break; //but save reg_stat
        reg_stat=0;
    }
    reg = ioread32(BASE[b] + MUART_FIFO_STATE);
    if((reg & 0x7FF)>= 0x400){
      if (is_TTRIS != 0){
        rumboot_printf("Mode RS232. Interrupt event of TTRIS has occured.\n");
        rumboot_printf("Data transfer flow control by RTS/CTS is working\n");
        rumboot_printf("TEST PASS \n");
        return 0;
      }else{
        rumboot_printf("Mode RS232. Event of TTRIS has occured but\n");
        rumboot_printf("Interrupt TTRIS has not occured.\n");
        rumboot_printf("For last %d writes integrated MUART_STATUS %x\n",
                       j, reg_stat);
        rumboot_printf("TEST FAIL \n");
        return 1;
      }
    }else{
       rumboot_printf("Mode RS232. Event of TTRIS has not occured.\n");
       rumboot_printf("MUART_FIFO_STATE=%x Writes count=%d\n",reg,w_cnt);
       rumboot_printf("TEST UNTESTED \n");
       return 2;
    }
}
