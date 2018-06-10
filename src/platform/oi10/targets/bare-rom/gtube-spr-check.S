#undef __linux__
#include <platform/trace.S.h>


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:

    gtube_init
    rumboot_putstring "myYarr!!!\n"
    load_const r5, 0x70075
    rumboot_puthex r5
    putchar 'Y'
    putchar 'a'
    putchar 'r'
    putchar 'r'
    putchar '!'
    putchar '!'
    putchar '\n'
    exit 0
