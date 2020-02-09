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

struct l2_scrubbing_context {
    uint32_t log0     ;
    uint32_t log1     ;
    uint32_t log2     ;
    uint32_t log3     ;
    uint32_t log4     ;
    uint32_t log5     ;
    uint32_t ferr     ;
    uint32_t arrferr0 ;
    uint32_t arrferr1 ;
    uint32_t arrferr2 ;
    uint32_t arrstat0 ;
    uint32_t arrstat1 ;
    uint32_t arrstat2 ;
};



static inline void l2_scrubbing_context_dump(struct l2_scrubbing_context *ctx)
{
#define DO_CTX_DUMP(ctx,reg) dbg("%s: %x\n", #reg, (ctx)->reg);

    dbg("--- ctx ---\n");
    DO_CTX_DUMP(ctx, log0);
    DO_CTX_DUMP(ctx, log1);
    DO_CTX_DUMP(ctx, log2);
    DO_CTX_DUMP(ctx, log3);
    DO_CTX_DUMP(ctx, log4);
    DO_CTX_DUMP(ctx, log5);
    DO_CTX_DUMP(ctx, ferr);
    DO_CTX_DUMP(ctx, arrstat0);
    DO_CTX_DUMP(ctx, arrstat1);
    DO_CTX_DUMP(ctx, arrstat2);
    DO_CTX_DUMP(ctx, arrferr0);
    DO_CTX_DUMP(ctx, arrferr1);
    DO_CTX_DUMP(ctx, arrferr2);
    dbg("--- --- ---\n");
}

#if 0
#define DO_READ(val, reg) \
    tmp = l2c_l2_read(scr->dcr_base, reg); \
    if (tmp) { \
        reads++; \
        val = tmp;\
    };
#else
#define DO_READ(val, reg) \
    tmp = l2c_l2_read(scr->dcr_base, reg); \
    if (tmp != val) { \
        reads++; \
        val = tmp;\
    };
#endif

static inline void l2_scrubbing_context_read(struct l2_scrubber *scr, struct l2_scrubbing_context *ctx)
{
    /* We need to read the last error from theese */
    int total_reads = 0;
    int reads;
    uint32_t tmp;
    ctx->arrstat0 = l2c_l2_read(scr->dcr_base, L2C_L2ARRSTAT0);
    ctx->arrstat1 = l2c_l2_read(scr->dcr_base, L2C_L2ARRSTAT1);
    ctx->arrstat2 = l2c_l2_read(scr->dcr_base, L2C_L2ARRSTAT2);

    ctx->ferr     = l2c_l2_read(scr->dcr_base, L2C_L2FERR);
    ctx->arrferr0 = l2c_l2_read(scr->dcr_base, L2C_L2ARRFERR0);
    ctx->arrferr1 = l2c_l2_read(scr->dcr_base, L2C_L2ARRFERR1);
    ctx->arrferr2 = l2c_l2_read(scr->dcr_base, L2C_L2ARRFERR2);

    do {
            reads = 0;
            DO_READ(ctx->log0, L2C_L2LOG0);
            DO_READ(ctx->log1, L2C_L2LOG1);
            DO_READ(ctx->log2, L2C_L2LOG2);
            DO_READ(ctx->log3, L2C_L2LOG3);
            DO_READ(ctx->log4, L2C_L2LOG4);
            DO_READ(ctx->log5, L2C_L2LOG5);
            total_reads += reads;
        l2_scrubbing_context_dump(ctx);
    } while(0);
    dbg("ctx took %d reads\n", total_reads);


}




static bool l2_correct_tag(struct l2_scrubber *scr, struct l2_scrubbing_context *ctx, int way)
{
    return false;
}

static int find_data(struct l2_scrubber *scr, uint64_t data) {
    int idx;
    for (idx = 0; idx < (scr->layout.l2size_bytes / 8); idx++) {
        uint64_t actual_data = l2c_read_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA, idx);
        if (data == actual_data)
            return idx;
    }
    return -1;
}
static bool l2_correct_data(struct l2_scrubber *scr, struct l2_scrubbing_context *ctx, int word)
{
    /* 
     * We can't be sure if the log registers contain out data, so we need to check them
     * against the actual array data. But, if we're lucky - the word argument is the one 
     * we seek.
     */
    uint32_t failarraddr = (ctx->log0 << 3) | word;
    uint64_t actual_data = l2c_read_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA, failarraddr);
    uint64_t actual_ecc  = l2c_read_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA_ECC, failarraddr);
    uint64_t data = (((uint64_t) ctx->log2) << 32 | ((uint64_t) ctx->log3));
    if (data != actual_data) { 
        dbg("Multihit: actual: 0x%x %x | logged: 0x%x %x (addr %d log0 0x%x)\n", 
            (uint32_t) ((actual_data >> 32) & 0xffffffff),
            (uint32_t) ((actual_data) & 0xffffffff),
            (uint32_t) ((data >> 32) & 0xffffffff),
            (uint32_t) ((data) & 0xffffffff),
            failarraddr, ctx->log0
            );
//        dbg("Index should be %d\n", find_data(scr, data));
//        l2_scrubbing_context_read(scr, ctx);
//        l2_dump_data((failarraddr - 8) > 0 ? (failarraddr - 8) : 0 , 16);
        return false; /* That's not our bitflip!*/
    }
    /* That's our baby, let's fix it */

    l2_dump_data(failarraddr, 1);
    scr->lastfailaddr = failarraddr;
    int bit = ctx->log1 >> 16;
    bit = data_ecc_fix_lookup[bit];
    if (bit<64) {
        data ^= (1ULL << bit);
        l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA, failarraddr, data);
        scr->lastfailmem = L2C_MEM_DATA;
        scr->lastfailbit = bit;
    } else {
        ctx->log1 ^= (1<<(bit - 64));
        l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA_ECC, failarraddr, ctx->log1 & 0xff);
        scr->lastfailmem = L2C_MEM_DATA_ECC;
        scr->lastfailbit = bit;
    }
    scr->corrected_data_errors++;
    dbg("Corrected cache data bitflip at index %d bit %d\n", failarraddr, bit);
    l2_dump_data(failarraddr, 1);
    return true;
}

