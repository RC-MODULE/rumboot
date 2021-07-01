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
#include <devices/rcm_cp.h>
#include <rumboot/printf.h>
#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/mmu.h>
#include <rumboot/timer.h>

#define CP_STRB_RATE 10000

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
                        }
                }
                int c = rumboot_platform_getchar(0);
                if (c != -1) {
                        nfifo_write(&nf, (c & 0xff));
                }
        }
        return ret;
}

static void nmc_generate_trampoline(void *at, uint32_t ep)
{
        uint8_t *nmc_memory = (void *) at; /* The trampoline is always there */
        uint32_t *nmc_goto = (uint32_t *) &nmc_memory[4];
        nmc_memory[0] = 0x0;
        nmc_memory[1] = 0x0;
        nmc_memory[2] = 0x27; /* delayed goto */ 
        nmc_memory[3] = 0x48;
        *nmc_goto = ep;
        nmc_memory[8] = 0x0;
        nmc_memory[9] = 0x0; /* nop, nop */
        nmc_memory[10] = 0x0;
        nmc_memory[11] = 0x0;
        nmc_memory[12] = 0x0;
        nmc_memory[13] = 0x0;
        nmc_memory[14] = 0x0;
        nmc_memory[15] = 0x0;
        asm("msync");
        rumboot_printf("boot: Created trampoline to 0x%x at 0x%x\n", __swap32(*nmc_goto), at);
}

static int nmc_start(const struct rumboot_cpu_cluster *cpu, struct rumboot_bootheader *hdr, void *data, int swap)
{
    uint32_t ep = rumboot_bootimage_header_item32(hdr->entry_point[0], swap);
    if (ep) {
        nmc_generate_trampoline((void *) IM1_BASE, swap ? __swap32(ep) : ep);
    }
    dcr_write(DCR_SCTL_BASE + SCTL_NMPU_NMI, 1<<1);             
    dcr_write(DCR_SCTL_BASE + SCTL_NMPU_NMI, 0);
    return 0;
}

static void nmc_kill(const struct rumboot_cpu_cluster *cpu)
{
    dcr_write(DCR_SCTL_BASE + SCTL_NMPU_PRST, 1<<0);
    while (!(dcr_read(DCR_SCTL_BASE + SCTL_NMPU_PRST) & (1<<1)));; // Wait for ack
    dcr_write(DCR_SCTL_BASE + SCTL_NMPU_PRST, 0);
}

static void ext_kill(const struct rumboot_cpu_cluster *cpu)
{
    if (cpu->base == COM0_BASE) {
        uint32_t v = ioread32(GPIO_1_BASE + GPIO_DIR);
        iowrite32(v | 1<<0, GPIO_1_BASE + GPIO_DIR);
        iowrite32(1<<0, GPIO_1_BASE + GPIO_DATA + ((1<<0) << 2));
        udelay(100);
        iowrite32(0<<0, GPIO_1_BASE + GPIO_DATA + ((1<<0) << 2));
        udelay(100);
        iowrite32(1<<0, GPIO_1_BASE + GPIO_DATA + ((1<<0) << 2));
    }
 
    if (cpu->base == COM1_BASE) {
        uint32_t v = ioread32(GPIO_1_BASE + GPIO_DIR);
        iowrite32(v | 1<<1, GPIO_1_BASE + GPIO_DIR);
        iowrite32(1<<1, GPIO_1_BASE + GPIO_DATA + ((1<<1) << 2));
        udelay(100);
        iowrite32(0<<1, GPIO_1_BASE + GPIO_DATA + ((1<<1) << 2));
        udelay(100);
        iowrite32(1<<1, GPIO_1_BASE + GPIO_DATA + ((1<<1) << 2));
    }
}


static int ext_start(const struct rumboot_cpu_cluster *cpu, struct rumboot_bootheader *hdr, void *data, int swap)
{
    struct rcm_cp_instance cp; 
    uint32_t len = rumboot_bootimage_header_item32(hdr->datalen, swap);
    rumboot_printf("boot: Using cp @ 0x%x to transfer %d bytes\n", cpu->base, len);
    cp_instance_init(&cp, cpu->base, 100000);
    cp_set_speed(&cp, CP_STRB_RATE);

    if (len % 8) {
        return -EBADDATACRC;
    }

    cp_start_tx(&cp, data, len);
    int ret = cp_wait(&cp, 1, 1, len * 50);
    rumboot_printf("boot: cp transfer completed with code %d (%s) \n", ret, ret ? "failure" : "success");
    return 0;
}

