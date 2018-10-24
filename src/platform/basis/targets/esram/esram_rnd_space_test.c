
//-----------------------------------------------------------------------------
//  This program is for checking some memory space with random write/read 
//    transactions.
//  Can cover different wires, so useful for RTL and netlist modeling
//  Test includes:
//    - declare some memory in ESRAM (ESRAM 0 and 1 are mixed for declaration)
//    - cycle CHECK_AMOUNT
//      - generate random array element number
//      - write, read and compare to this element
//    - 
//    - 
//    - 
//
//    Test duration (RTL): < 1.5 ms (wo DEBUG_INFO)
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <rumboot/platform.h>

//  Uncomment to watch more info during the test.
//  Debug output increase execution time and can influence on test results!
// #define DEBUG_INFO

//  Check array size in bytes. Must cover as much place as possible, but it couldnt
//    cover all
//  Not all this array will be checked, because it is too big.
#define SPACE_SIZE (1024*(128+64))
#define ARRAY_SIZE (SPACE_SIZE >> 2)

//  Amount of checked cells
#define CHECK_AMOUNT 512


#define RANDOM_SEED_1 300
#define RANDOM_SEED_2 400
#define RANDOM_SEED_3 500
#define TAUSWORTHE(s,a,b,c,d) ((s & c) << d) ^ (((s << a) ^ s) >> b)

uint32_t memory_space_check ()
{
    volatile uint32_t array_element;
    
    uint32_t s1 = RANDOM_SEED_1;
    uint32_t s2 = RANDOM_SEED_2;
    uint32_t s3 = RANDOM_SEED_3;
    
    volatile uint32_t space_for_check [ARRAY_SIZE];
    rumboot_printf("space to check:   0x%x  to  0x%x\n", space_for_check, space_for_check + ARRAY_SIZE);
    
    for (volatile uint32_t i = 0; i < CHECK_AMOUNT; i++)
    {
        s1 = TAUSWORTHE (s1,13, 19, 4294967294UL, 12);
        s2 = TAUSWORTHE (s2, 2, 25, 4294967288UL,  4);
        s3 = TAUSWORTHE (s3, 3, 11, 4294967280UL, 17);
        array_element = (s1 ^ s2 ^ s3) % ARRAY_SIZE;
        
        space_for_check [array_element] = ((i+0)<<0) + ((i+1)<<8) + ((i+2)<<16) + ((i+3)<<24);
#ifdef DEBUG_INFO
        rumboot_printf("write 0x%x    to 0x%x    array_element %d\n", space_for_check [array_element], &(space_for_check [array_element]), array_element);
#endif
        
        if (space_for_check [array_element] != ((i+0)<<0) + ((i+1)<<8) + ((i+2)<<16) + ((i+3)<<24))
        {
            rumboot_printf("  ERROR: 0x%x != 0x%x  ptr = 0x%x\n", space_for_check [array_element], ((i+0)<<0) + ((i+1)<<8) + ((i+2)<<16) + ((i+3)<<24), &(space_for_check [array_element]));
            return -1;
        }
#ifdef DEBUG_INFO
        rumboot_printf("read  0x%x == 0x%x    array_element %d\n", space_for_check [array_element], ((i+0)<<0) + ((i+1)<<8) + ((i+2)<<16) + ((i+3)<<24), array_element);
#endif
    }
    
    return 0;
}

int main ()
{
    rumboot_printf("esram_rnd_space_test start\n");
    
    if (memory_space_check () != 0)
        return -1;
        
    return 0;
}

