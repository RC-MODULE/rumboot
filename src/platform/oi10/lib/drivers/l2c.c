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


struct mem_config {
    uint32_t    array_addr_tag_n;
    uint32_t    array_addr_lru_n;
    uint32_t    array_addr_data_n;

    uint32_t    l2arraccdx0_tag_i;
    uint32_t    l2arraccdx0_tag_n;

    uint32_t    lru_array_size;
};

static struct mem_config const mem_configs[4] = {
    [L2C_L2Size_128KB] = {
        L2C_ARRAY_ADDR_TAG_128KB_n, L2C_ARRAY_ADDR_LRU_128KB_n, L2C_ARRAY_ADDR_DATA_128KB_n+L2C_ARRAY_ADDR_DATA_DW_n,
        L2C_L2ARRACCDX0_TAG_128KB_i, L2C_L2ARRACCDX0_TAG_128KB_n,
        256
    },
    [L2C_L2Size_256KB] = {
        L2C_ARRAY_ADDR_TAG_256KB_n, L2C_ARRAY_ADDR_LRU_256KB_n, L2C_ARRAY_ADDR_DATA_256KB_n+L2C_ARRAY_ADDR_DATA_DW_n,
        L2C_L2ARRACCDX0_TAG_256KB_i, L2C_L2ARRACCDX0_TAG_256KB_n,
        512
    },
    [L2C_L2Size_512KB] = {
        L2C_ARRAY_ADDR_TAG_512KB_n, L2C_ARRAY_ADDR_LRU_512KB_n, L2C_ARRAY_ADDR_DATA_512KB_n+L2C_ARRAY_ADDR_DATA_DW_n,
        L2C_L2ARRACCDX0_TAG_512KB_i, L2C_L2ARRACCDX0_TAG_512KB_n,
        1024
    },
    [L2C_L2Size_1MB] = {
        L2C_ARRAY_ADDR_TAG_1MB_n, L2C_ARRAY_ADDR_LRU_1MB_n, L2C_ARRAY_ADDR_DATA_1MB_n+L2C_ARRAY_ADDR_DATA_DW_n,
        L2C_L2ARRACCDX0_TAG_1MB_i, L2C_L2ARRACCDX0_TAG_1MB_n,
        2048
    }
};

#define L2C_ARRACC_TIMEOUT                 (0xffffffff)

static inline __attribute__((always_inline)) bool l2c_arracc_read(
        uint32_t const base,
        uint32_t const array_addr,
        uint32_t const l2arraccctl_fields
) {
    bool valid = false;

    l2c_l2_write( base, L2C_L2ARRACCADR, array_addr );

    l2c_l2_write( base, L2C_L2ARRACCCTL,
                        ( l2arraccctl_fields
                        | (1 << L2C_L2ARRACCCTL_REQUEST_i)
                        | (L2C_ARRACC_ReqType_RD8WOECC << L2C_L2ARRACCCTL_REQTYPE_i) ));

    int t = 0;
    while ( (valid = (t < L2C_ARRACC_TIMEOUT))
         && !(l2c_l2_read( base, L2C_L2ARRACCCTL ) & L2C_L2ARRACCCTL_READREQUESTCOMP_mask) )
        t++;

    if( !valid ) {
        rumboot_printf( "Timeout while reading L2C array\n" );
    }

    return valid;
}

static inline __attribute__((always_inline)) bool l2c_arracc_write(
        uint32_t const base,
        uint32_t const array_addr,
        uint32_t const l2arraccctl_fields
) {
    bool valid = false;

    l2c_l2_write( base, L2C_L2ARRACCADR, array_addr );

    l2c_l2_write( base, L2C_L2ARRACCCTL,
                        ( l2arraccctl_fields
                        | (1 << L2C_L2ARRACCCTL_REQUEST_i) ));

    int t = 0;
    while ( (valid = (t < L2C_ARRACC_TIMEOUT))
         && !(l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_WRTREQUESTCOMP_mask) )
        t++;

    if( !valid ) {
        rumboot_printf( "Timeout while writing L2C array\n" );
    }

    return valid;
}

