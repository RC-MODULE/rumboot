#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>

#undef errno
extern int errno;

char *__env[1] = { 0 };
char **environ = __env;

int _write(int file, char *ptr, int len);

int _gettimeofday(struct timeval *__tp, void *__tzp)
{
	return -1;
}

void _exit(int status)
{
	while (1)
		;
}

int _close(int file)
{
	return -1;
}

int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}

int _fork()
{
	errno = EAGAIN;
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _getpid()
{
	return 1;
}

int _isatty(int file)
{
	switch (file) {
	case STDOUT_FILENO:
	case STDERR_FILENO:
	case STDIN_FILENO:
		return 1;
	default:
		//errno = ENOTTY;
		errno = EBADF;
		return 0;
	}
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}



#if 1
caddr_t _sbrk(int incr)
{
    rumboot_platform_panic("malloc() not allowed in ROM code\n");
	return (caddr_t)0;
}
#else
#include <rumboot/platform.h>
caddr_t _sbrk(int incr)
{
	rumboot_platform_raise_event(EVENT_PERF, 3);
	char *prev_heap_end;

	if (rumboot_platform_runtime_info.current_heap_end == 0)
		rumboot_platform_runtime_info.current_heap_end = &rumboot_platform_heap_start;

	prev_heap_end = rumboot_platform_runtime_info.current_heap_end;

	if ((rumboot_platform_runtime_info.current_heap_end + incr) > &rumboot_platform_heap_end) {
		rumboot_printf("FATAL: heap and stack collision\n");
		/* Heap and stack collision */
		return (caddr_t)0;
	}

	rumboot_platform_runtime_info.current_heap_end += incr;
	rumboot_platform_raise_event(EVENT_PERF, 4);
	return (caddr_t)prev_heap_end;
}
#endif

int _stat(const char *filepath, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

clock_t _times(struct tms *buf)
{
	return -1;
}

int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

int _wait(int *status)
{
	errno = ECHILD;
	return -1;
}

int _write(int file, char *ptr, int len)
{
	errno = EBADF;
	return -1;
}

int _read(int file, char *ptr, int len)
{
	errno = EBADF;
	return -1;
}
