#include <stdint.h>

#include <rumboot/gpio.h>
#include <rumboot/io.h>
#include <rumboot/regpoker.h>
#include <regs/regs_gpio_rcm.h>
#include <arch/irq_macros.h>

#define EVENT_OFFSET 2  // (some hack)
/* GPIO_IRQ_MASK
    [
      line 0 [ edge_pos | edge_neg | event_bit | reserved ]
      line 1 [ edge_pos | edge_neg | event_bit | reserved ]
      ...
    ]
*/

enum irq_event { EDGE_POS, EDGE_NEG };

static struct regpoker_checker default_array[] = {
    {"GPIO_ID", REGPOKER_READ32, GPIO_ID, GPIO_ID_VALUE, 0xFFFFFFFF},
    {"GPIO_VERSION", REGPOKER_READ32, GPIO_VERSION, GPIO_VERSION_VALUE, 0xFFFFFFFF},
    { /* Sentinel */ }
};

static int get_sector(int gpio) { return gpio * 4; }

static int get_event_bit(int gpio)
{
    return get_sector(gpio) + EVENT_OFFSET;
}

static enum irq_event get_event(uint32_t mask, int gpio)
{
    return (mask >> get_event_bit(gpio)) & 1;
}

static void reg_write(uintptr_t address, int gpio, int value)
{
    uint32_t current_value = ioread32(address);
    if (value == 0) current_value &= ~(1 << gpio);
    else current_value |= 1 << gpio;
    iowrite32(current_value, address);
}

void rcm_direction(struct rumboot_gpio_controller *self, int gpio, enum gpio_direction direction)
{
    reg_write(self->base + GPIO_PAD_DIR, gpio, direction);
}

uint32_t rcm_read(struct rumboot_gpio_controller *self, int gpio)
{
    return (ioread32(self->base + GPIO_RD_DATA) >> gpio) & 1;
}

void rcm_write(struct rumboot_gpio_controller *self, int gpio, int value)
{
    reg_write(self->base + GPIO_WR_DATA, gpio, value);
}

int rcm_IRQ_configure(struct rumboot_gpio_controller *self, int gpio, int enable, int flags)
{
    /* check EVENT BIT to know which MASK BIT to set / clear */
    uint32_t mask = ioread32(self->base + GPIO_IRQ_MASK);
    enum irq_event event = get_event(mask, gpio);
    int sector = get_sector(gpio);
    int mask_bit = sector + event;
    if (enable == 0 || enable == 1) {
        if (enable) mask |= 1 << mask_bit;
        else mask &= ~(1 << mask_bit);
        iowrite32(mask, self->base + GPIO_IRQ_MASK);
        return 0;
    }

    int new_event = 0;
    switch (flags) {
        case RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS:
            new_event = 0;
            break;
        case RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_NEG:
            new_event = 1;
            break;
        default:
            return -1;
    }
    if (new_event == event) return 0;
    if ((mask >> mask_bit) & 1) {
        int new_mask_bit = sector + (1 - event);
        mask &= ~(1 << mask_bit); // clear current MASK BIT
        mask |= 1 << (new_mask_bit);  // set new MASK BIT (left / right neighbour)
    }
    /* change EVENT BIT */
    if (new_event) mask |= 1 << get_event_bit(gpio);
    else mask &= ~(1 << get_event_bit(gpio));

    iowrite32(mask, self->base + GPIO_IRQ_MASK);
    return 0;
}

int rcm_IRQ_get_pending(struct rumboot_gpio_controller *self)
{
    uint32_t status = ioread32(self->base + GPIO_STATUS);
    if (!status) return -1;
    int bitmask = 0;
    for (int i = 0; i < GPIO_NUM_PER_CTRL; i++) {
        if (status & 1 || (status >> 1) & 1) bitmask |= 1 << i;
        status >>= GPIO_NUM_PER_CTRL / 2;
    }
    return bitmask;
}

void rcm_IRQ_clear_pending(struct rumboot_gpio_controller *self, int gpio)
{
    ioread32(self->base + GPIO_STATUS);
}

int rcm_afsel(struct rumboot_gpio_controller *self, int gpio, int value)
{
    if (value == 0 || value == 1) {
        reg_write(self->base + GPIO_SWITCH_SOURCE, gpio, 1 - value);
        return 0;
    }
    return -1;
}

int rcm_selftest(struct rumboot_gpio_controller *self)
{
    return rumboot_regpoker_check_array(default_array, self->base);
}
