
//-----------------------------------------------------------------------------
//  This program is for checking that DDR0 and DDR1 can turn On
//    Test pass even in absence of external DDR memories.
//    Test fail, if initialisation timeout happen.
//  
//  Test includes:
//    - DDR0 turning On function
//    - DDR1 turning On function
//    
//    
//    Test duration (RTL): < 280us
//-----------------------------------------------------------------------------

#include <rumboot/ddr_test_lib.h>

int main ()
{
    if (ddr_init (DDR0_BASE) != 0)
        return -1;
    if (ddr_init (DDR1_BASE) != 0)
        return -2;
    return 0;
}

