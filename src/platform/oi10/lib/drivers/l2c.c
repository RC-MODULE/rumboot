#include <stddef.h>
#include <rumboot/macros.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/devices/l2c.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c_pmu.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/arch/ppc/test_macro.h>
#include <rumboot/printf.h>

//#define L2C_TRACE_DEBUG_MSG

void l2c_global_enable_interrupt( uint32_t const l2c_dcr_base, L2INTEN_bits_t const mask )
{
    l2c_l2_write (l2c_dcr_base, L2C_L2INTEN, l2c_l2_read (l2c_dcr_base, L2C_L2INTEN) | mask );
}

void l2c_global_mck_enable( uint32_t const l2c_dcr_base, L2MCKEN_bits_t const mask )
{
    l2c_l2_write (l2c_dcr_base, L2C_L2MCKEN, l2c_l2_read (l2c_dcr_base, L2C_L2MCKEN) | mask );
}

void l2c_global_mck_disable( uint32_t const l2c_dcr_base, L2MCKEN_bits_t const mask )
{
    uint32_t value = l2c_l2_read (l2c_dcr_base, L2C_L2MCKEN) & ~mask;
    l2c_l2_write (l2c_dcr_base, L2C_L2MCKEN, value);
    TEST_ASSERT( l2c_l2_read (l2c_dcr_base, L2C_L2MCKEN) == value, "Error setting L2MCKEN" );
}

void l2c_pmu_set_CE_bit( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index )
{
    l2c_pmu_write (pmu_dcr_base, dcr_index, l2c_pmu_read (pmu_dcr_base, dcr_index) | PMULCX_CE );
}

void l2c_pmu_enable_interrupt( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index, uint32_t const mask )
{
    l2c_pmu_write (pmu_dcr_base, dcr_index, l2c_pmu_read (pmu_dcr_base, dcr_index) | mask );
}

void l2c_pmu_disable_interrupt( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index, uint32_t const mask )
{
    l2c_pmu_write (pmu_dcr_base, dcr_index, l2c_pmu_read (pmu_dcr_base, dcr_index) & ~mask );
}

void l2c_pmu_clear_interrupt( uint32_t const pmu_dcr_base )
{
    l2c_pmu_write (pmu_dcr_base, L2C_PMUIS0, l2c_pmu_read (pmu_dcr_base, L2C_PMUIS0) );
}

void l2c_pmu_set_cx( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index, uint32_t const value )
{
    l2c_pmu_write (pmu_dcr_base, dcr_index, value);
}

void l2c_enable_interrupt( uint32_t const l2c_dcr_base, L2C_L2REG const l2c_int_reg_index, uint32_t const mask )
{
    l2c_l2_write (l2c_dcr_base, l2c_int_reg_index, l2c_l2_read(l2c_dcr_base, l2c_int_reg_index) | mask );
}

