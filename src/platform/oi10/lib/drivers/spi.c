#include <stdint.h>
#include <stdlib.h>

#include <platform/oi10/platform/devices/spi.h>
#include <platform/oi10/platform/regs/regs_spi.h>
#include <platform/oi10/platform/common_macros/common_macros.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>


void spi_eeprom_write_enable(uint32_t base_addr)
{
    iowrite32(0x06, base_addr+GSPI_SSPDR); //write data to SPI - command write enable

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR);  //read data from rx fifo
}

uint32_t read_eeprom_status (uint32_t base_addr)
{

//  MEM(S_GPIO_BASE+0x3fc) = 0xd0;    //start operation
    iowrite32(0x05, base_addr+GSPI_SSPDR); //write data to SPI - command read status
    iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - data staff

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

//  MEM(S_GPIO_BASE+0x3fc) = 0xf0;    //stop operation
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo

    return (ioread32(base_addr+GSPI_SSPDR)); //read data from rx fifo
}

void spi_eeprom_erase_init(uint32_t base_addr)
{
    iowrite32(0xD8, base_addr+GSPI_SSPDR); //sector erase command
    iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - write address
    iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - write address
    iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - write address

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo

    while ((read_eeprom_status(base_addr) & 0x1) == 0x1) //wait write complete
        ;
}

void spi_eeprom_write_data(uint32_t base_addr, uint32_t mem_addr, uint32_t data)
{
    iowrite32(0x02, base_addr+GSPI_SSPDR); //write data to SPI - command write
    iowrite32((mem_addr & 0x000000FF), base_addr+GSPI_SSPDR); //write data to SPI - write address
    iowrite32((mem_addr & 0x0000FF00) >> 8, base_addr+GSPI_SSPDR); //write data to SPI - write address
    iowrite32((mem_addr & 0x00FF0000) >> 16, base_addr+GSPI_SSPDR); //write data to SPI - write address
    iowrite32((data & 0x000000FF), base_addr+GSPI_SSPDR); //write data to SPI - write data 1
    iowrite32((data & 0x0000FF00) >> 8, base_addr+GSPI_SSPDR); //write data to SPI - write data 2
    iowrite32((data & 0x00FF0000) >> 16, base_addr+GSPI_SSPDR); //write data to SPI - write data 3
    iowrite32((data & 0xFF000000) >> 24, base_addr+GSPI_SSPDR); //write data to SPI - write data 4

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo

    while ((read_eeprom_status(base_addr) & 0x1) == 0x1) //wait write complete
        ;
}

uint32_t spi_eeprom_read_data(uint32_t base_addr)
{
        uint32_t data;
        iowrite32(0x03, base_addr+GSPI_SSPDR); //write data to SPI - command read
        iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - read address
        iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - read address
        iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - read address
        iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - staff
        iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - staff
        iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - staff
        iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - staff

        while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
            ;

        (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
        (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
        (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
        (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo

        data = ioread32(base_addr+GSPI_SSPDR);
        data = data | (ioread32(base_addr+GSPI_SSPDR) << 8);
        data = data | (ioread32(base_addr+GSPI_SSPDR) << 16);
        data = data | (ioread32(base_addr+GSPI_SSPDR) << 24);
    return data;
}










/*
int ssp_send_word(uint32_t base_address, uint16_t word){
    //check transmit FIFO full
    if(SSP_read_SSP_SR(base_address) & TNF){ // transmit FIFO is not full
        SSP_write_SSP_DR(base_address, word);
        return true;
    } else {
        trace_msg("Warning: writing to SSP TFIFO when TFIFO is full\n");
        return false;
    }
}

int ssp_get_word(uint32_t base_address, uint16_t * word){
    //check receive FIFO empty
    if(SSP_read_SSP_SR(base_address) & RNE){ //receive FIFO is not empty
        *word = SSP_read_SSP_DR(base_address);
        return true;
    } else {
        trace_msg("Warning: reading from SSP RFIFO when RFIFO is empty\n");
        return false;
    }
}

void ssp_set_int_mask(uint32_t base_address, uint16_t mask){
    SSP_write_SSP_IMSC(base_address,mask);
    TEST_ASSERT((SSP_read_SSP_IMSC(base_address) == mask), "SSP_IMSC not set");
}

uint16_t ssp_get_ris(uint32_t base_address){
    return SSP_read_SSP_RIS(base_address);
}

uint16_t ssp_get_mis(uint32_t base_address){
    return SSP_read_SSP_MIS(base_address);
}

uint16_t ssp_get_status(uint32_t base_address){
    return SSP_read_SSP_SR(base_address);
}

void ssp_dma_enable(uint32_t base_address, bool enabled){
    if(enabled){
        SSP_write_SSP_DMACR(base_address, (TXDMAE | RXDMAE));//enable DMA tx and rx FIFO
        TEST_ASSERT((SSP_read_SSP_DMACR(base_address) == (TXDMAE | RXDMAE)), "SSP_DMACR not set");
    }else{
        SSP_write_SSP_DMACR(base_address,0x0);
        TEST_ASSERT((SSP_read_SSP_DMACR(base_address) == 0x0),"SSP_DMACR not set");
    }
}

void ssp_set_clock_rate(uint32_t base_address, uint32_t clk_rate){
    //NOTE: CR0 should be zero
    SSP_write_SSP_CPSR(base_address,CPSDVR);//set divider
    int scr = (SSP_SYS_FREQ_HZ / (clk_rate * 2)) - 1;
    SSP_write_SSP_CR0(base_address,scr << SCR_SHIFT);//set SCR
    TEST_ASSERT((SSP_read_SSP_CR0(base_address) == (scr << SCR_SHIFT)),"SSP_CR0 not set");
}

void ssp_disable(uint32_t base_address){
    SSP_clear_SSP_CR1(base_address, SSE);//clear enable bit
    TEST_ASSERT(((SSP_read_SSP_CR1(base_address) & SSE) == false), "SSP_CR1 not set");
}

void ssp_init(uint32_t base_address, ssp_params params){
    //disable ssp
    SSP_write_SSP_CR1(base_address,0x0);
    TEST_ASSERT((SSP_read_SSP_CR1(base_address) == 0x0),"SSP_CR1 not set");
    SSP_write_SSP_CR0(base_address,0x0000);
    TEST_ASSERT((SSP_read_SSP_CR0(base_address) == 0x0000),"SSP_CR0 not set");
    //setup the parameters
    //clock rate
    ssp_set_clock_rate(base_address,params.clock_rate);
    //cpol, cpha, format
    SSP_set_SSP_CR0(base_address,
            ((params.cpha & 0x1) << SPH_SHIFT) |
            ((params.cpol & 0x1) << SPO_SHIFT) |
            (params.fr_format << FRF_SHIFT) |
            (params.data_size << DSS_SHIFT));
    TEST_ASSERT((SSP_read_SSP_CR0(base_address) ==
            (((params.cpha & 0x1) << SPH_SHIFT) |
            ((params.cpol & 0x1) << SPO_SHIFT) |
            (params.fr_format << FRF_SHIFT) |
            (params.data_size << DSS_SHIFT))),"SSP_CR0 not set");

    //mode, loopback, enable core
    SSP_set_SSP_CR1(base_address,
            ((params.loopback & 0x1) << LBM_SHIFT) |
            (params.mode << MS_SHIFT) |
            SSE);
    TEST_ASSERT(((SSP_read_SSP_CR1(base_address) & 0xF) ==
            (((params.loopback & 0x1) << LBM_SHIFT) |
            (params.mode << MS_SHIFT) |
            SSE)), "SSP_CR1 not set");
}
*/
