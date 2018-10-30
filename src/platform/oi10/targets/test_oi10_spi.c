#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <devices/pl022.h>
#include <devices/pl022_flash.h>
#include <devices/gpio_pl061.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/devices/emi.h>

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

static uint32_t gspi_check_regs(uint32_t const base_addr)
{
    struct regpoker_checker check_array[] = {
     { "STATUS",        REGPOKER_READ32, GSPI_STATUS,       GSPI_STATUS_DEFAULT,        GSPI_STATUS_MASK },
     { "SSPCR0",        REGPOKER_READ32, GSPI_SSPCR0,       GSPI_SSPCR0_DEFAULT,        GSPI_SSPCR0_MASK },
     { "SSPCR1",        REGPOKER_READ32, GSPI_SSPCR1,       GSPI_SSPCR1_DEFAULT,        GSPI_SSPCR1_MASK },
     { "SSPSR",         REGPOKER_READ32, GSPI_SSPSR,        GSPI_SSPSR_DEFAULT,         GSPI_SSPSR_MASK },
     { "SSPCPSR",       REGPOKER_READ32, GSPI_SSPCPSR,      GSPI_SSPCPSR_DEFAULT,       GSPI_SSPCPSR_MASK },
     { "SSPIMSC",       REGPOKER_READ32, GSPI_SSPIMSC,      GSPI_SSPIMSC_DEFAULT,       GSPI_SSPIMSC_MASK },
     { "SSPRIS",        REGPOKER_READ32, GSPI_SSPRIS,       GSPI_SSPRIS_DEFAULT,        GSPI_SSPRIS_MASK },
     { "SSPDMACR",      REGPOKER_READ32, GSPI_SSPDMACR,     GSPI_SSPDMACR_DEFAULT,      GSPI_SSPDMACR_MASK },
     { "SSPPeriphID0",  REGPOKER_READ32, GSPI_SSPPeriphID0, GSPI_SSPPeriphID0_DEFAULT,  GSPI_SSPPeriphID0_MASK },
     { "SSPPeriphID1",  REGPOKER_READ32, GSPI_SSPPeriphID1, GSPI_SSPPeriphID1_DEFAULT,  GSPI_SSPPeriphID1_MASK },
     { "SSPPeriphID2",  REGPOKER_READ32, GSPI_SSPPeriphID2, GSPI_SSPPeriphID2_DEFAULT,  GSPI_SSPPeriphID2_MASK },
     { "SSPPeriphID3",  REGPOKER_READ32, GSPI_SSPPeriphID3, GSPI_SSPPeriphID3_DEFAULT,  GSPI_SSPPeriphID3_MASK },
     { "SSPPCellID0",   REGPOKER_READ32, GSPI_SSPPCellID0,  GSPI_SSPPCellID0_DEFAULT,   GSPI_SSPPCellID0_MASK },
     { "SSPPCellID1",   REGPOKER_READ32, GSPI_SSPPCellID1,  GSPI_SSPPCellID1_DEFAULT,   GSPI_SSPPCellID1_MASK },
     { "SSPPCellID2",   REGPOKER_READ32, GSPI_SSPPCellID2,  GSPI_SSPPCellID2_DEFAULT,   GSPI_SSPPCellID2_MASK },
     { "SSPPCellID3",   REGPOKER_READ32, GSPI_SSPPCellID3,  GSPI_SSPPCellID3_DEFAULT,   GSPI_SSPPCellID3_MASK },
     { "SSPCR0",        REGPOKER_WRITE32, GSPI_SSPCR0,      0x00,                       GSPI_SSPCR0_MASK },
     { "SSPCR1",        REGPOKER_WRITE32, GSPI_SSPCR1,      0x00,                       GSPI_SSPCR1_MASK },
     { "SSPCPSR",       REGPOKER_WRITE32, GSPI_SSPCPSR,     0x00,                       GSPI_SSPCPSR_MASK & ~0x01 },
     { "SSPIMSC",       REGPOKER_WRITE32, GSPI_SSPIMSC,     0x00,                       GSPI_SSPIMSC_MASK },
     { "SSPDMACR",      REGPOKER_WRITE32, GSPI_SSPDMACR,    0x00,                       GSPI_SSPDMACR_MASK },
     { "DMAWSTART",     REGPOKER_READ32, GSPI_DMAWSTART,    GSPI_DMAWSTART_DEFAULT,     GSPI_DMAWSTART_MASK },
     { "DMAWEND",       REGPOKER_READ32, GSPI_DMAWEND,      GSPI_DMAWEND_DEFAULT,       GSPI_DMAWEND_MASK },
     { "DMAWCNTRL",     REGPOKER_READ32, GSPI_DMAWCNTRL,    GSPI_DMAWCNTRL_DEFAULT,     GSPI_DMAWCNTRL_MASK },
     { "DMAWCUR",       REGPOKER_READ32, GSPI_DMAWCUR,      GSPI_DMAWCUR_DEFAULT,       GSPI_DMAWCUR_MASK },
     { "AXIW_BRESP",    REGPOKER_READ32, GSPI_AXIW_BRESP,   GSPI_AXIW_BRESP_DEFAULT,    GSPI_AXIW_BRESP_MASK },
     { "DMARSTART",     REGPOKER_READ32, GSPI_DMARSTART,    GSPI_DMARSTART_DEFAULT,     GSPI_DMARSTART_MASK },
     { "DMAREND",       REGPOKER_READ32, GSPI_DMAREND,      GSPI_DMAREND_DEFAULT,       GSPI_DMAREND_MASK },
     { "DMARCNTRL",     REGPOKER_READ32, GSPI_DMARCNTRL,    GSPI_DMARCNTRL_DEFAULT,     GSPI_DMARCNTRL_MASK },
     { "DMARCUR",       REGPOKER_READ32, GSPI_DMARCUR,      GSPI_DMARCUR_DEFAULT,       GSPI_DMARCUR_MASK },
     { "AXIR_BUFTYPE",  REGPOKER_READ32, GSPI_AXIR_BUFTYPE, GSPI_AXIR_BUFTYPE_DEFAULT,  GSPI_AXIR_BUFTYPE_MASK },
     { "AXIR_BUFENA",   REGPOKER_READ32, GSPI_AXIR_BUFENA,  GSPI_AXIR_BUFENA_DEFAULT,   GSPI_AXIR_BUFENA_MASK },
     { "AXIR_ERRADDR",  REGPOKER_READ32, GSPI_AXIR_ERRADDR, GSPI_AXIR_ERRADDR_DEFAULT,  GSPI_AXIR_ERRADDR_MASK },
     { "AXI_PARAMS",    REGPOKER_READ32, GSPI_AXI_PARAMS,   GSPI_AXI_PARAMS_DEFAULT,    GSPI_AXI_PARAMS_MASK },
     { "WORDOP",        REGPOKER_READ32, GSPI_WORDOP,       GSPI_WORDOP_DEFAULT,        GSPI_WORDOP_MASK },
     { "SOFTSS",        REGPOKER_READ32, GSPI_SOFTSS,       GSPI_SOFTSS_DEFAULT,        GSPI_SOFTSS_MASK },
     { "SOFTRST",       REGPOKER_READ32, GSPI_SOFTRST,      GSPI_SOFTRST_DEFAULT,       GSPI_SOFTRST_MASK },
     { "IRQMASKS",      REGPOKER_READ32, GSPI_IRQMASKS,     GSPI_IRQMASKS_DEFAULT,      GSPI_IRQMASKS_MASK },
     { "DMAWSTART",     REGPOKER_WRITE32, GSPI_DMAWSTART,    0x00,                      GSPI_DMAWSTART_MASK },
     { "DMAWEND",       REGPOKER_WRITE32, GSPI_DMAWEND,      0x00,                      GSPI_DMAWEND_MASK },
     { "DMARSTART",     REGPOKER_WRITE32, GSPI_DMARSTART,    0x00,                      GSPI_DMARSTART_MASK },
     { "DMAREND",       REGPOKER_WRITE32, GSPI_DMAREND,      0x00,                      GSPI_DMAREND_MASK },
     { "AXIR_BUFTYPE",  REGPOKER_WRITE32, GSPI_AXIR_BUFTYPE, 0x00,                      GSPI_AXIR_BUFTYPE_MASK },
     { "AXI_PARAMS",    REGPOKER_WRITE32, GSPI_AXI_PARAMS,   0x00,                      GSPI_AXI_PARAMS_MASK },
     { "WORDOP",        REGPOKER_WRITE32, GSPI_WORDOP,       0x00,                      GSPI_WORDOP_MASK },
     { "SOFTSS",        REGPOKER_WRITE32, GSPI_SOFTSS,       0x00,                      GSPI_SOFTSS_MASK },
     { "IRQMASKS",      REGPOKER_WRITE32, GSPI_IRQMASKS,     0x00,                      GSPI_IRQMASKS_MASK },
     { /* Sentinel */ }
    };

    return rumboot_regpoker_check_array(check_array, base_addr);
}

