#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <rumboot/rumboot.h>
#include <stdlib.h>
#include <devices/pl310.h>
#include <rumboot/io.h>
#include <string.h>


/*
 * A very ugly L2 cache test with lots of copy-paste and other hardcode. 
 * HAZARD: Please put on your hazard protection suite before going any
 * 		   futher
 * 
 */

#define ISB	asm volatile ("isb sy" : : : "memory")
#define DSB	asm volatile ("dsb sy" : : : "memory")
#define DMB	asm volatile ("dmb sy" : : : "memory")

#define isb()	ISB
#define dsb()	DSB
#define dmb()   DMB


#if 0

#define ARMV7_DCACHE_INVAL_RANGE	1
#define ARMV7_DCACHE_CLEAN_INVAL_RANGE	2

/* Asm functions from cache_v7_asm.S */
void v7_flush_dcache_all(void);
void v7_invalidate_dcache_all(void);

//FixMe: From nowhere
#define CONFIG_SYS_CACHELINE_SIZE 128



/* Values for Ctype fields in CLIDR */
#define ARMV7_CLIDR_CTYPE_NO_CACHE		0
#define ARMV7_CLIDR_CTYPE_INSTRUCTION_ONLY	1
#define ARMV7_CLIDR_CTYPE_DATA_ONLY		2
#define ARMV7_CLIDR_CTYPE_INSTRUCTION_DATA	3
#define ARMV7_CLIDR_CTYPE_UNIFIED	4

/* CCSIDR */
#define CCSIDR_LINE_SIZE_OFFSET		0
#define CCSIDR_LINE_SIZE_MASK		0x7
#define CCSIDR_ASSOCIATIVITY_OFFSET	3
#define CCSIDR_ASSOCIATIVITY_MASK	(0x3FF << 3)
#define CCSIDR_NUM_SETS_OFFSET		13
#define CCSIDR_NUM_SETS_MASK		(0x7FFF << 13)


/* read L2 control register (L2CTLR) */
static inline uint32_t read_l2ctlr(void)
{
	uint32_t val = 0;

	asm volatile ("mrc p15, 1, %0, c9, c0, 2" : "=r" (val));

	return val;
}

/* write L2 control register (L2CTLR) */
static inline void write_l2ctlr(uint32_t val)
{
	/*
	 * Note: L2CTLR can only be written when the L2 memory system
	 * is idle, ie before the MMU is enabled.
	 */
	asm volatile("mcr p15, 1, %0, c9, c0, 2" : : "r" (val) : "memory");
	isb();
}


int check_cache_range(unsigned long start, unsigned long stop)
{
	int ok = 1;

	if (start & (CONFIG_SYS_CACHELINE_SIZE - 1))
		ok = 0;

	if (stop & (CONFIG_SYS_CACHELINE_SIZE - 1))
		ok = 0;

	if (!ok) {
		warn_non_spl("CACHE: Misaligned operation at range [%08lx, %08lx]\n",
			     start, stop);
	}

	return ok;
}


static uint32_t get_ccsidr(void)
{
	uint32_t ccsidr;

	/* Read current CP15 Cache Size ID Register */
	asm volatile ("mrc p15, 1, %0, c0, c0, 0" : "=r" (ccsidr));
	return ccsidr;
}

static void v7_dcache_clean_inval_range(uint32_t start, uint32_t stop, uint32_t line_len)
{
	uint32_t mva;

	/* Align start to cache line boundary */
	start &= ~(line_len - 1);
	for (mva = start; mva < stop; mva = mva + line_len) {
		/* DCCIMVAC - Clean & Invalidate data cache by MVA to PoC */
		asm volatile ("mcr p15, 0, %0, c7, c14, 1" : : "r" (mva));
	}
}

static void v7_dcache_inval_range(uint32_t start, uint32_t stop, uint32_t line_len)
{
	uint32_t mva;

	if (!check_cache_range(start, stop))
		return;

	for (mva = start; mva < stop; mva = mva + line_len) {
		/* DCIMVAC - Invalidate data cache by MVA to PoC */
		asm volatile ("mcr p15, 0, %0, c7, c6, 1" : : "r" (mva));
	}
}

