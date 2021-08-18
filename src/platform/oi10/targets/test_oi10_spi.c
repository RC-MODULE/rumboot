#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <devices/pl022.h>
#include <devices/pl022_flash.h>
#include <devices/gpio_pl061.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
#include <devices/ugly/emi.h>

#include <regs/regs_spi.h>
#include <regs/regs_gpio_pl061.h>

#include <rumboot/irq.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <rumboot/platform.h>
#include <rumboot/timer.h>



#define BYTE_NUMBER             16
#define DATA_SIZE               (BYTE_NUMBER+4)
#define PAGE_PROGRAM_COMMAND    0x02000000
#define READ_COMMAND            0x03000000
#define TEST_DATA               0x99887766
#define TEST_DATA_LOOP          0x3C
#define TEST_DATA_IM2           0x5A
#define TEST_DATA_IM1           0xC3
#define TEST_DATA_EM2           0xA5

#ifdef GSPI_CHECK_REGS

static uint32_t gspi_check_regs( uint32_t const base_addr ) {
    rumboot_printf( "Checking GSPI registers ...\n" );

    static struct regpoker_checker const check_array[] = {
        { "STATUS",         REGPOKER_READ32,    GSPI_STATUS,        GSPI_STATUS_DEFAULT,        GSPI_STATUS_MASK },
        { "SSPCR0",         REGPOKER_READ32,    GSPI_SSPCR0,        GSPI_SSPCR0_DEFAULT,        GSPI_SSPCR0_MASK },
        { "SSPCR1",         REGPOKER_READ32,    GSPI_SSPCR1,        GSPI_SSPCR1_DEFAULT,        GSPI_SSPCR1_MASK },
        { "SSPSR",          REGPOKER_READ32,    GSPI_SSPSR,         GSPI_SSPSR_DEFAULT,         GSPI_SSPSR_MASK },
        { "SSPCPSR",        REGPOKER_READ32,    GSPI_SSPCPSR,       GSPI_SSPCPSR_DEFAULT,       GSPI_SSPCPSR_MASK },
        { "SSPIMSC",        REGPOKER_READ32,    GSPI_SSPIMSC,       GSPI_SSPIMSC_DEFAULT,       GSPI_SSPIMSC_MASK },
        { "SSPRIS",         REGPOKER_READ32,    GSPI_SSPRIS,        GSPI_SSPRIS_DEFAULT,        GSPI_SSPRIS_MASK },
        { "SSPDMACR",       REGPOKER_READ32,    GSPI_SSPDMACR,      GSPI_SSPDMACR_DEFAULT,      GSPI_SSPDMACR_MASK },
        { "SSPPeriphID0",   REGPOKER_READ32,    GSPI_SSPPeriphID0,  GSPI_SSPPeriphID0_DEFAULT,  GSPI_SSPPeriphID0_MASK },
        { "SSPPeriphID1",   REGPOKER_READ32,    GSPI_SSPPeriphID1,  GSPI_SSPPeriphID1_DEFAULT,  GSPI_SSPPeriphID1_MASK },
        { "SSPPeriphID2",   REGPOKER_READ32,    GSPI_SSPPeriphID2,  GSPI_SSPPeriphID2_DEFAULT,  GSPI_SSPPeriphID2_MASK },
        { "SSPPeriphID3",   REGPOKER_READ32,    GSPI_SSPPeriphID3,  GSPI_SSPPeriphID3_DEFAULT,  GSPI_SSPPeriphID3_MASK },
        { "SSPPCellID0",    REGPOKER_READ32,    GSPI_SSPPCellID0,   GSPI_SSPPCellID0_DEFAULT,   GSPI_SSPPCellID0_MASK },
        { "SSPPCellID1",    REGPOKER_READ32,    GSPI_SSPPCellID1,   GSPI_SSPPCellID1_DEFAULT,   GSPI_SSPPCellID1_MASK },
        { "SSPPCellID2",    REGPOKER_READ32,    GSPI_SSPPCellID2,   GSPI_SSPPCellID2_DEFAULT,   GSPI_SSPPCellID2_MASK },
        { "SSPPCellID3",    REGPOKER_READ32,    GSPI_SSPPCellID3,   GSPI_SSPPCellID3_DEFAULT,   GSPI_SSPPCellID3_MASK },
        { "SSPCR0",         REGPOKER_WRITE32,   GSPI_SSPCR0,        0x00,                       GSPI_SSPCR0_MASK },
        { "SSPCR1",         REGPOKER_WRITE32,   GSPI_SSPCR1,        0x00,                       GSPI_SSPCR1_MASK },
        { "SSPCPSR",        REGPOKER_WRITE32,   GSPI_SSPCPSR,       0x00,                       GSPI_SSPCPSR_MASK & ~0x01 },
        { "SSPIMSC",        REGPOKER_WRITE32,   GSPI_SSPIMSC,       0x00,                       GSPI_SSPIMSC_MASK },
        { "SSPDMACR",       REGPOKER_WRITE32,   GSPI_SSPDMACR,      0x00,                       GSPI_SSPDMACR_MASK },
        { "DMAWSTART",      REGPOKER_READ32,    GSPI_DMAWSTART,     GSPI_DMAWSTART_DEFAULT,     GSPI_DMAWSTART_MASK },
        { "DMAWEND",        REGPOKER_READ32,    GSPI_DMAWEND,       GSPI_DMAWEND_DEFAULT,       GSPI_DMAWEND_MASK },
        { "DMAWCNTRL",      REGPOKER_READ32,    GSPI_DMAWCNTRL,     GSPI_DMAWCNTRL_DEFAULT,     GSPI_DMAWCNTRL_MASK },
        { "DMAWCUR",        REGPOKER_READ32,    GSPI_DMAWCUR,       GSPI_DMAWCUR_DEFAULT,       GSPI_DMAWCUR_MASK },
        { "AXIW_BRESP",     REGPOKER_READ32,    GSPI_AXIW_BRESP,    GSPI_AXIW_BRESP_DEFAULT,    GSPI_AXIW_BRESP_MASK },
        { "DMARSTART",      REGPOKER_READ32,    GSPI_DMARSTART,     GSPI_DMARSTART_DEFAULT,     GSPI_DMARSTART_MASK },
        { "DMAREND",        REGPOKER_READ32,    GSPI_DMAREND,       GSPI_DMAREND_DEFAULT,       GSPI_DMAREND_MASK },
        { "DMARCNTRL",      REGPOKER_READ32,    GSPI_DMARCNTRL,     GSPI_DMARCNTRL_DEFAULT,     GSPI_DMARCNTRL_MASK },
        { "DMARCUR",        REGPOKER_READ32,    GSPI_DMARCUR,       GSPI_DMARCUR_DEFAULT,       GSPI_DMARCUR_MASK },
        { "AXIR_BUFTYPE",   REGPOKER_READ32,    GSPI_AXIR_BUFTYPE,  GSPI_AXIR_BUFTYPE_DEFAULT,  GSPI_AXIR_BUFTYPE_MASK },
        { "AXIR_BUFENA",    REGPOKER_READ32,    GSPI_AXIR_BUFENA,   GSPI_AXIR_BUFENA_DEFAULT,   GSPI_AXIR_BUFENA_MASK },
        { "AXIR_ERRADDR",   REGPOKER_READ32,    GSPI_AXIR_ERRADDR,  GSPI_AXIR_ERRADDR_DEFAULT,  GSPI_AXIR_ERRADDR_MASK },
        { "AXI_PARAMS",     REGPOKER_READ32,    GSPI_AXI_PARAMS,    GSPI_AXI_PARAMS_DEFAULT,    GSPI_AXI_PARAMS_MASK },
        { "WORDOP",         REGPOKER_READ32,    GSPI_WORDOP,        GSPI_WORDOP_DEFAULT,        GSPI_WORDOP_MASK },
        { "SOFTSS",         REGPOKER_READ32,    GSPI_SOFTSS,        GSPI_SOFTSS_DEFAULT,        GSPI_SOFTSS_MASK },
        { "SOFTRST",        REGPOKER_READ32,    GSPI_SOFTRST,       GSPI_SOFTRST_DEFAULT,       GSPI_SOFTRST_MASK },
        { "IRQMASKS",       REGPOKER_READ32,    GSPI_IRQMASKS,      GSPI_IRQMASKS_DEFAULT,      GSPI_IRQMASKS_MASK },
        { "DMAWSTART",      REGPOKER_WRITE32,   GSPI_DMAWSTART,     0x00,                       GSPI_DMAWSTART_MASK },
        { "DMAWEND",        REGPOKER_WRITE32,   GSPI_DMAWEND,       0x00,                       GSPI_DMAWEND_MASK },
        { "DMARSTART",      REGPOKER_WRITE32,   GSPI_DMARSTART,     0x00,                       GSPI_DMARSTART_MASK },
        { "DMAREND",        REGPOKER_WRITE32,   GSPI_DMAREND,       0x00,                       GSPI_DMAREND_MASK },
        { "AXIR_BUFTYPE",   REGPOKER_WRITE32,   GSPI_AXIR_BUFTYPE,  0x00,                       GSPI_AXIR_BUFTYPE_MASK },
        { "AXI_PARAMS",     REGPOKER_WRITE32,   GSPI_AXI_PARAMS,    0x00,                       GSPI_AXI_PARAMS_MASK },
        { "WORDOP",         REGPOKER_WRITE32,   GSPI_WORDOP,        0x00,                       GSPI_WORDOP_MASK },
        { "SOFTSS",         REGPOKER_WRITE32,   GSPI_SOFTSS,        0x00,                       GSPI_SOFTSS_MASK },
        { "IRQMASKS",       REGPOKER_WRITE32,   GSPI_IRQMASKS,      0x00,                       GSPI_IRQMASKS_MASK },
        { /* Sentinel */ }
    };

    return rumboot_regpoker_check_array( check_array, base_addr );
}