static void l2_correct_arrstat2(struct l2_scrubber *scr, struct l2_scrubbing_context *ctx)
{
    uint32_t arrstat2 = ctx->arrstat2;
    while(1) {
        int failure = ffs(arrstat2);
        if (failure <= 0)
            break;
        failure -= 1;
        dbg("Processing failbit %d |  %x\n", failure, arrstat2);
            
        if (failure < 8) {
            /* Correctable flips in data, fix it! */
            int failword = 7 - failure;
            if (l2_correct_data(scr, ctx, failword)) {
                goto bailout; /* We've corrected something! Good!*/
            }
            /* If not, it's in the other word */
        } else if (failure < 12) {
            /* Correctable flips in tag, go fix it! */
            uint32_t way = 3 - (failure - 8);
            if (l2_correct_tag(scr, ctx, way)) {
                goto bailout; /* We've corrected something! Good!*/
            }
        }
        arrstat2 &= ~(1<<failure); /* Clear this one */
        dbg("Arrstat2 now %x\n", arrstat2);
    }

    bailout:
        dbg("--------DONE--------\n");
        l2c_l2_write(scr->dcr_base, L2C_L2ARRSTAT2, ctx->arrstat2);

#if 0
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
        scr->corrected_data_errors++;
        dbg("Corrected cache data bitflip at index %d\n", failarraddr);
    } else if (failbit < 12) {
        uint32_t way = 3 - (failbit - 8);
        log0 = log0 >> 1;
        int index = l2_tag_address_encode(&scr->layout, way, log0);
        int bit = log1 >> 16;
        bit = tag_ecc_fix_lookup[bit];
        if (bit<32) {
            log2 = log2 << 2;
            log2 ^= (1<< bit);
            l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG, index, ((uint64_t) log2));
        } else {
            bit -= 64;
            log1 &= 0xff;
            log1 ^= (1<<(bit - 64));
            l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG_ECC, index, log1);
        }
   //     l2c_tag_directwrite(scr->dcr_base, &scr->layout, index, log2, log1);
   //     l2_dump_tag(&scr->layout, index);
   //     l2_dump_tag(&scr->layout, index);
        scr->corrected_tag_errors++;
    }

    l2c_l2_write(scr->dcr_base, L2C_L2ARRSTAT2, arrstat2);

    arrstat2 &= ~(1<<failbit);
    if (arrstat2) {
        rumboot_printf("LOG REGS: LOG0: 0x%x LOG1: 0x%x LOG2: 0x%x LOG3: 0x%x LOG4: 0x%x LOG5: 0x%x FERR: 0x%x\n", 
        log0,
        log1,
        log2,
        log3,
        log4,
        log5,
        ferr 
        );


        //rumboot_platform_panic("FATAL: Multiple errors in TAGs (%x & %x), don't know what to do\n", arrstat2, (1<<failbit));
        return;
    }
#endif
}


static void l2_scrubber_handler(int irq, void *arg)
{
    /* Read the context ASAP */
    struct l2_scrubber *scr = arg;
    struct l2_scrubbing_context ctx;
    l2_scrubbing_context_read(scr, &ctx);

    uint32_t start = rumboot_platform_get_uptime();
    uint32_t status = l2c_l2_read(DCR_L2C_BASE, L2C_L2INT);
    scr->irq_count++;
	dbg("IRQ %d STATUS 0x%x\n", irq, status);
    
    if (status & (1<<(31-23))) {
        dbg("ARRSTAT0\n");
    }
    if (status & (1<<(31-24))) {
        dbg("ARRSTAT1\n");
    }
    if (status & (1<<(31-25))) {
        dbg("ARRSTAT2\n");
        l2_correct_arrstat2(scr, &ctx);
    }

    uint32_t end = rumboot_platform_get_uptime();
    scr->time_wasted += end - start;
}

static volatile struct l2_scrubbing_context *the_scrubber;

void l2scrub_mck_handler(int id, const char *name)
{
    uint32_t mcsr = spr_read(SPR_MCSR_RW);
    dbg("MCK: MCSR %x\n", mcsr);
    l2_scrubber_handler(4, the_scrubber);
    spr_write(0x33C, mcsr);
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

    #ifdef USE_IRQ
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
    l2c_l2_write (dcr_base, L2C_L2ARRMCKEN1, 0xffffffff);
    l2c_l2_write (dcr_base, L2C_L2ARRMCKEN2, 0x01110000);
#else
    l2c_l2_write (dcr_base, L2C_L2ARRMCKEN1, 0xffffffff);
    l2c_l2_write (dcr_base, L2C_L2ARRMCKEN2, 0xffffffff);
    rumboot_irq_set_exception_handler(l2scrub_mck_handler);
    the_scrubber = scrub;
#endif
    /* Use single-threaded access */
    uint32_t cfg =     l2c_l2_read (dcr_base, L2C_L2ARRCFG);
    l2c_l2_write (dcr_base, L2C_L2ARRCFG, cfg | 1 | (0x7<<9));
    /* Read out any pending log errors */
    struct l2_scrubbing_context ctx;
    l2_scrubbing_context_read(scrub, &ctx);
    return scrub;
}