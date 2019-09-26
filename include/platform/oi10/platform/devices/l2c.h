#ifndef L2C_H
#define L2C_H

#include <stdbool.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c_pmu.h>
#include <platform/arch/ppc/test_macro.h>


//L2C REG value
typedef enum
{
    L2C_L2CNFG1_VAL     = reg_field(26, 0x1),
    L2C_L2DBGSEL_VAL    = 0x00000000,
    L2C_L2DBGDATA0_VAL  = 0x00000000,
    L2C_L2DBGDATA1_VAL  = 0x00000000,
    L2C_L2SLEEPSTAT_VAL = 0x00000000,
    L2C_L2SLEEPREQ_VAL  = 0x00000000,
    L2C_L2MCK_VAL       = 0x00000000,
    L2C_L2MCKEN_VAL     = reg_field(30, 0x7FF),
    L2C_L2FERR_VAL      = 0x00000000,
    L2C_L2INT_VAL       = 0x00000000,
    L2C_L2INTEN_VAL     = reg_field(30, 0x3FF),
    L2C_L2LOG0_VAL      = 0x00000000,
    L2C_L2LOG1_VAL      = 0x00000000,
    L2C_L2LOG2_VAL      = 0x00000000,
    L2C_L2LOG3_VAL      = 0x00000000,
    L2C_L2LOG4_VAL      = 0x00000000,
    L2C_L2LOG5_VAL      = 0x00000000,
    L2C_L2PLBCFG_VAL    = 0x00000000,
    L2C_L2PLBDBG_VAL    = 0x00000000,
    L2C_L2PLBERAP_VAL   = 0x00000000,
    L2C_L2PLBSTAT0_VAL  = 0x00000000,
    L2C_L2PLBSTAT1_VAL  = 0x00000000,
    L2C_L2PLBFRC0_VAL   = 0x00000000,
    L2C_L2PLBFRC1_VAL   = 0x00000000,
    L2C_L2PLBMCKEN0_VAL = 0xFFFFFFFF,
    L2C_L2PLBMCKEN1_VAL = 0xFFFFFFFF,
    L2C_L2PLBFERR0_VAL  = 0x00000000,
    L2C_L2PLBFERR1_VAL  = 0x00000000,
    L2C_L2PLBINTEN0_VAL = 0x00000000,
    L2C_L2PLBINTEN1_VAL = 0x00000000,
    L2C_L2ARRCFG_VAL    = 0x00000000,
    L2C_L2ARRDBG0_VAL   = 0x00000000,
    L2C_L2ARRDBG1_VAL   = 0x00000000,
    L2C_L2ARRDBG2_VAL   = 0x00000000,
    L2C_L2ARRDBG3_VAL   = 0x00000000,
    L2C_L2ARRACCCTL_VAL = 0x00000000,
    L2C_L2ARRACCADR_VAL = 0x00000000,
    L2C_L2ARRACCDI0_VAL = 0x00000000,
    L2C_L2ARRACCDI1_VAL = 0x00000000,
    L2C_L2ARRACCDI2_VAL = 0x00000000,
    L2C_L2ARRACCDO0_VAL = 0x00000000,
    L2C_L2ARRACCDO1_VAL = 0x00000000,
    L2C_L2ARRACCDO2_VAL = 0x00000000,
    L2C_L2ARRSTAT0_VAL  = 0x00000000,
    L2C_L2ARRSTAT1_VAL  = 0x00000000,
    L2C_L2ARRSTAT2_VAL  = 0x00000000,
    L2C_L2ARRFRC0_VAL   = 0x00000000,
    L2C_L2ARRFRC1_VAL   = 0x00000000,
    L2C_L2ARRFRC2_VAL   = 0x00000000,
    L2C_L2ARRMCKEN0_VAL = 0xFFFFFFFF,
    L2C_L2ARRMCKEN1_VAL = 0x00001FFF,
    L2C_L2ARRMCKEN2_VAL = 0x00000000,
    L2C_L2ARRFERR0_VAL  = 0x00000000,
    L2C_L2ARRFERR1_VAL  = 0x00000000,
    L2C_L2ARRFERR2_VAL  = 0x00000000,
    L2C_L2ARRINTEN0_VAL = 0x00000000,
    L2C_L2ARRINTEN1_VAL = 0x00000000,
    L2C_L2ARRINTEN2_VAL = 0x00FFFFFF,
    L2C_L2CPUCFG_VAL    = 0x00000000,
    L2C_L2CPUDBG_VAL    = 0x00000000,
    L2C_L2CPUSTAT_VAL   = 0x00000000,
    L2C_L2CPUFRC_VAL    = 0x00000000,
    L2C_L2CPUMCKEN_VAL  = 0x00000FFF,
    L2C_L2CPUFERR_VAL   = 0x00000000,
    L2C_L2CPUINTEN_VAL  = 0x00000000,
    L2C_L2RACCFG_VAL    = 0x00000000,
    L2C_L2RACDBG0_VAL   = 0x00000000,
    L2C_L2RACDBG1_VAL   = 0x00000000,
    L2C_L2RACSTAT0_VAL  = 0x00000000,
    L2C_L2RACFRC0_VAL   = 0x00000000,
    L2C_L2RACMCKEN0_VAL = 0x03FFFFFF,
    L2C_L2RACFERR0_VAL  = 0x00000000,
    L2C_L2RACINTEN0_VAL = 0x00000000,
    L2C_L2WACCFG_VAL    = 0x00000000,
    L2C_L2WACDBG0_VAL   = 0x00000000,
    L2C_L2WACDBG1_VAL   = 0x00000000,
    L2C_L2WACDBG2_VAL   = 0x00000000,
    L2C_L2WACSTAT0_VAL  = 0x00000000,
    L2C_L2WACSTAT1_VAL  = 0x00000000,
    L2C_L2WACSTAT2_VAL  = 0x00000000,
    L2C_L2WACFRC0_VAL   = 0x00000000,
    L2C_L2WACFRC1_VAL   = 0x00000000,
    L2C_L2WACFRC2_VAL   = 0x00000000,
    L2C_L2WACMCKEN0_VAL = 0x00FFFFFF,
    L2C_L2WACMCKEN1_VAL = 0x00FFFFFF,
    L2C_L2WACMCKEN2_VAL = 0x00FFFFFF,
    L2C_L2WACFERR0_VAL  = 0x00000000,
    L2C_L2WACFERR1_VAL  = 0x00000000,
    L2C_L2WACFERR2_VAL  = 0x00000000,
    L2C_L2WACINTEN0_VAL = 0x00000000,
    L2C_L2WACINTEN1_VAL = 0x00000000,
    L2C_L2WACINTEN2_VAL = 0x00000000

} l2c_reg_val_t;

