#include <stddef.h>
#include <rumboot/macros.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/devices/l2c.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c_pmu.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/arch/ppc/test_macro.h>

void l2c_global_enable_interrupt( uint32_t const l2c_dcr_base, L2INTEN_bits_t const mask ) {
    l2c_l2_write( l2c_dcr_base, L2C_L2INTEN, l2c_l2_read( l2c_dcr_base, L2C_L2INTEN ) | mask );
}

void l2c_global_mck_enable( uint32_t const l2c_dcr_base, L2MCKEN_bits_t const mask ) {
    l2c_l2_write( l2c_dcr_base, L2C_L2MCKEN, l2c_l2_read( l2c_dcr_base, L2C_L2MCKEN ) | mask );
}

void l2c_global_mck_disable( uint32_t const l2c_dcr_base, L2MCKEN_bits_t const mask ) {
    uint32_t value = l2c_l2_read( l2c_dcr_base, L2C_L2MCKEN ) & ~mask;
    l2c_l2_write( l2c_dcr_base, L2C_L2MCKEN, value );
    TEST_ASSERT( l2c_l2_read( l2c_dcr_base, L2C_L2MCKEN ) == value, "Error setting L2MCKEN" );
}

void l2c_enable_interrupt( uint32_t const l2c_dcr_base, L2C_L2REG const l2c_int_reg_index, uint32_t const mask ) {
    l2c_l2_write( l2c_dcr_base, l2c_int_reg_index, l2c_l2_read( l2c_dcr_base, l2c_int_reg_index ) | mask );
}

void l2c_pmu_set_CE_bit( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index ) {
    l2c_pmu_write( pmu_dcr_base, dcr_index, l2c_pmu_read( pmu_dcr_base, dcr_index ) | PMULCX_CE );
}

void l2c_pmu_enable_interrupt( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index, uint32_t const mask ) {
    l2c_pmu_write( pmu_dcr_base, dcr_index, l2c_pmu_read( pmu_dcr_base, dcr_index ) | mask );
}

void pmu_disable_interrupt( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index, uint32_t const mask ) {
    l2c_pmu_write( pmu_dcr_base, dcr_index, l2c_pmu_read( pmu_dcr_base, dcr_index ) & ~mask );
}

void l2c_pmu_clear_interrupt( uint32_t const pmu_dcr_base ) {
    l2c_pmu_write( pmu_dcr_base, L2C_PMUIS0, l2c_pmu_read( pmu_dcr_base, L2C_PMUIS0 ) );
}

void l2c_pmu_set_cx( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index, uint32_t const value ) {
    l2c_pmu_write( pmu_dcr_base, dcr_index, value );
}

#define I_M_SELF_ID(base) ( (base == L2C0_DCR_BASE) ? 0x00 : 0x01 )
//from trunk/cpu/units/ppc476l2c_v001/verilog/rtl/PPC476L2C.parameters.v
#define  L2C_VERSION_NUMBER   0x001
#define  L2C_REVISION_ID      0x02
//from dut
#define L2C_L2SIZE              0x1
#define L2C_PLB_CLK_RATIO       0x0
#define L2C_TSNOOP             0x0 //acc. to L2C_PLBTSNOOP[0:2] strap pin
#define L2C_MASTER_ID(base)     I_M_SELF_ID(base)

#define L2C_L2ISTAT_VAL     0x00000001 //1- initialization complete

#define L2C_L2PNCR_VAL_BASE(base) ( (base == L2C0_DCR_BASE) ? 0x00000069 : 0x00000075 ) //see trunk/cpu/units/ppc476l2c_v001/verilog/rtl/PPC476L2C_DCR_LFSR.v. Dependent from I_M_SELF_ID strap.
#define L2C_L2PNCR_VAL      L2C_L2PNCR_VAL_BASE(L2C0_DCR_BASE)

