//Offset regs
#define GSPI_SSPCR0                  0x00
#define GSPI_SSPCR1                  0x04
#define GSPI_SSPDR                   0x08
#define GSPI_SSPSR                   0x0C
#define GSPI_SSPCPSR                 0x10
#define GSPI_SSPIMSC                 0x14
#define GSPI_SSPRIS                  0x18
#define GSPI_SSPMIS                  0x1C
#define GSPI_SSPICR                  0x20
#define GSPI_SSPDMACR                0x24
#define GSPI_SSPPeriphID0            0xFE0
#define GSPI_SSPPeriphID1            0xFE4
#define GSPI_SSPPeriphID2            0xFE8
#define GSPI_SSPPeriphID3            0xFEC
#define GSPI_SSPPCellID0             0xFF0
#define GSPI_SSPPCellID1             0xFF4
#define GSPI_SSPPCellID2             0xFF8
#define GSPI_SSPPCellID3             0xFFC

#define GSPI_DMAWSTART               0x90
#define GSPI_DMAWEND                 0x94
#define GSPI_DMAWCNTRL               0x98
#define GSPI_DMAWCUR                 0x9C
#define GSPI_AXIW_BRESP              0xA0
#define GSPI_DMARSTART               0xA4
#define GSPI_DMAREND                 0xA8
#define GSPI_DMARCNTRL               0xAC
#define GSPI_DMARCUR                 0xB0
#define GSPI_AXIR_BUFTYPE            0xB4
#define GSPI_AXIR_BUFENA             0xB8
#define GSPI_AXIR_ERRADDR            0xC0
#define GSPI_AXI_PARAMS              0xC4
#define GSPI_WORDOP                  0xC8
#define GSPI_SOFTSS                  0xCC
#define GSPI_SOFTRST                 0xD0
#define GSPI_STATUS                  0xD4
#define GSPI_IRQMASKS                0xD8


//Default values
#define GSPI_SSPCR0_DEFAULT          0x0000#define GSPI_SSPCR1_DEFAULT          0x0#define GSPI_SSPSR_DEFAULT           0x03#define GSPI_SSPCPSR_DEFAULT         0x00#define GSPI_SSPIMSC_DEFAULT         0x0#define GSPI_SSPRIS_DEFAULT          0x8#define GSPI_SSPMIS_DEFAULT          0x0#define GSPI_SSPICR_DEFAULT          0x0#define GSPI_SSPDMACR_DEFAULT        0x0#define GSPI_SSPPeriphID0_DEFAULT    0x22#define GSPI_SSPPeriphID1_DEFAULT    0x10#define GSPI_SSPPeriphID2_DEFAULT    0x34#define GSPI_SSPPeriphID3_DEFAULT    0x00#define GSPI_SSPPCellID0_DEFAULT     0x0D#define GSPI_SSPPCellID1_DEFAULT     0xF0#define GSPI_SSPPCellID2_DEFAULT     0x05#define GSPI_SSPPCellID3_DEFAULT     0xB1

#define GSPI_DMAWSTART_DEFAULT       0x00
#define GSPI_DMAWEND_DEFAULT         0x00
#define GSPI_DMAWCNTRL_DEFAULT       0x00
#define GSPI_DMAWCUR_DEFAULT         0x00
#define GSPI_AXIW_BRESP_DEFAULT      0x00
#define GSPI_DMARSTART_DEFAULT       0x00
#define GSPI_DMAREND_DEFAULT         0x00
#define GSPI_DMARCNTRL_DEFAULT       0x00
#define GSPI_DMARCUR_DEFAULT         0x00
#define GSPI_AXIR_BUFTYPE_DEFAULT    0x00
#define GSPI_AXIR_BUFENA_DEFAULT     0x00
#define GSPI_AXIR_ERRADDR_DEFAULT    0x00
#define GSPI_AXI_PARAMS_DEFAULT      0x00
#define GSPI_WORDOP_DEFAULT          0x00
#define GSPI_SOFTSS_DEFAULT          0x00
#define GSPI_SOFTRST_DEFAULT         0x00
#define GSPI_STATUS_DEFAULT          0x80
#define GSPI_IRQMASKS_DEFAULT        0x00


//Mask
#define GSPI_SSPCR0_MASK             0xFFFF
#define GSPI_SSPCR1_MASK             0xF
#define GSPI_SSPDR_MASK              0xFFFF
#define GSPI_SSPSR_MASK              0x1F
#define GSPI_SSPCPSR_MASK            0xFF
#define GSPI_SSPIMSC_MASK            0xF
#define GSPI_SSPRIS_MASK             0xF
#define GSPI_SSPMIS_MASK             0xF
#define GSPI_SSPICR_MASK             0xF
#define GSPI_SSPDMACR_MASK           0x3
#define GSPI_SSPPeriphID0_MASK       0xFF
#define GSPI_SSPPeriphID1_MASK       0xFF
#define GSPI_SSPPeriphID2_MASK       0xFF
#define GSPI_SSPPeriphID3_MASK       0xFF
#define GSPI_SSPPCellID0_MASK        0xFF
#define GSPI_SSPPCellID1_MASK        0xFF
#define GSPI_SSPPCellID2_MASK        0xFF
#define GSPI_SSPPCellID3_MASK        0xFF

#define GSPI_DMAWSTART_MASK          0xFFFFFFFF
#define GSPI_DMAWEND_MASK            0xFFFFFFFF
#define GSPI_DMAWCNTRL_MASK          0xDFFFFFFF
#define GSPI_DMAWCUR_MASK            0xFFFFFFFF
#define GSPI_AXIW_BRESP_MASK         0x00000003
#define GSPI_DMARSTART_MASK          0xFFFFFFFF
#define GSPI_DMAREND_MASK            0xFFFFFFFF
#define GSPI_DMARCNTRL_MASK          0xDFFFFFFF
#define GSPI_DMARCUR_MASK            0xFFFFFFFF
#define GSPI_AXIR_BUFTYPE_MASK       0x00000001
#define GSPI_AXIR_BUFENA_MASK        0x00000001
#define GSPI_AXIR_ERRADDR_MASK       0xFFFFFFFF
#define GSPI_AXI_PARAMS_MASK         0x000003FF
#define GSPI_WORDOP_MASK             0x00000001
#define GSPI_SOFTSS_MASK             0x00000003
#define GSPI_SOFTRST_MASK            0x00000001
#define GSPI_STATUS_MASK             0x000000FF
#define GSPI_IRQMASKS_MASK           0x0000007F

//bitmask
//CR0
#define SPH_SHIFT 7
#define SPO_SHIFT 6
#define DSS_SHIFT 0
#define FRF_SHIFT 4
#define SCR_SHIFT 8
//CR1
#define SSE (1 << 1)
#define LBM_SHIFT   0
#define MS_SHIFT    2
//SSPSR
#define RFF (1 << 3)
#define RNE (1 << 2)
#define TNF (1 << 1)
#define TFE (1 << 0)
//SSPDMACR
#define TXDMAE  (1 << 1)
#define RXDMAE  (1 << 0)

#define GSPI_TIMEOUT                 200