static void v7_dcache_maint_range(uint32_t start, uint32_t stop, uint32_t range_op)
{
	uint32_t line_len, ccsidr;

	ccsidr = get_ccsidr();
	line_len = ((ccsidr & CCSIDR_LINE_SIZE_MASK) >>
			CCSIDR_LINE_SIZE_OFFSET) + 2;
	/* Converting from words to bytes */
	line_len += 2;
	/* converting from log2(linelen) to linelen */
	line_len = 1 << line_len;

	switch (range_op) {
	case ARMV7_DCACHE_CLEAN_INVAL_RANGE:
		v7_dcache_clean_inval_range(start, stop, line_len);
		break;
	case ARMV7_DCACHE_INVAL_RANGE:
		v7_dcache_inval_range(start, stop, line_len);
		break;
	}

	/* DSB to make sure the operation is complete */
	dsb();
}


void invalidate_dcache_all(void)
{
	v7_invalidate_dcache_all();

	v7_outer_cache_inval_all();
}

/*
 * Performs a clean & invalidation of the entire data cache
 * at all levels
 */
void flush_dcache_all(void)
{
	v7_flush_dcache_all();

	v7_outer_cache_flush_all();
}

/*
 * Invalidates range in all levels of D-cache/unified cache used:
 * Affects the range [start, stop - 1]
 */
void invalidate_dcache_range(unsigned long start, unsigned long stop)
{
	check_cache_range(start, stop);

	v7_dcache_maint_range(start, stop, ARMV7_DCACHE_INVAL_RANGE);

	v7_outer_cache_inval_range(start, stop);
}

/*
 * Flush range(clean & invalidate) from all levels of D-cache/unified
 * cache used:
 * Affects the range [start, stop - 1]
 */
void flush_dcache_range(unsigned long start, unsigned long stop)
{
	check_cache_range(start, stop);

	v7_dcache_maint_range(start, stop, ARMV7_DCACHE_CLEAN_INVAL_RANGE);

	v7_outer_cache_flush_range(start, stop);
}

void arm_init_before_mmu(void)
{
	v7_outer_cache_enable();
	invalidate_dcache_all();
	v7_inval_tlb();
}

void mmu_page_table_flush(unsigned long start, unsigned long stop)
{
	flush_dcache_range(start, stop);
	v7_inval_tlb();
}



/* Invalidate entire I-cache and branch predictor array */
void invalidate_icache_all(void)
{
	/*
	 * Invalidate all instruction caches to PoU.
	 * Also flushes branch target cache.
	 */
	asm volatile ("mcr p15, 0, %0, c7, c5, 0" : : "r" (0));

	/* Invalidate entire branch predictor array */
	asm volatile ("mcr p15, 0, %0, c7, c5, 6" : : "r" (0));

	/* Full system DSB - make sure that the invalidation is complete */
	dsb();

	/* ISB - make sure the instruction stream sees it */
	isb();
}


#endif



#define clrbits(addr, clear) \
	iowrite32(ioread32(addr) & ~(clear), addr)

#define setbits(addr, set) \
	iowrite32(ioread32(addr) | (set), addr)