#define L2C_L2REVID_VAL     ( reg_field(31,L2C_REVISION_ID) | reg_field(23,L2C_VERSION_NUMBER) )
#define L2C_L2CNFG0_VAL_BASE(base)     ( reg_field(31,L2C_L2SIZE) | reg_field(27,L2C_PLB_CLK_RATIO) | reg_field(23,L2C_TSNOOP) | reg_field(19,L2C_MASTER_ID(base)) )
#define L2C_L2CNFG0_VAL     L2C_L2CNFG0_VAL_BASE(L2C0_DCR_BASE)

#define L2C_L2CNFG1_VAL     ( reg_field(26,0x1) )
#define L2C_L2DBGSEL_VAL    0x00000000
#define L2C_L2DBGDATA0_VAL  0x00000000
#define L2C_L2DBGDATA1_VAL  0x00000000
#define L2C_L2SLEEPSTAT_VAL 0x00000000
#define L2C_L2SLEEPREQ_VAL  0x00000000
#define L2C_L2MCK_VAL       0x00000000
#define L2C_L2MCKEN_VAL     reg_field(30,0x7FF)
#define L2C_L2FERR_VAL      0x00000000
#define L2C_L2INT_VAL       0x00000000
#define L2C_L2INTEN_VAL     reg_field(30,0x3FF)
#define L2C_L2LOG0_VAL      0x00000000
#define L2C_L2LOG1_VAL      0x00000000
#define L2C_L2LOG2_VAL      0x00000000
#define L2C_L2LOG3_VAL      0x00000000
#define L2C_L2LOG4_VAL      0x00000000
#define L2C_L2LOG5_VAL      0x00000000
#define L2C_L2PLBCFG_VAL    0x00000000
#define L2C_L2PLBDBG_VAL    0x00000000
#define L2C_L2PLBERAP_VAL   0x00000000
#define L2C_L2PLBSTAT0_VAL  0x00000000
#define L2C_L2PLBSTAT1_VAL  0x00000000
#define L2C_L2PLBFRC0_VAL   0x00000000
#define L2C_L2PLBFRC1_VAL   0x00000000
#define L2C_L2PLBMCKEN0_VAL 0xFFFFFFFF
#define L2C_L2PLBMCKEN1_VAL 0xFFFFFFFF
#define L2C_L2PLBFERR0_VAL  0x00000000
#define L2C_L2PLBFERR1_VAL  0x00000000
#define L2C_L2PLBINTEN0_VAL 0x00000000
#define L2C_L2PLBINTEN1_VAL 0x00000000
#define L2C_L2ARRCFG_VAL    0x00000000
#define L2C_L2ARRDBG0_VAL   0x00000000
#define L2C_L2ARRDBG1_VAL   0x00000000
#define L2C_L2ARRDBG2_VAL   0x00000000
#define L2C_L2ARRDBG3_VAL   0x00000000
#define L2C_L2ARRACCCTL_VAL 0x00000000
#define L2C_L2ARRACCADR_VAL 0x00000000
#define L2C_L2ARRACCDI0_VAL 0x00000000
#define L2C_L2ARRACCDI1_VAL 0x00000000
#define L2C_L2ARRACCDI2_VAL 0x00000000
#define L2C_L2ARRACCDO0_VAL 0x00000000
#define L2C_L2ARRACCDO1_VAL 0x00000000
#define L2C_L2ARRACCDO2_VAL 0x00000000
#define L2C_L2ARRSTAT0_VAL  0x00000000
#define L2C_L2ARRSTAT1_VAL  0x00000000
#define L2C_L2ARRSTAT2_VAL  0x00000000
#define L2C_L2ARRFRC0_VAL   0x00000000
#define L2C_L2ARRFRC1_VAL   0x00000000
#define L2C_L2ARRFRC2_VAL   0x00000000
#define L2C_L2ARRMCKEN0_VAL 0xFFFFFFFF
#define L2C_L2ARRMCKEN1_VAL 0x00001FFF
#define L2C_L2ARRMCKEN2_VAL 0x00000000
#define L2C_L2ARRFERR0_VAL  0x00000000
#define L2C_L2ARRFERR1_VAL  0x00000000
#define L2C_L2ARRFERR2_VAL  0x00000000
#define L2C_L2ARRINTEN0_VAL 0x00000000
#define L2C_L2ARRINTEN1_VAL 0x00000000
#define L2C_L2ARRINTEN2_VAL 0x00FFFFFF
#define L2C_L2CPUCFG_VAL    0x00000000
#define L2C_L2CPUDBG_VAL    0x00000000
#define L2C_L2CPUSTAT_VAL   0x00000000
#define L2C_L2CPUFRC_VAL    0x00000000
#define L2C_L2CPUMCKEN_VAL  0x00000FFF
#define L2C_L2CPUFERR_VAL   0x00000000
#define L2C_L2CPUINTEN_VAL  0x00000000
#define L2C_L2RACCFG_VAL    0x00000000
#define L2C_L2RACDBG0_VAL   0x00000000
#define L2C_L2RACDBG1_VAL   0x00000000
#define L2C_L2RACSTAT0_VAL  0x00000000
#define L2C_L2RACFRC0_VAL   0x00000000
#define L2C_L2RACMCKEN0_VAL 0x03FFFFFF
#define L2C_L2RACFERR0_VAL  0x00000000
#define L2C_L2RACINTEN0_VAL 0x00000000
#define L2C_L2WACCFG_VAL    0x00000000
#define L2C_L2WACDBG0_VAL   0x00000000
#define L2C_L2WACDBG1_VAL   0x00000000
#define L2C_L2WACDBG2_VAL   0x00000000
#define L2C_L2WACSTAT0_VAL  0x00000000
#define L2C_L2WACSTAT1_VAL  0x00000000
#define L2C_L2WACSTAT2_VAL  0x00000000
#define L2C_L2WACFRC0_VAL   0x00000000
#define L2C_L2WACFRC1_VAL   0x00000000
#define L2C_L2WACFRC2_VAL   0x00000000
#define L2C_L2WACMCKEN0_VAL 0x00FFFFFF
#define L2C_L2WACMCKEN1_VAL 0x00FFFFFF
#define L2C_L2WACMCKEN2_VAL 0x00FFFFFF
#define L2C_L2WACFERR0_VAL  0x00000000
#define L2C_L2WACFERR1_VAL  0x00000000
#define L2C_L2WACFERR2_VAL  0x00000000
#define L2C_L2WACINTEN0_VAL 0x00000000
#define L2C_L2WACINTEN1_VAL 0x00000000
#define L2C_L2WACINTEN2_VAL 0x00000000

