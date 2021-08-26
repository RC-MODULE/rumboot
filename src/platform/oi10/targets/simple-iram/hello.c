#include <rumboot/printf.h>
#include <algo/crc8.h>

int main()
{
    rumboot_printf("Hello world from IRAM!\n");
    int a = 123456789;
    int my_crc = crc8(0, &a, sizeof(a));
    rumboot_printf("hello: CRC = 0x%X\n", my_crc);
    return 0;
}
