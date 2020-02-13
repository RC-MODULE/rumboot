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
#include <devices/sp805.h>
#include <regs/regs_sp805.h>

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
    uint32_t l2int    ;
    uint32_t l2mck;
};



static inline void l2_scrubbing_context_dump(struct l2_scrubbing_context *ctx)
{
#define DO_CTX_DUMP(ctx,reg) dbg("%s: %x\n", #reg, (ctx)->reg);

    dbg("--- ctx ---\n");
    DO_CTX_DUMP(ctx, l2int);
    DO_CTX_DUMP(ctx, l2mck);
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

    ctx->l2int = l2c_l2_read(DCR_L2C_BASE, L2C_L2INT);
    ctx->l2mck = l2c_l2_read(DCR_L2C_BASE, L2C_L2MCK);

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


static int find_data(struct l2_scrubber *scr, uint64_t data) {
    int idx;
    for (idx = 0; idx < (scr->layout.l2size_bytes / 8); idx++) {
        uint64_t actual_data = l2c_read_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA, idx);
        if (data == actual_data)
            return idx;
    }
    return -1;
}

static int find_tag(struct l2_scrubber *scr, uint64_t data) {
    int idx;
    for (idx = 0; idx < (scr->layout.l2size_bytes / 8); idx++) {
        uint64_t actual_data = l2c_read_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG, idx);
        if (data == actual_data)
            return idx;
    }
    return -1;
}

static bool l2_correct_tag(struct l2_scrubber *scr, struct l2_scrubbing_context *ctx, int way)
{
        int index = l2_tag_address_encode(&scr->layout, way, ctx->log0 >> 1);
        uint64_t actual_tag = l2c_read_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG, index);
        uint64_t actual_ecc  = l2c_read_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG_ECC, index);
        uint64_t tag_ecc = ctx->log1 & 0xff;
        uint64_t tag     = ctx->log2 << 2;

        int bit = ctx->log1 >> 16;
        bit = tag_ecc_fix_lookup[bit];

        if (tag != actual_tag) {
        dbg("Multihit: actual: 0x%x %x ecc: %x | logged: 0x%x %x ecc: %x (addr %d log0 0x%x bit %d)\n", 
            (uint32_t) ((actual_tag >> 32) & 0xffffffff),
            (uint32_t) ((actual_tag) & 0xffffffff),
            (uint32_t) ((actual_ecc) & 0xff),
            0,
            (uint32_t) tag,
            (uint32_t) ((tag_ecc) & 0xff),
            index, ctx->log0, bit
            );
        dbg("index must be %d\n", find_tag(scr, tag));
        return false;
        }


        dbg("Correcting tag index %d way %d bit %d\n", index, way, bit);
        if (bit<32) {
            tag ^= (1<< bit);
            l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG, index, ((uint64_t) tag));
        } else {
            bit -= 64;
            ctx->log1 &= 0xff;
            ctx->log1 ^= (1<<(bit - 64));
            l2c_write_mem(scr->dcr_base, &scr->layout, L2C_MEM_TAG_ECC, index, ctx->log1);
        }
        scr->corrected_tag_errors++;
    return false;
}


static bool l2_correct_data(struct l2_scrubber *scr, struct l2_scrubbing_context *ctx, int word)
{
    /* 
     * BEWARE: Hardware BUG
     * We can't be sure if the log registers contain our data, so we need to check them
     * against the actual array data. But, if we're lucky - the word argument is the one 
     * we seek.
     * But sometimes the data in LOG2 & LOG3 is completely wrong, and therefore we won't be 
     * able to do anything about this bitflip. 
     */
    uint32_t failarraddr = (ctx->log0 << 3) | word;
    uint64_t actual_data = l2c_read_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA, failarraddr);
    uint64_t actual_ecc  = l2c_read_mem(scr->dcr_base, &scr->layout, L2C_MEM_DATA_ECC, failarraddr);
    uint64_t data = (((uint64_t) ctx->log2) << 32 | ((uint64_t) ctx->log3));
    uint64_t data_ecc = ctx->log1 & 0xff;
    scr->lastfailaddr = failarraddr;
    int bit = ctx->log1 >> 16;
    bit = data_ecc_fix_lookup[bit];

    if (scr->scrub_with_pattern) {
        l2c_data_direct_write(scr->dcr_base, &scr->layout, failarraddr, scr->pattern);
        dbg("Fixed bitflip by overriting the pattern @ index %d\n", failarraddr);
        scr->corrected_data_errors++;
        /* When scrubbing with pattern we can fix multiple flips in one go */
        return false; 
    }

    if (data != actual_data) { 
        dbg("Multihit: actual: 0x%x %x ecc: %x | logged: 0x%x %x ecc: %x (addr %d log0 0x%x bit %d)\n", 
            (uint32_t) ((actual_data >> 32) & 0xffffffff),
            (uint32_t) ((actual_data) & 0xffffffff),
            (uint32_t) ((actual_ecc) & 0xff),
            (uint32_t) ((data >> 32) & 0xffffffff),
            (uint32_t) ((data) & 0xffffffff),
            (uint32_t) ((data_ecc) & 0xff),
            failarraddr, ctx->log0, bit
            );
//        data = actual_data;
//        dbg("Index should be %d\n", find_data(scr, data));
//        l2_scrubbing_context_read(scr, ctx);
//        l2_dump_data((failarraddr - 8) > 0 ? (failarraddr - 8) : 0 , 16);
        return false; /* That's not our bitflip!*/
    }

    /* That's our baby, let's fix it */
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
    return true;
}