#define L2C_L2ARRACCADR_MSK             0x1FFF
#define L2C_L2ARRACCADR_ADR_MSK         0x7FF
#define L2C_TIMEOUT                         0x00000020
#define L2C_WAY0_NUM                        0x0
#define L2C_WAY1_NUM                        0x1
#define L2C_WAY2_NUM                        0x2
#define L2C_WAY3_NUM                        0x3
#define L2C_WAY_MSK                         0x3
#define L2C_CACHE_LINE_ADDR_MSK                     0x3FF
#define L2C_CACHE_HALFLINE_ADDR_MSK                 0x7FF //L2C_L2ARRACCADR_ADR_MSK
#define L2C_ARRACCCTL_WAY_MSK                           reg_field(23,0x3)
#define L2C_ARRACCCTL_DWORD_MSK                         reg_field(31,0xFF)
#define L2C_ARRACCCTL_MSK                             ( L2C_ARRACCCTL_REQUEST_FIELD |\
                                                        L2C_ARRACCCTL_LRU_ARRAY_SELECT_FIELD |\
                                                        L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD |\
                                                        L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD |\
                                                        L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD |\
                                                        L2C_ARRACCCTL_WAY_MSK |\
                                                        L2C_ARRACCCTL_DWORD_MSK \
                                                      )
