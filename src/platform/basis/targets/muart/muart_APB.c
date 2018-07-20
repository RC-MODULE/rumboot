#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/timer.h>
#include <rumboot/testsuite.h>
#include <rumboot/rumboot.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <regs/regs_muart.h>
#include <devices/muart.h>
#include <regs/regs_mdma.h>
#include <devices/mdma_chan_api.h>

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


*/
    char ch[] = {'1','2','3','4','5','6','7','8'};
    char *dd=NULL;
    char *src=NULL;
    size_t size = 2;
    int res = 0;
    char wlen_maska[] = {
        0x1F,
        0x3F,
        0x7F,
        0xFF
    };
    uint32_t BASE[] = {UART0_BASE, UART1_BASE};
    int b, e=0, p=0, m, stp=0, w;
    uint32_t reg, cnt_err=0;
    int heap_id=0;

    rumboot_printf("Test 1.1 MUART APB transmit\n");

    if((dd=(char *)rumboot_malloc_from_heap(heap_id,8)) == NULL){
        rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap dd\n");
        return false;
    }
    if((src=(char *)rumboot_malloc_from_heap(heap_id,8)) == NULL){
        rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap src\n");
        rumboot_free(dd);
        return false;
    }

    for ( b=0; b<= 1; b++) {        //base loop
    for ( m=0; m<= RS_422; m++) {   //mode loop
    for ( w=0; w<4; w++) {          //wlen = 5,...,8 bit

        rumboot_printf("TESTING: base=%x stp2=%x wlen=%x mode=%x " \
                       "pan=%x eps=%x\n",
                      BASE[b], stp, w, m, p, e);
        //set right data for current wlen
         for(int i=0; i<size; i++) {
                dd[i] = (ch[i] & wlen_maska[w]);
                src[i] = '\0';
        }
        muart_soft_reset(BASE[b]);
        conf.wlen = w;
        conf.stp2 = stp;
        conf.is_even = e;
        conf.is_parity = p;
        conf.mode = m;
        conf.is_loopback = 1;
        conf.baud_rate = 12500000;  // bod/s (bdiv=4)
        conf.is_dma = 0;

        muart_init(BASE[b], &conf);
        muart_enable(BASE[b]);
        muart_dump(BASE[b]);

       // rumboot_printf("before dd[1]=%x src[1]=%x\n",dd[1],src[1]);
        muart_write_data_apb(BASE[b], dd, size);
        muart_read_data_apb(BASE[b], src, size);
        for(int i=0; i<size; i++) {
            if(dd[i] != src[i]){
                res = 1;
                break;
            }
        }

        if (res != 0) {
           rumboot_printf("TEST FAIL: base=%x stp2=%x wlen=%x mode=%x " \
                          "pan=%x eps=%x\n",
                         BASE[b], stp, w, m, p, e);
           cnt_err++;
        }
        if((reg = ioread32(BASE[b] + MUART_STATUS)) !=0){
            cnt_err++;
            rumboot_printf("TEST FAIL: MUART_STATUS indicates error\n");
            rumboot_printf("MUART_STATUS = %x\n", reg);
        }
        if ((res == 0) & (reg == 0)) {
            rumboot_printf(" PASS\n");
        }
    } //stop bit loop
    if (res != 0) break;
    } //wlen loop
    if (res != 0) break;
    } //base loop

    rumboot_free(dd);
    rumboot_free(src);

    if (cnt_err == 0) {
       rumboot_printf("TEST PASS: %d errors happened\n", cnt_err);
    }else{
        rumboot_printf("TEST FAIL: %d errors happened\n", cnt_err);
    }
    return cnt_err;
}
