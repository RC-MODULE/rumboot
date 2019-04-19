#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <rumboot/irq.h>

#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/interrupts.h>
#include <regs/regs_gpio.h>

#define LINES_AMOUNT 8
#define GPIO_AMOUNT  6
#define TIMEOUT 1000
#define DEBUG 1

volatile uint32_t irq_set;
volatile uint32_t status [GPIO_AMOUNT];
volatile uint8_t lines [GPIO_AMOUNT] [LINES_AMOUNT];

void GPIO_config (uint32_t base_gpio_addr, uint8_t direction, uint8_t source, uint32_t mask) {

  uint32_t gpio_intr;

    switch (base_gpio_addr) {
    case GPIO1_BASE: {
            gpio_intr = GPIO1_INTR;
            // return gpio_intr;
            break;
    };
    case GPIO2_BASE: {
            gpio_intr = GPIO2_INTR;
            // return gpio_intr;
            break;
    };
    case GPIO3_BASE: {
            gpio_intr = GPIO3_INTR;
            // return gpio_intr;
            break;
    };
    case GPIO4_BASE: {
            gpio_intr = GPIO4_INTR;
            // return gpio_intr;
            break;
    };
    case GPIO5_BASE: {
            gpio_intr = MDIO_GPIO0_INT;
            // return gpio_intr;
            break;
    };
    case GPIO6_BASE: {
            gpio_intr = MDIO_GPIO0_INT;
            // return gpio_intr;
            break;
    };

    default: {
      rumboot_printf ("Wrong Base Address! ERROR!");
      return 1;
      break;
    }
  }

  ioread32 (base_gpio_addr + GPIO_STATUS);
  iowrite32 (direction, base_gpio_addr + GPIO_DIRECTION); 
  iowrite32 (source, base_gpio_addr + GPIO_SOURCE);
  iowrite32 (mask, base_gpio_addr + GPIO_MASK);
  rumboot_irq_enable (gpio_intr);
  #ifdef DEBUG
  rumboot_printf ("Direction = %x\n", direction);
  #endif

}

void GPIO_handler (uint32_t gpio_intr)
{ 
  uint32_t mask;
  uint32_t gpio_number;
  uint32_t base_gpio_addr;
  uint8_t irq;
  // rumboot_printf ("IRQ handler!");
  irq_set = 0x1;

    switch (gpio_intr) {
    case GPIO1_INTR: {
            base_gpio_addr = GPIO1_BASE;
            gpio_number = 0;
            break;
    };
    case GPIO2_INTR: {
            base_gpio_addr = GPIO2_BASE;
            gpio_number = 1;
            break;
    };
    case GPIO3_INTR: {
            base_gpio_addr = GPIO3_BASE;
            gpio_number = 2;
            break;
    };
    case GPIO4_INTR: {
            base_gpio_addr = GPIO4_BASE;
            gpio_number = 3;
            break;
    };
    case MDIO_GPIO0_INT: {
            base_gpio_addr = GPIO5_BASE;
            gpio_number = 4;
            break;
    };
    case MDIO_GPIO1_INT: {
            base_gpio_addr = GPIO6_BASE;
            gpio_number = 5;
            break;
    };

    default: {
      rumboot_printf ("Wrong Base Address! ERROR!");
      return 1;
      break;
    }
  }
  status [gpio_number] = ioread32 (base_gpio_addr + GPIO_STATUS);
  rumboot_printf ("status [%d] = %x\n", gpio_number, status [gpio_number]);
  mask = ioread32 (base_gpio_addr + GPIO_MASK); 



  for (int i = 0; i < LINES_AMOUNT; ++i) {
    lines [gpio_number] [i] = (status [gpio_number] & (0x3 << 4*i)) >> 4*i;
    if ((lines [gpio_number] [i] ) != 0x0) {
      rumboot_printf ("IRQ from GPIO Address %x on line %d\n", base_gpio_addr, i);
      irq = irq + lines [gpio_number] [i];
    }
  }
  if (irq == 0) rumboot_printf ("No IRQ from GPIO %d\n", gpio_number);

  

}

void GPIO_out (uint32_t base_gpio_addr, uint8_t tx_data) {

  iowrite32 (tx_data, base_gpio_addr + GPIO_WRITE_APBtoPAD);

  #ifdef DEBUG
  uint32_t data;
  data = ioread32 (base_gpio_addr + GPIO_WRITE_APBtoPAD);
  rumboot_printf ("tx_data = x%x \n", data);
  #endif
}

int GPIO_in (uint32_t base_gpio_addr, uint8_t rx_data_prev) {

  uint32_t rx_data;
  uint32_t tx_data;
  uint32_t count;

  count = 0;

  rx_data = ioread32 (base_gpio_addr + GPIO_READ_PADtoAPB);
  rumboot_printf ("rx_data = %x\n", rx_data);

  if (rx_data != rx_data_prev) {

  while (irq_set == 0x0)
    {
      count = count + 1;
      if (count >= TIMEOUT)
      {
       rumboot_printf ("IRQ TIMEOUT!\n");
       return 1;
      }
    }
  }

  return rx_data;



}

