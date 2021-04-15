#include <arch/common_macros.h>
#include <arch/ibm_bit_ordering_macros.h>
#include <assert.h>
#include <platform/devices.h>
#include <rumboot/printf.h>
#include <devices/ugly/plb6mcif2.h>

#define PLB6MCIF2_MEMRANK_N 4
#define PLB6MCIF2_WATERMARK 0x00000707

BEGIN_ENUM( PLB6MCIF2_REG )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_read,     0x00 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_clear,    0x00 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_set,      0x01 )
DECLARE_ENUM_VAL( PLB6MCIF2_BEARL,         0x02 )
DECLARE_ENUM_VAL( PLB6MCIF2_BEARU,         0x03 )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN,       0x04 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC,      0x07 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD,        0x08 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG,        0x09 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM,        0x0F )
DECLARE_ENUM_VAL( PLB6MCIF2_PUABA,         0x10 )
DECLARE_ENUM_VAL( PLB6MCIF2_MR0CF,         0x11 )
DECLARE_ENUM_VAL( PLB6MCIF2_MR1CF,         0x12 )
DECLARE_ENUM_VAL( PLB6MCIF2_MR2CF,         0x13 )
DECLARE_ENUM_VAL( PLB6MCIF2_MR3CF,         0x14 )
DECLARE_ENUM_VAL( PLB6MCIF2_SR0CF,         0x15 )
DECLARE_ENUM_VAL( PLB6MCIF2_SR1CF,         0x16 )
DECLARE_ENUM_VAL( PLB6MCIF2_SR2CF,         0x17 )
DECLARE_ENUM_VAL( PLB6MCIF2_SR3CF,         0x18 )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG1,      0x1D )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG2,      0x1E )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG3,      0x1F )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS,        0x20 )
DECLARE_ENUM_VAL( PLB6MCIF2_RID,           0x21 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAP0CF,        0x22 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAP1CF,        0x23 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAP2CF,        0x24 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAP3CF,        0x25 )
END_ENUM( PLB6MCIF2_REG )

BEGIN_ENUM( PLB6MCIF2_REG_DFLT )
DECLARE_ENUM_VAL( PLB6MCIF2_BEARL_DFLT,     0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_BEARU_DFLT,     0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_DFLT,      0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_DFLT,   0xFFE001C0 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_DFLT,    0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_DFLT,    0xC0000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_DFLT,     0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG1_DFLT,  0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG2_DFLT,  0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG3_DFLT,  0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_DFLT,  0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_DFLT,    0x200000F0 )
DECLARE_ENUM_VAL( PLB6MCIF2_PUABA_DFLT,     0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_DFLT,     0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS_DFLT,    0x80000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_RID_DFLT,       PLB6MCIF2_WATERMARK )
END_ENUM( PLB6MCIF2_REG_DFLT )

BEGIN_ENUM( PLB6MCIF2_REG_MSK )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_MSK,     0xC0000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_MSK,      0xFFE0F001 )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG1_MSK,   0xFFFFFFFC )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG2_MSK,   0xFFFFFFFC )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG3_MSK,   0xFFFFF000 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_MSK,   0xCFFFFF00 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_MSK,     0xF100E000 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_MSK,      0xFFF8F001 )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS_MSK,     0x80000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_MSK,     0xFC0000FF )
END_ENUM( PLB6MCIF2_REG_MSK )

