
//-----------------------------------------------------------------------------
//  This program is for testing Gaisler MKIO write/read functionality
//    Test includes:
//    - set 
//    - repeat few times
//      - 

//    Test duration (RTL): < TODO
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>

#include <platform/devices.h>
#include <regs/regs_mkio.h>
#include <devices/mkio.h>

//  Single Array Data Size In Bytes
#define DATA_SIZE   16




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
    rumboot_printf("cmp_arrays  src %x,  dst %x,  size %x\n", address_src, address_dst, size);
    for (uint32_t i = 0; i < size; i+=4)
    {
        if (*(uint32_t*)(address_src + i) != *(uint32_t*)(address_dst + i))
            return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------




//-------------------------------------------------------------------------
//  Device specific functions
//-------------------------------------------------------------------------
void mkio_bc_start_schedule (uint32_t base_address, uint32_t descriptor_pointer)
{
    //  Safety code (BCKEY) - Must be 0x1552 when writing, otherwise register write is ignored
    uint32_t BCKEY = 0x1552 ;
    //  Schedule start (SCSRT) - Write ‘1’ to start schedule
    uint32_t SCSRT = 0x1    ;
    //  Set external trigger (SETT) - Write ‘1’ to force the trigger memory to set
    // uint32_t SETT = 0x1     ;
    
    
    iowrite32 (descriptor_pointer,             base_address + BCTNP );
    iowrite32 ((BCKEY << 16) | (SCSRT << 0),   base_address + BCA   );
    // iowrite32 ((BCKEY << 16) | (SETT << 3 ),   base_address + BCA   );
}

void mkio_rt_start_schedule (uint32_t base_address, uint32_t subaddress_table_pointer)
{
    //  Safety code (RTKEY) - Must be written as 0x1553 when changing the RT address, otherwise the address
    //    field is unaffected by the write.
    uint32_t RTKEY  = 0x1553 ;
    //  RT Address (RTADDR) - This RT:s address (0-30)
    uint32_t RTADDR = 0x00   ;
    //  RT Enable (RTEN) - Set to ‘1’ to enable listening for requests
    uint32_t RTEN   = 0x1    ;
    
    
    iowrite32 (subaddress_table_pointer,                        base_address + RTSTBA );
    iowrite32 ((RTKEY << 16) | (RTADDR << 1) | (RTEN << 0),     base_address + RTC    );
}

uint32_t mkio_write_read (uint32_t data_src, uint32_t data_mid, uint32_t data_dst, uint32_t size, uint32_t bc_base_address, uint32_t rt_base_address)
{
    mkio_bc_descriptor         mkio_bc_descriptor        __attribute__ ((aligned(16)))  ;
    mkio_rt_subaddress_table   mkio_rt_subaddress_table  __attribute__ ((aligned(512))) ;
    mkio_rt_descriptor         mkio_rt_rx_descriptor     __attribute__ ((aligned(16)))  ;
    mkio_rt_descriptor         mkio_rt_tx_descriptor     __attribute__ ((aligned(16)))  ;
    
    //  Suspend normally (SUSN) - Always suspends after transfer
    uint32_t SUSN = 0x0;
    
    //  RT Address (RTAD1)
    uint32_t RTAD1 = 0x00;
    //  RT Subaddress (RTSA1)
    uint32_t RTSA1 = 0x01;
    //  0 - transmit (BC->RT), 1 - receive (RT->BC)
    uint32_t TR = 0;
    //  Word count/Mode code (WCMC)
    //  Word is 2 bytes in MKIO 1553
    uint32_t WCMC = size >> 1;
    
    //  Allow receive transfers to this subaddress
    uint32_t RXEN = 1;
    //  Allow transmit transfers from this subaddress
    uint32_t TXEN = 1;
    
    uint32_t bc_descriptor_end_of_list = 0x800000FF ;
    uint32_t rt_descriptor_end_of_list = 0x00000003 ;
    
    uint32_t unused = 0xDEADBEEF ;
    
    rumboot_printf("mkio_write_read parameters:\n");
    // rumboot_printf("    data_src %x\n",                 (uint32_t) (&data_src       ));
    // rumboot_printf("    data_mid %x\n",                 (uint32_t) (&data_mid       ));
    // rumboot_printf("    data_dst %x\n",                 (uint32_t) (&data_dst       ));
    // rumboot_printf("    mkio_bc_descriptor %x\n",       (uint32_t) (&mkio_bc_descriptor       ));
    // rumboot_printf("    mkio_rt_subaddress_table %x\n", (uint32_t) (&mkio_rt_subaddress_table ));
    // rumboot_printf("    mkio_rt_rx_descriptor %x\n",    (uint32_t) (&mkio_rt_rx_descriptor    ));
    
    // mkio_report_rt_status (rt_base_address);
    // mkio_report_bc_status (bc_base_address);

    //-----------------------------------------------------------------------------
    //  Write data to RT
    //-----------------------------------------------------------------------------
    rumboot_printf("write procedure\n");
    
    TR = 0;
    
    mkio_bc_descriptor.ctrl_word_0      = 0x00000000 | (SUSN << 25);
    mkio_bc_descriptor.ctrl_word_1      = 0x00000000 | (RTAD1 << 11) | (TR << 10) | (RTSA1 << 5) | (WCMC << 0) ;
    mkio_bc_descriptor.data_pointer     = data_src ;
    mkio_bc_descriptor.result_word      = 0xFFFFFFFF ;
    mkio_bc_descriptor.condition_word   = bc_descriptor_end_of_list;
    mkio_bc_descriptor.branch_address   = unused;
    mkio_bc_descriptor.reserved_0       = unused;
    mkio_bc_descriptor.reserved_1       = unused;
    
    mkio_rt_subaddress_table.sa0_ctrl_word             = unused ;
    mkio_rt_subaddress_table.sa0_tx_descriptor_pointer = unused ;
    mkio_rt_subaddress_table.sa0_rx_descriptor_pointer = unused ;
    mkio_rt_subaddress_table.sa0_reserved              = unused ;
    mkio_rt_subaddress_table.sa1_ctrl_word             = 0x00000000 | (RXEN << 15) | (TXEN << 7) ;
    mkio_rt_subaddress_table.sa1_tx_descriptor_pointer = (uint32_t) (&mkio_rt_tx_descriptor) ;
    mkio_rt_subaddress_table.sa1_rx_descriptor_pointer = (uint32_t) (&mkio_rt_rx_descriptor) ;
    mkio_rt_subaddress_table.sa1_reserved              = 0x00000000 ;
    
    mkio_rt_rx_descriptor.ctrl_status_word        = 0x03FFFFFF ;
    mkio_rt_rx_descriptor.data_pointer            = data_mid ;
    mkio_rt_rx_descriptor.next_descriptor_pointer = rt_descriptor_end_of_list ;
    
    mkio_rt_start_schedule (rt_base_address, (uint32_t) (&mkio_rt_subaddress_table));
    mkio_bc_start_schedule (bc_base_address, (uint32_t) (&mkio_bc_descriptor      ));
    
    while (((mkio_bc_descriptor.result_word & 0x7) != 0) | ((mkio_rt_rx_descriptor.ctrl_status_word & 0x7) != 0))
    {
        delay_cycle (10);
    }
    rumboot_printf("bc and rt descriptors closed successfully\n");
    
    cmp_arrays (data_src, data_mid, size);
    rumboot_printf("write OK\n");
    
    //-----------------------------------------------------------------------------
    //  Read data from RT
    //-----------------------------------------------------------------------------
    rumboot_printf("read procedure\n");
    
    TR = 1;
    
    mkio_bc_descriptor.ctrl_word_0      = 0x00000000 | (SUSN << 25);
    mkio_bc_descriptor.ctrl_word_1      = 0x00000000 | (RTAD1 << 11) | (TR << 10) | (RTSA1 << 5) | (WCMC << 0) ;
    mkio_bc_descriptor.data_pointer     = data_dst ;
    mkio_bc_descriptor.result_word      = 0xFFFFFFFF ;
    
    mkio_rt_tx_descriptor.ctrl_status_word        = 0x03FFFFFF ;
    mkio_rt_tx_descriptor.data_pointer            = data_mid ;
    mkio_rt_tx_descriptor.next_descriptor_pointer = rt_descriptor_end_of_list ;
    
    mkio_rt_start_schedule (rt_base_address, (uint32_t) (&mkio_rt_subaddress_table));
    mkio_bc_start_schedule (bc_base_address, (uint32_t) (&mkio_bc_descriptor      ));
    
    while (((mkio_bc_descriptor.result_word & 0x7) != 0) | ((mkio_rt_rx_descriptor.ctrl_status_word & 0x7) != 0))
    {
        delay_cycle (10);
    }
    rumboot_printf("bc and rt descriptors closed successfully\n");
    
    cmp_arrays (data_mid, data_dst, size);
    rumboot_printf("read OK\n");
    

    return 0;
}

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
    
    if (mkio_write_read ((uint32_t) (&data_src), (uint32_t) (&data_mid), (uint32_t) (&data_dst), DATA_SIZE, MKIO0_BASE, MKIO1_BASE) != 0)
        return -3;

    return 0;
}

