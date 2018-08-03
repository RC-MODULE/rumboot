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
#include <platform/regs/fields/emi.h>

//bank0
#define EM2_BANK0_SIZE          0x20000000
#define SRAM0_SIZE              0x1000000 //16MB //EM2_BANK0_SIZE
//bank1
#define EM2_BANK1_SIZE          0x20000000
#define SDRAM_SIZE              0x4000000 //64MB //EM2_BANK1_SIZE
//bank2
#define EM2_BANK2_SIZE          0x10000000
#define SSRAM_SIZE              0x400000  //4MB  //EM2_BANK2_SIZE
//bank3
#define EM2_BANK3_SIZE          0x10000000
#define PIPELINED_SIZE          0x400000  //4MB  //EM2_BANK3_SIZE
//bank4
#define EM2_BANK4_SIZE          0x10000000
#define SRAM1_SIZE              0x400000  //4MB  //EM2_BANK4_SIZE
//bank5
#define EM2_BANK5_SIZE          0x10000000
#define NOR_SIZE                0x400000  //4MB  //EM2_BANK5_SIZE

typedef enum
{
    emi_b0_sram0 = 0,
    emi_b1_sdram = 1,
    emi_b2_ssram = 2,
    emi_b3_pipelined = 3,
    emi_b4_sram1 = 4,
    emi_b5_nor = 5,
    emi_bank_all = 0xF,
} emi_bank_num;

typedef enum
{
    emi_ecc_off = 0,
    emi_ecc_on = 1,
} emi_ecc_status;

typedef struct
{
   ssx_btyp_t   BTYP;
   ssx_ptyp_t   PTYP;
   ssx_srdy_t   SRDY;
   ssx_twr_t    TWR;
   ssx_sst_t    SST;
   ssx_tssoe_t  T_SSOE;
   ssx_tsoe_t   T_SOE;
   ssx_tcyc_t   T_CYC;
   uint16_t     T_RDY;
   ssx_tdel_t   T_DEL;
} emi_ssx_reg_cfg;

typedef struct
{
   sdx_csp_t    CSP;
   sdx_sds_t    SDS;
   sdx_cl_t     CL;
   sdx_trdl_t   T_RDL;
   sdx_si_t     SI;
   sdx_trcd_t   T_RCD;
   sdx_tras_t   T_RAS;
} emi_sdx_reg_cfg;

typedef struct
{
    emi_ssx_reg_cfg ssx_cfg;
    emi_sdx_reg_cfg sdx_cfg;
} emi_bank_cfg;

typedef struct
{
    rfc_trfc_t TRFC;
    uint16_t   RP;
} emi_rfc_cfg;

void emi_init         ();
void emi_init_impl    (uint32_t emi_dcr_base, uint32_t plb6mcif2_dcr_base, uint32_t puaba);
void emi_get_bank_cfg (uint32_t emi_base, emi_bank_num num_bank, emi_bank_cfg* bn_cfg);
void emi_set_bank_cfg (uint32_t emi_base, emi_bank_num num_bank, emi_bank_cfg* bn_cfg);
void emi_set_rfc      (uint32_t emi_base, emi_rfc_cfg* rfc);
void emi_get_rfc      (uint32_t emi_base, emi_rfc_cfg* rfc);
void emi_set_ecc      (uint32_t emi_base, emi_bank_num num_bank, emi_ecc_status ecc_stat);

#endif /* EMI_H_ */
