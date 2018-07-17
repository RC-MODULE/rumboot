#include <stddef.h>
#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/devices/l2c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/arch/ppc/test_macro.h>

void l2c_dcr_write(uint32_t l2c_dcr_base, uint32_t dcr_index, uint32_t wval)
{
	dcr_write(l2c_dcr_base+L2C_DCRAI, dcr_index);
	dcr_write(l2c_dcr_base+L2C_DCRDI, wval);
}

uint32_t l2c_dcr_read(uint32_t l2c_dcr_base, uint32_t dcr_index)
{
	dcr_write(l2c_dcr_base+L2C_DCRAI, dcr_index);
	return dcr_read(l2c_dcr_base+L2C_DCRDI);
}

inline void l2c_global_enable_interrupt(uint32_t l2c_dcr_base, L2INTEN_bits_t mask)
{
    uint32_t tmp;
    tmp = l2c_dcr_read(l2c_dcr_base, L2INTEN);
    SET_BITS_BY_MASK(tmp, mask);
    l2c_dcr_write(l2c_dcr_base, L2INTEN, tmp);
}

inline void l2c_global_mck_enable(uint32_t l2c_dcr_base, L2MCKEN_bits_t mask)
{
    uint32_t tmp;
    tmp = l2c_dcr_read(l2c_dcr_base, L2MCKEN);
    SET_BITS_BY_MASK(tmp, mask);
    l2c_dcr_write(l2c_dcr_base, L2MCKEN, tmp);
}

inline void l2c_global_mck_disable(uint32_t l2c_dcr_base, L2MCKEN_bits_t mask)
{
    uint32_t tmp;
    tmp = l2c_dcr_read(l2c_dcr_base, L2MCKEN);
    CLEAR_BITS_BY_MASK(tmp, mask);
    l2c_dcr_write(l2c_dcr_base, L2MCKEN, tmp);
    TEST_ASSERT(l2c_dcr_read(l2c_dcr_base, L2MCKEN)==tmp, "Error setting L2MCKEN");
}

inline void l2c_enable_interrupt(uint32_t l2c_dcr_base, uint32_t l2c_int_reg_index, uint32_t mask)
{
    uint32_t tmp;
    tmp = l2c_dcr_read(l2c_dcr_base, l2c_int_reg_index);
    SET_BITS_BY_MASK(tmp, mask);
    l2c_dcr_write(l2c_dcr_base, l2c_int_reg_index, tmp);
}

void pmu_dcr_write(uint32_t pmu_dcr_base, uint32_t dcr_index, uint32_t wval)
{
    dcr_write(PMUDCRAI(pmu_dcr_base), dcr_index);
    dcr_write(PMUDCRDI(pmu_dcr_base), wval);
}

uint32_t pmu_dcr_read(uint32_t pmu_dcr_base, uint32_t dcr_index)
{
    dcr_write(PMUDCRAI(pmu_dcr_base), dcr_index);
    return dcr_read(PMUDCRDI(pmu_dcr_base));
}

inline void l2c_pmu_set_CE_bit(uint32_t pmu_dcr_base, uint32_t dcr_index)
{
    uint32_t tmp;
    tmp = pmu_dcr_read(pmu_dcr_base, dcr_index);
    pmu_dcr_write(pmu_dcr_base, dcr_index, tmp | PMULCX_CE);
}

inline void l2c_pmu_enable_interrupt(uint32_t pmu_dcr_base, uint32_t dcr_index, uint32_t mask)
{
    uint32_t tmp;
    tmp = pmu_dcr_read(pmu_dcr_base, dcr_index);
    pmu_dcr_write(pmu_dcr_base, dcr_index, tmp | mask);
}

inline void pmu_disable_interrupt(uint32_t pmu_dcr_base, uint32_t dcr_index, uint32_t mask)
{
    uint32_t tmp;
    tmp = pmu_dcr_read(pmu_dcr_base, dcr_index);
    CLEAR_BITS_BY_MASK(tmp, mask);
    pmu_dcr_write(pmu_dcr_base, dcr_index, tmp);
}

inline void l2c_pmu_clear_interrupt(uint32_t pmu_dcr_base)
{
    uint32_t tmp;
    tmp = pmu_dcr_read(pmu_dcr_base, PMUIS0);
    pmu_dcr_write(pmu_dcr_base, PMUIS0, tmp);
}

inline void l2c_pmu_set_cx(uint32_t pmu_dcr_base, uint32_t dcr_index, uint32_t value)
{
    pmu_dcr_write(pmu_dcr_base, dcr_index, value);
}