//bits [0:8] are reserved
#define L2C_ARRACCCTL_REQUEST_FIELD                     reg_field(9,0x1)
#define L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD           reg_field(10,0x1)
#define L2C_ARRACCCTL_WRITE_REQUEST_COMP_FIELD          reg_field(11,0x1)
//bit 12 is reserved
#define L2C_ARRACCCTL_LRU_ARRAY_SELECT_FIELD            reg_field(13,0x1)
#define L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD            reg_field(14,0x1)
#define L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD           reg_field(15,0x1)
#define L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD           reg_field(19,0xF)
#define L2C_ARRACCCTL_WRITE_8BYTE_GENERATE_ECC_FIELD    reg_field(19,0x6)
#define L2C_ARRACCCTL_WRITE_8BYTE_ECC_AS_IS_FIELD       reg_field(19,0x7)
#define L2C_ARRACCCTL_WAY_FIELD(way_number)             reg_field(23,((way_number) & 0x3))
#define L2C_ARRACCCTL_MSK_DWORD_FIELD(addr)             reg_field(31,(0x1 << (0x7 - ( ((addr) >> 0x3) & 0x7) )))
#define L2C_ARRACCADR_TAG_LRU_ADDR_FIELD(addr)          reg_field( 30,( ( (addr) >> 7) & L2C_CACHE_LINE_ADDR_MSK ) )
#define L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(addr)       reg_field( 31,( ( (addr) >> 6) & L2C_L2ARRACCADR_ADR_MSK ) )

#define L2C_TAG_ADDRESS_OFFSET                          (17)
#define L2C_TAG_ADDRESS_MSK                             0x1FFFFFF
#define L2C_TAG_CACHE_STATE_MSK                         0x7
#define L2C_TAG_CACHE_STATE_INVALID                     0x0
#define L2C_TAG_ADDRESS_FROM_64BIT(addr)                (( (addr) & ( (uint64_t)L2C_TAG_ADDRESS_MSK << L2C_TAG_ADDRESS_OFFSET ) ) >> L2C_TAG_ADDRESS_OFFSET)
#define L2C_TAG_ADDRESS_FROM_ARRACCDO0(val)             ( ( (val) & (L2C_TAG_ADDRESS_MSK << 4) ) >> 4)
#define L2C_TAG_CACHE_STATE_FROM_ARRACCDO0(val)         ( ((val) >> IBM_BIT_INDEX(32, 2)) & L2C_TAG_CACHE_STATE_MSK )
#define L2C_TAG_CACHE_STATE_INVALID_VAL                 0x0

#define L2C_EXT_ADDR_MSK                                    0x3FF

#define DEFINE_L2MCKEN_EXTINT      (1 << 11)
#define DEFINE_L2MCKEN_PLBINT0     (1 << 10)
#define DEFINE_L2MCKEN_PLBINT1     (1 << 9)
#define DEFINE_L2MCKEN_L2AINT0     (1 << 8)
#define DEFINE_L2MCKEN_L2AINT1     (1 << 7)
#define DEFINE_L2MCKEN_L2AINT2     (1 << 6)
#define DEFINE_L2MCKEN_L1CINT0     (1 << 5)
#define DEFINE_L2MCKEN_RACINT0     (1 << 4)
#define DEFINE_L2MCKEN_WACINT0     (1 << 3)
#define DEFINE_L2MCKEN_WACINT1     (1 << 2)
#define DEFINE_L2MCKEN_WACINT2     (1 << 1)

