
//-----------------------------------------------------------------------------
//  This program is for checking all bits of AXI bus address, that are used by
//    DDR1 controller.
//  Also, it checks, that space are not mirror.
//  
//  Test includes:
//    - Change DDR0 CRG frequency to correspond ddr3 1600 data rate
//    - DDR0 turning On function
//    - check DDR addresses with running 1 (repeat 3 times)
//      - write 64'h0, read 64'h0, go to next address
//      - write 64'hF, read 64'hF, go to next address
//      - write addr, go to next address
//      - read addr, go to next address
//    
//    
//    Test duration (RTL): < 520us
//-----------------------------------------------------------------------------

#include <rumboot/ddr_test_lib.h>
#include <platform/devices.h>

uint32_t ddr_addr_shift1_test ()
{
    
    volatile uint64_t *DDR3_cell [29] =
    {
        (uint64_t*) 0xC0000000 ,
        (uint64_t*) 0xC0000008 ,
        (uint64_t*) 0xC0000010 ,
        (uint64_t*) 0xC0000020 ,
        (uint64_t*) 0xC0000040 ,
        (uint64_t*) 0xC0000080 ,
        (uint64_t*) 0xC0000100 ,
        (uint64_t*) 0xC0000200 ,
        (uint64_t*) 0xC0000400 ,
        (uint64_t*) 0xC0000800 ,
        (uint64_t*) 0xC0001000 ,
        (uint64_t*) 0xC0002000 ,
        (uint64_t*) 0xC0004000 ,
        (uint64_t*) 0xC0008000 ,
        (uint64_t*) 0xC0010000 ,
        (uint64_t*) 0xC0020000 ,
        (uint64_t*) 0xC0040000 ,
        (uint64_t*) 0xC0080000 ,
        (uint64_t*) 0xC0100000 ,
        (uint64_t*) 0xC0200000 ,
        (uint64_t*) 0xC0400000 ,
        (uint64_t*) 0xC0800000 ,
        (uint64_t*) 0xC1000000 ,
        (uint64_t*) 0xC2000000 ,
        (uint64_t*) 0xC4000000 ,
        (uint64_t*) 0xC8000000 ,
        (uint64_t*) 0xD0000000 ,
        (uint64_t*) 0xD8000000 ,
        (uint64_t*) 0xDFFFFFF8
    };
    
    
    for (volatile int j = 0; j < 3; j++)
    {
        for (volatile int i = 0; i <= 28; i++)
        {
            *(DDR3_cell [i]) = 0x0000000000000000;
            if (*(DDR3_cell [i]) != 0x0000000000000000)
                return -1;
        }
        
        for (volatile int i = 28; i >= 0; i--)
        {
            *(DDR3_cell [i]) = 0xFFFFFFFFFFFFFFFF;
            if (*(DDR3_cell [i]) != 0xFFFFFFFFFFFFFFFF)
                return -1;
        }
        
        for (volatile int i = 0; i <= 28; i++)
        {
            *(DDR3_cell [i]) = (uint64_t) (uint32_t) DDR3_cell [i];
        }
        for (volatile int i = 0; i <= 28; i++)
        {
            if (*(DDR3_cell [i]) != (uint64_t) (uint32_t) DDR3_cell [i])
                return -1;
        }
    }
    
    return 0;
}

int main ()
{
    if (ddr_init (DDR1_BASE) != 0)
        return -1;
    if (ddr_addr_shift1_test () != 0)
        return -2;
    return 0;
}

