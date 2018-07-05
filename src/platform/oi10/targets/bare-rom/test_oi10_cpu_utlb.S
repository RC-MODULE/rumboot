
#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point


.section ".init.text","ax",@progbits

rumboot_entry_point:
    gtube_init
    load_const  r0, 0x00000000  /* make r0 "all zeros" source */
    load_const  r1, 0xFFFFFFFF  /* make r1 "all ones" source */

    rumboot_putstring   "Init SPRs...\n"


    rumboot_putstring   "TEST IS NOT READY!!!\n"
    b           test_error

test_ok:
    rumboot_putstring "TEST OK\n"
    test_event EVENT_OK
    exit RESULT_OK

test_error:
    rumboot_putstring "TEST ERROR\n"
    test_event  EVENT_ERROR
    exit RESULT_ERROR

stop:
    b stop

