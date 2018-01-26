
//-----------------------------------------------------------------------------
//  This program is for checking 8, 16 and 64 bit data width write/read to
//    esram.
//
//  Test includes:
//    - repeat 3 times:
//      - write/read/check  8 bit data to few  8 bit aligned addresses
//      - write/read/check 16 bit data to few 16 bit aligned addresses
//      - write/read/check 64 bit data to few 64 bit aligned addresses
//-----------------------------------------------------------------------------

#define addr_first ((int*) 0x00048000)
#define addr_last  ((int*) 0x00048100)

void clear_space (int* this_addr_first, int* this_addr_last)
{
    int*   address   ;
    for (address = this_addr_first; address <= this_addr_last; address++)
    {
        *address = 0x00000000;
    }
}

int esram_byte_access_test ()
{
    volatile char            data_8bit   = 0x5A               ;
    volatile short int       data_16bit  = 0x486B             ;
    volatile long long int   data_64bit  = 0x1234567890ABCDEF ;

    clear_space (addr_first, addr_last);

    (*(char *) 0x00048000) = data_8bit;
    if ((*(volatile char *) 0x00048000) != data_8bit)
        return -1;
    (*(char *) 0x00048001) = data_8bit + 1;
    if ((*(volatile char *) 0x00048001) != data_8bit + 1)
        return -1;
    (*(char *) 0x00048002) = data_8bit + 2;
    if ((*(volatile char *) 0x00048002) != data_8bit + 2)
        return -1;
    (*(char *) 0x00048003) = data_8bit + 3;
    if ((*(volatile char *) 0x00048003) != data_8bit + 3)
        return -1;
    (*(char *) 0x00048004) = data_8bit + 4;
    if ((*(volatile char *) 0x00048004) != data_8bit + 4)
        return -1;
    (*(char *) 0x00048005) = data_8bit + 5;
    if ((*(volatile char *) 0x00048005) != data_8bit + 5)
        return -1;
    (*(char *) 0x00048006) = data_8bit + 6;
    if ((*(volatile char *) 0x00048006) != data_8bit + 6)
        return -1;
    (*(char *) 0x00048007) = data_8bit + 7;
    if ((*(volatile char *) 0x00048007) != data_8bit + 7)
        return -1;

    (*(short int *) 0x00048000) = data_16bit;
    if ((*(volatile short int *) 0x00048000) != data_16bit)
        return -1;
    (*(short int *) 0x00048002) = data_16bit + 1;
    if ((*(volatile short int *) 0x00048002) != data_16bit + 1)
        return -1;
    (*(short int *) 0x00048004) = data_16bit + 2;
    if ((*(volatile short int *) 0x00048004) != data_16bit + 2)
        return -1;
    (*(short int *) 0x00048006) = data_16bit + 3;
    if ((*(volatile short int *) 0x00048006) != data_16bit + 3)
        return -1;

    (*(long long int *) 0x00048000) = data_64bit;
    if ((*(volatile long long int *) 0x00048000) != data_64bit)
        return -1;
    (*(long long int *) 0x00048008) = data_64bit + 1;
    if ((*(volatile long long int *) 0x00048008) != data_64bit + 1)
        return -1;


    return 0;
}

int main ()
{
    if (esram_byte_access_test () != 0)
        return -1;
    if (esram_byte_access_test () != 0)
        return -1;
    if (esram_byte_access_test () != 0)
        return -1;
    if (esram_byte_access_test () != 0)
        return -1;
    if (esram_byte_access_test () != 0)
        return -1;

    return 0;
}