#endif //GSPI_CHECK_REGS

static volatile uint32_t IRQ = 0;
static volatile uint32_t SSP_data = 0;
static volatile uint32_t DMA_write_buffer_end = 0;

static void gspi_irq_handler( int irq, void *arg )
{
    uint32_t ssp_status = pl022_get_ssp_status(GSPI_BASE);
    uint32_t dma_status = pl022_get_dma_status(GSPI_BASE);

    rumboot_printf( "IRQ arrived  \n" );
    rumboot_printf("GSPI SSPSR is 0x%x\n", ssp_status);
    rumboot_printf("GSPI STATUS is 0x%x\n", dma_status);

    if (ssp_status & 0x4)
    {
        SSP_data = pl022_get_word(GSPI_BASE);
    }

    if (dma_status & 0x4)
    {
        DMA_write_buffer_end = 1;
    }

    rumboot_printf("Clear interrupts\n");
    iowrite32(0x0F, GSPI_BASE + GSPI_SSPICR);

    rumboot_printf("GSPI STATUS is 0x%x\n", pl022_get_dma_status(GSPI_BASE));

    IRQ = 1;
}

static uint32_t wait_gspi_int()
{
    unsigned t;

    for (t=1; t<=GSPI_TIMEOUT; t++)
    {
        if (IRQ)
        {
            IRQ = 0;
            break;
        }
    }
    if (t>=GSPI_TIMEOUT)
    {
        rumboot_printf("Error! IRQ flag wait timeout! \n");
        return 1;
    }
    return 0;
}

