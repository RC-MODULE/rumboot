#include <stddef.h>
#include <rumboot/macros.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/devices/l2c.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c_pmu.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <rumboot/printf.h>

//#define L2C_TRACE_DEBUG_MSG

void l2c_global_enable_interrupt( uint32_t const l2c_dcr_base, L2INTEN_bits_t const mask ) {
    l2c_l2_write (l2c_dcr_base, L2C_L2INTEN, l2c_l2_read (l2c_dcr_base, L2C_L2INTEN) | mask );
}

void l2c_global_mck_enable( uint32_t const l2c_dcr_base, L2MCKEN_bits_t const mask ) {
    l2c_l2_write (l2c_dcr_base, L2C_L2MCKEN, l2c_l2_read (l2c_dcr_base, L2C_L2MCKEN) | mask );
}

void l2c_global_mck_disable( uint32_t const l2c_dcr_base, L2MCKEN_bits_t const mask ) {
    uint32_t value = l2c_l2_read (l2c_dcr_base, L2C_L2MCKEN) & ~mask;
    l2c_l2_write (l2c_dcr_base, L2C_L2MCKEN, value);
    TEST_ASSERT( l2c_l2_read (l2c_dcr_base, L2C_L2MCKEN) == value, "Error setting L2MCKEN" );
}

void l2c_pmu_set_CE_bit( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index ) {
    l2c_pmu_write (pmu_dcr_base, dcr_index, l2c_pmu_read (pmu_dcr_base, dcr_index) | PMULCX_CE );
}

void l2c_pmu_enable_interrupt( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index, uint32_t const mask ) {
    l2c_pmu_write (pmu_dcr_base, dcr_index, l2c_pmu_read (pmu_dcr_base, dcr_index) | mask );
}

void l2c_pmu_disable_interrupt( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index, uint32_t const mask ) {
    l2c_pmu_write (pmu_dcr_base, dcr_index, l2c_pmu_read (pmu_dcr_base, dcr_index) & ~mask );
}

void l2c_pmu_clear_interrupt( uint32_t const pmu_dcr_base ) {
    l2c_pmu_write (pmu_dcr_base, L2C_PMUIS0, l2c_pmu_read (pmu_dcr_base, L2C_PMUIS0) );
}

void l2c_pmu_set_cx( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index, uint32_t const value ) {
    l2c_pmu_write (pmu_dcr_base, dcr_index, value);
}

void l2c_enable_interrupt( uint32_t const l2c_dcr_base, L2C_L2REG const l2c_int_reg_index, uint32_t const mask ) {
    l2c_l2_write (l2c_dcr_base, l2c_int_reg_index, l2c_l2_read(l2c_dcr_base, l2c_int_reg_index) | mask );
}

#define L2C_ARRACC_TIMEOUT                 (0x00000020)

