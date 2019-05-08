
//-----------------------------------------------------------------------------
//  This program is for checking all bits of AXI bus address, that are used by
//    DDR0 controller.
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
        (uint64_t*) 0x80000000 ,
        (uint64_t*) 0x80000008 ,
        (uint64_t*) 0x80000010 ,
        (uint64_t*) 0x80000020 ,
        (uint64_t*) 0x80000040 ,
        (uint64_t*) 0x80000080 ,
        (uint64_t*) 0x80000100 ,
        (uint64_t*) 0x80000200 ,
        (uint64_t*) 0x80000400 ,
        (uint64_t*) 0x80000800 ,
        (uint64_t*) 0x80001000 ,
        (uint64_t*) 0x80002000 ,
        (uint64_t*) 0x80004000 ,
        (uint64_t*) 0x80008000 ,
        (uint64_t*) 0x80010000 ,
        (uint64_t*) 0x80020000 ,
        (uint64_t*) 0x80040000 ,
        (uint64_t*) 0x80080000 ,
        (uint64_t*) 0x80100000 ,
        (uint64_t*) 0x80200000 ,
        (uint64_t*) 0x80400000 ,
        (uint64_t*) 0x80800000 ,
        (uint64_t*) 0x81000000 ,
        (uint64_t*) 0x82000000 ,
        (uint64_t*) 0x84000000 ,
        (uint64_t*) 0x88000000 ,
        (uint64_t*) 0x90000000 ,
        (uint64_t*) 0x98000000 ,
        (uint64_t*) 0x9FFFFFF8
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
    if (ddr_init (DDR0_BASE) != 0)
        return -1;
    if (ddr_addr_shift1_test () != 0)
        return -2;
    return 0;
}

