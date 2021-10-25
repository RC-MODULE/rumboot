#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


uintptr_t rumboot_native_request_device(int dev_id, uint32_t addr, size_t len)
{
    size_t pagesize = sysconf(_SC_PAGE_SIZE);
    off_t page_base = (addr / pagesize) * pagesize;
    off_t page_offset = addr - page_base;

    int fd = -1;
    char *name = NULL;
    asprintf(&name, "/dev/mmappcimem-%d", dev_id);
    if (!name) {
        rumboot_platform_panic("asprintf failed ;(");       
    }

    if ((fd = open(name, O_RDWR|O_SYNC)) < 0) {
        rumboot_platform_panic("Failed to open %s", name);       
    }
        
    volatile void* ctx = mmap(
        NULL,                   // Any adddress in our space will do
        page_offset + len,                    // Map length
        PROT_READ|PROT_WRITE,   // Enable reading & writting to mapped memory
        MAP_SHARED,             // Shared with other processes
        fd,                     // File to map
        page_base                    // Offset to peripheral
    );

    close(fd);

    if (ctx == MAP_FAILED) {
        rumboot_platform_panic("mmap failed");
    }    

    return (uintptr_t) ctx;
}

void rumboot_native_free_device(int id) {

}


