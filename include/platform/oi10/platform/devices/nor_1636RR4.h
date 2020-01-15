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
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/devices.h>
#include <platform/test_assert.h>

typedef struct {
        uint32_t    addr;
        uint32_t    data;
} nor_addr_data32_pair;

#define NOR_WRITE32_SEQUENCE(base, sequence) \
        MACRO_BEGIN \
        int sequence_length = ARRAY_SIZE(sequence); \
        int i = 0; \
        for (i = 0; i < sequence_length; i++) \
                iowrite32(sequence[i].data, base + sequence[i].addr); \
        MACRO_END

static inline void nor_write32(uint32_t write_DATA, uint32_t write_ADDR)
{
        RUMBOOT_ATOMIC_BLOCK() {
                nor_addr_data32_pair NOR_write32_word_program_sequence[] =
                {
                        { (0x555 << 2), 0xAAAAAAAA },
                        { (0x2AA << 2), 0x55555555 },
                        { (0x555 << 2), 0xA0A0A0A0 },
                        { 0,        0      }
                };

                NOR_write32_word_program_sequence[3].addr = write_ADDR - NOR_BASE;
                NOR_write32_word_program_sequence[3].data = write_DATA;
                NOR_WRITE32_SEQUENCE(NOR_BASE, NOR_write32_word_program_sequence);
                udelay(200); //workaround
#if !defined(NOR_SELFCHECKING_DISABLE)
                nop();
                nop();
                nop();
                nop();
                TEST_ASSERT(ioread32(write_ADDR) == write_DATA, "ERROR: nor_write32 failed");
#endif
        }
}

static inline void nor_reset(){
    iowrite32(0xF0F0F0F0, NOR_BASE);
}

static inline void nor_erase_sect(uint32_t addr){

    RUMBOOT_ATOMIC_BLOCK() {
        nor_addr_data32_pair NOR_sector_erase_sequence[] =
        {
            {(0x555 << 2),     0xAAAAAAAA},
            {(0x2AA << 2),     0x55555555},
            {(0x555 << 2),     0x80808080},
            {(0x555 << 2),     0xAAAAAAAA},
            {(0x2AA << 2),     0x55555555},
            {0,                0x30303030}
        } ;

        NOR_sector_erase_sequence[5].addr = addr - NOR_BASE;
        NOR_WRITE32_SEQUENCE(NOR_BASE,NOR_sector_erase_sequence);
        udelay(50);
        /*
         * TODO: For detecting of sector erase finishing we have to poll STATUS[D3] bit (4.17 of document 1636RR4.pdf)
         */
#if !defined(NOR_SELFCHECKING_DISABLE)
        TEST_ASSERT(ioread32(addr)==0xffffffff, "ERROR: nor_erase_sect failed");
#endif
        nor_reset();
    }
}

#endif /* NOR_1636RR4_H_ */
