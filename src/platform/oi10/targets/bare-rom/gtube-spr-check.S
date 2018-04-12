#undef __linux__
#include <ppc-asm.h>
#define SPR_SPRG0       272
#define SPR_SPRG1       273
#define SPR_SPRG2       274
#define SPR_SPRG3       275
#define SPR_SPRG4       276
#define SPR_SPRG5       277
#define SPR_SPRG6       278
#define SPR_SPRG7       279
#define SPR_SPRG8       604
#define r1  1
#define r2  2


.macro  load_const rD, constant
.if ((\constant & 0xFFFF8000) == 0xFFFF8000) || ((\constant & 0xFFFF8000) == 0x00000000)
    li      \rD, \constant
.else
    lis     \rD, \constant@h
.if ((\constant & 0x0000FFFF) != 0x00000000)
    ori     \rD, \rD, \constant@l
.endif
.endif
.endm

.macro gtube_init
    load_const  r1, 0x0003c0de;
    mtspr       SPR_SPRG0, r1
.endm

.macro putchar char
    load_const  r2, 2;
    load_const  r1, \char; /* test stdout */
    mtspr       SPR_SPRG2, r1

    mtspr       SPR_SPRG1, r2
    mtspr       SPR_SPRG1, r0
.endm

.macro exit code
    load_const  r2, 1;
    load_const  r1, \code; /* test stdout */
    mtspr       SPR_SPRG2, r1

    mtspr       SPR_SPRG1, r2
    mtspr       SPR_SPRG1, r0
.endm


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:
    load_const  r0, 0x00000000  // make r0 "all zeros" source
    mtspr       SPR_SPRG0, r0
    mtspr       SPR_SPRG1, r0
    mtspr       SPR_SPRG2, r0
    mtspr       SPR_SPRG3, r0
    mtspr       SPR_SPRG4, r0
    mtspr       SPR_SPRG5, r0
    mtspr       SPR_SPRG6, r0
    mtspr       SPR_SPRG7, r0
    mtspr       SPR_SPRG8, r0

    gtube_init
    putchar 'Y'
    putchar 'a'
    putchar 'r'
    putchar 'r'
    putchar '!'
    putchar '!'
    putchar '\n'
    exit 0
