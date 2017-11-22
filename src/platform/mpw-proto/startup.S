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

.macro  load_const_link rD, constant
    lis     \rD, \constant@h
    ori     \rD, \rD, \constant@l
.endm


.global c_start
.global c_start1

.section ".text.startup","ax",@progbits
.global boot_image_entry0
boot_image_entry0:
    b rumboot_main

.global boot_image_entry1
boot_image_entry1:
    b endless_loop

endless_loop:
    b endless_loop

.global crash
crash:
    div 0,0,0