void armv7_outer_cache_enable(struct pl310_regs *const pl310)
{
	unsigned int val, cache_id;

	/*
	 * Must disable the L2 before changing the latency parameters
	 * and auxiliary control register.
	 */
//	clrbits(&pl310->pl310_ctrl, L2X0_CTRL_EN);

	/*
	 * Set bit 22 in the auxiliary control register. If this bit
	 * is cleared, PL310 treats Normal Shared Non-cacheable
	 * accesses as Cacheable no-allocate.
	 */
	//setbits(&pl310->pl310_aux_ctrl, L310_SHARED_ATT_OVERRIDE_ENABLE);
//
	//iowrite32(0x132, &pl310->pl310_tag_latency_ctrl);
	//iowrite32(0x132, &pl310->pl310_data_latency_ctrl);

//	val = ioread32(&pl310->pl310_prefetch_ctrl);

	/* Turn on the L2 I/D prefetch, double linefill */
	/* Set prefetch offset with any value except 23 as per errata 765569 */
//	val |= 0x7000000f;

	/*
	 * The L2 cache controller(PL310) version on the i.MX6D/Q is r3p1-50rel0
	 * The L2 cache controller(PL310) version on the i.MX6DL/SOLO/SL/SX/DQP
	 * is r3p2.
	 * But according to ARM PL310 errata: 752271
	 * ID: 752271: Double linefill feature can cause data corruption
	 * Fault Status: Present in: r3p0, r3p1, r3p1-50rel0. Fixed in r3p2
	 * Workaround: The only workaround to this erratum is to disable the
	 * double linefill feature. This is the default behavior.
	 */

	/**
	cache_id = ioread32(&pl310->pl310_cache_id);
	if (((cache_id & L2X0_CACHE_ID_PART_MASK) == L2X0_CACHE_ID_PART_L310)
	    && ((cache_id & L2X0_CACHE_ID_RTL_MASK) < L2X0_CACHE_ID_RTL_R3P2))
		val &= ~(1 << 30);
	iowrite32(val, &pl310->pl310_prefetch_ctrl);
	*/

//	val = ioread32(&pl310->pl310_power_ctrl);
//	val |= L2X0_DYNAMIC_CLK_GATING_EN;
//	val |= L2X0_STNDBY_MODE_EN;
//	iowrite32(val, &pl310->pl310_power_ctrl);

	iowrite32(ioread32(&pl310->pl310_ctrl) | L2X0_CTRL_EN, &pl310->pl310_ctrl);
}


struct armv7_page_flags {
	uint32_t permissions;
	uint32_t domain;
	uint32_t bufcache;
	uint32_t descriptor;
};

#define ARM_TLB_SIZE 16 * 1024
uint32_t *armv7_tlb_allocate() {
	uint32_t *tbl = rumboot_malloc_from_heap_aligned(0, 16*1024, 16*1024);
	memset(tbl, 0x0, ARM_TLB_SIZE);
}

void armv7_tlb_map(uint32_t *tlb, uint32_t phys, uint32_t virt, struct armv7_page_flags *flags)
{
	uint32_t page_id = (virt & 0xfff00000) >> 20;
	uint32_t value = phys & 0xfff00000;	
	if (flags) {
		value |= (flags->permissions & 0x3) << 10;
		value |= (flags->domain & 0xf) << 5;
		value |= (flags->bufcache & 0x3) << 2;
	} else {
		value |= 0x3 << 10;
	}
	value |= 0b10;
	tlb[page_id] = value;
}

void armv7_inval_tlb(void)
{
	/* Invalidate entire unified TLB */
	asm volatile ("mcr p15, 0, %0, c8, c7, 0" : : "r" (0));
	/* Invalidate entire data TLB */
	asm volatile ("mcr p15, 0, %0, c8, c6, 0" : : "r" (0));
	/* Invalidate entire instruction TLB */
	asm volatile ("mcr p15, 0, %0, c8, c5, 0" : : "r" (0));
	/* Full system DSB - make sure that the invalidation is complete */
	dsb();
	/* Full system ISB - make sure the instruction stream sees it */
	isb();
}

void armv7_enable_mmu()
{
	
}



void armv7_tlb_drop(uint32_t *tlb, uint32_t page_id) {
	tlb[page_id] = 0x0;
}


/* Short-Descriptor Translation Table Level 1 Bits */
#define TTB_SECT_NS_MASK	(1 << 19)
#define TTB_SECT_NG_MASK	(1 << 17)
#define TTB_SECT_S_MASK		(1 << 16)
/* Note: TTB AP bits are set elsewhere */
#define TTB_SECT_AP		(3 << 10)
#define TTB_SECT_TEX(x)		((x & 0x7) << 12)
#define TTB_SECT_DOMAIN(x)	((x & 0xf) << 5)
#define TTB_SECT_XN_MASK	(1 << 4)
#define TTB_SECT_C_MASK		(1 << 3)
#define TTB_SECT_B_MASK		(1 << 2)
#define TTB_SECT			(2 << 0)

