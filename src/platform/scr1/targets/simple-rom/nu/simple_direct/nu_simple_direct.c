#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices.h> 

#include <regs/regs_na.h>
#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 

ConfigMPE cfg_mpe;
ConfigVPE cfg_vpe;
ConfigPPE cfg_ppe;
ConfigREGPPE cfg_reg_ppe;

int PPE_ENABLED;


void fill_cfgs(
  ConfigMPE* cfg_mpe, 
  ConfigVPE* cfg_vpe, 
  ConfigPPE* cfg_ppe, 
  ConfigREGPPE* cfg_reg_ppe, 
  Enable PPE_ENABLED
) {
  
cfg_mpe->   H        = 1    ;
cfg_mpe->   W        = 1    ;
cfg_mpe->   C        = 2048 ;
cfg_mpe->   Tp       = 0    ;
cfg_mpe->   Bp       = 0    ;
cfg_mpe->   Lp       = 0    ;
cfg_mpe->   Rp       = 0    ;
cfg_mpe->   R        = 1    ;
cfg_mpe->   S        = 1    ;
cfg_mpe->   Ds       = 1    ;
cfg_mpe->   Dr       = 1    ;
cfg_mpe->   Sw       = 1    ;
cfg_mpe->   Sh       = 1    ;
cfg_mpe->   K        = 1    ;
cfg_mpe->   dt       = DataType_Int16 ;
cfg_mpe->   rnd_mode = RoundMode_HalfAwayFromZero;
cfg_mpe->   sat_en   = Enable_En;
cfg_mpe->   rnd_size = 0 ;
  
cfg_mpe->ma_config.ADD_CountI0  = 1   ;
cfg_mpe->ma_config.CMP_CountI0  = 8   ;
cfg_mpe->ma_config.ADD_CountI1  = 16  ;
cfg_mpe->ma_config.CMP_CountI1  = 1   ;
cfg_mpe->ma_config.ADD_CountJ   = 1   ;
cfg_mpe->ma_config.CMP_CountJ   = 1   ;
cfg_mpe->ma_config.ADD_CountPPS = 1   ;
cfg_mpe->ma_config.CMP_CountPPS = 1   ;
cfg_mpe->ma_config.ADD_CountLCZ = 1   ;
cfg_mpe->ma_config.CMP_CountLCZ = 32  ;
cfg_mpe->ma_config.ADD_CountPLC = 1   ;
cfg_mpe->ma_config.CMP_CountPLC = 1   ;
cfg_mpe->ma_config.ADD_CountX   = 128 ;
cfg_mpe->ma_config.CMP_CountX   = 1   ;
cfg_mpe->ma_config.ADD_CountY   = 1   ;
cfg_mpe->ma_config.CMP_CountY   = 1   ;
cfg_mpe->ma_config.ADD_CountM   = 128 ;
cfg_mpe->ma_config.CMP_CountM   = 1   ;
cfg_mpe->ma_config.BRDR   = 256       ;
cfg_mpe->ma_config.WA     = 256       ;
cfg_mpe->ma_config.DA     = 0         ;
cfg_mpe->ma_config.VRA    = 0         ;
cfg_mpe->ma_config.NR     = 0         ;
cfg_mpe->ma_config.D_BIAS = 1         ;
cfg_mpe->ma_config.D_NULL = 16777215  ;

cfg_mpe->dma_d_config.   MAINCNT = 0;

cfg_mpe->dma_d_config.rdma.   AOffset = 0x0                      ;
cfg_mpe->dma_d_config.rdma.   LPXOffset = 0                      ;
cfg_mpe->dma_d_config.rdma.   RPXOffset = 4096                   ;
cfg_mpe->dma_d_config.rdma.   TPYOffset = 0                      ;
cfg_mpe->dma_d_config.rdma.   BPYOffset = 4096                   ;
cfg_mpe->dma_d_config.rdma.   CntSha = 127                       ;
cfg_mpe->dma_d_config.rdma.   CntThresholdSha = 4095             ;
cfg_mpe->dma_d_config.rdma.   ThreCtrl = 0                       ;
cfg_mpe->dma_d_config.rdma.   LPXEn = Enable_NotEn               ;
cfg_mpe->dma_d_config.rdma.   RPXEn = Enable_NotEn               ;
cfg_mpe->dma_d_config.rdma.   TPYEn = Enable_NotEn               ;
cfg_mpe->dma_d_config.rdma.   BPYEn = Enable_NotEn               ;
cfg_mpe->dma_d_config.rdma.   LPXData = 0x0                      ;
cfg_mpe->dma_d_config.rdma.   RPXData = 0x0                      ;
cfg_mpe->dma_d_config.rdma.   TPYData = 0x0                      ;
cfg_mpe->dma_d_config.rdma.   BPYData = 0x0                      ;

cfg_mpe->dma_d_config.rdma.Bias[0].   BiasEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[0].   ThreCtrl = 0               ;
cfg_mpe->dma_d_config.rdma.Bias[0].   DecCtrl = 0                ;
cfg_mpe->dma_d_config.rdma.Bias[0].   PXBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[0].   PYBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[0].   Bias = 0x0                 ;
cfg_mpe->dma_d_config.rdma.Bias[0].   AOffset = 0x0              ;
cfg_mpe->dma_d_config.rdma.Bias[0].   CntSha = 0                 ;
cfg_mpe->dma_d_config.rdma.Bias[0].   CntOffsetEn = Enable_NotEn        ;
cfg_mpe->dma_d_config.rdma.Bias[0].   CntOffset = 0              ;
cfg_mpe->dma_d_config.rdma.Bias[0].   CntThresholdSha = 0        ;
cfg_mpe->dma_d_config.rdma.Bias[0].   CntCmp = 0                 ;

cfg_mpe->dma_d_config.rdma.Bias[1].   BiasEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[1].   ThreCtrl = 0               ;
cfg_mpe->dma_d_config.rdma.Bias[1].   DecCtrl = 0                ;
cfg_mpe->dma_d_config.rdma.Bias[1].   PXBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[1].   PYBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[1].   Bias = 0x0                 ;
cfg_mpe->dma_d_config.rdma.Bias[1].   AOffset = 0x0              ;
cfg_mpe->dma_d_config.rdma.Bias[1].   CntSha = 0                 ;
cfg_mpe->dma_d_config.rdma.Bias[1].   CntOffsetEn = Enable_NotEn        ;
cfg_mpe->dma_d_config.rdma.Bias[1].   CntOffset = 0              ;
cfg_mpe->dma_d_config.rdma.Bias[1].   CntThresholdSha = 0        ;
cfg_mpe->dma_d_config.rdma.Bias[1].   CntCmp = 0                 ;

cfg_mpe->dma_d_config.rdma.Bias[2].   BiasEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[2].   ThreCtrl = 0               ;
cfg_mpe->dma_d_config.rdma.Bias[2].   DecCtrl = 0                ;
cfg_mpe->dma_d_config.rdma.Bias[2].   PXBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[2].   PYBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[2].   Bias = 0x0                 ;
cfg_mpe->dma_d_config.rdma.Bias[2].   AOffset = 0x0              ;
cfg_mpe->dma_d_config.rdma.Bias[2].   CntSha = 0                 ;
cfg_mpe->dma_d_config.rdma.Bias[2].   CntOffsetEn = Enable_NotEn        ;
cfg_mpe->dma_d_config.rdma.Bias[2].   CntOffset = 0              ;
cfg_mpe->dma_d_config.rdma.Bias[2].   CntThresholdSha = 0        ;
cfg_mpe->dma_d_config.rdma.Bias[2].   CntCmp = 0                 ;

cfg_mpe->dma_d_config.rdma.Bias[3].   BiasEn = Enable_En                ;
cfg_mpe->dma_d_config.rdma.Bias[3].   ThreCtrl = 0               ;
cfg_mpe->dma_d_config.rdma.Bias[3].   DecCtrl = 0                ;
cfg_mpe->dma_d_config.rdma.Bias[3].   PXBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[3].   PYBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[3].   Bias = 0x80                ;
cfg_mpe->dma_d_config.rdma.Bias[3].   AOffset = 0x0              ;
cfg_mpe->dma_d_config.rdma.Bias[3].   CntSha = 31                ;
cfg_mpe->dma_d_config.rdma.Bias[3].   CntOffsetEn = Enable_NotEn        ;
cfg_mpe->dma_d_config.rdma.Bias[3].   CntOffset = 0              ;
cfg_mpe->dma_d_config.rdma.Bias[3].   CntThresholdSha = 0        ;
cfg_mpe->dma_d_config.rdma.Bias[3].   CntCmp = 0                 ;

cfg_mpe->dma_d_config.rdma.Bias[4].   BiasEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[4].   ThreCtrl = 0               ;
cfg_mpe->dma_d_config.rdma.Bias[4].   DecCtrl = 0                ;
cfg_mpe->dma_d_config.rdma.Bias[4].   PXBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[4].   PYBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[4].   Bias = 0x0                 ;
cfg_mpe->dma_d_config.rdma.Bias[4].   AOffset = 0x0              ;
cfg_mpe->dma_d_config.rdma.Bias[4].   CntSha = 0                 ;
cfg_mpe->dma_d_config.rdma.Bias[4].   CntOffsetEn = Enable_NotEn        ;
cfg_mpe->dma_d_config.rdma.Bias[4].   CntOffset = 0              ;
cfg_mpe->dma_d_config.rdma.Bias[4].   CntThresholdSha = 0        ;
cfg_mpe->dma_d_config.rdma.Bias[4].   CntCmp = 0                 ;

cfg_mpe->dma_d_config.rdma.Bias[5].   BiasEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[5].   ThreCtrl = 0               ;
cfg_mpe->dma_d_config.rdma.Bias[5].   DecCtrl = 0                ;
cfg_mpe->dma_d_config.rdma.Bias[5].   PXBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[5].   PYBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[5].   Bias = 0x0                 ;
cfg_mpe->dma_d_config.rdma.Bias[5].   AOffset = 0x0              ;
cfg_mpe->dma_d_config.rdma.Bias[5].   CntSha = 0                 ;
cfg_mpe->dma_d_config.rdma.Bias[5].   CntOffsetEn = Enable_NotEn        ;
cfg_mpe->dma_d_config.rdma.Bias[5].   CntOffset = 0              ;
cfg_mpe->dma_d_config.rdma.Bias[5].   CntThresholdSha = 0        ;
cfg_mpe->dma_d_config.rdma.Bias[5].   CntCmp = 0                 ;

cfg_mpe->dma_d_config.rdma.Bias[6].   BiasEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[6].   ThreCtrl = 0               ;
cfg_mpe->dma_d_config.rdma.Bias[6].   DecCtrl = 0                ;
cfg_mpe->dma_d_config.rdma.Bias[6].   PXBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[6].   PYBSEn = Enable_NotEn             ;
cfg_mpe->dma_d_config.rdma.Bias[6].   Bias = 0x0                 ;
cfg_mpe->dma_d_config.rdma.Bias[6].   AOffset = 0x0              ;
cfg_mpe->dma_d_config.rdma.Bias[6].   CntSha = 0                 ;
cfg_mpe->dma_d_config.rdma.Bias[6].   CntOffsetEn = Enable_NotEn        ;
cfg_mpe->dma_d_config.rdma.Bias[6].   CntOffset = 0              ;
cfg_mpe->dma_d_config.rdma.Bias[6].   CntThresholdSha = 0        ;
cfg_mpe->dma_d_config.rdma.Bias[6].   CntCmp = 0                 ;

cfg_mpe->dma_d_config.wdma.   BADR = 0                           ;
cfg_mpe->dma_d_config.wdma.   LADR = 256                         ;
cfg_mpe->dma_d_config.wdma.   USED = Enable_En                   ;
cfg_mpe->dma_d_config.wdma.   ADR_PROL = Enable_NotEn            ;
cfg_mpe->dma_d_config.wdma.   C_BND = Enable_En                  ;
cfg_mpe->dma_d_config.wdma.   GLUE_EN = Enable_En                ;
cfg_mpe->dma_d_config.wdma.   BStrideX = 1                       ;
cfg_mpe->dma_d_config.wdma.   BConvX = 1                         ;
cfg_mpe->dma_d_config.wdma.   Thre_PLC = 0                       ;
cfg_mpe->dma_d_config.wdma.   Thre_VLC = 0                       ;
cfg_mpe->dma_d_config.wdma.   Thre_CXC = 0                       ;
cfg_mpe->dma_d_config.wdma.   Dec_PLC = 0                        ;
cfg_mpe->dma_d_config.wdma.   PLC_CntSha = 0                     ;
cfg_mpe->dma_d_config.wdma.   VLC_CntSha = 0                     ;
cfg_mpe->dma_d_config.wdma.   PLC_ThreSha = 0                    ;
cfg_mpe->dma_d_config.wdma.   VLC_ThreSha = 0                    ;

cfg_mpe->dma_d_config.wdma.Bias[0].   CntSha = 0                  ;
cfg_mpe->dma_d_config.wdma.Bias[0].   CntCmp = 0                  ;

cfg_mpe->dma_d_config.wdma.Bias[1].   CntSha = 31                 ;
cfg_mpe->dma_d_config.wdma.Bias[1].   CntCmp = 0                  ;

cfg_mpe->dma_d_config.wdma.Bias[2].   CntSha = 0                  ;
cfg_mpe->dma_d_config.wdma.Bias[2].   CntCmp = 0                  ;

cfg_mpe->dma_d_config.wdma.Bias[3].   CntSha = 0                  ;
cfg_mpe->dma_d_config.wdma.Bias[3].   CntCmp = 0                  ;

cfg_mpe->dma_d_config.wdma.Bias[4].   CntSha = 0                  ;
cfg_mpe->dma_d_config.wdma.Bias[4].   CntCmp = 0                  ;

cfg_mpe->dma_d_config.wdma.Bias[5].   CntSha = 0                  ;
cfg_mpe->dma_d_config.wdma.Bias[5].   CntCmp = 0                  ;

cfg_mpe->dma_w_config.   MAINCNT = 0;

cfg_mpe->dma_w_config.rdma.   AOffset = 0x0                      ;
cfg_mpe->dma_w_config.rdma.   LPXOffset = 0                      ;
cfg_mpe->dma_w_config.rdma.   RPXOffset = 0                      ;
cfg_mpe->dma_w_config.rdma.   TPYOffset = 0                      ;
cfg_mpe->dma_w_config.rdma.   BPYOffset = 0                      ;
cfg_mpe->dma_w_config.rdma.   CntSha = 127                       ;
cfg_mpe->dma_w_config.rdma.   CntThresholdSha = 4095             ;
cfg_mpe->dma_w_config.rdma.   ThreCtrl = 0                       ;
cfg_mpe->dma_w_config.rdma.   LPXEn = Enable_NotEn               ;
cfg_mpe->dma_w_config.rdma.   RPXEn = Enable_NotEn               ;
cfg_mpe->dma_w_config.rdma.   TPYEn = Enable_NotEn               ;
cfg_mpe->dma_w_config.rdma.   BPYEn = Enable_NotEn               ;
cfg_mpe->dma_w_config.rdma.   LPXData = 0x0                      ;
cfg_mpe->dma_w_config.rdma.   RPXData = 0x0                      ;
cfg_mpe->dma_w_config.rdma.   TPYData = 0x0                      ;
cfg_mpe->dma_w_config.rdma.   BPYData = 0x0                      ;

cfg_mpe->dma_w_config.rdma.Bias[0].   BiasEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[0].   ThreCtrl = 0               ;
cfg_mpe->dma_w_config.rdma.Bias[0].   DecCtrl = 0                ;
cfg_mpe->dma_w_config.rdma.Bias[0].   PXBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[0].   PYBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[0].   Bias = 0x0                 ;
cfg_mpe->dma_w_config.rdma.Bias[0].   AOffset = 0x0              ;
cfg_mpe->dma_w_config.rdma.Bias[0].   CntSha = 0                 ;
cfg_mpe->dma_w_config.rdma.Bias[0].   CntOffsetEn = Enable_NotEn ;
cfg_mpe->dma_w_config.rdma.Bias[0].   CntOffset = 0              ;
cfg_mpe->dma_w_config.rdma.Bias[0].   CntThresholdSha = 0        ;
cfg_mpe->dma_w_config.rdma.Bias[0].   CntCmp = 0                 ;

cfg_mpe->dma_w_config.rdma.Bias[1].   BiasEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[1].   ThreCtrl = 0               ;
cfg_mpe->dma_w_config.rdma.Bias[1].   DecCtrl = 0                ;
cfg_mpe->dma_w_config.rdma.Bias[1].   PXBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[1].   PYBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[1].   Bias = 0x0                 ;
cfg_mpe->dma_w_config.rdma.Bias[1].   AOffset = 0x0              ;
cfg_mpe->dma_w_config.rdma.Bias[1].   CntSha = 0                 ;
cfg_mpe->dma_w_config.rdma.Bias[1].   CntOffsetEn = Enable_NotEn ;
cfg_mpe->dma_w_config.rdma.Bias[1].   CntOffset = 0              ;
cfg_mpe->dma_w_config.rdma.Bias[1].   CntThresholdSha = 0        ;
cfg_mpe->dma_w_config.rdma.Bias[1].   CntCmp = 0                 ;

cfg_mpe->dma_w_config.rdma.Bias[2].   BiasEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[2].   ThreCtrl = 0               ;
cfg_mpe->dma_w_config.rdma.Bias[2].   DecCtrl = 0                ;
cfg_mpe->dma_w_config.rdma.Bias[2].   PXBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[2].   PYBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[2].   Bias = 0x0                 ;
cfg_mpe->dma_w_config.rdma.Bias[2].   AOffset = 0x0              ;
cfg_mpe->dma_w_config.rdma.Bias[2].   CntSha = 0                 ;
cfg_mpe->dma_w_config.rdma.Bias[2].   CntOffsetEn = Enable_NotEn ;
cfg_mpe->dma_w_config.rdma.Bias[2].   CntOffset = 0              ;
cfg_mpe->dma_w_config.rdma.Bias[2].   CntThresholdSha = 0        ;
cfg_mpe->dma_w_config.rdma.Bias[2].   CntCmp = 0                 ;

cfg_mpe->dma_w_config.rdma.Bias[3].   BiasEn = Enable_En         ;
cfg_mpe->dma_w_config.rdma.Bias[3].   ThreCtrl = 0               ;
cfg_mpe->dma_w_config.rdma.Bias[3].   DecCtrl = 0                ;
cfg_mpe->dma_w_config.rdma.Bias[3].   PXBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[3].   PYBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[3].   Bias = 0x80                ;
cfg_mpe->dma_w_config.rdma.Bias[3].   AOffset = 0x0              ;
cfg_mpe->dma_w_config.rdma.Bias[3].   CntSha = 31                ;
cfg_mpe->dma_w_config.rdma.Bias[3].   CntOffsetEn = Enable_NotEn ;
cfg_mpe->dma_w_config.rdma.Bias[3].   CntOffset = 0              ;
cfg_mpe->dma_w_config.rdma.Bias[3].   CntThresholdSha = 0        ;
cfg_mpe->dma_w_config.rdma.Bias[3].   CntCmp = 0                 ;

cfg_mpe->dma_w_config.rdma.Bias[4].   BiasEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[4].   ThreCtrl = 0               ;
cfg_mpe->dma_w_config.rdma.Bias[4].   DecCtrl = 0                ;
cfg_mpe->dma_w_config.rdma.Bias[4].   PXBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[4].   PYBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[4].   Bias = 0x0                 ;
cfg_mpe->dma_w_config.rdma.Bias[4].   AOffset = 0x0              ;
cfg_mpe->dma_w_config.rdma.Bias[4].   CntSha = 0                 ;
cfg_mpe->dma_w_config.rdma.Bias[4].   CntOffsetEn = Enable_NotEn ;
cfg_mpe->dma_w_config.rdma.Bias[4].   CntOffset = 0              ;
cfg_mpe->dma_w_config.rdma.Bias[4].   CntThresholdSha = 0        ;
cfg_mpe->dma_w_config.rdma.Bias[4].   CntCmp = 0                 ;

cfg_mpe->dma_w_config.rdma.Bias[5].   BiasEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[5].   ThreCtrl = 0               ;
cfg_mpe->dma_w_config.rdma.Bias[5].   DecCtrl = 0                ;
cfg_mpe->dma_w_config.rdma.Bias[5].   PXBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[5].   PYBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[5].   Bias = 0x0                 ;
cfg_mpe->dma_w_config.rdma.Bias[5].   AOffset = 0x0              ;
cfg_mpe->dma_w_config.rdma.Bias[5].   CntSha = 0                 ;
cfg_mpe->dma_w_config.rdma.Bias[5].   CntOffsetEn = Enable_NotEn ;
cfg_mpe->dma_w_config.rdma.Bias[5].   CntOffset = 0              ;
cfg_mpe->dma_w_config.rdma.Bias[5].   CntThresholdSha = 0        ;
cfg_mpe->dma_w_config.rdma.Bias[5].   CntCmp = 0                 ;

cfg_mpe->dma_w_config.rdma.Bias[6].   BiasEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[6].   ThreCtrl = 0               ;
cfg_mpe->dma_w_config.rdma.Bias[6].   DecCtrl = 0                ;
cfg_mpe->dma_w_config.rdma.Bias[6].   PXBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[6].   PYBSEn = Enable_NotEn      ;
cfg_mpe->dma_w_config.rdma.Bias[6].   Bias = 0x0                 ;
cfg_mpe->dma_w_config.rdma.Bias[6].   AOffset = 0x0              ;
cfg_mpe->dma_w_config.rdma.Bias[6].   CntSha = 0                 ;
cfg_mpe->dma_w_config.rdma.Bias[6].   CntOffsetEn = Enable_NotEn ;
cfg_mpe->dma_w_config.rdma.Bias[6].   CntOffset = 0              ;
cfg_mpe->dma_w_config.rdma.Bias[6].   CntThresholdSha = 0        ;
cfg_mpe->dma_w_config.rdma.Bias[6].   CntCmp = 0                 ;

cfg_mpe->dma_w_config.wdma.   BADR = 256                         ;
cfg_mpe->dma_w_config.wdma.   LADR = 0                           ;
cfg_mpe->dma_w_config.wdma.   USED = Enable_En                   ;
cfg_mpe->dma_w_config.wdma.   ADR_PROL = Enable_NotEn            ;
cfg_mpe->dma_w_config.wdma.   C_BND = Enable_En                  ;
cfg_mpe->dma_w_config.wdma.   GLUE_EN = Enable_En                ;
cfg_mpe->dma_w_config.wdma.   BStrideX = 1                       ;
cfg_mpe->dma_w_config.wdma.   BConvX = 1                         ;
cfg_mpe->dma_w_config.wdma.   Thre_PLC = 0                       ;
cfg_mpe->dma_w_config.wdma.   Thre_VLC = 0                       ;
cfg_mpe->dma_w_config.wdma.   Thre_CXC = 0                       ;
cfg_mpe->dma_w_config.wdma.   Dec_PLC = 0                        ;
cfg_mpe->dma_w_config.wdma.   PLC_CntSha = 0                     ;
cfg_mpe->dma_w_config.wdma.   VLC_CntSha = 0                     ;
cfg_mpe->dma_w_config.wdma.   PLC_ThreSha = 0                    ;
cfg_mpe->dma_w_config.wdma.   VLC_ThreSha = 0                    ;

cfg_mpe->dma_w_config.wdma.Bias[0].   CntSha = 0                  ;
cfg_mpe->dma_w_config.wdma.Bias[0].   CntCmp = 0                  ;

cfg_mpe->dma_w_config.wdma.Bias[1].   CntSha = 31                 ;
cfg_mpe->dma_w_config.wdma.Bias[1].   CntCmp = 0                  ;

cfg_mpe->dma_w_config.wdma.Bias[2].   CntSha = 0                  ;
cfg_mpe->dma_w_config.wdma.Bias[2].   CntCmp = 0                  ;

cfg_mpe->dma_w_config.wdma.Bias[3].   CntSha = 0                  ;
cfg_mpe->dma_w_config.wdma.Bias[3].   CntCmp = 0                  ;

cfg_mpe->dma_w_config.wdma.Bias[4].   CntSha = 0                  ;
cfg_mpe->dma_w_config.wdma.Bias[4].   CntCmp = 0                  ;

cfg_mpe->dma_w_config.wdma.Bias[5].   CntSha = 0                  ;
cfg_mpe->dma_w_config.wdma.Bias[5].   CntCmp = 0                  ;


cfg_vpe->  src_flying = Enable_En                  ;
if(PPE_ENABLED==Enable_En)
  cfg_vpe->  dst_flying = Enable_En                    ;
else
  cfg_vpe->  dst_flying = Enable_NotEn;
cfg_vpe->  in_data_type = DataTypeExt_Int32;  // Use DataTypeExt!!!
cfg_vpe->  out_data_type = DataType_Int16                         ;
cfg_vpe->  mark = Enable_NotEn                                    ;
cfg_vpe->  cube_size = 0                                          ; // Calculated By nu_vpe_decide_dma_config_trivial
cfg_vpe->  op0_en = Enable_En                                     ;
cfg_vpe->  op1_en = Enable_NotEn                                  ;
cfg_vpe->  op2_en = Enable_NotEn                                  ;
cfg_vpe->  c3_offset = 0                                          ;
cfg_vpe->  c3_scale = 1                                           ;
cfg_vpe->  c3_trunc = 0                                           ;
cfg_vpe->  c3_satur_en = 1                                        ;
cfg_vpe->  c3_round_mode = RoundMode_HalfAwayFromZero             ;
cfg_vpe->  nan_to_zero_input = Enable_NotEn                       ;
cfg_vpe->  nan_to_zero_output = Enable_NotEn                      ;
cfg_vpe->  trace_mode = TraceMode_MPE                             ;

cfg_vpe->op0_config.  coef_type = DataType_Int16                             ;
cfg_vpe->op0_config.  alu_en = Enable_En                                     ;
cfg_vpe->op0_config.  mux_en = Enable_En                                     ;
cfg_vpe->op0_config.  relu_en = Enable_NotEn                                 ;
cfg_vpe->op0_config.  prelu_en = Enable_NotEn                                ;
cfg_vpe->op0_config.  alu_operation = AluOperationSwitch_Sum                                    ;
cfg_vpe->op0_config.  lshift_value = 0                                       ;
cfg_vpe->op0_config.  alu_mode = Mode_Channel;
cfg_vpe->op0_config.  alu_value = 26673153                                   ;
cfg_vpe->op0_config.  mux_mode = Mode_Channel                                     ;
cfg_vpe->op0_config.  mux_value = 26673154                                   ;
cfg_vpe->op0_config.  norm_round_mode = RoundMode_HalfAwayFromZero           ;
cfg_vpe->op0_config.  norm_saturation_en = Enable_En                         ;
cfg_vpe->op0_config.  norm_round_size = 0                                    ;

cfg_vpe->op1_config.  coef_type = DataType_Int16                             ;
cfg_vpe->op1_config.  alu_en = Enable_NotEn                                  ;
cfg_vpe->op1_config.  mux_en = Enable_NotEn                                  ;
cfg_vpe->op1_config.  relu_en = Enable_NotEn                                 ;
cfg_vpe->op1_config.  prelu_en = Enable_NotEn                                ;
cfg_vpe->op1_config.  alu_operation = AluOperationSwitch_Sum                                    ;
cfg_vpe->op1_config.  lshift_value = 0                                       ;
cfg_vpe->op1_config.  alu_mode = Mode_Unitary                                     ;
cfg_vpe->op1_config.  alu_value = 26673153                                   ;
cfg_vpe->op1_config.  mux_mode = Mode_Unitary                                     ;
cfg_vpe->op1_config.  mux_value = 2674065410                                 ;
cfg_vpe->op1_config.  norm_round_mode = RoundMode_HalfAwayFromZero           ;
cfg_vpe->op1_config.  norm_saturation_en = Enable_En                         ;
cfg_vpe->op1_config.  norm_round_size = 0                                    ;

cfg_vpe->op2_config.  coef_type = DataType_Int16                             ;
cfg_vpe->op2_config.  alu_en = Enable_NotEn                                  ;
cfg_vpe->op2_config.  mux_en = Enable_NotEn                                  ;
cfg_vpe->op2_config.  lut_en = Enable_NotEn                                  ;
cfg_vpe->op2_config.  prelu_en = Enable_NotEn                                ;
cfg_vpe->op2_config.  alu_operation = AluOperationExtSwitch_Sum                                    ;// Use Ext!
cfg_vpe->op2_config.  c1_en = Enable_NotEn                                   ;
cfg_vpe->op2_config.  c2_en = Enable_NotEn                                   ;
cfg_vpe->op2_config.  alu_mode = Mode_Unitary                                     ;
cfg_vpe->op2_config.  alu_value = 0                                          ;
cfg_vpe->op2_config.  mux_mode = Mode_Unitary                                     ;
cfg_vpe->op2_config.  mux_value = 0                                          ;
cfg_vpe->op2_config.  norm_round_mode = RoundMode_HalfAwayFromZero           ;
cfg_vpe->op2_config.  norm_saturation_en = Enable_En                         ;
cfg_vpe->op2_config.  norm_round_size = 0                                    ;
cfg_vpe->op2_config.  c1_offset = 0                                          ;
cfg_vpe->op2_config.  c1_scale = 1                                           ;
cfg_vpe->op2_config.  c1_trunc = 0                                           ;
cfg_vpe->op2_config.  c2_offset = 0                                          ;
cfg_vpe->op2_config.  c2_scale = 1                                           ;
cfg_vpe->op2_config.  c2_trunc = 0                                           ;
cfg_vpe->op2_config.  lut_sel = 0                                            ;
cfg_vpe->op2_config.  lut_right_priority = 0                                 ;
cfg_vpe->op2_config.  lut_left_priority = 0                                  ;
cfg_vpe->op2_config.  lut_tab1_x_start = 0                                   ;
cfg_vpe->op2_config.  lut_tab1_x_end = 0                                     ;
cfg_vpe->op2_config.  lut_tab2_x_start = 0                                   ;
cfg_vpe->op2_config.  lut_tab2_x_end = 0                                     ;
cfg_vpe->op2_config.  lut_tab1_slope_ovrf_scale = 0                          ;
cfg_vpe->op2_config.  lut_tab1_slope_undf_scale = 0                          ;
cfg_vpe->op2_config.  lut_tab2_slope_ovrf_scale = 0                          ;
cfg_vpe->op2_config.  lut_tab2_slope_undf_scale = 0                          ;
cfg_vpe->op2_config.  lut_tab1_slope_ovrf_shift = 0                          ;
cfg_vpe->op2_config.  lut_tab1_slope_undf_shift = 0                          ;
cfg_vpe->op2_config.  lut_tab2_slope_ovrf_shift = 0                          ;
cfg_vpe->op2_config.  lut_tab2_slope_undf_shift = 0                          ;
cfg_vpe->op2_config.  lut_mrr_en = Enable_NotEn                              ;
cfg_vpe->op2_config.  lut_mrr_type = 0                                       ;
cfg_vpe->op2_config.  lut_xoffset = 0                                        ;
cfg_vpe->op2_config.  lut_yoffset = 0                                        ;


cfg_ppe->   H      = 1              ;
cfg_ppe->   W      = 1              ;
cfg_ppe->   C      = 1              ;
cfg_ppe->   Kh     = 1              ;
cfg_ppe->   Kw     = 1              ;
cfg_ppe->   Sh     = 2              ;
cfg_ppe->   Sw     = 2              ;
cfg_ppe->   Kh_r   = 0x1000000      ;
cfg_ppe->   Kw_r   = 0x1000000      ;
cfg_ppe->   Tp     = 0              ;
cfg_ppe->   Bp     = 0              ;
cfg_ppe->   Lp     = 0              ;
cfg_ppe->   Rp     = 0              ;
cfg_ppe->   pv[0]  = 0x7fff         ;
cfg_ppe->   pv[1]  = 0x7fff         ;
cfg_ppe->   pv[2]  = 0x7fff         ;
cfg_ppe->   pv[3]  = 0x7fff         ;
cfg_ppe->   pv[4]  = 0x7fff         ;
cfg_ppe->   pv[5]  = 0x7fff         ;
cfg_ppe->   pv[6]  = 0x7fff         ;
cfg_ppe->   meth     = PoolingOperationSwitch_Min ;
cfg_ppe->   dt       = DataType_Int16;

}

