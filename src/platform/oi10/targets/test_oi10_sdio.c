#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/rumboot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/irq.h>
#include <rumboot/io.h>
#include <devices/sdio_spi.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/test_event_c.h>


#define DATA_ARRAY_SIZE 128

static int init_sd() {
    uint32_t read_data;

    //Switch to SDIO from GSPI
    // SDIO_SPI = true;
    iowrite32( 0x1, SDIO_BASE + SDIO_ENABLE ); //sdio enable
    iowrite32( 0x0, SDIO_BASE + SDIO_CLK_DIVIDE ); //sdio clock
    iowrite32( 0x7c, SDIO_BASE + SDIO_INT_MASKS ); //all sdio interrupts enable

    //Init SD card

    iowrite32( 0x16f, SDIO_BASE + 0x014 ); //enable the interrupt and the error flag

    iowrite32( 0x0, SDIO_BASE + 0x008 ); //set CMD0 argument
    iowrite32( 0x0, SDIO_BASE + 0x004 ); //set CMD0 index and send out the command
    read_data = ioread32( SDIO_BASE + 0x048 );
    while( !( read_data & 0x004 ) ) {
        read_data = ioread32( SDIO_BASE + 0x048 );
    }
    iowrite32( 0x004, SDIO_BASE + 0x048 );

    iowrite32( 0x0, SDIO_BASE + 0x008 ); //set CMD0 argument
    iowrite32( 0x0, SDIO_BASE + 0x004 ); //set CMD0 index and send out the command
    read_data = ioread32( SDIO_BASE + 0x048 );
    while( !( read_data & 0x004 ) ) {
        read_data = ioread32( SDIO_BASE + 0x048 );
    }
    iowrite32( 0x004, SDIO_BASE + 0x048 );

    iowrite32( 0x000001aa, SDIO_BASE + 0x008 ); //set CMD8 argument
    iowrite32( 0x00083811, SDIO_BASE + 0x004 ); //set CMD8 index and send out the command
    read_data = ioread32( SDIO_BASE + 0x048 );
    while( !( read_data & 0x004 ) ) {
        read_data = ioread32( SDIO_BASE + 0x048 );
    }
    iowrite32( 0x004, SDIO_BASE + 0x048 );
    //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}

    iowrite32( 0x00000000, SDIO_BASE + 0x008 ); // set CMD55 argument
    iowrite32( 0x00373811, SDIO_BASE + 0x004 ); //set CMD55 index and send out the command
    //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data = ioread32( SDIO_BASE + 0x048 );
    while( !( read_data & 0x004 ) ) {
        read_data = ioread32( SDIO_BASE + 0x048 );
    }
    iowrite32( 0x004, SDIO_BASE + 0x048 );

    iowrite32( 0x00000000, SDIO_BASE + 0x008 ); //set CMD41 argument
    iowrite32( 0x00293811, SDIO_BASE + 0x004 ); //set CMD41 index and send out the command
    //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data = ioread32( SDIO_BASE + 0x048 );
    while( !( read_data & 0x004 ) ) {
        read_data = ioread32( SDIO_BASE + 0x048 );
    }
    iowrite32( 0x004, SDIO_BASE + 0x048 );

    iowrite32( 0x00000000, SDIO_BASE + 0x008 ); //set CMD2 argument
    iowrite32( 0x00023411, SDIO_BASE + 0x004 ); //set CMD2 index and send out the command
    //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data = ioread32( SDIO_BASE + 0x048 );
    while( !( read_data & 0x004 ) ) {
        read_data = ioread32( SDIO_BASE + 0x048 );
    }
    iowrite32( 0x004, SDIO_BASE + 0x048 );

    iowrite32( 0x00000000, SDIO_BASE + 0x008 ); //set CMD3 argument
    iowrite32( 0x00033811, SDIO_BASE + 0x004 ); //set CMD3 index and send out the command
    //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data = ioread32( SDIO_BASE + 0x048 );
    while( !( read_data & 0x004 ) ) {
        read_data = ioread32( SDIO_BASE + 0x048 );
    }
    iowrite32( 0x004, SDIO_BASE + 0x048 );

    iowrite32( 0x12340000, SDIO_BASE + 0x008 ); //set CMD7 argument
    iowrite32( 0x00073c11, SDIO_BASE + 0x004 ); //set CMD7index and send out the command
    //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data = ioread32( SDIO_BASE + 0x048 );
    while( !( read_data & 0x004 ) ) {
        read_data = ioread32( SDIO_BASE + 0x048 );
    }
    iowrite32( 0x004, SDIO_BASE + 0x048 );

    iowrite32( 0x12340000, SDIO_BASE + 0x008 ); //set CMD55 argument
    iowrite32( 0x00373811, SDIO_BASE + 0x004 ); //set CMD55 index and send out the command
    //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data = ioread32( SDIO_BASE + 0x048 );
    while( !( read_data & 0x004 ) ) {
        read_data = ioread32( SDIO_BASE + 0x048 );
    }
    iowrite32( 0x004, SDIO_BASE + 0x048 );

    iowrite32( 0x00000002, SDIO_BASE + 0x008 ); //set CMD6 argument
    iowrite32( 0x00063811, SDIO_BASE + 0x004 ); //set CMD6 index and send out the command
    //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data = ioread32( SDIO_BASE + 0x048 );
    while( !( read_data & 0x004 ) ) {
        read_data = ioread32( SDIO_BASE + 0x048 );
    }
    iowrite32( 0x004, SDIO_BASE + 0x048 );

    return 0;
}

