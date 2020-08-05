// Test: simple-test on connect demonstrator in scr1-system 

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map scr1
#include <regs/regs_demonstrator.h> // regs names demonstrator

#include <devices/mdma_simple.h> // lib for work with mdma


int main() {
  rumboot_printf("TEST simple-test start\n");



  rumboot_printf("TEST simple-test PASSED\n");
  return 0;
}