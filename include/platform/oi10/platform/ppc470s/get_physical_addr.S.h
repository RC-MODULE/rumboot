/*
 * get_physical_addr.S.h
 *
 *  Created on: May 24, 2018
 *      Author: d.arsentiev
 */

#ifndef GET_PHYSICAL_ADDR_S_H_
#define GET_PHYSICAL_ADDR_S_H_


.macro implement__get_physical_addr
/*r3 - parameter, 32bit effective address (EA)*/
/*r4,r3 - result, 42bit real address (int64_t)*/
/*if return value <0 (bit 31 = 1) then tlb entry was not found*/
/*r5,r6,r7,r8,r9,r10 - tmp registers*/
.global get_physical_addr
get_physical_addr:
    mfMMUCR r10
    /*clear r10[16:31]*/
    load_const r9, 0xFFFF0000
    and r10, r9, r10
    /*PID->MMUCR[STID]*/
    mfPID r9
    andi. r9, r9, 0xFFFF
    or r10, r9, r10
    mtspr 946,r10
    /*save EA*/
    ori r9, r3,0
    /*search for tlb entry*/
    tlbsx. r5,0,r3
    /*Check tlb entry found (CR[CR0[2]] == 1) 4.8.1. UG*/
    bne ra_error
    /*if WS=0 RT[54:63] <= ERPN, RT[32:60] <= tlbentry[EPN(0:19), V, TS, DSIZ(0:5), BLTD]*/
    tlbre r6, r5, 0
    srwi r6,r6, 0x4
    andi. r6,r6,0x3F
    /*r6[58:63] = DSIZ, swith(DSIZ):*/
    cmpwi cr7, r6, 0x0
    beq cr7, dsiz_4k
    cmpwi cr7, r6, 0x1
    beq cr7, dsiz_16k
    cmpwi cr7, r6, 0x3
    beq cr7, dsiz_64k
    cmpwi cr7, r6, 0x7
    beq cr7, dsiz_1m
    cmpwi cr7, r6, 0xF
    beq cr7, dsiz_16m
    cmpwi cr7, r6, 0x1F
    beq cr7, dsiz_256m
    cmpwi cr7, r6, 0x3F
    beq cr7, dsiz_1g
    /*default - error*/
    b ra_error
/*Offset mask  - R8*/
dsiz_4k:
    load_const r8, 20
    b dsiz_done
dsiz_16k:
    load_const r8, 18
    b dsiz_done
dsiz_64k:
    load_const r8, 16
    b dsiz_done
dsiz_1m:
    load_const r8, 12
    b dsiz_done
dsiz_16m:
    load_const r8, 8
    b dsiz_done
dsiz_256m:
    load_const r8, 4
    b dsiz_done
dsiz_1g:
    load_const r8, 2
    b dsiz_done
dsiz_done:
    /*if WS=1 RT[32:51] <= RPN, RT[54:63] <= ERPN*/
    tlbre r6, r5, 1
    /*r3 <= ERPN*/
    andi. r3,r6,0x3FF
    /*clear R6[52:63] by mask, shift RPN*/
    load_const r7, 0xFFFFF000
    and r6,r6,r7
    /*Offset*/
    load_const r10, 0xFFFFFFFF
    srw r10, r10, r8
    and r9,r9,r10
    /*get R4=RPN(r6) | OFFSET(r9)*/
    or r4, r6, r9
    blr
ra_error:
    /*return invalid value (< 0) if an error*/
    load_const r3, -1
    blr
.endm


#endif /* GET_PHYSICAL_ADDR_S_H_ */
