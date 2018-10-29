#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/oi10/platform/regs/regs_spi.h>
#include <platform/oi10/platform/devices/spi.h>

#include <rumboot/irq.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <platform/regs/fields/mpic128.h>
#include <rumboot/regpoker.h>
#include <rumboot/platform.h>
#include <regs/regs_gpio_pl061.h>
#include <devices/gpio_pl061.h>

#define TEST_DATA       0xDCFE3412

#ifdef GSPI_CHECK_REGS

static uint32_t gspi_check_regs(uint32_t base_addr)
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

static volatile uint32_t IRQ;
static volatile uint32_t SSP_data;
static volatile uint32_t DMA_write_buffer_end;

static void gspi_irq_handler( int irq, void *arg )
{
    uint32_t ssp_status = ioread32(GSPI_BASE + GSPI_SSPSR);
    uint32_t dma_status = ioread32(GSPI_BASE + GSPI_STATUS);

    rumboot_printf( "IRQ arrived  \n" );
    rumboot_printf("GSPI SSPSR is 0x%x\n", ssp_status);
    rumboot_printf("GSPI STATUS is 0x%x\n", dma_status);

    if (ssp_status & 0x4)
        SSP_data = ioread32(GSPI_BASE + GSPI_SSPDR);

    if (dma_status & 0x4)
        DMA_write_buffer_end = 1;

    rumboot_printf("Clear interrupts\n");
    iowrite32(0x0F, GSPI_BASE + GSPI_SSPICR);
    IRQ = 1;
}

static uint32_t wait_gspi_int(){
    unsigned t;

    //rumboot_printf ("wait_spi_int \n");

    for (t=1; t<=GSPI_TIMEOUT; t++){
        if (IRQ)
        {
            IRQ = 0;
            break;
        }
    }
    if (t>=GSPI_TIMEOUT) {
        rumboot_printf("Error! IRQ flag wait timeout! \n");
        return 1;
    }
    return 0;
}

static uint8_t data_src[] = { 0x02, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04 };

static uint8_t data_dst[4];

static uint32_t gspi_dma_axi(uint32_t base_addr, uint32_t* r_mem_addr, uint32_t* w_mem_addr)
{
    rumboot_printf("Read from address =%x\n", r_mem_addr);
    rumboot_printf("Write to address =%x\n", w_mem_addr);

    gspi_reset(base_addr);

    iowrite32(0xc7, base_addr+GSPI_SSPCR0); //set 8-bit data
    iowrite32(0x02, base_addr+GSPI_SSPCR1); //turn on SSP controller
    iowrite32(0x06, base_addr+GSPI_SSPCPSR); //clock prescale
    iowrite32(0x02, base_addr+GSPI_SSPIMSC); //interrupt masks - unmask rx_fifo not empty


    spi_eeprom_write_enable(base_addr);
    spi_eeprom_erase_init(base_addr);


    spi_eeprom_write_enable(base_addr);

//    gspi_set_irq_mask(base_addr, 0x04);

    iowrite32(0x04, base_addr + GSPI_IRQMASKS); //set irq masks

//    gspi_dma_enable(base_addr);
    iowrite32(0x03, base_addr + GSPI_SSPDMACR); //enable DMA

    iowrite32(0x01, base_addr + GSPI_AXIR_BUFTYPE); //base mode
    iowrite32(0x108, base_addr + GSPI_AXI_PARAMS); //AXI params

    iowrite32((uint32_t)r_mem_addr, base_addr + GSPI_DMARSTART);
    iowrite32((uint32_t)((uint8_t*)(r_mem_addr) + 7), base_addr + GSPI_DMAREND);

    iowrite32((uint32_t)(w_mem_addr), base_addr + GSPI_DMAWSTART);
    iowrite32((uint32_t)((uint8_t*)(w_mem_addr) + 3), base_addr + GSPI_DMAWEND);

    iowrite32(0xDFFFFFE0, base_addr + GSPI_DMARCNTRL);
    iowrite32(0xDFFFFFE0, base_addr + GSPI_DMAWCNTRL);

    iowrite32(0x01, base_addr + GSPI_AXIR_BUFENA); //enable transmitting

    while(!DMA_write_buffer_end)
        ;
    DMA_write_buffer_end = 0;

    rumboot_putstring("Reading data...\n");
    (void)spi_eeprom_read_data(base_addr);
    rumboot_printf("Read data from memory =%x\n", ioread32((uint32_t)w_mem_addr));
    rumboot_putstring("Checking data ...\n");
    if (ioread32((uint32_t)w_mem_addr) != 0x01020304)
    {
        rumboot_putstring("GSPI AXI: SPI data read fail\n");
        return 1;
    }
    rumboot_putstring("Check GSPI AXI: OK\n");
    return 0;
}