#endif //GSPI_CHECK_REGS

static volatile uint32_t IRQ = 0;
static volatile uint32_t SSP_data = 0;
static volatile uint32_t DMA_write_buffer_end = 0;
static volatile uint32_t GSPI_ext = 0;

static void gspi_irq_handler( int irq, void* arg ) {
    uint32_t const base_addr = (uint32_t)arg;

    uint32_t ssp_status = pl022_get_ssp_status( base_addr );
    uint32_t dma_status = pl022_get_dma_status( base_addr );

    rumboot_printf( "IRQ arrived  \n" );
    rumboot_printf( "GSPI SSPSR is 0x%x\n", ssp_status );
    rumboot_printf( "GSPI STATUS is 0x%x\n", dma_status );

    if( ssp_status & 0x4 ) {
        SSP_data = pl022_get_word( base_addr );

        rumboot_printf( "Clear SSP interrupts\n" );
        iowrite32( 0x0F, base_addr + GSPI_SSPICR );
    }

    if( dma_status & end_buf_write ) {
        DMA_write_buffer_end = 1;
    }

    if( dma_status & gspi_int ) {
        GSPI_ext = 1;
        gpio_set_direction_by_mask( GPIO_1_BASE, GPIO_REG_MASK, gpio_pin_direction_in );
    }

    msync();

    rumboot_printf( "GSPI STATUS is 0x%x\n", pl022_get_dma_status( base_addr ) );

    IRQ = 1;
}

