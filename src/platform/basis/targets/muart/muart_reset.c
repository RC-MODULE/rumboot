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

uint32_t main()
{
    const uint32_t base = UART1_BASE;
    uint32_t reg;

/*
15 dum       MDMA
12 wlen       Длина слова
10 str2      Выбор количества бит
9  SPS       Выбор фиксированной чётности
8  EPS       Выбор чётности
7  PEN       Доступность чётности
6  RTS_POL   Выбор полярности сигнала RTS, режим RS-485
5  CTSen     Устанавливается в 1, если включен режим CTS
4  RTSen     Устанавливается в 1, если включен режим RTS
3  MDS       Устанавливает режим функционирования сигнала RTS
            0 = режим RS-232,  1 = режим RS-485
2  APB_MD    Режим работы с APB
1  LBE       Включение режима петли
0  MEN       MUART enable
*/

  rumboot_printf("Test 1.1 MUART RESET read-write regiters\n");

  rumboot_printf("===== write registers \n");
 set_reg(base, MUART_BDIV, 0xF);
 set_reg(base, MUART_FIFOWM, 0xF00F0);
 set_reg(base, MUART_CTRL, 0x4);
//set_reg(base, MUART_CTRL, 0x1FA;
 //set_reg(base, MUART_CTRL, 0xFA);
 set_reg(base, MUART_MASK, 0xF);
 set_reg(base, MUART_TIMEOUT, 0xF);

  muart_dump(base);

  rumboot_printf("===== RESET \n");
  muart_soft_reset(base);

  rumboot_printf("===== DUMP \n");
  muart_dump(base);

      reg = ioread32(base + MUART_ID);
      if (reg != 0x55415254){
          rumboot_printf("FAIL MUART_ID = %x\n", reg);
          rumboot_printf("expect MUART_ID = %x\n", 0x55415254);
          return -1;
      }
      reg = ioread32(base + MUART_VERSION);
      if (reg != 0x10190){
          rumboot_printf("FAIL MUART_VERSION = %x\n", reg);
          rumboot_printf("expect MUART_VERSION = %x\n", 0x10190);
          return -1;
      }

      reg = ioread32(base + MUART_SW_RST);
      if (reg != 0){
          rumboot_printf("FAIL MUART_SW_RST = %x\n", reg);
          rumboot_printf("expect MUART_SW_RST = 0\n");
          return -1;
      }
      reg = ioread32(base + MUART_GEN_STATUS);
      if (reg != 0){
          rumboot_printf("FAIL MUART_GEN_STATUS = %x\n", reg);
          rumboot_printf("expect MUART_GEN_STATUS = 0\n");
          return -1;
      }

      reg = ioread32(base + MUART_FIFO_STATE);
      if (reg != 0){
          rumboot_printf("FAIL MUART_FIFO_STATE = %x\n", reg);
          rumboot_printf("expect MUART_FIFO_STATE = 0\n");
          return -1;
      }

      reg = ioread32(base + MUART_STATUS);
      if (reg != 0){
          rumboot_printf("FAIL MUART_STATUS = %x\n", reg);
          rumboot_printf("expect MUART_STATUS = 0\n");
          return -1;
      }

      reg = ioread32(base + MUART_DTRANS);
      if (reg != 0){
          rumboot_printf("FAIL MUART_DTRANS = %x\n", reg);
          rumboot_printf("expect MUART_DTRANS = 0\n");
          return -1;
      }


  //    reg = ioread32(base + MUART_DREC);
  //    rumboot_printf("MUART_DREC = %x\n", reg);
      reg = ioread32(base + MUART_BDIV);
      if (reg != 4){
          rumboot_printf("FAIL MUART_BDIV = %x\n", reg);
          rumboot_printf("expect MUART_BDIV = 4\n");
          return -1;
      }

      reg = ioread32(base + MUART_FIFOWM);
      if (reg != 0x2000200){
          rumboot_printf("FAIL MUART_FIFOWM = %x\n", reg);
          rumboot_printf("expect MUART_FIFOWM = %x\n", 0x2000200);
          return -1;
      }

      reg = ioread32(base + MUART_CTRL);
      if (reg != 0){
          rumboot_printf("FAIL MUART_CTRL = %x\n", reg);
          rumboot_printf("expect MUART_CTRL = 0\n");
          return -1;
      }


      reg = ioread32(base + MUART_MASK);
      if (reg != 0){
          rumboot_printf("FAIL MUART_MASK = %x\n", reg);
          rumboot_printf("expect MUART_MASK = 0\n");
          return -1;
      }

      reg = ioread32(base + MUART_TIMEOUT);
      if (reg != 0){
          rumboot_printf("FAIL MUART_TIMEOUT = %x\n", reg);
          rumboot_printf("expect MUART_TIMEOUT = 0\n");
          return -1;
      }


   rumboot_printf("Test 1.1 MUART RESET PASS\n");
   return 0;

}
