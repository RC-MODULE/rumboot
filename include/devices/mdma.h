#ifndef MDMA_H
#define MDMA_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * \defgroup devices_mdio MDIO
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

/**
 * mdma_device: Structure contains
 * base: base address of mdma
 * cfg: configuration of mdma descriptor table
 * rxtbl: pointer to first descriptor of rx table
 * txtbl: pointer to first descriptor of tx table
 */
struct mdma_device {
        uint32_t base;
        struct mdma_config conf;
        struct descriptor * rxtbl;
        struct descriptor * txtbl;
        size_t cur_rxdesc_index;
        size_t cur_txdesc_index;
};

/**
 * mdma_create: Allocate memory for mdma object and tables.
 * base: base address of memory
 * cfg: configuration of mdma descriptor table
 * return: mdma_device object
 */
struct mdma_device *mdma_create(uint32_t base, struct mdma_config *cfg);
/**
 * mdma_remove: Free memory from mdma object and tables.
 * dev: pointer to allocated memory
 */
void mdma_remove(struct mdma_device *dev);

/**
 * mdma_init: Initialization of mdma block
 * mdma structure of mdma device
 */
void mdma_init(struct mdma_device* mdma);

/**
 * mdma_deinit: Deinitialization of mdma block
 * mdma structure of mdma device
 */
void mdma_deinit(struct mdma_device* mdma);

/**
 * mdma_configure: Set configuration settings
 * dev: pointer to mdma_device object
 * cfg: pointer to mdma configuration settings
 */
void mdma_configure(struct mdma_device *dev, struct mdma_config *cfg);

/**
 * mdma_transaction: Write descriptors in TX descriptor tables
 * mdma structure of mdma device
 * src: address of memory with source data
 * len: length of transmit data
 * is_last: flag - whether this transaction last or not?
 */
void mdma_write_txdescriptor(struct mdma_device *mdma, void* src, size_t len, bool is_last);

/**
* mdma_transaction: Write descriptors in RX descriptor tables
* mdma structure of mdma device
* dst: address of memory where data should be after transaction
* len: length of transmit data
* is_last: flag - whether this transaction last or not?
 */
void mdma_write_rxdescriptor(struct mdma_device *mdma, void* dst, size_t len, bool is_last);

/**
 * mdma_is_finished: Find out whether this transaction finished or not?
 * mdma structure of mdma device
 * @return: false or true
 */
bool mdma_is_finished(struct mdma_device* mdma);

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
 * @return      [description]
 */
bool mdma_transmit_data(uint32_t base, void* dest, void* src, size_t len);

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
        void *        dest;
        void *        src;
        size_t len;
        enum mdma_transaction_state state;
        struct mdma_device *    owner;
        bool is_last;
};

/**
 * mmdma_transaction_create: Allocate memory for mdma_transaction oblect
 * dev: mdma_device object which initiate transaction
 * dest: address of memory where data should be after transaction
 * src: address of memory with source data
 * len: length of transmit memory
 * return: structure contains mdma transaction parameter
 */
struct mdma_transaction *mdma_transaction_create(struct mdma_device *dev, void *dest, void *src, size_t len);

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

#endif
