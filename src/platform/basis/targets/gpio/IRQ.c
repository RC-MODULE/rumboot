#include <stdint.h>
#include <assert.h>

#include <rumboot/gpio.h>
#include <arch/irq_macros.h>
#include <platform/interrupts.h>
#include <platform/devices.h>
#include <devices/gpio/rcm.h>

#define CTRL_CNT 2
#define PIN_CNT 3

static int get_IRQ(int gpio)
{
    int ctrls[] = {GPIO1_INTR, GPIO2_INTR};
    return ctrls[gpio / GPIO_NUM_PER_CTRL];
}

int main()
{
    GPIO_CONTROLLER(rcm, 0, GPIO1_BASE, GPIO1_INTR);
    GPIO_CONTROLLER(rcm, 0, GPIO2_BASE, GPIO2_INTR);

    assert(gpio_selftest(-1) == 0);
    for (int i = 0; i < GPIO_NUM_PER_CTRL * CTRL_CNT; i++) assert(gpio_to_IRQ(i) == get_IRQ(i));

    int input_pin, output_pin;
    for (int i = 0; i < PIN_CNT; i++) {
        input_pin = i + GPIO_NUM_PER_CTRL;
        output_pin = i;
        assert(!gpio_IRQ_configure(input_pin, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS));
        gpio_IRQ_disable(input_pin);
    }

    assert(gpio_IRQ_get_pending(GPIO2_INTR) == -1);

    for (int i = 0; i < PIN_CNT; i++) {
        input_pin = i + GPIO_NUM_PER_CTRL;
        output_pin = i;
        gpio_direction(output_pin, GPIO_OUTPUT);
        gpio_write(output_pin, 0);
        gpio_direction(input_pin, GPIO_INPUT);
        assert(gpio_read(input_pin) == 0);
        gpio_write(output_pin, 1);
        assert(gpio_read(input_pin) == 1);
        gpio_write(output_pin, 0);
        assert(gpio_read(input_pin) == 0);
    }
    assert(gpio_IRQ_get_pending(GPIO2_INTR) == 0b111);
    return 0;
}
