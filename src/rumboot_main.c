#include <rumboot/platform.h>

extern int main();
void rumboot_main()
{
    /*
     * This is common initialization code needed to bring up proper C environment.
     * It does the following:
     * 1. Initialize the global runtime table with sane initial values
     * 2. Initialize bss section with zeroes (if we're using the BSS)
     * 3. Calls main() function
     */
     /* TODO */
     uint32_t *ptr = 0xdeadc0de;
     *ptr = 0x1;
     main();
}
