#ifndef REMOTE_ARCH_IO_H
#define REMOTE_ARCH_IO_H

#define RUMBOOT_ARCH_HAS_IO

#include <rumboot/platform.h>
#include <platform/vl_api.h>

extern struct vl_instance *g_vl_instance; 

static __attribute__((no_instrument_function)) inline double ioread64d(uint32_t const base_addr)
{
    return (double) vl_ioread64(g_vl_instance, base_addr);
}

static __attribute__((no_instrument_function)) inline void iowrite64d(double const value, uint32_t const base_addr)
{
    vl_iowrite64(g_vl_instance, (double) value, base_addr);
}

static __attribute__((no_instrument_function)) inline uint64_t ioread64(uint32_t const base_addr)
{
    return vl_ioread64(g_vl_instance, base_addr);
}

static __attribute__((no_instrument_function)) inline void iowrite64(uint64_t const value, uint32_t const base_addr)
{
    vl_iowrite64(g_vl_instance, value, base_addr);
}

static __attribute__((no_instrument_function)) inline uint32_t ioread32(uint32_t const base_addr)
{
    return vl_ioread32(g_vl_instance, base_addr);
}

static __attribute__((no_instrument_function)) inline void iowrite32(uint32_t const value, uint32_t const base_addr)
{
    vl_iowrite32(g_vl_instance, value, base_addr);
}

static __attribute__((no_instrument_function)) inline uint16_t ioread16(uint32_t const base_addr)
{
    return vl_ioread16(g_vl_instance, base_addr);
}

static __attribute__((no_instrument_function)) inline void iowrite16(uint16_t const value, uint32_t const base_addr)
{
    vl_iowrite16(g_vl_instance, value, base_addr);
}

static __attribute__((no_instrument_function)) inline uint8_t ioread8(uint32_t const base_addr)
{
    return vl_ioread8(g_vl_instance, base_addr);
}

static __attribute__((no_instrument_function)) inline void iowrite8(uint8_t const value, uint32_t const base_addr)
{
    vl_iowrite8(g_vl_instance, value, base_addr);
}



#endif /* NMC3_ARCH_IO_H */
