
//-----------------------------------------------------------------------------
//  This program is for simple eSRAM1 testing.
//  It includes:
//    - write 0x55555555 to eSRAM1
//    - read 0x55555555 from eSRAM1
//    - compare read data with etalon
//    - write 0xAAAAAAAA to eSRAM1
//    - read 0xAAAAAAAA from eSRAM1
//    - compare read data with etalon
//  
//    Test duration (RTL): 
//-----------------------------------------------------------------------------

#include <basis/defs.h>

#define ESRAM1_start_addr 0x60000 + 0x10000
#define ESRAM1_check_size 0x10000

.global main
main:
    PUSH {LR}
    
    
/**********************************************************
 *  Set success flag
 *********************************************************/
    LDR R0, =0x00000000
    
/**********************************************************
 *  256 (0x100) bytes are written in 1 cycle. So, amount
 *  of bytes must be ROR by 8
 *********************************************************/
    LDR R4, =0x55555555
    LDR R5, =0x55555555
    LDR R6, =0x55555555
    LDR R7, =0x55555555

    LDR R1, =ESRAM1_check_size >> 8
    LDR R2, =ESRAM1_start_addr
    BL create_array
    
    LDR R3, =0x55555555
    LDR R1, =ESRAM1_check_size >> 8
    LDR R2, =ESRAM1_start_addr
    BL check_array
    
    LDR R4, =0xAAAAAAAA
    LDR R5, =0xAAAAAAAA
    LDR R6, =0xAAAAAAAA
    LDR R7, =0xAAAAAAAA

    LDR R1, =ESRAM1_check_size >> 8
    LDR R2, =ESRAM1_start_addr
    BL create_array
    
    LDR R3, =0xAAAAAAAA
    LDR R1, =ESRAM1_check_size >> 8
    LDR R2, =ESRAM1_start_addr
    BL check_array
    
    POP {PC}
    
    
    
    
    
    
create_array:
    PUSH {LR}
create_array_cycle:    
/**********************************************************
 *    increase data store performance with multiple cmd
 *********************************************************/
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    STMIA R2!, {R4-R7}
    SUB R1, #1
    CMP R1, #0
    BNE create_array_cycle
    
    POP {PC}

check_array:
    PUSH {LR}
check_array_cycle:    
/**********************************************************
 *    increase data load performance with multiple cmd
 *********************************************************/
    LDMIA R2!, {R4-R7}
    CMP   R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    LDMIA R2!, {R4-R7}
    CMPEQ R4, R3
    CMPEQ R5, R3
    CMPEQ R6, R3
    CMPEQ R7, R3
    LDRNE R0, =0xFFFFFFFF
    
    SUB R1, #1
    CMP R1, #0
    BNE check_array_cycle
    
    POP {PC}
    
    