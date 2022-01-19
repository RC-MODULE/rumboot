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

int g_argc = 0;
static int ipc_id;
char *g_argv[64];
#define NAME CMAKE_BINARY_DIR "/rumboot-native-" CMAKE_BUILD_TYPE "-spl-fail"
static struct rumboot_bootsource arr[] = {
        {
                .name = NAME,
                .plugin = &g_bootmodule_file,
        },
        {
                .name = NAME,
                .plugin = &g_bootmodule_file,
        },

        { /*Sentinel*/ }
};


void my_handler(int signum)
{
        if (signum == SIGUSR1) {
                printf("Received SIGUSR1, exiting with 0!\n");
                exit(0);
        }
        if (signum == SIGUSR2) {
                printf("Received SIGUSR2, exiting with 1!\n");
                exit(1);
        }
}


/* Platform-specific glue */
static uint64_t _boottime;
uint32_t rumboot_platform_get_uptime()
{
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        uint32_t  ret = (ts.tv_sec * 1000000) + (ts.tv_nsec / 1000) - _boottime;
        return ret;
}

void rumboot_platform_init_loader(struct rumboot_config *conf)
{

}

static int shmid;
static int the_ipc_id;
static void detach()
{
        rumboot_printf("Detaching from shared memory\n");
        shmdt(rumboot_platform_runtime_info);
        /* This is very hacky. It looks like the shared memory is released
         * not at the moment when the last process exits, but when
         * shmctl(shmid, IPC_RMID, NULL) is called.
         *
         * If we schedule removal at creation time, tests will fail since
         * each spl will work with a freshly allocated shared memory segment
         * (lolwut?)
         *
         */
        if (the_ipc_id == getpid()) {
                if (-1 == shmctl(shmid, IPC_RMID, NULL)) {
                        perror("shmctl: ");
                        exit(1);
                }
        }
}

static void *create_shared_memory(const char *skey, int id, size_t size)
{
        /* make the key: */
        key_t key;


        if ((key = ftok(skey, id)) == -1) { /*Here the file must exist */
                perror("ftok");
                exit(1);
        }

        /*  create the segment: */
        if ((shmid = shmget(key, size, 0644 | IPC_CREAT)) == -1) {
                perror("shmget");
                exit(1);
        }

        /* attach to the segment to get a pointer to it: */
        void *data = shmat(shmid, (void *)0, 0);
        if (data == (char *)(-1)) {
                perror("shmat");
                exit(1);
        }

        the_ipc_id = id;
        atexit(detach);

        return data;
}



static int do_selftest = 0;
static int do_host = 0;
static char *hfile = "null";

static struct option long_options[] =
{
        { "selftest", no_argument,	 &do_selftest, 1   },
        { "host",     no_argument,	 &do_host,     1   },
        { "align",    required_argument, 0,	       'a' },
        { "file",     required_argument, 0,	       'f' },
        { "file2",    required_argument, 0,	       'F' },
        { "hfile",    required_argument, 0,	       'h' },
        { "ipckey",   required_argument, 0,	       'i' },
        { 0,	      0,		 0,	       0   }
};

#ifdef RUMBOOT_ENABLE_NATIVE_PCIE
uintptr_t DUT_BASE;
#endif

