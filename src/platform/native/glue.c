#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/file.h>
#include <getopt.h>

extern int g_argc;
extern char *g_argv[64];

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
        return 0;
}

void rumboot_platform_setup()
{
        FILE *fd = fopen("/proc/self/cmdline", "r");
        char *tmp = malloc(4096);
        size_t sz = fread(tmp, 1, 4096, fd);
        size_t pos = 0;
        int argc = 0;

        while (pos < sz) {
                char *arg = &tmp[pos];
                size_t len = strlen(arg);
                pos += len + 1;
                g_argv[g_argc++] = arg;
        }
        fclose(fd);
        /* No - op */
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
        return (uint8_t)getc(stdin);
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
static struct rumboot_bootsource arr[] = {
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


static int do_selftest = 0;
static int do_host = 0;
static int do_align = 0;

static struct option long_options[] =
{
        { "selftest", no_argument,	 &do_selftest, 1   },
        { "host",     no_argument,	 &do_host,     1   },
        { "align",    required_argument, 0,	       'a' },
        { "create",   required_argument, 0,	       'c' },
        { "file",     required_argument, 0,	       'f' },
        { 0,	      0,		 0,	       0   }
};

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        while (1) {
                int option_index = 0;
                int c = getopt_long(g_argc, g_argv, "sha:",
                                    long_options, &option_index);
                if (c == -1) {
                        break;
                }
                switch (c) {
                case 'a':
                        g_bootmodule_file.align = atoi(optarg);
                        rumboot_printf("DEBUG: setting alignment to %d\n", g_bootmodule_file.align);
                        break;
                case 'f':
                        arr[0].name = optarg;
                        rumboot_printf("DEBUG: setting boot file to %d\n", optarg);
                        break;
                }
        }

        conf->hostmode = do_host;
        conf->selftest = do_selftest;
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
        return ret;
        if (ret < -1) {
                return ret;
        }
        printf("native: Not going to host mode: %d\n", ret);
}

uint32_t rumboot_virt_to_dma(volatile void *addr)
{
        return (uint32_t)addr;
}
