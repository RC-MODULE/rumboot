
//-----------------------------------------------------------------------------
//  This program is for testing Gaisler MKIO external signals connectivity
//    Test includes:
//    - initialise source array
//    - check mkio_inh signals idle
//    - write data to MKIO0 (RT) with MKIO1 (BC)
//    - check mkio_inh signals work
//    - 
//    - 
//
//
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
#include <regs/regs_mkio.h>
#include <regs/regs_gpio.h>
#include <devices/mkio.h>

//  Single Array Data Size In Bytes
#define DATA_SIZE   2

//  This variable must be seen for irq handler, so it is global
uint32_t irq_flag;



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
uint32_t check_mkio0_A_inh_idle ()
{
    for (uint32_t i = 0; i < 8; i++)
        if ((ioread32 (GPIO1_BASE + GPIO_READ_PADtoAPB) & 0x1) != 0x1)
            return -1;
    return 0;
}

uint32_t check_mkio0_A_inh_active ()
{
    for (uint32_t i = 0; i < 8; i++)
        if ((ioread32 (GPIO1_BASE + GPIO_READ_PADtoAPB) & 0x1) != 0x0)
            return -1;
    return 0;
}

uint32_t check_mkio0_B_inh_idle ()
{
    for (uint32_t i = 0; i < 8; i++)
        if ((ioread32 (GPIO1_BASE + GPIO_READ_PADtoAPB) & 0x2) != 0x2)
            return -1;
    return 0;
}

uint32_t check_mkio0_B_inh_active ()
{
    for (uint32_t i = 0; i < 8; i++)
        if ((ioread32 (GPIO1_BASE + GPIO_READ_PADtoAPB) & 0x2) != 0x0)
            return -1;
    return 0;
}

uint32_t check_mkio1_A_inh_idle ()
{
    for (uint32_t i = 0; i < 8; i++)
        if ((ioread32 (GPIO1_BASE + GPIO_READ_PADtoAPB) & 0x4) != 0x4)
            return -1;
    return 0;
}

uint32_t check_mkio1_A_inh_active ()
{
    for (uint32_t i = 0; i < 8; i++)
        if ((ioread32 (GPIO1_BASE + GPIO_READ_PADtoAPB) & 0x4) != 0x0)
            return -1;
    return 0;
}

uint32_t check_mkio1_B_inh_idle ()
{
    for (uint32_t i = 0; i < 8; i++)
        if ((ioread32 (GPIO1_BASE + GPIO_READ_PADtoAPB) & 0x8) != 0x8)
            return -1;
    return 0;
}

uint32_t check_mkio1_B_inh_active ()
{
    for (uint32_t i = 0; i < 8; i++)
        if ((ioread32 (GPIO1_BASE + GPIO_READ_PADtoAPB) & 0x8) != 0x0)
            return -1;
    return 0;
}