bool l2c_arracc_tag_info_read_by_way( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way,
        volatile uint32_t* tag_info ) {
    int indx = 0;
    uint32_t volatile l2arraccadr;
    uint32_t volatile l2arraccctl;
    bool valid = false;

    rumboot_putstring( "l2c_arracc_tag_info_read_by_way\n" );
    rumboot_putstring( "address upper == " );
    rumboot_puthex( ext_phys_addr );
    rumboot_putstring( "address lower == " );
    rumboot_puthex( phys_addr );
    rumboot_putstring( "cache_way == " );
    rumboot_puthex( cache_way );
    l2arraccadr = l2c_l2_read( base, L2C_L2ARRACCADR ) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read( base, L2C_L2ARRACCCTL ) & ~L2C_ARRACCCTL_MSK;

    l2c_l2_write( base, L2C_L2ARRACCADR, ( ( l2arraccadr ) | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD( phys_addr ) ) );

    l2c_l2_write(
            base,
            L2C_L2ARRACCCTL,
            ( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
            L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                    | L2C_ARRACCCTL_WAY_FIELD( cache_way ) | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr) );
    while( ( ( l2c_l2_read( base, L2C_L2ARRACCCTL ) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD ) == 0 )
            && ( indx < L2C_TIMEOUT ) )
        indx++;
    valid = ( indx < L2C_TIMEOUT );

    if( valid ) {
        *tag_info = l2c_l2_read( base, L2C_L2ARRACCDO0 );
        rumboot_putstring( "tag_info == " );
        rumboot_puthex( *tag_info );
    } else {
        rumboot_putstring( "Timeout while accessing L2C via DCR.\n" );
    }

    return valid;

}

bool l2c_arracc_data_read_by_way( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way,
        volatile uint64_t* cache_data ) {

    int indx;
    uint32_t volatile l2arraccadr;
    uint32_t volatile l2arraccctl;
    bool valid = false;

    rumboot_putstring( "l2c_arracc_data_read_by_way\n" );
    rumboot_putstring( "address upper == " );
    rumboot_puthex( ext_phys_addr );
    rumboot_putstring( "address lower == " );
    rumboot_puthex( phys_addr );
    l2arraccadr = l2c_l2_read( base, L2C_L2ARRACCADR ) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read( base, L2C_L2ARRACCCTL ) & ~L2C_ARRACCCTL_MSK;

    l2c_l2_write( base, L2C_L2ARRACCADR, ( ( l2arraccadr ) | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD( phys_addr ) ) );

    l2c_l2_write(
            base,
            L2C_L2ARRACCCTL,
            ( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
            L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                    | L2C_ARRACCCTL_WAY_FIELD( cache_way ) | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr) );

    indx = 0;
    while( ( ( l2c_l2_read( base, L2C_L2ARRACCCTL ) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD ) == 0 )
            && ( indx < L2C_TIMEOUT ) )
        indx++;
    valid = ( indx < L2C_TIMEOUT );

    if( valid ) {
        *cache_data = ( l2c_l2_read( base, L2C_L2ARRACCDO1 )
                | ( ( uint64_t )l2c_l2_read( base, L2C_L2ARRACCDO0 ) << 32 ) );
        rumboot_putstring( "cache_data upper == " );
        rumboot_puthex( l2c_l2_read( base, L2C_L2ARRACCDO0 ) );
        rumboot_putstring( "cache_data lower == " );
        rumboot_puthex( l2c_l2_read( base, L2C_L2ARRACCDO1 ) );
        rumboot_putstring( "cache_data upper == " );
        rumboot_puthex( ( uint32_t )( ( ( *cache_data ) >> 32 ) & 0xFFFFFFFF ) );
        rumboot_putstring( "cache_data lower == " );
        rumboot_puthex( ( uint32_t )( *cache_data & 0xFFFFFFFF ) );
    } else {
        rumboot_putstring( "Timeout while accessing L2C via DCR.\n" );
    }

    return valid;
}