static inline __attribute__((always_inline)) bool l2c_arracc_tag_info_wt_ecc_read_raw(
        uint32_t const base,
        uint32_t const tag_addr, int const cache_way,
        uint32_t * const tag_info,
        uint32_t * const tag_ecc
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_tag_info_wt_ecc_read_raw\n" );
    rumboot_printf( "tag_addr: %x\n", tag_addr );
    rumboot_printf( "cache_way: %x\n", cache_way );
#endif
    if( l2c_arracc_read(
            base,
            (tag_addr << L2C_L2ARRACCADR_TAG_i),
            ( (L2C_ARRACC_BufferID_TAG << L2C_L2ARRACCCTL_BUFFERID_i)
            | (cache_way << L2C_L2ARRACCCTL_L2WAY_i) )
        )
    ) {
        *tag_info   = l2c_l2_read( base, L2C_L2ARRACCDO0 );
        *tag_ecc    = l2c_l2_read( base, L2C_L2ARRACCDO2 );
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf( "tag_info: %x\n", *tag_info );
        rumboot_printf( "tag_ecc: %x\n", *tag_ecc );
#endif
        return true;
    }

    return false;
}

bool l2c_arracc_tag_info_read_by_way(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint32_t * const tag_info
) {
    uint32_t tag_ecc;
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_tag_info_read_by_way\n" );
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf( "cache_way: %x\n", cache_way );
#endif
    return l2c_arracc_tag_info_wt_ecc_read_raw( base, (phys_addr >> L2C_ARRAY_ADDR_TAG_i), cache_way, tag_info, &tag_ecc );
}

bool l2c_arracc_tag_ecc_read_by_way(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint32_t * const tag_ecc
) {
    uint32_t tag_info;
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_tag_ecc_read_by_way\n" );
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf( "cache_way: %x\n", cache_way );
#endif
    return l2c_arracc_tag_info_wt_ecc_read_raw( base, (phys_addr >> L2C_ARRAY_ADDR_TAG_i), cache_way, &tag_info, tag_ecc );
}

static inline __attribute__((always_inline)) bool l2c_arracc_tag_info_write_raw(
        uint32_t const base,
        uint32_t const tag_addr, int const cache_way,
        uint32_t const tag_info
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_tag_info_write_raw\n" );
    rumboot_printf( "tag_addr: %x\n", tag_addr );
    rumboot_printf( "cache_way: %x\n", cache_way );
    rumboot_printf( "tag_info: %x\n", tag_info );
#endif
    l2c_l2_write (base, L2C_L2ARRACCDI0, tag_info);

    return l2c_arracc_write(
            base,
            (tag_addr << L2C_L2ARRACCADR_TAG_i),
            ( (L2C_ARRACC_BufferID_TAG << L2C_L2ARRACCCTL_BUFFERID_i)
            | (L2C_ARRACC_ReqType_WR8WTECC << L2C_L2ARRACCCTL_REQTYPE_i)
            | (cache_way << L2C_L2ARRACCCTL_L2WAY_i) )
        );
}

static inline __attribute__((always_inline)) bool l2c_arracc_tag_info_wt_ecc_write_raw(
        uint32_t const base,
        uint32_t const tag_addr, int const cache_way,
        uint32_t const tag_info, uint32_t const tag_ecc
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_tag_info_write_wt_ecc_raw\n" );
    rumboot_printf( "tag_addr: %x\n", tag_addr );
    rumboot_printf( "cache_way: %x\n", cache_way );
    rumboot_printf( "tag_info: %x\n", tag_info );
    rumboot_printf( "tag_ecc: %x\n", tag_ecc );
#endif
    l2c_l2_write (base, L2C_L2ARRACCDI0, tag_info);
    l2c_l2_write (base, L2C_L2ARRACCDI2, tag_ecc);

    return l2c_arracc_write(
            base,
            (tag_addr << L2C_L2ARRACCADR_TAG_i),
            ( (L2C_ARRACC_BufferID_TAG << L2C_L2ARRACCCTL_BUFFERID_i)
            | (L2C_ARRACC_ReqType_WR8WOECC << L2C_L2ARRACCCTL_REQTYPE_i)
            | (cache_way << L2C_L2ARRACCCTL_L2WAY_i) )
        );
}