//L2INTEN bits
typedef enum
{
    L2INTEN_EXTINT      = (1 << L2C_L2INTEN_EXTINT_i),
    L2INTEN_PLBINT0     = (1 << L2C_L2INTEN_PLBINT0_i),
    L2INTEN_PLBINT1     = (1 << L2C_L2INTEN_PLBINT1_i),
    L2INTEN_L2AINT0     = (1 << L2C_L2INTEN_L2AINT0_i),
    L2INTEN_L2AINT1     = (1 << L2C_L2INTEN_L2AINT1_i),
    L2INTEN_L2AINT2     = (1 << L2C_L2INTEN_L2AINT2_i),
    L2INTEN_L1CINT0     = (1 << L2C_L2INTEN_L1CINT0_i),
    L2INTEN_RACINT0     = (1 << L2C_L2INTEN_RACINT0_i),
    L2INTEN_WACINT0     = (1 << L2C_L2INTEN_WACINT0_i),
    L2INTEN_WACINT1     = (1 << L2C_L2INTEN_WACINT1_i),
    L2INTEN_WACINT2     = (1 << L2C_L2INTEN_WACINT2_i),
} L2INTEN_bits_t;

//L2MCKEN bits
typedef enum
{
    L2MCKEN_EXTMCK      = (1 << L2C_L2MCKEN_EXTMCK_i),
    L2MCKEN_PLBMCK0     = (1 << L2C_L2MCKEN_PLBMCK0_i),
    L2MCKEN_PLBMCK1     = (1 << L2C_L2MCKEN_PLBMCK1_i),
    L2MCKEN_L2AMCK0     = (1 << L2C_L2MCKEN_L2AMCK0_i),
    L2MCKEN_L2AMCK1     = (1 << L2C_L2MCKEN_L2AMCK1_i),
    L2MCKEN_L2AMCK2     = (1 << L2C_L2MCKEN_L2AMCK2_i),
    L2MCKEN_L1CMCK0     = (1 << L2C_L2MCKEN_L1CMCK0_i),
    L2MCKEN_RACMCK0     = (1 << L2C_L2MCKEN_RACMCK0_i),
    L2MCKEN_WACMCK0     = (1 << L2C_L2MCKEN_WACMCK0_i),
    L2MCKEN_WACMCK1     = (1 << L2C_L2MCKEN_WACMCK1_i),
    L2MCKEN_WACMCK2     = (1 << L2C_L2MCKEN_WACMCK2_i),
} L2MCKEN_bits_t;

