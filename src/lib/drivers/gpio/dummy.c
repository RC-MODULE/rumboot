#include <stdint.h>

#include <rumboot/gpio.h>
#include <rumboot/platform.h>

static uint32_t get_state(struct rumboot_gpio_controller *self)
{
    return rumboot_platform_runtime_info->persistent[self->base];
}

static void set_state(struct rumboot_gpio_controller *self, int value)
{
    rumboot_platform_runtime_info->persistent[self->base] = value;
}

uint32_t dummy_read(struct rumboot_gpio_controller *self, int gpio)
{
    return get_state(self) >> gpio & 1;
}

static void state_write(struct rumboot_gpio_controller *self, int gpio, int value)
{
    if (value == 0) set_state(self, get_state(self) & ~(1 << gpio));
    else set_state(self, get_state(self) | 1 << gpio);
}

static void reg_write(struct rumboot_gpio_controller *self, int gpio, int value)
{
    state_write(self, gpio, value);
    rumboot_printf("dummy_driver: Pin [%d] state is: %d\n", gpio, value);
}

void dummy_write(struct rumboot_gpio_controller *self, int gpio, int value)
{
    if (dummy_read(self, gpio + GPIO_NUM_PER_CTRL) == 1) {
        reg_write(self, gpio, value);
        int opposite_pin;
        if (gpio < GPIO_NUM_PER_CTRL / 2) opposite_pin = gpio + (GPIO_NUM_PER_CTRL / 2);
        else opposite_pin = gpio - (GPIO_NUM_PER_CTRL / 2);
        if (dummy_read(self, opposite_pin + GPIO_NUM_PER_CTRL)) reg_write(self, opposite_pin, value);
    } else rumboot_printf("dummy_driver: Can\'t set pin [%d], direction is INPUT\n", gpio);
}

void dummy_direction(struct rumboot_gpio_controller *self, int gpio, enum gpio_direction direction)
{
    state_write(self, gpio + GPIO_NUM_PER_CTRL, direction);
    rumboot_printf("dummy_driver: Direction of pin [%d] is: %d\n", gpio, direction);
}

int dummy_afsel(struct rumboot_gpio_controller *self, int gpio, int value)
{
    state_write(self, gpio + 2 * GPIO_NUM_PER_CTRL, value);
    rumboot_printf("dummy_driver: Function of pin [%d] is: %d\n", gpio, value);
    return 0;
}

int dummy_selftest(struct rumboot_gpio_controller *self) { return 0; }

int dummy_IRQ_configure(struct rumboot_gpio_controller *self) { return 0; }

int dummy_get_IRQ(struct rumboot_gpio_controller *self) { return 0; }

int dummy_IRQ_get_pending(struct rumboot_gpio_controller *self) { return 0; }

void dummy_IRQ_clear_pending(struct rumboot_gpio_controller *self) { /* Empty for a while */  }