bool l2c_arracc_tag_info_write_by_way(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint32_t const tag_info
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_tag_info_write_by_way\n" );
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr );
#endif
    return l2c_arracc_tag_info_write_raw( base, (phys_addr >> L2C_ARRAY_ADDR_TAG_i), cache_way, tag_info );
}

bool l2c_arracc_tag_info_write_by_way_wo_gen_ecc(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint32_t const tag_info, uint32_t const tag_ecc
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_tag_info_write_by_way_wo_gen_ecc\n" );
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr );
#endif
    return l2c_arracc_tag_info_wt_ecc_write_raw( base, (phys_addr >> L2C_ARRAY_ADDR_TAG_i), cache_way, tag_info, tag_ecc );
}

bool l2c_arracc_get_way_by_address(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr,
        int * const cache_way )
{
    struct l2c_mem_layout mem_layout;
    l2c_get_mem_layout( base, &mem_layout );

    uint32_t const tag_address_n    = mem_layout.tag_n - L2C_L2ARRACCDX0_TAG_CACHE_STATE_n;
    uint32_t const tag_address_i    = IBM_BIT_INDEX( 42, tag_address_n-1 );

    uint64_t const full_phys_addr   = ((uint64_t)ext_phys_addr) << 32 | phys_addr;
    uint32_t const phys_addr_tag    = (uint32_t)GET_BITS( full_phys_addr, tag_address_i, tag_address_n );

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_get_way_by_address\n");
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr);
    rumboot_printf( "tag_address_n: %d\n", tag_address_n);
    rumboot_printf( "tag_address_i: %d\n", tag_address_i);
    rumboot_printf( "mem_layout.tag_i: %d\n", mem_layout.tag_i);
    rumboot_printf( "phys_addr_tag: %x\n", phys_addr_tag);
#endif

    uint32_t tag_info;
    bool tag_valid;
    uint32_t tag_info_tag;
    int current_cache_way = -1;
    do {
        current_cache_way++;
        tag_valid       = l2c_arracc_tag_info_read_by_way( base, ext_phys_addr, phys_addr, current_cache_way, &tag_info );
        tag_info_tag    = GET_BITS(tag_info, mem_layout.tag_i, tag_address_n);

#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("cache_way: %d\n", current_cache_way);
        rumboot_printf ("tag_valid: %x\n", tag_valid);
        rumboot_printf ("tag_info_tag: %x\n", tag_info_tag);
#endif
    } while (
        (current_cache_way < L2C_COUNT_WAYS-1)
    &&  ( !tag_valid || (phys_addr_tag != tag_info_tag) )
    );

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_get_way_by_address loop finished\n");
#endif

    if ( tag_valid && (phys_addr_tag == tag_info_tag) ) {
        *cache_way = current_cache_way;
        return true;
    } else {
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("Tag address not found in Tag array.\n");
#endif
        return false;
    }
}

static inline __attribute__((always_inline)) bool l2c_arracc_lru_info_read_raw(
        uint32_t const base,
        uint32_t const lru_addr,
        uint32_t * const lru_info
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_lru_info_read_raw\n" );
    rumboot_printf( "lru_addr: %x\n", lru_addr );
#endif
    if( l2c_arracc_read(
            base,
            (lru_addr << L2C_L2ARRACCADR_LRU_i),
            (L2C_ARRACC_BufferID_LRU << L2C_L2ARRACCCTL_BUFFERID_i)
        )
    ) {
        *lru_info = l2c_l2_read( base, L2C_L2ARRACCDO0 );
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf("lru_info: %x\n", *lru_info );
#endif
        return true;
    }

    return false;
}

bool l2c_arracc_lru_info_read(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr,
        uint32_t * const lru_info
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_lru_info_read\n" );
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr );
#endif

    return l2c_arracc_lru_info_read_raw( base, (phys_addr >> L2C_ARRAY_ADDR_LRU_i), lru_info );
}

