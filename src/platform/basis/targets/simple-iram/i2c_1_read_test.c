
//-----------------------------------------------------------------------------
//  This program is for I2C controller read functionality
//    Test includes:
//    - set I2C: 1MHz, filter On
//    - repeat few times
//      - turn i2c_1 On
//      - write 8 bytes to external EEPROM
//      - read 8 bytes with "Current Address" mode
//      - compare with etalon
//      - read 8 bytes in "Sequential" mode
//      - compare with etalon
//      - turn i2c_1 Off

//    Test duration (RTL): < 1.1 ms
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>

#include <platform/devices.h>
#include <regs/regs_i2c.h>


void delay_cycle (volatile uint32_t delay_value)
{
    for (volatile uint32_t i = 0; i < delay_value; i++)
        ;
}

//-----------------------------------------------------------------------------
//  Functions for further use. Probably will form some library.
//-----------------------------------------------------------------------------
static inline void i2c_1_turn_on ()
{
    uint32_t rdata;
    rdata = ioread32 (I2C1_BASE + I2C_CTRL);
    iowrite32 (rdata | 0x01, I2C1_BASE + I2C_CTRL);
}

static inline void i2c_1_turn_off ()
{
    uint32_t rdata;
    rdata = ioread32 (I2C1_BASE + I2C_CTRL);
    iowrite32 (rdata & (~0x01), I2C1_BASE + I2C_CTRL);
}

static inline void i2c_1_start ()
{
    uint32_t rdata;
    rdata = ioread32 (I2C1_BASE + I2C_CTRL);
    iowrite32 (rdata | 0x02, I2C1_BASE + I2C_CTRL);
}

static inline void i2c_1_stop ()
{
    uint32_t rdata;
    rdata = ioread32 (I2C1_BASE + I2C_CTRL);
    iowrite32 (rdata | 0x40, I2C1_BASE + I2C_CTRL);
}

static inline void i2c_1_read ()
{
    uint32_t rdata;
    rdata = ioread32 (I2C1_BASE + I2C_CTRL);
    iowrite32 (rdata | 0x08, I2C1_BASE + I2C_CTRL);
}

static inline void i2c_1_write ()
{
    uint32_t rdata;
    rdata = ioread32 (I2C1_BASE + I2C_CTRL);
    iowrite32 (rdata | 0x10, I2C1_BASE + I2C_CTRL);
}

static inline void i2c_1_repeat ()
{
    uint32_t rdata;
    rdata = ioread32 (I2C1_BASE + I2C_CTRL);
    iowrite32 (rdata | 0x20, I2C1_BASE + I2C_CTRL);
}

static inline void i2c_1_few_bits_read ()
{
    uint32_t rdata;
    rdata = ioread32 (I2C1_BASE + I2C_CTRL);
    iowrite32 (rdata | 0x6B, I2C1_BASE + I2C_CTRL);
}


//-------------------------------------------------------------
//  [0]        filter bypass (1 - On, 0 - Off)
//  [10..1]    filter shift register mask
//               3F - 6 bits are used - for 100MHz APB clk
//-------------------------------------------------------------
void i2c_1_filter_on ()
{
    iowrite32 (0x7E, I2C1_BASE + I2C_FILTR);
}
//-----------------------------------------------------------------------------


static inline void i2c_1_set_freq_1m ()
{
    iowrite32 (0x00000013, I2C1_BASE + I2C_PRESCALE);
}
//-----------------------------------------------------------------------------


static inline void i2c_1_generate_stop ()
{
    iowrite32 (1, I2C1_BASE + I2C_STAT_RST);
    iowrite32 (0, I2C1_BASE + I2C_STAT_RST);
    i2c_1_stop ();
    while ((ioread32 (I2C1_BASE + I2C_STATUS) & 0x400) == 0)
        ;
}
//-----------------------------------------------------------------------------


void i2c_1_store_data_out (uint32_t wdata)
{
    iowrite32 (wdata, I2C1_BASE + I2C_TRANSMIT);
}

uint32_t i2c_1_load_data_in ()
{
    return ioread32 (I2C1_BASE + I2C_RECEIVE);
}

void i2c_1_set_read_number (uint32_t read_number)
{
    iowrite32 (read_number, I2C1_BASE + I2C_NUMBER);
}
//-----------------------------------------------------------------------------


uint32_t data_read_1byte ()
{
    uint32_t rdata;
    //-----------------------------------------------------------------
    //  Current address read
    //-----------------------------------------------------------------
    i2c_1_set_read_number (1);
    
    //  I2C slave Address
    i2c_1_store_data_out (0xA0 | 0x1);
    i2c_1_start ();
    i2c_1_stop ();
    i2c_1_read ();     //  I2C transaction starts after this command
    //  Wait full data read
    while ((ioread32 (I2C1_BASE + I2C_ISR) & 0x80) == 0)
        ;
    
    rdata = i2c_1_load_data_in ();
    
    return rdata;
}

