#ifndef GPIO_DUMMY_H
#define GPIO_DUMMY_H

void dummy_direction(struct rumboot_gpio_controller *self, int gpio, enum gpio_direction direction);

uint32_t dummy_read(struct rumboot_gpio_controller *self, int gpio);

void dummy_write(struct rumboot_gpio_controller *self, int gpio, int value);

int dummy_IRQ_configure(struct rumboot_gpio_controller *self, int gpio, int enable, int flags);

int dummy_IRQ_get_pending(struct rumboot_gpio_controller *self);

void dummy_IRQ_clear_pending(struct rumboot_gpio_controller *self);

int dummy_afsel(struct rumboot_gpio_controller *self, int gpio, int value);

int dummy_selftest(struct rumboot_gpio_controller *self);

#endif