//L2PLBINTEN0 bits
typedef enum
{
    L2PLBINTEN0_IntvnDataPE0    = (1 << 31),
    L2PLBINTEN0_IntvnDataPE1    = (1 << 30),
    L2PLBINTEN0_IntvnDataPE2    = (1 << 29),
    L2PLBINTEN0_IntvnDataPE3    = (1 << 28),
    L2PLBINTEN0_IntvnDataPE4    = (1 << 27),
    L2PLBINTEN0_IntvnDataPE5    = (1 << 26),
    L2PLBINTEN0_IntvnDataPE6    = (1 << 25),
    L2PLBINTEN0_IntvnDataPE7    = (1 << 24),
    L2PLBINTEN0_IntvnDataPE8    = (1 << 23),
    L2PLBINTEN0_IntvnDataPE9    = (1 << 22),
    L2PLBINTEN0_IntvnDataPE10   = (1 << 21),
    L2PLBINTEN0_IntvnDataPE11   = (1 << 20),
    L2PLBINTEN0_IntvnDataPE12   = (1 << 19),
    L2PLBINTEN0_IntvnDataPE13   = (1 << 18),
    L2PLBINTEN0_IntvnDataPE14   = (1 << 17),
    L2PLBINTEN0_IntvnDataPE15   = (1 << 16),
    L2PLBINTEN0_WrDataPE0       = (1 << 15),
    L2PLBINTEN0_WrDataPE1       = (1 << 14),
    L2PLBINTEN0_WrDataPE2       = (1 << 13),
    L2PLBINTEN0_WrDataPE3       = (1 << 12),
    L2PLBINTEN0_WrDataPE4       = (1 << 11),
    L2PLBINTEN0_WrDataPE5       = (1 << 10),
    L2PLBINTEN0_WrDataPE6       = (1 << 9),
    L2PLBINTEN0_WrDataPE7       = (1 << 8),
    L2PLBINTEN0_WrDataPE8       = (1 << 7),
    L2PLBINTEN0_WrDataPE9       = (1 << 6),
    L2PLBINTEN0_WrDataPE10      = (1 << 5),
    L2PLBINTEN0_WrDataPE11      = (1 << 4),
    L2PLBINTEN0_WrDataPE12      = (1 << 3),
    L2PLBINTEN0_WrDataPE13      = (1 << 2),
    L2PLBINTEN0_WrDataPE14      = (1 << 1),
    L2PLBINTEN0_WrDataPE15      = (1)
} L2PLBINTEN0_bits_t;

