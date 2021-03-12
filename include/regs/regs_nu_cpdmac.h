
#define NU_CPDMAC_REGBLOCK_OFFSET(index) (index << 12)
#define NU_CPDMAC_REGBLOCK_SIZE NU_CPDMAC_REGBLOCK_OFFSET(1)

#define NU_CPDMAC_MainCounter_tr           0x00
#define NU_CPDMAC_Address_tr               0x04
#define NU_CPDMAC_Bias_tr                          0x08
#define NU_CPDMAC_RowCounter_tr            0x0C
#define NU_CPDMAC_AddressMode_tr           0x10
#define NU_CPDMAC_CSR_tr                           0x14
#define NU_CPDMAC_InterruptMask_tr         0x18
#define NU_CPDMAC_InternalState_tr         0x1C

#define NU_CPDMAC_MainCounter_rcv          0x20
#define NU_CPDMAC_Address_rcv              0x24
#define NU_CPDMAC_Bias_rcv                         0x28
#define NU_CPDMAC_RowCounter_rcv           0x2C
#define NU_CPDMAC_AddressMode_rcv          0x30
#define NU_CPDMAC_CSR_rcv                          0x34
#define NU_CPDMAC_InterruptMask_rcv        0x38
#define NU_CPDMAC_InternalState_rcv        0x3C