static inline __attribute__((always_inline)) bool l2c_arracc_lru_info_write_raw(
        uint32_t const base,
        uint32_t const lru_addr,
        uint32_t const lru_info
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_lru_info_write_raw\n" );
    rumboot_printf( "lru_addr: %x\n", lru_addr );
    rumboot_printf( "lru_info: %x\n", lru_info );
#endif
    l2c_l2_write( base, L2C_L2ARRACCDI0, lru_info );

    return l2c_arracc_write(
            base,
            (lru_addr << L2C_L2ARRACCADR_LRU_i),
            ( (L2C_ARRACC_BufferID_LRU << L2C_L2ARRACCCTL_BUFFERID_i)
            | (L2C_ARRACC_ReqType_WR8WOECC << L2C_L2ARRACCCTL_REQTYPE_i) )
        );
}

bool l2c_arracc_lru_info_write(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr,
        uint32_t const lru_info
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_lru_info_write\n" );
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf( "lru_info: %x\n", lru_info );
#endif
    return l2c_arracc_lru_info_write_raw( base, (phys_addr >> L2C_ARRAY_ADDR_LRU_i), lru_info );
}

static inline __attribute__((always_inline)) bool l2c_arracc_data_wt_ecc_read_raw(
        uint32_t const base,
        uint32_t const data_addr, int const cache_way,
        uint64_t * const cache_data,
        uint32_t * const cache_data_ecc
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_data_read_raw\n" );
    rumboot_printf( "data_addr: %x\n", data_addr );
    rumboot_printf( "cache_way: %x\n", cache_way );
#endif
    if( l2c_arracc_read(
            base,
            ((data_addr >> L2C_ARRAY_ADDR_DATA_DW_n) << L2C_L2ARRACCADR_DATA_i),
            ( (L2C_ARRACC_BufferID_DATA << L2C_L2ARRACCCTL_BUFFERID_i)
            | (cache_way << L2C_L2ARRACCCTL_L2WAY_i)
            | ((0b10000000 >> GET_BITS(data_addr, 0, L2C_ARRAY_ADDR_DATA_DW_n)) << L2C_L2ARRACCCTL_MASKDW_i) )
        )
    ) {
        *cache_data     = l2c_l2_read( base, L2C_L2ARRACCDO1 ) | ((uint64_t)l2c_l2_read( base, L2C_L2ARRACCDO0 ) << 32);
        *cache_data_ecc = l2c_l2_read( base, L2C_L2ARRACCDO2 );
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf( "cache_data upper: %x\n", (uint32_t)(*cache_data >> 32) );
        rumboot_printf( "cache_data lower: %x\n", (uint32_t)(*cache_data & 0xFFFFFFFF) );
        rumboot_printf( "cache_data_ecc: %x\n", cache_data_ecc );
#endif
        return true;
    }

    return false;
}

bool l2c_arracc_data_read_by_way(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint64_t * const cache_data
) {
    uint32_t cache_data_ecc;
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_data_read_by_way\n" );
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf( "cache_way: %x\n", cache_way );
#endif
    return l2c_arracc_data_wt_ecc_read_raw( base, (phys_addr >> L2C_ARRAY_ADDR_DATA_DW_i), cache_way, cache_data, &cache_data_ecc );
}

bool l2c_arracc_data_ecc_read_by_way(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint32_t * const cache_data_ecc
) {
    uint64_t cache_data;
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_data_ecc_read_by_way\n" );
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf( "cache_way: %x\n", cache_way );
#endif
    return l2c_arracc_data_wt_ecc_read_raw( base, (phys_addr >> L2C_ARRAY_ADDR_DATA_DW_i), cache_way, &cache_data, cache_data_ecc );
}

