#ifndef MDMA_H
#define MDMA_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

enum DESC_TYPE {
        NORMAL = 0b00,
        LONG = 0b10,
        PITCH = 0b11,
};

struct mdma_config {
        enum DESC_TYPE desc_type;
        uint32_t desc_gap;
        size_t num_descriptors;
};

struct descriptor;

struct mdma_device {
        uint32_t base;
        struct mdma_config conf;
        struct descriptor * rxtbl;
        struct descriptor * txtbl;
};

struct mdma_device *mdma_create(uint32_t base, struct mdma_config *cfg);
void mdma_remove(struct mdma_device *dev);
void mdma_init(uint32_t base);
void mdma_deinit(uint32_t base);
void mdma_set(struct mdma_device *dev, struct mdma_config *cfg);
void mdma_transaction(uint32_t base, void* src, void* dst, size_t len, bool is_last);
bool mdma_is_finished(uint32_t base);
void mdma_dump(uint32_t base);

enum mdma_transaction_state {
        IDLE,
        STARTED,
        FINISHED
};

struct mdma_transaction {
        void *        dest;
        void *        src;
        size_t len;
        enum mdma_transaction_state state;
        struct mdma_device *    owner;
        bool is_last;
};

struct mdma_transaction *mmdma_transaction_create(struct mdma_device *dev, void *dest, void *src, size_t len);
int mmdma_transaction_remove(struct mdma_transaction *t);
int mdma_get_transaction_state(struct mdma_transaction *t);
int mdma_transaction_queue(struct mdma_transaction *t);
bool mdma_transaction_is_finished(struct mdma_transaction *t);

#endif
