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


void l2_dump_tag(struct l2c_mem_layout *layout, int index)
{
    uint64_t tag = l2c_read_mem(DCR_L2C_BASE, layout, L2C_MEM_TAG, index);
    uint64_t ecc = l2c_read_mem(DCR_L2C_BASE, layout, L2C_MEM_TAG_ECC, index);
    rumboot_printf("TAG[%d]: DATA: 0x%x%x ECC: 0x%x%x\n",
    index, 
        (uint32_t) ((tag >> 32) & 0xffffffff),
        (uint32_t) ((tag) & 0xffffffff),
        (uint32_t) ((ecc >> 32) & 0xffffffff),
        (uint32_t) ((ecc) & 0xffffffff)
    );
}
/**
 * @brief Dumps all L2 tags to stdout
 * 
 */
void l2_dump_tags()
{
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    int i; 
    for (i=0; i<16; i++) {
        l2_dump_tag(&layout, i);
    }
}

/**
 * @brief Dumps a cache line by its index to stdout
 * 
 * @param idx cache line index
 */
void l2_dump_line(uint32_t idx)
{
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    uint64_t tag = l2c_read_mem(DCR_L2C_BASE, &layout, L2C_MEM_DATA, idx);
    uint64_t ecc = l2c_read_mem(DCR_L2C_BASE, &layout, L2C_MEM_DATA_ECC, idx);
    rumboot_printf("DATA[%d]: DATA: 0x%08x%08x ECC: 0x%08x%08x\n",
        idx, 
        (uint32_t) ((tag >> 32) & 0xffffffff),
        (uint32_t) ((tag) & 0xffffffff),
        (uint32_t) ((ecc >> 32) & 0xffffffff),
        (uint32_t) ((ecc) & 0xffffffff)
    );
}

/**
 * @brief Dumps a maximum of 'max' cache lines to stdout
 * 
 * @param max 
 */
void l2_dump_data(uint32_t start, uint32_t count)
{
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    int i; 
    for (i=start; i<start + count; i++) {
        l2_dump_line(i);
    }
}

/**
 * @brief Injects a single bit data ecc fault.
 * 
 * @param base L2 DCR base address
 * @param pos array index
 * @param bit bit to flip
 */
void l2_inject_data_ecc_fault(uintptr_t base, int pos, int bit)
{
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    RUMBOOT_ATOMIC_BLOCK() {
        l2_dump_line(pos);
        uint64_t ecc = l2c_read_mem(DCR_L2C_BASE, &layout, L2C_MEM_DATA_ECC, pos);
        ecc ^= 1<<bit;
        l2c_write_mem(DCR_L2C_BASE, &layout, L2C_MEM_DATA_ECC, pos, ecc); 
        l2_dump_line(pos);
    }
}

/**
 * @brief Injects a single bit tag fault.
 * 
 * @param base L2 DCR base address
 * @param pos array index
 * @param bit bit to flip
 */
void l2_inject_tag_fault(uintptr_t base, int pos, int bit)
{
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    RUMBOOT_ATOMIC_BLOCK() {
        uint64_t tag = l2c_read_mem(DCR_L2C_BASE, &layout, L2C_MEM_TAG, pos);
        tag ^= 1ULL<<bit;
        l2c_write_mem(DCR_L2C_BASE, &layout, L2C_MEM_TAG, pos, tag);    
    }
}

/**
 * @brief Injects a single bit tag ecc fault.
 * 
 * @param base L2 DCR base address
 * @param pos array index
 * @param bit bit to flip
 */
void l2_inject_tag_ecc_fault(uintptr_t base, int pos, int bit)
{
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    uint64_t tag = l2c_read_mem(DCR_L2C_BASE, &layout, L2C_MEM_TAG_ECC, pos);
    tag ^= 1ULL<<bit;
    l2c_write_mem(DCR_L2C_BASE, &layout, L2C_MEM_TAG_ECC, pos, tag);    
}

/**
 * @brief Injects a single bit data fault.
 * 
 * @param base L2 DCR base address
 * @param pos array index
 * @param bit bit to flip
 */
void l2_inject_data_fault(uintptr_t base, int pos, int bit)
{
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    RUMBOOT_ATOMIC_BLOCK() {
        uint64_t tag = l2c_read_mem(DCR_L2C_BASE, &layout, L2C_MEM_DATA, pos);
        tag ^= 1ULL<<bit;
        l2c_write_mem(DCR_L2C_BASE, &layout, L2C_MEM_DATA, pos, tag);    
    }
}