bool l2c_arracc_tag_info_read_by_way( uint32_t const base, uint32_t const ext_phys_addr, uint32_t const phys_addr, int32_t const cache_way,
        volatile uint32_t* tag_info )
{
    int32_t  indx = 0;
    uint32_t l2arraccadr;
    uint32_t l2arraccctl;
    bool valid = false;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_tag_info_read_by_way\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
    rumboot_printf ("cache_way == %x\n", cache_way);
#endif

    l2arraccadr = l2c_l2_read (base, L2C_L2ARRACCADR) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read (base, L2C_L2ARRACCCTL) & ~L2C_ARRACCCTL_MSK;

    l2c_l2_write (base, L2C_L2ARRACCADR, l2arraccadr | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
           (l2arraccctl | L2C_ARRACCCTL_REQUEST_FIELD
                        | L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD
                        | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                        | L2C_ARRACCCTL_WAY_FIELD(cache_way)
                        | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr)
           ));

    while ( (0 == (l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD))  && (indx < L2C_TIMEOUT) )
        indx++;
    valid = (indx < L2C_TIMEOUT);

    if (valid)
    {
        *tag_info = l2c_l2_read (base, L2C_L2ARRACCDO0);
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("tag_info == %x\n", *tag_info);
#endif
    }
    else
    {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_tag_info_write_by_way_wo_gen_ecc( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way,
        uint32_t cache_data, uint32_t ecc_data)
{
    int indx;
    uint32_t l2arraccadr;
    uint32_t l2arraccctl;
    bool valid = false;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_tag_info_write_by_way_wo_gen_ecc\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
    rumboot_printf ("cache_data == %x\n", cache_data);
    rumboot_printf ("ecc_data == %x\n", ecc_data);
#endif

    l2arraccadr = l2c_l2_read (base, L2C_L2ARRACCADR) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read (base, L2C_L2ARRACCCTL) & ~L2C_ARRACCCTL_MSK;

    l2c_l2_write (base, L2C_L2ARRACCDI0, cache_data);
    l2c_l2_write (base, L2C_L2ARRACCDI2, ecc_data);

    l2c_l2_write (base, L2C_L2ARRACCADR, l2arraccadr | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
           ( l2arraccctl | L2C_ARRACCCTL_REQUEST_FIELD
                         | L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD
                         | L2C_ARRACCCTL_WRITE_8BYTE_ECC_AS_IS_FIELD
                         | L2C_ARRACCCTL_WAY_FIELD(cache_way)
                         | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr)
           ));

    indx = 0;
    while ( (0 == (l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_ARRACCCTL_WRITE_REQUEST_COMP_FIELD)) && (indx < L2C_TIMEOUT) )
        indx++;
    valid = (indx < L2C_TIMEOUT);

    if (!valid)
    {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_lru_info_read_by_way( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way,
        volatile uint32_t* lru_info )
{
    int indx = 0;
    uint32_t l2arraccadr;
    uint32_t l2arraccctl;
    bool valid = false;

    l2arraccadr = l2c_l2_read (base, L2C_L2ARRACCADR) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read (base, L2C_L2ARRACCCTL) & ~L2C_ARRACCCTL_MSK;

    l2c_l2_write (base, L2C_L2ARRACCADR, l2arraccadr | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) );

    l2c_l2_write( base, L2C_L2ARRACCCTL,
           (l2arraccctl | L2C_ARRACCCTL_REQUEST_FIELD
                        | L2C_ARRACCCTL_LRU_ARRAY_SELECT_FIELD
                        | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                        | L2C_ARRACCCTL_WAY_FIELD(cache_way)
                        | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr)
           ));

    while ( (0 == (l2c_l2_read( base, L2C_L2ARRACCCTL ) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD)) && (indx < L2C_TIMEOUT) )
        indx++;
    valid = ( indx < L2C_TIMEOUT );

    if (valid)
    {
        *lru_info = l2c_l2_read( base, L2C_L2ARRACCDO0 );
    }
    else
    {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_data_read_by_way( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way,
        volatile uint64_t* cache_data )
{
    int indx;
    uint32_t l2arraccadr;
    uint32_t l2arraccctl;
    bool valid = false;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_data_read_by_way\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
#endif

    l2arraccadr = l2c_l2_read (base, L2C_L2ARRACCADR) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read (base, L2C_L2ARRACCCTL) & ~L2C_ARRACCCTL_MSK;

    l2c_l2_write (base, L2C_L2ARRACCADR, l2arraccadr | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
           (l2arraccctl | L2C_ARRACCCTL_REQUEST_FIELD
                        | L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD
                        | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                        | L2C_ARRACCCTL_WAY_FIELD(cache_way)
                        | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr)
           ));

    indx = 0;
    while ( (0 == (l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD)) && (indx < L2C_TIMEOUT) )
        indx++;
    valid = ( indx < L2C_TIMEOUT );

    if (valid)
    {
        *cache_data =  l2c_l2_read (base, L2C_L2ARRACCDO1) | ( (uint64_t) l2c_l2_read(base, L2C_L2ARRACCDO0) << 32 ) ;
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("cache_data upper == %x\n", l2c_l2_read (base, L2C_L2ARRACCDO0) );
        rumboot_printf ("cache_data lower == %x\n", l2c_l2_read (base, L2C_L2ARRACCDO1) );
        rumboot_printf ("cache_data upper == %x\n", (uint32_t) (((*cache_data) >> 32) & 0xFFFFFFFF) );
        rumboot_printf ("cache_data lower == %x\n", (uint32_t) (*cache_data & 0xFFFFFFFF) );
#endif
    }
    else
    {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_data_write_by_way( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way,
        uint64_t cache_data )
{
    int indx;
    uint32_t l2arraccadr;
    uint32_t l2arraccctl;
    bool valid = false;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_data_write_by_way\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf ("cache_data upper == %x\n", (uint32_t) ((cache_data >> 32) & 0xFFFFFFFF) );
    rumboot_printf ("cache_data lower == %x\n", (uint32_t) ( cache_data & 0xFFFFFFFF) );
#endif

    l2arraccadr = l2c_l2_read (base, L2C_L2ARRACCADR) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read (base, L2C_L2ARRACCCTL) & ~L2C_ARRACCCTL_MSK;

    l2c_l2_write (base, L2C_L2ARRACCDI0, (uint32_t) (cache_data >> 32));
    l2c_l2_write (base, L2C_L2ARRACCDI1, (uint32_t) (cache_data & 0xFFFFFFFF));

    l2c_l2_write (base, L2C_L2ARRACCADR, l2arraccadr | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
           (l2arraccctl | L2C_ARRACCCTL_REQUEST_FIELD
                        | L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD
                        | L2C_ARRACCCTL_WRITE_8BYTE_GENERATE_ECC_FIELD
                        | L2C_ARRACCCTL_WAY_FIELD(cache_way)
                        | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr)
           ));

    indx = 0;
    while ( (0 == (l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_ARRACCCTL_WRITE_REQUEST_COMP_FIELD)) && (indx < L2C_TIMEOUT) )
        indx++;
    valid = (indx < L2C_TIMEOUT);

    if(!valid)
    {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_data_write_by_way_wo_gen_ecc( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way,
        uint64_t cache_data, uint32_t ecc_data)
{
    int indx;
    uint32_t l2arraccadr;
    uint32_t l2arraccctl;
    bool valid = false;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_data_write_by_way_wo_gen_ecc\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
    rumboot_printf ("cache_data upper == %x\n", (uint32_t) ((cache_data >> 32) & 0xFFFFFFFF) );
    rumboot_printf ("cache_data lower == %x\n", (uint32_t) ( cache_data & 0xFFFFFFFF) );
    rumboot_printf ("ecc_data == %x\n", ecc_data);
#endif

    l2arraccadr = l2c_l2_read (base, L2C_L2ARRACCADR) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read (base, L2C_L2ARRACCCTL) & ~L2C_ARRACCCTL_MSK;

    l2c_l2_write (base, L2C_L2ARRACCDI0, (uint32_t) (cache_data >> 32));
    l2c_l2_write (base, L2C_L2ARRACCDI1, (uint32_t) (cache_data & 0xFFFFFFFF));
    l2c_l2_write (base, L2C_L2ARRACCDI2, ecc_data);

    l2c_l2_write (base, L2C_L2ARRACCADR, l2arraccadr | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
           (l2arraccctl | L2C_ARRACCCTL_REQUEST_FIELD
                        | L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD
                        | L2C_ARRACCCTL_WRITE_8BYTE_ECC_AS_IS_FIELD
                        | L2C_ARRACCCTL_WAY_FIELD( cache_way )
                        | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr)
           ));

    indx = 0;
    while( ( 0 == (l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_ARRACCCTL_WRITE_REQUEST_COMP_FIELD)) && (indx < L2C_TIMEOUT) )
        indx++;
    valid = (indx < L2C_TIMEOUT);

    if(!valid)
    {
        rumboot_printf ("Timeout while accessing L2C via DCR.\n");
    }

    return valid;
}

bool l2c_arracc_get_way_by_address( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr,
        volatile int32_t* cache_way )
{
    uint64_t addr64;
    uint32_t tag_info;
    bool valid = false;
    bool tag_valid;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_get_way_by_address\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
#endif

    addr64 = ( ((uint64_t) (ext_phys_addr & L2C_EXT_ADDR_MSK)) << 32) + phys_addr;
    *cache_way = -1;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("cache_way == %x\n", *cache_way);
#endif

    do
    {
        (*cache_way)++;
        tag_valid = l2c_arracc_tag_info_read_by_way (base, ext_phys_addr, phys_addr, *cache_way, &tag_info);

#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("cache_way == %x\n", *cache_way);
        rumboot_printf ("tag_valid == %x\n", tag_valid);
#endif

    } while (
              ((*cache_way) < L2C_WAY3_NUM) &&
                 ( ((uint64_t)L2C_TAG_ADDRESS_FROM_64BIT(addr64) != (uint64_t)L2C_TAG_ADDRESS_FROM_ARRACCDO0(tag_info)) || !tag_valid )
            );

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_get_way_by_address loop finished\n");
#endif

    if ( (L2C_TAG_ADDRESS_FROM_ARRACCDO0(tag_info) == L2C_TAG_ADDRESS_FROM_64BIT(addr64)) && tag_valid )
    {
        valid = true;
    }
    else
    {
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("Tag address not found in Tag array.\n");
#endif
        valid = false;
    }

    return valid;
}

bool l2c_arracc_get_data_by_address( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr,
        volatile uint64_t* data64 )
{
    int32_t cache_way = L2C_WAY0_NUM - 1;
    bool valid = false;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_get_data_by_address\n");
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr);
#endif

    if (l2c_arracc_get_way_by_address (base, ext_phys_addr, phys_addr, &cache_way))
    {
        valid = l2c_arracc_data_read_by_way (base, ext_phys_addr, phys_addr, cache_way, data64);
    }
    else
    {
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("Tag address not found in Tag array.\n");
#endif
    }

    return valid;
}

bool l2c_arracc_set_data_by_address( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr,
        uint64_t data64 )
{
    int32_t cache_way = L2C_WAY0_NUM - 1;
    bool valid = false;

#ifdef L2C_TRACE_DEBUG_MSG
    rumboot_printf ("l2c_arracc_set_data_by_address\n" );
    rumboot_printf ("address == %x_%x\n", ext_phys_addr, phys_addr );
    rumboot_printf ("data64 upper == %x\n", (uint32_t)((data64 >> 32) & 0xFFFFFFFF) );
    rumboot_printf ("data64 lower == %x\n", (uint32_t)(data64 & 0xFFFFFFFF) );
#endif

    if (l2c_arracc_get_way_by_address (base, ext_phys_addr, phys_addr, &cache_way))
    {
        valid = l2c_arracc_data_write_by_way (base, ext_phys_addr, phys_addr, cache_way, data64);
    }
    else
    {
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf ("Tag address not found in Tag array.\n");
#endif
    }

    return valid;
}

uint64_t l2c_arracc_data_read( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr )
{
    int indx;
    uint32_t data;
    uint32_t l2arraccadr;
    uint32_t l2arraccctl;
    int32_t cache_way = L2C_WAY0_NUM - 1;
    uint64_t addr64;

    //rumboot_printf( "l2c_arracc_data_read\n" );
    l2arraccadr = l2c_l2_read (base, L2C_L2ARRACCADR) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read (base, L2C_L2ARRACCCTL) & ~L2C_ARRACCCTL_MSK;

    addr64 = (((uint64_t) (ext_phys_addr & L2C_EXT_ADDR_MSK)) << 32) + phys_addr;
    do
    {
        cache_way++;
        l2c_l2_write (base, L2C_L2ARRACCADR, l2arraccadr | L2C_ARRACCADR_TAG_LRU_ADDR_FIELD(phys_addr) );
        l2c_l2_write (base, L2C_L2ARRACCCTL,
               (l2arraccctl | L2C_ARRACCCTL_REQUEST_FIELD
                            | L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD
                            | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                            | L2C_ARRACCCTL_WAY_FIELD(cache_way)
               ));

        indx = 0;
        while ( (0 == (l2c_l2_read( base, L2C_L2ARRACCCTL) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD)) && (indx < L2C_TIMEOUT) )
            indx++;
        TEST_ASSERT(indx < L2C_TIMEOUT, "Timeout while accessing L2C via DCR.\n" );
        data = l2c_l2_read (base, L2C_L2ARRACCDO0);

    } while ( (cache_way < L2C_WAY3_NUM) && (L2C_TAG_ADDRESS_FROM_64BIT(addr64) != L2C_TAG_ADDRESS_FROM_ARRACCDO0(data)) );

    COMPARE_VALUES ( L2C_TAG_ADDRESS_FROM_ARRACCDO0(data), (uint64_t)L2C_TAG_ADDRESS_FROM_64BIT(addr64), "Tag address not found in Tag array.\n" );

    l2c_l2_write (base, L2C_L2ARRACCADR, l2arraccadr | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
           (l2arraccctl | L2C_ARRACCCTL_REQUEST_FIELD
                        | L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD
                        | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                        | L2C_ARRACCCTL_WAY_FIELD(cache_way)
                        | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr)
           ));

    indx = 0;
    while ( (0 == (l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD)) && (indx < L2C_TIMEOUT) )
        indx++;
    TEST_ASSERT(indx < L2C_TIMEOUT, "Timeout while accessing L2C via DCR.\n" );

    return ( l2c_l2_read (base, L2C_L2ARRACCDO1) | (((uint64_t) l2c_l2_read (base, L2C_L2ARRACCDO0)) << 32) );
}

void l2c_arracc_data_write( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, uint64_t data64)
{
    int indx;
    uint32_t data;
    uint32_t l2arraccadr;
    uint32_t l2arraccctl;
    int32_t cache_way = L2C_WAY0_NUM - 1;
    uint64_t addr64;

    //rumboot_printf( "l2c_arracc_data_write\n" );
    l2arraccadr = l2c_l2_read (base, L2C_L2ARRACCADR) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read (base, L2C_L2ARRACCCTL) & ~L2C_ARRACCCTL_MSK;

    addr64 = (((uint64_t) (ext_phys_addr & L2C_EXT_ADDR_MSK)) << 32) + phys_addr;
    do
    {
        cache_way++;
        l2c_l2_write (base, L2C_L2ARRACCADR, l2arraccadr | L2C_ARRACCADR_TAG_LRU_ADDR_FIELD(phys_addr) );
        l2c_l2_write (base, L2C_L2ARRACCCTL,
               (l2arraccctl  | L2C_ARRACCCTL_REQUEST_FIELD
                             | L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD
                             | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                             | L2C_ARRACCCTL_WAY_FIELD(cache_way)
               ));

        indx = 0;
        while ( ( 0 == (l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD)) && (indx < L2C_TIMEOUT) )
            indx++;
        TEST_ASSERT(indx < L2C_TIMEOUT, "Timeout while accessing L2C via DCR.\n" );
        data = l2c_l2_read (base, L2C_L2ARRACCDO0);

    } while( (cache_way < L2C_WAY3_NUM) && (L2C_TAG_ADDRESS_FROM_64BIT(addr64) != L2C_TAG_ADDRESS_FROM_ARRACCDO0(data)) );

    COMPARE_VALUES( L2C_TAG_ADDRESS_FROM_ARRACCDO0(data), (uint64_t)L2C_TAG_ADDRESS_FROM_64BIT(addr64), "Tag address not found in Tag array.\n");

    l2c_l2_write (base, L2C_L2ARRACCDI0, (uint32_t) (data64 >> 32));
    l2c_l2_write (base, L2C_L2ARRACCDI1, (uint32_t) (data64 & 0xFFFFFFFF));

    l2c_l2_write (base, L2C_L2ARRACCADR, l2arraccadr | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) );

    l2c_l2_write (base, L2C_L2ARRACCCTL,
           (l2arraccctl | L2C_ARRACCCTL_REQUEST_FIELD
                        | L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD
                        | L2C_ARRACCCTL_WRITE_8BYTE_GENERATE_ECC_FIELD
                        | L2C_ARRACCCTL_WAY_FIELD(cache_way)
                        | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr)
           ));

    indx = 0;
    while ( (0 == (l2c_l2_read (base, L2C_L2ARRACCCTL) & L2C_ARRACCCTL_WRITE_REQUEST_COMP_FIELD)) && (indx < L2C_TIMEOUT) )
        indx++;
    TEST_ASSERT( indx < L2C_TIMEOUT, "Timeout while accessing L2C via DCR.\n");
}