static void scrubber_exception(struct l2_scrubber *scr, struct l2_scrubbing_context *ctx, const char *descr)
{

    struct sp805_conf wd_conf =
        {
            .mode              = FREERUN,
            .interrupt_enable  = 0x1,
            .width             = 0x20,
            .load              = 0x2fffff
        };

    sp805_unlock_access(DCR_WATCHDOG_BASE);
    sp805_clrint(DCR_WATCHDOG_BASE);
    sp805_config(DCR_WATCHDOG_BASE, &wd_conf);
    dcr_write(DCR_WATCHDOG_BASE + WD_REG_CONTROL, dcr_read(DCR_WATCHDOG_BASE + WD_REG_CONTROL) | WD_CTRL_RESEN);
    sp805_enable(DCR_WATCHDOG_BASE);

    if (ctx) {
        rumboot_printf("  /!\\  FATAL SCRUBBING EXCEPTION  /!\\ \n");
        rumboot_printf("  -  L2 Debug Info  -\n");
        rumboot_printf("L2INT:      0x%x\n",  ctx->l2int);   
        rumboot_printf("L2MCK:      0x%x\n",  ctx->l2mck);   
        rumboot_printf("ARRSTAT0:   0x%x\n",  ctx->arrstat0);   
        rumboot_printf("ARRSTAT1:   0x%x\n",  ctx->arrstat1);   
        rumboot_printf("ARRSTAT2:   0x%x\n",  ctx->arrstat2);   
        rumboot_printf("FERR:       0x%x\n",  ctx->ferr);   
        rumboot_printf("ARRFERR0:   0x%x\n",  ctx->arrferr0);   
        rumboot_printf("ARRFERR1:   0x%x\n",  ctx->arrferr1);   
        rumboot_printf("ARRFERR2:   0x%x\n",  ctx->arrferr2);   
        rumboot_printf("LOG0:   0x%x\n",  ctx->log0);   
        rumboot_printf("LOG1:   0x%x\n",  ctx->log1);   
        rumboot_printf("LOG2:   0x%x\n",  ctx->log2);   
        rumboot_printf("LOG3:   0x%x\n",  ctx->log3);   
        rumboot_printf("LOG4:   0x%x\n",  ctx->log4);   
        rumboot_printf("LOG5:   0x%x\n",  ctx->log5);   
        l2_scrubber_dump_stats(scr);
   }
   if (scr->old_mck_handler) {
        scr->old_mck_handler(4, descr);
   }
}

static void l2_correct_arrstat2(struct l2_scrubber *scr, struct l2_scrubbing_context *ctx)
{
    uint32_t arrstat2 = ctx->arrstat2;
    int multihit = 0;
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
        } else {
            scrubber_exception(scr, &ctx, "Uncorrectables from ARRSTAT2");
        }
        arrstat2 &= ~(1<<failure); /* Clear this one */
        dbg("Arrstat2 now %x\n", arrstat2);
        multihit++;
    }

    bailout:
        if (multihit) {
            scr->multihits++;
        }
        if (scr->maxhit < multihit) {
            scr->maxhit = multihit;
        }
        dbg("--------DONE--------\n");
        l2c_l2_write(scr->dcr_base, L2C_L2ARRSTAT2, ctx->arrstat2);
}


static void l2_scrubber_handler(int irq, void *arg)
{
    /* Read the context ASAP */
    struct l2_scrubber *scr = arg;
    struct l2_scrubbing_context ctx;
    l2_scrubbing_context_read(scr, &ctx);

    uint32_t start = rumboot_platform_get_uptime();
    uint32_t status = ctx.l2int | ctx.l2mck;
    if (status & (1<<(31-23))) {
        scrubber_exception(scr, &ctx, "Uncorrectables from ARRSTAT0");
    }
    if (status & (1<<(31-24))) {
        scrubber_exception(scr, &ctx, "Uncorrectables from ARRSTAT1");
    }

    if (status & (1<<(31-25))) {
        l2_correct_arrstat2(scr, &ctx);
    }

    uint32_t end = rumboot_platform_get_uptime();
    uint32_t time_between_errors = start - scr->last_error_timestamp;
    scr->last_error_timestamp = start;

    if (scr->irq_count == 1) {
        scr->min_time_between_errors = time_between_errors;
        scr->max_time_between_errors = time_between_errors;
        scr->avg_time_between_errors = time_between_errors;
    } else if (scr->irq_count > 1) {
        /* Theoretically this should more or less allow us to avoid overflows */
        scr->avg_time_between_errors = (time_between_errors + (scr->avg_time_between_errors * scr->irq_count)) / (scr->irq_count + 1);
    }

    if (time_between_errors < scr->min_time_between_errors) {
        scr->min_time_between_errors = time_between_errors;
    }

    if (time_between_errors > scr->max_time_between_errors) {
        scr->max_time_between_errors = time_between_errors;
    }

    scr->irq_count++;
    scr->time_wasted += end - start;
}

