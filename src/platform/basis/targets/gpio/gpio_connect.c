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
#define IRQ_TIMEOUT 1000
#define STATION_TIMEOUT 0
// #define DEBUG 1
#define VECTOR0 0x0
#define VECTOR1 0x55
#define VECTOR2 0xAA
#define FLAG 0x40


volatile uint32_t irq_set [GPIO_AMOUNT];
volatile uint32_t status [GPIO_AMOUNT];
volatile uint8_t lines [GPIO_AMOUNT] [LINES_AMOUNT];
uint32_t rx_data_prev [GPIO_AMOUNT];

void GPIO_config (uint32_t base_gpio_addr, uint8_t direction) {

  uint32_t gpio_intr;

    switch (base_gpio_addr) {
    case GPIO1_BASE: {
            gpio_intr = GPIO1_INTR;
            break;
    };
    case GPIO2_BASE: {
            gpio_intr = GPIO2_INTR;
            break;
    };
    case GPIO3_BASE: {
            gpio_intr = GPIO3_INTR;
            break;
    };
    case GPIO4_BASE: {
            gpio_intr = GPIO4_INTR;
            break;
    };
    case MGPIO0_BASE: {
            gpio_intr = MDIO_GPIO0_INT;
            break;
    };
    case MGPIO1_BASE: {
            gpio_intr = MDIO_GPIO1_INT;
            break;
    };

    default: {
      rumboot_printf ("Wrong Base Address! ERROR! %x\n", base_gpio_addr);
      return 1;
      break;
    }
  }

  ioread32 (base_gpio_addr + GPIO_STATUS);
  iowrite32 (direction, base_gpio_addr + GPIO_DIRECTION); 
  iowrite32 (0x33333333, base_gpio_addr + GPIO_MASK);
  rumboot_irq_enable (gpio_intr);

}

void GPIO_handler (uint32_t gpio_intr)
{ 
  uint32_t mask;
  uint32_t gpio_number;
  uint32_t base_gpio_addr;
  uint8_t irq;

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
            base_gpio_addr = MGPIO0_BASE;
            gpio_number = 4;
            break;
    };
    case MDIO_GPIO1_INT: {
            base_gpio_addr = MGPIO1_BASE;
            gpio_number = 5;
            break;
    };

    default: {
      rumboot_printf ("Wrong Base Address! ERROR! %x\n", base_gpio_addr);
      return 1;
      break;
    }
  }
  irq_set [gpio_number] = 1;
  status [gpio_number] = ioread32 (base_gpio_addr + GPIO_STATUS);
  #ifdef DEBUG
  rumboot_printf ("status [%d] = %x\n", gpio_number, status [gpio_number]);

  #endif

}

void GPIO_out (uint32_t base_gpio_addr, uint8_t tx_data) {

  iowrite32 (tx_data, base_gpio_addr + GPIO_WRITE_APBtoPAD);

  #ifdef DEBUG
  uint32_t data;
  data = ioread32 (base_gpio_addr + GPIO_WRITE_APBtoPAD);
  rumboot_printf ("tx_data = x%x \n", data);
  #endif
}

int GPIO_in (uint32_t base_gpio_addr, uint8_t tx_data) {

  uint32_t rx_data;
  uint32_t count;
  uint32_t gpio_number;
  // uint32_t irq;

  count = 0;
  switch (base_gpio_addr) {
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
    case MGPIO0_BASE: {
            gpio_number = 4;
            break;
    };
    case MGPIO1_BASE: {
            gpio_number = 5;
            break;
    };

    default: {
      rumboot_printf ("Wrong Base Address! ERROR!");
      return 1;
      break;
    }
  }

  #ifdef DEBUG
  rumboot_printf ("irq_set [%d] = %d\n", gpio_number, irq_set [gpio_number]);
  rumboot_printf ("rx_data_prev [%d] = %x\n", gpio_number, rx_data_prev [gpio_number]);
  #endif

  if (tx_data != rx_data_prev [gpio_number]) {
    while (irq_set [gpio_number] == 0x0)
    {
      count = count + 1;
      if (count >= IRQ_TIMEOUT)
      {
       rumboot_printf ("IRQ TIMEOUT!\n");
       return 1;
      }
    }
  }
  rx_data = ioread32 (base_gpio_addr + GPIO_READ_PADtoAPB);
  
  #ifdef DEBUG
  rumboot_printf ("rx_data = %x\n", rx_data);
  #endif

  return rx_data;

}

