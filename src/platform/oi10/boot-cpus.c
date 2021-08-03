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
    }
};


const struct rumboot_cpu_cluster *rumboot_platform_get_cpus(int *cpu_count)
{
    *cpu_count = ARRAY_SIZE(cpus);
    return cpus;
}
