
//-----------------------------------------------------------------------------
//  This program is for checking some memory space with MOVI1 3N Algorithm
//  Data width = 32
//    Space was reduced for simulation.
//    Set it to neccessary values in real program.
//
//    Function MOVI_memory_check, probably, can be used for testing all memories
//    But it will take lots of time on simulation. TODO on real chip.
//
//    Test duration (RTL): < 540us
//-----------------------------------------------------------------------------

#define addr_first ((int*) 0x00040000)
#define addr_last  ((int*) 0x00040100)

int MOVI_memory_check (int* this_addr_first, int* this_addr_last)
{
    int    data      ;
    int    data_n    ;
    int*   address   ;
    
    for (int i = 0; i < 6; i++)
    {
        //---------------------------------------------------------------------
        //  data and inversed data setting
        //---------------------------------------------------------------------
        if (i == 0) data = 0x00000000;
        if (i == 1) data = 0x55555555;
        if (i == 2) data = 0x33333333;
        if (i == 3) data = 0x0F0F0F0F;
        if (i == 4) data = 0x00FF00FF;
        if (i == 5) data = 0x0000FFFF;
        data_n = ~data;
        
        //---------------------------------------------------------------------
        //  [down(wX)]
        //---------------------------------------------------------------------
        for (address = this_addr_last; address >= this_addr_first; address--)
        {
            *address = data;
        }
        //---------------------------------------------------------------------
        //  [up(rX, wY, rY)]
        //---------------------------------------------------------------------
        for (address = this_addr_first; address <= this_addr_last; address++)
        {
            if (*address != data)
                return -1;
            *address = data_n;
            if (*address != data_n)
                return -1;
        }
        //---------------------------------------------------------------------
        //  [up(rY, wX, rX)]
        //---------------------------------------------------------------------
        for (address = this_addr_first; address <= this_addr_last; address++)
        {
            if (*address != data_n)
                return -1;
            *address = data;
            if (*address != data)
                return -1;
        }
        //---------------------------------------------------------------------
        //  [down(rX, wY, rY)]
        //---------------------------------------------------------------------
        for (address = this_addr_last; address >= this_addr_first; address--)
        {
            if (*address != data)
                return -1;
            *address = data_n;
            if (*address != data_n)
                return -1;
        }
        //---------------------------------------------------------------------
        //  [down(rY, wX, rX)]
        //---------------------------------------------------------------------
        for (address = this_addr_last; address >= this_addr_first; address--)
        {
            if (*address != data_n)
                return -1;
            *address = data;
            if (*address != data)
                return -1;
        }
    }
    return 0;
}

int main ()
{
    if ((MOVI_memory_check (addr_first, addr_last)) != 0)
        return -1;
        
    return 0;
}