static inline __attribute__((always_inline)) bool l2c_arracc_data_write_raw(
        uint32_t const base,
        uint32_t const data_addr, int const cache_way,
        uint64_t const cache_data
) {
    uint32_t const cache_data_upper = (uint32_t)(cache_data >> 32);
    uint32_t const cache_data_lower = (uint32_t)(cache_data & 0xFFFFFFFF);
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_data_write_raw\n" );
    rumboot_printf( "data_addr: %x\n", data_addr );
    rumboot_printf( "cache_way: %x\n", cache_way );
    rumboot_printf( "cache_data upper: %x\n", cache_data_upper );
    rumboot_printf( "cache_data lower: %x\n", cache_data_lower );
#endif
    l2c_l2_write( base, L2C_L2ARRACCDI0, cache_data_upper );
    l2c_l2_write( base, L2C_L2ARRACCDI1, cache_data_lower );

    return l2c_arracc_write(
            base,
            ((data_addr >> L2C_ARRAY_ADDR_DATA_DW_n) << L2C_L2ARRACCADR_DATA_i),
            ( (L2C_ARRACC_BufferID_DATA << L2C_L2ARRACCCTL_BUFFERID_i)
            | (L2C_ARRACC_ReqType_WR8WTECC << L2C_L2ARRACCCTL_REQTYPE_i)
            | (cache_way << L2C_L2ARRACCCTL_L2WAY_i)
            | ((0b10000000 >> GET_BITS(data_addr, 0, L2C_ARRAY_ADDR_DATA_DW_n)) << L2C_L2ARRACCCTL_MASKDW_i) )
        );
}

static inline __attribute__((always_inline)) bool l2c_arracc_data_wt_ecc_write_raw(
        uint32_t const base,
        uint32_t const data_addr, int const cache_way,
        uint64_t const cache_data, uint32_t const cache_data_ecc
) {
    uint32_t const cache_data_upper = (uint32_t)(cache_data >> 32);
    uint32_t const cache_data_lower = (uint32_t)(cache_data & 0xFFFFFFFF);
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_data_write_wt_ecc_raw\n" );
    rumboot_printf( "data_addr: %x\n", data_addr );
    rumboot_printf( "cache_way: %x\n", cache_way );
    rumboot_printf( "cache_data upper: %x\n", cache_data_upper );
    rumboot_printf( "cache_data lower: %x\n", cache_data_lower );
    rumboot_printf( "cache_data_ecc: %x\n", cache_data_ecc );
#endif
    l2c_l2_write( base, L2C_L2ARRACCDI0, cache_data_upper );
    l2c_l2_write( base, L2C_L2ARRACCDI1, cache_data_lower );
    l2c_l2_write( base, L2C_L2ARRACCDI2, cache_data_ecc );

    return l2c_arracc_write(
            base,
            ((data_addr >> L2C_ARRAY_ADDR_DATA_DW_n) << L2C_L2ARRACCADR_DATA_i),
            ( (L2C_ARRACC_BufferID_DATA << L2C_L2ARRACCCTL_BUFFERID_i)
            | (L2C_ARRACC_ReqType_WR8WOECC << L2C_L2ARRACCCTL_REQTYPE_i)
            | (cache_way << L2C_L2ARRACCCTL_L2WAY_i)
            | ((0b10000000 >> GET_BITS(data_addr, 0, L2C_ARRAY_ADDR_DATA_DW_n)) << L2C_L2ARRACCCTL_MASKDW_i) )
        );
}

bool l2c_arracc_data_write_by_way(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint64_t const cache_data
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_data_write_by_way\n" );
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf( "cache_data upper: %x\n", (uint32_t)(cache_data >> 32) );
    rumboot_printf( "cache_data lower: %x\n", (uint32_t)(cache_data & 0xFFFFFFFF) );
#endif
    return l2c_arracc_data_write_raw( base, (phys_addr >> L2C_ARRAY_ADDR_DATA_DW_i), cache_way, cache_data );
}