bool l2c_arracc_tag_info_read_by_way (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, volatile uint32_t* tag_info)
{
	int			indx = 0;
    uint32_t	volatile 	l2arraccadr;
    uint32_t	volatile 	l2arraccctl;
    bool 					valid = false;

    rumboot_putstring("l2c_arracc_tag_info_read_by_way\n");
    rumboot_putstring("address upper == ");
    rumboot_puthex(ext_phys_addr);
    rumboot_putstring("address lower == ");
    rumboot_puthex(phys_addr);
    rumboot_putstring("cache_way == ");
    rumboot_puthex(cache_way);
	l2arraccadr = L2C_L2ARRACCADR_DCR_read(base) & ~L2C_L2ARRACCADR_MSK;
	l2arraccctl = L2C_L2ARRACCCTL_DCR_read(base) & ~L2C_ARRACCCTL_MSK;

	L2C_L2ARRACCADR_DCR_write(base,( ( l2arraccadr ) | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) ) );

	L2C_L2ARRACCCTL_DCR_write(base,( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
			L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD |
			L2C_ARRACCCTL_WAY_FIELD(cache_way) | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr));
	while(((L2C_L2ARRACCCTL_DCR_read(base) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD) == 0) && (indx < L2C_TIMEOUT))
		indx++;
	valid = (indx < L2C_TIMEOUT);

	if(valid)
	{
		*tag_info = L2C_L2ARRACCDO0_DCR_read(base);
		rumboot_putstring("tag_info == ");
		rumboot_puthex(*tag_info);
	}
	else
	{
		rumboot_putstring("Timeout while accessing L2C via DCR.\n");
	}


	return valid;

}

bool l2c_arracc_data_read_by_way (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, volatile uint64_t* cache_data)
{

	int			indx;
    uint32_t	volatile l2arraccadr;
    uint32_t	volatile l2arraccctl;
    bool 		valid = false;

    rumboot_putstring("l2c_arracc_data_read_by_way\n");
    rumboot_putstring("address upper == ");
    rumboot_puthex(ext_phys_addr);
    rumboot_putstring("address lower == ");
    rumboot_puthex(phys_addr);
	l2arraccadr = L2C_L2ARRACCADR_DCR_read(base) & ~L2C_L2ARRACCADR_MSK;
	l2arraccctl = L2C_L2ARRACCCTL_DCR_read(base) & ~L2C_ARRACCCTL_MSK;

	L2C_L2ARRACCADR_DCR_write(base,( ( l2arraccadr ) | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) ) );

	L2C_L2ARRACCCTL_DCR_write(base,( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
			L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD |
			L2C_ARRACCCTL_WAY_FIELD(cache_way) | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr));

	indx = 0;
	while(((L2C_L2ARRACCCTL_DCR_read(base) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD) == 0) && (indx < L2C_TIMEOUT))
		indx++;
	valid = (indx < L2C_TIMEOUT);

	if(valid)
	{
		*cache_data = ( L2C_L2ARRACCDO1_DCR_read(base) | ( (uint64_t) L2C_L2ARRACCDO0_DCR_read(base) << 32 ) );
		rumboot_putstring("cache_data upper == ");
		rumboot_puthex(L2C_L2ARRACCDO0_DCR_read(base));
		rumboot_putstring("cache_data lower == ");
		rumboot_puthex(L2C_L2ARRACCDO1_DCR_read(base));
		rumboot_putstring("cache_data lower == ");
		rumboot_puthex((uint32_t)(*cache_data && 0xFFFFFFFF));
		rumboot_putstring("cache_data upper == ");
		rumboot_puthex((uint32_t)(((*cache_data)>>32) && 0xFFFFFFFF));
	}
	else
	{
		rumboot_putstring("Timeout while accessing L2C via DCR.\n");
	}

	return valid;
}

