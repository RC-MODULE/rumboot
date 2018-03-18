/*
 * startup.S
 *
 *  Created on: Mar 8, 2018
 *      Author: v.strukov
 */
//#if 0
//#endif
.global rumboot_entry_point
.global rumboot_reset_handler
.global boot_start


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point


.section ".init.text","ax",@progbits


rumboot_entry_point:
rumboot_reset_handler:

endless_loop:
    b           endless_loop