/* options available for data cache on each page */
enum dcache_option {
	DCACHE_OFF = TTB_SECT_DOMAIN(0) | TTB_SECT_XN_MASK | TTB_SECT,
	DCACHE_WRITETHROUGH = DCACHE_OFF | TTB_SECT_C_MASK,
	DCACHE_WRITEBACK = DCACHE_WRITETHROUGH | TTB_SECT_B_MASK,
	DCACHE_WRITEALLOC = DCACHE_WRITEBACK | TTB_SECT_TEX(1),
};

#define TTBR0_BASE_ADDR_MASK	0xFFFFC000
#define TTBR0_RGN_NC			(0 << 3)
#define TTBR0_RGN_WBWA			(1 << 3)
#define TTBR0_RGN_WT			(2 << 3)
#define TTBR0_RGN_WB			(3 << 3)
/* TTBR0[6] is IRGN[0] and TTBR[0] is IRGN[1] */
#define TTBR0_IRGN_NC			(0 << 0 | 0 << 6)
#define TTBR0_IRGN_WBWA			(0 << 0 | 1 << 6)
#define TTBR0_IRGN_WT			(1 << 0 | 0 << 6)
#define TTBR0_IRGN_WB			(1 << 0 | 1 << 6)
#define MMU_SECTION_SHIFT 20

#define CR_M		(1 << 0)	/* MMU enable			*/
#define CR_A		(1 << 1)	/* Alignment abort enable	*/
#define CR_C		(1 << 2)	/* Dcache enable		*/
#define CR_SA		(1 << 3)	/* Stack Alignment Check Enable	*/
#define CR_I		(1 << 12)	/* Icache enable		*/
#define CR_WXN		(1 << 19)	/* Write Permision Imply XN	*/
#define CR_EE		(1 << 25)	/* Exception (Big) Endian	*/



unsigned int get_cr(void)
{
	unsigned int val;

	asm volatile("mrc p15, 0, %0, c1, c0, 0	@ get CR" : "=r" (val)
							  :
							  : "cc");
	return val;
}

void set_cr(unsigned int val)
{
	asm volatile("mcr p15, 0, %0, c1, c0, 0	@ set CR" :
							  : "r" (val)
							  : "cc");
	isb();
}



unsigned int get_ttbcr0(void)
{
	unsigned int val;

	asm volatile("mrc p15, 0, %0, c2, c0, 2" : "=r" (val)
							  :
							  : "cc");
	return val;
}

void set_ttbcr0(unsigned int reg)
{
	asm volatile("mcr p15, 0, %0, c2, c0, 0"
		     : : "r" (reg) : "memory");
}



void armv7_tlb_activate(void *tlb) {
	int i;
	uint32_t tlbaddr = (uint32_t) tlb;
	uint32_t reg;

//	arm_init_before_mmu();
#if 1
	/* Set TTBCR to disable LPAE */
	asm volatile("mcr p15, 0, %0, c2, c0, 2"
		: : "r" (0) : "memory");

	/* Set TTBR0 */
	reg = tlbaddr & TTBR0_BASE_ADDR_MASK;
#if defined(CONFIG_SYS_ARM_CACHE_WRITETHROUGH)
	reg |= TTBR0_RGN_WT | TTBR0_IRGN_WT;
#elif defined(CONFIG_SYS_ARM_CACHE_WRITEALLOC)
	reg |= TTBR0_RGN_WBWA | TTBR0_IRGN_WBWA;
#else
	reg |= TTBR0_RGN_WB | TTBR0_IRGN_WB;
#endif
	asm volatile("mcr p15, 0, %0, c2, c0, 0"
		     : : "r" (reg) : "memory");
#else
	/* Copy the page table address to cp15 */
	asm volatile("mcr p15, 0, %0, c2, c0, 0"
		     : : "r" (tlbaddr) : "memory");
#endif
	/* Set the access control to all-supervisor */
	asm volatile("mcr p15, 0, %0, c3, c0, 0"
		     : : "r" (~0));

	/* and enable the mmu */
	reg = get_cr();	/* get control reg. */
	set_cr(reg | CR_M);
}

