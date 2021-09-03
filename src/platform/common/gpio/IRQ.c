#include <stdint.h>
#include <assert.h>

#include <rumboot/io.h>
#include <rumboot/gpio.h>
#include <arch/irq_macros.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <devices/gpio/pl061.h>
#include <regs/regs_gpio_pl061.h>

#define CTRL_CNT 2

static uint32_t get_bit(uintptr_t address, int bit)
{
    return ioread32(address) >> bit & 1;
}

static uintptr_t get_base(int gpio)
{
    uintptr_t ctrls[] = {GPIO_0_BASE, GPIO_1_BASE};
    return ctrls[gpio / GPIO_NUM_PER_CTRL];
}

static int get_IRQ(int gpio)
{
    int ctrls[] = {GPIO0_INT, GPIO1_INT};
    return ctrls[gpio / GPIO_NUM_PER_CTRL];
}

int main()
{
    GPIO_CONTROLLER(pl061, 0, GPIO_0_BASE, GPIO0_INT);
    GPIO_CONTROLLER(pl061, 0, GPIO_1_BASE, GPIO1_INT);

    for (int i = 0; i < GPIO_NUM_PER_CTRL * CTRL_CNT; i++) {
        assert(!gpio_selftest(-1));
        assert(gpio_to_IRQ(i) == get_IRQ(i));
    }

    for (int i = 0; i < GPIO_NUM_PER_CTRL * CTRL_CNT; i++) {
        assert(!gpio_IRQ_configure(i, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS));
        gpio_IRQ_enable(i);
        gpio_direction(i, GPIO_OUTPUT);
        gpio_write(i, 0);
    }

    for (int i = 0; i < CTRL_CNT; i++) {
        assert(ioread32(get_base(i * GPIO_NUM_PER_CTRL) + GPIO_ADDR_MASK) == 0);
        assert(gpio_IRQ_get_pending(get_IRQ(i * GPIO_NUM_PER_CTRL)) == -1);
    }

    for (int i = 0; i < GPIO_NUM_PER_CTRL * CTRL_CNT; i++) {
        gpio_write(i, 1);
        assert(gpio_IRQ_get_pending(get_IRQ(i)) == 1 << i % GPIO_NUM_PER_CTRL);
        assert(get_bit(get_base(i) + GPIO_RIS, i % GPIO_NUM_PER_CTRL) == 1);
        assert(get_bit(get_base(i) + GPIO_MIS, i % GPIO_NUM_PER_CTRL) == 1);
        gpio_IRQ_clear_pending(i);
        assert(get_bit(get_base(i) + GPIO_RIS, i % GPIO_NUM_PER_CTRL) == 0);
        assert(get_bit(get_base(i) + GPIO_MIS, i % GPIO_NUM_PER_CTRL) == 0);
        assert(gpio_IRQ_get_pending(get_IRQ(i)) == -1);
    }
    return 0;
}
