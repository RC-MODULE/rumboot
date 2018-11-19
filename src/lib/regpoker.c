#include <stdint.h>
#include <stdlib.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>

#define READCHECK( tag, sz ) \
case tag: { \
    uint32_t const address = array->base + offset; \
    uint##sz##_t const v = ioread##sz(address); \
    /*rumboot_printf( "mask: 0x%x\n", (uint32_t)array->mask );*/ \
    if( (v & (uint##sz##_t)array->mask) != (uint##sz##_t)array->expected ) { \
        fails++; \
        rumboot_printf( "Read register %s (0x%x): got: 0x%x expected: %x mask: 0x%x\n", \
                                       array->name, \
                                             address, \
                                                         v, \
                                                                      (uint32_t)array->expected, \
                                                                                 (uint32_t)array->mask \
        ); \
    } \
    break; \
}

#define WRITECHECK( tag, sz ) \
case tag: { \
    int i; \
    uint32_t const address = array->base + offset; \
    /*rumboot_printf( "mask: 0x%x\n", (uint32_t)array->mask );*/ \
    if( (uint##sz##_t)array->mask != 0 ) { \
        for( i = 0; i < sz; i++ ) { \
            uint##sz## _t v, readback; \
            if( ( (v = (1 << i)) & (uint##sz##_t)array->mask ) != 0 ) { \
                iowrite##sz( v, address ); \
                readback = ioread##sz( address ); \
                if( ( readback & (uint##sz##_t)array->mask ) != v ) { \
                    fails++; \
                    rumboot_printf( "Write register %s (0x%x): wrote: 0x%x got: 0x%x mask: 0x%x\n", \
                                                    array->name, \
                                                          address, \
                                                                        v, \
                                                                                  readback, \
                                                                                             (uint32_t)array->mask \
                    ); \
                } \
            } \
        } \
    } \
    iowrite##sz( (uint##sz##_t)array->expected, address ); \
    break; \
}


int rumboot_regpoker_check_array( const struct regpoker_checker *array, uint32_t offset ) {
    int fails = 0;

    while( array->tp != REGPOKER_EOL ) {
        switch( array->tp ) {
            #define ioread8(address) (array->readfunc ? array->readfunc(address) : ioread8(address))
            #define ioread16(address) (array->readfunc ? array->readfunc(address) : ioread16(address))
            #define ioread32(address) (array->readfunc ? array->readfunc(address) : ioread32(address))
            #define ioread64(address) (array->readfunc ? array->readfunc(address) : ioread64(address))

            #define iowrite8( v, address ) (array->writefunc ? array->writefunc(v, address) :  iowrite8(v, address))
            #define iowrite16( v, address ) (array->writefunc ? array->writefunc(v, address) : iowrite16(v, address))
            #define iowrite32( v, address ) (array->writefunc ? array->writefunc(v, address) : iowrite32(v, address))
            #define iowrite64( v, address ) (array->writefunc ? array->writefunc(v, address) : iowrite64(v, address))

            READCHECK( REGPOKER_READ8, 8 );
            READCHECK( REGPOKER_READ16, 16 );
            READCHECK( REGPOKER_READ32, 32 );
            READCHECK( REGPOKER_READ64, 64 );
            WRITECHECK( REGPOKER_WRITE8, 8 );
            WRITECHECK( REGPOKER_WRITE16, 16 );
            WRITECHECK( REGPOKER_WRITE32, 32 );
            WRITECHECK( REGPOKER_WRITE64, 64 );
#ifdef __PPC__
    #define ioread32( address )     dcr_read( address )
    #define iowrite32( v, address ) dcr_write( address, v )
            READCHECK( REGPOKER_READ_DCR, 32 );
            WRITECHECK( REGPOKER_WRITE_DCR, 32 );
    #undef  ioread32
    #undef  iowrite32
#endif
            default: break;
        }
        array++;
    }
    return fails;
}