int GPIO_check (uint32_t base_gpio0_addr, uint32_t base_gpio1_addr, uint32_t number0, uint32_t number1, uint8_t tx_data, uint8_t rx_data, uint8_t rx_data_prev) {


  uint32_t apb_to_pad;
  uint32_t gpio_number;
  uint32_t rx_data_b [LINES_AMOUNT];
  uint32_t rx_data_prev_b [LINES_AMOUNT];
  uint32_t tx_data_b [LINES_AMOUNT];
  uint32_t tx_data_prev_b [LINES_AMOUNT];
  uint32_t status_b [32];


  #ifdef DEBUG
  apb_to_pad = ioread32 (base_gpio0_addr + GPIO_WRITE_APBtoPAD);
   if (apb_to_pad != tx_data)

   {rumboot_printf("ERROR!\n");
      return 1;
    }
  #endif

    switch (base_gpio1_addr) {
    case GPIO1_BASE: {
            gpio_number = 0;
            break;
    };
    case GPIO2_BASE: {
            gpio_number = 1;
            break;
    };
    case GPIO3_BASE: {
            gpio_number = 2;
            break;
    };
    case GPIO4_BASE: {
            gpio_number = 3;
            break;
    };
    case GPIO5_BASE: {
            gpio_number = 4;
            break;
    };
    case GPIO6_BASE: {
            gpio_number = 5;
            break;
    };

    default: {
      rumboot_printf ("Wrong Base Address! ERROR!");
      return 1;
      break;
    }
  }

  for (int i = 0; i < LINES_AMOUNT; ++i)
  {
    rx_data_b [i] = 0;
    tx_data_b [i] = 0;
    rx_data_prev_b [i] = 0;
  }
  #ifdef DEBUG
  rumboot_printf ("------------CHECK--------------\n");
  rumboot_printf ("rx_data = %x\n", rx_data);
  #endif
  switch (number1) {
    case 0: {
      rx_data_b [0] = rx_data & 0x1;
      rx_data_prev_b [0] = rx_data_prev & 0x1;
      break;
    };
    case 1: {
      rx_data_b [1] = (rx_data & 0x2) >> 1;
      rx_data_prev_b [1] = (rx_data_prev & 0x2) >> 1;
      break;
    };
    case 2: {
      rx_data_b [2] = (rx_data & 0x4) >> 2;
      rx_data_prev_b [2] = (rx_data_prev & 0x4) >> 2;
      break;
    };
    case 3: {
      rx_data_b [3] = (rx_data & 0x8) >> 3;
      rx_data_prev_b [3] = (rx_data_prev & 0x8) >> 3;
      break;
    };
    case 4: {
      rx_data_b [4] = (rx_data & 0x10) >> 4;
      rx_data_prev_b [4] = (rx_data_prev & 0x10) >> 4;
      break;
    };
    case 5: {
      rx_data_b [5] = (rx_data & 0x20) >> 5;
      rx_data_prev_b [5] = (rx_data_prev & 0x20) >> 5;
      break;
    };
    case 6: {
      rx_data_b [6] = (rx_data & 0x40) >> 6;
      rx_data_prev_b [6] = (rx_data_prev & 0x40) >> 6;
      break;
    };
    case 7: {
      rx_data_b [7] = (rx_data & 0x80) >> 7;
      rx_data_prev_b [7] = (rx_data_prev & 0x80) >> 7;
      break;
    };
    default : {
      rumboot_printf ("Wrong rx line number! ERROR!\n");
      return 1;
    };
  }
    switch (number0) {
    case 0: {
      tx_data_b [0] =  tx_data & 0x1;
      break;
    };
    case 1: {
      tx_data_b [1] = (tx_data & 0x2) >> 1;
      break;
    };
    case 2: {
      tx_data_b [2] = (tx_data & 0x4) >> 2;
      break;
    };
    case 3: {
      tx_data_b [3] = (tx_data & 0x8) >> 3;
      break;
    };
    case 4: {
      tx_data_b [4] = (tx_data & 0x10) >> 4;
      break;
    };
    case 5: {
      tx_data_b [5] = (tx_data & 0x20) >> 5;
      break;
    };
    case 6: {
      tx_data_b [6] = (tx_data & 0x40) >> 6;
      break;
    };
    case 7: {
      tx_data_b [7] = (tx_data & 0x80) >> 7;
      break;
    };
    default : {
      rumboot_printf ("Wrong tx line number! ERROR!\n");
      return 1;
    };
  }

      status_b [4*number1] = (status [gpio_number] & (0x1 << (4*number1))) >> 4*number1;
      status_b [4*number1 + 1] = (status [gpio_number] & (0x1 << (4*number1 + 1))) >> (4*number1 +1);
      if (rx_data_prev_b [number1] != rx_data_b [number1]) {
        if (!(status_b [4*number1] | status_b [4*number1 + 1])) {

          rumboot_printf ("Incorrect Status! ERROR!\n");
          return 1;
        }
      }
      rumboot_printf ("Correct Status! OK!\n");

      if (rx_data_b [number1] != tx_data_b [number0]) {
        rumboot_printf ("Wrong data received! ERROR!\n Expected %x on line %d\n Received %x\n", tx_data_b [number0], number1, rx_data_b [number1]);
        return 1;
      }
      else {
        rumboot_printf ("Correct Data! OK!\n");
        return 0;
      }



}




