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

typedef struct
{
   uint8_t BTYP;
   uint8_t PTYP;
   uint8_t SRDY;
   uint8_t TWR;
   uint8_t SST;
   uint8_t T_SSOE;
   uint8_t T_SOE;
   uint8_t T_CYC;
   uint16_t T_RDY;
   uint8_t T_DEL;
   uint8_t UWEB;
} emi_ssx_reg_cfg;

typedef struct
{
   uint8_t CSP;
   uint8_t SDS;
   uint8_t CL;
   uint8_t T_RDL;
   uint8_t SI;
   uint8_t T_RCD;
   uint8_t T_RAS;
} emi_sdx_reg_cfg;

typedef struct
{
    emi_ssx_reg_cfg ssx_cfg;
    emi_sdx_reg_cfg sdx_cfg;
} emi_init_bank_cfg;

void emi_init ();
void emi_init_impl (uint32_t emi_dcr_base, uint32_t plb6mcif2_dcr_base, uint32_t puaba);

#endif /* EMI_H_ */
