#ifndef RUMBOOT_IRQ_H
#define RUMBOOT_IRQ_H


#define RUMBOOT_IRQ_TYPE_NORMAL       0
#define RUMBOOT_IRQ_TYPE_EXCEPTION    1


/* ARM Exception names */
#define RUMBOOT_IRQ_PREFETCH_ABORT          0
#define RUMBOOT_IRQ_DATA_ABORT              1
#define RUMBOOT_IRQ_UNDEFINED_INSTRUCTION   2

/* IRQ & FIQ */
#define RUMBOOT_IRQ_IRQ  0
#define RUMBOOT_IRQ_FIQ  1

#ifndef __ASSEMBLER__
    struct rumboot_irq_table;

    /**
     * This function is called to
     * @param type [description]
     * @param id   [description]
     */
    void rumboot_irq_core_dispatch(uint32_t type, uint32_t id);

#endif

#endif /* end of include guard: RUMBOOT_IRQ_H */