int GPIO_test (uint32_t base_gpio0_addr, uint32_t base_gpio1_addr, uint8_t direction0, uint8_t direction1, uint8_t connection [], 
  uint8_t source0, uint8_t source1, uint32_t mask0, uint32_t mask1, uint8_t tx_data)
{   
  uint32_t count;
  uint32_t number0;
  uint32_t number1;

  uint32_t rx_data_prev;
  uint32_t rx_data;
  uint32_t test_status;


  irq_set = 0x0;
  count = 0x0;
  test_status = 0x0;

  rumboot_printf ("--------------------------GPIO %x test-------------------------------\n", base_gpio0_addr);
  if (base_gpio0_addr != base_gpio1_addr) {

    GPIO_config (base_gpio0_addr, direction0, source0, mask0);
    GPIO_config (base_gpio1_addr, direction1, source1, mask1);

  }
  else GPIO_config (base_gpio0_addr, direction0, source0, mask0);


    rx_data_prev = ioread32 (base_gpio1_addr + GPIO_READ_PADtoAPB);

    GPIO_out (base_gpio0_addr, tx_data);
    rx_data = GPIO_in (base_gpio1_addr, rx_data_prev);


    for (int i = 0; i < LINES_AMOUNT; ++i)
    {
      if (connection [i] != 0xf) {

        number0 = i;
        number1 = connection [i];
        test_status = test_status | GPIO_check (base_gpio0_addr, base_gpio1_addr, number0, number1, tx_data, rx_data, rx_data_prev);
        #ifdef DEBUG
        rumboot_printf ("connection [%d] = %d\n number0 = %d\n number1 = %d\n test_status = %d\n", i, connection [i], number0, number1, test_status);
        #endif
        
      }
    }


    return test_status;
    

  }

  
      

//


int main()
{
  rumboot_irq_cli();
  struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

  rumboot_irq_set_handler(tbl, GPIO1_INTR    , 0, GPIO_handler, NULL);
  rumboot_irq_set_handler(tbl, GPIO2_INTR    , 0, GPIO_handler, NULL);
  rumboot_irq_set_handler(tbl, GPIO3_INTR    , 0, GPIO_handler, NULL);
  rumboot_irq_set_handler(tbl, GPIO4_INTR    , 0, GPIO_handler, NULL);
  rumboot_irq_set_handler(tbl, MDIO_GPIO0_INT, 0, GPIO_handler, NULL);
  rumboot_irq_set_handler(tbl, MDIO_GPIO1_INT, 0, GPIO_handler, NULL);
  /* Activate the table */
  rumboot_irq_table_activate(tbl);
  rumboot_irq_sei();

  uint8_t connection1 [LINES_AMOUNT];
  uint8_t connection2 [LINES_AMOUNT];
  connection1 [0] = 4;
  connection1 [1] = 0xf;
  connection1 [2] = 6;
  connection1 [3] = 0xf;
  connection1 [4] = 0xf;
  connection1 [5] = 0xf;
  connection1 [6] = 0xf;
  connection1 [7] = 7;

  connection2 [0] = 4   ;
  connection2 [1] = 5   ;
  connection2 [2] = 6   ;
  connection2 [3] = 7   ;
  connection2 [4] = 0xf ;
  connection2 [5] = 0xf ;
  connection2 [6] = 0xf ;
  connection2 [7] = 0xf ;




  if (
      !GPIO_test (GPIO1_BASE, GPIO2_BASE, 0x85, 0x2f, connection1, 0xff, 0xff, 0x33333333, 0x33333333, 0x85) &
      !GPIO_test (GPIO3_BASE, GPIO3_BASE, 0xf0, 0xf0, connection2, 0xff, 0xff, 0x33333333, 0x33333333, 0xf0)

     )
  {
    rumboot_printf ("Test passed! OK!\n");
    return 0;
  }
  else {
    rumboot_printf ("Test Failed!\n");
    return 1;
  }
}