#ifndef SDIO_SRC_HEAP
#define SDIO_SRC_HEAP SDIO_HEAP
#endif
#ifndef SDIO_DST_HEAP
#define SDIO_DST_HEAP SDIO_HEAP
#endif

void prepare_arrays( uint32_t ** src_array, uint32_t ** dst_array ) {
    *src_array = rumboot_malloc_from_named_heap( SDIO_SRC_HEAP, sizeof(uint32_t)*DATA_ARRAY_SIZE );
    *dst_array = rumboot_malloc_from_named_heap( SDIO_DST_HEAP, sizeof(uint32_t)*DATA_ARRAY_SIZE );
    for( uint32_t i = 0; i < DATA_ARRAY_SIZE; i++ )
        (*src_array)[ i ] = i + ( i << 8 ) + ( i << 16 ) + ( i << 24 );
}

static volatile uint32_t SDIO_TRAN_DONE;
static volatile uint32_t SDIO_CMD_DONE;
static volatile uint32_t SDIO_BUF_FINISH;

static void handler1() {
    uint32_t SDIO_clear = 0;
    uint32_t const SDIO_Status = ioread32( SDIO_BASE + SDIO_INT_STATUS ); //Read interrupt status

    if( ( SDIO_Status >> 5 ) & 0x1 ) {
        SDIO_clear = SDIO_clear | 0x4; //Clear command done interrupt
        SDIO_CMD_DONE = 1;
    }
    if( ( SDIO_Status >> 2 ) & 0x1 ) {
        SDIO_clear = SDIO_clear | 0x1; //Clear buf transfer finish interrupt
        SDIO_BUF_FINISH = 1;
    }
    if( ( SDIO_Status >> 4 ) & 0x1 ) {
        SDIO_clear = SDIO_clear | 0x2; //Clear card transfer done interrupt
        SDIO_TRAN_DONE = 1;
    }
    if( ( SDIO_Status >> 6 ) & 0x1 ) {
        rumboot_printf( "Error in SDIO card !\n" );
        SDIO_clear = SDIO_clear | 0x10;
    }
    iowrite32( SDIO_clear, SDIO_BASE + 0x048 );
    msync();
}

static inline __attribute__((always_inline)) uint32_t wait_sdio_int_handled( uint32_t timeout, volatile uint32_t * const flag ) {
    do {
        if( *flag ) {
            *flag = 0;
            msync();

            return 1;
        }
    } while ( --timeout );

    return 0;
}

