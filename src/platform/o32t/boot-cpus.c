#include <rumboot/boot.h>
#include <rumboot/macros.h>
#include <rumboot/platform.h>
#include <devices/nfifo.h>
#include <platform/devices.h>
#include <platform/regs/sctl.h>
#include <arch/ppc_476fp_lib_c.h>
#include <devices/gpio_pl061.h>
#include <regs/regs_gpio_pl061.h>
#include <regs/regs_uart_pl011.h>

static int nmc_poll(const struct rumboot_cpu_cluster *cpu)
{
        struct nfifo_instance nf;
        int ret = -1; 
        nfifo_instance_init(&nf, DCR_APBFIFO_BASE);
        nfifo_enable(&nf, 1);
        while(true) {
                if (nfifo_can_read(&nf)) {
                        uint32_t word = nfifo_read(&nf, 0); /* Non-blocking */
                        if (word & (1<<31)) { /* exit code */
                                ret = word & 0xff;
                                break;
                        } else { /* stdio */
                                uint8_t ch = (uint8_t) word & 0xff;
                                rumboot_platform_putchar(ch);
                                if (ch == '\n') {
                                }
                        }
                }
                int c = rumboot_platform_getchar(0);
                if (c != -1) {
                        nfifo_write(&nf, (c & 0xff));
                }
        }
        return ret;
}

static int nmc_start(const struct rumboot_cpu_cluster *cpu, struct rumboot_bootheader *hdr, void *data, int swap)
{
    uint32_t ep = rumboot_bootimage_header_item32(hdr->entry_point[0], swap);
    if (ep) {
        uint8_t *nmc_memory = (void *) IM1_BASE; /* The trampoline is always there */
        uint32_t *nmc_goto = &nmc_memory[4];
        nmc_memory[0] = 0x0;
        nmc_memory[1] = 0x0;
        nmc_memory[2] = 0x27; /* delayed goto */ 
        nmc_memory[3] = 0x48;
        *nmc_goto = swap ? __swap32(ep) : ep;
        nmc_memory[8] = 0x0;
        nmc_memory[9] = 0x0; /* nop, nop */
        nmc_memory[10] = 0x0;
        nmc_memory[11] = 0x0;
        nmc_memory[12] = 0x0;
        nmc_memory[13] = 0x0;
        nmc_memory[14] = 0x0;
        nmc_memory[15] = 0x0;
        rumboot_printf("nmc: Created trampoline to %x at reset vector location\n", nmc_goto);
    }
    dcr_write(DCR_SCTL_BASE + SCTL_NMPU_NMI, 1<<1);                
    dcr_write(DCR_SCTL_BASE + SCTL_NMPU_NMI, 0);
    return 0;
}

static void nmc_kill(const struct rumboot_cpu_cluster *cpu)
{
    dcr_write(DCR_SCTL_BASE + SCTL_NMPU_PRST, 1<<0); //Issue reset                
    while (!(dcr_read(DCR_SCTL_BASE + SCTL_NMPU_PRST) & (1<<1)));; // Wait for ack
}


#if 0
struct ecom_instance {
    uintptr_t base;
    bool      swap_endian;
};


void ecom_instance_init(struct ecom_settings *inst, uintptr_t base)
{
    inst->base = base;
}

void ecom_send(struct ecom_instance *inst, const void *buf, size_t len)
{
	iowrite32( len,  inst->base + MainCounter_rcv ); //set dma total data 512 byte
	iowrite32( (uint32_t) buf, inst->base + Address_rcv ); //dma destination atart address
	iowrite32( 0x0,  inst->base + Bias_rcv );
	iowrite32( 0x0,  inst->base + RowCounter_rcv );
	iowrite32( 0x0,  inst->base + AddressMode_rcv );
    
}

void ecom_recv(struct ecom_instance *inst, void *buf, size_t len)
{

}

#endif

static void ext_kill(const struct rumboot_cpu_cluster *cpu)
{
    if (cpu->base == COM0_BASE) {
        uint32_t v = ioread32(GPIO_1_BASE + GPIO_DIR);
        iowrite32(v | 1<<0, GPIO_1_BASE + GPIO_DIR);
        iowrite32(1<<0, GPIO_1_BASE + GPIO_DATA + ((1<<0) << 2));
    }
 
    if (cpu->base == COM1_BASE) {
        uint32_t v = ioread32(GPIO_1_BASE + GPIO_DIR);
        iowrite32(v | 1<<1, GPIO_1_BASE + GPIO_DIR);
        iowrite32(1<<1, GPIO_1_BASE + GPIO_DATA + ((1<<1) << 2));
    }
}

static int ext_start(const struct rumboot_cpu_cluster *cpu,  struct rumboot_bootheader *hdr, int swap)
{

}

static int ext_poll(const struct rumboot_cpu_cluster *cpu,  struct rumboot_bootheader *hdr, int swap)
{

}

static const struct rumboot_cpu_cluster cpus[] = {
    {
        .name  = "PowerPC",
        .start = rumboot_bootimage_jump_to_ep_with_args,
    },
    {
        .name = "NMRISC",
        .kill = nmc_kill,
        .poll = nmc_poll,
        .start = nmc_start,
    },
    {
        .base = COM0_BASE,
        .name = "External NMC (COM0)",
        .kill = ext_kill,
        .poll = ext_poll,
        .start = ext_start,        
    },
    {
        .base = COM1_BASE,
        .name = "External NMC (COM1)",
        .kill = ext_kill,
        .poll = ext_poll,
        .start = ext_start,        
    }
};

const struct rumboot_cpu_cluster *rumboot_platform_get_cpus(int *cpu_count)
{
    *cpu_count = ARRAY_SIZE(cpus);
    return cpus;
}
