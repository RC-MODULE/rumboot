#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include <rumboot/printf.h>

#undef errno
extern int errno;

char *__env[1] = { 0 };
char **environ = __env;

int _write(int file, char *ptr, int len);

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
    rumboot_printf("FATAL: malloc() not allowed in ROM code\n");
	return (caddr_t)0;
}
#else
caddr_t _sbrk(int incr)
{
	extern char *heap_low;   /* Defined by the linker */
	extern char *heap_top;   /* Defined by the linker */
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = &heap_low;
	prev_heap_end = heap_end;

	if (heap_end + incr > &heap_top) {
		rumboot_printf("FATAL: heap and stack collision\n");
		/* Heap and stack collision */
		return (caddr_t)0;
	}

	heap_end += incr;
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