bool l2c_arracc_tag_info_read_by_way(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint32_t * const tag_info
) {
    bool valid = false;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_tag_info_read_by_way\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
    rumboot_printf ("cache_way == %d\n", cache_way);
#endif

    l2c_l2_write (base, L2C_L2ARRACCADR, ((phys_addr >> L2C_ARRAY_ADDR_TAG_i) << L2C_L2ARRACCADR_TAG_i) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
                        ( (1 << L2C_L2ARRACCCTL_REQUEST_i)
                        | (L2C_ARRACC_BufferID_TAG << L2C_L2ARRACCCTL_BUFFERID_i)
                        | (L2C_ARRACC_ReqType_RD8WOECC << L2C_L2ARRACCCTL_REQTYPE_i)
                        | (cache_way << L2C_L2ARRACCCTL_L2WAY_i) ));

    int t = 0;
    while ( (valid = (t < L2C_ARRACC_TIMEOUT))
         && !(l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_READREQUESTCOMP_mask) )
        t++;

    if (valid) {
        *tag_info = l2c_l2_read (base, L2C_L2ARRACCDO0);
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("tag_info == %x\n", *tag_info);
#endif
    } else {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_tag_info_write_by_way_wo_gen_ecc(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint32_t const cache_data, uint32_t const ecc_data
) {
    bool valid = false;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_tag_info_write_by_way_wo_gen_ecc\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
    rumboot_printf ("cache_data == %x\n", cache_data);
    rumboot_printf ("ecc_data == %x\n", ecc_data);
#endif

    l2c_l2_write (base, L2C_L2ARRACCDI0, cache_data);
    l2c_l2_write (base, L2C_L2ARRACCDI2, ecc_data);

    l2c_l2_write (base, L2C_L2ARRACCADR, ((phys_addr >> L2C_ARRAY_ADDR_TAG_i) << L2C_L2ARRACCADR_TAG_i) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
                        ( (1 << L2C_L2ARRACCCTL_REQUEST_i)
                        | (L2C_ARRACC_BufferID_TAG << L2C_L2ARRACCCTL_BUFFERID_i)
                        | (L2C_ARRACC_ReqType_WR8WOECC << L2C_L2ARRACCCTL_REQTYPE_i)
                        | (cache_way << L2C_L2ARRACCCTL_L2WAY_i) ));

    int t = 0;
    while ( (valid = (t < L2C_ARRACC_TIMEOUT))
         && !(l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_WRTREQUESTCOMP_mask) )
        t++;

    if (!valid) {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_lru_info_read(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr,
        uint32_t * const lru_info
) {
    bool valid = false;

    l2c_l2_write (base, L2C_L2ARRACCADR, ((phys_addr >> L2C_ARRAY_ADDR_LRU_i) << L2C_L2ARRACCADR_LRU_i) );

    l2c_l2_write( base, L2C_L2ARRACCCTL,
                        ( (1 << L2C_L2ARRACCCTL_REQUEST_i)
                        | (L2C_ARRACC_BufferID_LRU << L2C_L2ARRACCCTL_BUFFERID_i)
                        | (L2C_ARRACC_ReqType_RD8WOECC << L2C_L2ARRACCCTL_REQTYPE_i) ));

    int t = 0;
    while ( (valid = (t < L2C_ARRACC_TIMEOUT))
         && !(l2c_l2_read( base, L2C_L2ARRACCCTL ) & L2C_L2ARRACCCTL_READREQUESTCOMP_mask) )
        t++;

    if (valid) {
        *lru_info = l2c_l2_read( base, L2C_L2ARRACCDO0 );
    } else {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_data_read_by_way(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint64_t * const cache_data
) {
    bool valid = false;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_data_read_by_way\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
#endif

    l2c_l2_write (base, L2C_L2ARRACCADR, ((phys_addr >> L2C_ARRAY_ADDR_DATA_i) << L2C_L2ARRACCADR_DATA_i) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
                        ( (1 << L2C_L2ARRACCCTL_REQUEST_i)
                        | (L2C_ARRACC_BufferID_DATA << L2C_L2ARRACCCTL_BUFFERID_i)
                        | (L2C_ARRACC_ReqType_RD8WOECC << L2C_L2ARRACCCTL_REQTYPE_i)
                        | (cache_way << L2C_L2ARRACCCTL_L2WAY_i)
                        | ((0b10000000 >> GET_BITS(phys_addr, L2C_ARRAY_ADDR_DATA_DW_i, L2C_ARRAY_ADDR_DATA_DW_n)) << L2C_L2ARRACCCTL_MASKDW_i) ));

    int t = 0;
    while ( (valid = (t < L2C_ARRACC_TIMEOUT))
         && !(l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_READREQUESTCOMP_mask) )
        t++;

    if (valid) {
        *cache_data =  l2c_l2_read (base, L2C_L2ARRACCDO1) | ( (uint64_t) l2c_l2_read(base, L2C_L2ARRACCDO0) << 32 ) ;
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("cache_data upper == %x\n", (uint32_t) (*cache_data >> 32) );
        rumboot_printf ("cache_data lower == %x\n", (uint32_t) (*cache_data & 0xFFFFFFFF) );
#endif
    } else {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_data_write_by_way(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint64_t const cache_data
) {
    bool valid = false;
    uint32_t cache_data_upper = (uint32_t)(cache_data >> 32);
    uint32_t cache_data_lower = (uint32_t)(cache_data & 0xFFFFFFFF);

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_data_write_by_way\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf ("cache_data upper == %x\n", cache_data_upper );
    rumboot_printf ("cache_data lower == %x\n", cache_data_lower );
#endif

    l2c_l2_write (base, L2C_L2ARRACCDI0, cache_data_upper);
    l2c_l2_write (base, L2C_L2ARRACCDI1, cache_data_lower);

    l2c_l2_write (base, L2C_L2ARRACCADR, ((phys_addr >> L2C_ARRAY_ADDR_DATA_i) << L2C_L2ARRACCADR_DATA_i) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
                        ( (1 << L2C_L2ARRACCCTL_REQUEST_i)
                        | (L2C_ARRACC_BufferID_DATA << L2C_L2ARRACCCTL_BUFFERID_i)
                        | (L2C_ARRACC_ReqType_WR8WTECC << L2C_L2ARRACCCTL_REQTYPE_i)
                        | (cache_way << L2C_L2ARRACCCTL_L2WAY_i)
                        | ((0b10000000 >> GET_BITS(phys_addr, L2C_ARRAY_ADDR_DATA_DW_i, L2C_ARRAY_ADDR_DATA_DW_n)) << L2C_L2ARRACCCTL_MASKDW_i) ));

    int t = 0;
    while ( (valid = (t < L2C_ARRACC_TIMEOUT))
         && !(l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_WRTREQUESTCOMP_mask) )
        t++;

    if (!valid) {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_data_write_by_way_wo_gen_ecc(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint64_t const cache_data, uint32_t const ecc_data
) {
    bool valid = false;
    uint32_t cache_data_upper = (uint32_t)(cache_data >> 32);
    uint32_t cache_data_lower = (uint32_t)(cache_data & 0xFFFFFFFF);

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_data_write_by_way_wo_gen_ecc\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
    rumboot_printf ("cache_data upper == %x\n", cache_data_upper );
    rumboot_printf ("cache_data lower == %x\n", cache_data_lower );
    rumboot_printf ("ecc_data == %x\n", ecc_data);
#endif

    l2c_l2_write (base, L2C_L2ARRACCDI0, cache_data_upper);
    l2c_l2_write (base, L2C_L2ARRACCDI1, cache_data_lower);
    l2c_l2_write (base, L2C_L2ARRACCDI2, ecc_data);

    l2c_l2_write (base, L2C_L2ARRACCADR, ((phys_addr >> L2C_ARRAY_ADDR_DATA_i) << L2C_L2ARRACCADR_DATA_i) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
                        ( (1 << L2C_L2ARRACCCTL_REQUEST_i)
                        | (L2C_ARRACC_BufferID_DATA << L2C_L2ARRACCCTL_BUFFERID_i)
                        | (L2C_ARRACC_ReqType_WR8WOECC << L2C_L2ARRACCCTL_REQTYPE_i)
                        | (cache_way << L2C_L2ARRACCCTL_L2WAY_i)
                        | ((0b10000000 >> GET_BITS(phys_addr, L2C_ARRAY_ADDR_DATA_DW_i, L2C_ARRAY_ADDR_DATA_DW_n)) << L2C_L2ARRACCCTL_MASKDW_i) ));

    int t = 0;
    while ( (valid = (t < L2C_ARRACC_TIMEOUT))
         && !(l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_WRTREQUESTCOMP_mask) )
        t++;

    if (!valid) {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_get_way_by_address(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr,
        int * const cache_way )
{
    uint64_t const full_phys_addr = ((uint64_t)ext_phys_addr) << 32 | phys_addr;
    uint32_t tag_info;
    bool tag_valid;
    uint32_t phys_addr_tag, tag_info_tag;

    struct l2c_mem_layout mem_layout;
    l2c_get_mem_layout( base, &mem_layout );

    uint32_t tag_address_n = mem_layout.tag_n-L2C_L2ARRACCDX0_TAG_CACHE_STATE_n;
    uint32_t tag_address_i = IBM_BIT_INDEX(42, tag_address_n-1);
    phys_addr_tag   = (uint32_t)GET_BITS(full_phys_addr, tag_address_i, tag_address_n);

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_get_way_by_address\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
    rumboot_printf ("tag_address_n == %d\n", tag_address_n);
    rumboot_printf ("tag_address_i == %d\n", tag_address_i);
    rumboot_printf ("mem_layout.tag_i == %d\n", mem_layout.tag_i);
    rumboot_printf ("phys_addr_tag == %x\n", phys_addr_tag);
#endif

    *cache_way = -1;

    do {
        (*cache_way)++;
        tag_valid       = l2c_arracc_tag_info_read_by_way (base, ext_phys_addr, phys_addr, *cache_way, &tag_info);
        tag_info_tag    = GET_BITS(tag_info, mem_layout.tag_i, tag_address_n);

#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("cache_way == %d\n", *cache_way);
        rumboot_printf ("tag_valid == %x\n", tag_valid);
        rumboot_printf ("tag_info_tag == %x\n", tag_info_tag);
#endif
    } while (
        (*cache_way < L2C_COUNT_WAYS-1)
    &&  ( !tag_valid || (phys_addr_tag != tag_info_tag) )
    );

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_get_way_by_address loop finished\n");
#endif

    if ( tag_valid && (phys_addr_tag == tag_info_tag) ) {
        return true;
    } else {
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("Tag address not found in Tag array.\n");
#endif
        return false;
    }
}

bool l2c_arracc_get_data_by_address(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr,
        uint64_t * const data64
) {
    int cache_way;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_get_data_by_address\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
#endif

    if (l2c_arracc_get_way_by_address (base, ext_phys_addr, phys_addr, &cache_way)) {
        return l2c_arracc_data_read_by_way (base, ext_phys_addr, phys_addr, cache_way, data64);
    }

    return false;
}

bool l2c_arracc_set_data_by_address(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr,
        uint64_t const data64
) {
    int cache_way;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_set_data_by_address\n" );
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf ("data64 upper == %x\n", (uint32_t)(data64 >> 32) );
    rumboot_printf ("data64 lower == %x\n", (uint32_t)(data64 & 0xFFFFFFFF) );
#endif

    if (l2c_arracc_get_way_by_address (base, ext_phys_addr, phys_addr, &cache_way)) {
        return l2c_arracc_data_write_by_way (base, ext_phys_addr, phys_addr, cache_way, data64);
    }

    return false;
}

uint64_t l2c_arracc_data_read(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr
) {
    uint64_t data64;

    bool get_data_by_address_ok = l2c_arracc_get_data_by_address( base, ext_phys_addr, phys_addr, &data64 );
    TEST_ASSERT( get_data_by_address_ok, "Data read failed" );

    return data64;
}

void l2c_arracc_data_write(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr,
        uint64_t const data64
) {
    bool set_data_by_address_ok = l2c_arracc_set_data_by_address( base, ext_phys_addr, phys_addr, data64 );
    TEST_ASSERT( set_data_by_address_ok, "Data write failed" );
}


void l2c_get_mem_layout(
        uint32_t const base,
        struct l2c_mem_layout * const mem_layout
) {
    mem_layout->l2size              = l2c_l2_read(base, L2C_L2CNFG0) & L2C_L2CNFG0_L2SIZE_mask;
    mem_layout->tag_ecc_i           = L2C_L2ARRACCDX2_TAG_ECC_i;
    mem_layout->tag_ecc_n           = L2C_L2ARRACCDX2_TAG_ECC_n;
    mem_layout->lru_i               = L2C_L2ARRACCDX0_LRU_i;
    mem_layout->lru_n               = L2C_L2ARRACCDX0_LRU_n;
    mem_layout->data_ecc_i          = L2C_L2ARRACCDX2_DATA_ECC_i;
    mem_layout->data_ecc_n          = L2C_L2ARRACCDX2_DATA_ECC_n;
    switch( mem_layout->l2size ) {
    case L2C_L2Size_128KB:
        mem_layout->lru_array_size  = 256;
        mem_layout->tag_i           = L2C_L2ARRACCDX0_TAG_128KB_i;
        mem_layout->tag_n           = L2C_L2ARRACCDX0_TAG_128KB_n;
        break;
    case L2C_L2Size_256KB:
        mem_layout->lru_array_size  = 512;
        mem_layout->tag_i           = L2C_L2ARRACCDX0_TAG_256KB_i;
        mem_layout->tag_n           = L2C_L2ARRACCDX0_TAG_256KB_n;
        break;
    case L2C_L2Size_512KB:
        mem_layout->lru_array_size  = 1024;
        mem_layout->tag_i           = L2C_L2ARRACCDX0_TAG_512KB_i;
        mem_layout->tag_n           = L2C_L2ARRACCDX0_TAG_512KB_n;
        break;
    case L2C_L2Size_1MB:
        mem_layout->lru_array_size  = 2048;
        mem_layout->tag_i           = L2C_L2ARRACCDX0_TAG_1MB_i;
        mem_layout->tag_n           = L2C_L2ARRACCDX0_TAG_1MB_n;
        break;
    }
    mem_layout->tag_array_size      = 4 * mem_layout->lru_array_size;
    mem_layout->data_array_size     = 8 * mem_layout->lru_array_size;
}