#include <rumboot/hexdump.h>

void set_section_dcache(void *tlb, int section, enum dcache_option option)
{
	uint32_t *page_table = (uint32_t *)tlb;
	uint32_t value = TTB_SECT_AP;

	/* Add the page offset */
	value |= ((uint32_t)section << MMU_SECTION_SHIFT);

	/* Add caching bits */
	value |= option;

	/* Set PTE */
	page_table[section] = value;
}

#define PL310_TAG_RAM_LATENCY ((1 << 8) | (1 << 4) | (1 << 0))
#define PL310_DATA_RAM_LATENCY ((1 << 8) | (2 << 4) | (1 << 0))


/* registers */
#define REG0_CACHE_ID              0x000
#define REG0_CACHE_TYPE            0x004
#define REG1_CONTROL               0x100
#define REG1_AUX_CONTROL           0x104
#define REG1_TAG_RAM_CONTROL       0x108
#define REG1_DATA_RAM_CONTROL      0x10c
#define REG2_EV_COUNTER_CTRL       0x200
#define REG2_EV_COUNTER1_CFG       0x204
#define REG2_EV_COUNTER0_CFG       0x208
#define REG2_EV_COUNTER1           0x20c
#define REG2_EV_COUNTER0           0x210
#define REG2_INT_MASK              0x214
#define REG2_INT_MASK_STATUS       0x218
#define REG2_INT_RAW_STATUS        0x21c
#define REG2_INT_CLEAR             0x220
#define REG7_CACHE_SYNC            0x730
#define REG7_INV_PA                0x770
#define REG7_INV_WAY               0x77c
#define REG7_CLEAN_PA              0x7b0
#define REG7_CLEAN_INDEX           0x7b8
#define REG7_CLEAN_WAY             0x7bc
#define REG7_CLEAN_INV_PA          0x7f0
#define REG7_CLEAN_INV_INDEX       0x7f8
#define REG7_CLEAN_INV_WAY         0x7fc
#define REG9_D_LOCKDOWN0           0x900
#define REG9_I_LOCKDOWN0           0x904
#define REG9_D_LOCKDOWN1           0x908
#define REG9_I_LOCKDOWN1           0x90c
#define REG9_D_LOCKDOWN2           0x910
#define REG9_I_LOCKDOWN2           0x914
#define REG9_D_LOCKDOWN3           0x918
#define REG9_I_LOCKDOWN3           0x91c
#define REG9_D_LOCKDOWN4           0x920
#define REG9_I_LOCKDOWN4           0x924
#define REG9_D_LOCKDOWN5           0x928
#define REG9_I_LOCKDOWN5           0x92c
#define REG9_D_LOCKDOWN6           0x930
#define REG9_I_LOCKDOWN6           0x934
#define REG9_D_LOCKDOWN7           0x938
#define REG9_I_LOCKDOWN7           0x93c
#define REG9_LOCK_LINE_EN          0x950
#define REG9_UNLOCK_WAY            0x954
#define REG12_ADDR_FILTERING_START 0xc00
#define REG12_ADDR_FILTERING_END   0xc04
#define REG15_DEBUG_CTRL           0xf40
#define REG15_PREFETCH_CTRL        0xf60
#define REG15_POWER_CTRL 0xf80