static uint32_t gspi_ssp_eeprom(uint32_t base_addr)
{
    //Read and check ID
    unsigned gspi_sspid;
    gspi_sspid =  (ioread32(base_addr+0xffc) & 0x000000ff)         | ((ioread32(base_addr+0xff8) << 8)  & 0x0000ff00) |
                  ((ioread32(base_addr+0xff4) << 16) & 0x00ff0000) | ((ioread32(base_addr+0xff0) << 24) & 0xff000000);

    if (gspi_sspid != 0x0df005b1) {
        rumboot_printf("ID read error! ID from SSP: 0x%x\n", gspi_sspid);
        return 1;
    }


//    gspi_set_irq_mask(base_addr, 0x01); //mask all interrupts except SSP
    iowrite32(0x01, base_addr + GSPI_IRQMASKS); //mask all interrupts except SSP

    //Check SPI interrupt with loop operation
    iowrite32(0xc7, base_addr+GSPI_SSPCR0); //set 8-bit data
    iowrite32(0x03, base_addr+GSPI_SSPCR1); //turn on SSP controller, loop operation
    iowrite32(0x06, base_addr+GSPI_SSPCPSR); //clock prescale
    iowrite32(0x02, base_addr+GSPI_SSPIMSC); //interrupt masks - unmask rx_fifo not empty

    iowrite32(0x5a, base_addr+GSPI_SSPDR); //write data to SPI with IRQ

    if (wait_gspi_int())
    {
        rumboot_printf("SPI interrupt timeout\n");
        return 1;
    }

    if (SSP_data != 0x5a)
    {
        rumboot_printf("SPI loop read error\n");
        return 1;
    } //Read and check data


    //Write data to spi eeprom
    iowrite32(0x02, base_addr+GSPI_SSPCR1); //SSP controller from loop to normal operation
    iowrite32(0x00, base_addr+GSPI_SSPIMSC); //mask all interrupts

    spi_eeprom_write_enable(base_addr);
    spi_eeprom_erase_init(base_addr);

    spi_eeprom_write_enable(base_addr);
    spi_eeprom_write_data(base_addr, 0x00, TEST_DATA);


    //Read and check data from spi slash
    if (spi_eeprom_read_data(base_addr) != TEST_DATA)
    {
        rumboot_printf("SPI data read fail\n");
        return 1;
    }
    return 0;
}


int main(void)
{
    uint32_t test_result = 0;

    rumboot_printf("GSPI test has been started ... \n");

    #ifdef GSPI_CHECK_REGS
        rumboot_printf("Checking GSPI interrupt ...\n");
        gpio_set_port_direction(GPIO_1_BASE, GPIO1_X);
        iowrite32( GPIO1_X, GPIO_1_BASE + GPIO_ADDR_MASK );
        TEST_ASSERT((ioread32(GSPI_BASE + GSPI_STATUS) == 0x82), "ERROR!!! GSPI interrupt not set");
        gpio_set_port_direction(GPIO_1_BASE, 0x0);

        rumboot_printf("Checking GSPI registers ...\n");
        test_result += gspi_check_regs(GSPI_BASE);
    #endif

    IRQ = 0;
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, GSPI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, gspi_irq_handler, (void*)0);

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( GSPI_INT );
    rumboot_irq_sei();


    //IM0 - IM0
    test_result += gspi_dma_axi(GSPI_BASE, (uint32_t*)data_src, (uint32_t*)data_dst);
    //IM0 - IM1
//    test_result += gspi_dma_axi(GSPI_BASE, (uint32_t*)data_src, (uint32_t*)rumboot_virt_to_dma((void*)IM1_BASE));
    //IM0 - EM2
//    test_result += gspi_dma_axi(GSPI_BASE, (uint32_t*)data_src, (uint32_t*)rumboot_virt_to_dma((void*)EM2_BASE));

    // Copy data in IM1
//    memcpy(data_src, (uint32_t*)rumboot_virt_to_dma((uint32_t*)IM1_BASE), sizeof(data_src));
//    for(int i=0; i <8; i++)
//    {
//        rumboot_printf("data_src[%d] =0x%x\n", i, data_src[i]);
//        rumboot_printf("address = 0x%x\n", (uint32_t)data_src);
//        iowrite8(data_src[i], IM1_BASE+i);
//        rumboot_printf("Copy data =%x\n", ioread8(IM1_BASE+i));
//    }
    //IM1 - IM0
//    test_result += gspi_dma_axi(GSPI_BASE, (uint32_t*)rumboot_virt_to_dma((void*)IM1_BASE), (uint32_t*)data_dst);
    //IM1 - IM1
//    test_result += gspi_dma_axi(GSPI_BASE, (uint32_t*)rumboot_virt_to_dma((void*)IM1_BASE), (uint32_t*)rumboot_virt_to_dma((void*)IM1_BASE + 16));
//    //IM1 - EM2
//    test_result += gspi_dma_axi(GSPI_BASE, (uint32_t*)rumboot_virt_to_dma((void*)IM1_BASE), (uint32_t*)rumboot_virt_to_dma((void*)EM2_BASE));

    // Copy data in EM2
//    for(int i=0; i <8; i++)
//    {
//        rumboot_printf("data_src[%d] =0x%x\n", i, data_src[i]);
//        rumboot_printf("address = 0x%x\n", (uint32_t)data_src);
//        iowrite8(data_src[i], EM2_BASE+i);
//        rumboot_printf("Copy data =%x\n", ioread8(EM2_BASE+i));
//    }
//    //EM2 - IM0
//    test_result += gspi_dma_axi(GSPI_BASE, (uint32_t*)rumboot_virt_to_dma((void*)EM2_BASE), (uint32_t*)data_dst);
//    //EM2 - IM1
//    test_result += gspi_dma_axi(GSPI_BASE, (uint32_t*)rumboot_virt_to_dma((void*)EM2_BASE), (uint32_t*)rumboot_virt_to_dma((void*)IM1_BASE));
//    //EM2 - EM2
//    test_result += gspi_dma_axi(GSPI_BASE, (uint32_t*)rumboot_virt_to_dma((void*)EM2_BASE), (uint32_t*)rumboot_virt_to_dma((void*)EM2_BASE + 16));


    rumboot_printf("Checking GSPI EEPROM read/write functionality ...\n");
    test_result += gspi_ssp_eeprom(GSPI_BASE);

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);

//    rumboot_printf("GSPI test has been finished successfully.\n");
    return test_result;
}
