/*
 * emi.h
 *
 *  Created on: Jul 25, 2018
 *      Author: s.chernousov
 */

#ifndef EMI_H_
#define EMI_H_

#include <stdint.h>
#include <stdbool.h>

//bank0
#define EM2_BANK0_SIZE          0x20000000
#define SRAM0_SIZE              EM2_BANK0_SIZE
//bank1
#define EM2_BANK1_SIZE          0x20000000
#define SDRAM_SIZE              EM2_BANK1_SIZE
//bank2
#define EM2_BANK2_SIZE          0x10000000
#define SSRAM_SIZE              EM2_BANK2_SIZE
//bank3
#define EM2_BANK3_SIZE          0x10000000
#define PIPELINED_SIZE          EM2_BANK3_SIZE
//bank4
#define EM2_BANK4_SIZE          0x10000000
#define SRAM1_SIZE              EM2_BANK4_SIZE
//bank5
#define EM2_BANK5_SIZE          0x10000000
#define NOR_SIZE                EM2_BANK5_SIZE

void emi_init ();
void emi_init_impl (uint32_t emi_dcr_base, uint32_t plb6mcif2_dcr_base, uint32_t puaba);

#endif /* EMI_H_ */