//L2PLBMCKEN0 bits
typedef enum
{
    L2PLBMCKEN0_IntvnDataPE0    = (1 << 31),
    L2PLBMCKEN0_IntvnDataPE1    = (1 << 30),
    L2PLBMCKEN0_IntvnDataPE2    = (1 << 29),
    L2PLBMCKEN0_IntvnDataPE3    = (1 << 28),
    L2PLBMCKEN0_IntvnDataPE4    = (1 << 27),
    L2PLBMCKEN0_IntvnDataPE5    = (1 << 26),
    L2PLBMCKEN0_IntvnDataPE6    = (1 << 25),
    L2PLBMCKEN0_IntvnDataPE7    = (1 << 24),
    L2PLBMCKEN0_IntvnDataPE8    = (1 << 23),
    L2PLBMCKEN0_IntvnDataPE9    = (1 << 22),
    L2PLBMCKEN0_IntvnDataPE10   = (1 << 21),
    L2PLBMCKEN0_IntvnDataPE11   = (1 << 20),
    L2PLBMCKEN0_IntvnDataPE12   = (1 << 19),
    L2PLBMCKEN0_IntvnDataPE13   = (1 << 18),
    L2PLBMCKEN0_IntvnDataPE14   = (1 << 17),
    L2PLBMCKEN0_IntvnDataPE15   = (1 << 16),
    L2PLBMCKEN0_WrDataPE0       = (1 << 15),
    L2PLBMCKEN0_WrDataPE1       = (1 << 14),
    L2PLBMCKEN0_WrDataPE2       = (1 << 13),
    L2PLBMCKEN0_WrDataPE3       = (1 << 12),
    L2PLBMCKEN0_WrDataPE4       = (1 << 11),
    L2PLBMCKEN0_WrDataPE5       = (1 << 10),
    L2PLBMCKEN0_WrDataPE6       = (1 << 9),
    L2PLBMCKEN0_WrDataPE7       = (1 << 8),
    L2PLBMCKEN0_WrDataPE8       = (1 << 7),
    L2PLBMCKEN0_WrDataPE9       = (1 << 6),
    L2PLBMCKEN0_WrDataPE10      = (1 << 5),
    L2PLBMCKEN0_WrDataPE11      = (1 << 4),
    L2PLBMCKEN0_WrDataPE12      = (1 << 3),
    L2PLBMCKEN0_WrDataPE13      = (1 << 2),
    L2PLBMCKEN0_WrDataPE14      = (1 << 1),
    L2PLBMCKEN0_WrDataPE15      = (1)
} L2PLBMCKEN0_bits_t;

