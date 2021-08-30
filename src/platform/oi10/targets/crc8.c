#include <stdint.h>
#include <stddef.h>
#include <rumboot/rumboot.h>
#include <rumboot/platform.h>
#include <algo/crc8.h>
#include <assert.h>

int main()
{
    size_t buff_sz = 32 * sizeof(uint8_t);
    uint8_t hw = 0, tb = 0;
    uint8_t *buff = (uint8_t *) rumboot_malloc(buff_sz);
    for (int i = 0; i < buff_sz; i++) {
        buff[i] = random_num + i;
    }
    rumboot_platform_dump_region("buff.bin", buff, buff_sz);
    hw = crc8_hw(hw, buff, buff_sz);
    tb = crc8(tb, buff, buff_sz);
    assert(hw == tb);
    return 0;
}