bool l2c_arracc_data_write_by_way( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way,
        uint64_t cache_data ) {

    int indx;
    uint32_t volatile l2arraccadr;
    uint32_t volatile l2arraccctl;
    bool valid = false;

    rumboot_putstring( "l2c_arracc_data_write_by_way\n" );
    rumboot_putstring( "address upper == " );
    rumboot_puthex( ext_phys_addr );
    rumboot_putstring( "address lower == " );
    rumboot_puthex( phys_addr );
    rumboot_putstring( "cache_data upper == " );
    rumboot_puthex( ( uint32_t )( ( cache_data >> 32 ) & 0xFFFFFFFF ) );
    rumboot_putstring( "cache_data lower == " );
    rumboot_puthex( ( uint32_t )( cache_data & 0xFFFFFFFF ) );
    l2arraccadr = l2c_l2_read( base, L2C_L2ARRACCADR ) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read( base, L2C_L2ARRACCCTL ) & ~L2C_ARRACCCTL_MSK;

    l2c_l2_write( base, L2C_L2ARRACCDI0, (uint32_t)(cache_data >> 32));
    l2c_l2_write( base, L2C_L2ARRACCDI1, (uint32_t)(cache_data & 0xFFFFFFFF));

    l2c_l2_write( base, L2C_L2ARRACCADR, ( ( l2arraccadr ) | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD( phys_addr ) ) );

    l2c_l2_write(
            base,
            L2C_L2ARRACCCTL,
            ( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
            L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_WRITE_8BYTE_GENERATE_ECC_FIELD
                    | L2C_ARRACCCTL_WAY_FIELD( cache_way ) | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr) );

    indx = 0;
    while( ( ( l2c_l2_read( base, L2C_L2ARRACCCTL ) & L2C_ARRACCCTL_WRITE_REQUEST_COMP_FIELD ) == 0 )
            && ( indx < L2C_TIMEOUT ) )
        indx++;
    valid = ( indx < L2C_TIMEOUT );

    if( !valid ) {
        rumboot_putstring( "Timeout while accessing L2C via DCR.\n" );
    }

    return valid;
}

bool l2c_arracc_get_way_by_address( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr,
        volatile int32_t* cache_way ) {
    uint64_t addr64;
    uint32_t tag_info;
    bool valid = false;
    bool tag_valid;

    rumboot_putstring( "l2c_arracc_get_way_by_address\n" );
    rumboot_putstring( "address upper == " );
    rumboot_puthex( ext_phys_addr );
    rumboot_putstring( "address lower == " );
    rumboot_puthex( phys_addr );
    addr64 = ( ( uint64_t )( ext_phys_addr & L2C_EXT_ADDR_MSK ) << 32 ) + phys_addr;
    ( *cache_way ) = -1;
    rumboot_putstring( "cache_way == " );
    rumboot_puthex( ( int32_t )( *cache_way ) );
    do {
        ( int32_t )( *cache_way )++;
        tag_valid = l2c_arracc_tag_info_read_by_way( base, ext_phys_addr, phys_addr, *cache_way, &tag_info );
        rumboot_putstring( "cache_way == " );
        rumboot_puthex( ( int32_t )( *cache_way ) );
        rumboot_putstring( "tag_valid == " );
        rumboot_puthex( tag_valid );
    } while( ( ( ( int32_t )( *cache_way ) ) < L2C_WAY3_NUM )
            && ( ( ( uint64_t )L2C_TAG_ADDRESS_FROM_64BIT( addr64 )
                    != ( uint64_t )L2C_TAG_ADDRESS_FROM_ARRACCDO0( tag_info ) ) || !tag_valid ) );
    rumboot_putstring( "l2c_arracc_get_way_by_address loop finished\n" );
    if( ( L2C_TAG_ADDRESS_FROM_ARRACCDO0(tag_info) == L2C_TAG_ADDRESS_FROM_64BIT( addr64 ) ) && tag_valid ) {
        valid = true;
    } else {
        rumboot_putstring( "Tag address not found in Tag array.\n" );
        valid = false;
    }
    return valid;
}

