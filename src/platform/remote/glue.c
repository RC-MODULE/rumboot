#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <rumboot/timer.h>
#include <rumboot/bootsrc/file.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/testsuite.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <rumboot/bitswapper.h>
#include <platform/devices.h>
#include <time.h>
#include <rumboot/rumboot.h>
#include <platform/vl_api.h>

int g_argc = 0;
static int   vl_port = 3425;
static char *vl_host = "localhost";
struct vl_instance *g_vl_instance; 
uint64_t g_ticks_per_us = 5;

char *g_argv[64];

static struct rumboot_bootsource arr[] = {
        { /*Sentinel*/ }
};

void my_handler(int signum)
{
        /* TODO: */
        if (signum == SIGUSR1) {
                printf("Received SIGUSR1, exiting with 0!\n");
                exit(0);
        }
        if (signum == SIGUSR2) {
                printf("Received SIGUSR2, exiting with 1!\n");
                exit(1);
        }
}


/* FixMe: ??? */


uint32_t rumboot_platform_get_uptime()
{
        uint64_t timestamp  = vl_get_uptime(g_vl_instance);
        return timestamp / g_ticks_per_us;
}

void rumboot_platform_init_loader(struct rumboot_config *conf)
{

}


static int do_selftest = 0;
static int do_host = 0;
static int wdelay = 0;
static int rdelay = 0;

static struct option long_options[] =
{
        { "host",       required_argument, 0,          'H' },
        { "port",       required_argument, 0,	       'p' },
        { "wdelay",     required_argument, 0,	       'w' },
        { "rdelay",     required_argument, 0,	       'r' },
        { "help",       no_argument,       0,	       'h' },
        { 0,	      0,		 0,	       0   }
};

#ifdef RUMBOOT_ENABLE_NATIVE_PCIE
uintptr_t DUT_BASE;
#endif



void cleanup()
{
        printf("Disconnecting from model\n");
        vl_disconnect(g_vl_instance);
}

void rumboot_platform_setup()
{
        rumboot_platform_runtime_info = malloc(sizeof(*rumboot_platform_runtime_info));
        signal(SIGUSR1, my_handler);
        signal(SIGUSR2, my_handler);
        FILE *fd = fopen("/proc/self/cmdline", "r");
        char *tmp = malloc(4096);
        size_t sz = fread(tmp, 1, 4096, fd);
        size_t pos = 0;

        while (pos < sz) {
                char *arg = &tmp[pos];
                size_t len = strlen(arg);
                pos += len + 1;
                g_argv[g_argc++] = arg;
        }
        fclose(fd);


        while (1) {
                int option_index = 0;
                int c = getopt_long(g_argc, g_argv, "H:p:w:r:h",
                                    long_options, &option_index);
                if (c == -1) {
                        break;
                }

                if (c == '?') {
                        rumboot_printf("FATAL: Missing required argument");
                        exit(1);
                }

                switch (c) {
                case 'p':
                        vl_port = atoi(optarg);
                        break;
                case 'H':
                        vl_host = strdup(optarg);
                        break;
                case 'w':
                        wdelay = atoi(optarg);
                        break;
                case 'r':
                        rdelay = atoi(optarg);
                        break;
                case 'h':
                        rumboot_printf("--- Valid commandline options ---\n");
                        rumboot_printf("--help           - this help\n");
                        rumboot_printf("--host=localhost - specify server host\n");
                        rumboot_printf("--port=3425      - specify server port\n");
                        rumboot_printf("--wdelay=0       - specify iowrite delay (in cycles)\n");
                        rumboot_printf("--rdelay=0       - specify ioread delay  (in cycles)\n");
                        break;
                }
        }
        
        for (int i=optind; i<g_argc; i++) {
                if (g_argv[i][0]!='+')
                        continue;

                char *tmp = strdup(&g_argv[i][1]);
                char *key = strtok(tmp, "=");
                char *value = strtok(NULL, "");
                setenv(key, value, 1);
                free(tmp);
        }

        #define IM0_HEAP_SIZE (512 * 1024)
        uint8_t *heap = malloc(IM0_HEAP_SIZE);
	rumboot_malloc_register_heap("IM0", &heap[0], &heap[IM0_HEAP_SIZE]);

        g_vl_instance = vl_create(vl_host, vl_port);
        if (!g_vl_instance) {
                rumboot_platform_panic("VL_API: Failed to connect to %s:%d\n", vl_host, vl_port);
        }

        if (wdelay || rdelay) {
                rumboot_printf("VL_API: Adjusting io delays: wdelay: %d rdelay: %d\n", wdelay, rdelay);
                vl_set_io_delay(g_vl_instance, rdelay, wdelay);
        }

        g_ticks_per_us = vl_get_ticks_per_us(g_vl_instance);
        if (g_ticks_per_us == 0) {
                rumboot_platform_panic("VL_API: FATAL: g_ticks_per_us can't be zero");
        }
        rumboot_printf("VL_API: Simulation timescale: %u ticks per us\n", (uint32_t) g_ticks_per_us);


        struct vl_shmem* shm = vl_shmem_list(g_vl_instance, 0);
        while (shm->size) {
                printf("VL_API: shmem #%d name: %s size: %llx start: %llx\n", shm->id, shm->name, shm->size, shm->sys_addr);
                uint8_t *ptr = vl_shmem_map(shm);
                if (!ptr) {
                        printf("VL_API: Fatal. Failed to mmap heap %s\n", shm->name);
                }
                rumboot_malloc_register_heap(shm->name, &ptr[0], &ptr[shm->size]);
                shm++;
        }
        atexit(cleanup);
}