//L2PLBFRC0 bits
typedef enum
{
    L2PLBFRC0_IntvnDataPE0      = (1 << L2C_L2PLBFRC0_IntvnDataPE0_i),
    L2PLBFRC0_IntvnDataPE1      = (1 << L2C_L2PLBFRC0_IntvnDataPE1_i),
    L2PLBFRC0_IntvnDataPE2      = (1 << L2C_L2PLBFRC0_IntvnDataPE2_i),
    L2PLBFRC0_IntvnDataPE3      = (1 << L2C_L2PLBFRC0_IntvnDataPE3_i),
    L2PLBFRC0_IntvnDataPE4      = (1 << L2C_L2PLBFRC0_IntvnDataPE4_i),
    L2PLBFRC0_IntvnDataPE5      = (1 << L2C_L2PLBFRC0_IntvnDataPE5_i),
    L2PLBFRC0_IntvnDataPE6      = (1 << L2C_L2PLBFRC0_IntvnDataPE6_i),
    L2PLBFRC0_IntvnDataPE7      = (1 << L2C_L2PLBFRC0_IntvnDataPE7_i),
    L2PLBFRC0_IntvnDataPE8      = (1 << L2C_L2PLBFRC0_IntvnDataPE8_i),
    L2PLBFRC0_IntvnDataPE9      = (1 << L2C_L2PLBFRC0_IntvnDataPE9_i),
    L2PLBFRC0_IntvnDataPE10     = (1 << L2C_L2PLBFRC0_IntvnDataPE10_i),
    L2PLBFRC0_IntvnDataPE11     = (1 << L2C_L2PLBFRC0_IntvnDataPE11_i),
    L2PLBFRC0_IntvnDataPE12     = (1 << L2C_L2PLBFRC0_IntvnDataPE12_i),
    L2PLBFRC0_IntvnDataPE13     = (1 << L2C_L2PLBFRC0_IntvnDataPE13_i),
    L2PLBFRC0_IntvnDataPE14     = (1 << L2C_L2PLBFRC0_IntvnDataPE14_i),
    L2PLBFRC0_IntvnDataPE15     = (1 << L2C_L2PLBFRC0_IntvnDataPE15_i),
    L2PLBFRC0_WrDataPE0         = (1 << L2C_L2PLBFRC0_WrDataPE0_i),
    L2PLBFRC0_WrDataPE1         = (1 << L2C_L2PLBFRC0_WrDataPE1_i),
    L2PLBFRC0_WrDataPE2         = (1 << L2C_L2PLBFRC0_WrDataPE2_i),
    L2PLBFRC0_WrDataPE3         = (1 << L2C_L2PLBFRC0_WrDataPE3_i),
    L2PLBFRC0_WrDataPE4         = (1 << L2C_L2PLBFRC0_WrDataPE4_i),
    L2PLBFRC0_WrDataPE5         = (1 << L2C_L2PLBFRC0_WrDataPE5_i),
    L2PLBFRC0_WrDataPE6         = (1 << L2C_L2PLBFRC0_WrDataPE6_i),
    L2PLBFRC0_WrDataPE7         = (1 << L2C_L2PLBFRC0_WrDataPE7_i),
    L2PLBFRC0_WrDataPE8         = (1 << L2C_L2PLBFRC0_WrDataPE8_i),
    L2PLBFRC0_WrDataPE9         = (1 << L2C_L2PLBFRC0_WrDataPE9_i),
    L2PLBFRC0_WrDataPE10        = (1 << L2C_L2PLBFRC0_WrDataPE10_i),
    L2PLBFRC0_WrDataPE11        = (1 << L2C_L2PLBFRC0_WrDataPE11_i),
    L2PLBFRC0_WrDataPE12        = (1 << L2C_L2PLBFRC0_WrDataPE12_i),
    L2PLBFRC0_WrDataPE13        = (1 << L2C_L2PLBFRC0_WrDataPE13_i),
    L2PLBFRC0_WrDataPE14        = (1 << L2C_L2PLBFRC0_WrDataPE14_i),
    L2PLBFRC0_WrDataPE15        = (1 << L2C_L2PLBFRC0_WrDataPE15_i)
} L2PLBFRC0_bits_t;

//PMUIE0 bits
typedef enum
{
    PMUIE0_IE0      = (1 << L2C_PMUIE0_IE0_i),
    PMUIE0_IE1      = (1 << L2C_PMUIE0_IE1_i),
    PMUIE0_IE2      = (1 << L2C_PMUIE0_IE2_i),
    PMUIE0_IE3      = (1 << L2C_PMUIE0_IE3_i),
    PMUIE0_IE4      = (1 << L2C_PMUIE0_IE4_i),
    PMUIE0_IE5      = (1 << L2C_PMUIE0_IE5_i),
    PMUIE0_IE6      = (1 << L2C_PMUIE0_IE6_i),
    PMUIE0_IE7      = (1 << L2C_PMUIE0_IE7_i),
    PMUIE0_IE8      = (1 << L2C_PMUIE0_IE8_i),
    PMUIE0_IE9      = (1 << L2C_PMUIE0_IE9_i),
    PMUIE0_IE10     = (1 << L2C_PMUIE0_IE10_i),
    PMUIE0_IE11     = (1 << L2C_PMUIE0_IE11_i),
    PMUIE0_IE12     = (1 << L2C_PMUIE0_IE12_i),
    PMUIE0_IE13     = (1 << L2C_PMUIE0_IE13_i),
    PMUIE0_IE14     = (1 << L2C_PMUIE0_IE14_i),
    PMUIE0_IE15     = (1 << L2C_PMUIE0_IE15_i)
} PMUIE0_bits_t;