void rumboot_platform_setup()
{
        _boottime = rumboot_platform_get_uptime();
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

        ipc_id = getpid();

        while (1) {
                int option_index = 0;
                int c = getopt_long(g_argc, g_argv, "sha:F:f:h:i:",
                                    long_options, &option_index);
                if (c == -1) {
                        break;
                }

                if (c == '?') {
                        rumboot_printf("FATAL: Missing required argument");
                        exit(1);
                }

                switch (c) {
                case 'i':
                        ipc_id = atoi(optarg);
                        break;

                case 'a':
                        g_bootmodule_file.align = atoi(optarg);
                        rumboot_printf("DEBUG: setting alignment to %d\n", g_bootmodule_file.align);
                        break;

                case 'h':
                        hfile = strdup(optarg);
                        hfile = strtok(hfile, ",");
                        rumboot_printf("DEBUG: setting host boot file to %s\n", optarg);
                        break;

                case 'F':
                        arr[1].name = optarg;
                        rumboot_printf("DEBUG: setting 2nd boot file to %s\n", optarg);
                        break;

                case 'f':
                        arr[0].name = optarg;
                        rumboot_printf("DEBUG: setting 1st boot file to %s\n", optarg);
                        break;
                }
        }

        const char *token, *token_touch_cmd;
        if (0 == access(CMAKE_BINARY_DIR, W_OK)) {
                token = CMAKE_BINARY_DIR "/rumboot.tmp";
        } else {
                token = tmpnam(NULL);
        }
        FILE *tkfile = fopen(token, "w+");
        fclose(tkfile);

        //system("touch " token "/rumboot.tmp");
        #define HEAP_SIZE (1 * 1024 * 1024)
        rumboot_platform_runtime_info = create_shared_memory(token, ipc_id, sizeof(rumboot_platform_runtime_info) + HEAP_SIZE);
        if (ipc_id == getpid()) {
                /* If we own memory, clear it! */
                memset(rumboot_platform_runtime_info, 0x0, sizeof(*rumboot_platform_runtime_info));
                /* And add a heap */
                char *heap = rumboot_platform_runtime_info;
                heap = &heap[sizeof(*rumboot_platform_runtime_info)];
	        rumboot_malloc_register_heap("IM0", heap, &heap[HEAP_SIZE]);
#ifdef RUMBOOT_ENABLE_NATIVE_PCIE
                void *ptr = rumboot_native_request_device(0, 0x0000, 2 * 1024 * 1024);
        	rumboot_malloc_register_heap("IM1", ptr, ptr + 32 * 1024 * 1024);
                ptr = rumboot_native_request_device(2, 32 * 1024 * 1024, 2 * 1024 * 1024);
        	rumboot_malloc_register_heap("IM2", ptr, ptr + 32 * 1024 * 1024);
                DUT_BASE = rumboot_native_request_device(1, 0x0000, 0xfd0000);
#endif
        }

}