#define PL310_REG(reg) (* (volatile uint32_t* ) (base + reg))
void pl310_init(uintptr_t base)
{
    /* set tag and data ram latency */
    PL310_REG(REG1_TAG_RAM_CONTROL) = PL310_TAG_RAM_LATENCY;
    PL310_REG(REG1_DATA_RAM_CONTROL) = PL310_DATA_RAM_LATENCY;

    /* configure */
    /* early BRESP enable, instruction/data prefetch, exclusive cache, full line of zero */
	PL310_REG(REG1_AUX_CONTROL) &= ~((1<<29) | (1<<28) | (0b11 << 23) | (1<<25));
    PL310_REG(REG1_AUX_CONTROL) |= (0b10 << 23) | (1<<30)|(0<<29)|(0<<28)|(1<<12)|(1<<0);

    /* flush all the ways */
    PL310_REG(REG7_INV_WAY) = 0xffff;
}

int pl310_set_enable(uintptr_t base, bool enable)
{
    if (enable) {
        if ((PL310_REG(REG1_CONTROL) & 1) == 0) {
            /* if disabled */
            pl310_invalidate();
            PL310_REG(REG1_CONTROL) = 1;
        }
    } else {
        if ((PL310_REG(REG1_CONTROL) & 1) == 1) {
            /* if enabled */
            pl310_flush_invalidate();
            PL310_REG(REG1_CONTROL) = 0;
            /* this seems to not always latch on the first try */
            while (PL310_REG(REG1_CONTROL) & 1) {
                PL310_REG(REG1_CONTROL) = 0;
            }
        }
    }

    return 0;
}


void pl310_invalidate(uintptr_t base)
{
    PL310_REG(REG7_INV_WAY) = 0xffff;
    while (PL310_REG(REG7_INV_WAY) != 0)
        ;
}

void pl310_flush_invalidate(uintptr_t base)
{
    PL310_REG(REG7_CLEAN_INV_WAY) = 0xffff;
    while (PL310_REG(REG7_CLEAN_INV_WAY) != 0)
        ;
}



#define AREA_SIZE (256*1024)
void timed_memtest(uintptr_t start, size_t sz)
{
	int i; 
	uint32_t tm_start = rumboot_platform_get_uptime();
	int err = 0;
	for (i=0; i<1; i++)
		err += memtest(start, sz);
	uint32_t tm_end = rumboot_platform_get_uptime();
	rumboot_printf("Addr: %x Test took %d us, %d errors\n", 
	start, tm_end - tm_start, err);
}

void timed_test_once(char *tag)
{
	rumboot_printf("-- %s --\n", tag);
	timed_memtest(0x80000000, AREA_SIZE);
	timed_memtest(0x80000000, AREA_SIZE);	
}

#define CACHE_SIZE (256 * 1024 / 4)
void check_cached_access(volatile uint32_t *cached, volatile uint32_t *uncached)
{
	rumboot_printf("-- cached %x uncached %x --\n", cached, uncached);
#if 0
	*uncached = 0xdeadc0de;
	if (*uncached == *cached) {
		rumboot_printf("Cache miss, got valid data\n");
	}
	*uncached = 0xbadc0de1;
	if (*cached == 0xdeadc0de) {
		rumboot_printf("Cached data successfully\n");		
	}
	*cached = 0xb00bc0de;
	if (*uncached == *cached) {
		rumboot_printf("Data propagated to backend storage\n");
	}
	rumboot_printf("[%x %x]\n", *cached, *uncached);
#endif
	int i; 
	for (i=0; i< CACHE_SIZE; i++) {
		uncached[i] = 0;
	}

	for (i=0; i< CACHE_SIZE; i++) {
		cached[i] = i + 1;
	}
	int moved = 0; 
	for (i=0; i < CACHE_SIZE; i++) {
		if (cached[i] == uncached[i]) {
			moved++;
		}
	}
	rumboot_printf("%d bytes evicted test\n", moved * 4);
}

