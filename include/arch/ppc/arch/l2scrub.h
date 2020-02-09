#ifndef L2SCRUB_H
#define L2SCRUB_H


struct l2_scrubber {
    uintptr_t dcr_base;
    int irq;
    struct l2c_mem_layout layout;
    volatile uint32_t irq_count;
    volatile uint32_t corrected_data_errors;
    volatile uint32_t corrected_tag_errors;
    volatile uint32_t time_wasted;
    volatile l2c_mem_t lastfailmem;
    volatile uint32_t lastfailaddr;
    volatile uint32_t lastfailbit;
    volatile int injected_bit;
};

struct l2_scrubber *l2_scrubber_create(uintptr_t dcr_base, int irq);

void l2_dump_data(uint32_t start, uint32_t count);
void l2_dump_tag(struct l2c_mem_layout *layout, int index);
void l2_dump_tags();


void l2_inject_data_ecc_fault(uintptr_t base, int pos, int bit);
void l2_inject_tag_fault(uintptr_t base, int pos, int bit);
void l2_inject_tag_ecc_fault(uintptr_t base, int pos, int bit);
void l2_inject_data_fault(uintptr_t base, int pos, int bit);



#endif