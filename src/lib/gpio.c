#include <stdint.h>

#include <rumboot/gpio.h>
#include <rumboot/platform.h>

void gpio_register_ctrl(const struct rumboot_gpio_controller *new_ctrl)
{
    if (rumboot_platform_runtime_info->
        num_gpio_controllers >= RUMBOOT_PLATFORM_NUM_GPIO_CONTROLLERS) {
            rumboot_platform_panic("gpio: Maximum of GPIO controllers has reached\n");
    }
    else rumboot_platform_runtime_info->
        gpio_ctrl_dev[rumboot_platform_runtime_info->num_gpio_controllers++] = new_ctrl;
}

static void check_pin(int gpio)
{
    int ctrl_count = rumboot_platform_runtime_info->num_gpio_controllers;
    if (gpio < 0 || gpio >= GPIO_NUM_PER_CTRL * ctrl_count) {
        rumboot_platform_panic("gpio: no such pin: [%d]\n", gpio);
    }
}

static struct rumboot_gpio_controller *get_ctrl(int gpio)
{
    check_pin(gpio);
    return rumboot_platform_runtime_info->gpio_ctrl_dev[gpio / GPIO_NUM_PER_CTRL];
}

void gpio_direction(int gpio, enum gpio_direction direction)
{
    struct rumboot_gpio_controller *ctrl = get_ctrl(gpio);
    ctrl->direction(ctrl, gpio % GPIO_NUM_PER_CTRL, direction);
}

void gpio_write(int gpio, int value)
{
    struct rumboot_gpio_controller *ctrl = get_ctrl(gpio);
    ctrl->write(ctrl, gpio % GPIO_NUM_PER_CTRL, value);
}

uint32_t gpio_read(int gpio)
{
    struct rumboot_gpio_controller *ctrl = get_ctrl(gpio);
    return ctrl->read(ctrl, gpio % GPIO_NUM_PER_CTRL);
}

int gpio_IRQ_configure(int gpio, int flags)
{
    struct rumboot_gpio_controller *ctrl = get_ctrl(gpio);
    return ctrl->IRQ_configure(ctrl, gpio % GPIO_NUM_PER_CTRL, -1, flags);
}

void gpio_IRQ_enable(int gpio)
{
    struct rumboot_gpio_controller *ctrl = get_ctrl(gpio);
    ctrl->IRQ_configure(ctrl, gpio % GPIO_NUM_PER_CTRL, 1, -1);
}

void gpio_IRQ_disable(int gpio)
{
    struct rumboot_gpio_controller *ctrl = get_ctrl(gpio);
    ctrl->IRQ_configure(ctrl, gpio % GPIO_NUM_PER_CTRL, 0, -1);
}

int gpio_IRQ_get_pending(int irq)
{
    int ctrl_count = rumboot_platform_runtime_info->num_gpio_controllers;
    struct rumboot_gpio_controller *ctrl;
    for (int i = 0; i < ctrl_count; i++) {
        ctrl = rumboot_platform_runtime_info->gpio_ctrl_dev[i];
        if (ctrl->IRQ == irq) return ctrl->IRQ_get_pending(ctrl);
    }
    return -1;
}

void gpio_IRQ_clear_pending(int gpio)
{
    struct rumboot_gpio_controller *ctrl;
    ctrl = rumboot_platform_runtime_info->gpio_ctrl_dev[gpio / GPIO_NUM_PER_CTRL];
    ctrl->IRQ_clear_pending(ctrl, gpio % GPIO_NUM_PER_CTRL);
}

int gpio_to_IRQ(int gpio)
{
    struct rumboot_gpio_controller *ctrl = get_ctrl(gpio);
    return ctrl->IRQ;
}

int gpio_afsel(int gpio, int value)
{
    struct rumboot_gpio_controller *ctrl = get_ctrl(gpio);
    return ctrl->afsel(ctrl, gpio % GPIO_NUM_PER_CTRL, value);
}

int gpio_selftest(int ctrl_id)
{ 
    int ctrl_count = rumboot_platform_runtime_info->num_gpio_controllers;
    if (ctrl_id < -1 || ctrl_id > ctrl_count) {
        rumboot_platform_panic("gpio: Ivalid CTRL ID [%d], currently registered: %d",
            ctrl_id, ctrl_count);
    }

    int errors_num = 0;
    struct rumboot_gpio_controller *ctrl;
    if (ctrl_id == -1) {
        for (int i = 0; i < ctrl_count; i++) {
            ctrl = rumboot_platform_runtime_info->gpio_ctrl_dev[i];
            errors_num += ctrl->selftest(ctrl);
        }
    } else {
        ctrl = rumboot_platform_runtime_info->gpio_ctrl_dev[ctrl_id];
        errors_num += ctrl->selftest(ctrl);
    }
    return errors_num;
}
