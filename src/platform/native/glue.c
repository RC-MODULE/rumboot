#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/file.h>

/* HACK: dummy variables */
char rumboot_platform_spl_start;
char rumboot_platform_spl_end;

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

void rumboot_platform_putchar(uint8_t c)
{
    putc(c, stdout);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
    return (uint8_t) getc(stdin);
}

struct rumboot_runtime_info rumboot_platform_runtime_info;


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
  printf("REQUEST %s\n", plusarg);
}

void rumboot_platform_dump_region(const char *filename, uint32_t addr, uint32_t len)
{

}

void rumboot_platform_perf(const char *tag)
{

}

#define NAME CMAKE_BINARY_DIR "/rumboot-native-Production-spl-ok"
static const struct rumboot_bootsource arr[] = {
  {
      .name = NAME,
      .plugin = &g_bootmodule_file,
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

void *rumboot_platform_get_spl_area(size_t *size)
{
  #define SIZE  4096 * 1024
  *size = SIZE;
  return malloc(SIZE);
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{
	conf->hostmode = 0;
	conf->selftest = 1;
}


void rumboot_platform_selftest(struct rumboot_config *conf)
{
	/* Execute selftest routines */
}

int rumboot_platform_exec(struct rumboot_bootheader *hdr)
{
  int ret;
  FILE *tmp = fopen("binary", "w");
  fwrite(hdr->data, hdr->datalen, 1, tmp);
  fclose(tmp);
  system("chmod +x binary");
  ret = system("./binary");
  if (ret < -1) {
    return ret;
  }
  printf("native: Not going to host mode: %d\n", ret);
}

uint32_t rumboot_virt_to_dma(volatile void *addr)
{
    return (uint32_t) addr;
}