static uint32_t wait_gspi_int() {
    for( uint32_t t = 1; t <= GSPI_TIMEOUT; t++ ) {
        if( IRQ ) {
            IRQ = 0;
            msync();
            return 0;
        }
    }

    rumboot_printf( "Error! IRQ flag wait timeout! \n" );
    return 1;
}

static struct ssp_params ssp_param;
static struct dma_params dma_param;
static struct pl022_config conf;

static uint32_t gspi_dma_axi( uint32_t const base_addr, uint32_t const r_mem_addr, uint32_t const w_mem_addr, uint32_t const test_data ) {
    rumboot_printf( "Read from address =%x\n", rumboot_virt_to_dma( ( void* )r_mem_addr ) );
    rumboot_printf( "Write to address =%x\n", rumboot_virt_to_dma( ( void* )w_mem_addr ) );

    pl022_set_int_mask( base_addr, 0x02 ); //interrupt masks - unmask rx_fifo not empty

    pl022_flash_write_enable( base_addr );
    pl022_flash_erase( base_addr );

    pl022_flash_write_enable( base_addr );
    pl022_dma_set_irq_mask( base_addr, end_buf_write );
    pl022_dma_enable( base_addr, all );

    pl022_dma_set_mode( base_addr, base_mode );

    //set dma parameters
    dma_param.endian_read = lsb;
    dma_param.arlen = 8;
    dma_param.endian_write = lsb;
    dma_param.awlen = 8;
    pl022_dma_set_param( base_addr, &dma_param );

    pl022_dma_set_read_addr( base_addr, ( uint32_t* )rumboot_virt_to_dma( ( void* )r_mem_addr ), BYTE_NUMBER );
    pl022_dma_set_write_addr( base_addr, ( uint32_t* )rumboot_virt_to_dma( ( void* )w_mem_addr ), BYTE_NUMBER );

    iowrite32( 0x9FFFFFE0, base_addr + GSPI_DMARCNTRL );
    iowrite32( 0X9FFFFFE0, base_addr + GSPI_DMAWCNTRL );

    iowrite32( 0x01, base_addr + GSPI_AXIR_BUFENA ); //enable transmitting

    while( !DMA_write_buffer_end ) {}
    DMA_write_buffer_end = 0;

    rumboot_printf( "Reading data...\n" );
    iowrite32( READ_COMMAND, r_mem_addr );

    iowrite32( 0x01, base_addr + GSPI_AXIR_BUFENA ); //enable transmitting

    while( !DMA_write_buffer_end ) {}
    DMA_write_buffer_end = 0;

    rumboot_printf( "Checking data ...\n" );
    for( uint32_t addr = (( uint32_t )w_mem_addr + 4); addr < (( uint32_t )w_mem_addr + DATA_SIZE); addr++ ) {
        rumboot_printf( "Read data from memory =%x\n", ioread8( addr ) );
        if( ioread8( addr ) != test_data ) {
            rumboot_printf( "GSPI AXI: SPI data read fail\n" );
            pl022_dma_enable( base_addr, disable );
            return 1;
        }
    }

    rumboot_printf( "Check GSPI AXI: OK\n" );
    pl022_dma_enable( base_addr, disable );
    return 0;
}