static struct ssp_params ssp_param;
static struct dma_params dma_param;
static struct pl022_config conf;

static uint32_t gspi_dma_axi(uint32_t const base_addr, uint32_t const r_mem_addr, uint32_t const w_mem_addr, uint32_t const test_data)
{
    rumboot_printf("Read from address =%x\n", rumboot_virt_to_dma((void*)r_mem_addr));
    rumboot_printf("Write to address =%x\n", rumboot_virt_to_dma((void*)w_mem_addr));

    pl022_set_int_mask(base_addr, 0x02); //interrupt masks - unmask rx_fifo not empty


    pl022_flash_write_enable(base_addr);
    pl022_flash_erase(base_addr);

    pl022_flash_write_enable(base_addr);
    pl022_dma_set_irq_mask(base_addr, end_buf_write);
    pl022_dma_enable(base_addr, all);

    pl022_dma_set_mode(base_addr, base_mode);

    //set dma parameters
    dma_param.endian_read  = lsb;
    dma_param.arlen        = 8;
    dma_param.endian_write = lsb;
    dma_param.awlen        = 8;
    pl022_dma_set_param(base_addr, &dma_param);

    pl022_dma_set_read_addr(base_addr, (uint32_t*)rumboot_virt_to_dma((void*)r_mem_addr), BYTE_NUMBER);
    pl022_dma_set_write_addr(base_addr, (uint32_t*)rumboot_virt_to_dma((void*)w_mem_addr), BYTE_NUMBER);

    iowrite32(0x9FFFFFE0, base_addr + GSPI_DMARCNTRL);
    iowrite32(0X9FFFFFE0, base_addr + GSPI_DMAWCNTRL);

    iowrite32(0x01, base_addr + GSPI_AXIR_BUFENA); //enable transmitting

    while(!DMA_write_buffer_end)
        ;
    DMA_write_buffer_end = 0;

    rumboot_putstring("Reading data...\n");
    iowrite32(READ_COMMAND, r_mem_addr);

    iowrite32(0x01, base_addr + GSPI_AXIR_BUFENA); //enable transmitting

    while(!DMA_write_buffer_end)
        ;
    DMA_write_buffer_end = 0;

    rumboot_putstring("Checking data ...\n");
    for(uint32_t addr = ((uint32_t)w_mem_addr +4); addr < ((uint32_t)w_mem_addr+DATA_SIZE); addr++)
    {
        rumboot_printf("Read data from memory =%x\n", ioread8(addr));
        if (ioread8(addr) != test_data)
        {
            rumboot_putstring("GSPI AXI: SPI data read fail\n");
            pl022_dma_enable(base_addr, disable);
            return 1;
        }
    }
    rumboot_putstring("Check GSPI AXI: OK\n");
    pl022_dma_enable(base_addr, disable);
    return 0;
}

