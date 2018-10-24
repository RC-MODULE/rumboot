
//-----------------------------------------------------------------------------
//  This file contains MKIO (Gaisler gr1553b) related functions 
//  It includes next functions:
//    - mkio_present
//    - mkio_report_bc_status
//    - mkio_report_rt_status
//    - mkio_report_bm_status       TDB
//    - mkio_bc_start_schedule
//    - mkio_rt_start_schedule
//    - mkio_write_to_rt
//    - mkio_read_from_rt
//    - 
//    - 
//    - 
//
//-----------------------------------------------------------------------------
#include <stdint.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/devices.h>

#include <regs/regs_mkio.h>

#include <devices/mkio.h>

uint32_t mkio_present  (uint32_t base_address)
{
    if (
            // (ioread32 (base_address + HWCFG) != 0x00000000) |
            ((ioread32 (base_address + BCSL) & 0xF0000000) != 0xF0000000) |
            ((ioread32 (base_address + RTS ) & 0x80000000) != 0x80000000) |
            ((ioread32 (base_address + BMS ) & 0x80000000) != 0x80000000)
        )
        return -1;
    rumboot_printf("    mkio_present.OK 0x%x\n", base_address);
    return 0;
}

void mkio_report_bc_status (uint32_t base_address)
{
    uint32_t rdata;
    rdata = ioread32 (base_address + BCSL);
    rumboot_printf("MKIO_BC BCSL reg value: 0x%x\n", rdata);
    rdata = ioread32 (base_address + IRQ);
    rumboot_printf("MKIO_BC IRQ reg value: 0x%x\n", rdata);
}

void mkio_report_rt_status (uint32_t base_address)
{
    uint32_t rdata;
    rdata = ioread32 (base_address + RTS);
    rumboot_printf("MKIO_RT RTS reg value: 0x%x\n", rdata);
    rdata = ioread32 (base_address + IRQ);
    rumboot_printf("MKIO_RT IRQ reg value: 0x%x\n", rdata);
}

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
    
    uint32_t rdata;
    
    iowrite32 (subaddress_table_pointer, base_address + RTSTBA );
    rdata = ioread32 ( base_address + RTC);
    iowrite32 ((rdata & (~0x3E)) | (RTKEY << 16) | (RTADDR << 1) | (RTEN << 0), base_address + RTC );
}

void mkio_bm_start_logging (uint32_t base_address, uint32_t ring_buffer_pointer, uint32_t ring_buffer_size)
{
    //  Safety code (RTKEY) - Must be written as 0x1553 when changing the RT address, otherwise the address
    //    field is unaffected by the write.
    uint32_t BMKEY  = 0x1543 ;
    //  RT Enable (RTEN) - Set to ‘1’ to enable listening for requests
    uint32_t BMEN   = 0x1    ;
    
    iowrite32 (ring_buffer_pointer, base_address + BMLBS);
    iowrite32 (ring_buffer_pointer + (ring_buffer_size << 2), base_address + BMLBE);
    iowrite32 ((BMKEY << 16) | (BMEN << 0), base_address + BMC);
}

uint32_t mkio_write_to_rt (uint32_t data_src, uint32_t data_dst, uint32_t size, uint32_t bc_base_address, uint32_t rt_base_address, uint32_t bus_sel)
{
    mkio_bc_descriptor         mkio_bc_descriptor        __attribute__ ((aligned(16 << 2)))  ;
    mkio_rt_subaddress_table   mkio_rt_subaddress_table  __attribute__ ((aligned(512))) ;
    mkio_rt_descriptor         mkio_rt_rx_descriptor     __attribute__ ((aligned(16 << 2)))  ;
    
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


    rumboot_printf("execute mkio_write_to_rt\n");
    
    rumboot_printf("  mkio_bc_descriptor        0x%x\n", (uint32_t)(&mkio_bc_descriptor      ));
    rumboot_printf("  mkio_rt_subaddress_table  0x%x\n", (uint32_t)(&mkio_rt_subaddress_table));
    rumboot_printf("  mkio_rt_rx_descriptor     0x%x\n", (uint32_t)(&mkio_rt_rx_descriptor   ));
    
    mkio_bc_descriptor.ctrl_word_0      = 0x00000000 | (SUSN << 25);
    mkio_bc_descriptor.ctrl_word_1      = 0x00000000 | (bus_sel << 30) | (RTAD1 << 11) | (TR << 10) | (RTSA1 << 5) | (WCMC << 0) ;
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
    
    
    mkio_rt_start_schedule (rt_base_address, (uint32_t) (&mkio_rt_subaddress_table));
    mkio_bc_start_schedule (bc_base_address, (uint32_t) (&mkio_bc_descriptor      ));
    
    //  Wait end of transaction with "polling descriptor" mechanism
    while (((mkio_bc_descriptor.result_word & 0x7) != 0) | ((mkio_rt_rx_descriptor.ctrl_status_word & 0x7) != 0))
    {
        for (volatile uint32_t i = 0; i < 10; i++)
            ;
    }
    rumboot_printf("bc and rt descriptors closed successfully\n");
    
    return 0;
}

