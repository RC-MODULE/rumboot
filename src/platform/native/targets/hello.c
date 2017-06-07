#include <rumboot/printf.h>
int main()
{
    rumboot_print_logo();
    rumboot_printf("Hello world: %d !\n", 1);
    return 0;
}
