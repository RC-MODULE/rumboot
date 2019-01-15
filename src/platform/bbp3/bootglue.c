#include <stdint.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsrc/eeprom.h>
#include <rumboot/bootsrc/boilerplate.h>
#include <rumboot/bootsrc/sdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>

//#define BOOTM_SELFTEST     (1 << 0)
//#define BOOTM_HOST         (1 << 1)
//#define BOOTM_FASTUART     (1 << 2)
//#define BOOTM_SDIO0_CD     (1 << 3)
//#define BOOTM_SDIO1_CD     (1 << 4)
//#define BOOTM_SPI1_CS1     (1 << 5)

void rumboot_platform_read_config(struct rumboot_config *conf)
{
//        uint32_t bootm = ioread32(SCTL_BASE + SCTL_BOOTM);
//
//        if (bootm & BOOTM_FASTUART) {
//                conf->baudrate = 6250000;
//        } else {
//                conf->baudrate = 115200;
//        }
//
//        conf->hostmode = (bootm & BOOTM_HOST);
//        conf->selftest = (bootm & BOOTM_SELFTEST);
}


static inline int get_cpsr()
{
    int result = 0;
    asm volatile (
    "mrs %0, cpsr\n" : "=r" (result) );
    return result;
}

__attribute__( ( always_inline ) ) static inline uint32_t __get_APSR(void)
{
  uint32_t result;

  asm volatile ("MRS %0, apsr" : "=r" (result) );
  return(result);
}

__attribute__( ( always_inline ) ) static inline  void set_cpsr(uint32_t cpsr)
{
  asm volatile ("MSR cpsr, %0" : : "r" (cpsr) : "memory");
}

static __attribute__( ( always_inline ) ) uint32_t __get_FP_usr(void)
{
  uint32_t cpsr = get_cpsr();
  uint32_t result;
  asm volatile(
//    "CPS     #0x13  \n"
    "MOV     %0, fp   " : "=r"(result) : : "memory"
   );
  set_cpsr(cpsr);
  asm volatile ("isb 0xF":::"memory");
  return result;
}


void print_backtrace(void)
{
  uint32_t topfp = (int) __get_FP_usr();
    for (int i=0; i < 32; i++) {
        uint32_t pos;
        uint32_t fp = *(((uint32_t*)topfp) -3);
        uint32_t sp = *(((uint32_t*)topfp) -2);
        uint32_t lr = *(((uint32_t*)topfp) -1);
        uint32_t pc = *(((uint32_t*)topfp) -0);


        pos = lr - 4;


        rumboot_printf("frame[%d] address: 0x%x (FP: 0x%x SP: 0x%x LR: 0x%x PC: 0x%x)\n",
        i, pos, fp, sp, lr, pc);
        if (fp == 0)
          break;
        if ((fp < (uint32_t)&rumboot_platform_stack_area_start) ||
            (fp > (uint32_t)&rumboot_platform_stack_area_end)) {
                rumboot_printf("Next frame looks invalid, we'll stop here, sorry\n");
                break;
        }
        topfp = fp;
    }
}

static void exception_handler(int id, const char *name)
{
        rumboot_printf("\n\n\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
        rumboot_printf("--- Guru Meditation ---\n");
        rumboot_printf("  -   Registers     -\n");
        rumboot_printf("CPSR: 0x%x \n", get_cpsr());
        rumboot_printf("APSR: 0x%x \n", __get_APSR());
        rumboot_printf("VBAR: 0x%x \n", arm_vbar_get());
        rumboot_printf("  -   Stack trace   -\n");
        print_backtrace();
        rumboot_printf("---       ---       ---\n");
        rumboot_platform_sv_event("EXCEPTION");
        rumboot_platform_panic("Please reset or power-cycle the board\n");
}

void rumboot_platform_init_loader(struct rumboot_config *conf)
{
//        muart_write_char(UART0_BASE, 0x55);
        rumboot_irq_set_exception_handler(exception_handler);
}


static bool spi0_1_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

//        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
//        v |= 1 << 5;
//        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
//        iowrite32((1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET1);
        return true;
}

static void spi0_1_disable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

//        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
//        v &= ~(1 << 5);
//        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
}

static void spi0_1_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
//        if (!select) {
//                iowrite32(~(1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET0);
//        } else {
//                iowrite32((1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET1);
//        }
}

static const struct rumboot_bootsource arr[] = {
        {
                .name = "SPI0 (CS: ???)",
                .base = SPI0_Base,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_spiflash,
                .enable = spi0_1_enable,
                .disable = spi0_1_disable,
                .chipselect = spi0_1_cs,
        },
        { /*Sentinel*/ }
};


void rumboot_platform_enter_host_mode()
{
        uint32_t v;

//        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
//        v |= BOOTM_HOST;
//        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
//        iowrite32(BOOTM_HOST, GPIO0_BASE + GPIO_WR_DATA_SET1);
}

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}

const struct rumboot_bootsource *rumboot_platform_get_bootsources()
{
        return arr;
}

bool rumboot_platform_check_entry_points(struct rumboot_bootheader *hdr)
{
        /* Any entry point is okay */
        return true;
}

int rumboot_platform_exec(struct rumboot_bootheader *hdr)
{
        return rumboot_bootimage_execute_ep((void *)hdr->entry_point[0]);
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{
}


int rumboot_platform_selftest(struct rumboot_config *conf)
{
  return 0;
}


#ifndef CMAKE_BUILD_TYPE_DEBUG
void __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t ch)
{
//        while ((ioread32(UART0_BASE + MUART_FIFO_STATE) & 0x7ff0000) >= 0x3ff0000);
//        iowrite32(ch, UART0_BASE + MUART_DTRANS);
}


int rumboot_platform_getchar(uint32_t timeout_us)
{
//        uint32_t start = rumboot_platform_get_uptime();
//
//        while (rumboot_platform_get_uptime() - start < timeout_us) {
//                if ((ioread32(UART0_BASE + MUART_FIFO_STATE)) & 0xfff) {
//                        return ioread32(UART0_BASE + MUART_DREC);
//                }
//        }
//        return -1;
}
#endif