static struct rumboot_irq_entry * init_irq() {
    rumboot_irq_cli();
    struct rumboot_irq_entry * const tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, GSPI_SDIO_IRQ, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );

    rumboot_irq_table_activate( tbl );
    rumboot_irq_sei();
    return tbl;
}

static void deinit_irq( struct rumboot_irq_entry * const tbl ) {
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

static int sdio_dma_tr( uint32_t src_addr, uint32_t dst_addr ) {
    //write data
    iowrite32( 0x00000101, SDIO_BASE + 0x000 ); //one block data transfer,512 byte
    iowrite32( 0x00000006, SDIO_BASE + 0x050 ); //select buf0,write transfer start
    iowrite32( 0x00000200, SDIO_BASE + 0x034 ); //set dma total data 512 byte
    iowrite32( src_addr, SDIO_BASE + 0x02c ); //dma source atart address
    iowrite32( 0x00002001, SDIO_BASE + 0x028 ); //ARLEN=16,ARSIZE=2(4bytes)
    rumboot_printf( "check_1\n" );
    if( !wait_sdio_int_handled( 1000, &SDIO_BUF_FINISH ) ) {
        rumboot_printf( "SDIO interrupt timeout_1\n" );
        return 1;
    }

    iowrite32( 0x00000000, SDIO_BASE + 0x00c ); //set use buf0 and write data to SD bus
    iowrite32( 0x00000101, SDIO_BASE + 0x000 ); //set data block 512 byte ans send one block
    iowrite32( 0x00000000, SDIO_BASE + 0x008 ); //set cmd24 argument
    iowrite32( 0x00187811, SDIO_BASE + 0x004 ); //set cmd24 index out the command
    rumboot_printf( "check_2\n" );
    if( !wait_sdio_int_handled( 1000, &SDIO_CMD_DONE ) ) {
        rumboot_printf( "SDIO interrupt timeout_2\n" );
        return 1;
    }
    if( !wait_sdio_int_handled( 1000, &SDIO_TRAN_DONE ) ) {
        rumboot_printf( "SDIO interrupt timeout_3\n" );
        return 1;
    }

    //Read data from SD card
    iowrite32( 0x00000000, SDIO_BASE + 0x00c );
    iowrite32( 0x00000101, SDIO_BASE + 0x000 );
    iowrite32( 0x00000000, SDIO_BASE + 0x008 );
    iowrite32( 0x00117911, SDIO_BASE + 0x004 );
    rumboot_printf( "check_3\n" );
    if( !wait_sdio_int_handled( 1000, &SDIO_CMD_DONE ) ) {
        rumboot_printf( "SDIO interrupt timeout_4\n" );
        return 1;
    }
    if( !wait_sdio_int_handled( 1000, &SDIO_TRAN_DONE ) ) {
        rumboot_printf( "SDIO interrupt timeout_5\n" );
        return 1;
    }

    iowrite32( 0x00000004, SDIO_BASE + 0x050 );
    iowrite32( 0x00000200, SDIO_BASE + 0x044 );
    iowrite32( dst_addr, SDIO_BASE + 0x040 );
    iowrite32( 0x00020001, SDIO_BASE + 0x038 );
    rumboot_printf( "check_4\n" );
    if( !wait_sdio_int_handled( 1000, &SDIO_BUF_FINISH ) ) {
        rumboot_printf( "SDIO interrupt timeout_6\n" );
        return 1;
    }
    return 0;
}

int main() {
    int result = 0;
    uint32_t * src_array;
    uint32_t * dst_array;

    rumboot_memfill8_modelling((void*)SRAM0_BASE, 0x1000, 0x00, 0x00); //workaround (init 4KB SRAM0)

    init_sd();
    struct rumboot_irq_entry * tbl = init_irq();

    prepare_arrays( &src_array, &dst_array );
    if( (sdio_dma_tr( rumboot_virt_to_dma(src_array), rumboot_virt_to_dma(dst_array) ) != 0)
     || (memcmp( src_array, dst_array, sizeof(uint32_t)*DATA_ARRAY_SIZE ) != 0) ) {
        result = 1;
    }

    deinit_irq(tbl);

    return result;
}