static void upload(const char *hfile, void *addr, uint32_t maxsize)
{
        if (!hfile) {
                return;
        }
        FILE *fd = fopen(hfile, "r");
        if (!fd) {
                return;
        }
        size_t sz;
        fseek(fd, 0, SEEK_END);
        sz = ftell(fd);
        fseek(fd, 0, SEEK_SET);
        if (maxsize<sz)
                sz=maxsize;
        fread((void *)addr, sz, 1, fd);
        fclose(fd);
        hfile = strtok(NULL, ",");
}

void rumboot_platform_event_raise(enum rumboot_simulation_event event, uint32_t const *data, uint32_t len)
{
        switch (event) {
        case EVENT_TERM:
                exit(data[0]);
                break;
        case EVENT_UPLOAD: {
                size_t dummy;
                const char *plusarg = (void *)data[0];
                void *addr = (void *)data[1];
                if (access(plusarg, R_OK) == 0) {
                        upload(plusarg, addr,data[2]);
                        return;
                } 
                
                if (NULL != getenv(plusarg)) {
                        char * path = getenv(plusarg);
                        if (access(path, R_OK) == 0) {
                                upload(path, addr,data[2]);
                                return;        
                        }
                }
                rumboot_platform_panic("FATAL: Upload of %s failed", plusarg);
                break;
        }
        default:
                break;
        }
}

void rumboot_platform_putchar(uint8_t c)
{
        putc(c, stdout);
        fflush(stdout);
}

int ngetc()
{
    char c;
    struct pollfd pollfds;
    pollfds.fd = STDIN_FILENO;
    pollfds.events = POLLIN;

    poll(&pollfds, 1, 0);

    if(pollfds.revents & POLLIN)
    {
            //Bonus points go to the people that can tell me if
            //read() will change the value of '*c' if an error
            //occurs during the read
        read(STDIN_FILENO, &c, 1);
        return (int) c;
    }

    return -1;
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
        uint32_t start = rumboot_platform_get_uptime();
        while (rumboot_platform_get_uptime() - start < timeout_us) {
                int ret = ngetc();
                if (ret >= 0)
                        return ret;
        }
        return -1;
}


uint32_t rumboot_arch_irq_disable()
{
        return 0;
}

uint32_t rumboot_arch_irq_enable()
{
        return 0;
}

const struct rumboot_bootsource *rumboot_platform_get_bootsources()
{
        return arr;
}

bool rumboot_platform_check_entry_points(struct rumboot_bootheader *hdr)
{
        /* Any entry point is okay */
        return true;
}

static void *spl_area;
void *rumboot_platform_get_spl_area(size_t *size)
{
#define SIZE  4096 * 1024
        *size = SIZE;
        if (spl_area) {
                return spl_area;
        }
        spl_area = malloc(SIZE);
        printf("malloc spl are: %x\n", spl_area);
        return spl_area;
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        conf->hostmode = do_host;
        conf->selftest = do_selftest;
        conf->baudrate = 115200;
}



int rumboot_platform_selftest(struct rumboot_config *conf)
{
        return 0;
}

static const struct rumboot_cpu_cluster own[] = {
};

const struct rumboot_cpu_cluster *rumboot_platform_get_cpus(int *cpu_count)
{
    *cpu_count = ARRAY_SIZE(own);
    return own;
}

uint32_t rumboot_virt_to_dma(volatile const void *addr)
{
        int id = rumboot_malloc_heap_by_addr(addr);
        if (id == -1 ) {
                rumboot_platform_panic("rumboot_virt_to_dma: can't find heap id for addr %x\n", addr);
        }
        uint32_t ret = (uint32_t) (vl_virt_to_phys(g_vl_instance, addr) & 0xffffffff);
        if (ret == 0) {
                rumboot_platform_panic("rumboot_virt_to_dma: No dma mapping for heap %s", rumboot_malloc_heap_name(id));
        }
        return ret;
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{

}

void rumboot_platform_enter_host_mode(struct rumboot_config *conf)
{

}
