#ifndef MDMA_H
#define MDMA_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * \defgroup devices_mdma MDMA
 * \ingroup devices
 *
 *  MDIO function libriary.
 *  Contains functions such as:
 *  - mdma create
 *  - mdma remove
 *  - mdma init
 *  - mdma deinit
 *  - mdma set
 *  -
 *  - Read data from SDIO
 *
 * \code{.c}
 * #include <devices/mdma.h>
 * \endcode
 *
 * TODO: Insert usage example here
 *
 * \addtogroup devices_mdma
 *
 * @{
 */


/******************MDMA API***************************/

enum DESC_TYPE {
        NORMAL = 0b00,
        LONG = 0b10,
        PITCH = 0b11,
};

/**
 * mdma_config: Structure contains configuration of mdma descriptor table
 * desc_type: type of descriptors
 * desc_gap: gap between descriptors
   // * num_descriptors: number of descriptors in descriptor table. The same for TX and RX tables.
 */
struct mdma_config {
        enum DESC_TYPE desc_type;
        uint32_t desc_gap;
        bool irq_en;
        size_t num_rxdescriptors; /*for each table!*/
        size_t num_txdescriptors;
};

struct descriptor;

enum MDMA_TYPE {
  MDMA_MDMA,
  MDMA_MUART,
  MDMA_MGETH
};

/**
 * mdma_device: Structure contains
 * base: base address of mdma
 * cfg: configuration of mdma descriptor table
 * rxtbl: pointer to first descriptor of rx table
 * txtbl: pointer to first descriptor of tx table
 */
struct mdma_device {
        uint32_t base;
        enum MDMA_TYPE type;
        struct mdma_config conf;
        volatile struct descriptor * rxtbl;
        volatile struct descriptor * txtbl;
};

/**
 * mdma_create: Allocate memory for mdma object and tables.
 * base: base address of memory
 * cfg: configuration of mdma descriptor table
 * return: mdma_device object
 */
volatile struct mdma_device *mdma_create(uint32_t base, struct mdma_config *cfg);
/**
 * mdma_remove: Free memory from mdma object and tables.
 * dev: pointer to allocated memory
 */
void mdma_remove(volatile struct mdma_device *dev);

/**
 * mdma_init: Initialization of mdma block
 * mdma structure of mdma device
 */
void mdma_init(volatile struct mdma_device* mdma);

/**
 * mdma_deinit: Deinitialization of mdma block
 * mdma structure of mdma device
 */
void mdma_deinit(volatile struct mdma_device* mdma);

/**
 * mdma_configure: Set configuration settings
 * dev: pointer to mdma_device object
 * cfg: pointer to mdma configuration settings
 */
void mdma_configure(volatile struct mdma_device *dev, struct mdma_config *cfg);

/**
 * mdma_transaction: Write descriptors in TX descriptor tables
 * mdma structure of mdma device
 * src: address of memory with source data
 * len: length of transmit data
 * is_last: flag - whether this transaction last or not?
 */
void mdma_write_txdescriptor(volatile struct mdma_device *mdma, volatile void* src, size_t len, size_t index);

/**
* mdma_transaction: Write descriptors in RX descriptor tables
* mdma structure of mdma device
* dst: address of memory where data should be after transaction
* len: length of transmit data
* is_last: flag - whether this transaction last or not?
 */
void mdma_write_rxdescriptor(volatile struct mdma_device *mdma, volatile void* dst, size_t len, size_t index);

/**
 * mdma_is_finished: Find out whether this transaction finished or not?
 * mdma structure of mdma device
 * @return: false or true
 */
bool mdma_is_finished(volatile struct mdma_device* mdma);

/**
 * mdma_dump: Dump mdma registers
 * base: base adress of mdma
 */
void mdma_dump(uint32_t base);

/**
 * transmit_data_throught_dma Transmit data throught DMA controller
 * base: base address of DMA
 * dst: address of memory where data should be after transaction
 * src: address of memory with source data
 * len: length of transmit data
 * @return error code, if error code less than 0 - error!
 */
int mdma_transmit_data(uint32_t base, volatile void* dest, volatile void* src, size_t len);

/**
 * Handle interrupt request
 * irq number of IRQ in IRQ table
 * arg pointer to mdma_device structure
 */
void mdma_irq_handler(int irq, void *arg);

/******************TRANSACTIONS API***************************/

enum mdma_transaction_state {
        IDLE,
        STARTED,
        FINISHED
};

/**
 * mdma_transaction: Structure contains mdma transaction parameter
 * dest: address of memory where data should be after transaction
 * src: address of memory with source data
 * len: length of transmit memory
 * state: state of transaction
 * owner: mdma_device object which initiate transaction
 * is_last: flag - whether this transaction last or not?
 */
struct mdma_transaction {
        volatile void *        dest;
        volatile void *        src;
        size_t len;
        enum mdma_transaction_state state;
        volatile struct mdma_device *    owner;
};

/**
 * mmdma_transaction_create: Allocate memory for mdma_transaction oblect
 * dev: mdma_device object which initiate transaction
 * dest: address of memory where data should be after transaction
 * src: address of memory with source data
 * len: length of transmit memory
 * return: structure contains mdma transaction parameter
 */
struct mdma_transaction *mdma_transaction_create(volatile struct mdma_device *dev,
  volatile void *dest, volatile void *src, size_t len);

/**
 * mmdma_transaction_remove: Free memory from mdma_transaction object
 * t: structure contains mdma transaction parameter
 * return: error code
 */
int mdma_transaction_remove(struct mdma_transaction *t);

/**
 * mdma_get_transaction_state: Get transaction state
 * t: structure contains mdma transaction parameter
 * @return: state of transaction
 */
int mdma_get_transaction_state(struct mdma_transaction *t);

/**
 * mdma_transaction_queue: Push transaction to gueue
 * t: structure contains mdma transaction parameter
 * @return: error code
 */
int mdma_transaction_queue(struct mdma_transaction *t);

/**
 * mdma_transaction_is_finished: Find out whether this transaction finished or not?
 * t: structure contains mdma transaction parameter
 * @return: false or true
 */
bool mdma_transaction_is_finished(struct mdma_transaction *t);

/**
 * [mdma_transaction_dump description]
 * @param t [description]
 */
void mdma_transaction_dump(struct mdma_transaction *t);

/**
 * @}
 */

#endif