uint32_t mkio_write_to_rt_with_irq (uint32_t data_src, uint32_t data_dst, uint32_t size, uint32_t bc_base_address, uint32_t rt_base_address)
{
    mkio_bc_descriptor         mkio_bc_descriptor        __attribute__ ((aligned(16)))  ;
    mkio_rt_subaddress_table   mkio_rt_subaddress_table  __attribute__ ((aligned(512))) ;
    mkio_rt_descriptor         mkio_rt_rx_descriptor     __attribute__ ((aligned(16)))  ;
    
    uint32_t mkio_irq_ring_buffer [16] __attribute__ ((aligned(64)));
    
    //  Suspend normally (SUSN) - Always suspends after transfer
    uint32_t SUSN = 0x1;
    //  IRQ after transfer on Error (IRQE_)
    uint32_t IRQE_ = 1;
    //  IRQ normally (IRQN) - Always interrupts after transfer
    uint32_t IRQN = 1;
    //  Bus selection (BUS) - Bus to use for transfer, 0 - Bus A, 1 - Bus B
    uint32_t BUS = 0;
    
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
    

    rumboot_printf("execute mkio_write_to_rt_with_irq\n");
    rumboot_printf("    data_src %x\n",                 (uint32_t) (&data_src                 ));
    rumboot_printf("    data_dst %x\n",                 (uint32_t) (&data_dst                 ));
    rumboot_printf("    mkio_bc_descriptor %x\n",       (uint32_t) (&mkio_bc_descriptor       ));
    rumboot_printf("    mkio_rt_subaddress_table %x\n", (uint32_t) (&mkio_rt_subaddress_table ));
    rumboot_printf("    mkio_rt_rx_descriptor %x\n",    (uint32_t) (&mkio_rt_rx_descriptor    ));
    rumboot_printf("    mkio_irq_ring_buffer %x\n",     (uint32_t) (&mkio_irq_ring_buffer     ));
    
    if (
            (check_mkio0_A_inh_idle () != 0) |
            (check_mkio0_B_inh_idle () != 0) |
            (check_mkio1_A_inh_idle () != 0) |
            (check_mkio1_B_inh_idle () != 0)
        )
        return -1;
    
    //-------------------------------------------------------------------------
    //  For 1553 BUS A
    //-------------------------------------------------------------------------
    mkio_bc_descriptor.ctrl_word_0      = 0x00000000 | (IRQE_ << 28) | (IRQN << 27) | (SUSN << 25);
    mkio_bc_descriptor.ctrl_word_1      = 0x00000000 | (BUS << 30) | (RTAD1 << 11) | (TR << 10) | (RTSA1 << 5) | (WCMC << 0) ;
    mkio_bc_descriptor.data_pointer     = data_src ;
    mkio_bc_descriptor.result_word      = 0xFFFFFFFF ;
    mkio_bc_descriptor.condition_word   = bc_descriptor_end_of_list;
    mkio_bc_descriptor.branch_address   = unused;
    mkio_bc_descriptor.reserved_0       = unused;
    mkio_bc_descriptor.reserved_1       = unused;
    
    //  Subaddress = 0x00 and 0x1F are reserved to identify "mode command"
    //  So, use 0x01 table entrance
    mkio_rt_subaddress_table.sa0_ctrl_word             = unused ;
    mkio_rt_subaddress_table.sa0_tx_descriptor_pointer = unused ;
    mkio_rt_subaddress_table.sa0_rx_descriptor_pointer = unused ;
    mkio_rt_subaddress_table.sa0_reserved              = unused ;
    mkio_rt_subaddress_table.sa1_ctrl_word             = 0x00000000 | (RXEN << 15) | (TXEN << 7) ;
    mkio_rt_subaddress_table.sa1_tx_descriptor_pointer = 0x00000000 ;
    mkio_rt_subaddress_table.sa1_rx_descriptor_pointer = (uint32_t) (&mkio_rt_rx_descriptor) ;
    mkio_rt_subaddress_table.sa1_reserved              = 0x00000000 ;
    
    mkio_rt_rx_descriptor.ctrl_status_word        = 0x03FFFFFF ;
    mkio_rt_rx_descriptor.data_pointer            = data_dst ;
    mkio_rt_rx_descriptor.next_descriptor_pointer = rt_descriptor_end_of_list ;
    
    //  reset interrupt flag
    irq_flag = 0;
    //  Enable all interrupts in BC controller
    iowrite32 (0xFFFFFFFF, bc_base_address + IRQE);
    iowrite32 ((uint32_t) (&mkio_irq_ring_buffer), bc_base_address + BCRD);
    
    mkio_rt_start_schedule (rt_base_address, (uint32_t) (&mkio_rt_subaddress_table));
    if (
            (check_mkio0_A_inh_idle () != 0) |
            (check_mkio0_B_inh_idle () != 0) |
            (check_mkio1_A_inh_active () != 0) |
            (check_mkio1_B_inh_active () != 0)
        )
        return -1;
    mkio_bc_start_schedule (bc_base_address, (uint32_t) (&mkio_bc_descriptor      ));
    
    if (
            (check_mkio0_A_inh_active () != 0) |
            (check_mkio0_B_inh_active () != 0) |
            (check_mkio1_A_inh_active () != 0) |
            (check_mkio1_B_inh_active () != 0)
        )
        return -1;
    
    //  Wait end of transaction with "polling descriptor" mechanism
    while (((mkio_bc_descriptor.result_word & 0x7) != 0) | ((mkio_rt_rx_descriptor.ctrl_status_word & 0x7) != 0))
    {
        for (volatile uint32_t i = 0; i < 10; i++)
            ;
    }
    rumboot_printf("bc and rt descriptors closed successfully\n");
    
    //-------------------------------------------------------------------------
    //  For 1553 BUS B
    //-------------------------------------------------------------------------
    BUS = 1;
    
    mkio_bc_descriptor.ctrl_word_0      = 0x00000000 | (IRQE_ << 28) | (IRQN << 27) | (SUSN << 25);
    mkio_bc_descriptor.ctrl_word_1      = 0x00000000 | (BUS << 30) | (RTAD1 << 11) | (TR << 10) | (RTSA1 << 5) | (WCMC << 0) ;
    mkio_bc_descriptor.data_pointer     = data_src ;
    mkio_bc_descriptor.result_word      = 0xFFFFFFFF ;
    mkio_bc_descriptor.condition_word   = bc_descriptor_end_of_list;
    mkio_bc_descriptor.branch_address   = unused;
    mkio_bc_descriptor.reserved_0       = unused;
    mkio_bc_descriptor.reserved_1       = unused;
    
    //  Subaddress = 0x00 and 0x1F are reserved to identify "mode command"
    //  So, use 0x01 table entrance
    mkio_rt_subaddress_table.sa0_ctrl_word             = unused ;
    mkio_rt_subaddress_table.sa0_tx_descriptor_pointer = unused ;
    mkio_rt_subaddress_table.sa0_rx_descriptor_pointer = unused ;
    mkio_rt_subaddress_table.sa0_reserved              = unused ;
    mkio_rt_subaddress_table.sa1_ctrl_word             = 0x00000000 | (RXEN << 15) | (TXEN << 7) ;
    mkio_rt_subaddress_table.sa1_tx_descriptor_pointer = 0x00000000 ;
    mkio_rt_subaddress_table.sa1_rx_descriptor_pointer = (uint32_t) (&mkio_rt_rx_descriptor) ;
    mkio_rt_subaddress_table.sa1_reserved              = 0x00000000 ;
    
    mkio_rt_rx_descriptor.ctrl_status_word        = 0x03FFFFFF ;
    mkio_rt_rx_descriptor.data_pointer            = data_dst ;
    mkio_rt_rx_descriptor.next_descriptor_pointer = rt_descriptor_end_of_list ;
    
    //  reset interrupt flag
    irq_flag = 0;
    //  Enable all interrupts in BC controller
    iowrite32 (0xFFFFFFFF, bc_base_address + IRQE);
    iowrite32 ((uint32_t) (&mkio_irq_ring_buffer), bc_base_address + BCRD);
    
    mkio_rt_start_schedule (rt_base_address, (uint32_t) (&mkio_rt_subaddress_table));
    mkio_bc_start_schedule (bc_base_address, (uint32_t) (&mkio_bc_descriptor      ));
    
    if (
            (check_mkio0_A_inh_active () != 0) |
            (check_mkio0_B_inh_active () != 0) |
            (check_mkio1_A_inh_active () != 0) |
            (check_mkio1_B_inh_active () != 0)
        )
        return -1;
    
    //  Wait end of transaction with "polling descriptor" mechanism
    while (((mkio_bc_descriptor.result_word & 0x7) != 0) | ((mkio_rt_rx_descriptor.ctrl_status_word & 0x7) != 0))
    {
        for (volatile uint32_t i = 0; i < 10; i++)
            ;
    }
    rumboot_printf("bc and rt descriptors closed successfully\n");
    
    if (
            (check_mkio0_A_inh_active () != 0) |
            (check_mkio0_B_inh_active () != 0) |
            (check_mkio1_A_inh_active () != 0) |
            (check_mkio1_B_inh_active () != 0)
        )
        return -1;
    
    return 0;
}

//-----------------------------------------------------------------------------
//  Test sequence
//-----------------------------------------------------------------------------
uint32_t main ()
{
    uint32_t data_src [DATA_SIZE >> 2] __attribute__ ((aligned(4)));
    uint32_t data_dst [DATA_SIZE >> 2] __attribute__ ((aligned(4)));
    
    rumboot_printf("    mkio_signal_test\n");
    
    create_etalon_array (data_src, DATA_SIZE);

    if (mkio_write_to_rt_with_irq ((uint32_t) (&data_src), (uint32_t) (&data_dst), DATA_SIZE, MKIO0_BASE, MKIO1_BASE) != 0)
        return -3;

    return 0;
}

