#ifndef RUMBOOT_GPIO_H
#define RUMBOOT_GPIO_H

#define GPIO_NUM_PER_CTRL 8

#define GPIO_CONTROLLER(name, _offset, _base, _irq) { \
    static const struct rumboot_gpio_controller name ## _ctrl = { \
        .base = _base + _offset, \
        .IRQ = _irq, \
        .direction = name ## _direction, \
        .read = name ## _read, \
        .write = name ## _write, \
        .IRQ_configure = name ## _IRQ_configure, \
        .IRQ_get_pending = name ## _IRQ_get_pending, \
        .IRQ_clear_pending = name ## _IRQ_clear_pending, \
        .afsel = name ## _afsel, \
        .selftest = name ## _selftest, \
    }; \
    gpio_register_ctrl(& name ## _ctrl); \
}

enum gpio_direction { GPIO_INPUT, GPIO_OUTPUT };

struct rumboot_gpio_controller {

    uintptr_t base;

    uint32_t IRQ;

    uint32_t capabilities;

    void (*direction)(struct rumboot_gpio_controller *self, int gpio,
        enum gpio_direction direction);

    void (*write)(struct rumboot_gpio_controller *self, int gpio, int value);

    uint32_t (*read)(struct rumboot_gpio_controller *self, int gpio);

    int (*IRQ_configure)(struct rumboot_gpio_controller *self, int gpio, int enable, int flags);

    int (*IRQ_get_pending)(struct rumboot_gpio_controller *self);

    void (*IRQ_clear_pending)(struct rumboot_gpio_controller *self, int gpio);

    int (*afsel)(struct rumboot_gpio_controller *self, int gpio, int value);

    int (*selftest)(struct rumboot_gpio_controller *self);
};

void gpio_register_ctrl(const struct rumboot_gpio_controller *new_ctrl);

void gpio_direction(int gpio, enum gpio_direction direction);

uint32_t gpio_read(int gpio);

void gpio_write(int gpio, int value);

int gpio_IRQ_configure(int gpio, int flags);

void gpio_IRQ_enable(int gpio);

void gpio_IRQ_disable(int gpio);

int gpio_IRQ_get_pending(int irq);

void gpio_IRQ_clear_pending(int gpio);

int gpio_to_IRQ(int gpio);

int gpio_afsel(int gpio, int value);

int gpio_selftest(int ctrl_id);

#endif