static volatile struct l2_scrubbing_context *the_scrubber;

void l2scrub_mck_handler(int id, const char *name)
{
    uint32_t mcsr = spr_read(SPR_MCSR_RW);
    dbg("MCK: MCSR %x\n", mcsr);
    if (mcsr & (1 << ITRPT_MCSR_L2_i)) {
        /* That's our business */
        l2_scrubber_handler(4, the_scrubber);
        spr_write(0x33C, (1 << ITRPT_MCSR_MCS_i) | (1 << ITRPT_MCSR_L2_i));
    }

    mcsr = spr_read(SPR_MCSR_RW);
    /* Some other shit happened, let previous handler handle it */
    if (mcsr) {
        scrubber_exception(the_scrubber, NULL, "Machine check");
    }
}

struct l2_scrubber *l2_scrubber_create(uintptr_t dcr_base, int irq)
{
    struct l2_scrubber *scrub = rumboot_malloc(sizeof(*scrub));
    memset(scrub, 0x0, sizeof(*scrub));
    scrub->dcr_base = dcr_base;
    scrub->irq = irq;
    scrub->old_mck_handler = rumboot_platform_runtime_info->irq_exception_hndlr;
    rumboot_irq_set_exception_handler(l2scrub_mck_handler);
    the_scrubber = scrub;

    l2c_get_mem_layout(DCR_L2C_BASE, &scrub->layout);
    rumboot_printf("l2-scrubber: Attaching to L2 Cache at 0x%x IRQ %d\n", scrub->dcr_base, scrub->irq);
    rumboot_printf("l2-scrubber: L2 total: %d Kbytes tag: %d items lru: %d items data: %d items\n", 
        scrub->layout.l2size_bytes / 1024, 
        scrub->layout.tag_n, 
        scrub->layout.lru_n,
        scrub->layout.data_array_size
    );

    if (irq >= 0) {
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
    } else if (irq == -1) {
        /* Use machine-checks */
        l2c_l2_write (dcr_base, L2C_L2ARRMCKEN1, 0xffffffff);
        l2c_l2_write (dcr_base, L2C_L2ARRMCKEN2, 0xffffffff);
    } else {
        /* Polling, please call l2_scrubber_scrub_once() periodically */
    }

    /* Use single-threaded access */
    uint32_t cfg =     l2c_l2_read (dcr_base, L2C_L2ARRCFG);
    l2c_l2_write (dcr_base, L2C_L2ARRCFG, cfg | 1 | (0x7<<9));
    /* Read out any pending log errors */
    struct l2_scrubbing_context ctx;
    l2_scrubbing_context_read(scrub, &ctx);
    return scrub;
}

void l2_scrubber_enable_pattern_scrubbing(struct l2_scrubber *scr, uint64_t pattern)
{
    scr->scrub_with_pattern = 1;
    scr->pattern = pattern;
}

void l2_scrubber_dump_stats(struct l2_scrubber *scr)
{
    rumboot_printf("---- L2 Scrubber Statistics ----\n");
    rumboot_printf("IRQs served:             %d\n", scr->irq_count);
    rumboot_printf("Corrected TAGs:          %d\n", scr->corrected_tag_errors);
    rumboot_printf("Corrected DATA:          %d\n", scr->corrected_data_errors);
    rumboot_printf("Multiword hits:          %u\n", scr->multihits);
    rumboot_printf("Max simultaneous hits:   %u\n", scr->maxhit);
    rumboot_printf("Time wasted:             %u us\n", scr->time_wasted);
    rumboot_printf("Time between errors:     min: %u us max: %u us avg: %u us\n", 
        scr->min_time_between_errors, scr->max_time_between_errors, scr->avg_time_between_errors);
}

void l2_scrubber_reset_stats(struct l2_scrubber *scr)
{
    int len = ((uint8_t *) &(scr->stats_end)) - ((uint8_t *) &(scr->stats_start));
    memset(&(scr->stats_start), 0x0, len);
}

void l2_scrubber_scrub_once(struct l2_scrubber *scr)
{
    l2_scrubber_handler(0, scr);
}