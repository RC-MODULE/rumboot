/*
 * test_l2c_im0_scrubber_apb.c
 *
 *  Created on: Mar 30, 2021
 *      Author: M.Chelyshev
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <platform/trace.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <platform/devices/l2c.h>
#include <platform/regs/regs_scrubber.h>


int scrubber_rw_check (uint32_t address, uint32_t test_value) {
    dcr_write(address, test_value);

    uint32_t tmp = dcr_read(address);
    if (tmp == test_value)
        return 0;
    else {
        rumboot_printf("Error at address %x: %x, but wait %x.\n", address, dcr_read(address), test_value);
        return 1;
    }
}

int scrubber_num_check (uint32_t address, uint32_t number) {
    uint32_t tmp = dcr_read(address);
    if (tmp == number)
        return 0;
    else {
        rumboot_printf("Error at address %x: %x, but wait %x.\n", address, dcr_read(address), number);
        return 1;
    }
}

int main() {
    rumboot_printf("\nStart of L2Cache and IM0 scrubbers APB test\n");
    
// Enable L2 Cache scrubbing    
    rumboot_printf( "L2 Cache scrubber\n");
    if (scrubber_num_check(L2C_SCRUB_BASE + SCRUB_ID , SCRUB_IDENTIFYER)) return 1;
    if (scrubber_num_check(L2C_SCRUB_BASE + SCRUB_VER, SCRUB_VERSION))    return 1;
    if (scrubber_rw_check (L2C_SCRUB_BASE + SCRUB_PERIOD, 0xa5a5a5a5))    return 1;
        
    if (scrubber_num_check(L2C_SCRUB_BASE + SCRUB_L2C_0 + SCRUB_X_NUMER, SCRUB_L2C_0_NUMBER)) return 1;
    if (scrubber_num_check(L2C_SCRUB_BASE + SCRUB_L2C_1 + SCRUB_X_NUMER, SCRUB_L2C_1_NUMBER)) return 1;
    if (scrubber_num_check(L2C_SCRUB_BASE + SCRUB_L2C_2 + SCRUB_X_NUMER, SCRUB_L2C_2_NUMBER)) return 1;
    if (scrubber_num_check(L2C_SCRUB_BASE + SCRUB_L2C_3 + SCRUB_X_NUMER, SCRUB_L2C_3_NUMBER)) return 1;
    if (scrubber_num_check(L2C_SCRUB_BASE + SCRUB_L2C_4 + SCRUB_X_NUMER, SCRUB_L2C_4_NUMBER)) return 1;
    if (scrubber_num_check(L2C_SCRUB_BASE + SCRUB_L2C_5 + SCRUB_X_NUMER, SCRUB_L2C_5_NUMBER)) return 1;
    if (scrubber_num_check(L2C_SCRUB_BASE + SCRUB_L2C_6 + SCRUB_X_NUMER, SCRUB_L2C_6_NUMBER)) return 1;
    if (scrubber_num_check(L2C_SCRUB_BASE + SCRUB_L2C_7 + SCRUB_X_NUMER, SCRUB_L2C_7_NUMBER)) return 1;

    rumboot_printf( "IM0 scrubber\n");
    if (scrubber_num_check(IM0_SCRUB_BASE + SCRUB_ID , SCRUB_IDENTIFYER)) return 1;
    if (scrubber_num_check(IM0_SCRUB_BASE + SCRUB_VER, SCRUB_VERSION))    return 1;
    if (scrubber_rw_check (IM0_SCRUB_BASE + SCRUB_PERIOD, 0x5a5a5a5a))    return 1;
    
    if (scrubber_num_check(IM0_SCRUB_BASE + SCRUB_IM0_0_1 + SCRUB_X_NUMER, SCRUB_IM0_0_1_NUMBER)) return 1;
    if (scrubber_num_check(IM0_SCRUB_BASE + SCRUB_IM0_2_3 + SCRUB_X_NUMER, SCRUB_IM0_2_3_NUMBER)) return 1;

    
    rumboot_printf("\nL2Cache and IM0 scrubbers APB test OK!\n");
    
    return 0;
}
