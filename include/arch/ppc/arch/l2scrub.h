#ifndef L2SCRUB_H
#define L2SCRUB_H


struct l2_scrubber {
    uintptr_t dcr_base;
    int irq;
    struct l2c_mem_layout layout;

    volatile int injected_bit;
    int scrub_with_pattern;
    uint64_t pattern;
    /* Private data */
    void (*old_mck_handler)(int id, const char *name);

    int stats_start;
    volatile uint32_t irq_count;
    volatile uint32_t multihits;
    volatile uint32_t maxhit;
    volatile uint32_t corrected_data_errors;
    volatile uint32_t corrected_tag_errors;
    volatile l2c_mem_t lastfailmem;
    volatile uint32_t lastfailaddr;
    volatile uint32_t lastfailbit;

    volatile uint32_t time_wasted;
    volatile uint32_t last_error_timestamp;
    volatile uint32_t min_time_between_errors;
    volatile uint32_t max_time_between_errors;
    volatile uint32_t avg_time_between_errors; 
    int stats_end;
};

struct l2_scrubber *l2_scrubber_create(uintptr_t dcr_base, int irq);
void l2_scrubber_enable_pattern_scrubbing(struct l2_scrubber *scr, uint64_t pattern);
void l2_scrubber_scrub_once(struct l2_scrubber *scr);

void l2_dump_data(uint32_t start, uint32_t count);
void l2_dump_tag(struct l2c_mem_layout *layout, int index);
void l2_dump_tags(int start, int count);


void l2_inject_data_ecc_fault(uintptr_t base, int pos, int bit);
void l2_inject_tag_fault(uintptr_t base, int pos, int bit);
void l2_inject_tag_ecc_fault(uintptr_t base, int pos, int bit);
void l2_inject_data_fault(uintptr_t base, int pos, int bit);
void l2_scrubber_dump_stats(struct l2_scrubber *scr);
void l2_scrubber_reset_stats(struct l2_scrubber *scr);
#endif