bool l2c_arracc_data_write_by_way_wo_gen_ecc(
        uint32_t const base,
        uint32_t const ext_phys_addr, uint32_t const phys_addr, int const cache_way,
        uint64_t const cache_data, uint32_t const cache_data_ecc
) {
#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf( "l2c_arracc_data_write_by_way_wo_gen_ecc\n" );
    rumboot_printf( "full_phys_addr: %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf( "cache_data upper: %x\n", (uint32_t)(cache_data >> 32) );
    rumboot_printf( "cache_data lower: %x\n", (uint32_t)(cache_data & 0xFFFFFFFF) );
    rumboot_printf( "cache_data_ecc: %x\n", cache_data_ecc );
#endif
    return l2c_arracc_data_wt_ecc_write_raw( base, (phys_addr >> L2C_ARRAY_ADDR_DATA_DW_i), cache_way, cache_data, cache_data_ecc );
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
    mem_layout->l2size          = l2c_l2_read(base, L2C_L2CNFG0) & L2C_L2CNFG0_L2SIZE_mask;

    switch (mem_layout->l2size)
    {
    case L2C_L2Size_128KB:
        mem_layout->l2size_bytes = 128 * 1024;
        break;
    case L2C_L2Size_256KB:
        mem_layout->l2size_bytes = 256 * 1024;
        break;
    case L2C_L2Size_512KB:
        mem_layout->l2size_bytes = 512 * 1024;
        break;
    case L2C_L2Size_1MB:
        mem_layout->l2size_bytes = 1 * 1024 * 1024;
        break;
    }

    mem_layout->tag_ecc_i       = L2C_L2ARRACCDX2_TAG_ECC_i;
    mem_layout->tag_ecc_n       = L2C_L2ARRACCDX2_TAG_ECC_n;

    mem_layout->lru_i           = L2C_L2ARRACCDX0_LRU_i;
    mem_layout->lru_n           = L2C_L2ARRACCDX0_LRU_n;

    mem_layout->data_ecc_i      = L2C_L2ARRACCDX2_DATA_ECC_i;
    mem_layout->data_ecc_n      = L2C_L2ARRACCDX2_DATA_ECC_n;

    mem_layout->tag_i           = mem_configs[mem_layout->l2size].l2arraccdx0_tag_i;
    mem_layout->tag_n           = mem_configs[mem_layout->l2size].l2arraccdx0_tag_n;

    mem_layout->lru_array_size  = mem_configs[mem_layout->l2size].lru_array_size;
    mem_layout->tag_array_size  = 4 * mem_layout->lru_array_size;
    mem_layout->data_array_size = 8 * mem_layout->lru_array_size;
}

uint64_t l2c_read_mem(
        uint32_t const base,
        struct l2c_mem_layout const * const mem_layout, l2c_mem_t const mem_type, uint32_t const index
) {
    switch( mem_type ) {
    case L2C_MEM_TAG: {
        uint32_t    tag_addr    = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_tag_n );
        int         cache_way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_tag_n, L2C_L2ARRACCCTL_L2WAY_n );
        uint32_t    tag_info;
        uint32_t    tag_ecc;

        bool tag_info_with_ecc_read_ok = l2c_arracc_tag_info_wt_ecc_read_raw( base, tag_addr, cache_way, &tag_info, &tag_ecc );
        TEST_ASSERT( tag_info_with_ecc_read_ok, "TAG info read failed" );

        return tag_info;
    }
    case L2C_MEM_TAG_ECC: {
        uint32_t    tag_addr    = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_tag_n );
        int         cache_way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_tag_n, L2C_L2ARRACCCTL_L2WAY_n );
        uint32_t    tag_info;
        uint32_t    tag_ecc;

        bool tag_info_with_ecc_read_ok = l2c_arracc_tag_info_wt_ecc_read_raw( base, tag_addr, cache_way, &tag_info, &tag_ecc );
        TEST_ASSERT( tag_info_with_ecc_read_ok, "TAG ECC read failed" );

        return tag_ecc;
    }
    case L2C_MEM_LRU: {
        uint32_t    lru_addr    = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_lru_n );
        uint32_t    lru_info;

        bool lru_info_read_ok = l2c_arracc_lru_info_read_raw( base, lru_addr, &lru_info );
        TEST_ASSERT( lru_info_read_ok, "LRU info read failed" );

        return lru_info;
    }
    case L2C_MEM_DATA: {
        uint32_t    data_addr   = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_data_n );
        int         cache_way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_data_n, L2C_L2ARRACCCTL_L2WAY_n );
        uint64_t    cache_data;
        uint32_t    cache_data_ecc;

        bool cache_data_read_ok = l2c_arracc_data_wt_ecc_read_raw( base, data_addr, cache_way, &cache_data, &cache_data_ecc );
        TEST_ASSERT( cache_data_read_ok, "DATA read failed" );

        return cache_data;
    }
    case L2C_MEM_DATA_ECC: {
        uint32_t    data_addr   = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_data_n );
        int         cache_way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_data_n, L2C_L2ARRACCCTL_L2WAY_n );
        uint64_t    cache_data;
        uint32_t    cache_data_ecc;

        bool cache_data_read_ok = l2c_arracc_data_wt_ecc_read_raw( base, data_addr, cache_way, &cache_data, &cache_data_ecc );
        TEST_ASSERT( cache_data_read_ok, "DATA ECC read failed" );

        return cache_data_ecc;
    }
    default:
        TEST_ASSERT( 0, "Unknown mem type" );
        return 0;
    }
}


