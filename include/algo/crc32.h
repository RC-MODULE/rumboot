#ifndef CRC32_H_
#define CRC32_H_

#include <stddef.h>
#include <stdint.h>

/**
 *
 * \defgroup algo Algorithms
 * \ingroup libraries
 *
 * \addtogroup algo
 *  @{
 */

/**
 * Calculates a CRC32 checksum of a buffer
 * @param  crc  Initial CRC32 value (use 0 for the first block)
 * @param  buf  The pointer the the buffer
 * @param  size The size of the buffer in byte
 * @return      A 32-bit cyclic redundancy check value
 */

uint32_t crc32(uint32_t crc, const void *buf, size_t size);

/** @}*/

#endif /* CRC32_H_ */
