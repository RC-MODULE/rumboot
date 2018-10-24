
//-----------------------------------------------------------------------------
//  This program is for checking some memory space with random write/read 
//    transactions.
//  Can cover different wires, so useful for RTL and netlist modeling
//
//
//
//    Test duration (RTL): < 
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
#define SPACE_SIZE 1024*16
#define ARRAY_SIZE (SPACE_SIZE >> 2)


#define RANDOM_SEED_1 300
#define RANDOM_SEED_2 400
#define RANDOM_SEED_3 500
#define TAUSWORTHE(s,a,b,c,d) ((s & c) << d) ^ (((s << a) ^ s) >> b)

uint32_t asm_mia_write_read (volatile uint32_t *space_for_check, uint32_t size)
{
    uint32_t asm_result;
    
    //  16 commands * 4 regs * 4 bytes = 256 byte per cycle
    //  So, shift array size in bytes on 256 to the right
    asm (
        //  Write part
        
        "MOV R2, %[start_addr]\n\t"
        "MOV R1, %[size]\n\t"
        "PUSH {R1, R2}\n\t"
        
        "LDR R4, =0x55555555\n\t"
        "LDR R5, =0x55555555\n\t"
        "LDR R6, =0x55555555\n\t"
        "LDR R7, =0x55555555\n\t"
        
        "create_array_cycle:\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "STMIA R2!, {R4-R7}\n\t"
        "SUB R1, #1\n\t"
        "CMP R1, #0\n\t"
        "BNE create_array_cycle\n\t"
        
        // Read and compare part
        
        "POP {R1, R2}\n\t"
        
        "LDR R3, =0x55555555\n\t"
        "LDR R0, =0x11235813\n\t"
            
        "check_array_cycle:       \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMP   R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"
        "LDMIA R2!, {R4-R7}       \n\t"
        "CMPEQ R4, R3             \n\t"
        "CMPEQ R5, R3             \n\t"
        "CMPEQ R6, R3             \n\t"
        "CMPEQ R7, R3             \n\t"
        "LDRNE R0, =0xFFFFFFFF    \n\t"

        "SUB R1, #1               \n\t"
        "CMP R1, #0               \n\t"
        "BNE check_array_cycle    \n\t"
        
        //  Result output
        "MOV %0, R0\n\t"
        
        : [result] "=r" (asm_result)
        : [start_addr] "r" (space_for_check), [size] "r" (SPACE_SIZE >> 8)
        :
    );
    
    return asm_result;
}

uint32_t esram0_mia_test ()
{
    volatile uint32_t *space_for_check = (uint32_t*) rumboot_malloc_from_heap_aligned (0, SPACE_SIZE, 4);
    
    uint32_t asm_result;
    
#ifdef DEBUG_INFO
    rumboot_printf("space to check:   0x%x  to  0x%x\n", space_for_check, space_for_check + ARRAY_SIZE);
#endif    
    asm_result = asm_mia_write_read (space_for_check, SPACE_SIZE);

#ifdef DEBUG_INFO
    rumboot_printf("Data write, read and compare finished\n");
    rumboot_printf("    asm_result = 0x%x\n", asm_result);
#endif    
    if (asm_result != 0x11235813)
        return -1;
    
    return 0;
}

int main ()
{
    rumboot_printf("esram_mia_test start\n");
    
    if (esram0_mia_test () != 0)
        return -1;
        
    return 0;
}

