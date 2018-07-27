/*
 * nor_1636RR4.h
 *
 *  Created on: Jul 26, 2018
 *      Author: s.chernousov
 */

#ifndef NOR_1636RR4_H_
#define NOR_1636RR4_H_

#include <stdint.h>
#include <stdbool.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/test_assert.h>

typedef struct
{
    uint32_t addr;
    uint32_t data;
} nor_addr_data32_pair;

#define NOR_WRITE32_SEQUENCE(base,sequence) \
    MACRO_BEGIN \
        int sequence_length = ARRAY_SIZE(sequence);\
        int i= 0;\
        for(i=0;i<sequence_length;i++)\
          iowrite32 (sequence[i].data, base + sequence[i].addr);\
    MACRO_END

static inline void nor_write32 (uint32_t write_DATA, uint32_t write_ADDR)
{
    nor_addr_data32_pair NOR_write32_word_program_sequence[] =
    {
        {(0x555 << 2),     0xAAAAAAAA},
        {(0x2AA << 2),     0x55555555},
        {(0x555 << 2),     0xA0A0A0A0},
        {0,                0}
    };

    NOR_write32_word_program_sequence[3].addr = write_ADDR - NOR_BASE;
    NOR_write32_word_program_sequence[3].data = write_DATA;
    NOR_WRITE32_SEQUENCE(NOR_BASE, NOR_write32_word_program_sequence);
}

#endif /* NOR_1636RR4_H_ */
