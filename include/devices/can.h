#define CCAN_CAN_Contr_R    0x000   //CAN Control Register
#define CCAN_St_R		    0x004   //Status Register
#define CCAN_Err_Count	    0x008   //Error Counter (read only)
#define CCAN_Bit_Time_R	    0x00C   //Bit Timing Register (write enabled by CCE)
#define CCAN_Interr_R	    0x010   //Interrupt Register (read only)
#define CCAN_Test_R  	    0x014   //Test Register (write enabled by CCE)
#define CCAN_BRP_Ext_R	    0x018   //BRP Extension Register (write enabled by CCE)
#define CCAN_IF1_Com_Req    0x020   //IF1 Command Request
#define CCAN_IF1_Com_M	    0x024   //IF1 Command Mask
#define CCAN_IF1_Mask_1	    0x028   //IF1 Mask 1
#define CCAN_IF1_Mask_2	    0x02C   //IF1 Mask 2
#define CCAN_IF1_Arb_1	    0x030   //IF1 Arbitration 1
#define CCAN_IF1_Arb_2	    0x034   //IF1 Arbitration 2
#define CCAN_IF1_Msg_Con    0x038   //IF1 Message Control
#define CCAN_IF1_Dat_A_1    0x03C   //IF1 Data A 1
#define CCAN_IF1_Dat_A_2    0x040   //IF1 Data A 2
#define CCAN_IF1_Dat_B_1    0x044   //IF1 Data B 1
#define CCAN_IF1_Dat_B_2    0x048   //IF1 Data B 2
#define CCAN_IF2_Com_Req    0x080   //IF2 Command Request
#define CCAN_IF2_Com_M	    0x084   //IF2 Command Mask
#define CCAN_IF2_Mask_1	    0x088   //IF2 Mask 1
#define CCAN_IF2_Mask_2	    0x08C   //IF2 Mask 2
#define CCAN_IF2_Arb_1	    0x090   //IF2 Arbitration 1
#define CCAN_IF2_Arb_2	    0x094   //IF2 Arbitration 2
#define CCAN_IF2_Msg_Con    0x098   //IF2 Message Control
#define CCAN_IF2_Dat_A_1    0x09C   //IF2 Data A 1
#define CCAN_IF2_Dat_A_2    0x0A0   //IF2 Data A 2
#define CCAN_IF2_Dat_B_1    0x0A4   //IF2 Data B 1
#define CCAN_IF2_Dat_B_2    0x0A8   //IF2 Data B 2
#define CCAN_Trans_Req_1    0x100   //Transmission Request 1 (read only)
#define CCAN_Trans_Req_2    0x104   //Transmission Request 1 (read only)
#define CCAN_New_Data_1	    0x120   //New Data 1 (read only)
#define CCAN_New_Data_2	    0x124   //New Data 2 (read only)
#define CCAN_Int_Pend_1	    0x140   //Interrupt Pending 1 (read only)
#define CCAN_Int_Pend_2	    0x144   //Interrupt Pending 1 (read only)
#define CCAN_Msg_Valid_1    0x160   //Message Valid 1 (read only)
#define CCAN_Msg_Valid_2    0x164   //Message Valid 2 (read only)