static void upload(void *addr)
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
        case EVENT_GENERIC: {
                size_t tmp;
                const char *plusarg = (void *)data[0];
                void *addr = rumboot_platform_get_spl_area(&tmp);
                if (strcmp(plusarg, "HOST") == 0) {
                        upload(addr);
                }
                break;
        }
        case EVENT_UPLOAD: {
                size_t dummy;
                const char *plusarg = (void *)data[0];
                void *addr = (void *)data[1];
                if (strcmp(plusarg, "HOSTMOCK") == 0) {
                        upload(addr);
                }
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


static bool mytest(uint32_t arg)
{
        /* Return true if test passed, false otherwise */
        return (rand() % 2) ? true : false;
}

static bool myskip_func(uint32_t arg)
{
        /* return true to skip test */
        return true;
}

TEST_SUITE_BEGIN(selftest, "dummies")
TEST_ENTRY("shit-o-test-1", mytest, 0),
TEST_ENTRY_COND("shit-o-test-cond", mytest, 0, myskip_func),
TEST_ENTRY("shit-o-test-2", mytest, 0),
TEST_ENTRY("shit-o-test-3", mytest, 0),
TEST_ENTRY("shit-o-test-4", mytest, 0),
TEST_ENTRY("shit-o-test-5", mytest, 0),
TEST_ENTRY("shit-o-test-6", mytest, 0),
TEST_SUITE_END();

int rumboot_platform_selftest(struct rumboot_config *conf)
{
        return test_suite_run(NULL, &selftest);
}


int run_binary(const char *command)
{
        sigset_t blockMask, origMask;
        struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
        pid_t childPid;
        int status, savedErrno;

        if (command == NULL) {          /* Is a shell available? */
                return system(":") == 0;
        }

        /* The parent process (the caller of system()) blocks SIGCHLD
         * and ignore SIGINT and SIGQUIT while the child is executing.
         * We must change the signal settings prior to forking, to avoid
         * possible race conditions. This means that we must undo the
         * effects of the following in the child after fork(). */

        sigemptyset(&blockMask);        /* Block SIGCHLD */
        sigaddset(&blockMask, SIGCHLD);
        sigprocmask(SIG_BLOCK, &blockMask, &origMask);

        saIgnore.sa_handler = SIG_IGN;  /* Ignore SIGINT and SIGQUIT */
        saIgnore.sa_flags = 0;
        sigemptyset(&saIgnore.sa_mask);
        sigaction(SIGINT, &saIgnore, &saOrigInt);
        sigaction(SIGQUIT, &saIgnore, &saOrigQuit);

        switch (childPid = fork()) {
        case -1:        /* fork() failed */
                status = -1;
                break;  /* Carry on to reset signal attributes */

        case 0:         /* Child: exec command */

                /* We ignore possible error returns because the only specified error
                 * is for a failed exec(), and because errors in these calls can't
                 * affect the caller of system() (which is a separate process) */

                saDefault.sa_handler = SIG_DFL;
                saDefault.sa_flags = 0;
                sigemptyset(&saDefault.sa_mask);

                if (saOrigInt.sa_handler != SIG_IGN) {
                        sigaction(SIGINT, &saDefault, NULL);
                }
                if (saOrigQuit.sa_handler != SIG_IGN) {
                        sigaction(SIGQUIT, &saDefault, NULL);
                }

                sigprocmask(SIG_SETMASK, &origMask, NULL);

                char tmp[128];
                sprintf(tmp, "%d", ipc_id);
                execl(command, command, "--ipckey", tmp, (char *)NULL);
                kill(getppid(), SIGUSR2); /* Tell parent we've failed to exec the shell */
        default:                /* Parent: wait for our child to terminate */

                /* We must use waitpid() for this task; using wait() could inadvertently
                 * collect the status of one of the caller's other children */
                while (waitpid(childPid, &status, 0) == -1) {
                        if (errno != EINTR) {   /* Error other than EINTR */
                                status = -1;
                                break;          /* So exit loop */
                        }
                }
                break;
        }

        /* Unblock SIGCHLD, restore dispositions of SIGINT and SIGQUIT */

        savedErrno = errno;             /* The following may change 'errno' */

        sigprocmask(SIG_SETMASK, &origMask, NULL);
        sigaction(SIGINT, &saOrigInt, NULL);
        sigaction(SIGQUIT, &saOrigQuit, NULL);

        errno = savedErrno;

        return WEXITSTATUS(status);
}

static int exec_prog(const struct rumboot_cpu_cluster *cpu,  struct rumboot_bootheader *hdr, void *data, int swap)
{
        char ret;
        int err;
        FILE *tmp = fopen("binary", "w");
        if (swap) {
                err = fwrite(hdr->data, __swap32(hdr->datalen), 1, tmp);
        } else {
                err = fwrite(hdr->data, hdr->datalen, 1, tmp);                
        }
        if (err <= 0) {
                fprintf(stderr, "failed to write temp file!\n");
        }
        fclose(tmp);
        system("chmod +x binary");
        rumboot_printf("rom: spl exec count: %d\n",
                       rumboot_platform_runtime_info->persistent[0]
                       );
        ret = run_binary("binary");
        rumboot_printf("%d\n", ret);
        return ret;
}

static const struct rumboot_cpu_cluster own[] = {
    {
        .name = "native (boot)",
        .start = exec_prog,
    }
};

const struct rumboot_cpu_cluster *rumboot_platform_get_cpus(int *cpu_count)
{
    *cpu_count = ARRAY_SIZE(own);
    return own;
}

uint32_t rumboot_virt_to_dma(volatile const void *addr)
{
        return (uint32_t) addr;
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{

}

void rumboot_platform_enter_host_mode(struct rumboot_config *conf)
{

}
