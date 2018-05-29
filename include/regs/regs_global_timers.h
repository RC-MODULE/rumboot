#ifndef GLOBAL_TIMERS_H
#define GLOBAL_TIMERS_H


#define GT_ID                0x00 	//	| RO
#define GT_VER          	 0x04 	// 	| RO
#define FREE_RUN_L       	 0x08 	// 	| RO
#define FREE_RUN_H       	 0x0c  	// 	| RO
#define GLOBAL_NS        	 0x10 	// 	| RO
#define GLOBAL_S_L      `	 0x14 	// 	| RO
#define GLOBAL_S_H       	 0x18 	// 	| RO
#define SW_RST           	 0x1C 	// 	| RW
#define FIX_CMD          	 0x20 	// 	| RW 
#define INC_NS           	 0x24 	// 	| RW
#define INC_S            	 0x28 	// 	| RW
#define MOVE_NS          	 0x2C 	// 	| RW
#define MOVE_S           	 0x30 	// 	| RW
#define SET_GLOBAL_NS    	 0x34 	// 	| RW
#define SET_GLOBAL_S_L   	 0x38 	// 	| RW
#define SET_GLOBAL_S_H   	 0x3C 	// 	| RW

#define ENABLE            	 0x40	// 	| RW
#define TMR_0_STATE   		 0x400	// 	| RW
#define TMR_0_LIMIT   		 0x404	// 	| RW
#define TMR_0_GLOB_CNT_NS 	 0x408	// 	| RW
#define TMR_0_GLOB_CNT_S_L 	 0x40c	// 	| RW
#define TMR_0_GLOB_CNT_S_H 	 0x410	// 	| RW

#define TMR_1_STATE   		 0x420	// 	| RW
#define TMR_1_LIMIT   		 0x424	// 	| RW
#define TMR_1_GLOB_CNT_NS 	 0x428	// 	| RW
#define TMR_1_GLOB_CNT_S_L 	 0x42c	// 	| RW
#define TMR_1_GLOB_CNT_S_H 	 0x430	// 	| RW

#endif /* end of include guard: GLOBAL_TIMERS_H */
