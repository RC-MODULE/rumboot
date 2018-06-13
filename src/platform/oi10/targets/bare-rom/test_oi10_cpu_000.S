
#undef __linux__

#include <platform/trace.S.h>

#define SAVE_MEM_ADDR	0xC0011000

.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:
    gtube_init
    load_const r0,  0xFF00
    load_const r1,  0xFE01
    load_const r2,  0xFD02
    load_const r3,  0xFC03
    load_const r4,  0xFB04
    load_const r5,  0xFA05
    load_const r6,  0xF906
    load_const r7,  0xF807
    load_const r8,  0xF708
    load_const r9,  0xF609
    load_const r10, 0xF50A
    load_const r11, 0xF40B
    load_const r12, 0xF30C
    load_const r13, 0xF20D
    load_const r14, 0xF10E
    load_const r15, 0xF00F
    load_const r16, 0x0F10
    load_const r17, 0x0E11
    load_const r18, 0x0D12
    load_const r19, 0x0C13
    load_const r20, 0x0B14
    load_const r21, 0x0A15
    load_const r22, 0x0916
    load_const r23, 0x0817
    load_const r24, 0x0718
    load_const r25, 0x0619
    load_const r26, 0x051A
    load_const r27, 0x041B
    load_const r28, 0x031C
    load_const r29, 0x021D
    load_const r30, 0x011E
    load_const r31, 0x001F

    rumboot_putstring "TEST OK\n"
    exit RESULT_OK


