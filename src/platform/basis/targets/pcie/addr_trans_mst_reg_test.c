
//-----------------------------------------------------------------------------
//  This program is for AXI3_addr_translator_mst register testing
//  It includes:
//    - checking number of accessable registers
//    - checking region registers address crossing
//    - checking that bits are correct
//    -
//
//  Test duration (RTL): < 0.6 ms
//
//-----------------------------------------------------------------------------
#include <platform/devices.h>

#define regions_number         9
#define registers_number       (regions_number * 3 + 1)

int check_reg_number ()
{
    volatile int *pointer;

    pointer = (int *)(ADDR_TRANS_MST_BASE);
    for (int i = 0; i < 1024; i++)
    {
        *pointer = 0xFFFFFFFF;
        if ((*pointer == 0) & (i < registers_number))
            return -1;
        pointer++;
    }
    return 0;
}

int check_reg_addr ()
{
    volatile int *pointer;

    pointer = (int *)(ADDR_TRANS_MST_BASE);
    pointer++;
    for (int i = 0; i < regions_number; i++)
    {
        *pointer = (int) pointer << 12;
        pointer++;
        *pointer = (int) pointer << 12;
        pointer++;
        *pointer = (int) pointer << 12;
        pointer++;
    }
    pointer = (int *)(ADDR_TRANS_MST_BASE);
    pointer++;
    for (int i = 0; i < regions_number; i++)
    {
        if (*pointer != (((int) pointer << 12) & 0xFFFFF001))
            return -1;
        pointer++;
        if (*pointer != ((((int) pointer << 12) & 0xFFFFF000) | 0x00000FFF))
            return -1;
        pointer++;
        if (*pointer != (((int) pointer << 12) & 0xFFFFF000))
            return -1;
        pointer++;
    }
    return 0;
}

int check_reg_bits ()
{
    volatile int *pointer;

    pointer = (int *)(ADDR_TRANS_MST_BASE);
    *pointer = 0x00000000;
    if (*pointer != 0x00000000)
        return -1;
    *pointer = 0x55555555;
    if (*pointer != 0x00000004)
        return -1;
    *pointer = 0xAAAAAAAA;
    if (*pointer != 0x00000002)
        return -1;
    *pointer = 0xFFFFFFFF;
    if (*pointer != 0x00000006)
        return -1;

    for (int i = 0; i < regions_number; i++)
    {
        pointer++;
        *pointer = 0x00000000;
        if (*pointer != 0x00000000)
            return -1;
        *pointer = 0x55555555;
        if (*pointer != 0x55555001)
            return -1;
        *pointer = 0xAAAAAAAA;
        if (*pointer != 0xAAAAA000)
            return -1;
        *pointer = 0xFFFFFFFF;
        if (*pointer != 0xFFFFF001)
            return -1;

        pointer++;
        *pointer = 0x00000000;
        if (*pointer != 0x00000FFF)
            return -1;
        *pointer = 0x55555555;
        if (*pointer != 0x55555FFF)
            return -1;
        *pointer = 0xAAAAAAAA;
        if (*pointer != 0xAAAAAFFF)
            return -1;
        *pointer = 0xFFFFFFFF;
        if (*pointer != 0xFFFFFFFF)
            return -1;

        pointer++;
        *pointer = 0x00000000;
        if (*pointer != 0x00000000)
            return -1;
        *pointer = 0x55555555;
        if (*pointer != 0x55555000)
            return -1;
        *pointer = 0xAAAAAAAA;
        if (*pointer != 0xAAAAA000)
            return -1;
        *pointer = 0xFFFFFFFF;
        if (*pointer != 0xFFFFF000)
            return -1;
    }
    return 0;
}

int main ()
{
    if (check_reg_number () != 0)
        return -1;

    if (check_reg_addr () != 0)
        return -1;

    if (check_reg_bits () != 0)
        return -1;

    return 0;
}
