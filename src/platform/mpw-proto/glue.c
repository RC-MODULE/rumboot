#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
//#include <rumboot/irq.h>

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
    return 0;
}

void __attribute__((noreturn)) rumboot_platform_panic(const char *why, ...)
{
    va_list ap;
    va_start(ap, why);
    printf("PANIC: ");
    vprintf(why, ap);
    va_end(ap);
    exit(1);
}

void rumboot_platform_trace(void *pc)
{
    /* stack tracing code here */
}

void rumboot_platform_event_raise(enum rumboot_simulation_event event, uint32_t *data, uint32_t len)
{
    exit(event);
}

#define UART0__       0x3C034000
#define PL011_UARTDR  0x0
#define PL011_UARTFR  0x018
#define PL011_UARTRIS 0x03C
#define PL011_TXFF_i  5
#define PL011_TXFE_i  7
#define PL011_TXRIS_i 5

static int tx_fifo_ready(uint32_t base_addr)
{
  if (ioread32(base_addr + PL011_UARTFR) & (1 << PL011_TXFE_i))
    return 0;

  if (ioread32(base_addr + PL011_UARTRIS) & (1 << PL011_TXRIS_i))
    return 0;

  do{

    if (!(ioread32(base_addr + PL011_UARTFR) & (1 << PL011_TXFF_i)))
      return 0;
  }
  while(1);

  return -1;
}

void rumboot_platform_putchar(uint8_t c)
{
  if (tx_fifo_ready(UART0__) == 0)
    iowrite32((char)c, UART0__ + PL011_UARTDR);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
    return (uint8_t) getc(stdin);
}

struct rumboot_runtime_info rumboot_platform_runtime_info;

void rumboot_platform_irq_init()
{

}

void rumboot_arch_irq_disable()
{

}

void rumboot_arch_irq_enable()
{

}

void rumboot_platform_request_file(const char *plusarg, uint32_t addr)
{

}

void rumboot_platform_dump_region(const char *filename, uint32_t addr, uint32_t len)
{

}

void rumboot_platform_perf(const char *tag)
{


}

void rumboot_platform_setup()
{
  
}
