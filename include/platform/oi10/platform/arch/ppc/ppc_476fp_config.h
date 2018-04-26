#ifndef PPC_476FP_CONFIG_H
#define PPC_476FP_CONFIG_H

#undef __linux__
#include <ppc-asm.h> /*for r0-r31 etc*/

/*r0 defined in standard ppc-asm.h*/
#define r1  1   /*same as sp in ppc-asm.h*/
#define r2  2   /*same as toc in ppc-asm.h*/
/*r3-r31 defined in standard ppc-asm.h*/

/*Define all SPR codes according to User Manual v.2.1 p.43*/
#define SPR_CCR0        947
#define SPR_CCR1        888
#define SPR_CCR2        889
#define SPR_CTR         9
#define SPR_CSRR0       58
#define SPR_CSRR1       59
#define SPR_DAC1        316
#define SPR_DAC2        317
#define SPR_DCDBTRH     925
#define SPR_DCDBTRL     924
#define SPR_DEAR        61
#define SPR_DVC1        318
#define SPR_DVC2        319
#define SPR_DCESR       850
#define SPR_DCRIPR      891
#define SPR_DBCR0       308
#define SPR_DBCR1       309
#define SPR_DBCR2       310
#define SPR_DBDR        1011

/*DBSR for read and clear operations*/
#define SPR_DBSR_RC     304

/*DBSR for write operation*/
#define SPR_DBSR_W      816

#define SPR_DEC         22
#define SPR_DECAR       54
#define SPR_ESR         62
#define SPR_ICESR       851
#define SPR_IAC1        312
#define SPR_IAC2        313
#define SPR_IAC3        314
#define SPR_IAC4        315
#define SPR_ICDBDR0     979
#define SPR_ICDBDR1     980
#define SPR_ICDBTRH     927
#define SPR_ICDBTRL     926
#define SPR_IOCCR       860
#define SPR_IOCR1       861
#define SPR_IOCR2       862
#define SPR_XER         1
#define SPR_IVOR0       400
#define SPR_IVOR1       401
#define SPR_IVOR2       402
#define SPR_IVOR3       403
#define SPR_IVOR4       404
#define SPR_IVOR5       405
#define SPR_IVOR6       406
#define SPR_IVOR7       407
#define SPR_IVOR8       408
#define SPR_IVOR9       409
#define SPR_IVOR10      410
#define SPR_IVOR11      411
#define SPR_IVOR12      412
#define SPR_IVOR13      413
#define SPR_IVOR14      414
#define SPR_IVOR15      415
#define SPR_IVPR        63
#define SPR_LR          8
#define SPR_MCSRR0      570
#define SPR_MCSRR1      571

/*MCSR for read and write operations*/
#define SPR_MCSR_RW     572

/*MCSR for clear operation*/
#define SPR_MCSR_C      828

/*MSR code is not defined in User Manual p.44 v2.1. Use mtmsr and mfmsr instructions instead*/
/*#define SPR_MSR*/

#define SPR_MMUBE0      820
#define SPR_MMUBE1      821
#define SPR_MMUCR       946
#define SPR_PMUCC0      858

/*PMUCC0 for User mode read operation*/
#define SPR_PMUCC0_UR   842

#define SPR_PID         48
#define SPR_PIR         286
#define SPR_PVR         287
#define SPR_PWM         886
#define SPR_RMPD        825
#define SPR_RSTCFG      923
#define SPR_SRR0        26
#define SPR_SRR1        27
#define SPR_SPRG0       272
#define SPR_SPRG1       273
#define SPR_SPRG2       274
#define SPR_SPRG3       275

/*SPRG3 for user mode read operation*/
#define SPR_SPRG3_UR    259

/*SPRG4 for user mode read operation*/
#define SPR_SPRG4_UR    260

/*SPRG5 for user mode read operation*/
#define SPR_SPRG5_UR    261

/*SPRG6 for user mode read operation*/
#define SPR_SPRG6_UR    262

/*SPRG7 for user mode read operation*/
#define SPR_SPRG7_UR    263

#define SPR_SPRG4       276
#define SPR_SPRG5       277
#define SPR_SPRG6       278
#define SPR_SPRG7       279
#define SPR_SPRG8       604
#define SPR_SSPCR       830

/*TBL for read operation*/
#define SPR_TBL_R       268

/*TBL for write operation*/
#define SPR_TBL_W       284

/*TBU for read operation*/
#define SPR_TBU_R       269

/*TBU for write operation*/
#define SPR_TBU_W       285

#define SPR_TCR         340

/*TSR for read and clear operations*/
#define SPR_TSR_RC      336

/*TSR for write operation*/
#define SPR_TSR_W       848
#define SPR_ISPCR       829
#define SPR_USPCR       831
#define SPR_USPGR0      256

/*Condition register field numbers*/
#define cr0    0
#define cr1    1
#define cr2    2
#define cr3    3
#define cr4    4
#define cr5    5
#define cr6    6
#define cr7    7

#define PPC0_CORE_ID    0b00
#define PPC1_CORE_ID    0b01

#endif  // PPC_476FP_CONFIG_H
