#ifndef GLOBAL_TIMERS_H
#define GLOBAL_TIMERS_H


#define ID               	 0x00 	// 0x00 	| RO
#define VERSION          	 0x01 	// 0x04 	| RO
#define FREE_RUN_L       	 0x02 	// 0x08 	| RO
#define FREE_RUN_H       	 0x03 	// 0x0c 	| RO
#define GLOBAL_NS        	 0x04 	// 0x10 	| RO
#define GLOBAL_S_L      `	 0x05 	// 0x14 	| RO
#define GLOBAL_S_H       	 0x06 	// 0x18 	| RO
#define SW_RST           	 0x07 	// 0x1c 	| RW
#define FIX_CMD          	 0x08 	// 0x20 	| RW 
#define INC_NS           	 0x09 	// 0x24 	| RW
#define INC_S            	 0x0a 	// 0x28		| RW
#define MOVE_NS          	 0x0b 	// 0x2c 	| RW
#define MOVE_S           	 0x0c 	// 0x30 	| RW
#define SET_GLOBAL_NS    	 0x0d 	// 0x34	 	| RW
#define SET_GLOBAL_S_L   	 0x0e 	// 0x38 	| RW
#define SET_GLOBAL_S_H   	 0x0f 	// 0x3C 	| RW

#define ENABLE   			 0x10	// 0x0x100 	| RW
#define TMR_0_STATE   		 0x40	// 0x0x100 	| RW
#define TMR_0_LIMIT   		 0x41	// 0x0x104 	| RW
#define TMR_0_GLOB_CNT_NS 	 0x42	// 0x0x108 	| RW
#define TMR_0_GLOB_CNT_S_L 	 0x43	// 0x0x10c 	| RW
#define TMR_0_GLOB_CNT_S_H 	 0x44	// 0x0x110 	| RW

#define TMR_1_STATE   		 0x48	// 0x0x120 	| RW
#define TMR_1_LIMIT   		 0x49	// 0x0x124 	| RW
#define TMR_1_GLOB_CNT_NS 	 0x4a	// 0x0x128 	| RW
#define TMR_1_GLOB_CNT_S_L 	 0x4b	// 0x0x104 	| RW
#define TMR_1_GLOB_CNT_S_H 	 0x4c	// 0x0x104 	| RW

#endif /* end of include guard: GLOBAL_TIMERS_H */