int l2_data_address_encode(struct l2c_mem_layout *mem_layout, int way, int addr, int subaddr)
{
    int ret = (addr << L2C_ARRAY_ADDR_DATA_DW_n) | subaddr;
    ret |= (way << mem_configs[mem_layout->l2size].array_addr_data_n);
    return ret;
}


void l2_data_address_decode(struct l2c_mem_layout *mem_layout, int index, int *way, int *addr, int *subaddr)
{
    *addr    = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_data_n);
    *subaddr = *addr & 0x7;
    *addr    = *addr >> L2C_ARRAY_ADDR_DATA_DW_n;
    *way     = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_data_n, L2C_L2ARRACCCTL_L2WAY_n);
}

int l2_tag_address_encode(struct l2c_mem_layout *mem_layout, int way, int addr)
{
    return (way << mem_configs[mem_layout->l2size].array_addr_tag_n) | addr;
}

void l2_tag_address_decode(struct l2c_mem_layout *mem_layout, int index, int *way, int *addr)
{
    *addr    = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_tag_n );
    *way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_tag_n, L2C_L2ARRACCCTL_L2WAY_n );
}

void l2c_tag_directwrite(uint32_t const dcr_base, struct l2c_mem_layout *mem_layout, int index, uint32_t log2, uint32_t log1)
{
    int way, addr;
    l2_tag_address_decode(mem_layout, index, &way, &addr);
    l2c_arracc_tag_info_wt_ecc_write_raw( dcr_base, addr, way, log2, log1 );
}

void l2c_tag_regen(uint32_t const base, struct l2c_mem_layout *mem_layout, int index)
{
    int way, addr;
    uint32_t tag, ecc;
    bool ret;
    l2_tag_address_decode(mem_layout, index, &way, &addr);
    ret = l2c_arracc_tag_info_wt_ecc_read_raw(base, addr, way, &tag, &ecc);
}