int GPIO_check (uint32_t base_gpio_addr, uint32_t tx_data, uint32_t rx_data) {

  uint32_t gpio_number;
  uint32_t rx_data_b [LINES_AMOUNT];
  // uint32_t rx_data_prev_b [LINES_AMOUNT];
  uint32_t status_b [32];
  uint8_t  test_status;

    switch (base_gpio_addr) {
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
    case MGPIO0_BASE: {
            gpio_number = 4;
            break;
    };
    case MGPIO1_BASE: {
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
  }






    if (rx_data == 0x0) {
      if ((status [gpio_number] != 0x0) & (gpio_number != 5)) {
        rumboot_printf ("Wrong IRQ status! ERROR! Got %x, expected %x\n", status [gpio_number], 0x0);
        // test_status = 1;
      }
    }
    else {
      if (rx_data == 0x55) {
        // rumboot_printf ("VECTOR1 = %x, rx_data = %x \n", VECTOR1, rx_data);
          if (rx_data_prev [gpio_number] == 0x0) {
            if (status [gpio_number] != 0x01010101) {
              rumboot_printf ("Wrong IRQ status! ERROR! Got %x, expected %x\n", status [gpio_number], 0x01010101);
              // test_status = 1;
            }
          }
          else {
            if (status [gpio_number] != 0x21212121) {
              // rumboot_printf ("rx_data_prev [%d] = %x\n", gpio_number, rx_data_prev [gpio_number]);
              rumboot_printf ("Wrong IRQ status! ERROR! Got %x, expected %x\n", status [gpio_number], 0x21212121);
              // test_status = 1;
            }
          }
      }
      else {  
        if (status [gpio_number] != 0x12121212) {
            rumboot_printf ("Wrong IRQ status! ERROR! Got %x, expected %x\n", status [gpio_number], 0x12121212);
            // test_status = 1;
          }

      }
    }
      if (rx_data != tx_data) {
        rumboot_printf ("Wrong data received! ERROR!\n Expected %x on GPIO %d\n Received %x\n", tx_data, gpio_number, rx_data);
        return 1;
      }
      else {
        #ifdef DEBUG
        rumboot_printf ("Correct Data! OK!\n");
        #endif
        return 0;
      }

}


int GPIO_test_rx (uint32_t base_gpio_addr, uint8_t tx_data, uint32_t rx_data_prev)
{   
  uint32_t count;

  uint32_t rx_data;
  uint32_t test_status;
  uint32_t gpio_number;

  count = 0x0;
  test_status = 0x0;

  #ifdef DEBUG
  rumboot_printf ("--------------------------GPIO %x rx test-------------------------------\n", base_gpio_addr);
  #endif

  switch (base_gpio_addr) {
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
    case MGPIO0_BASE: {
            gpio_number = 4;
            break;
    };
    case MGPIO1_BASE: {
            gpio_number = 5;
            break;
    };

    default: {
      rumboot_printf ("Wrong Base Address! ERROR!");
      return 1;
      break;
    }
  }


    // rx_data_prev [gpio_number] = rx_data;
    rx_data = GPIO_in (base_gpio_addr, tx_data);
  
    test_status = GPIO_check (base_gpio_addr, tx_data, rx_data);
    return test_status;
    

  }

  void wait_irq ()
  {
    uint32_t count;

    while (!irq_set [0]) {
      if (STATION_TIMEOUT != 0) {
        count = count + 1;
        if (count >= STATION_TIMEOUT)
        {
          rumboot_printf ("STATION TIMEOUT!\n");
          return 1;
        }
      }
      else {
      }
    }
  }

    void wait_tx (uint8_t exp_data)
  {
    uint32_t count;
    uint32_t rx_data;

    while (!irq_set [5]) {

      if (STATION_TIMEOUT != 0) {
        count = count + 1;
        if (count >= STATION_TIMEOUT)
        { 
          rumboot_printf ("STATION TIMEOUT!\n");
          return 1;
        }
      }
      else {
        // count = count + 1;
      }
    }
    #ifdef DEBUG
    rumboot_printf ("IRQ!\n");
    #endif
    rx_data = ioread32 (MGPIO1_BASE + GPIO_READ_PADtoAPB);
    rx_data = rx_data & 0x1;

    while (rx_data != exp_data) {
      rx_data = ioread32 (MGPIO1_BASE + GPIO_READ_PADtoAPB);
      rx_data = rx_data & 0x1;
    }
    #ifdef DEBUG
    rumboot_printf ("Correct IRQ!\n");
    #endif
  }

 int GPIO_test ()
{   
  uint32_t rx_data;
  uint32_t test_status;

  uint32_t address [6] = {GPIO1_BASE, GPIO2_BASE, GPIO3_BASE, GPIO4_BASE, MGPIO0_BASE, MGPIO1_BASE};

  test_status = 0x0;
  rx_data = 0x0;

  // #ifdef DEBUG
  rumboot_printf ("--------------------------GPIO rx test-------------------------------\n");
  // #endif


     #ifdef DEBUG
     rumboot_printf ("Change direction command\n");
     #endif
     iowrite32 (0x80, GPIO1_BASE + GPIO_DIRECTION);

    GPIO_config (MGPIO1_BASE, 0x0);

    
    GPIO_out (GPIO1_BASE, FLAG);

    wait_tx (0x1);

        

    for (int i = 0; i < GPIO_AMOUNT; ++i)
  {
      rx_data_prev [i] = 0;
      GPIO_config (address [i], 0x0);
  }
    wait_tx (0x0);

  #ifdef DEBUG
  rumboot_printf ("----------------------------VECTOR 0---------------------------------\n");
  #endif

    for (int i = 0; i < GPIO_AMOUNT; ++i)
  { 
    rx_data_prev [i] = ioread32 (address [i] + GPIO_READ_PADtoAPB);
  }

    for (int i = 0; i < GPIO_AMOUNT; ++i)
  { 
    test_status = test_status | GPIO_test_rx (address [i], VECTOR0, rx_data_prev [i]);
    irq_set [i] = 0x0;
  }
  wait_irq ();

  #ifdef DEBUG
  rumboot_printf ("----------------------------VECTOR 1---------------------------------\n");
  #endif

  for (int i = 0; i < GPIO_AMOUNT; ++i)
  {
    test_status = test_status | GPIO_test_rx (address [i], VECTOR1, rx_data_prev [i]);
    irq_set [i] = 0x0;
  }
    for (int i = 0; i < GPIO_AMOUNT; ++i)
  {
    rx_data_prev [i] = ioread32 (address [i] + GPIO_READ_PADtoAPB);

  }

  wait_irq ();

  #ifdef DEBUG
  rumboot_printf ("----------------------------VECTOR 2---------------------------------\n");
  #endif

  for (int i = 0; i < GPIO_AMOUNT; ++i)
  {
    test_status = test_status | GPIO_test_rx (address [i], VECTOR2, rx_data_prev [i]);
    irq_set [i] = 0x0;
  }

    for (int i = 0; i < GPIO_AMOUNT; ++i)
  {
    rx_data_prev [i] = ioread32 (address [i] + GPIO_READ_PADtoAPB);

  }

  wait_irq ();

  #ifdef DEBUG
  rumboot_printf ("----------------------------VECTOR 1---------------------------------\n");
  #endif

  for (int i = 0; i < GPIO_AMOUNT; ++i)
  {
    test_status = test_status | GPIO_test_rx (address [i], VECTOR1, rx_data_prev [i]);
  }

  // #ifdef DEBUG
  rumboot_printf ("--------------------------Wait for direction switch-------------------------------\n");
  // #endif

  wait_tx (0x0);
   // #ifdef DEBUG 
  rumboot_printf ("----------------------------GPIO tx test---------------------------------\n");
  // #endif

    for (int i = 0; i < GPIO_AMOUNT; ++i)
    {
      if (address [i] == MGPIO1_BASE) iowrite32 (0xfe, address [i] + GPIO_DIRECTION);
      else iowrite32 (0xff, address [i] + GPIO_DIRECTION);
    }
    
    for (int i = 0; i < GPIO_AMOUNT; ++i)
    {
      GPIO_out (address [i], VECTOR0);
    }


  wait_tx (0x0);


    for (int i = 0; i < GPIO_AMOUNT; ++i)
    {
     GPIO_out (address [i], VECTOR2);
    }

  wait_tx (0x0);
  
    for (int i = 0; i < GPIO_AMOUNT; ++i)
    {
      GPIO_out (address [i], VECTOR1);
    }

  wait_tx (0x0);

    for (int i = 0; i < GPIO_AMOUNT; ++i)
    {
     GPIO_out (address [i], VECTOR2);
    }

  wait_tx (0x0);

    for (int i = 0; i < GPIO_AMOUNT; ++i)
    {
      GPIO_config (address [i], 0x0);
    }

    return test_status;

  } 
      

int main()
{ 
  uint32_t test_status;

  rumboot_irq_cli();
  struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

  rumboot_irq_set_handler(tbl, GPIO1_INTR    , 0, GPIO_handler, NULL);
  rumboot_irq_set_handler(tbl, GPIO2_INTR    , 0, GPIO_handler, NULL);
  rumboot_irq_set_handler(tbl, GPIO3_INTR    , 0, GPIO_handler, NULL);
  rumboot_irq_set_handler(tbl, GPIO4_INTR    , 0, GPIO_handler, NULL);
  rumboot_irq_set_handler(tbl, MDIO_GPIO0_INT, 0, GPIO_handler, NULL);
  rumboot_irq_set_handler(tbl, MDIO_GPIO1_INT, 0, GPIO_handler, NULL);
  rumboot_irq_table_activate(tbl);
  rumboot_irq_sei();

  if ( !GPIO_test() )
  {
    rumboot_printf ("Test passed! OK!\n");
    return 0;
  }
  else {
    rumboot_printf ("Test Failed!\n");
    return 1;
  }
}