void l2c_get_mem_layout( uint32_t const base, struct l2c_mem_layout * const mem_layout ) {
    mem_layout->l2size              = l2c_l2_read(base, L2C_L2CNFG0) & L2C_L2CNFG0_L2SIZE_mask;
    mem_layout->tag_ecc_mask        = 0x7F;
    mem_layout->lru_mask            = 0x3FFFFFFF;
    mem_layout->data_ecc_mask       = 0xFF;
    switch( mem_layout->l2size ) {
    case L2C_L2Size_128KB:
        mem_layout->lru_array_size  = 256;
        mem_layout->tag_mask        = 0x3FFFFFFF;
        break;
    case L2C_L2Size_256KB:
        mem_layout->lru_array_size  = 512;
        mem_layout->tag_mask        = 0x1FFFFFFF;
        break;
    case L2C_L2Size_512KB:
        mem_layout->lru_array_size  = 1024;
        mem_layout->tag_mask        = 0x0FFFFFFF;
        break;
    case L2C_L2Size_1MB:
        mem_layout->lru_array_size  = 2048;
        mem_layout->tag_mask        = 0x07FFFFFF;
        break;
    }
    mem_layout->tag_array_size      = 4 * mem_layout->lru_array_size;
    mem_layout->data_array_size     = 8 * mem_layout->lru_array_size;
}
