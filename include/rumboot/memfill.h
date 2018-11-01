
int rumboot_memfill32(void *addr, size_t sz, int val, int incr);
int rumboot_memcheck32(void *src, void *dst, size_t sz);
/**
 * Returns value like memcmp, comparing bytewise memory
 * regions from addresses src and dst of size sz.
 * Unlike memcmp it compares everything up to the end of
 * specified memory regions, provides tracing and
 * can be accelerated in SystemVerilog while modelling
 *
 * @param  src  first memory area address
 * @param  dst  second memory area address
 * @param  sz   count of bytes to be compared
 * @return      like in memcmp
 */
int rumboot_memcmp(void *src, void *dst, size_t sz);