//PMULCX bits
typedef enum
{
    PMULCX_CMODE    = (1 << L2C_PMULCx_CMODE_i),
    PMULCX_FC       = (1 << L2C_PMULCx_FC_i),
    PMULCX_FCS      = (1 << L2C_PMULCx_FCS_i),
    PMULCX_FCU      = (1 << L2C_PMULCx_FCU_i),
    PMULCX_FCM1     = (1 << L2C_PMULCx_FCM1_i),
    PMULCX_FCM0     = (1 << L2C_PMULCx_FCM0_i),
    PMULCX_CE       = (1 << L2C_PMULCx_CE_i)
} PMULCX_bits_t;


void l2c_global_enable_interrupt(uint32_t l2c_dcr_base, L2INTEN_bits_t mask);
void l2c_global_mck_enable(uint32_t l2c_dcr_base, L2MCKEN_bits_t mask);
void l2c_global_mck_disable(uint32_t l2c_dcr_base, L2MCKEN_bits_t mask);

void l2c_pmu_set_CE_bit(uint32_t pmu_dcr_base, L2C_PMUREG dcr_index);
void l2c_pmu_enable_interrupt(uint32_t pmu_dcr_base, L2C_PMUREG dcr_index, uint32_t mask);
void l2c_pmu_disable_interrupt( uint32_t const pmu_dcr_base, L2C_PMUREG const dcr_index, uint32_t const mask );
void l2c_pmu_clear_interrupt(uint32_t pmu_dcr_base);
void l2c_pmu_set_cx(uint32_t pmu_dcr_base, L2C_PMUREG dcr_index, uint32_t value);

void l2c_enable_interrupt(uint32_t l2c_dcr_base, L2C_L2REG l2c_int_reg_index, uint32_t mask);

uint64_t l2c_arracc_data_read (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr);
void l2c_arracc_data_write( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, uint64_t data64);
bool l2c_arracc_tag_info_read_by_way (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, volatile uint32_t* tag_info);
bool l2c_arracc_tag_info_write_by_way_wo_gen_ecc( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, uint32_t cache_data, uint32_t ecc_data);
bool l2c_arracc_data_read_by_way (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, volatile uint64_t* cache_data);
bool l2c_arracc_data_write_by_way (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, uint64_t cache_data);
bool l2c_arracc_data_write_by_way_wo_gen_ecc( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, uint64_t cache_data, uint32_t ecc_data);
bool l2c_arracc_get_way_by_address (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, volatile int32_t* cache_way);
bool l2c_arracc_get_data_by_address (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, volatile uint64_t* data64);
bool l2c_arracc_set_data_by_address (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, uint64_t data64);
bool l2c_arracc_lru_info_read_by_way( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, volatile uint32_t* lru_info );


enum l2c_mem_type {
    L2C_MEM_TAG,
    L2C_MEM_TAG_ECC,
    L2C_MEM_LRU,
    L2C_MEM_DATA,
    L2C_MEM_DATA_ECC,
};

struct l2c_mem_layout {
    L2C_L2Size_t    l2size;

    uint32_t        tag_array_size;
    uint32_t        lru_array_size;
    uint32_t        data_array_size;

    uint32_t        tag_i;
    uint32_t        tag_n;
    uint32_t        tag_ecc_i;
    uint32_t        tag_ecc_n;
    uint32_t        lru_i;
    uint32_t        lru_n;
    uint32_t        data_ecc_i;
    uint32_t        data_ecc_n;
};

void l2c_get_mem_layout( uint32_t base, struct l2c_mem_layout * mem_layout );


#endif  /* L2C_H */
