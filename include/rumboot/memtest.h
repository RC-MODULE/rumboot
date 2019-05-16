#ifndef MEMTEST_H
#define MEMTEST_H

/**
 * @brief Tests a chunk of memory once with a specified pattern
 * 
 * @param pattern 32-bit pattern to write & verify
 * @param start_phys Starting address
 * @param size Size of chunk to test
 * @return uint32_t Number of words containing readback errors
 */
uint64_t memtest_once(uint32_t pattern, uintptr_t start_phys, uintptr_t size);

/**
 * @brief Tests a chunk of memory with a set predefined patterns
 * 
 * @param start_phys 
 * @param size 
 * @return uint32_t  Number of words containing readback errors
 */
uint64_t memtest(uintptr_t start_phys, uintptr_t size);


#endif