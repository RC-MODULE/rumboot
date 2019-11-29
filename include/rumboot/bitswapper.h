#ifndef BITSWAPPER_H
#define BITSWAPPER_H

#include <stdint.h>
#include <unistd.h>

struct bitswapper {
    int num_bits;
    int endian_swap;
    uint8_t dst[64];
};


 
#define __swap16(value)                                 \
         ((((uint16_t)((value) & 0x00FF)) << 8) |        \
          (((uint16_t)((value) & 0xFF00)) >> 8))

#define __swap32(value)                                 \
        ((((uint32_t)((value) & 0x000000FF)) << 24) |   \
         (((uint32_t)((value) & 0x0000FF00)) << 8) |    \
         (((uint32_t)((value) & 0x00FF0000)) >> 8) |    \
         (((uint32_t)((value) & 0xFF000000)) >> 24))

#define __swap64(value)                                         \
        (((((uint64_t)value)<<56) & 0xFF00000000000000ULL)  |     \
         ((((uint64_t)value)<<40) & 0x00FF000000000000ULL)  |     \
         ((((uint64_t)value)<<24) & 0x0000FF0000000000ULL)  |     \
         ((((uint64_t)value)<< 8) & 0x000000FF00000000ULL)  |     \
         ((((uint64_t)value)>> 8) & 0x00000000FF000000ULL)  |     \
         ((((uint64_t)value)>>24) & 0x0000000000FF0000ULL)  |     \
         ((((uint64_t)value)>>40) & 0x000000000000FF00ULL)  |     \
         ((((uint64_t)value)>>56) & 0x00000000000000FFULL))


/**
 * @brief Swap bits in one word. 
 *
 * Returned value is casted as 64 bit. The actual bits swapped 
 * are defined in struct bitswapper
 *  
 * 
 * @param swp 
 * @param data 
 * @return uint64_t 
 */
uint64_t bitswapper_transmute(struct bitswapper *swp, uint64_t data);
uint64_t bitswapper_transmute_reverse(struct bitswapper *swp, uint64_t data);
/**
 * @brief Swap bits in an array of data. Data is interpreted as 
 * 8, 16, 32 or 64 bits as defined in the bitswapper struct
 * 
 * @param swp 
 * @param data 
 * @param length 
 */
void bitswapper_transmute_array(struct bitswapper *swp, void *data, size_t length);



#endif 