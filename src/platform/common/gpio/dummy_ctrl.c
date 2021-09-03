#include <stdint.h>
#include <assert.h>

#include <rumboot/gpio.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>

#include <devices/gpio/dummy.h>

static void check_single_bit_set(int gpio)
{
    uint32_t current_pin_state;
    uint32_t predict;
    for (int i = 0; i < GPIO_NUM_PER_CTRL; i++) {
        current_pin_state = gpio_read(i);
        if (i == gpio || i == gpio + GPIO_NUM_PER_CTRL / 2) predict = 1;
        else predict = 0;
        assert(current_pin_state == predict);
    }
}

int main()
{
    GPIO_CONTROLLER(dummy, 0, 0);
    for (int i = 0; i < GPIO_NUM_PER_CTRL; i++) {
        gpio_direction(i, 1);
        gpio_write(i, 0);
    }
    for (int i = 0; i < GPIO_NUM_PER_CTRL / 2; i++) {
        gpio_write(i, 1);
        check_single_bit_set(i);
        gpio_write(i, 0);
    }
    return 0;
}