void data_read_bytes (uint32_t *rdata, uint32_t number_bytes)
{
    //-----------------------------------------------------------------
    //  Current address read
    //-----------------------------------------------------------------
    i2c_1_set_read_number (number_bytes);
    
    //  I2C slave Address
    i2c_1_store_data_out (0xA0 | 0x1);
    i2c_1_start ();
    i2c_1_repeat ();
    i2c_1_stop ();
    i2c_1_read ();     //  I2C transaction starts after this command
    //  Wait full data read
    while ((ioread32 (I2C1_BASE + I2C_ISR) & 0x80) == 0)
        ;
    
    for (uint32_t i = 0; i < number_bytes; i++)
        rdata [i] = i2c_1_load_data_in ();
    
}

void data_write ()
{
    //-----------------------------------------------------------------
    //  Few bytes write
    //-----------------------------------------------------------------
    //  I2C slave Address
    i2c_1_store_data_out (0xA0);
    //  I2C wdata
    i2c_1_store_data_out (0x00);
    i2c_1_store_data_out (0x00);
    i2c_1_store_data_out (0x01);
    i2c_1_store_data_out (0x03);
    i2c_1_store_data_out (0x07);
    i2c_1_store_data_out (0x0F);
    i2c_1_store_data_out (0x1F);
    i2c_1_store_data_out (0x3F);
    i2c_1_store_data_out (0x7F);
    i2c_1_store_data_out (0xFF);
    
    
    i2c_1_start ();
    i2c_1_write ();     //  I2C transaction starts after this command
    while ((ioread32 (I2C1_BASE + I2C_STATUS) & 0x10) == 0)
        ;
    iowrite32 (1, I2C1_BASE + I2C_STAT_RST);
    iowrite32 (0, I2C1_BASE + I2C_STAT_RST);
    i2c_1_stop ();
    while ((ioread32 (I2C1_BASE + I2C_STATUS) & 0x400) == 0)
        ;
}

void EEPROM_set_addr (uint32_t addr)
{
    //-----------------------------------------------------------------
    //  Set read address
    //    It must be done before usual read
    //-----------------------------------------------------------------
        //  I2C slave Address
        i2c_1_store_data_out (0xA0);
        //  I2C wdata
        //  Here it will be used by I2C slave as 16 bit address
        i2c_1_store_data_out (addr);
        i2c_1_store_data_out (addr >> 8);
        i2c_1_start ();
        i2c_1_write ();     //  I2C transaction starts after this command
        while ((ioread32 (I2C1_BASE + I2C_STATUS) & 0x10) == 0)
            ;
}

uint32_t check_rdata_array (uint32_t *read_array)
{
    if ((read_array [0] != 0x01) 
       | (read_array [1] != 0x03) 
       | (read_array [2] != 0x07) 
       | (read_array [3] != 0x0F) 
       | (read_array [4] != 0x1F) 
       | (read_array [5] != 0x3F) 
       | (read_array [6] != 0x7F) 
       | (read_array [7] != 0xFF))
        return -1;
    return 0;
}

uint32_t i2c_write_read_test ()
{
    uint32_t read_array [32];
    //-----------------------------------------------------------------
    //  Frequency changing
    //    Its OK. Checked with signals values.
    //-----------------------------------------------------------------
    for (uint32_t i = 0; i <= 1; i++)
    {
        i2c_1_set_freq_1m ();
        i2c_1_filter_on ();
        i2c_1_turn_on ();
        data_write ();
        
        rumboot_printf("write OK\n");	  
        //  Not mandatory delay
        delay_cycle (100);
        
        //---------------------------------------------------------
        //  read 8 bytes with "Current Address" mode
        //---------------------------------------------------------
        EEPROM_set_addr (0x0000);
        i2c_1_generate_stop ();
        for (uint32_t j = 0; j < 8; j++)
            read_array [j] = data_read_1byte ();
        
        if (check_rdata_array (read_array) != 0) return -1;
        rumboot_printf("read_0 OK\n");	  
        
        //  Not mandatory delay
        delay_cycle (100);
        
        //---------------------------------------------------------
        //  read 8 bytes in "Sequential" mode
        //---------------------------------------------------------
        EEPROM_set_addr (0x0000);
        data_read_bytes (read_array, 8);
        
        if (check_rdata_array (read_array) != 0) return -1;
        rumboot_printf("read_1 OK\n");	  
        
        //---------------------------------------------------------
        //  This delay is mandatory.
        //    Othewise there will be some garbage on I2C bus
        //---------------------------------------------------------
        delay_cycle (10);
        i2c_1_turn_off ();
    }
    
    return 0;
}

//-------------------------------------------------------------------------
//  Test sequence
//-------------------------------------------------------------------------
uint32_t main ()
{
    if (i2c_write_read_test () != 0)
        return -1;

    return 0;
}

