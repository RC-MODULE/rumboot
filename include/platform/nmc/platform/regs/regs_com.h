
#define MainCounter_tr           0x00
#define Address_tr           	 0x04
#define Bias_tr           	 	 0x08
#define RowCounter_tr            0x0C
#define AddressMode_tr           0x10
#define CSR_tr           	 	 0x14
#define InterruptMask_tr         0x18
#define InternalState_tr         0x1C

#define MainCounter_rcv          0x20
#define Address_rcv           	 0x24
#define Bias_rcv           	 	 0x28
#define RowCounter_rcv           0x2C
#define AddressMode_rcv          0x30
#define CSR_rcv           	 	 0x34
#define InterruptMask_rcv        0x38
#define InternalState_rcv        0x3C

#define HC_EN       			 0x40
#define PHYConfig       		 0x44
#define Err1CNT       		     0x68
#define Err2CNT       		     0x6C

//Mask
#define COM_MAINCOUNTER_TR_MASK             0x0000FFFF
#define COM_ADDRESS_TR_MASK           	    0xFFFFFFF8
#define COM_BIAS_TR_MASK           	 	    0xFFFFFFF8
#define COM_ROWCOUNTER_TR_MASK              0x0000FFFF
#define COM_ADDRESSMODE_TR_MASK             0x00000001
#define COM_CSR_TR_MASK           	 	    0x0000000F
#define COM_INTERRUPTMASK_TR_MASK           0x00000003
#define COM_INTERNALSTATE_TR_MASK           0x0F1C001F

#define COM_MAINCOUNTER_RCV_MASK            0x0000FFFF
#define COM_ADDRESS_RCV_MASK           	    0xFFFFFFFF
#define COM_BIAS_RCV_MASK           	    0xFFFFFFFF
#define COM_ROWCOUNTER_RCV_MASK             0x0000FFFF
#define COM_ADDRESSMODE_RCV_MASK            0x00000001
#define COM_CSR_RCV_MASK           	 	    0x0000000F
#define COM_INTERRUPTMASK_RCV_MASK          0x00000003
#define COM_INTERNALSTATE_RCV_MASK          0x0F00001F

#define COM_HC_EN_MASK       		        0x00000001
#define COM_PHYCONFIG_MASK       	        0x0000001F
#define COM_ERR1CNT_MASK       		        0x8000FFFF
#define COM_ERR2CNT_MASK       		        0x8000FFFF

//Deafault values
#define COM_MAINCOUNTER_TR_DEFAULT          0x100
#define COM_ADDRESS_TR_DEFAULT           	0x0
#define COM_BIAS_TR_DEFAULT           	 	0x0
#define COM_ROWCOUNTER_TR_DEFAULT           0x0
#define COM_ADDRESSMODE_TR_DEFAULT          0x0
#define COM_CSR_TR_DEFAULT           	    0x0
#define COM_INTERRUPTMASK_TR_DEFAULT        0x0
#define COM_INTERNALSTATE_TR_DEFAULT        0x0

#define COM_MAINCOUNTER_RCV_DEFAULT         0x100
#define COM_ADDRESS_RCV_DEFAULT             0x0
#define COM_BIAS_RCV_DEFAULT           	    0x0
#define COM_ROWCOUNTER_RCV_DEFAULT          0x0
#define COM_ADDRESSMODE_RCV_DEFAULT         0x0
#define COM_CSR_RCV_DEFAULT           	 	0x0
#define COM_INTERRUPTMASK_RCV_DEFAULT       0x0
#define COM_INTERNALSTATE_RCV_DEFAULT       0x0

#define COM_HC_EN_DEFAULT       		    0x0
#define COM_PHYCONFIG_DEFAULT       	    0x1
#define COM_ERR1CNT_DEFAULT       		    0x0
#define COM_ERR2CNT_DEFAULT       		    0x0