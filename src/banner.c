#include <rumboot/printf.h>
static __attribute__((constructor)) void announce_testname()
{
    rumboot_printf("=== Running test: " RUMBOOT_PRODUCT_NAME " === \n");
} 