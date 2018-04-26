#undef __linux__
#include <platform/trace.S.h>

.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:
    load_const r0, 0x00
    load_const r1, 0x01
    load_const r2, 0x02
    load_const r3, 0x03
    load_const r4, 0x04
    load_const r5, 0x05
    load_const r6, 0x06
    load_const r7, 0x07
    load_const r8, 0x08
    load_const r9, 0x09
    load_const r10, 0x0A
    load_const r11, 0x0B
    load_const r12, 0x0C
    load_const r13, 0x0D
    load_const r14, 0x0E
    load_const r15, 0x0F
    load_const r16, 0x10
    load_const r17, 0x11
    load_const r18, 0x12
    load_const r19, 0x13
    load_const r20, 0x14
    load_const r21, 0x15
    load_const r22, 0x16
    load_const r23, 0x17
    load_const r24, 0x18
    load_const r25, 0x19
    load_const r26, 0x1A
    load_const r27, 0x1B
    load_const r28, 0x1C
    load_const r29, 0x1D
    load_const r30, 0x1E
    load_const r31, 0x1F

    gtube_init
    rumboot_putstring "TEST OK\n"
    exit 0
