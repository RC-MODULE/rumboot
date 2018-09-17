
//-----------------------------------------------------------------------------
//  This program is for checking DDR0 and DDR1 accessible for 
//    8, 16, 32 and 64 bits data
//  
//  Test includes:
//    - DDR0 turning On function
//    - DDR1 turning On function
//    - repeat few times for DDR0
//      - write zeroes to solve X problem
//      - write 8, 16, 32 and 64 data
//      - read and check data
//    - repeat few times for DDR1
//      - write zeroes to solve X problem
//      - write 8, 16, 32 and 64 data
//      - read and check data
//    
//    
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <rumboot/ddr_test_lib.h>
#include <rumboot/io.h>
#include <platform/devices.h>

void clear_space (uint32_t base_address)
{
    iowrite32 (0x00000000, base_address + 0x0);
    iowrite32 (0x00000000, base_address + 0x4);
}

uint32_t byte_test (uint32_t base_address)
{
    uint8_t  data_B  = 0x52               ;
    uint16_t data_W  = 0x9E43             ;
    uint32_t data_DW = 0x44B16FB4         ;
    uint64_t data_QW = 0x7BF08AF8578AA334 ;
    
    for (uint32_t i = 0; i < 8; i++)
    {
        clear_space (base_address);
        iowrite8 (data_B   , base_address + 0x0) ;
        if (ioread8 (base_address + 0x0) != data_B)
            return -1;
        clear_space (base_address);
        iowrite8 (data_B   , base_address + 0x1) ;
        if (ioread8 (base_address + 0x1) != data_B)
            return -1;
        clear_space (base_address);
        iowrite8 (data_B   , base_address + 0x2) ;
        if (ioread8 (base_address + 0x2) != data_B)
            return -1;
        clear_space (base_address);
        iowrite8 (data_B   , base_address + 0x3) ;
        if (ioread8 (base_address + 0x3) != data_B)
            return -1;
        clear_space (base_address);
        iowrite8 (data_B   , base_address + 0x4) ;
        if (ioread8 (base_address + 0x4) != data_B)
            return -1;
        clear_space (base_address);
        iowrite8 (data_B   , base_address + 0x5) ;
        if (ioread8 (base_address + 0x5) != data_B)
            return -1;
        clear_space (base_address);
        iowrite8 (data_B   , base_address + 0x6) ;
        if (ioread8 (base_address + 0x6) != data_B)
            return -1;
        clear_space (base_address);
        iowrite8 (data_B   , base_address + 0x7) ;
        if (ioread8 (base_address + 0x7) != data_B)
            return -1;
            
        clear_space (base_address);
        iowrite16 (data_W   , base_address + 0x0) ;
        if (ioread16 (base_address + 0x0) != data_W)
            return -1;
        clear_space (base_address);
        iowrite16 (data_W   , base_address + 0x2) ;
        if (ioread16 (base_address + 0x2) != data_W)
            return -1;
        clear_space (base_address);
        iowrite16 (data_W   , base_address + 0x4) ;
        if (ioread16 (base_address + 0x4) != data_W)
            return -1;
        clear_space (base_address);
        iowrite16 (data_W   , base_address + 0x6) ;
        if (ioread16 (base_address + 0x6) != data_W)
            return -1;
            
        clear_space (base_address);
        iowrite32 (data_DW   , base_address + 0x0) ;
        if (ioread32 (base_address + 0x0) != data_DW)
            return -1;
        clear_space (base_address);
        iowrite32 (data_DW   , base_address + 0x4) ;
        if (ioread32 (base_address + 0x4) != data_DW)
            return -1;
        
        if (base_address == EMI0_BASE)
            asm (
                "PUSH {R0-R7}\n\t"
                "LDR R0, =0x80000000\n\t"
                "LDR R4, =0x517DF10C\n\t"
                "LDR R5, =0x44D50C98\n\t"
                "STMIA R0!, {R4, R5}\n\t"
                "POP {R0-R7}\n\t"
            );
        else
            asm (
                "PUSH {R0-R7}\n\t"
                "LDR R0, =0xC0000000\n\t"
                "LDR R4, =0x517DF10C\n\t"
                "LDR R5, =0x44D50C98\n\t"
                "STMIA R0!, {R4, R5}\n\t"
                "POP {R0-R7}\n\t"
            );
        
        clear_space (base_address);
        iowrite64 (data_QW   , base_address + 0x0) ;
        if (ioread64 (base_address + 0x0) != data_QW)
            return -1;
    }
    return 0;
}

int main ()
{
    if (ddr_init (DDR0_BASE) != 0)
        return -1;
    if (ddr_init (DDR1_BASE) != 0)
        return -2;
    
    if (byte_test (EMI0_BASE) != 0)
        return -3;
    if (byte_test (EMI1_BASE) != 0)
        return -3;
    
    return 0;
}

