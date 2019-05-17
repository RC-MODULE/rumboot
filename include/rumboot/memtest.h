#ifndef MEMTEST_H
#define MEMTEST_H

#include <stdbool.h>
/**
 * @brief Tests a chunk of memory once with a specified pattern
 * 
 * @param pattern 32-bit pattern to write & verify
 * @param start_phys Starting address
 * @param size Size of chunk to test
 * @return uint64_t Number of sigle bit errors during readback
 */
uint64_t memtest_once(uint32_t pattern, uintptr_t start_phys, uintptr_t size);

/**
 * @brief Tests a chunk of memory with a set predefined patterns
 * 
 * @param start_phys 
 * @param size 
 * @return uint64_t Number of sigle bit errors during readback
 */
uint64_t memtest(uintptr_t start_phys, uintptr_t size);

/**
 * @brief Check a pattern in memory, optionally refresh on error
 * 
 * @param pattern 32-bit pattern to check
 * @param refresh If true - refresh when when a read error is detected
 * @param mem_start starting memory address
 * @param length length of memory chunk to check (bytes)
 * @return uint64_t Number of sigle bit errors during readback
 */
uint64_t memtest_check_pattern(uint32_t pattern, bool refresh, uintptr_t mem_start, size_t length);

/**
 * @brief Fill memory with a user-defined pattern
 * 
 * @param mem Starting memory address
 * @param length Length of memory chunk (in bytes)
 * @param pattern Pattern to use
 */
void memtest_fill_pattern(uintptr_t mem, size_t length, uint32_t pattern);


#endif