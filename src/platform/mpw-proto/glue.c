#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/boot.h>
#include <platform/devices.h>
#include <rumboot/bootsrc/sdio.h>



#define IBM_BIT_INDEX(size, index)    (((size) - 1) - ((index) % (size)))
#define ITRPT_XSR_FP_e  50
#define ITRPT_XSR_FP_i IBM_BIT_INDEX(64, ITRPT_XSR_FP_e)

void msr_write(uint32_t const wval)
{
	__asm volatile
	(
		"mtmsr %0 \n\t"
		::"r" (wval)
	);
}

uint32_t msr_read()
{
	uint32_t rval = 0;
	__asm volatile
	(
		"mfmsr %0 \n\t"
		: "=r" (rval)
	);

	return rval;
}

static void enable_fpu()
{
	msr_write(msr_read() | (1 << ITRPT_XSR_FP_i));
}


/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{

  #define TIMER_TICKS_PER_US  800
  #define SPR_TBL_R           0x10C

  uint32_t value = 0;

  __asm volatile
  (
      "mfspr %0, %1 \n\t"
      :"=r"(value)
        :"i"(SPR_TBL_R)
      :
  );

  return value / TIMER_TICKS_PER_US;
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
  if (c == '\n')
    rumboot_platform_putchar('\r');

  if (tx_fifo_ready(UART0_BASE) == 0)
    iowrite32((char)c, UART0_BASE + PL011_UARTDR);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
    return (uint8_t) getc(stdin);
}

struct rumboot_runtime_info rumboot_platform_runtime_info;

void rumboot_platform_irq_init()
{

}

uint32_t rumboot_arch_irq_disable()
{
    return 0;
}

uint32_t rumboot_arch_irq_enable()
{
    return 0;
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
  enable_fpu();
}

static bool sdio_enable(const struct rumboot_bootsource* src, void* pdata, int select)
{
  return true;
}

static void sdio_disable(const struct rumboot_bootsource* src, void* pdata)
{

}


static const struct rumboot_bootsource arr[] = {
  {
      .name = "SDIO (CD: GPIO1_X)",
      .base = SDIO_BASE,
      .freq_khz = 100000,
      .plugin = &g_bootmodule_sdio,
      .enable  = sdio_enable,
      .disable = sdio_disable,
  },
	{ /*Sentinel*/ }
};

const struct rumboot_bootsource *rumboot_platform_get_bootsources()
{
	return arr;
}

bool rumboot_platform_check_entry_points(struct rumboot_bootheader *hdr)
{
	/* Any entry point is okay */
	return true;
}

void rumboot_platform_exec(struct rumboot_bootheader *hdr)
{
	/* No-op, this chip has only one core */
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{
  conf->baudrate = 1000000;
  conf->hostmode = 0;

}


void rumboot_platform_selftest(struct rumboot_config *conf)
{
	/* Execute selftest routines */
}
