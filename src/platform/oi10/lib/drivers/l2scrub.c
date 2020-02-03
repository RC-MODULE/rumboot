#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <stdio.h>
#include <inttypes.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/interrupts.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <math.h>
#include <arch/l2scrub.h>

#define DEBUG
#ifdef DEBUG
#define dbg(fmt, ...) rumboot_printf("l2-scrubber: " fmt, ##__VA_ARGS__)
#else
#define dbg(fmt, ...)
#endif


/* Used for initial lookup tables generation */
static volatile int bit_id;
static volatile int inject_index;


static const char tag_ecc_fix_lookup[255] = {
    [176] = 3,
    [112] = 4,
    [28] = 5,
    [50] = 6,
    [56] = 7,
    [26] = 8,
    [208] = 9,
    [42] = 10,
    [168] = 11,
    [104] = 12,
    [44] = 13,
    [164] = 14,
    [100] = 15,
    [38] = 16,
    [52] = 17,
    [88] = 18,
    [196] = 19,
    [84] = 20,
    [76] = 21,
    [98] = 22,
    [82] = 23,
    [74] = 24,
    [70] = 25,
    [200] = 26,
    [148] = 27,
    [140] = 28,
    [194] = 29,
    [162] = 30,
    [152] = 31,
    [2] = 65,
    [4] = 66,
    [8] = 67,
    [16] = 68,
    [32] = 69,
    [64] = 70,
    [128] = 71,
};

static const char data_ecc_fix_lookup[255] = {
    [1] = 64,
    [2] = 65,
    [4] = 66,
    [8] = 67,
    [16] = 68,
    [32] = 69,
    [64] = 70,
    [128] = 71,
    [37] = 0,
    [35] = 1,
    [67] = 2,
    [69] = 3,
    [121] = 4,
    [131] = 5,
    [133] = 6,
    [137] = 7,
    [74] = 8,
    [70] = 9,
    [134] = 10,
    [138] = 11,
    [242] = 12,
    [7] = 13,
    [11] = 14,
    [19] = 15,
    [148] = 16,
    [140] = 17,
    [13] = 18,
    [21] = 19,
    [229] = 20,
    [14] = 21,
    [22] = 22,
    [38] = 23,
    [41] = 24,
    [25] = 25,
    [26] = 26,
    [42] = 27,
    [203] = 28,
    [28] = 29,
    [44] = 30,
    [76] = 31,
    [82] = 32,
    [50] = 33,
    [52] = 34,
    [84] = 35,
    [151] = 36,
    [56] = 37,
    [88] = 38,
    [152] = 39,
    [164] = 40,
    [100] = 41,
    [104] = 42,
    [168] = 43,
    [47] = 44,
    [112] = 45,
    [176] = 46,
    [49] = 47,
    [73] = 48,
    [200] = 49,
    [208] = 50,
    [81] = 51,
    [94] = 52,
    [224] = 53,
    [97] = 54,
    [98] = 55,
    [146] = 56,
    [145] = 57,
    [161] = 58,
    [162] = 59,
    [188] = 60,
    [193] = 61,
    [194] = 62,
    [196] = 63,    
};


#define IBIT(x) (1 << (31-x))


