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

int _write(int file, const char *ptr, int len)
{
#ifndef RUMBOOT_NEWLIB_PRINTF
    rumboot_printf(ptr);
#else
	int i;
	for (i=0; i<len; i++)
		rumboot_putchar(ptr[i]);
#endif
	return len;
}


struct timeval;
int _gettimeofday(struct timeval *__tp, void *__tzp)
{
	return -1;
}

extern void (*__fini_array_start []) (void) __attribute__((weak));
extern void (*__fini_array_end []) (void) __attribute__((weak));
void _fini();

static void rumboot_fini_array (void)
{
  size_t count;
  size_t i;

  count = __fini_array_end - __fini_array_start;
  for (i = count; i > 0; i--)
    __fini_array_start[i-1] ();

  _fini();

}

/* Override newlib exit */
void exit(int status)
{
    rumboot_fini_array();
	_exit(status);
}

void __attribute__((weak)) rumboot_platform_exit(int status) {
	/* Default implementation does nothing */
}

void _exit(int status)
{
		rumboot_platform_exit(status);
		#ifndef RUMBOOT_NOINIT
			/* Normal binary, do a long-jump and return to bootrom */
			longjmp(rumboot_platform_runtime.exit_trampoline, 256 + status);
		#endif

		#ifndef __NM__
		rumboot_printf("\n_exit: System halted, code %d\n", status);
		#endif
		while(1);;
}

int _close(int file)
{
	return -1;
}

int _execve(const char *name, char * const *argv, char * const *env)
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

int _open( const char *name, int flags, int mode)
{
    //rumboot_printf("OPEN: %s\n", name);
    return -1;
}


#if 1
caddr_t _sbrk(int incr)
{
    rumboot_platform_panic("libc malloc() disabled\n");
	return (caddr_t)0;
}

#else
caddr_t _sbrk(int incr)
{
	char *prev_heap_end;
	struct rumboot_heap *hp = &rumboot_platform_runtime_info->heaps[0];


	if ((hp->pos + incr) > hp->end) {
		rumboot_printf("FATAL: heap overflow\n");
		/* Heap and stack collision */
		return (caddr_t)-1;
	}
	prev_heap_end = hp->pos;
	hp->pos += incr;
	return (caddr_t) prev_heap_end;
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


int _read(int file, char *ptr, int len)
{
	errno = EBADF;
	return -1;
}



/* reentrant stubs */

int _close_r  (struct _reent *r, int v)
{
    return _close(v);
}

int _execve_r  (struct _reent *r, const char *n, char *const *a, char *const *e)
{
	return _execve(n, a, e);
}

void *_sbrk_r  (struct _reent *r, ptrdiff_t d)
{
	return _sbrk(d);
}

_ssize_t _write_r  (struct _reent *r, int fd, const void * ptr, size_t sz)
{
	return _write(fd, ptr, sz);
}

int _fstat_r  (struct _reent *r, int fd, struct stat *s)
{
	return _fstat(fd, s);
}

int _isatty_r  (struct _reent *r , int fd)
{
	return _isatty(fd);
}

_off_t _lseek_r  (struct _reent *r, int f, _off_t ptr, int dir)
{
	return _lseek(f, ptr, dir);
}

_ssize_t _read_r  (struct _reent *r, int f, void *ptr, size_t sz)
{
	return _read(f, ptr, sz);
}

int _open_r  (struct _reent *r, const char *path, int a, int b)
{
  return _open(path, a, b);
}

int _unlink_r (struct _reent * r, char *path) {

  return _unlink(path);
}

void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  rumboot_printf("assertion \"%s\" failed: file \"%s\", line %d%s%s\n",
	   failedexpr, file, line,
	   func ? ", function: " : "", func ? func : "");	
  exit(1);
}

#if 0
int _getpid_r (struct _reent * r ) {

  return _getpid();
}


int _fcntl_r  ((struct _reent *, int, int, int));
int _fork_r  ((struct _reent *));


int _kill_r  ((struct _reent *, int, int));
int _link_r  ((struct _reent *, const char *, const char *));

int _mkdir_r  ((struct _reent *, const char *, int));


int _rename_r  ((struct _reent *, const char *, const char *));

int _stat_r  ((struct _reent *, const char *, struct stat *));
_CLOCK_T_ _times_r  ((struct _reent *, struct tms *));
int _wait_r  ((struct _reent *, int *));

#endif
