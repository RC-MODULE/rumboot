#ifndef RCM_H
#define RCM_H

void rcm_direction(struct rumboot_gpio_controller *self, int gpio, enum gpio_direction direction);

uint32_t rcm_read(struct rumboot_gpio_controller *self, int gpio);

void rcm_write(struct rumboot_gpio_controller *self, int gpio, int value);

int rcm_IRQ_configure(struct rumboot_gpio_controller *self, int gpio, int enable, int flags);

int rcm_IRQ_get_pending(struct rumboot_gpio_controller *self);

void rcm_IRQ_clear_pending(struct rumboot_gpio_controller *self, int gpio);

int rcm_afsel(struct rumboot_gpio_controller *self, int gpio, int value);

int rcm_selftest(struct rumboot_gpio_controller *self);

#endif
