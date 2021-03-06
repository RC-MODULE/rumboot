#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>

#define DATA_LEN  4096

uint8_t test_data_arr [DATA_LEN] __attribute__((section(".data"))) = {0};

int main()
{
  test_event_send_test_id("test_iss_base");

  rumboot_putstring ("START ADDR");
  rumboot_puthex ((uint32_t) test_data_arr);

  rumboot_putstring ("END ADDR");
  rumboot_puthex ((uint32_t) &test_data_arr[DATA_LEN - 1]);

  rumboot_putstring ("START WRITE DATA\n");
  uint8_t count = 0;
  uint32_t i = 0;
  while (i < DATA_LEN)
  {
    test_data_arr [i] = count;
  	i++; count++;
  }
  rumboot_putstring ("END WRITE DATA\n");
  rumboot_putstring ("CREATE DUMP");
  rumboot_issdump((uint32_t) test_data_arr, DATA_LEN);

  return 0;
}
