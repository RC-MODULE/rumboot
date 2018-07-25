/*
 * emi.c
 *
 *  Created on: Jul 25, 2018
 *      Author: s.chernousov
 */

#include <stdint.h>
#include <stdbool.h>
#include <platform/devices.h>
#include <platform/reg_access/reg_access_emi.h>
#include <platform/devices/plb6mcif2.h>

void emi_init_impl (uint32_t emi_dcr_base, uint32_t plb6mcif2_dcr_base, uint32_t puaba)
{
    //init bridge
    plb6mcif2_simple_init( plb6mcif2_dcr_base,  puaba );

    //init bank0
    //TODO:
    emi_dcr_write_EMI_SS0 (emi_dcr_base, 0x00);

    //init bank1
    //TODO:

    //init bank2
    //TODO:

    //init bank3
    //TODO:

    //init bank4
    //TODO:

    //init bank5
    //TODO:
    emi_dcr_write_EMI_SS5 (emi_dcr_base, 0x10);
    emi_dcr_write_EMI_BUSEN(emi_dcr_base, 0x01);
}

void emi_init ()
{
    emi_init_impl (DCR_EM2_EMI_BASE, DCR_EM2_PLB6MCIF2_BASE, 0x00);
}
