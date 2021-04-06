#include <rumboot/boot.h>
#include <rumboot/macros.h>
#include <rumboot/platform.h>
#include <devices/nfifo.h>
#include <platform/devices.h>
#include <platform/regs/sctl.h>
#include <arch/ppc_476fp_lib_c.h>


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

static void nmc_start(const struct rumboot_cpu_cluster *cpu, struct rumboot_bootheader *hdr, int swap)
{
    dcr_write(DCR_SCTL_BASE + SCTL_NMPU_NMI, 1<<1);                
    dcr_write(DCR_SCTL_BASE + SCTL_NMPU_NMI, 0);
}

static void nmc_kill(const struct rumboot_cpu_cluster *cpu)
{
    dcr_write(DCR_SCTL_BASE + SCTL_NMPU_PRST, 1<<0); //Issue reset                
    while (!(dcr_read(DCR_SCTL_BASE + SCTL_NMPU_PRST) & (1<<1)));; // Wait for ack
}


#if 0

struct ecom_instance {
    uintptr_t base;
    bool     swap_endian;

    uint64_t *rxbuf;
    uint64_t *txbuf;
};

void ecom_instance_init(struct ecom_settings *inst, uintptr_t base)
{

}

void ecom_send(struct ecom_instance *inst, const void *buf, size_t len)
{

}

void ecom_recv(struct ecom_instance *inst, void *buf, size_t len)
{

}

#endif

static void ext_kill(const struct rumboot_cpu_cluster *cpu)
{
    /* TODO: Issue a GPIO reset */    
    if (cpu->base == COM0_BASE) {
        /* TODO: ... */
    }
 
    if (cpu->base == COM1_BASE) {
        /* TODO: ... */
    }
}

static void ext_start(const struct rumboot_cpu_cluster *cpu,  struct rumboot_bootheader *hdr, int swap)
{
    /* TODO: Send payload via COMx */
}

static void ext_poll(const struct rumboot_cpu_cluster *cpu,  struct rumboot_bootheader *hdr, int swap)
{
    /* TODO: Same as poll, but via COMx port */
}

static const struct rumboot_cpu_cluster cpus[] = {
    {
        .name  = "PowerPC"
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
