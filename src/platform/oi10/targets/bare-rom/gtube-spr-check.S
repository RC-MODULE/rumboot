#undef __linux__
#include <platform/trace.S.h>


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:

    gtube_init
    trace_msg "myYarr!!!\n"
    putchar 'Y'
    putchar 'a'
    putchar 'r'
    putchar 'r'
    putchar '!'
    putchar '!'
    putchar '\n'
    exit 0
