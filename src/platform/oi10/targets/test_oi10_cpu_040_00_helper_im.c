/*
 * test_oi10_cpu_039_00_helper.c
 *
 *  Created on: Dec 21, 2018
 *   Original Author: m.smolina
 *   Rebuild by     : Dmitry Dryagalkin
 */
// 16.03.20, 
// hit to L2C bug (OI10-415, store gathering buffer)
// hit to L1D bug (OI10-426, L1D LRU)
//    write and read to the same index at the same time, first load loop, second load loop

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>

#define TEST_ERROR 1
#define TEST_OK 0

/*
int __attribute__((section(".text.test"))) check_cpu_ppc_039(void)
{
    const uint32_t num_elem = TEST_OI10_CPU_039_ARRAY_SIZE / 4;
    const uint32_t val_A = 0x0;
    const uint32_t data_base_address = SRAM0_BASE + 0x8000;

    int32_t i;
    uint32_t register R1;
    uint32_t register R2;

    //stage 1
    rumboot_printf("Stage 1, write offsets\n");

    for(i = 0, R1 = val_A; i < num_elem; i++, R1+=4)
    {
        iowrite32( R1, data_base_address + 4*i);
        msync();
    }

    //stage 2
    rumboot_printf("Stage 2, read and compare offsets\n");
    for(i = (num_elem - 1),  R1 = (val_A + 4 * (num_elem - 1)); i >= 0; i--, R1-=4)
    {
        R2 = ioread32(data_base_address + 4*i);
        msync();
        if( R1 != R2 )
        {
            rumboot_printf("Error!  R1 == 0x%x and R2 == 0x%x do not coincide at virtual address == 0x%x!\n", R1, R2, data_base_address + 4*i);
            return TEST_ERROR;
        }
        //R1 -= 4;
    }

    //stage 3
    rumboot_printf("Stage 3, write inverted offset\n");
    R1 = val_A;
    for(i = 0; i < num_elem; i++)
    {
        iowrite32( ~R1, data_base_address + 4*i);
        R1 +=4;
    }
    msync();

    //stage 4
    rumboot_printf("Stage 4, read and compare inverted offset\n");
    R1 = val_A + 4 * (num_elem - 1);
    for(i = num_elem -1 ; i >= 0; i--)
    {
        R2 = ioread32(data_base_address + 4*i);
        if( (~R1) != (R2) )
        {
            rumboot_printf("Error!  ~R1 == 0x%x and R2 == 0x%x do not coincide at virtual address == 0x%x!\n", ~R1, R2, data_base_address + 4*i);
            return TEST_ERROR;
        }
        R1 -= 4;
    }

    return TEST_OK;
}
*/
/*
uint32_t read_dcr_reg(uint32_t addr) {
  uint32_t res;

  asm volatile   (
  "mfdcrx (%0), (%1) \n\t"       // mfdcrx RT, RA
  :"=r"(res)
  :"r"((uint32_t) addr)
  :
        );
  return res;
};
*/
// Inputs: start address, number of iteractions
// Outputs: status - CR register, error_addr
int __attribute__((section(".text.test"))) check_cpu_ppc_039(void)
{
//  uint32_t addr=SRAM0_BASE + 0x8000;
  uint32_t addr=0xC0000000; // did, 03.03.21
  uint32_t iterat=TEST_OI10_CPU_039_MEM_SIZE / 32;
//  uint32_t iterat=8;        //     8x32=256,     0x8x0x20=  0x100
//  uint32_t iterat=2048;     //  2048x32=64K,   0x800x0x20=0x10000
//  uint32_t iterat=16384;    // 16384x32=512K, 0x4000x0x20=0x80000
  uint32_t status, loop_type, error_addr;
    asm volatile (
        ".set  r0, 0               \n\t"
        ".set  r3, 3               \n\t"
        ".set  r4, 4               \n\t"
        ".set  r5, 5               \n\t"
        ".set  r6, 6               \n\t"
        ".set  r7, 7               \n\t"
        ".set  r8, 8               \n\t"
        ".set  r9, 9               \n\t"
        ".set r10, 10               \n\t"
        ".set r11, 11               \n\t"
        ".set r12, 12               \n\t"
        ".set r13, 13               \n\t"
        ".set r14, 14               \n\t"
        ".set r15, 15               \n\t"
        ".set r16, 16               \n\t"
        ".set r17, 17               \n\t"
        ".set r18, 18               \n\t"
        ".set r19, 19               \n\t"
        ".set r20, 20               \n\t"
        ".set r21, 21               \n\t"
        ".set r22, 22               \n\t"
        ".set r25, 25               \n\t" // CR copy
        ".set r26, 26               \n\t" // 1 - Direct values , 2 - Inverse Values
        "    addi r26, r0, 0x0    \n\t" // 1 - direct values error, 2 - inverse values errors, 0 - no errors
// clear only conflict lines
        "    addis r4, r0, 0x0    \n\t" // number of inerrations
        "    ori   r4, r4, 0x4    \n\t" // 
        "    mtctr r4             \n\t" // ctr - number of iterations, 4x8=32, cache line=128*8=1024, 1024/32=32, ctr=ctr/32
        "    mr r4, (%3)          \n\t" // addr
        "    addis r5, r0, 0x0    \n\t" //
        "l0_0: dcbz r5, r4        \n\t" //
        "    addi r5, r5, 0x80    \n\t"
        "    dcbz r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5, 0x80    \n\t"
        "    dcbz r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5, 0x80    \n\t"
        "    dcbz r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5, 0x80    \n\t"
        "    dcbz r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5, 0x80    \n\t"
        "    dcbz r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5, 0x80    \n\t"
        "    dcbz r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5, 0x80    \n\t"
        "    dcbz r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5, 0x80    \n\t" 
        "    addis r5, r5, 0x1    \n\t" // r5 = r5 + 0x0001_0000
        "    bdnz+ l0_0           \n\t"
        "    addis r4, r0, 0x0    \n\t" // number of interations
        "    ori   r4, r4, 0x4    \n\t" // number of interations = number of interations / 4 / 8
        "    mtctr r4             \n\t" // ctr - number of iterations, 4x8=32, cache line=128*8=1024, 1024/32=32, ctr=ctr/32
        "    mr r4, (%3)          \n\t" // addr
        "    addis r5, r0, 0x0    \n\t" //
        "l0_1: dcbf r5, r4        \n\t" //
        "    addi r5, r5,  0x80   \n\t"
        "    dcbf r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5,  0x80   \n\t"
        "    dcbf r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5,  0x80   \n\t"
        "    dcbf r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5,  0x80   \n\t"
        "    dcbf r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5,  0x80   \n\t"
        "    dcbf r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5,  0x80   \n\t"
        "    dcbf r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5,  0x80   \n\t"
        "    dcbf r5, r4          \n\t" // two commands in the store loop
        "    addi r5, r5,  0x80   \n\t" 
        "    addis r5, r5, 0x1    \n\t" // r5 = r5 + 0x0001_0000
        "    bdnz+ l0_1           \n\t"
// store loop 1
        "    mtctr (%4)           \n\t" // ctr - number of iterations
        "    mr r4, (%3)          \n\t" // addr
        "    mr r5, (%3)          \n\t" // data
        "    addi r4, r4, -0x4    \n\t" // 
        "l1: stwu r5, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    stwu r5, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    stwu r5, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    stwu r5, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    stwu r5, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    stwu r5, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    stwu r5, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    stwu r5, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t" 
        "    bdnz+ l1              \n\t"
// load loop 1
        "    addi  r4,  r4, 0x20     \n\t" // r4 - final addr
        "    addi  r5,  r4, -0x4      \n\t" // r4 - final addr
        "    addi  r6,  r5, -0x4      \n\t" // r4 - final addr
        "    addi  r7,  r6, -0x4      \n\t" // r4 - final addr
        "    addi  r8,  r7, -0x4      \n\t" // r4 - final addr
        "    addi  r9,  r8, -0x4      \n\t" // r4 - final addr
        "    addi r10,  r9, -0x4      \n\t" // r4 - final addr
        "    addi r11, r10, -0x4      \n\t" // r4 - final addr
        "    mtctr (%4)           \n\t" // ctr - number of iterations
        "l2: lwzu  r12, -32( r4)      \n\t" // two commands in the load loop, -4
        "    lwzu  r13, -32( r5)      \n\t" // two commands in the load loop
        "    lwzu  r14, -32( r6)      \n\t" // two commands in the load loop
        "    lwzu  r15, -32( r7)      \n\t" // two commands in the load loop
        "    lwzu  r16, -32( r8)      \n\t" // two commands in the load loop
        "    lwzu  r17, -32( r9)      \n\t" // two commands in the load loop
        "    lwzu  r18, -32(r10)      \n\t" // two commands in the load loop
        "    lwzu  r19, -32(r11)      \n\t" // two commands in the load loop
        "    cmpw cr0, r12,  r4      \n\t"
        "    cmpw cr1, r13,  r5      \n\t"
        "    cmpw cr2, r14,  r6      \n\t"
        "    cmpw cr3, r15,  r7      \n\t"
        "    cmpw cr4, r16,  r8      \n\t"
        "    cmpw cr5, r17,  r9      \n\t"
        "    cmpw cr6, r18, r10      \n\t"
        "    cmpw cr7, r19, r11      \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr0+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr2+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr3+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr4+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr5+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr6+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr7+eq   \n\t"
        "    bdnzt+ 4*cr1+eq,l2      \n\t"
        "    mfcr r25                \n\t" // full CR
        "    addi r26, r0, 0x1       \n\t" // r26 == 1, direct values
        "    bne cr1, exit1          \n\t"
// store loop 2
        "    xor r4, r4, r4       \n\t" 
        "    mtcr r4              \n\t" // clear CR 
        "    mtctr  (%4)          \n\t" // ctr - number of iterations
        "    mr r4, (%3)          \n\t" // addr
        "    mr r5, (%3)          \n\t" // data
        "    nor r6, r5, r5       \n\t" // r6 <- inverse value
        "    addi r4, r4, -0x4    \n\t" // 
        "l3: stwu r6, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    nor r6, r5, r5       \n\t" // r6 <- inverse value
        "    stwu r6, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    nor r6, r5, r5       \n\t" // r6 <- inverse value
        "    stwu r6, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    nor r6, r5, r5       \n\t" // r6 <- inverse value
        "    stwu r6, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    nor r6, r5, r5       \n\t" // r6 <- inverse value
        "    stwu r6, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    nor r6, r5, r5       \n\t" // r6 <- inverse value
        "    stwu r6, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    nor r6, r5, r5       \n\t" // r6 <- inverse value
        "    stwu r6, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    nor r6, r5, r5       \n\t" // r6 <- inverse value
        "    stwu r6, 4(r4)       \n\t" // two commands in the store loop
        "    addi r5, r5, 0x4     \n\t"
        "    nor r6, r5, r5       \n\t" // r6 <- inverse value
        "    bdnz+ l3              \n\t"
// load loop 2
        "    addi  r4,  r4, 0x20      \n\t" // r4 - final addr
        "    addi  r5,  r4, -0x4      \n\t" // r4 - final addr
        "    addi  r6,  r5, -0x4      \n\t" // r4 - final addr
        "    addi  r7,  r6, -0x4      \n\t" // r4 - final addr
        "    addi  r8,  r7, -0x4      \n\t" // r4 - final addr
        "    addi  r9,  r8, -0x4      \n\t" // r4 - final addr
        "    addi r10,  r9, -0x4      \n\t" // r4 - final addr
        "    addi r11, r10, -0x4      \n\t" // r4 - final addr
        "    mtctr (%4)               \n\t" // ctr - number of iterations
        "l4: lwzu  r12, -32( r4)      \n\t" // two commands in the load loop
        "    lwzu  r13, -32( r5)      \n\t" // two commands in the load loop
        "    lwzu  r14, -32( r6)      \n\t" // two commands in the load loop
        "    lwzu  r15, -32( r7)      \n\t" // two commands in the load loop
        "    lwzu  r16, -32( r8)      \n\t" // two commands in the load loop
        "    lwzu  r17, -32( r9)      \n\t" // two commands in the load loop
        "    lwzu  r18, -32(r10)      \n\t" // two commands in the load loop
        "    lwzu  r19, -32(r11)      \n\t" // two commands in the load loop
        "    nor r12, r12, r12        \n\t" // r12 <- inverse value
        "    nor r13, r13, r13        \n\t" // r13 <- inverse value
        "    nor r14, r14, r14        \n\t" // r14 <- inverse value
        "    nor r15, r15, r15        \n\t" // r15 <- inverse value
        "    nor r16, r16, r16        \n\t" // r16 <- inverse value
        "    nor r17, r17, r17        \n\t" // r17 <- inverse value
        "    nor r18, r18, r18        \n\t" // r18 <- inverse value
        "    nor r19, r19, r19        \n\t" // r19 <- inverse value
        "    cmpw cr0, r12,  r4       \n\t"
        "    cmpw cr1, r13,  r5       \n\t"
        "    cmpw cr2, r14,  r6       \n\t"
        "    cmpw cr3, r15,  r7       \n\t"
        "    cmpw cr4, r16,  r8       \n\t"
        "    cmpw cr5, r17,  r9       \n\t"
        "    cmpw cr6, r18, r10       \n\t"
        "    cmpw cr7, r19, r11       \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr0+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr2+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr3+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr4+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr5+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr6+eq   \n\t"
        "    crand 4*cr1+eq, 4*cr1+eq, 4*cr7+eq   \n\t"
        "    bdnzt+ 4*cr1+eq,l4        \n\t"
        "    mfcr r25                  \n\t"
        "    addi r26, r0, 0x2         \n\t" // r26 == 2, inverse values
        "exit1: mr (%1), r26              \n\t" // loop type
        "    mr (%2), r4               \n\t" // error_addr, first addr of unrolled loop
        "    mr (%0), r25              \n\t" // status
        : "=r"(status), "=r"(loop_type), "=r"(error_addr)
        : "r"((uint32_t) addr), "r"((uint32_t) iterat) 
        :"r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13","r14","r15","r16","r17","r18","r19","r20","r21","r22",
        "r25","r26","cr0","cr1","cr2","cr3","cr4","cr5","cr6","cr7","ctr"
    );
     // CR1 (CR[4:7]) collects CR0..CR7 from 8 loads
     // if (CR1.EQ == 0) it means that at least one load (CR1.EQ) gets wrong values
     // Look at CR0..CR7 for more information about wrong values loads
     // As loops were unrolled then no info about wrong values just address
    status = status & 0x22222222;
    if (status == 0x22222222) {   // 
      return TEST_OK;
    } else if (loop_type == 1)  { // Direct values
      rumboot_printf("Error! Direct value, stored value and loaded value are different at address block == 0x%x, Masked CR == 0x%x!\n", error_addr, status);
      return TEST_ERROR;
    } else {                      // Inverse values
      rumboot_printf("Error! Inverse value, stored value and loaded value are different at address block == 0x%x, Masked CR == 0x%x!\n", error_addr, status);
      return TEST_ERROR;
    };

}
