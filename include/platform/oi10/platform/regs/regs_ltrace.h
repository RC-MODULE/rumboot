#ifndef LTRACE_REGS_H_
#define LTRACE_REGS_H_

#define LTR_TC          0x00
#define LTR_TS          0x01
#define LTR_LA1         0x02
#define LTR_LA2         0x03
#define LTR_WC          0x04
#define LTR_CT          0x05
#define LTR_CC1PH       0x06
#define LTR_CC1PL       0x07
#define LTR_CC1MH       0x08
#define LTR_CC1ML       0x09
#define LTR_CC2PH       0x0A
#define LTR_CC2PL       0x0B
#define LTR_CC2MH       0x0C
#define LTR_CC2ML       0x0D
#define LTR_CC3PH       0x0E
#define LTR_CC3PL       0x0F
#define LTR_CC3MH       0x10
#define LTR_CC3ML       0x11
#define LTR_CMH         0x12
#define LTR_CML         0x13
#define LTR_AI          0x14
#define LTR_AD          0x15

//default values
#define LTR_TC_DEFAULT        0x00
#define LTR_TS_DEFAULT        0x00
#define LTR_WC_DEFAULT        0x00
#define LTR_CT_DEFAULT        0x00
#define LTR_CC1PH_DEFAULT     0x00
#define LTR_CC1PL_DEFAULT     0x00
#define LTR_CC1MH_DEFAULT     0x00
#define LTR_CC1ML_DEFAULT     0x00
#define LTR_CC2PH_DEFAULT     0x00
#define LTR_CC2PL_DEFAULT     0x00
#define LTR_CC2MH_DEFAULT     0x00
#define LTR_CC2ML_DEFAULT     0x00
#define LTR_CC3PH_DEFAULT     0x00
#define LTR_CC3PL_DEFAULT     0x00
#define LTR_CC3MH_DEFAULT     0x00
#define LTR_CC3ML_DEFAULT     0x00
#define LTR_CMH_DEFAULT       0x00
#define LTR_CML_DEFAULT       0x00

//Masks
#define LTR_TC_MSK      0xFFFDFFF3
#define LTR_TS_MSK      0xFC000000
#define LTR_AI_MSK      0xC00000FF

#endif /* LTRACE_REGS_H_ */
