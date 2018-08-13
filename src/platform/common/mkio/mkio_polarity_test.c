
//-----------------------------------------------------------------------------
//  This program is for testing Gaisler MKIO polarity change features
//    Test includes:
//    - 
//    - 
//    - 
//    - 
//    - 
//    - 
//    - 
//    - 
//
//
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/devices.h>
#include <regs/regs_mkio.h>
#include <devices/mkio.h>

//  Single Array Data Size In Bytes
#define DATA_SIZE   2




//-----------------------------------------------------------------------------
//  Common functions
//-----------------------------------------------------------------------------
void delay_cycle (volatile uint32_t delay_value)
{
    for (volatile uint32_t i = 0; i < delay_value; i++)
        ;
}

//---------------------------------------------------------------------
//  Create reference data array for next transactions
//      size in bytes
//---------------------------------------------------------------------
void create_etalon_array (uint32_t* address, uint32_t size)
{
    rumboot_printf("create_etalon_array  %x\n", address);
    for (uint32_t i = 0; i < (size >> 2); i++)
    {
        uint32_t seed = i*4;
        *address = ((seed+0)<<0) + ((seed+1)<<8) + ((seed+2)<<16) + ((seed+3)<<24);
        address++;
    }
}

//---------------------------------------------------------------------
//  Erase destination space.
//  Useful for repeating tests.
//      size in bytes
//---------------------------------------------------------------------
void clear_destination_space (uint32_t* address, uint32_t size)
{
    rumboot_printf("clear_destination_space  %x\n", address);
    for (uint32_t i = 0; i < (size >> 2); i++)
    {
        *address = 0xDEADBEEF;
        address++;
    }
}

uint32_t cmp_arrays (uint32_t address_src, uint32_t address_dst, uint32_t size)
{
    rumboot_printf("cmp_arrays  src %x,  dst %x,  size %d bytes\n", address_src, address_dst, size);
    for (uint32_t i = 0; i < size; i+=4)
    {
        if (*(uint32_t*)(address_src + i) != *(uint32_t*)(address_dst + i))
            return -1;
    }
    return 0;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Test sequence
//-----------------------------------------------------------------------------
uint32_t main ()
{
    uint32_t data_src [DATA_SIZE >> 2] __attribute__ ((aligned(4)));
    uint32_t data_mid [DATA_SIZE >> 2] __attribute__ ((aligned(4)));
    uint32_t data_dst [DATA_SIZE >> 2] __attribute__ ((aligned(4)));
    
    rumboot_printf("    mkio_write_read_test\n");
    
    clear_destination_space (data_src, DATA_SIZE);
    clear_destination_space (data_mid, DATA_SIZE);
    clear_destination_space (data_dst, DATA_SIZE);
    
    create_etalon_array (data_src, DATA_SIZE);
    
    if (mkio_present (MKIO0_BASE) != 0)
        return -1;
    if (mkio_present (MKIO1_BASE) != 0)
        return -2;

    //  This cycle is for checking our polarity scheme.
    //  Has no functional meaning for program.
    for (uint32_t i = 0; i <= 31; i++)
    {
        mkio_set_polarity ((1 << i), MKIO0_BASE);
        mkio_set_polarity ((1 << i), MKIO1_BASE);
    }

    //  Change BC and RT data signals polarity
    mkio_set_polarity (0x0F, MKIO0_BASE);
    mkio_set_polarity (0x0F, MKIO1_BASE);
    
    if (mkio_write_to_rt ((uint32_t) (&data_src), (uint32_t) (&data_mid), DATA_SIZE, MKIO0_BASE, MKIO1_BASE) != 0)
        return -3;
    cmp_arrays ((uint32_t) (&data_src), (uint32_t) (&data_mid), DATA_SIZE);
    rumboot_printf("mkio_write_to_rt OK\n");

    if (mkio_read_from_rt ((uint32_t) (&data_mid), (uint32_t) (&data_dst), DATA_SIZE, MKIO0_BASE, MKIO1_BASE) != 0)
        return -4;
    cmp_arrays ((uint32_t) (&data_mid), (uint32_t) (&data_dst), DATA_SIZE);
    rumboot_printf("mkio_read_from_rt OK\n");


    return 0;
}