bool l2c_arracc_get_data_by_address( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr,
        volatile uint64_t* data64 ) {
    int32_t cache_way = L2C_WAY0_NUM - 1;
    bool valid = false;

    rumboot_putstring( "l2c_arracc_get_data_by_address\n" );
    rumboot_putstring( "address upper == " );
    rumboot_puthex( ext_phys_addr );
    rumboot_putstring( "address lower == " );
    rumboot_puthex( phys_addr );
    if( l2c_arracc_get_way_by_address( base, ext_phys_addr, phys_addr, &cache_way ) ) {
        valid = l2c_arracc_data_read_by_way( base, ext_phys_addr, phys_addr, cache_way, data64 );
    } else {
        rumboot_putstring( "Tag address not found in Tag array.\n" );
    }
    return valid;
}

bool l2c_arracc_set_data_by_address( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr,
        uint64_t data64 ) {
    int32_t cache_way = L2C_WAY0_NUM - 1;
    bool valid = false;

    rumboot_putstring( "l2c_arracc_set_data_by_address\n" );
    rumboot_putstring( "address upper == " );
    rumboot_puthex( ext_phys_addr );
    rumboot_putstring( "address lower == " );
    rumboot_puthex( phys_addr );
    rumboot_putstring( "data64 upper == " );
    rumboot_puthex( ( uint32_t )( ( data64 >> 32 ) & 0xFFFFFFFF ) );
    rumboot_putstring( "data64 lower == " );
    rumboot_puthex( ( uint32_t )( data64 & 0xFFFFFFFF ) );
    if( l2c_arracc_get_way_by_address( base, ext_phys_addr, phys_addr, &cache_way ) ) {
        valid = l2c_arracc_data_write_by_way( base, ext_phys_addr, phys_addr, cache_way, data64 );
    } else {
        rumboot_putstring( "Tag address not found in Tag array.\n" );
    }
    return valid;
}

