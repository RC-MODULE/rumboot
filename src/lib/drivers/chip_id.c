/*
 * chip_id.c
 *
 *  Created on: Jul 20, 2021
 *      Author: M.Chelyshev
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>

#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <platform/trace.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <platform/devices/l2c.h>

#include <devices/chip_id.h>

/*
 * Chip definition function
 *  Use specific registers to define chip 
 *!  Be careful!
 *!   'if' statements order matters!
 */
enum chip_ip get_chip_id() {
    enum chip_ip id_marker = UNKNOWN;
    if (dcr_read(0x800E0000) == 0x0){ // Check L2Cach scrubber is absent
        id_marker = OI10v3;
    }
    if (dcr_read(0x800E0000) == 0x62726353){ // Check L2Cach scrubber identifier
        id_marker = O32T;
    }
    return id_marker;
}