bool l2c_arracc_get_way_by_address (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, volatile int32_t* cache_way)
{
    uint64_t	addr64;
    uint32_t	tag_info;
    bool 		valid = false;
    bool 		tag_valid;

    rumboot_putstring("l2c_arracc_get_way_by_address\n");
    rumboot_putstring("address upper == ");
    rumboot_puthex(ext_phys_addr);
    rumboot_putstring("address lower == ");
    rumboot_puthex(phys_addr);
	addr64 = ( (uint64_t) (ext_phys_addr & L2C_EXT_ADDR_MSK) << 32 ) + phys_addr;
	(*cache_way) = -1;
	rumboot_putstring("cache_way == ");
	rumboot_puthex((int32_t)(*cache_way));
	do
	{
		(int32_t)(*cache_way)++;
		tag_valid = l2c_arracc_tag_info_read_by_way (base, ext_phys_addr, phys_addr, *cache_way, &tag_info);
		rumboot_putstring("cache_way == ");
		rumboot_puthex((int32_t)(*cache_way));
		rumboot_putstring("tag_valid == ");
		rumboot_puthex(tag_valid);
	}
	while ( (((int32_t)(*cache_way)) < L2C_WAY3_NUM) &&
			( ( (uint64_t)L2C_TAG_ADDRESS_FROM_64BIT(addr64) != (uint64_t)L2C_TAG_ADDRESS_FROM_ARRACCDO0(tag_info) ) || !tag_valid) );
	rumboot_putstring("l2c_arracc_get_way_by_address loop finished\n");
	if(( L2C_TAG_ADDRESS_FROM_ARRACCDO0(tag_info)  ==  L2C_TAG_ADDRESS_FROM_64BIT(addr64) ) && tag_valid)
	{
		valid = true;
	}
	else
	{
		rumboot_putstring("Tag address not found in Tag array.\n");
		valid = false;
	}
	return valid;
}


bool l2c_arracc_get_data_by_address (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, volatile uint64_t* data64)
{
    int32_t 	cache_way = L2C_WAY0_NUM - 1;
    bool		valid = false;

    rumboot_putstring("l2c_arracc_get_data_by_address\n");
    rumboot_putstring("address upper == ");
    rumboot_puthex(ext_phys_addr);
    rumboot_putstring("address lower == ");
    rumboot_puthex(phys_addr);
	if( l2c_arracc_get_way_by_address( base,  ext_phys_addr,  phys_addr, &cache_way) )
	{
		valid = l2c_arracc_data_read_by_way ( base,  ext_phys_addr,  phys_addr,  cache_way,  data64);
	}
	else
	{
		rumboot_putstring("Tag address not found in Tag array.\n");
	}
	return valid;
}

uint64_t l2c_arracc_data_read (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr)
{
	int			indx;
	uint32_t 	data;
    uint32_t	volatile l2arraccadr;
    uint32_t	volatile l2arraccctl;
    int32_t		cache_way = L2C_WAY0_NUM - 1;
    uint64_t	addr64;

    rumboot_putstring("l2c_arracc_data_read\n");
	l2arraccadr = L2C_L2ARRACCADR_DCR_read(base) & ~L2C_L2ARRACCADR_MSK;
	l2arraccctl = L2C_L2ARRACCCTL_DCR_read(base) & ~L2C_ARRACCCTL_MSK;

	addr64 = ( (uint64_t) (ext_phys_addr & L2C_EXT_ADDR_MSK) << 32 ) + phys_addr;
	do
	{
		cache_way++;
		L2C_L2ARRACCADR_DCR_write(base,( ( l2arraccadr ) | L2C_ARRACCADR_TAG_LRU_ADDR_FIELD(phys_addr) ) );
		L2C_L2ARRACCCTL_DCR_write(base,( ( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
				L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD |
				L2C_ARRACCCTL_WAY_FIELD(cache_way) ) );
		indx = 0;
		while(((L2C_L2ARRACCCTL_DCR_read(base) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD) == 0) && (indx < L2C_TIMEOUT))
			indx++;
		TEST_ASSERT((indx < L2C_TIMEOUT),"Timeout while accessing L2C via DCR.\n");
		data = L2C_L2ARRACCDO0_DCR_read(base);
	}
	while ( (cache_way < L2C_WAY3_NUM) && ( L2C_TAG_ADDRESS_FROM_64BIT(addr64) != L2C_TAG_ADDRESS_FROM_ARRACCDO0(data) ) );
	COMPARE_VALUES( ( L2C_TAG_ADDRESS_FROM_ARRACCDO0(data) ), ( (uint64_t)L2C_TAG_ADDRESS_FROM_64BIT(addr64) ), "Tag address not found in Tag array.\n");

	L2C_L2ARRACCADR_DCR_write(base,( ( l2arraccadr ) | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(phys_addr) ) );

	L2C_L2ARRACCCTL_DCR_write(base,( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
			L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD |
			L2C_ARRACCCTL_WAY_FIELD(cache_way) | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr));

	indx = 0;
	while(((L2C_L2ARRACCCTL_DCR_read(base) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD) == 0) && (indx < L2C_TIMEOUT))
		indx++;
	TEST_ASSERT((indx < L2C_TIMEOUT),"Timeout while accessing L2C via DCR.\n");
	return ( L2C_L2ARRACCDO1_DCR_read(base) | ( (uint64_t) L2C_L2ARRACCDO0_DCR_read(base) << 32 ) );
}