static void l2_correct_arrstat2(struct l2_scrubber *scr)
{
    uint32_t arrstat2 = l2c_l2_read(DCR_L2C_BASE, L2C_L2ARRSTAT2);
    uint32_t log0     = l2c_l2_read(scr->dcr_base, L2C_L2LOG0); 
    uint32_t log1     = l2c_l2_read(scr->dcr_base, L2C_L2LOG1);
    uint32_t log2     = l2c_l2_read(scr->dcr_base, L2C_L2LOG2);
    uint32_t log3     = l2c_l2_read(scr->dcr_base, L2C_L2LOG3);
    uint32_t log4     = l2c_l2_read(scr->dcr_base, L2C_L2LOG4);
    uint32_t log5     = l2c_l2_read(scr->dcr_base, L2C_L2LOG5);
    uint32_t ferr     = l2c_l2_read(scr->dcr_base, L2C_L2FERR);

    dbg("LOG REGS: LOG0: 0x%x LOG1: 0x%x LOG2: 0x%x LOG3: 0x%x LOG4: 0x%x LOG5: 0x%x FERR: 0x%x\n", 
        log0,
        log1,
        log2,
        log3,
        log4,
        log5,
        ferr 
        );

    int failbit = ffs(arrstat2) - 1;
    if (failbit < 8) { 
        int failword = 7-(failbit);
        uint32_t failarraddr = (log0 << 3) | failword;
        scr->lastfailaddr = failarraddr;
        int bit = log1 >> 16;
        bit = data_ecc_fix_lookup[bit];
        if (bit<32) {
            log3 ^= (1<< bit);
            l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA, failarraddr, (((uint64_t) log2) << 32 | log3));
            scr->lastfailmem = L2C_MEM_DATA;
            scr->lastfailbit = bit;
        } else if (bit <64) {
            log2 ^= (1<< (bit - 32));
            l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA, failarraddr, (((uint64_t) log2) << 32 | log3));
            scr->lastfailmem = L2C_MEM_DATA;
            scr->lastfailbit = bit;
        } else {
            log1 ^= (1<<(bit - 64));
            l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA_ECC, failarraddr, log1 & 0xff);
            scr->lastfailmem = L2C_MEM_DATA_ECC;
            scr->lastfailbit = bit;
        }
        l2c_l2_write(scr->dcr_base, L2C_L2ARRSTAT2, arrstat2);
        scr->corrected_data_errors++;
        dbg("Corrected cache data bitflip at index %d\n", failarraddr);
    } else if (failbit < 12) {
        uint32_t way = 3 - (failbit - 8);
        uint32_t failarraddr = (way << 8) | (log0 >> 1);

        rumboot_printf("failaddr %d(way: %d addr: %d) must be %d arrstat %x\n", failarraddr, way, log0, inject_index, arrstat2);
//        uint32_t failarraddr = 0;
//        l2_dump_tags();

        rumboot_printf("DATA [%x] [0x%x%x] ECC: %x\n", log0, log2, log3, log1 & 0xff);

        int bit = log1 >> 16;
        bit = tag_ecc_fix_lookup[bit];
        if (bit<32) {
            log2 = log2 << 2;
            log2 ^= (1<< bit);
            l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG, failarraddr, ((uint64_t) log2));
        } else {
            log1 ^= (1<<(bit - 64));
            l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG_ECC, failarraddr, log1 & 0xff);
        }
        
        rumboot_printf("FIXED DATA [%x] [0x%x%x] ECC: %x\n", log0, log2, log3, log1 & 0xff);


//        l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG, 0, 0x0c0000000);
//        l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG_ECC, 0, 0x0c4);
  //      rumboot_printf("[%d] = %d, \n",log1 >> 16, bit_id);
    
        dbg("Corrected tag data bitflip at way %d\n", way);
        l2_dump_tags();
        l2c_l2_write(scr->dcr_base, L2C_L2ARRSTAT2, arrstat2);
        scr->corrected_tag_errors++;
    }
}


static void l2_scrubber_handler(int irq, void *arg)
{
    uint32_t start = rumboot_platform_get_uptime();
    uint32_t status = l2c_l2_read(DCR_L2C_BASE, L2C_L2INT);
    struct l2_scrubber *scr = arg;
    scr->irq_count++;
	dbg("IRQ %d STATUS 0x%x\n", irq, status);

    if (status & (1<<(31-23))) {
        dbg("ARRSTAT0\n");
    }
    if (status & (1<<(31-24))) {
        dbg("ARRSTAT1\n");
    }
    if (status & (1<<(31-25))) {
        l2_correct_arrstat2(scr);
    }
    uint32_t end = rumboot_platform_get_uptime();
    scr->time_wasted += end - start;
}



struct l2_scrubber *l2_scrubber_create(uintptr_t dcr_base, int irq)
{
    struct l2_scrubber *scrub = rumboot_malloc(sizeof(*scrub));
    memset(scrub, 0x0, sizeof(*scrub));
    scrub->dcr_base = dcr_base;
    scrub->irq = irq;

    l2c_get_mem_layout(DCR_L2C_BASE, &scrub->layout);
    rumboot_printf("l2-scrubber: Attaching to L2 Cache at 0x%x IRQ %d\n", scrub->dcr_base, scrub->irq);
    rumboot_printf("l2-scrubber: L2 total: %d Kbytes tag: %d items lru: %d items data: %d items\n", 
        scrub->layout.l2size_bytes / 1024, 
        scrub->layout.tag_n, 
        scrub->layout.lru_n,
        scrub->layout.data_array_size
    );


    struct rumboot_irq_entry *tbl = rumboot_irq_table_get();
    if (!tbl) {
        tbl = rumboot_irq_create(NULL);
    }

    rumboot_irq_set_handler(tbl, irq,  (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH), l2_scrubber_handler, scrub);
    rumboot_irq_table_activate(tbl);
    rumboot_irq_sei();
    l2c_global_enable_interrupt(dcr_base, 0xffffffff);
    l2c_l2_write (dcr_base, L2C_L2ARRINTEN0, 0xffffffff);
    l2c_l2_write (dcr_base, L2C_L2ARRINTEN1, 0xffffffff);
    l2c_l2_write (dcr_base, L2C_L2ARRINTEN2, 0xffffffff);
    l2c_l2_write (dcr_base, L2C_L2ARRINTEN2, 0xffffffff);
    return scrub;
}