uint32_t mkio_read_from_rt (uint32_t data_src, uint32_t data_dst, uint32_t size, uint32_t bc_base_address, uint32_t rt_base_address, uint32_t bus_sel)
{
    mkio_bc_descriptor         mkio_bc_descriptor        __attribute__ ((aligned(16 << 2)))  ;
    mkio_rt_subaddress_table   mkio_rt_subaddress_table  __attribute__ ((aligned(512))) ;
    mkio_rt_descriptor         mkio_rt_tx_descriptor     __attribute__ ((aligned(16 << 2)))  ;
    
    //  Suspend normally (SUSN) - Always suspends after transfer
    uint32_t SUSN = 0x0;
    
    //  RT Address (RTAD1)
    uint32_t RTAD1 = 0x00;
    //  RT Subaddress (RTSA1)
    uint32_t RTSA1 = 0x01;
    //  0 - transmit (BC->RT), 1 - receive (RT->BC)
    uint32_t TR = 1;
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


    rumboot_printf("execute mkio_read_from_rt\n");
    
    mkio_bc_descriptor.ctrl_word_0      = 0x00000000 | (SUSN << 25);
    mkio_bc_descriptor.ctrl_word_1      = 0x00000000 | (bus_sel << 30) | (RTAD1 << 11) | (TR << 10) | (RTSA1 << 5) | (WCMC << 0) ;
    mkio_bc_descriptor.data_pointer     = data_dst ;
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
    mkio_rt_subaddress_table.sa1_tx_descriptor_pointer = (uint32_t) (&mkio_rt_tx_descriptor) ;
    mkio_rt_subaddress_table.sa1_rx_descriptor_pointer = 0x00000000 ;
    mkio_rt_subaddress_table.sa1_reserved              = 0x00000000 ;
    
    mkio_rt_tx_descriptor.ctrl_status_word        = 0x03FFFFFF ;
    mkio_rt_tx_descriptor.data_pointer            = data_src ;
    mkio_rt_tx_descriptor.next_descriptor_pointer = rt_descriptor_end_of_list ;
    
    mkio_rt_start_schedule (rt_base_address, (uint32_t) (&mkio_rt_subaddress_table));
    mkio_bc_start_schedule (bc_base_address, (uint32_t) (&mkio_bc_descriptor      ));
    
    //  Wait end of transaction with "polling descriptor" mechanism
    while (((mkio_bc_descriptor.result_word & 0x7) != 0) | ((mkio_rt_tx_descriptor.ctrl_status_word & 0x7) != 0))
    {
        for (volatile uint32_t i = 0; i < 10; i++)
            ;
    }
    rumboot_printf("bc and rt descriptors closed successfully\n");
    
    return 0;
}

void mkio_set_polarity (uint32_t polarity_reg, uint32_t base_address)
{
    iowrite32 (polarity_reg, base_address + POLARITY);
}

void mkio_trig_first_timestamp (uint32_t base_address)
{
    iowrite32 (0x1, base_address + SYNC_CTRL_ADDR);
}

void mkio_fix_timestamp (uint32_t base_address)
{
    iowrite32 (0x2, base_address + SYNC_CTRL_ADDR);
}

void mkio_report_timestamp (uint32_t base_address)
{
    uint32_t rdata;
    
    rumboot_printf("mkio_report_timestamp for device 0x%x\n", base_address);
    mkio_fix_timestamp (base_address);
    rdata = ioread32 (base_address + VALIDCMD_FIRST_L_ADDR);
    rumboot_printf("    VALIDCMD_FIRST_L_ADDR reg value: 0x%x\n", rdata);
    rdata = ioread32 (base_address + VALIDCMD_FIRST_H_ADDR);
    rumboot_printf("    VALIDCMD_FIRST_H_ADDR reg value: 0x%x\n", rdata);
    rdata = ioread32 (base_address + VALIDCMD_LAST_L_ADDR );
    rumboot_printf("    VALIDCMD_LAST_L_ADDR reg value: 0x%x\n",  rdata);
    rdata = ioread32 (base_address + VALIDCMD_LAST_H_ADDR );
    rumboot_printf("    VALIDCMD_LAST_H_ADDR reg value: 0x%x\n",  rdata);
    rdata = ioread32 (base_address + RTSYNC_LAST_L_ADDR   );
    rumboot_printf("    RTSYNC_LAST_L_ADDR reg value: 0x%x\n",    rdata);
    rdata = ioread32 (base_address + RTSYNC_LAST_H_ADDR   );
    rumboot_printf("    RTSYNC_LAST_H_ADDR reg value: 0x%x\n",    rdata);
}

