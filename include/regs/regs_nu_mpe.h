#ifndef REGS_NU_MPE_H
#define REGS_NU_MPE_H


#define NU_MPE_BUF00 (00<<11)
#define NU_MPE_BUF01 (01<<11)
#define NU_MPE_BUF02 (02<<11)
#define NU_MPE_BUF03 (03<<11)
#define NU_MPE_BUF04 (04<<11)
#define NU_MPE_BUF05 (05<<11)
#define NU_MPE_BUF06 (06<<11)
#define NU_MPE_BUF07 (07<<11)
#define NU_MPE_BUF08 (08<<11)
#define NU_MPE_BUF09 (09<<11)
#define NU_MPE_BUF10 (10<<11)
#define NU_MPE_BUF11 (11<<11)
#define NU_MPE_BUF12 (12<<11)
#define NU_MPE_BUF13 (13<<11)
#define NU_MPE_BUF14 (14<<11)
#define NU_MPE_BUF15 (15<<11)

// MPE_MA
#define MPE_CMD_IRC   0x400
#define MPE_CMD_IRCW  0x500
#define MPE_CMD_ICM   0x600
#define MPE_CMD_ICMW  0x700
#define MPE_COMMON_WA               0x000
#define MPE_COMMON_DA               0x004
#define MPE_COMMON_VRA              0x008
#define MPE_COMMON_NR               0x00C
#define MPE_COMMON_IN_FORMAT        0x010
#define MPE_COMMON_NORM_PARAM       0x014
#define MPE_COMMON_OUT_SATURATION   0x074
#define MPE_COMMON_IN_BUF_CFG       0x018
#define MPE_COMMON_D_BIAS           0x01C
#define MPE_COMMON_BUF_FULL_SET     0x020
#define MPE_COMMON_BUF_FULL_RST     0x024
#define MPE_ADD_CountM      0x028 
#define MPE_CMP_CountM      0x02C 
#define MPE_ADD_CountY      0x030 
#define MPE_CMP_CountY      0x034 
#define MPE_ADD_CountX      0x038 
#define MPE_CMP_CountX      0x03C 
#define MPE_ADD_CountPLC    0x040 
#define MPE_CMP_CountPLC    0x044 
#define MPE_ADD_CountLCZ    0x048 
#define MPE_CMP_CountLCZ    0x04C 
#define MPE_ADD_CountPPS    0x050 
#define MPE_CMP_CountPPS    0x054 
#define MPE_ADD_CountJ      0x058
#define MPE_CMP_CountJ      0x05C 
#define MPE_ADD_CountI1     0x060 
#define MPE_CMP_CountI1     0x064 
#define MPE_ADD_CountI0     0x068 
#define MPE_CMP_CountI0     0x06C 
#define MPE_NULL            0x070 
#define MPE_STATUS          0x080 

