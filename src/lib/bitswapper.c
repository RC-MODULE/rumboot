#include <rumboot/bitswapper.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>



uint64_t bitswapper_transmute(struct bitswapper *swp, uint64_t data)
{
    uint64_t ret = 0;
    int i; 
    for (i=0; i < swp->num_bits; i++) {
        ret |= ((data & (1 << i)) ? 1 : 0) << swp->dst[i];
    }
    
    if (!swp->endian_swap) {
        return ret;
    }
    
    switch (swp->num_bits) {
        case 64: 
            ret = __swap64(ret);
        case 32:
            ret = __swap32(ret);
            break;
        case 16:
            ret = __swap16(ret);
            break;
    }
    return ret;
}

uint64_t bitswapper_transmute_reverse(struct bitswapper *swp, uint64_t data)
{
    uint64_t ret = 0;
    int i; 

    if (swp->endian_swap) {
        switch (swp->num_bits) {
            case 64: 
                data = __swap64(data);
            case 32:
                data = __swap32(data);
                break;
            case 16:
                data = __swap16(data);
                break;
        }
    }

    for (i=0; i < swp->num_bits; i++) {
        ret |= ((data & (1 << swp->dst[i])) ? 1 : 0) << i;
    }
    
    return ret;
}


void bitswapper_transmute_array(struct bitswapper *swp, void *data, size_t length)
{
    uint8_t  *ptr8  = data;
    uint16_t *ptr16 = data;
    uint32_t *ptr32 = data;
    uint64_t *ptr64 = data;
    size_t i;

    if (length % (swp->num_bits / 8)) {
        rumboot_platform_panic("bitswapper: Got non-power-of-two size %d for swapping", swp->num_bits);
    }

    for (i = 0; i < length; i = i + (swp->num_bits / 8)) {
        switch (swp->num_bits)
        {
            case  8:
                ptr8[i] = (uint8_t ) bitswapper_transmute(swp, (uint64_t) ptr8[i]);
            break;
            case  16:
                ptr16[i] = (uint16_t ) bitswapper_transmute(swp, (uint64_t) ptr16[i]);
            break;
            case  32:
                ptr32[i] = (uint32_t ) bitswapper_transmute(swp, (uint64_t) ptr32[i]);
            break;
            case  64:
                ptr64[i] = (uint64_t ) bitswapper_transmute(swp, (uint64_t) ptr64[i]);
            break;
        }
    }   
}