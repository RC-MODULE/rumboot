#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
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
uint32_t rumboot_platform_get_uptime()
{
        return 0;
}


static int shmid;
static int the_ipc_id;
static void detach() {
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
                rumboot_printf("RELEASE!\n");
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

void rumboot_platform_setup()
{
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

        system("touch " CMAKE_BINARY_DIR "/rumboot.tmp");
        rumboot_platform_runtime_info = create_shared_memory(CMAKE_BINARY_DIR "/rumboot.tmp", ipc_id, sizeof(rumboot_platform_runtime_info));
        if (ipc_id == getpid()) {
                /* If we own memory, clear it! */
                memset(rumboot_platform_runtime_info, 0x0, sizeof(*rumboot_platform_runtime_info));
        }
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
        if (strcmp(plusarg, "HOSTMOCK") == 0) {
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
}

void rumboot_platform_dump_region(const char *filename, uint32_t addr, uint32_t len)
{
}

void rumboot_platform_perf(const char *tag)
{
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

void *rumboot_platform_get_spl_area(size_t *size)
{
#define SIZE  4096 * 1024
        *size = SIZE;
        return malloc(SIZE);
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

void rumboot_platform_selftest(struct rumboot_config *conf)
{
        test_suite_run(NULL, &selftest);
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
                _exit(127);     /* We could not exec the shell */

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


int rumboot_platform_exec(struct rumboot_bootheader *hdr)
{
        char ret;
        FILE *tmp = fopen("binary", "w");

        fwrite(hdr->data, hdr->datalen, 1, tmp);
        fclose(tmp);
        system("chmod +x binary");
        rumboot_printf("rom: spl exec count: %d\n",
                       rumboot_platform_runtime_info->persistent[0]
                       );
        ret = run_binary("binary");
        rumboot_printf("%d\n", ret);
        return ret;
}

uint32_t rumboot_virt_to_dma(volatile void *addr)
{
        return (uint32_t)addr;
}