// MPE_DMA Comtrol
#define DMA_START                 0x0000
#define DMA_STS                   0x0004
#define MAINCNT_Sha               0x0008
#define MAINCNT_RD                0x000C
#define MAINCNT_WR                0x0010
// MPE_DMA Wr Channel
#define X_Cfg_MSha                0x0104
#define WR_THRECtrl_MSha          0x0108
#define WR_DECCtrl_MSha           0x010C
#define CHCfg_MSha                0x0110
#define BADR_MSha                 0x0114
#define LADR_MSha                 0x0118
#define PLC_CntSha_MSha           0x011C
#define PLC_ThreSha_MSha          0x0120
#define PLC_CUR_Cnt               0x0124
#define PLC_Threshold             0x0128
#define VLC_CntSha_MSha           0x012C
#define VLC_ThreSha_MSha          0x0130
#define VLC_CUR_Cnt               0x0134
#define VLC_Threshold             0x0138
#define WR_Bias1CntSha_MSha       0x013C
#define WR_Bias1CntCmp_MSha       0x0140
#define Bias1_Cnt                 0x0144
#define WR_Bias2CntSha_MSha       0x0148
#define WR_Bias2CntCmp_MSha       0x014C
#define Bias2_Cnt                 0x0150
#define WR_Bias3CntSha_MSha       0x0154
#define WR_Bias3CntCmp_MSha       0x0158
#define Bias3_Cnt                 0x015C
#define WR_Bias4CntSha_MSha       0x0160
#define WR_Bias4CntCmp_MSha       0x0164
#define Bias4_Cnt                 0x0168
#define WR_Bias5CntSha_MSha       0x016C
#define WR_Bias5CntCmp_MSha       0x0170
#define Bias5_Cnt                 0x0174
#define WR_Bias6CntSha_MSha       0x0178
#define WR_Bias6CntCmp_MSha       0x017C
#define Bias6_Cnt                 0x0180
// MPE_DMA Rd Channel
#define BIASCtrl_MSha             0x0204
#define RD_THRECtrl_MSha          0x0208
#define RD_DECCtrl_MSha           0x020C
#define PADCtrl_MSha              0x0210
#define BFCA_MSha                 0x0214
#define AOffset_MSha              0x0218
#define CURRADR                   0x021C
#define LPXOffset_MSha            0x0220
#define RPXOffset_MSha            0x0224
#define LPadXTreshold             0x0228
#define RPadXTreshold             0x022C
#define TPadYThreshold_MSha       0x0230
#define BPadYThreshold_MSha       0x0234
#define CntSha_MSha               0x0238
#define CntThresholdSha_MSha      0x023C
#define SecThre                   0x0240
#define SecCnt                    0x0244
#define Bias1Sha_MSha             0x0248
#define Bias1AOffset_MSha         0x024C
#define Bias1Adr                  0x0250
#define RD_Bias1CntSha_MSha       0x0254
#define Bias1CntOffset_MSha       0x0258
#define Bias1CntThresholdSha_MSha 0x025C
#define RD_Bias1CntCmp_MSha       0x0260
#define Bias1BCnt                 0x0264
#define Bias1BThre                0x0268
#define Bias2Sha_MSha             0x026C
#define Bias2AOffset_MSha         0x0270
#define Bias2Adr                  0x0274
#define RD_Bias2CntSha_MSha       0x0278
#define Bias2CntOffset_MSha       0x027C
#define Bias2CntThresholdSha_MSha 0x0280
#define RD_Bias2CntCmp_MSha       0x0284
#define Bias2BCnt                 0x0288
#define Bias2BThre                0x028C
#define Bias3Sha_MSha             0x0290
#define Bias3AOffset_MSha         0x0294
#define Bias3Adr                  0x0298
#define RD_Bias3CntSha_MSha       0x029C
#define Bias3CntOffset_MSha       0x02A0
#define Bias3CntThresholdSha_MSha 0x02A4
#define RD_Bias3CntCmp_MSha       0x02A8
#define Bias3BCnt                 0x02AC
#define Bias3BThre                0x02B0
#define Bias4Sha_MSha             0x02B4
#define Bias4AOffset_MSha         0x02B8
#define Bias4Adr                  0x02BC
#define RD_Bias4CntSha_MSha       0x02C0
#define Bias4CntOffset_MSha       0x02C4
#define Bias4CntThresholdSha_MSha 0x02C8
#define RD_Bias4CntCmp_MSha       0x02CC
#define Bias4BCnt                 0x02D0
#define Bias4BThre                0x02D4
#define Bias5Sha_MSha             0x02D8
#define Bias5AOffset_MSha         0x02DC
#define Bias5Adr                  0x02E0
#define RD_Bias5CntSha_MSha       0x02E4
#define Bias5CntOffset_MSha       0x02E8
#define Bias5CntThresholdSha_MSha 0x02EC
#define RD_Bias5CntCmp_MSha       0x02F0
#define Bias5BCnt                 0x02F4
#define Bias5BThre                0x02F8
#define Bias6Sha_MSha             0x02FC
#define Bias6AOffset_MSha         0x0300
#define Bias6Adr                  0x0304
#define Bias6CntSha_MSha          0x0308
#define Bias6CntOffset_MSha       0x030C
#define Bias6CntThresholdSha_MSha 0x0310
#define Bias6CntCmp_MSha          0x0314
#define Bias6BCnt                 0x0318
#define Bias6BThre                0x031С
#define Bias7Sha_MSha             0x0320
#define Bias7AOffset_MSha         0x0324
#define Bias7Adr                  0x0328
#define Bias7CntSha_MSha          0x032С
#define Bias7CntOffset_MSha       0x0330
#define Bias7CntCmp_MSha          0x0334
#define Bias7BCnt                 0x0338
#define LPXDR_MSha                0x033C
#define RPXDR_MSha                0x0340
#define TPYDR_MSha                0x0344
#define BPYDR_MSha                0x0348

#endif
