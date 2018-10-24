
#ifndef MIVEM_REGS_ACCESS_H_
#define MIVEM_REGS_ACCESS_H_

#include <stdint.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>

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


#endif /* MIVEM_REGS_ACCESS_H_ */