void l2c_write_mem(
        uint32_t const base,
        struct l2c_mem_layout const * const mem_layout, l2c_mem_t const mem_type, uint32_t const index,
        uint64_t const data
) {
    switch( mem_type ) {
    case L2C_MEM_TAG: {
        uint32_t    tag_addr    = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_tag_n );
        int         cache_way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_tag_n, L2C_L2ARRACCCTL_L2WAY_n );
        uint32_t    tag_info_old, tag_ecc_old;

        bool tag_info_with_ecc_read_ok = l2c_arracc_tag_info_wt_ecc_read_raw( base, tag_addr, cache_way, &tag_info_old, &tag_ecc_old );
        TEST_ASSERT( tag_info_with_ecc_read_ok, "TAG ECC read failed" );
        bool tag_info_write_ok = l2c_arracc_tag_info_wt_ecc_write_raw( base, tag_addr, cache_way, (uint32_t)data, tag_ecc_old );
        TEST_ASSERT( tag_info_write_ok, "TAG info write failed" );
        return;
    }
    case L2C_MEM_TAG_ECC: {
        uint32_t    tag_addr    = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_tag_n );
        int         cache_way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_tag_n, L2C_L2ARRACCCTL_L2WAY_n );
        uint32_t    tag_info_old, tag_ecc_old;

        bool tag_info_with_ecc_read_ok = l2c_arracc_tag_info_wt_ecc_read_raw( base, tag_addr, cache_way, &tag_info_old, &tag_ecc_old );
        TEST_ASSERT( tag_info_with_ecc_read_ok, "TAG info read failed" );
        bool tag_info_write_ok = l2c_arracc_tag_info_wt_ecc_write_raw( base, tag_addr, cache_way, tag_info_old, (uint32_t)data );
        TEST_ASSERT( tag_info_write_ok, "TAG ECC write failed" );
        return;
    }
    case L2C_MEM_LRU: {
        uint32_t    lru_addr    = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_lru_n );

        bool lru_info_read_ok = l2c_arracc_lru_info_write_raw( base, lru_addr, (uint32_t)data );
        TEST_ASSERT( lru_info_read_ok, "LRU info write failed" );
        return;
    }
    case L2C_MEM_DATA: {
        uint32_t    data_addr   = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_data_n );
        int         cache_way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_data_n, L2C_L2ARRACCCTL_L2WAY_n );
        uint64_t    cache_data_old;
        uint32_t    cache_data_ecc_old;

        bool cache_data_read_ok = l2c_arracc_data_wt_ecc_read_raw( base, data_addr, cache_way, &cache_data_old, &cache_data_ecc_old );
        TEST_ASSERT( cache_data_read_ok, "DATA ECC read failed" );
        bool cache_data_write_ok = l2c_arracc_data_wt_ecc_write_raw( base, data_addr, cache_way, data, cache_data_ecc_old );
        TEST_ASSERT( cache_data_write_ok, "DATA write failed" );
        return;
    }
    case L2C_MEM_DATA_ECC: {
        uint32_t    data_addr   = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_data_n );
        int         cache_way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_data_n, L2C_L2ARRACCCTL_L2WAY_n );
        uint64_t    cache_data_old;
        uint32_t    cache_data_ecc_old;

        bool cache_data_read_ok = l2c_arracc_data_wt_ecc_read_raw( base, data_addr, cache_way, &cache_data_old, &cache_data_ecc_old );
        TEST_ASSERT( cache_data_read_ok, "DATA read failed" );
        bool cache_data_write_ok = l2c_arracc_data_wt_ecc_write_raw( base, data_addr, cache_way, cache_data_old, (uint32_t)data );
        TEST_ASSERT( cache_data_write_ok, "DATA ECC write failed" );
        return;
    }
    case L2C_MEM_DATAECC_CLEAR: {
        uint32_t    data_addr   = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_data_n );
        int         cache_way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_data_n, L2C_L2ARRACCCTL_L2WAY_n );
        bool cache_data_write_ok = l2c_arracc_data_wt_ecc_write_raw( base, data_addr, cache_way, 0x0, 0x0 );
        TEST_ASSERT( cache_data_write_ok, "DATA/ECC write failed" );
        return;
    }
    case L2C_MEM_TAGECC_CLEAR: {
        uint32_t    tag_addr    = GET_BITS( index, 0, mem_configs[mem_layout->l2size].array_addr_tag_n );
        int         cache_way   = GET_BITS( index, mem_configs[mem_layout->l2size].array_addr_tag_n, L2C_L2ARRACCCTL_L2WAY_n );
        bool tag_info_write_ok = l2c_arracc_tag_info_wt_ecc_write_raw( base, tag_addr, cache_way, 0x0, 0x0 );
        TEST_ASSERT( tag_info_write_ok, "TAG ECC write failed" );
        return;
    }

    default:
        TEST_ASSERT( 0, "Unknown mem type" );
        return;
    }
}