static void pl022_flash_write_data(uint32_t const base_addr, uint32_t const data)
{
    uint32_t addr =  base_addr+GSPI_SSPDR;
    iowrite8(SPIFLASH_PAGEPROG, addr); //write data to SPI - command write
    for (int i =0; i<3; i++)
        iowrite8(0x00, addr);//write data to SPI - write address

    iowrite8((data & 0x000000FF), base_addr+GSPI_SSPDR); //write data to SPI - write data 1
    iowrite8((data & 0x0000FF00) >> 8, base_addr+GSPI_SSPDR); //write data to SPI - write data 2
    iowrite8((data & 0x00FF0000) >> 16, base_addr+GSPI_SSPDR); //write data to SPI - write data 3
    iowrite8((data & 0xFF000000) >> 24, base_addr+GSPI_SSPDR); //write data to SPI - write data 4

    while((ioread8(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        {}
    for (int i =0; i<8; i++)
        (void)ioread8(addr); //read data from SPI - staff
    while ((read_flash_status(base_addr) & 0x1) == 0x1) //wait write complete
        {}
}

static uint32_t pl022_flash_read_data(uint32_t const base_addr)
{
    uint32_t data = 0x00000000;
    uint32_t addr =  base_addr+GSPI_SSPDR;

    iowrite8(SPIFLASH_READDATA, addr); //write data to SPI - command read
    for (int i =0; i<3; i++)
        iowrite8(0x00, addr);//write data to SPI - write address

    for (int i =0; i<4; i++)
        iowrite8(0xFF, addr);//write data to SPI - write address
    while((ioread8(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        {}

    for (int i =0; i<4; i++)
        (void)ioread8(addr); //read data from SPI - staff

    data = ioread8(addr);
    data = data | (ioread8(addr) << 8);
    data = data | (ioread8(addr) << 16);
    data = data | (ioread8(addr) << 24);
    return data;
}

static uint32_t gspi_ssp_flash(uint32_t const base_addr)
{
    //Read and check ID
    unsigned gspi_sspid;
    gspi_sspid =  (ioread32(base_addr+0xffc) & 0x000000ff)         | ((ioread32(base_addr+0xff8) << 8)  & 0x0000ff00) |
                  ((ioread32(base_addr+0xff4) << 16) & 0x00ff0000) | ((ioread32(base_addr+0xff0) << 24) & 0xff000000);

    if (gspi_sspid != 0x0df005b1)
    {
        rumboot_printf("ID read error! ID from SSP: 0x%x\n", gspi_sspid);
        return 1;
    }

    uint32_t read_data;

    pl022_dma_set_irq_mask(base_addr, ssp_int); //mask all interrupts except SSP


    rumboot_printf("Check SPI interrupt with loop operation\n");
    ssp_param.loopback = true;
    pl022_set_param(base_addr, &ssp_param); //turn on SSP controller, loop opera
    pl022_set_int_mask(base_addr, 0x02); //interrupt masks - unmask rx_fifo not empty

    pl022_send_word(base_addr, TEST_DATA_LOOP); //write data to SPI with IRQ

    if (wait_gspi_int())
    {
        rumboot_printf("SPI interrupt timeout\n");
        return 1;
    }


    if (SSP_data != TEST_DATA_LOOP)
    {
        rumboot_printf("SPI loop read error, read data = 0x%x\n", SSP_data);
        return 1;
    }


    rumboot_printf("Check read and write data to spi flash\n");
    ssp_param.loopback = false;
    pl022_set_param(base_addr, &ssp_param); //turn on SSP controller, loop opera
    pl022_set_int_mask(base_addr, 0x0); //mask all interrupts

    pl022_flash_write_enable(base_addr);
    pl022_flash_erase(base_addr);

    pl022_flash_write_enable(base_addr);
    pl022_flash_write_data(base_addr, TEST_DATA);

    //Read and check data from spi flash
    read_data = pl022_flash_read_data(base_addr);
    msync();

    if (read_data != TEST_DATA)
    {
        rumboot_printf("read_data = %x\n", read_data);
        rumboot_printf("SPI data read fail\n");
        return 1;
    }
    return 0;
}


int main(void)
{
    uint32_t test_result = 0;

    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("GSPI test has been started ... \n");

    #ifdef GSPI_CHECK_REGS
        rumboot_printf("Checking GSPI registers ...\n");
        test_result += gspi_check_regs(GSPI_BASE);

        rumboot_printf("Checking GSPI interrupt ...\n");
        (void)(ioread32(GSPI_BASE + GSPI_STATUS)); // clear interrupt
        gpio_set_port_direction(GPIO_1_BASE, GPIO1_X);
        iowrite32( GPIO1_X, GPIO_1_BASE + GPIO_ADDR_MASK );
        msync();
        TEST_ASSERT((ioread32(GSPI_BASE + GSPI_STATUS) == 0x82), "ERROR!!! GSPI interrupt not set");
        gpio_set_port_direction(GPIO_1_BASE, 0x0);
        (void)(ioread32(GSPI_BASE + GSPI_STATUS)); // clear interrupt
    #endif

    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, GSPI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, gspi_irq_handler, (void*)0);

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( GSPI_INT );
    rumboot_irq_sei();

    pl022_dma_reset(GSPI_BASE);
    //initial ssp
    conf.ssp_clk = 100000000;
    conf.spi_clk = 20000000;
    conf.data_size = 8;
    conf.soft_cs = 0;
    pl022_init(GSPI_BASE, &conf);

    //set ssp parameters
    ssp_param.cpol      = 1;
    ssp_param.cpha      = 1;
    ssp_param.mode      = master_mode;
    ssp_param.loopback  = false;
    ssp_param.fr_format = ssp_motorola_fr_form;
    pl022_set_param(GSPI_BASE, &ssp_param);//turn on SSP controller


    // Data for check IM2
    memset((void*)(IM2_BASE+4), TEST_DATA_IM2, BYTE_NUMBER);
    //IM2 - IM2
    iowrite32(PAGE_PROGRAM_COMMAND, IM2_BASE);
    msync();
    test_result += gspi_dma_axi(GSPI_BASE, IM2_BASE, (IM2_BASE + DATA_SIZE), TEST_DATA_IM2);
    //IM2 - IM1
    iowrite32(PAGE_PROGRAM_COMMAND, IM2_BASE);
    msync();
    test_result += gspi_dma_axi(GSPI_BASE, IM2_BASE, (IM1_BASE + DATA_SIZE), TEST_DATA_IM2);
    //IM2 - EM2
    iowrite32(PAGE_PROGRAM_COMMAND, IM2_BASE);
    msync();
    test_result += gspi_dma_axi(GSPI_BASE, IM2_BASE, (SRAM0_BASE + DATA_SIZE), TEST_DATA_IM2);


    // Data for check IM1
    memset((void*)(IM1_BASE+4), TEST_DATA_IM1, BYTE_NUMBER);
    //IM1 - IM2
    iowrite32(PAGE_PROGRAM_COMMAND, IM1_BASE);
    msync();
    test_result += gspi_dma_axi(GSPI_BASE, IM1_BASE, (IM2_BASE + DATA_SIZE), TEST_DATA_IM1);
    //IM1 - IM1
    iowrite32(PAGE_PROGRAM_COMMAND, IM1_BASE);
    msync();
    test_result += gspi_dma_axi(GSPI_BASE, IM1_BASE, (IM1_BASE + DATA_SIZE), TEST_DATA_IM1);
    //IM1 - EM2
    iowrite32(PAGE_PROGRAM_COMMAND, IM1_BASE);
    msync();
    test_result += gspi_dma_axi(GSPI_BASE, IM1_BASE, (SRAM0_BASE + DATA_SIZE), TEST_DATA_IM1);


    // Data for check EM2
    memset((void*)(SSRAM_BASE+4), TEST_DATA_EM2, BYTE_NUMBER);
    //EM2 - IM2
    iowrite32(PAGE_PROGRAM_COMMAND, SSRAM_BASE);
    msync();
    test_result += gspi_dma_axi(GSPI_BASE, SSRAM_BASE, (IM2_BASE + DATA_SIZE), TEST_DATA_EM2);
    //EM2 - IM1
    iowrite32(PAGE_PROGRAM_COMMAND, SSRAM_BASE);
    msync();
    test_result += gspi_dma_axi(GSPI_BASE, SSRAM_BASE, (IM1_BASE + DATA_SIZE), TEST_DATA_EM2);
    //EM2 - EM2
    iowrite32(PAGE_PROGRAM_COMMAND, SSRAM_BASE);
    msync();
    test_result += gspi_dma_axi(GSPI_BASE, SSRAM_BASE, (SRAM0_BASE + DATA_SIZE), TEST_DATA_EM2);


    IRQ = 0;
    pl022_dma_reset(GSPI_BASE);
    rumboot_printf("Checking GSPI FLASH read/write functionality ...\n");
    test_result += gspi_ssp_flash(GSPI_BASE);


    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);

    return test_result;
}
