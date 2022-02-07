#ifndef RUMBOOT_H
#define RUMBOOT_H

#include <stdbool.h>


/**
 *
 * \defgroup malloc Multi-heap memory allocation API
 *
 * This API can be used to allocate memory linearly from various available banks.
 *
 * This memory allocator is really DUMB:
 * - It cares about managing memory in several heaps
 * - It does the job to provide proper (mis)alignment
 * - It DOES NOT really free up memory (To speed up simulation)
 *
 * If proper malloc/free required - use malloc/free from libc.
 *
 * The platform setup code uses rumboot_malloc_heap_register to register a set
 * of memory heaps. The first heap registered will be the default one.
 *
 *
 * \ingroup libraries
 *
 * @{
 */

/**
 * Used by platform setup code to register a heap in the malloc subsystem
 *
 * @param  name
 * @param  heap_start
 * @param  heap_end
 * @return The heap id that was just registered
 */
int rumboot_malloc_register_heap(const char *name, void *heap_start, void *heap_end);



/**
 * Allocates length bytes from heap identified by heap_id
 *
 * This function will cause a panic if allocation fails
 *
 * @param  heap_id heap id
 * @param  length  length
 * @return         pointer to data area
 */
void *rumboot_malloc_from_heap(int heap_id, size_t length);


/**
 * Allocates length bytes from heap identified by heap_id. The buffer will be aligned.
 *
 * This function will cause a panic if allocation fails
 * @param  heap_id heap id
 * @param  length  length
 * @param  align   alignment
 */
void *rumboot_malloc_from_heap_aligned(int heap_id, size_t sz, int align);


/**
 * Allocates a misaligned buffer. The buffer is first aligned according to align, then
 * misalign bytes are added to it.
 *
 * This function will cause a panic if allocation fails
 *
 * @param  heap_id  [description]
 * @param  length   [description]
 * @param  align    [description]
 * @param  misalign [description]
 * @return          [description]
 */
void *rumboot_malloc_from_heap_misaligned(int heap_id, size_t length, int align, int misalign);

void rumboot_malloc_update_heaps(bool save);

/**
 * Allocates memory from default heap
 *
 * @param  length [description]
 * @return        [description]
 */
void *rumboot_malloc(uint32_t length);


/**
 * Duplicates a region of memory.
 * The memory is taken from default heap.
 *
 * @param  ptr [description]
 * @param  len [description]
 * @return     [description]
 */
void *rumboot_memdup(const void *ptr, uint32_t len);


/**
 * Frees memory. (Not really)
 *
 * @param ptr [description]
 */
void rumboot_free(void *ptr);


/**
 * Find heap id by name.
 * @param  name heap name
 * @return  heap id or -1 if it couldn't be found. 0 is a valid heap id.
 */
int rumboot_malloc_heap_by_name(const char *name);

/**
 * Find out what heap is this address from
 * 
 * @param addr 
 * @return int heap id or -1 if this memory is not from any heap
 */
int rumboot_malloc_heap_by_addr(uintptr_t addr);

/**
 * Get heap name by it's numeric id
 *
 * @param  heap_id heap id
 * @return   A const char* with heap name
 */
const char *rumboot_malloc_heap_name(int heap_id);


/**
 * Get the total count of heaps registered on this platform/configuration
 * @return
 */
int rumboot_malloc_num_heaps();


/**
 * Get the total size of a heap identified by heap_id
 * @param  heap_id
 * @return         heap size in bytes
 */
uint32_t rumboot_malloc_heap_length(int heap_id);


void *rumboot_malloc_from_named_heap_misaligned(const char *heap_name, size_t length, int align, int misalign);
void *rumboot_malloc_from_named_heap(const char *heap_name, size_t length);
void *rumboot_malloc_from_named_heap_aligned(const char *heap_name, size_t sz, int align);

/**
* @}
*/

#endif /* end of include guard: RUMBOOT_H */