uint64_t l2c_arracc_data_read( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr ) {
    int indx;
    uint32_t data;
    uint32_t volatile l2arraccadr;
    uint32_t volatile l2arraccctl;
    int32_t cache_way = L2C_WAY0_NUM - 1;
    uint64_t addr64;

    //rumboot_putstring( "l2c_arracc_data_read\n" );
    l2arraccadr = l2c_l2_read( base, L2C_L2ARRACCADR ) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read( base, L2C_L2ARRACCCTL ) & ~L2C_ARRACCCTL_MSK;

    addr64 = ( ( uint64_t )( ext_phys_addr & L2C_EXT_ADDR_MSK ) << 32 ) + phys_addr;
    do {
        cache_way++;
        l2c_l2_write( base, L2C_L2ARRACCADR, ( ( l2arraccadr ) | L2C_ARRACCADR_TAG_LRU_ADDR_FIELD( phys_addr ) ) );
        l2c_l2_write(
                base,
                L2C_L2ARRACCCTL,
                ( ( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
                L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                        | L2C_ARRACCCTL_WAY_FIELD( cache_way ) ) );
        indx = 0;
        while( ( ( l2c_l2_read( base, L2C_L2ARRACCCTL ) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD ) == 0 )
                && ( indx < L2C_TIMEOUT ) )
            indx++;
        TEST_ASSERT( (indx < L2C_TIMEOUT), "Timeout while accessing L2C via DCR.\n" );
        data = l2c_l2_read( base, L2C_L2ARRACCDO0 );
    } while( ( cache_way < L2C_WAY3_NUM )
            && ( L2C_TAG_ADDRESS_FROM_64BIT(addr64) != L2C_TAG_ADDRESS_FROM_ARRACCDO0( data ) ) );
    COMPARE_VALUES( ( L2C_TAG_ADDRESS_FROM_ARRACCDO0(data) ), ( (uint64_t)L2C_TAG_ADDRESS_FROM_64BIT(addr64) ),
                    "Tag address not found in Tag array.\n" );

    l2c_l2_write( base, L2C_L2ARRACCADR, ( ( l2arraccadr ) | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD( phys_addr ) ) );

    l2c_l2_write(
            base,
            L2C_L2ARRACCCTL,
            ( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
            L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                    | L2C_ARRACCCTL_WAY_FIELD( cache_way ) | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr) );

    indx = 0;
    while( ( ( l2c_l2_read( base, L2C_L2ARRACCCTL ) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD ) == 0 )
            && ( indx < L2C_TIMEOUT ) )
        indx++;
    TEST_ASSERT( (indx < L2C_TIMEOUT), "Timeout while accessing L2C via DCR.\n" );
    return ( l2c_l2_read( base, L2C_L2ARRACCDO1 ) | ( ( uint64_t )l2c_l2_read( base, L2C_L2ARRACCDO0 ) << 32 ) );
}

void l2c_arracc_data_write( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, uint64_t data64) {
    int indx;
    uint32_t data;
    uint32_t volatile l2arraccadr;
    uint32_t volatile l2arraccctl;
    int32_t cache_way = L2C_WAY0_NUM - 1;
    uint64_t addr64;

    //rumboot_putstring( "l2c_arracc_data_write\n" );
    l2arraccadr = l2c_l2_read( base, L2C_L2ARRACCADR ) & ~L2C_L2ARRACCADR_MSK;
    l2arraccctl = l2c_l2_read( base, L2C_L2ARRACCCTL ) & ~L2C_ARRACCCTL_MSK;

    addr64 = ( ( uint64_t )( ext_phys_addr & L2C_EXT_ADDR_MSK ) << 32 ) + phys_addr;
    do {
        cache_way++;
        l2c_l2_write( base, L2C_L2ARRACCADR, ( ( l2arraccadr ) | L2C_ARRACCADR_TAG_LRU_ADDR_FIELD( phys_addr ) ) );
        l2c_l2_write(
                base,
                L2C_L2ARRACCCTL,
                ( ( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
                L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD
                        | L2C_ARRACCCTL_WAY_FIELD( cache_way ) ) );
        indx = 0;
        while( ( ( l2c_l2_read( base, L2C_L2ARRACCCTL ) & L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD ) == 0 )
                && ( indx < L2C_TIMEOUT ) )
            indx++;
        TEST_ASSERT( (indx < L2C_TIMEOUT), "Timeout while accessing L2C via DCR.\n" );
        data = l2c_l2_read( base, L2C_L2ARRACCDO0 );
    } while( ( cache_way < L2C_WAY3_NUM )
            && ( L2C_TAG_ADDRESS_FROM_64BIT(addr64) != L2C_TAG_ADDRESS_FROM_ARRACCDO0( data ) ) );
    COMPARE_VALUES( ( L2C_TAG_ADDRESS_FROM_ARRACCDO0(data) ), ( (uint64_t)L2C_TAG_ADDRESS_FROM_64BIT(addr64) ),
                    "Tag address not found in Tag array.\n" );

    l2c_l2_write(base,L2C_L2ARRACCDI0, (uint32_t)(data64 >> 32));
    l2c_l2_write(base,L2C_L2ARRACCDI1, (uint32_t)(data64 & 0xFFFFFFFF));

    l2c_l2_write( base, L2C_L2ARRACCADR, ( ( l2arraccadr ) | L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD( phys_addr ) ) );

    l2c_l2_write(
            base,
            L2C_L2ARRACCCTL,
            ( l2arraccctl ) | L2C_ARRACCCTL_REQUEST_FIELD |
            L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD | L2C_ARRACCCTL_WRITE_8BYTE_GENERATE_ECC_FIELD
                    | L2C_ARRACCCTL_WAY_FIELD( cache_way ) | L2C_ARRACCCTL_MSK_DWORD_FIELD(phys_addr) );

    indx = 0;
    while( ( ( l2c_l2_read( base, L2C_L2ARRACCCTL ) & L2C_ARRACCCTL_WRITE_REQUEST_COMP_FIELD ) == 0 )
            && ( indx < L2C_TIMEOUT ) )
        indx++;
    TEST_ASSERT( (indx < L2C_TIMEOUT), "Timeout while accessing L2C via DCR.\n" );
}
