#undef __linux__
#include <platform/trace.S.h>
#include <platform/devices.h>

.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:

    gtube_init
    test_event_send_test_id "test_iss_base"
    rumboot_putstring "myYarr!!!\n"
    load_const r15, 0x70075
    rumboot_puthex r15
    load_const r15, BOOTROM_BASE
    ori r15, r15, 0xf000
    load_const r16, 0x1000
    rumboot_putdump r15, r16
    rumboot_issdump r15, r16
/*    putchar 'Y'
    putchar 'a'
    putchar 'r'
    putchar 'r'
    putchar '!'
    putchar '!'
    putchar '\n'
*/    exit 0
