//Offset regs
#define SSPCR0                  0X00
#define SSPCR1                  0X04
#define SSPDR                   0X08
#define SSPSR                   0X0C
#define SSPCPSR                 0X10
#define SSPIMSC                 0X14
#define SSPRIS                  0X18
#define SSPMIS                  0X1C
#define SSPICR                  0X20
#define SSPDMACR                0X24
#define SSPPeriphID0            0XFE0
#define SSPPeriphID1            0XFE4
#define SSPPeriphID2            0XFE8
#define SSPPeriphID3            0XFEC
#define SSPPCellID0             0XFF0
#define SSPPCellID1             0XFF4
#define SSPPCellID2             0XFF8
#define SSPPCellID3             0XFFC

//Default values
#define DEFAULT_SSPCR0          0x0000#define DEFAULT_SSPCR1          0x0#define DEFAULT_SSPSR           0x03#define DEFAULT_SSPCPSR         0x00#define DEFAULT_SSPIMSC         0x0#define DEFAULT_SSPRIS          0x8#define DEFAULT_SSPMIS          0x0#define DEFAULT_SSPICR          0x0#define DEFAULT_SSPDMACR        0x0#define DEFAULT_SSPPeriphID0    0x22#define DEFAULT_SSPPeriphID1    0x10#define DEFAULT_SSPPeriphID2    0x34#define DEFAULT_SSPPeriphID3    0x00#define DEFAULT_SSPPCellID0     0x0D#define DEFAULT_SSPPCellID1     0xF0#define DEFAULT_SSPPCellID2     0x05#define DEFAULT_SSPPCellID3     0xB1

//Mask
#define MASK_SSPCR0             0XFFFF
#define MASK_SSPCR1             0XF
#define MASK_SSPDR              0XFFFF
#define MASK_SSPSR              0X1F
#define MASK_SSPCPSR            0XFF
#define MASK_SSPIMSC            0XF
#define MASK_SSPRIS             0XF
#define MASK_SSPMIS             0XF
#define MASK_SSPICR             0XF
#define MASK_SSPDMACR           0X3

#define GSPI_TIMEOUT            200
