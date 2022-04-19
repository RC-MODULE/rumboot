#ifndef REMOTE_ARCH_IO_H
#define REMOTE_ARCH_IO_H

#define RUMBOOT_ARCH_HAS_IO

#include <rumboot/platform.h>

static __attribute__((no_instrument_function)) inline double ioread64d(uint32_t const base_addr)
{
    rumboot_platform_panic("Unsupported io access function: %s\n", __FUNCTION__);
    return  0;
}

static __attribute__((no_instrument_function)) inline void iowrite64d(double const value, uint32_t const base_addr)
{
    rumboot_platform_panic("Unsupported io access function: %s\n", __FUNCTION__);
}

static __attribute__((no_instrument_function)) inline uint64_t ioread64(uint32_t const base_addr)
{
    rumboot_platform_panic("Unsupported io access function: %s\n", __FUNCTION__);
    return 0;

}

static __attribute__((no_instrument_function)) inline void iowrite64(uint64_t const value, uint32_t const base_addr)
{
    rumboot_platform_panic("Unsupported io access function: %s\n", __FUNCTION__);

}

static __attribute__((no_instrument_function)) inline uint32_t ioread32(uint32_t const base_addr)
{
    rumboot_platform_panic("Unsupported io access function: %s\n", __FUNCTION__);
    return 0;

}

static __attribute__((no_instrument_function)) inline void iowrite32(uint32_t const value, uint32_t const base_addr)
{
    rumboot_platform_panic("Unsupported io access function: %s\n", __FUNCTION__);
}

static __attribute__((no_instrument_function)) inline uint16_t ioread16(uint32_t const base_addr)
{
    rumboot_platform_panic("Unsupported io access function: %s\n", __FUNCTION__);
    return 0;
}

static __attribute__((no_instrument_function)) inline void iowrite16(uint16_t const value, uint32_t const base_addr)
{
    rumboot_platform_panic("Unsupported io access function: %s\n", __FUNCTION__);
}

static __attribute__((no_instrument_function)) inline uint8_t ioread8(uint32_t const base_addr)
{
    rumboot_platform_panic("Unsupported io access function: %s\n", __FUNCTION__);
    return 0;
}

static __attribute__((no_instrument_function)) inline void iowrite8(uint8_t const value, uint32_t const base_addr)
{
    rumboot_platform_panic("Unsupported io access function: %s\n", __FUNCTION__);
}



#endif /* NMC3_ARCH_IO_H */