static int ext_poll(const struct rumboot_cpu_cluster *cpu,  struct rumboot_bootheader *hdr, int swap)
{
    struct rcm_cp_instance cp; 
    int ret;
    cp_instance_init(&cp, cpu->base, 100000);
    cp_set_speed(&cp, CP_STRB_RATE);
    /* HACK: This is a hard-coded per-chip address of RX/TX buffers */
    uint64_t *buf = (uint64_t *) (IM3_BASE + IM3_SIZE - 16);
    uint8_t *charbuf = (uint8_t *) buf;
    cp_start_rx(&cp, &buf[0], 8);
    rumboot_platform_sv_event("EXT_NMC_POLL");
    rumboot_printf("boot: Using cp scratch buffers: rx:0x%x tx:0x%x\n", &buf[0], &buf[1]);
    while(true) {
            enum cp_status status = cp_rx_status(&cp);
            if ((status == CP_DONE) | (status == CP_IDLE)) {
                    uint64_t word = buf[0];

                    if (charbuf[7] & (1<<7)) { /* exit code */
                            ret = word & 0x7F;
                            break;
                    } else { /* stdio */
                            rumboot_platform_putchar(charbuf[0]);
                    }
                /* Go on for next word */
                cp_start_rx(&cp, &buf[0], 8);
            }
            status = cp_tx_status(&cp);
            if ((status == CP_DONE) | (status == CP_IDLE)) {
                int c = rumboot_platform_getchar(0);
                if (c != -1) {
                    buf[1] = 0x0;
                    charbuf[8] = c;
                    asm("msync");
                    cp_start_tx(&cp, &buf[1], 8);
                }
            }
    }
    return ret;
}


static const tlb_entry little_endian_tlb[] =
{
  /* IM0 */
 { MMU_TLB_ENTRY(  0x010,  0x80000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_1,     0b1 )},
 { MMU_TLB_ENTRY(  0x010,  0x80010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
  /* IM1 */
 { MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_3,     0b1 )},
 { MMU_TLB_ENTRY(  0x020,  0xC0010,    0x80030,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_4,     0b1 )},
  /* IM2 */
 { MMU_TLB_ENTRY(  0x020,  0xC0040,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_5,     0b1 )},
 { MMU_TLB_ENTRY(  0x020,  0xC0050,    0x80050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )},
  /* IM3 */
 { MMU_TLB_ENTRY(  0x020,  0xC0060,    0x80060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )},
 { MMU_TLB_ENTRY(  0x020,  0xC0070,    0x80070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )} 
};


static const tlb_entry big_endian_tlb[] =
{
  /* IM0 */
 { MMU_TLB_ENTRY(  0x010,  0x80000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_1,     0b1 )},
 { MMU_TLB_ENTRY(  0x010,  0x80010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
  /* IM1 */
 { MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_3,     0b1 )},
 { MMU_TLB_ENTRY(  0x020,  0xC0010,    0x80030,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_4,     0b1 )},
  /* IM2 */
 { MMU_TLB_ENTRY(  0x020,  0xC0040,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_5,     0b1 )},
 { MMU_TLB_ENTRY(  0x020,  0xC0050,    0x80050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )},
  /* IM3 */
 { MMU_TLB_ENTRY(  0x020,  0xC0060,    0x80060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )},
 { MMU_TLB_ENTRY(  0x020,  0xC0070,    0x80070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )} 
};


int rumboot_ppc_jump_to_ep_with_args(const struct rumboot_cpu_cluster *cpu,  struct rumboot_bootheader *hdr, void *data, int swap) 
{
    int (*runme)(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e); 
	uint32_t ep0 = rumboot_bootimage_header_item32(hdr->entry_point[0], swap);
	uint32_t ep1 = rumboot_bootimage_header_item32(hdr->entry_point[1], swap);
    set_mem_window(MEM_WINDOW_SHARED);

#if UINTPTR_MAX > 0xFFFFFFFF
		runme = (void *) ((ep0) | (uint64_t) (ep1) << 32);
#else
		runme = (void *) ep0;
#endif

    if (swap) {
        write_tlb_entries(little_endian_tlb, ARRAY_SIZE(little_endian_tlb));
    }

    int ret = runme(
		rumboot_bootimage_header_item32(hdr->bootargs[0], swap),
		rumboot_bootimage_header_item32(hdr->bootargs[1], swap),
		rumboot_bootimage_header_item32(hdr->bootargs[2], swap),
		rumboot_bootimage_header_item32(hdr->bootargs[3], swap),
		rumboot_bootimage_header_item32(hdr->bootargs[4], swap)
		);

    if (swap) {
            write_tlb_entries(big_endian_tlb, ARRAY_SIZE(big_endian_tlb));
    }

    return ret;

}

static const struct rumboot_cpu_cluster cpus[] = {
    {
        .name  = "PowerPC",
        .start = rumboot_ppc_jump_to_ep_with_args,
    },
    {
        .name = "NMRISC",
        .kill = nmc_kill,
        .poll = nmc_poll,
        .start = nmc_start,
    },
    {
        .base = COM0_BASE,
        .name = "External NMC @ CP0",
        .kill = ext_kill,
        .poll = ext_poll,
        .start = ext_start,        
    },
    {
        .base = COM1_BASE,
        .name = "External NMC @ CP1",
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