static uint32_t gspi_dma_axi_mem( uint32_t const base_addr, uint32_t const r_mem_addr, uint32_t const test_data ) {
    rumboot_printf( "Checking GSPI DMA read/write functionality ...\n" );

    uint32_t result = 0;

    memset( ( void* )( r_mem_addr + 4 ), test_data, (BYTE_NUMBER + 8) );
    //r_mem_addr - IM2
    iowrite32( PAGE_PROGRAM_COMMAND, r_mem_addr );
    msync();
    result += gspi_dma_axi( base_addr, r_mem_addr, ( IM2_BASE + DATA_SIZE ), test_data );
    //r_mem_addr - IM1
    iowrite32( PAGE_PROGRAM_COMMAND, r_mem_addr );
    msync();
    result += gspi_dma_axi( base_addr, r_mem_addr, ( IM1_BASE + DATA_SIZE ), test_data );
    //r_mem_addr - EM2
    iowrite32( PAGE_PROGRAM_COMMAND, r_mem_addr );
    msync();
    result += gspi_dma_axi( base_addr, r_mem_addr, ( SRAM0_BASE + DATA_SIZE ), test_data );

    return result;
}

static void pl022_flash_write_data( uint32_t const base_addr, uint32_t const data ) {
    uint32_t addr = base_addr + GSPI_SSPDR;

    iowrite32( SPIFLASH_PAGEPROG, addr ); //write data to SPI - command write
    for( uint32_t i = 0; i < 3; i++ ) iowrite32( 0x00, addr ); //write data to SPI - write address

    iowrite32( ( data & 0x000000FF ) >> 0,  base_addr + GSPI_SSPDR ); //write data to SPI - write data 1
    iowrite32( ( data & 0x0000FF00 ) >> 8,  base_addr + GSPI_SSPDR ); //write data to SPI - write data 2
    iowrite32( ( data & 0x00FF0000 ) >> 16, base_addr + GSPI_SSPDR ); //write data to SPI - write data 3
    iowrite32( ( data & 0xFF000000 ) >> 24, base_addr + GSPI_SSPDR ); //write data to SPI - write data 4

    while( (ioread32( base_addr + GSPI_SSPSR ) & 0x1) == 0 ) {} //wait tx fifo empty

    for( uint32_t i = 0; i < 8; i++ ) ( void )ioread32( addr ); //read data from SPI - staff
    while( (pl022_flash_read_status( base_addr ) & 0x1) == 0x1 ) {} //wait write complete
}

