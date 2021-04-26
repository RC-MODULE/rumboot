
#ifndef REGS_RCM_CP
#define REGS_RCM_CP

#define RCM_CP_MAINCOUNTER_TR            0x00
#define RCM_CP_ADDRESS_TR           	 0x04
#define RCM_CP_BIAS_TR           	 	 0x08
#define RCM_CP_ROWCOUNTER_TR             0x0C
#define RCM_CP_ADDRESSMODE_TR            0x10
#define RCM_CP_CSR_TR           	 	 0x14
#define RCM_CP_INTERRUPTMASK_TR          0x18
#define RCM_CP_INTERNALSTATE_TR          0x1C
#define RCM_CP_IH_EN_TR       			 0x40
#define RCM_CP_MAINCOUNTER_RCV           0x20

#define RCM_CP_ADDRESS_RCV           	 0x24
#define RCM_CP_BIAS_RCV           	 	 0x28
#define RCM_CP_ROWCOUNTER_RCV            0x2C
#define RCM_CP_ADDRESSMODE_RCV           0x30
#define RCM_CP_CSR_RCV           	 	 0x34
#define RCM_CP_INTERRUPTMASK_RCV         0x38
#define RCM_CP_INTERNALSTATE_RCV         0x3C
#define RCM_CP_IH_EN_RCV       		     0x40
#define RCM_CP_PHYCONFIG       		     0x44
#define RCM_CP_ERR1CNT_RCV       		 0x68
#define RCM_CP_ERR2CNT_RCV       		 0x6C

#endif