#include <stdint.h>
#include <assert.h>

#include <rumboot/io.h>
#include <rumboot/gpio.h>
#include <platform/interrupts.h>
#include <regs/regs_gpio_pl061.h>
#include <devices/gpio/pl061.h>
#include <devices/gpio/dummy.h>
#include <platform/devices.h>

#define CTRL_NUM 2

enum reg { DIRECTION, AFSEL };

static uintptr_t get_base(int gpio)
{
    uintptr_t ctrls[] = {GPIO_0_BASE, GPIO_1_BASE};
    return ctrls[gpio / GPIO_NUM_PER_CTRL];
}

static uint32_t get_bit(uintptr_t address, int bit)
{
    return ioread32(address) >> bit & 1;
}

static void check_reg(int bit, enum reg reg_to_check)
{
    uintptr_t offset;
    switch (reg_to_check) {
        case DIRECTION:
            offset = GPIO_DIR;
            for (int i = 0; i < CTRL_NUM; i++) {
                gpio_direction(i, bit);
                assert(get_bit(get_base(i) + offset, i % GPIO_NUM_PER_CTRL) == bit);
            }
            break;
        case AFSEL:
            offset = GPIO_AFSEL;
            for (int i = 0; i < CTRL_NUM; i++) {
                assert(gpio_afsel(i, bit) == 0);
                assert(get_bit(get_base(i) + offset, i % GPIO_NUM_PER_CTRL) == bit);
                assert(gpio_afsel(i, bit + 2) == -1);
            }
            break;
        default:
            return;
    }
}

static void check_read_write(int bit)
{
    for (int i = 0; i < CTRL_NUM; i++) {
        gpio_write(i, bit);
        uint32_t expect = get_bit(get_base(i) + GPIO_ADDR_MASK, i % GPIO_NUM_PER_CTRL);
        assert(expect == bit);
        assert(expect == gpio_read(i));
    }
}

int main()
{
    GPIO_CONTROLLER(pl061, 0, GPIO_0_BASE, GPIO0_INT);
    GPIO_CONTROLLER(pl061, 0, GPIO_1_BASE, GPIO1_INT);
    GPIO_CONTROLLER(dummy, 0, 0, 0);
    GPIO_CONTROLLER(dummy, 0, 1, 0);

    for (int i = 0; i < 4; i++) {
        assert(!gpio_selftest(i));
    }

    for (int i = 0; i < 2; i++) {
        check_reg(i, DIRECTION);
        check_reg(i, AFSEL);
    }

    for (int i = 0; i < 2; i++) {
        check_read_write(i);
    }

    return 0;
}
