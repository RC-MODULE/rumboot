#include <stdint.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <regs/regs_gpio_pl061.h>
#include <regs/regs_uart_pl011.h>
#include <devices/uart_pl011.h>
#include <rumboot/bootsrc/physmap.h>
#include <rumboot/timer.h>


void rumboot_platform_read_config(struct rumboot_config *conf)
{
        conf->baudrate = 115200;
        conf->selftest = -1;
        conf->edcl = -1; /* EDCL Always on */
        conf->hostmode = 0;
}


void rumboot_platform_init_loader(struct rumboot_config *conf)
{

}


static const struct rumboot_bootsource bootsrc[] = {
        { /*Sentinel*/ }
};


void rumboot_platform_enter_host_mode(struct rumboot_config *conf)
{

}

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}

const struct rumboot_bootsource *rumboot_platform_get_bootsources()
{
        return bootsrc;
}

bool rumboot_platform_check_entry_points(struct rumboot_bootheader *hdr)
{
        /* Any entry point is okay */
        return true;
}

int rumboot_platform_exec(struct rumboot_bootheader *hdr, int swap)
{
        return rumboot_bootimage_execute_ep((void *)hdr->entry_point);
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{

}


int rumboot_platform_selftest(struct rumboot_config *conf)
{
        return 0;
}


#if 0

void __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t c)
{
        while (uart_check_tfifo_full(UART0_Base));;
        iowrite32(c, UART0_Base + UARTDR);
}


int rumboot_platform_getchar(uint32_t timeout_us)
{
        if (is_silent()) {
                udelay(timeout_us);
                return -1;
        }

        uint32_t start = rumboot_platform_get_uptime();

        while (rumboot_platform_get_uptime() - start < timeout_us) {
                if (!uart_check_rfifo_empty(UART0_Base)) {
                        return ioread32(UART0_Base + UARTDR) & 0xFF;
                }
        }
        return -1;
}
#endif
