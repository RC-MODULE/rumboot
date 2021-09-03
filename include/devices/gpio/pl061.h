#ifndef PL061_H
#define PL061_H

void pl061_direction(struct rumboot_gpio_controller *self, int gpio, enum gpio_direction direction);

uint32_t pl061_read(struct rumboot_gpio_controller *self, int gpio);

void pl061_write(struct rumboot_gpio_controller *self, int gpio, int value);

int pl061_IRQ_configure(struct rumboot_gpio_controller *self, int gpio, int enable, int flags);

int pl061_IRQ_get_pending(struct rumboot_gpio_controller *self);

void pl061_IRQ_clear_pending(struct rumboot_gpio_controller *self, int gpio);

int pl061_afsel(struct rumboot_gpio_controller *self, int gpio, int value);

int pl061_selftest(struct rumboot_gpio_controller *self);

#endif
