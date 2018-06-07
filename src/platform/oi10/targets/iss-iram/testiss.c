#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>

#define DATA_LEN = 1024;

uint8_t test_data_arr [DATA_LEN] __attribute__((section(".issdata"))) = {0};

int main()
{
  uint8_t count = 0;
  uint32_t i = 0;
  while (i++ < DATA_LEN)
    test_data_arr [i] = count++;

  while (0);

  return 0;
}
