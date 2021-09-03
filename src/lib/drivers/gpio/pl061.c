#include <stdint.h>

#include <rumboot/gpio.h>
#include <rumboot/io.h>
#include <rumboot/regpoker.h>
#include <platform/devices.h>
#include <regs/regs_gpio_pl061.h>
#include <arch/irq_macros.h>

static struct regpoker_checker default_array[] = {
    {"GPIO_PeriphID0", REGPOKER_READ32, GPIO_PeriphID0, GPIO_PeriphID0_DEFAULT, GPIO_REG_MASK},
    {"GPIO_PeriphID1", REGPOKER_READ32, GPIO_PeriphID1, GPIO_PeriphID1_DEFAULT, GPIO_REG_MASK},
    {"GPIO_PeriphID2", REGPOKER_READ32, GPIO_PeriphID2, GPIO_PeriphID2_DEFAULT, GPIO_REG_MASK},
    {"GPIO_PeriphID3", REGPOKER_READ32, GPIO_PeriphID3, GPIO_PeriphID3_DEFAULT, GPIO_REG_MASK},
    {"GPIO_CellID0", REGPOKER_READ32, GPIO_CellID0, GPIO_CellID0_DEFAULT, GPIO_REG_MASK},
    {"GPIO_CellID1", REGPOKER_READ32, GPIO_CellID1, GPIO_CellID1_DEFAULT, GPIO_REG_MASK},
    {"GPIO_CellID2", REGPOKER_READ32, GPIO_CellID2, GPIO_CellID2_DEFAULT, GPIO_REG_MASK},
    {"GPIO_CellID3", REGPOKER_READ32, GPIO_CellID3, GPIO_CellID3_DEFAULT, GPIO_REG_MASK},
    { /* Sentinel */ },
};

static void reg_write(uintptr_t address, int gpio, int value)
{
    uint32_t current_value = ioread32(address);
    if (value == 0) current_value &= ~(1 << gpio);
    else current_value |= 1 << gpio;
    iowrite32(current_value, address);
}

static int get_IRQ_sense_and_event(int flags, int *sense, int *event)
{
    switch (flags) {
        case RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS:
            *sense = 0;
            *event = 1;
            break;
        case RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_NEG:
            *sense = 0;
            *event = 0;
            break;
        case RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH:
            *sense = 1;
            *event = 1;
            break;
        case RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_LOW:
            *sense = 1;
            *event = 0;
            break;
        default:
            return -1;
    }
    return 0;
}

void pl061_direction(struct rumboot_gpio_controller *self, int gpio, enum gpio_direction direction)
{
    reg_write(self->base + GPIO_DIR, gpio, direction);
}

uint32_t pl061_read(struct rumboot_gpio_controller *self, int gpio)
{
    return (ioread32(self->base + GPIO_ADDR_MASK) >> gpio) & 1;
}

void pl061_write(struct rumboot_gpio_controller *self, int gpio, int value)
{
    reg_write(self->base + GPIO_ADDR_MASK, gpio, value);
}

int pl061_IRQ_configure(struct rumboot_gpio_controller *self, int gpio, int enable, int flags)
{
    if (enable == 0 || enable == 1) {
        reg_write(self->base + GPIO_IE, gpio, enable);
        return 0;
    }

    int sense, event;
    int error = get_IRQ_sense_and_event(flags, &sense, &event);
    if (error != 0) return error;
    reg_write(self->base + GPIO_IS, gpio, sense);
    reg_write(self->base + GPIO_IEV, gpio, event);
    return 0;
}

int pl061_IRQ_get_pending(struct rumboot_gpio_controller *self)
{
    uint32_t status = ioread32(self->base + GPIO_MIS);
    if (!status) return -1;
    return status;
}

void pl061_IRQ_clear_pending(struct rumboot_gpio_controller *self, int gpio)
{
    reg_write(self->base + GPIO_IC, gpio, 1);
}

int pl061_afsel(struct rumboot_gpio_controller *self, int gpio, int value)
{
    if (value == 0 || value == 1) {
        reg_write(self->base + GPIO_AFSEL, gpio, value);
        return 0;
    }
    return -1;
}

int pl061_selftest(struct rumboot_gpio_controller *self)
{
    return rumboot_regpoker_check_array(default_array, self->base);
}