static uint32_t pl022_flash_read_data( uint32_t const base_addr ) {
    uint32_t data = 0x00000000;
    uint32_t addr = base_addr + GSPI_SSPDR;

    iowrite32( SPIFLASH_READDATA, addr ); //write data to SPI - command read
    for( uint32_t i = 0; i < 3; i++ ) iowrite32( 0x00, addr ); //write data to SPI - write address

    for( uint32_t i = 0; i < 4; i++ ) iowrite32( 0xFF, addr ); //write data to SPI - write address
    while( (ioread32( base_addr + GSPI_SSPSR ) & 0x1) == 0 ) {} //wait tx fifo empty

    for( uint32_t i = 0; i < 4; i++ ) ( void )ioread32( addr ); //read data from SPI - staff

    data =  (( ioread32( addr ) << 0)   & 0x000000FF);
    data |= (( ioread32( addr ) << 8)   & 0x0000FF00);
    data |= (( ioread32( addr ) << 16)  & 0x00FF0000);
    data |= (( ioread32( addr ) << 24)  & 0xFF000000);
    return data;
}

static uint32_t gspi_ssp_flash( uint32_t const base_addr ) {
    rumboot_printf( "Checking GSPI FLASH read/write functionality ...\n" );


    //Read and check ID
    uint32_t const gspi_sspid = ((ioread32( base_addr + 0xffc ) << 0)     & 0x000000FF)
                              | ((ioread32( base_addr + 0xff8 ) << 8)     & 0x0000FF00)
                              | ((ioread32( base_addr + 0xff4 ) << 16)    & 0x00FF0000)
                              | ((ioread32( base_addr + 0xff0 ) << 24)    & 0xFF000000);

    if( gspi_sspid != 0x0df005b1 ) {
        rumboot_printf( "ID read error! ID from SSP: 0x%x\n", gspi_sspid );
        return 1;
    }

    IRQ = 0;
    msync();
    pl022_dma_set_irq_mask( base_addr, ssp_int ); //mask all interrupts except SSP

    rumboot_printf( "Check SPI interrupt with loop operation\n" );
    ssp_param.loopback = true;
    pl022_set_param( base_addr, &ssp_param ); //turn on SSP controller, loop opera
    pl022_set_int_mask( base_addr, 0x02 ); //interrupt masks - unmask rx_fifo not empty

    pl022_send_word( base_addr, TEST_DATA_LOOP ); //write data to SPI with IRQ

    if( wait_gspi_int() ) {
        rumboot_printf( "SPI interrupt timeout\n" );
        return 1;
    }

    if( SSP_data != TEST_DATA_LOOP ) {
        rumboot_printf( "SPI loop read error, read data = 0x%x\n", SSP_data );
        return 1;
    }

    rumboot_printf( "Check read and write data to spi flash\n" );
    ssp_param.loopback = false;
    pl022_set_param( base_addr, &ssp_param ); //turn on SSP controller, loop opera
    pl022_set_int_mask( base_addr, 0x0 ); //mask all interrupts

    pl022_flash_write_enable( base_addr );
    pl022_flash_erase( base_addr );

    pl022_flash_write_enable( base_addr );
    pl022_flash_write_data( base_addr, TEST_DATA );

    //Read and check data from spi flash
    uint32_t read_data = pl022_flash_read_data( base_addr );
    if( read_data != TEST_DATA ) {
        rumboot_printf( "read_data = %x\n", read_data );
        rumboot_printf( "SPI data read fail\n" );
        return 1;
    }
    return 0;
}