#if 0
int main()
{
	rumboot_printf("Hi there, I'll now fire up the mmu\n");
	memset(0x80000000, 0x0, 0x100);
	uint32_t reg = get_cr();	/* get control reg. */
	set_cr(reg & ~(CR_C));
	timed_test_once("UNCACHED");
	set_cr(reg | (CR_C));
	timed_test_once("L1 CACHE");
	
	rumboot_printf("%x\n", ioread32(L2C_BASE));
	void *tlb = armv7_tlb_allocate();
	rumboot_printf("L1 Page Table at 0x%x\n", tlb);


	rumboot_printf("Invalidating TLB\n");
	armv7_inval_tlb();

	rumboot_printf("Enabling outer cache\n");

	pl310_init(L2C_BASE);
	pl310_set_enable(L2C_BASE, 1);

	timed_test_once("AFTER L2C_INIT");
	//armv7_outer_cache_enable((void *) L2C_BASE);


	rumboot_printf("Activating MMU\n");
	/* Set up an identity-mapping for all 4GB, rw for everyone */

	struct armv7_page_flags f;
	memset(&f, 0x0, sizeof(f));
	f.bufcache=0x3;
	f.permissions=0x3;

	uint32_t i;
	armv7_tlb_map(tlb, 0x0, 0x0, NULL);
	armv7_tlb_map(tlb, 0x01044000, 0x01044000, NULL);

	f.bufcache=0x0;
	f.bufcache++;
	armv7_tlb_map(tlb, 0x80000000, 0x80000000, &f);
	f.bufcache++;
	armv7_tlb_map(tlb, 0x80000000, 0x90000000, &f);
	f.bufcache++;
	armv7_tlb_map(tlb, 0x80000000, 0x10000000, &f);
	f.bufcache++;
	armv7_tlb_map(tlb, 0x80000000, 0x11000000, &f);

	armv7_tlb_map(tlb, 0x0, 0xff000000, &f);
	rumboot_printf("GO!\n");
	armv7_tlb_activate(tlb);

	timed_test_once("AFTER TLB SETUP");

	reg = get_cr();	/* get control reg. */
	set_cr(reg | CR_C);

	timed_test_once("AFTER CACHE ENABLE");

	timed_memtest(0x80000000, AREA_SIZE);
	timed_memtest(0x90000000, AREA_SIZE);
	timed_memtest(0x10000000, AREA_SIZE);
	timed_memtest(0x11000000, AREA_SIZE);

	//check_cached_access((void *) 0x90000000, (void *) 0x80000000);
	check_cached_access((void *) 0x10000000, (void *) 0x80000000);
	//check_cached_access((void *) 0x11000000, (void *) 0x80000000);
	return 0;
}

#else 
int main() {
	rumboot_printf("Hi there, I'll now fire up the mmu\n");
	uint32_t reg = get_cr();	/* get control reg. */
	set_cr(reg | (CR_C));
	
	void *tlb = armv7_tlb_allocate();
	armv7_inval_tlb();
	pl310_init(L2C_BASE);
	pl310_set_enable(L2C_BASE, 1);

	struct armv7_page_flags f;
	memset(&f, 0x0, sizeof(f));
	f.bufcache=0x0;
	f.permissions=0x3;

	uint32_t i;
	armv7_tlb_map(tlb, 0x0, 0x0, &f);
	armv7_tlb_map(tlb, 0x01044000, 0x01044000, &f);

	f.bufcache=0x0;
//	armv7_tlb_map(tlb, 0x80000000, 0x80000000, &f);
	f.bufcache++;
//	armv7_tlb_map(tlb, 0x80000000, 0x90000000, &f);
	f.bufcache++;
//	armv7_tlb_map(tlb, 0x80000000, 0x10000000, &f);
	f.bufcache++;


	//ADDITIONAL CACHED ROM MAPPING 
	armv7_tlb_map(tlb, 0x00000000, 0x11000000, &f);

	//DDR MAPPING 
	//armv7_tlb_map(tlb, 0x80000000, 0x11000000, &f);

	armv7_tlb_map(tlb, 0x0, 0xff000000, &f);
	armv7_tlb_activate(tlb);

	reg = get_cr();	/* get control reg. */
	set_cr(reg & ~CR_C);
//	check_cached_access((void *) 0x11000000, (void *) 0x80000000);
	i = 100;
	while (i--) {
		timed_memtest(0x11000000, CACHE_SIZE);
	}
	return 0;
}
#endif