#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/printf.h>
#include "gdbmon-crc.h"


extern void memcpy_copyed ( void *d, void *s, size_t n );


#define BASE_ADDR   0x800000

// gdbmon buffers' addr:
#define TX_BUF_ADDR     (BASE_ADDR + 0x025fc)   // size = 0x400
#define RX_BUF_ADDR     (BASE_ADDR + 0x029fc)   // size = 0x400
#define TMP_BUF_ADDR    (BASE_ADDR + 0x02dfc)   // size = 0x200
#define TX_BUF_SIZE     (0x400)
#define RX_BUF_SIZE     (0x400)
#define TMP_BUF_SIZE    (0x200)

// test.elf structure:
#define SECTION_TEXT_ADDR   (BASE_ADDR + 0x080000)
#define SECTION_RODATA_ADDR (BASE_ADDR + 0x089e30)
#define SECTION_DATA_ADDR   (BASE_ADDR + 0x08a2d0)
#define SECTION_TEXT_SIZE   (0x9e30)
#define SECTION_RODATA_SIZE (0x4a0)
#define SECTION_DATA_SIZE   (0x18cb0)
#define SECTION_TEXT_CRC    0x8a903e60
#define SECTION_RODATA_CRC  0xcb5e8fe6
#define SECTION_DATA_CRC    0x02e52c2f

#define L2C_LINE_SIZE   128u
#define L2C_MASK        ( ~( L2C_LINE_SIZE - 1 ) )

uint32_t gdbmon_crc32 ( unsigned char *addr, unsigned len )
{
    unsigned tmp_buf_len = 512;     // size = 0x200
    uint32_t crc = GDBMON_CRC32_START;

    while ( len )
    {
        int rd = len > tmp_buf_len ? tmp_buf_len : len;
        memcpy_copyed( ( void * )TMP_BUF_ADDR, addr, rd );

        if ( rd > 0  )
        {
            len -= rd;
            addr += rd;
            crc = gdbmon_crc32_next ( ( void * )TMP_BUF_ADDR, rd, crc );
        }
        else
        {

            break;
        }
    }

    return crc;
}


void load_test ( uint32_t dest_addr, uint32_t length )
{
    uint32_t    value, size, i;
    uint32_t    *ptr;

    while ( length > 0 )
    {
        ptr = ( uint32_t * )RX_BUF_ADDR;
        value = dest_addr;

        for ( i = 0; i < ( RX_BUF_SIZE / 4 ); i++ )
        {
            *ptr++ = value;
            value += 4;
        }

        size = length > RX_BUF_SIZE ? RX_BUF_SIZE : length;

        memcpy_copyed( ( void * )dest_addr, ( void * )RX_BUF_ADDR, RX_BUF_SIZE );

        length -= size;
        dest_addr += size;
    }
}


int main ( void )
{
    uint32_t crc_result;
    uint32_t i;
    int     isError = 0;

    // На случай X в модели
    memset( ( void * )( ( TX_BUF_ADDR - BASE_ADDR ) & L2C_MASK ),         0, TX_BUF_SIZE + L2C_LINE_SIZE );
    memset( ( void * )( ( RX_BUF_ADDR - BASE_ADDR ) & L2C_MASK ),         0, RX_BUF_SIZE + L2C_LINE_SIZE );
    memset( ( void * )( ( TMP_BUF_ADDR - BASE_ADDR ) & L2C_MASK ),        0, TMP_BUF_SIZE + L2C_LINE_SIZE );
    memset( ( void * )( ( SECTION_TEXT_ADDR - BASE_ADDR ) & L2C_MASK ),   0, SECTION_TEXT_SIZE + L2C_LINE_SIZE );
    memset( ( void * )( ( SECTION_RODATA_ADDR - BASE_ADDR ) & L2C_MASK ), 0, SECTION_RODATA_SIZE + L2C_LINE_SIZE );
    memset( ( void * )( ( SECTION_DATA_ADDR - BASE_ADDR ) & L2C_MASK ),   0, SECTION_DATA_SIZE + L2C_LINE_SIZE );

    // Load test to memory
    load_test( SECTION_TEXT_ADDR, SECTION_TEXT_SIZE );
    load_test( SECTION_RODATA_ADDR, SECTION_RODATA_SIZE );
    load_test( SECTION_DATA_ADDR, SECTION_DATA_SIZE );

    for ( i = 0; i < 16; i++ )
    {
        crc_result = gdbmon_crc32( ( void * )SECTION_TEXT_ADDR, SECTION_TEXT_SIZE );

        if ( crc_result != SECTION_TEXT_CRC )
        {
            rumboot_printf( "Section .text:   0x%x\n", crc_result );
            isError = 1;
            break;
        }

        crc_result = gdbmon_crc32( ( void * )SECTION_RODATA_ADDR, SECTION_RODATA_SIZE );

        if ( crc_result != SECTION_RODATA_CRC )
        {
            rumboot_printf( "Section .rodata: 0x%x\n", crc_result );
            isError = 1;
            break;
        }

        crc_result = gdbmon_crc32( ( void * )SECTION_DATA_ADDR, SECTION_DATA_SIZE );

        if ( crc_result != SECTION_DATA_CRC )
        {
            rumboot_printf( "Section .data:   0x%x\n", crc_result );
            isError = 1;
            break;
        }
    }

    rumboot_printf( "Loop_number = %d\n", i );

    return isError;
}