void decide_dma_config(
  ConfigMPE* cfg_mpe, 
  ConfigVPE* cfg_vpe, 
  ConfigPPE* cfg_ppe, 
  ConfigREGPPE* cfg_reg_ppe, 
  void* in_data, 
  void* warr, 
  void*op0,
  void*op1,
  void*op2,
  void* res_data,
  CubeMetrics* res_metrics,
  CubeMetrics* mpe_out_metrics,
  Enable PPE_ENABLED
) {
  // Base Addresses For MPE
cfg_mpe->dma_d_config.rdma.   BFCA = nu_virt_to_dma(in_data); // nu_virt_to_dma Converts Addresses From Virtual In The OS On A Board
cfg_mpe->dma_w_config.rdma.   BFCA = nu_virt_to_dma(warr);    //   To A Physical That Programmed Into NU Regs
  
cfg_vpe->src_rdma_config.dma_bsize=0;   // Batch Parameters Are Default In This Program (Do Not Use Batch)
cfg_vpe->src_rdma_config.dma_bstride=0;
cfg_vpe->op0_rdma_config.dma_bsize=0;
cfg_vpe->op0_rdma_config.dma_bstride=0;
cfg_vpe->op1_rdma_config.dma_bsize=0;
cfg_vpe->op1_rdma_config.dma_bstride=0;
cfg_vpe->op2_rdma_config.dma_bsize=0;
cfg_vpe->op2_rdma_config.dma_bstride=0;
cfg_vpe->wdma_config.dma_bsize=0;
cfg_vpe->wdma_config.dma_bstride=0;

cfg_vpe->src_rdma_config.dma_baddr = (uint32_t)0xDEADBEEF;
cfg_vpe->op0_rdma_config.dma_baddr = nu_virt_to_dma(op0);
cfg_vpe->op1_rdma_config.dma_baddr = nu_virt_to_dma(op1);
cfg_vpe->op2_rdma_config.dma_baddr = nu_virt_to_dma(op2);
cfg_vpe->wdma_config.dma_baddr     = nu_virt_to_dma(res_data);

    // AxLEN Choosen For Most Performance
cfg_vpe->src_rdma_config.dma_axi_len = 15;
cfg_vpe->op0_rdma_config.dma_axi_len = 15;
cfg_vpe->op1_rdma_config.dma_axi_len = 15;
cfg_vpe->op2_rdma_config.dma_axi_len = 15;
cfg_vpe->wdma_config.dma_axi_len     = 15;

  // Lib Function That Calculates DMA Regs
nu_vpe_decide_dma_config_trivial(cfg_vpe,mpe_out_metrics);

cfg_reg_ppe->rBALi = (uintptr_t)0xDEADBEEF; // Data Base Addresses // Not Used
cfg_reg_ppe->wBALo = nu_virt_to_dma(res_data);

cfg_reg_ppe->wOpM = 3 << 8; // FLYING_BOXED 
                       // Other Fields Of wOpM Will Be Appended By nu_ppe_decide_dma_config_trivial

  // Lib Function That Calculates cfg_reg_ppe Structure Fields
nu_ppe_decide_dma_config_trivial(cfg_ppe, res_metrics, cfg_reg_ppe);
cfg_reg_ppe->wOpEn  = 0x0; // This Is Needed Because Some Other Setup Functions Write Into DMA Start Reg
}


