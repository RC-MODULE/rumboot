#define CAN_MODE_REG          0x0000
#define IMCR_REG              0x0004
#define INTRP_EN_REG          0x0008
#define STATUS_REG            0x004C
#define INTRP_FLAG_REG        0x0050
#define TR_TIMING_REG         0x0014
#define DIVE_REG              0x0018
#define SOFT_RST_REG          0x001C
#define ST_ERR_CNT_REG        0x0070
#define OVL_TEC_CNT_REG       0x0024
#define OVL_REC_CNT_REG       0x0028
#define WRN_LVL_CNT_REG       0x002C
#define ERR_PSV_CNT_REG       0x0030
#define BOFF_CNT_REG          0x0034
#define SUCCESS_TR_CNT_REG    0x0038
#define TIME_OUT_FR_CNT_REG   0x003C
#define SISH_TC_ERR_CNT_REG   0x0040
#define SUCCESS_REC_CNT_REG   0x0044
#define BUS_STAT_EN_REG       0x0010
#define TEST_REG              0x000C
#define TX_TEST_REG           0x0054
#define OVFL_EQ_RM_CNT_REG    0x0048
#define OVFL_EQ_TR_CNT_REG    0x0058
#define OVFL_EQ_BS_CNT_REG    0x005C
#define GLB_TR_CANCEL_REG     0x0060
#define GLB_TR_CANCEL_CNT_REG 0x0064
#define TQ_FORM_EN_REG        0x0020
#define TQ_FORM_ERR_CNT_REG   0x0074
#define IM_TC_ERR_CNT_REG     0x0078
#define BOFF_TC_ERR_CNT_REG   0x007C
#define SIZE_TQ0              0x0080
#define CTRL_TQ0              0x0084
#define EFC_TQ0               0x0088
#define WM_TQ0                0x008C
#define SIZE_TQ1              0x0090
#define CTRL_TQ1              0x0094
#define EFC_TQ1               0x0098
#define WM_TQ1                0x009C
#define SIZE_TQ2              0x00A0
#define CTRL_TQ2              0x00A4
#define EFC_TQ2               0x00A8
#define WM_TQ2                0x00AC
#define SIZE_TQ3              0x00B0
#define CTRL_TQ3              0x00B4
#define EFC_TQ3               0x00B8
#define WM_TQ3                0x00BC
#define SIZE_EQ0              0x0100
#define FC_EQ0                0x0104
#define WM_EQ0                0x0108
#define FILTER_F0             0x0200
#define MASK_F0               0x0204
#define FILTER_F1             0x0208
#define MASK_F1               0x020C
#define FILTER_F2             0x0210
#define MASK_F2               0x0214
#define FILTER_F3             0x0218
#define MASK_F3               0x021C
#define FILTER_F4             0x0220
#define MASK_F4               0x0224
#define FILTER_F5             0x0228
#define MASK_F5               0x022C
#define FILTER_F6             0x0230
#define MASK_F6               0x0234
#define FILTER_F7             0x0238
#define MASK_F7               0x023C
#define FILTER_F8             0x0240
#define MASK_F8               0x0244
#define FILTER_F9             0x0248
#define MASK_F9               0x024C
#define FILTER_F10            0x0250
#define MASK_F10              0x0254
#define FILTER_F11            0x0258
#define MASK_F11              0x025C
#define FILTER_F12            0x0260
#define MASK_F12              0x0264
#define FILTER_F13            0x0268
#define MASK_F13              0x026C
#define FILTER_F14            0x0270
#define MASK_F14              0x0274
#define FILTER_F15            0x0278
#define MASK_F15              0x027C
#define DATA_TQ0              0x1000
#define DATA_TQ1              0x2000
#define DATA_TQ2              0x3000
#define DATA_TQ3              0x4000
#define DATA_EQ0              0x5000

//EQ elements
#define TR_REPORT_ID          0x5AD912C3

#define BUS_STATUS_ID         0x7E39B6E8
#define BUS_IDLE              0x9

#define REC_MSG_ID            0x92DC79A5

//CAN Mode register
#define IM                    0x0
#define TRCE                  0x6
#define TME                   0x7
 
 // Init Mode change reg
#define IMCR                  0x0 
 //Interrupt enable register
#define BEIE                  0x0
#define TIE                   0x3
#define RIE                   0x4
#define EPIE                  0x5
#define WLIE                  0x6
#define BOIE                  0x7
#define BIIE                  0x8
#define GBIE                  0x9
 
 //Status register
#define BEC                   0x0
#define TXOK                  0x3
#define RXOK                  0x4
#define EPS                   0x5
#define EWL                   0x6
#define BOS                   0x7
#define BI                    0x8
 
 //TR timing register
#define DIV                   0x0
#define SJW                   0x6
#define TS1                   0x8
#define TS2                   0xC

 //DIV Extension
#define DIVE                  0x0 
 
 //Test register and TX request
 
#define SNT                   0x3
#define LBK                   0x4 
#define TX                    0x5
#define TX_RQST               0x7
 
 //Bus status enable
#define LARB                  0x0
#define TER                   0x1
#define RER                   0x2
#define EWRN                  0x3
#define EAEP                  0x4
#define EPEA                  0x5
#define BO                    0x6
#define BOR                   0x7
//#define BI                    0x8 // comment due to BI repeat like for Status Reg
 
 // TRSL
#define TIMEOUT               0x2

 //Increment
#define INC                   0x0
 
 //TQ format enable
#define EID                   0x0
#define DLCC                  0x1
 
 //Size TQx
#define SIZE_TQX              0x0
 
 //Control TQx
#define TC                    0x0
#define QC                    0x1
 
 //Global Transmit Cancel
#define GTC                   0x0
 
 //Size RQx
#define SIZE_RQX              0x0
 
 //Filter Fx
#define FILTER_FX             0x0
 
 //Mask Fx
#define MASK_FX               0x0