static uint32_t gspi_ext_int( uint32_t const base_addr ) {
    uint32_t result = 0;

    rumboot_printf( "Checking GSPI external interrupt ...\n" );

    pl022_get_dma_status( base_addr ); // clear interrupt
    pl022_dma_set_irq_mask( base_addr, gspi_int );

    gpio_set_direction_by_mask( GPIO_1_BASE, GPIO1_X, gpio_pin_direction_out );
    gpio_set_value_by_mask( GPIO_1_BASE, GPIO_REG_MASK, GPIO1_X );
    msync();

    if( wait_gspi_int() | !GSPI_ext ) {
        rumboot_printf( "GSPI external interrupt timeout\n" );
        result = 1;
    }

    return result;
}

int main( void ) {
    uint32_t test_result = 0;

    emi_init( DCR_EM2_EMI_BASE );

    rumboot_printf( "GSPI test has been started ... \n" );

#ifdef GSPI_CHECK_REGS
    test_result += gspi_check_regs(GSPI_BASE);
#endif

    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, GSPI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, gspi_irq_handler, ( void* )GSPI_BASE );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( GSPI_INT );
    rumboot_irq_sei();

    test_result += gspi_ext_int( GSPI_BASE );

    pl022_dma_reset( GSPI_BASE );
    //initial ssp
    conf.ssp_clk = 100000000;
    conf.spi_clk = 20000000;
    conf.data_size = 8;
    conf.soft_cs = 0;
    conf.variant = PL022_VARIANT_GSPI;

    pl022_init( GSPI_BASE, &conf );

    //set ssp parameters
    ssp_param.cpol = 1;
    ssp_param.cpha = 1;
    ssp_param.mode = master_mode;
    ssp_param.loopback = false;
    ssp_param.fr_format = ssp_motorola_fr_form;
    pl022_set_param( GSPI_BASE, &ssp_param ); //turn on SSP controller

    test_result += gspi_dma_axi_mem( GSPI_BASE, IM2_BASE, TEST_DATA_IM2 );
    test_result += gspi_dma_axi_mem( GSPI_BASE, IM1_BASE, TEST_DATA_IM1 );
    test_result += gspi_dma_axi_mem( GSPI_BASE, SSRAM_BASE, TEST_DATA_EM2 );

    pl022_dma_reset( GSPI_BASE );
    test_result += gspi_ssp_flash( GSPI_BASE );

    rumboot_irq_table_activate( NULL );
    rumboot_irq_free( tbl );

    return test_result;
}