void* in_data;
void* warr;
void* op0;
void* op1;
void* op2;
void* res_data;
void* etalon;

int main() {
  int heap_id;
  CubeMetrics in_metrics;
  WarrMetrics warr_metrics;
  CubeMetrics res_metrics;
  CubeMetrics mpe_out_metrics;
  
  rumboot_printf("Hello Simple Direct Test\n");
  
#ifdef DONT_USE_PPE
  PPE_ENABLED=0;
#else
  PPE_ENABLED=1;
#endif
  
    // Set Direct Mode For All Units
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_MPE_UNIT_MODE|NA_CU_VPE_UNIT_MODE|NA_CU_PPE_UNIT_MODE);
  
    // Get The Heap Identifier Where To Place All The Test Data
  heap_id = nu_get_heap_id();
  
    // Get The Base Cfg Parameters
  fill_cfgs(&cfg_mpe,&cfg_vpe,&cfg_ppe,&cfg_reg_ppe,PPE_ENABLED);
  
    // Calc Test Data Metrics
  in_metrics.H=cfg_mpe.H;
  in_metrics.W=cfg_mpe.W;
  in_metrics.C=cfg_mpe.C;
  in_metrics.s=cfg_mpe.H * cfg_mpe.W * cfg_mpe.C * (cfg_mpe.dt==DataType_Int8? 1 : 2);
  
  warr_metrics.H=cfg_mpe.R;
  warr_metrics.W=cfg_mpe.S;
  warr_metrics.C=cfg_mpe.C;
  warr_metrics.K=cfg_mpe.K;
  warr_metrics.s= warr_metrics.H * warr_metrics.W * warr_metrics.C * warr_metrics.K * (cfg_mpe.dt==DataType_Int8? 1 : 2);
  
  // mpe_out_metrics - Metrics Of VPE Input Data (Not Seen In Memory) Needed For Configuring VPE Later
  nu_calc_mpe_cube_out_metrics(&cfg_mpe, &mpe_out_metrics); 

  if(PPE_ENABLED==Enable_En) {
    res_metrics.H = cfg_ppe.H;
    res_metrics.W = cfg_ppe.W;
    res_metrics.C = cfg_ppe.C;
    res_metrics.s = cfg_ppe.H * cfg_ppe.W * cfg_ppe.C * (cfg_ppe.dt==DataType_Int8 ? 1 : 2);
  }
  else {
    res_metrics.H = mpe_out_metrics.H;
    res_metrics.W = mpe_out_metrics.W;
    res_metrics.C = mpe_out_metrics.C;
    res_metrics.s = mpe_out_metrics.H * mpe_out_metrics.W * mpe_out_metrics.C * (cfg_vpe.out_data_type==DataType_Int8 ? 1 : 2);
  }

    // Malloc The Test Data
  in_data = rumboot_malloc_from_heap_aligned(heap_id,in_metrics.s,16);
  warr    = rumboot_malloc_from_heap_aligned(heap_id,warr_metrics.s,16);
  res_data= rumboot_malloc_from_heap_aligned(heap_id,res_metrics.s,16);
  memset(res_data,0xA5,res_metrics.s); // Clear The Place Under Result Data
  etalon  = rumboot_malloc_from_heap_aligned(heap_id,res_metrics.s,16);
  
    // Read The Test Data Files
  rumboot_platform_request_file_ex("in_file_tag",(uintptr_t)in_data,in_metrics.s);
  rumboot_platform_request_file_ex("warr_file_tag",(uintptr_t)warr,warr_metrics.s);
  rumboot_platform_request_file_ex("etalon_file_tag",(uintptr_t)etalon,res_metrics.s);
  
    // Prepare The OP0 - OP2 Operands
  op0=NULL;op1=NULL;op2=NULL;
  
  if(cfg_vpe.op0_en==Enable_En) {
      // If OP0 Is A Cube
    if((cfg_vpe.op0_config.alu_en==Enable_En && cfg_vpe.op0_config.alu_mode==Mode_Element) || 
       (cfg_vpe.op0_config.mux_en==Enable_En && cfg_vpe.op0_config.mux_mode==Mode_Element) ) {
         // Malloc The Place And Load From File
      int size;
      size = mpe_out_metrics.H * mpe_out_metrics.W * mpe_out_metrics.C * 
            (cfg_vpe.op0_config.coef_type==DataType_Int8?1:2);
      if((cfg_vpe.op0_config.alu_en==Enable_En && cfg_vpe.op0_config.alu_mode==Mode_Element) && 
         (cfg_vpe.op0_config.mux_en==Enable_En && cfg_vpe.op0_config.mux_mode==Mode_Element) ) {
           size = size * 2;
      }
      op0 = rumboot_malloc_from_heap_aligned(heap_id,size,16);
      rumboot_platform_request_file_ex("op0_cube_file_tag",(uintptr_t)op0,size);
    }
    
      // If OP0 Is A Vector
    if((cfg_vpe.op0_config.alu_en==Enable_En && cfg_vpe.op0_config.alu_mode==Mode_Channel) || 
       (cfg_vpe.op0_config.mux_en==Enable_En && cfg_vpe.op0_config.mux_mode==Mode_Channel) ) {
      int size;  // Calc The Size Of The Vector
      size = mpe_out_metrics.C * (cfg_vpe.op0_config.coef_type==DataType_Int8 ? 1 : 2);
      if((cfg_vpe.op0_config.alu_en==Enable_En && cfg_vpe.op0_config.alu_mode==Mode_Channel ) && 
         (cfg_vpe.op0_config.mux_en==Enable_En && cfg_vpe.op0_config.mux_mode==Mode_Channel ) ) {
           size = size * 2;
      }
                 // Malloc The Place And Load From File
      op0 = rumboot_malloc_from_heap_aligned(heap_id,size,16);
      rumboot_platform_request_file_ex("op0_vec_file_tag",(uintptr_t)op0,size);
    }
  }
  
  if(cfg_vpe.op1_en==Enable_En) {
    if((cfg_vpe.op1_config.alu_en==Enable_En && cfg_vpe.op1_config.alu_mode==Mode_Element) || 
       (cfg_vpe.op1_config.mux_en==Enable_En && cfg_vpe.op1_config.mux_mode==Mode_Element) ) {
      int size;
      size = mpe_out_metrics.H * mpe_out_metrics.W * mpe_out_metrics.C * 
            (cfg_vpe.op1_config.coef_type==DataType_Int8?1:2);
      if((cfg_vpe.op1_config.alu_en==Enable_En && cfg_vpe.op1_config.alu_mode==Mode_Channel ) && 
         (cfg_vpe.op1_config.mux_en==Enable_En && cfg_vpe.op1_config.mux_mode==Mode_Channel ) ) {
           size = size * 2;
      }
      op1 = rumboot_malloc_from_heap_aligned(heap_id,size,16);
      rumboot_platform_request_file_ex("op1_cube_file_tag",(uintptr_t)op1,size);
    }
    if((cfg_vpe.op1_config.alu_en==Enable_En && cfg_vpe.op1_config.alu_mode==Mode_Channel) || 
       (cfg_vpe.op1_config.mux_en==Enable_En && cfg_vpe.op1_config.mux_mode==Mode_Channel) ) {
      int size;
      size = mpe_out_metrics.C * (cfg_vpe.op1_config.coef_type==DataType_Int8 ? 1 : 2);
      if((cfg_vpe.op1_config.alu_en==Enable_En && cfg_vpe.op1_config.alu_mode==Mode_Channel ) && 
         (cfg_vpe.op1_config.mux_en==Enable_En && cfg_vpe.op1_config.mux_mode==Mode_Channel ) ) {
           size = size * 2;
      }
      op1 = rumboot_malloc_from_heap_aligned(heap_id,size,16);
      rumboot_platform_request_file_ex("op1_vec_file_tag",(uintptr_t)op1,size);
    }
  }
  
  if(cfg_vpe.op2_en==Enable_En) {
    if((cfg_vpe.op2_config.alu_en==Enable_En && cfg_vpe.op2_config.alu_mode==Mode_Element) || 
       (cfg_vpe.op2_config.mux_en==Enable_En && cfg_vpe.op2_config.mux_mode==Mode_Element) ) {
      int size;
      size = mpe_out_metrics.H * mpe_out_metrics.W * mpe_out_metrics.C * 
            (cfg_vpe.op2_config.coef_type==DataType_Int8?1:2);
      if((cfg_vpe.op2_config.alu_en==Enable_En && cfg_vpe.op2_config.alu_mode==Mode_Channel ) && 
         (cfg_vpe.op2_config.mux_en==Enable_En && cfg_vpe.op2_config.mux_mode==Mode_Channel ) ) {
           size = size * 2;
      }
      rumboot_platform_request_file_ex("op2_cube_file_tag",(uintptr_t)op2,size);
      op2 = rumboot_malloc_from_heap_aligned(heap_id,size,16);
    }
    if((cfg_vpe.op2_config.alu_en==Enable_En && cfg_vpe.op2_config.alu_mode==Mode_Channel) || 
       (cfg_vpe.op2_config.mux_en==Enable_En && cfg_vpe.op2_config.mux_mode==Mode_Channel) ) {
      int size;
      size = mpe_out_metrics.C * (cfg_vpe.op2_config.coef_type==DataType_Int8 ? 1 : 2);
      if((cfg_vpe.op2_config.alu_en==Enable_En && cfg_vpe.op2_config.alu_mode==Mode_Channel ) && 
         (cfg_vpe.op2_config.mux_en==Enable_En && cfg_vpe.op2_config.mux_mode==Mode_Channel ) ) {
           size = size * 2;
      }
      op2 = rumboot_malloc_from_heap_aligned(heap_id,size,16);
      rumboot_platform_request_file_ex("op2_vec_file_tag",(uintptr_t)op2,size);
    }
  }
  
    // Calculate The Secondary Cfg Parameters
  decide_dma_config(
    & cfg_mpe, 
    & cfg_vpe, 
    & cfg_ppe, 
    & cfg_reg_ppe, 
    in_data, 
    warr, 
    op0,
    op1,
    op2,
    res_data,
    & res_metrics,
    & mpe_out_metrics,
    PPE_ENABLED
  );
  
    // Print All The Configurations Got
  nu_mpe_print_config(&cfg_mpe);
  nu_vpe_print_config(&cfg_vpe);
  if(PPE_ENABLED==Enable_En) {
    nu_ppe_print_config(&cfg_ppe);
    nu_ppe_print_config_reg(&cfg_reg_ppe);
  }
  
    // Program The VPE, MPE And PPE (Single Run)
  nu_vpe_setup(MY_VPE_REGS_BASE, &cfg_vpe);
  nu_mpe_setup(MY_MPE_REGS_BASE, &cfg_mpe);
  if(PPE_ENABLED==Enable_En) {
    nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, &cfg_reg_ppe);
  
    cfg_reg_ppe.wOpEn  = 0x1;  // This Is Needed Because nu_ppe_wdma_run Just Writes The Content Of wOpEn
    nu_ppe_wdma_run(MY_PPE_REGS_BASE, &cfg_reg_ppe);
  }
  nu_vpe_run(MY_VPE_REGS_BASE, &cfg_vpe);
  nu_mpe_run(MY_MPE_REGS_BASE, &cfg_mpe);
  
    // Wait For The Corresponding DMA Channels To Complete
  if(PPE_ENABLED==Enable_En)
    nu_ppe_wait_complete(MY_PPE_REGS_BASE);
  else
    nu_vpe_wait(MY_VPE_REGS_BASE, &cfg_vpe);
  
  rumboot_printf("Dumping 0x%x, %d bytes\n", (uint32_t) res_data,res_metrics.s);
  rumboot_platform_dump_region("res.bin", (uintptr_t) res_data, res_metrics.s);
  
  rumboot_printf("Comparing..\n");
  
    // Result vs Etalon Comparision
  if(nu_bitwise_compare(res_data, etalon, res_metrics.s) == 0)
    rumboot_printf("Test PASSED\n");
  else {
    rumboot_printf("Test FAILED\n");
    return 1;
  }
  
  return 0;
}