#define REG_READ(DEV_NAME, REG, REG_SIZE)                                                               \
inline static uint##REG_SIZE##_t DEV_NAME##_read_##REG(uint32_t const base_addr)                        \
{                                                                                                       \
    return *((volatile uint##REG_SIZE##_t*)(base_addr + REG));                                          \
}

#define REG_READ_ADDR_NAME(DEV_NAME, REG_ADDR, REG_NAME, REG_SIZE)                                  \
inline static uint##REG_SIZE##_t DEV_NAME##_read_##REG_NAME(uint32_t const base_addr)               \
{                                                                                                       \
    return *((volatile uint##REG_SIZE##_t*)(base_addr + REG_ADDR));                                          \
}

#define REG_READ_DCR(DEV_NAME, REG, REG_SIZE)                                                            \
inline static uint##REG_SIZE##_t DEV_NAME##_dcr_read_##REG(uint32_t const base_addr)               \
{                                                                                                       \
    return dcr_read(base_addr + REG);                                                                \
}

#define REG_READ_DCR_ADDR_NAME(DEV_NAME, REG_ADDR, REG_NAME, REG_SIZE)                                  \
inline static uint##REG_SIZE##_t DEV_NAME##_dcr_read_##REG_NAME(uint32_t const base_addr)               \
{                                                                                                       \
    return dcr_read(base_addr + REG_ADDR);                                                                \
}

#define REG_WRITE(DEV_NAME, REG, REG_SIZE)                                                              \
inline static void DEV_NAME##_write_##REG(uint32_t const base_addr, uint##REG_SIZE##_t const value)     \
{                                                                                                       \
    *((volatile uint##REG_SIZE##_t*)(base_addr + REG)) = value;                                         \
}

#define REG_WRITE_ADDR_NAME(DEV_NAME, REG_ADDR, REG_NAME, REG_SIZE)                                 \
inline static void DEV_NAME##_write_##REG_NAME(uint32_t const base_addr, uint##REG_SIZE##_t const value) \
{                                                                                                       \
    *((volatile uint##REG_SIZE##_t*)(base_addr + REG_ADDR)) = value;                                         \
}

#define REG_WRITE_DCR(DEV_NAME, REG, REG_SIZE)                                                          \
inline static void DEV_NAME##_dcr_write_##REG(uint32_t const base_addr, uint##REG_SIZE##_t const value) \
{                                                                                                       \
    dcr_write(base_addr + REG, value);                                                                \
}

#define REG_WRITE_DCR_ADDR_NAME(DEV_NAME, REG_ADDR, REG_NAME, REG_SIZE)                                 \
inline static void DEV_NAME##_dcr_write_##REG_NAME(uint32_t const base_addr, uint##REG_SIZE##_t const value) \
{                                                                                                       \
    dcr_write(base_addr + REG_ADDR, value);                                                                \
}

#define REG_SET(DEV_NAME, REG, REG_SIZE)                                                                \
inline static void DEV_NAME##_set_##REG(uint32_t const base_addr, uint##REG_SIZE##_t const value)       \
{                                                                                                       \
    *((volatile uint##REG_SIZE##_t*)(base_addr + REG)) |= value;                                        \
}

#define REG_CLEAR(DEV_NAME, REG, REG_SIZE)                                                              \
inline static void DEV_NAME##_clear_##REG(uint32_t const base_addr, uint##REG_SIZE##_t const value)     \
{                                                                                                       \
    *((volatile uint##REG_SIZE##_t*)(base_addr + REG)) &= ~value;                                       \
}

#define REG_SET_DCR(DEV_NAME, REG, REG_SIZE)                                                            \
inline static void DEV_NAME##_dcr_set_##REG(uint32_t const base_addr, uint##REG_SIZE##_t const value)   \
{                                                                                                       \
    dcr_write(base_addr + REG, (dcr_read(base_addr + REG) | value));                                         \
}

#define REG_CLEAR_DCR(DEV_NAME, REG, REG_SIZE)                                                          \
inline static void DEV_NAME##_dcr_clear_##REG(uint32_t const base_addr, uint##REG_SIZE##_t const value) \
{                                                                                                       \
    dcr_write(base_addr + REG, (dcr_read(base_addr + REG) & ~value));                                        \
}

#define READ_MEM_REG(REG_NAME, REG_OFFSET, REG_SIZE)\
inline static uint##REG_SIZE##_t read_##REG_NAME(uint32_t const base_addr)\
{\
    return *((volatile uint##REG_SIZE##_t*)(base_addr + REG_OFFSET));\
}

#define WRITE_MEM_REG(REG_NAME, REG_OFFSET, REG_SIZE)\
inline static void write_##REG_NAME(uint32_t const base_addr, uint32_t const value)\
{\
    *((volatile uint##REG_SIZE##_t*)(base_addr + REG_OFFSET)) = value;\
}

#define READ_DCR_REG(REG_NAME, REG_OFFSET)\
inline static uint32_t read_##REG_NAME(uint32_t const base_addr)\
{\
    return dcr_read(base_addr + REG_OFFSET);\
}

#define WRITE_DCR_REG(REG_NAME, REG_OFFSET)\
inline static void write_##REG_NAME(uint32_t const base_addr, uint32_t const value)\
{\
    dcr_write(base_addr + REG_OFFSET, value);\
}

inline static uint32_t plb6mcif2_dcr_read_PLB6MCIF2_BESR( uint32_t const base_addr ) {
    return dcr_read( base_addr+PLB6MCIF2_BESR_read );
}
inline static void plb6mcif2_dcr_set_PLB6MCIF2_BESR( uint32_t const base_addr, uint32_t const value ) {
    dcr_write( base_addr+PLB6MCIF2_BESR_set, value );
}
inline static void plb6mcif2_dcr_clear_PLB6MCIF2_BESR( uint32_t const base_addr, uint32_t const value ) {
    dcr_write( base_addr+PLB6MCIF2_BESR_clear, value );
}


REG_READ_DCR( plb6mcif2, PLB6MCIF2_BEARL, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_BEARU, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_INTR_EN, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_INTR_EN, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_PLBASYNC, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_PLBASYNC, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_PLBORD, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_PLBORD, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_PLBCFG, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_PLBCFG, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAXMEM, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAXMEM, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_PUABA, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_PUABA, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MR0CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MR0CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MR1CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MR1CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MR2CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MR2CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MR3CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MR3CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_SR0CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_SR0CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_SR1CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_SR1CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_SR2CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_SR2CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_SR3CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_SR3CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG1, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG1, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG2, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG2, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG3, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_P6BMTAG3, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAP0CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAP0CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAP1CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAP1CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAP2CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAP2CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_MAP3CF, 32 )
REG_WRITE_DCR( plb6mcif2, PLB6MCIF2_MAP3CF, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_STATUS, 32 )

REG_READ_DCR( plb6mcif2, PLB6MCIF2_RID, 32 )


BEGIN_ENUM( PLB6MCIF2_BESR_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxRW_e,        0 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxRW_i,        IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxRW_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxRW_n,        1 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxID_e,        5 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxID_i,        IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxID_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxID_n,        5 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MTAG_e,        10 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MTAG_i,        IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MTAG_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MTAG_n,        5 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_BufferID_e,    14 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_BufferID_i,    IBM_BIT_INDEX(32, PLB6MCIF2_BESR_BufferID_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_BufferID_n,    4 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_0_e,    15 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_0_i,    IBM_BIT_INDEX(32, PLB6MCIF2_BESR_RS_Err_0_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_0_n,    1 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_1_5_e,   20 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_1_5_i,   IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxET1_1_5_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_1_5_n,   5 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_1_2_e,  22 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_1_2_i,  IBM_BIT_INDEX(32, PLB6MCIF2_BESR_RS_Err_1_2_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_1_2_n,  2 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET2_e,       25 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET2_i,       IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxET2_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET2_n,       3 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_0_e,     26 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_0_i,     IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxET1_0_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_0_n,     1 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET3_e,       31 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET3_i,       IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxET3_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET3_n,       4 )
END_ENUM( PLB6MCIF2_BESR_FIELD )

DECLARE_CONST( PLB6MCIF2_BESR_MxET1_n,  PLB6MCIF2_BESR_MxET1_0_n+PLB6MCIF2_BESR_MxET1_1_5_n )
DECLARE_CONST( PLB6MCIF2_BESR_RS_Err_n, PLB6MCIF2_BESR_RS_Err_0_n+PLB6MCIF2_BESR_RS_Err_1_2_n )

BEGIN_ENUM( PLB6MCIF2_ERROR_TYPE1 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_No_Error,                               0 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_PLB6_Bus_Master_Access_Violation,       (1 << 5) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_Address_Decode_Miss,                    (1 << 4) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_Address_or_Byte_Enable_Parity_Error,    (1 << 3) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_PLB_Write_Data_Error,                   (1 << 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_MCIF_Read_Data_Error,                   (1 << 1) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_MCIF_Read_Parity_Error,                 (1 << 0) )
END_ENUM( PLB6MCIF2_ERROR_TYPE1 )

BEGIN_ENUM( PLB6MCIF2_ERROR_TYPE2 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET2_No_Error,                   0 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET2_PLB_Write_Parity_Error,     (1 << 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET2_MCIF_Write_Parity_Error,    (1 << 1) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET2_MCIF_Write_RMW_Error,       (1 << 0) )
END_ENUM( PLB6MCIF2_ERROR_TYPE2 )

BEGIN_ENUM( PLB6MCIF2_ERROR_TYPE3 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET3_No_flags,       0 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET3_Decode_flag,    (1 << 3) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET3_MCIFII_flag,    (1 << 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET3_Read_flag,      (1 << 1) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET3_Write_flag,     (1 << 0) )
END_ENUM( PLB6MCIF2_ERROR_TYPE3 )


BEGIN_ENUM( PLB6MCIF2_INTR_EN_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_ERR_EN_e,   15 )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_ERR_EN_i,   IBM_BIT_INDEX(32, PLB6MCIF2_INTR_EN_ERR_EN_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_ERR_EN_n,   16 )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_INTR_EN_e,  31 )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_INTR_EN_i,  IBM_BIT_INDEX(32, PLB6MCIF2_INTR_EN_INTR_EN_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_INTR_EN_n,  16 )
END_ENUM( PLB6MCIF2_INTR_EN_FIELD )


BEGIN_ENUM( PLB6MCIF2_PLBASYNC_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBgtMCIF_e,   0 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBgtMCIF_i,   IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_PLBgtMCIF_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBgtMCIF_n,   1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBeqMCIF_e,   1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBeqMCIF_i,   IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_PLBeqMCIF_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBeqMCIF_n,   1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLB6NUM_e,     7 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLB6NUM_i,     IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_PLB6NUM_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLB6NUM_n,     4 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_MCIFNUM_e,     11 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_MCIFNUM_i,     IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_MCIFNUM_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_MCIFNUM_n,     4 )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRDONE_TUNE_e, 12 )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRDONE_TUNE_i, IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_WRDONE_TUNE_e) )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRDONE_TUNE_n, 4 )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRRDY_TUNE_e,  16 )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRRDY_TUNE_i,  IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_WRRDY_TUNE_e) )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRRDY_TUNE_n,  4 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_RDRDY_TUNE_e,  23 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_RDRDY_TUNE_i,  IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_RDRDY_TUNE_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_RDRDY_TUNE_n,  4 )
END_ENUM( PLB6MCIF2_PLBASYNC_FIELD )


BEGIN_ENUM( PLB6MCIF2_PLBORD_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_RaW_e,           0 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_RaW_i,           IBM_BIT_INDEX(32, PLB6MCIF2_PLBORD_RaW_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_RaW_n,           1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_1_3_e, 3 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_1_3_i, IBM_BIT_INDEX(32, PLB6MCIF2_PLBORD_Threshold_1_3_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_1_3_n, 3 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_0_e,   7 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_0_i,   IBM_BIT_INDEX(32, PLB6MCIF2_PLBORD_Threshold_0_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_0_n,   1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_IT_OOO_e,        16 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_IT_OOO_i,        IBM_BIT_INDEX(32, PLB6MCIF2_PLBORD_IT_OOO_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_IT_OOO_n,        1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_TOM_e,           18 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_TOM_i,           IBM_BIT_INDEX(32, PLB6MCIF2_PLBORD_TOM_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_TOM_n,           2 )
END_ENUM( PLB6MCIF2_PLBORD_FIELD )

DECLARE_CONST( PLB6MCIF2_PLBORD_Threshold_n,    PLB6MCIF2_PLBORD_Threshold_0_n+PLB6MCIF2_PLBORD_Threshold_1_3_n )

BEGIN_ENUM( PLB6MCIF2_PLB_RAW_THRESHOLD )
DECLARE_ENUM_VAL( PLB6MCIF2_RAW_32DEEP,     (1 << 3) )
DECLARE_ENUM_VAL( PLB6MCIF2_RAW_16DEEP,     (1 << 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_RAW_8DEEP,      (1 << 1) )
END_ENUM( PLB6MCIF2_PLB_RAW_THRESHOLD )

BEGIN_ENUM( PLB6MCIF2_MCIF2_TOM )
DECLARE_ENUM_VAL( PLB6MCIF2_MCIF2_STRICT_IN_ORDER,          0x0 )
DECLARE_ENUM_VAL( PLB6MCIF2_MCIF2_PARTIAL_OUT_OF_ORDER,     0x2 )
DECLARE_ENUM_VAL( PLB6MCIF2_MCIF2_Full_OUT_OF_ORDER,        0x3 )
END_ENUM( PLB6MCIF2_MCIF2_TOM )


BEGIN_ENUM( PLB6MCIF2_PLBCFG_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Non_Coherent_e,      0 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Non_Coherent_i,      IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_Non_Coherent_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Non_Coherent_n,      1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Tsnoop_e,            4 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Tsnoop_i,            IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_Tsnoop_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Tsnoop_n,            4 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_PmstrID_chk_EN_e,    5 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_PmstrID_chk_EN_i,    IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_PmstrID_chk_EN_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_PmstrID_chk_EN_n,    1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Parity_Enables_e,    27 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Parity_Enables_i,    IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_Parity_Enables_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Parity_Enables_n,    4 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_CG_ENABLE_e,         30 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_CG_ENABLE_i,         IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_CG_ENABLE_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_CG_ENABLE_n,         3 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Br_Enable_e,         31 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Br_Enable_i,         IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_Br_Enable_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Br_Enable_n,         1 )
END_ENUM( PLB6MCIF2_PLBCFG_FIELD )

BEGIN_ENUM( PLB6MCIF2_TSNOOP )
#define PLB6MCIF2_PLBCFG_TSNOOP_x( i )\
DECLARE_ENUM_VAL( CAT( PLB6MCIF2_PLBCFG_TSNOOP_,i ),   i )
PLB6MCIF2_PLBCFG_TSNOOP_x (4)
PLB6MCIF2_PLBCFG_TSNOOP_x (5)
PLB6MCIF2_PLBCFG_TSNOOP_x (6)
PLB6MCIF2_PLBCFG_TSNOOP_x (7)
PLB6MCIF2_PLBCFG_TSNOOP_x (8)
END_ENUM( PLB6MCIF2_TSNOOP )

BEGIN_ENUM( PLB6MCIF2_PARITY )
DECLARE_ENUM_VAL( PLB6MCIF2_ADR_RS_PAR_EN,  (1 << 3) )
DECLARE_ENUM_VAL( PLB6MCIF2_BE_PAR_EN,      (1 << 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_CMD_PAR_EN,     (1 << 1) )
DECLARE_ENUM_VAL( PLB6MCIF2_MCIF_PAR_EN,    (1 << 0) )
END_ENUM( PLB6MCIF2_PARITY )

BEGIN_ENUM( PLB6MCIF2_CLK_GATING )
DECLARE_ENUM_VAL( PLB6MCIF2_CG_CLOCK_DOMAIN3,   (1 < 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_CG_CLOCK_DOMAIN4,   (1 < 1) )
DECLARE_ENUM_VAL( PLB6MCIF2_CG_CLOCK_DOMAIN5,   (1 < 0) )
END_ENUM( PLB6MCIF2_CLK_GATING )


BEGIN_ENUM( PLB6MCIF2_MAXMEM_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_e,   1 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_i,   IBM_BIT_INDEX(32, PLB6MCIF2_MAXMEM_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_n,   2 )
END_ENUM( PLB6MCIF2_MAXMEM_FIELD )

BEGIN_ENUM( PLB6MCIF2_PLB_MAXMEM )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_64GB,    0x3 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_32GB,    0x2 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_16GB,    0x1 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_8GB,     0x0 )
END_ENUM( PLB6MCIF2_PLB_MAXMEM )


BEGIN_ENUM( PLB6MCIF2_PUABA_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_PUABA_e,    31 )
DECLARE_ENUM_VAL( PLB6MCIF2_PUABA_i,    IBM_BIT_INDEX(32, PLB6MCIF2_PUABA_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PUABA_n,    32 )
END_ENUM( PLB6MCIF2_PUABA_FIELD )


BEGIN_ENUM( PLB6MCIF2_MRxCF_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_BA_e,   12 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_BA_i,   IBM_BIT_INDEX(32, PLB6MCIF2_MRxCF_M_BA_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_BA_n,   13 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_SZ_e,   19 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_SZ_i,   IBM_BIT_INDEX(32, PLB6MCIF2_MRxCF_M_SZ_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_SZ_n,   4 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_RE_e,   31 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_RE_i,   IBM_BIT_INDEX(32, PLB6MCIF2_MRxCF_M_RE_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_RE_n,   1 )
END_ENUM( PLB6MCIF2_MRxCF_FIELD )

BEGIN_ENUM( PLB6MCIF2_MEMRANK_SZ )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_8MB,    0x0 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_16MB,   0x1 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_32MB,   0x2 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_64MB,   0x3 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_128MB,  0x4 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_256MB,  0x5 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_512MB,  0x6 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_1GB,    0x7 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_2GB,    0x8 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_4GB,    0x9 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_8GB,    0xa )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_16GB,   0xb )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_32GB,   0xc )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_64GB,   0xd )
END_ENUM( PLB6MCIF2_MEMRANK_SZ )


BEGIN_ENUM( PLB6MCIF2_SRxCF_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_BA_e,   12 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_BA_i,   IBM_BIT_INDEX(32, PLB6MCIF2_SRxCF_S_BA_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_BA_n,   13 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_SZ_e,   19 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_SZ_i,   IBM_BIT_INDEX(32, PLB6MCIF2_SRxCF_S_SZ_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_SZ_n,   4 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_RE_e,   31 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_RE_i,   IBM_BIT_INDEX(32, PLB6MCIF2_SRxCF_S_RE_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_RE_n,   1 )
END_ENUM( PLB6MCIF2_SRxCF_FIELD )


#define PLB6MCIF2_MASTER_N                  16
DECLARE_CONST( PLB6MCIF2_P6BMTAGx_MstxID_n, 5 )

BEGIN_ENUM( PLB6MCIF2_P6BMTAG1_FIELD )
#define PLB6MCIF2_P6BMTAG1_MSTxID_E( i )\
DECLARE_ENUM_VAL( CAT(CAT( PLB6MCIF2_P6BMTAG1_Mst,i),ID_e ),  4 + (5 * i) )
PLB6MCIF2_P6BMTAG1_MSTxID_E (0)
PLB6MCIF2_P6BMTAG1_MSTxID_E (1)
PLB6MCIF2_P6BMTAG1_MSTxID_E (2)
PLB6MCIF2_P6BMTAG1_MSTxID_E (3)
PLB6MCIF2_P6BMTAG1_MSTxID_E (4)
PLB6MCIF2_P6BMTAG1_MSTxID_E (5)
#define PLB6MCIF2_P6BMTAG1_MSTxID_I( i )\
DECLARE_ENUM_VAL( CAT(CAT( PLB6MCIF2_P6BMTAG1_Mst,i),ID_i ),  IBM_BIT_INDEX(32, CAT(CAT( PLB6MCIF2_P6BMTAG1_Mst,i),ID_e )) )
PLB6MCIF2_P6BMTAG1_MSTxID_I (0)
PLB6MCIF2_P6BMTAG1_MSTxID_I (1)
PLB6MCIF2_P6BMTAG1_MSTxID_I (2)
PLB6MCIF2_P6BMTAG1_MSTxID_I (3)
PLB6MCIF2_P6BMTAG1_MSTxID_I (4)
PLB6MCIF2_P6BMTAG1_MSTxID_I (5)
END_ENUM( PLB6MCIF2_P6BMTAG1_FIELD )

BEGIN_ENUM( PLB6MCIF2_P6BMTAG2_FIELD )
#define PLB6MCIF2_P6BMTAG2_MSTxID_E( i )\
DECLARE_ENUM_VAL( CAT(CAT( PLB6MCIF2_P6BMTAG2_Mst,i),ID_e ),  4 + (5 * (i-6)) )
PLB6MCIF2_P6BMTAG2_MSTxID_E( 6 )
PLB6MCIF2_P6BMTAG2_MSTxID_E( 7 )
PLB6MCIF2_P6BMTAG2_MSTxID_E( 8 )
PLB6MCIF2_P6BMTAG2_MSTxID_E( 9 )
PLB6MCIF2_P6BMTAG2_MSTxID_E( 10 )
PLB6MCIF2_P6BMTAG2_MSTxID_E( 11 )
#define PLB6MCIF2_P6BMTAG2_MSTxID_I( i )\
DECLARE_ENUM_VAL( CAT(CAT( PLB6MCIF2_P6BMTAG2_Mst,i),ID_i ),  IBM_BIT_INDEX(32, CAT(CAT( PLB6MCIF2_P6BMTAG2_Mst,i),ID_e )) )
PLB6MCIF2_P6BMTAG2_MSTxID_I( 6 )
PLB6MCIF2_P6BMTAG2_MSTxID_I( 7 )
PLB6MCIF2_P6BMTAG2_MSTxID_I( 8 )
PLB6MCIF2_P6BMTAG2_MSTxID_I( 9 )
PLB6MCIF2_P6BMTAG2_MSTxID_I( 10 )
PLB6MCIF2_P6BMTAG2_MSTxID_I( 11 )
END_ENUM( PLB6MCIF2_P6BMTAG2_FIELD )

BEGIN_ENUM( PLB6MCIF2_P6BMTAG3_FIELD )
#define PLB6MCIF2_P6BMTAG3_MSTxID_E( i )\
DECLARE_ENUM_VAL( CAT(CAT( PLB6MCIF2_P6BMTAG3_Mst,i),ID_e ),  4 + (5 * (i-12)) )
PLB6MCIF2_P6BMTAG3_MSTxID_E( 12 )
PLB6MCIF2_P6BMTAG3_MSTxID_E( 13 )
PLB6MCIF2_P6BMTAG3_MSTxID_E( 14 )
PLB6MCIF2_P6BMTAG3_MSTxID_E( 15 )
#define PLB6MCIF2_P6BMTAG3_MSTxID_I( i )\
DECLARE_ENUM_VAL( CAT(CAT( PLB6MCIF2_P6BMTAG3_Mst,i),ID_i ),  IBM_BIT_INDEX(32, CAT(CAT( PLB6MCIF2_P6BMTAG3_Mst,i),ID_e )) )
PLB6MCIF2_P6BMTAG3_MSTxID_I( 12 )
PLB6MCIF2_P6BMTAG3_MSTxID_I( 13 )
PLB6MCIF2_P6BMTAG3_MSTxID_I( 14 )
PLB6MCIF2_P6BMTAG3_MSTxID_I( 15 )
END_ENUM( PLB6MCIF2_P6BMTAG3_FIELD )

BEGIN_ENUM( PLB6MCIF2_MAPxCF_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDWrAcc_e, 15 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDWrAcc_i, IBM_BIT_INDEX(32, PLB6MCIF2_MAPxCF_MstIDWrAcc_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDWrAcc_n, 16 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDRdAcc_e, 31 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDRdAcc_i, IBM_BIT_INDEX(32, PLB6MCIF2_MAPxCF_MstIDRdAcc_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDRdAcc_n, 16 )
END_ENUM( PLB6MCIF2_MAPxCF_FIELD )

BEGIN_ENUM( PLB6MCIF2_STATUS_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS_IDLE_e,  1 )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS_IDLE_i,  IBM_BIT_INDEX(32, PLB6MCIF2_STATUS_IDLE_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS_IDLE_n,  1 )
END_ENUM( PLB6MCIF2_STATUS_FIELD )

BEGIN_ENUM( PLB6MCIF2_RID_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_RID_e,  31 )
DECLARE_ENUM_VAL( PLB6MCIF2_RID_i,  IBM_BIT_INDEX(32, PLB6MCIF2_RID_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_RID_n,  32 )
END_ENUM( PLB6MCIF2_RID_FIELD )


typedef enum PLB6MCIF2_CLK_RELATION
{
    PLB6MCIF2_PLB_EQ_MCIF,  // PLB6 clock period equal to MCIF clock period within +/-5%
    PLB6MCIF2_PLB_LT_MCIF,  // PLB6 clock period is less than 0.95 MCIF clock period
    PLB6MCIF2_PLB_GT_MCIF,  // PLB6 clock period is greater than 1.05 MCIF clock period
} PLB6MCIF2_CLK_RELATION;


typedef struct plb6mcif2_errors
{
    PLB6MCIF2_ERROR_TYPE1   MxET1:     PLB6MCIF2_BESR_MxET1_0_n+PLB6MCIF2_BESR_MxET1_1_5_n;
    uint8_t                 MxET2:     PLB6MCIF2_BESR_MxET2_n;
    PLB6MCIF2_ERROR_TYPE3   MxET3:     PLB6MCIF2_BESR_MxET3_n;
    uint8_t                 RS_Err:    PLB6MCIF2_BESR_RS_Err_1_2_n;
} plb6mcif2_errors;


typedef struct plb6mcif2_master_map
{
    uint8_t id: PLB6MCIF2_P6BMTAGx_MstxID_n;
} plb6mcif2_master_map;


typedef struct plb6mcif2_memrank_cfg
{
    uint32_t                base_addr:  PLB6MCIF2_MRxCF_M_BA_n;
    PLB6MCIF2_MEMRANK_SZ    size:       PLB6MCIF2_MRxCF_M_SZ_n;
    bool                    enabled;
    struct
    {
        uint32_t            write:      PLB6MCIF2_MAPxCF_MstIDWrAcc_n;
        uint32_t            read:       PLB6MCIF2_MAPxCF_MstIDRdAcc_n;
    } master_acc;
} plb6mcif2_memrank_cfg;


typedef struct plb6mcif2_cfg
{
    bool                            enabled;
    struct
    {
        PLB6MCIF2_CLK_RELATION      relation;
        uint32_t                    plb6_cycles_ovrd:       PLB6MCIF2_PLBASYNC_PLB6NUM_n;
        uint32_t                    mcif_cycles_ovrd:       PLB6MCIF2_PLBASYNC_MCIFNUM_n;
//        uint32_t                    wrdone_cycles_ovrd:     PLB6MCIF2_PLBASYNC_WRDONE_TUNE_n;
//        uint32_t                    wrrdy_cycles_ovrd:      PLB6MCIF2_PLBASYNC_WRRDY_TUNE_n;
        uint32_t                    rdrdy_cycles_ovrd:      PLB6MCIF2_PLBASYNC_RDRDY_TUNE_n;
        PLB6MCIF2_CLK_GATING        gating:                 PLB6MCIF2_PLBCFG_CG_ENABLE_n;
    } clk;
    bool                            non_coherent;
    PLB6MCIF2_TSNOOP                tsnoop:                 PLB6MCIF2_PLBCFG_Tsnoop_n;
    uint8_t                         paryties:               PLB6MCIF2_PLBCFG_Parity_Enables_n;
    struct
    {
        plb6mcif2_errors            logs;
        plb6mcif2_errors            interrupts;
    } errors;
    struct
    {
        bool                        enabled;
        PLB6MCIF2_PLB_RAW_THRESHOLD threshold:              PLB6MCIF2_PLBORD_Threshold_n;
    } read_around_write;
    bool                            intra_thread_out_of_order_rd_ret_enabled;
    PLB6MCIF2_MCIF2_TOM             transaction_order_mode: PLB6MCIF2_PLBORD_TOM_n;
    PLB6MCIF2_PLB_MAXMEM            max_mem:                PLB6MCIF2_MAXMEM_n;
    uint32_t                        upper_addr_base_addr:   PLB6MCIF2_PUABA_n;
    bool                            master_acc_check_enabled;
    plb6mcif2_master_map            master[PLB6MCIF2_MASTER_N];
    plb6mcif2_memrank_cfg           mem_rank[PLB6MCIF2_MEMRANK_N];
} plb6mcif2_cfg;



void plb6mcif2_init( uint32_t const base_addr, plb6mcif2_cfg const *const cfg ) {
    register uint32_t reg_value;

    assert( cfg != 0);

    reg_value = plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr );
    assert( !(reg_value & (1 << PLB6MCIF2_PLBCFG_Br_Enable_i)));

    switch( cfg->clk.relation )
    {
        case PLB6MCIF2_PLB_EQ_MCIF: {
            reg_value = (1 << PLB6MCIF2_PLBASYNC_PLBeqMCIF_i) | (0 << PLB6MCIF2_PLBASYNC_PLBgtMCIF_i);
            break;
        }
        case PLB6MCIF2_PLB_LT_MCIF: {
            reg_value = (0 << PLB6MCIF2_PLBASYNC_PLBeqMCIF_i) | (0 << PLB6MCIF2_PLBASYNC_PLBgtMCIF_i);
            break;
        }
        case PLB6MCIF2_PLB_GT_MCIF: {
            reg_value = (0 << PLB6MCIF2_PLBASYNC_PLBeqMCIF_i) | (1 << PLB6MCIF2_PLBASYNC_PLBgtMCIF_i);
            break;
        }
        default: assert( false );
    }
    reg_value   |=  (cfg->clk.plb6_cycles_ovrd << PLB6MCIF2_PLBASYNC_PLB6NUM_i)
                |   (cfg->clk.mcif_cycles_ovrd << PLB6MCIF2_PLBASYNC_MCIFNUM_i)
//                |   (cfg->clk.wrdone_cycles_ovrd << PLB6MCIF2_PLBASYNC_WRDONE_TUNE_i)
//                |   (cfg->clk.wrrdy_cycles_ovrd << PLB6MCIF2_PLBASYNC_WRRDY_TUNE_i)
                |   (cfg->clk.rdrdy_cycles_ovrd << PLB6MCIF2_PLBASYNC_RDRDY_TUNE_i );
    plb6mcif2_dcr_write_PLB6MCIF2_PLBASYNC( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_PLBASYNC( base_addr ) == reg_value);

    reg_value   = (cfg->read_around_write.enabled << PLB6MCIF2_PLBORD_RaW_i)
                | (GET_BITS( cfg->read_around_write.threshold,  PLB6MCIF2_PLBORD_Threshold_1_3_n,   PLB6MCIF2_PLBORD_Threshold_0_n )    << PLB6MCIF2_PLBORD_Threshold_0_i)
                | (GET_BITS( cfg->read_around_write.threshold,  0,                                  PLB6MCIF2_PLBORD_Threshold_1_3_n )  << PLB6MCIF2_PLBORD_Threshold_1_3_i)
                | (cfg->intra_thread_out_of_order_rd_ret_enabled << PLB6MCIF2_PLBORD_IT_OOO_i)
                | (cfg->transaction_order_mode << PLB6MCIF2_PLBORD_TOM_i);
    plb6mcif2_dcr_write_PLB6MCIF2_PLBORD( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_PLBORD( base_addr ) == reg_value);

    switch( cfg->max_mem ) {
        case PLB6MCIF2_MAXMEM_64GB: break;
        case PLB6MCIF2_MAXMEM_32GB: {
            assert(    GET_BITS( cfg->mem_rank[0].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  0),     1 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       28),    1 ) );
            assert(    GET_BITS( cfg->mem_rank[1].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  0),     1 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       28),    1 ) );
            assert(    GET_BITS( cfg->mem_rank[2].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  0),     1 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       28),    1 ) );
            assert(    GET_BITS( cfg->mem_rank[3].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  0),     1 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       28),    1 ) );
            break;
        }
        case PLB6MCIF2_MAXMEM_16GB: {
            assert(    GET_BITS( cfg->mem_rank[0].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  1),     2 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       29),    2 ) );
            assert(    GET_BITS( cfg->mem_rank[1].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  1),     2 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       29),    2 ) );
            assert(    GET_BITS( cfg->mem_rank[2].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  1),     2 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       29),    2 ) );
            assert(    GET_BITS( cfg->mem_rank[3].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  1),     2 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       29),    2 ) );
            break;
        }
        case PLB6MCIF2_MAXMEM_8GB: {
            assert(    GET_BITS( cfg->mem_rank[0].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  2),     3 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       30),    3 ) );
            assert(    GET_BITS( cfg->mem_rank[1].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  2),     3 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       30),    3 ) );
            assert(    GET_BITS( cfg->mem_rank[2].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  2),     3 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       30),    3 ) );
            assert(    GET_BITS( cfg->mem_rank[3].base_addr,   IBM_BIT_INDEX( PLB6MCIF2_MRxCF_M_BA_n,  2),     3 )
                        ==  GET_BITS( cfg->upper_addr_base_addr,    IBM_BIT_INDEX( PLB6MCIF2_PUABA_n,       30),    3 ) );
            break;
        }
    }
    reg_value   = (cfg->max_mem << PLB6MCIF2_MAXMEM_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MAXMEM( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_MAXMEM( base_addr ) == reg_value );

    reg_value   = (cfg->upper_addr_base_addr << PLB6MCIF2_PUABA_i);
    plb6mcif2_dcr_write_PLB6MCIF2_PUABA( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_PUABA( base_addr ) == reg_value );

    reg_value   = (cfg->master[0].id << PLB6MCIF2_P6BMTAG1_Mst0ID_i)
                | (cfg->master[1].id << PLB6MCIF2_P6BMTAG1_Mst1ID_i)
                | (cfg->master[2].id << PLB6MCIF2_P6BMTAG1_Mst2ID_i)
                | (cfg->master[3].id << PLB6MCIF2_P6BMTAG1_Mst3ID_i)
                | (cfg->master[4].id << PLB6MCIF2_P6BMTAG1_Mst4ID_i)
                | (cfg->master[5].id << PLB6MCIF2_P6BMTAG1_Mst5ID_i);
    plb6mcif2_dcr_write_PLB6MCIF2_P6BMTAG1( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_P6BMTAG1( base_addr ) == reg_value );

    reg_value   = (cfg->master[6].id << PLB6MCIF2_P6BMTAG2_Mst6ID_i)
                | (cfg->master[7].id << PLB6MCIF2_P6BMTAG2_Mst7ID_i)
                | (cfg->master[8].id << PLB6MCIF2_P6BMTAG2_Mst8ID_i)
                | (cfg->master[9].id << PLB6MCIF2_P6BMTAG2_Mst9ID_i)
                | (cfg->master[10].id << PLB6MCIF2_P6BMTAG2_Mst10ID_i)
                | (cfg->master[11].id << PLB6MCIF2_P6BMTAG2_Mst11ID_i);
    plb6mcif2_dcr_write_PLB6MCIF2_P6BMTAG2( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_P6BMTAG2( base_addr ) == reg_value );

    reg_value   = (cfg->master[12].id << PLB6MCIF2_P6BMTAG3_Mst12ID_i)
                | (cfg->master[13].id << PLB6MCIF2_P6BMTAG3_Mst13ID_i)
                | (cfg->master[14].id << PLB6MCIF2_P6BMTAG3_Mst14ID_i)
                | (cfg->master[15].id << PLB6MCIF2_P6BMTAG3_Mst15ID_i);
    plb6mcif2_dcr_write_PLB6MCIF2_P6BMTAG3( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_P6BMTAG3( base_addr ) == reg_value );

    reg_value   = (cfg->mem_rank[0].master_acc.write << PLB6MCIF2_MAPxCF_MstIDWrAcc_i)
                | (cfg->mem_rank[0].master_acc.read << PLB6MCIF2_MAPxCF_MstIDRdAcc_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MAP0CF( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_MAP0CF( base_addr ) == reg_value );

    reg_value   = (cfg->mem_rank[1].master_acc.write << PLB6MCIF2_MAPxCF_MstIDWrAcc_i)
                | (cfg->mem_rank[1].master_acc.read << PLB6MCIF2_MAPxCF_MstIDRdAcc_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MAP1CF( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_MAP1CF( base_addr ) == reg_value );

    reg_value   = (cfg->mem_rank[2].master_acc.write << PLB6MCIF2_MAPxCF_MstIDWrAcc_i)
                | (cfg->mem_rank[2].master_acc.read << PLB6MCIF2_MAPxCF_MstIDRdAcc_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MAP2CF( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_MAP2CF( base_addr ) == reg_value );

    reg_value   = (cfg->mem_rank[3].master_acc.write << PLB6MCIF2_MAPxCF_MstIDWrAcc_i)
                | (cfg->mem_rank[3].master_acc.read << PLB6MCIF2_MAPxCF_MstIDRdAcc_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MAP3CF( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_MAP3CF( base_addr ) == reg_value );

    reg_value   = (cfg->mem_rank[0].base_addr << PLB6MCIF2_MRxCF_M_BA_i)
                | (cfg->mem_rank[0].size << PLB6MCIF2_MRxCF_M_SZ_i)
                | (cfg->mem_rank[0].enabled << PLB6MCIF2_MRxCF_M_RE_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MR0CF( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_MR0CF( base_addr ) == reg_value );

    reg_value   = (cfg->mem_rank[1].base_addr << PLB6MCIF2_MRxCF_M_BA_i)
                | (cfg->mem_rank[1].size << PLB6MCIF2_MRxCF_M_SZ_i)
                | (cfg->mem_rank[1].enabled << PLB6MCIF2_MRxCF_M_RE_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MR1CF( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_MR1CF( base_addr ) == reg_value );

    reg_value   = (cfg->mem_rank[2].base_addr << PLB6MCIF2_MRxCF_M_BA_i)
                | (cfg->mem_rank[2].size << PLB6MCIF2_MRxCF_M_SZ_i)
                | (cfg->mem_rank[2].enabled << PLB6MCIF2_MRxCF_M_RE_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MR2CF( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_MR2CF( base_addr ) == reg_value );

    reg_value   = (cfg->mem_rank[3].base_addr << PLB6MCIF2_MRxCF_M_BA_i)
                | (cfg->mem_rank[3].size << PLB6MCIF2_MRxCF_M_SZ_i)
                | (cfg->mem_rank[3].enabled << PLB6MCIF2_MRxCF_M_RE_i);
    plb6mcif2_dcr_write_PLB6MCIF2_MR3CF( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_MR3CF( base_addr ) == reg_value );

    reg_value   = (GET_BITS( cfg->errors.logs.MxET1,    PLB6MCIF2_BESR_MxET1_1_5_n, PLB6MCIF2_BESR_MxET1_0_n )      << (PLB6MCIF2_BESR_MxET1_0_i+PLB6MCIF2_INTR_EN_ERR_EN_i))
                | (GET_BITS( cfg->errors.logs.MxET1,    0,                          PLB6MCIF2_BESR_MxET1_1_5_n )    << (PLB6MCIF2_BESR_MxET1_1_5_i+PLB6MCIF2_INTR_EN_ERR_EN_i))
                | (cfg->errors.logs.MxET2 << (PLB6MCIF2_BESR_MxET2_i+PLB6MCIF2_INTR_EN_ERR_EN_i))
                | (cfg->errors.logs.MxET3 << (PLB6MCIF2_BESR_MxET3_i+PLB6MCIF2_INTR_EN_ERR_EN_i))
                | (cfg->errors.logs.RS_Err << (PLB6MCIF2_BESR_RS_Err_1_2_i+PLB6MCIF2_INTR_EN_ERR_EN_i))
                | (GET_BITS( cfg->errors.interrupts.MxET1,  PLB6MCIF2_BESR_MxET1_1_5_n, PLB6MCIF2_BESR_MxET1_0_n )      << (PLB6MCIF2_BESR_MxET1_0_i+PLB6MCIF2_INTR_EN_INTR_EN_i))
                | (GET_BITS( cfg->errors.interrupts.MxET1,  0,                          PLB6MCIF2_BESR_MxET1_1_5_n )    << (PLB6MCIF2_BESR_MxET1_1_5_i+PLB6MCIF2_INTR_EN_INTR_EN_i))
                | (cfg->errors.interrupts.MxET2 << (PLB6MCIF2_BESR_MxET2_i+PLB6MCIF2_INTR_EN_INTR_EN_i))
                | (cfg->errors.interrupts.MxET3 << (PLB6MCIF2_BESR_MxET3_i+PLB6MCIF2_INTR_EN_INTR_EN_i))
                | (cfg->errors.interrupts.RS_Err << (PLB6MCIF2_BESR_RS_Err_1_2_i+PLB6MCIF2_INTR_EN_INTR_EN_i));
    plb6mcif2_dcr_write_PLB6MCIF2_INTR_EN( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_INTR_EN( base_addr ) == reg_value );

    reg_value   = (cfg->non_coherent << PLB6MCIF2_PLBCFG_Non_Coherent_i)
                | (cfg->tsnoop << PLB6MCIF2_PLBCFG_Tsnoop_i)
                | (cfg->master_acc_check_enabled << PLB6MCIF2_PLBCFG_PmstrID_chk_EN_i)
                | (cfg->paryties << PLB6MCIF2_PLBCFG_Parity_Enables_i)
                | (cfg->clk.gating << PLB6MCIF2_PLBCFG_CG_ENABLE_i)
                | (cfg->enabled << PLB6MCIF2_PLBCFG_Br_Enable_i);
    plb6mcif2_dcr_write_PLB6MCIF2_PLBCFG( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr ) == reg_value );
}

void plb6mcif2_simple_init( uint32_t base_addr, const uint32_t puaba )
{
    plb6mcif2_dcr_write_PLB6MCIF2_INTR_EN(base_addr,
            //enable logging but disable interrupts
              reg_field(15, 0xFFFF)
            | reg_field(31, 0x0000));

    plb6mcif2_dcr_write_PLB6MCIF2_PLBASYNC(base_addr,
            reg_field(1, 0b1)); //PLB clock equals MCIF2 clock

    plb6mcif2_dcr_write_PLB6MCIF2_PLBORD(base_addr,
              reg_field(0, 0b0)  //Raw
            | reg_field(3, 0b010) //Threshold[1:3]
            | reg_field(7, 0b0)   //Threshold[0]
            | reg_field(16, 0b0) //IT_OOO
            | reg_field(18, 0b00) //TOM
            );

    plb6mcif2_dcr_write_PLB6MCIF2_MAXMEM(base_addr,
            reg_field(1, 0b00)); //Set MAXMEM = 8GB

    plb6mcif2_dcr_write_PLB6MCIF2_PUABA(base_addr,
            puaba); //Set PLB upper address base address from I_S_ADDR[0:30]

    plb6mcif2_dcr_write_PLB6MCIF2_MR0CF(base_addr,
            //Set Rank0 base addr
            //M_BA[0:2] = PUABA[28:30]
            //M_BA[3:12] = 0b0000000000
              reg_field(3, (puaba & 0b1110)) | reg_field(12, 0b0000000000)
            | reg_field(19, 0b1001) //Set Rank0 size = 4GB
            | reg_field(31, 0b1)); //Enable Rank0

    plb6mcif2_dcr_write_PLB6MCIF2_MR1CF(base_addr,
            //Set Rank1 base addr
            //M_BA[0:2] = PUABA[28:30]
            //M_BA[3:12] = 0b1000000000
            reg_field(3, (puaba & 0b1110)) | reg_field(12, 0b1000000000)
          | reg_field(19, 0b1001) //Set Rank1 size = 4GB
          | reg_field(31, 0b1)); //Enable Rank1

    plb6mcif2_dcr_write_PLB6MCIF2_MR2CF(base_addr,
          reg_field(31, 0b0)); //Disable Rank2

    plb6mcif2_dcr_write_PLB6MCIF2_MR3CF(base_addr,
            reg_field(31, 0b0)); //Disable Rank3

    const uint32_t Tsnoop = (plb6bc_dcr_read_PLB6BC_TSNOOP(DCR_PLB6_BC_BASE) & (0b1111 << 28)) >> 28;

    plb6mcif2_dcr_write_PLB6MCIF2_PLBCFG(base_addr,
            //PLB6MCIF2 enable [31] = 0b1
              reg_field(0, 0b0) //Non-coherent
            | reg_field(4, Tsnoop) //T-snoop
            | reg_field(5, 0b0) //PmstrID_chk_EN
            | reg_field(27, 0b1111) //Parity enables
            | reg_field(30, 0b000) //CG_Enable
            | reg_field(31, 0b1)); //BR_Enable

}

void plb6mcif2_get_cfg( uint32_t const base_addr, plb6mcif2_cfg *const cfg ) {
    assert( cfg != 0 );

    assert( false );
}

void plb6mcif2_enable( uint32_t const base_addr ) {
    register uint32_t reg_value = plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr );
    SET_BIT( reg_value, PLB6MCIF2_PLBCFG_Br_Enable_i );
    plb6mcif2_dcr_write_PLB6MCIF2_PLBCFG( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr ) == reg_value );
}

void plb6mcif2_disable( uint32_t const base_addr ) {
    register uint32_t reg_value = plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr );
    CLEAR_BIT( reg_value, PLB6MCIF2_PLBCFG_Br_Enable_i );
    plb6mcif2_dcr_write_PLB6MCIF2_PLBCFG( base_addr, reg_value );
    assert( plb6mcif2_dcr_read_PLB6MCIF2_PLBCFG( base_addr ) == reg_value );
}
