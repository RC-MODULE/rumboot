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
    load_const r5, 0x70075
    rumboot_puthex r5
    rumboot_putdump (BOOTROM_BASE+0xf000), 0x1000
    load_const r5, BOOTROM_BASE
    ori r5, r5, 0xf000
    load_const r6, 0x1000
    _rumboot_issdump r5, r6
    exit 0
