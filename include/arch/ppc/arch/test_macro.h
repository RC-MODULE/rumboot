#ifndef TEST_MACRO_H_
#define TEST_MACRO_H_


#include <stdint.h>
// #include <mivem_macro.h>


#define MEM8(addr)  (*(volatile uint8_t*)(addr))
#define MEM16(addr) (*(volatile uint16_t*)(addr))
#define MEM32(addr) (*(volatile uint32_t*)(addr))
#define MEM64(addr) (*(volatile uint64_t*)(addr))
#define MEM(addr) MEM32(addr)

#define BITS_CONTROL_BY_FEATURE(val, feat, mask)\
MACRO_BEGIN\
    if( feat ) SET_BITS_BY_MASK(val, mask);\
    else       CLEAR_BITS_BY_MASK(val, mask);\
MACRO_END

//interrupt stack frame size is 160
#define SAVE_CONTEXT_ASM \
asm ( \
    "stwu 1,-160(1)\n\t" \
    "stw 0, 156(1)\n\t" \
    "stmw 2, 36(1)\n\t"\
    \
    "mflr 0\n\t" \
    "stw 0,32(1)\n\t" \
    \
    "mfcr 0\n\t" \
    "stw 0,28(1)\n\t" \
    \
    "mfctr 0\n\t" \
    "stw 0,24(1)\n\t" \
    \
    "mfxer 0\n\t" \
    "stw 0,20(1)\n\t" \
);


//now R1 points to interrupt frame. Frame size is 160
#define RESTORE_CONTEXT_ASM \
asm (       \
    "lwz 0,20(1)\n\t" \
    "mtxer 0\n\t" \
    \
    "lwz 0,24(1)\n\t" \
    "mtctr 0\n\t" \
    \
    "lwz 0,28(1)\n\t" \
    "mtcr 0\n\t" \
    \
    "lwz 0,32(1)\n\t" \
    "mtlr 0\n\t" \
    \
    "lmw 2,36(1)\n\t" \
    "lwz 0,156(1)\n\t" \
    "addi 1,1,160\n\t" \
);


#define NON_CR_INTERRUPT_HANDLER \
    noncr_isr(void)  __attribute__((aligned(16))); \
    /*next function is a interrupt handler pointer (write this name to IVORx)*/ \
    void non_cr_interrupt_handler() __attribute__((aligned(16))); \
    asm (\
".align 4  \n\t"\
".global non_cr_interrupt_handler \n\t" \
"non_cr_interrupt_handler:\n\t");  \
    SAVE_CONTEXT_ASM   \
    asm ("    bl noncr_isr \n\t" );\
    RESTORE_CONTEXT_ASM \
    asm("    rfi  \n\t");\
    void noncr_isr

#define CR_INTERRUPT_HANDLER \
    cr_isr(void)  __attribute__((aligned(16))); \
    /*next function is a interrupt handler pointer (write this name to IVORx)*/ \
    void cr_interrupt_handler() __attribute__((aligned(16)));\
    asm (\
".align 4  \n\t"\
".global cr_interrupt_handler \n\t" \
"cr_interrupt_handler:\n\t");  \
    SAVE_CONTEXT_ASM   \
    asm ("    bl cr_isr \n\t" );\
    RESTORE_CONTEXT_ASM \
    asm("    rfci  \n\t");\
    void cr_isr


#define COMPARE_VALUES(received, expected, msg)     \
MACRO_BEGIN                                         \
    uint32_t rcv = received;                        \
    uint32_t exp = expected;                        \
    if (rcv!=exp)                                   \
    {                                               \
        rumboot_putstring(msg"\n");                         \
        rumboot_putstring("Received: ");                    \
        rumboot_puthex(rcv);                             \
        rumboot_putstring("Expected: ");                    \
        rumboot_puthex(exp);                             \
        test_event( EVENT_ASSERT );                 \
    }                                               \
MACRO_END


#endif /* TEST_MACRO_H_ */
