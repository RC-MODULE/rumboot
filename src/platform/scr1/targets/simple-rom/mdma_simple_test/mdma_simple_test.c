#include <stdint.h>
#include <stdlib.h>

#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <regs/regs_mdma.h>
#include <devices/mdma_simple.h>

int main() {
    int ret;

    void* addr_src;
    void* addr_dst;

    uint32_t dump_size = 64;
    uint32_t data_size = 32;

    rumboot_printf("mdma_simple_test\n");

    addr_src = rumboot_malloc_from_heap(1,dump_size);
    addr_dst = rumboot_malloc_from_heap(1,data_size);

    rumboot_platform_request_file("myfile", (uint32_t)addr_src);
    rumboot_platform_dump_region("mydump", (uint32_t)addr_src, dump_size);

    ret = simple_mdma_exec (1, addr_src, addr_dst, data_size);

    if (ret) rumboot_printf("TEST FAILED\n");
    else rumboot_printf("TEST PASSED\n");

    return ret;
}