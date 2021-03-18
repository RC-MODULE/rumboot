#ifndef NFIFO_H
#define NFIFO_H

enum nfifo_status {
        NFIFO_STATUS_READ_EMPTY=0,
        NFIFO_STATUS_WRITE_FULL=1,
        NFIFO_STATUS_INACTIVE=3,
        NFIFO_STATUS_IDLE=4,
};

struct nfifo_instance {
        uintptr_t base;
        int dcr;
        int spacing;
};

/**
 * @brief Initializes internal fifo structure instance
 * 
 * @param inst - instance to init
 * @param base - base address of the APB Fifo core
 * @param is_dcr - non-zero if the fifo is located on the dcr bus (ppc only)
 */
void nfifo_instance_init(struct nfifo_instance *inst, uintptr_t base);

/**
 * @brief Enables & disables the fifo
 * 
 * @param inst nFifo instance
 * @param enable 
 */
void nfifo_enable(struct nfifo_instance *inst, int enable);

/**
 * @brief Reads a 32-bit word from nfifo. This function may block
 * 
 */
uint32_t nfifo_read(struct nfifo_instance *inst, uint32_t timeout_us);

/**
 * @brief Writes a 32-bit word into the fifo. This function may block
 * 
 * @param inst 
 * @param word 
 */
void nfifo_write(struct nfifo_instance *inst, uint32_t word);

/**
 * @brief Returns non-zero when a read operation will not block
 * 
 * @param inst 
 * @return int 
 */
int nfifo_can_read(struct nfifo_instance *inst);

/**
 * @brief Returns non-zero when a read operation will not block
 * 
 */
int nfifo_can_write(struct nfifo_instance *inst);


#endif // !NFIFO_H