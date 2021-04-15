// Tests for NMC4 Write Buffer (WBUF) of internal memory bank 1!!!
// **************************************************************************************************

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test_res

// The test result dimension
test_result : long[90] = (0 dup 180);

// The control value dimension

contr_result : long[90] = (
	0FFF_FFFF_FFFF_FFFFhl,  0000_0000_3000_0000hl,
 0FFFF_FFFF_0FFF_FFFFhl,  3000_0002_0000_0001hl,
	0FFF_FFFF_FFFF_FFFFhl,  3000_0004_0000_0002hl,
	0FFF_FFFF_FFFF_FFFFhl,  0FFF_FFFF_0000_0003hl,
	0FFF_FFFF_FFFF_FFFFhl,  3000_0010_FFFF_FFFFhl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3000_0020_0000_0005hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3000_0040_0000_0006hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3000_0080_0000_0007hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3000_0100_0000_0008hl,		
 0FFFF_FFFF_0FFF_FFFFhl,  3000_0200_0000_0009hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3000_0400_0000_000Ahl,		
 0FFFF_FFFF_0FFF_FFFFhl,  3000_0800_0000_000Bhl,
 0FFFF_FFFF_0FFF_FFFFhl,  3000_1000_0000_000Chl,		
 0FFFF_FFFF_0FFF_FFFFhl,  3000_2000_0000_000Dhl,	 	
 0FFFF_FFFF_0FFF_FFFFhl,  3000_4000_0000_000Ehl,
 
 0000_0000_3000_0000hl,
 3000_0002_0000_0001hl,
 3000_0004_0000_0002hl,
 0FFF_FFFF_0000_0003hl,
 3000_0010_FFFF_FFFFhl,	
 3000_0020_0000_0005hl,	
 3000_0040_0000_0006hl,	
 3000_0080_0000_0007hl,	
 3000_0100_0000_0008hl,		
 3000_0200_0000_0009hl,	
 3000_0400_0000_000Ahl,		
 3000_0800_0000_000Bhl,
 3000_1000_0000_000Chl,		
 3000_2000_0000_000Dhl,	
 3000_4000_0000_000Ehl,	
 	
0F000_0000_0000_0000hl, 0FFFF_FFFF_3000_7FFEhl, 
 0000_0000_F000_0000hl,  3000_7FFC_FFFF_FFFEhl,
0F000_0000_0000_0000hl,  3000_7FFA_FFFF_FFFDhl,
0F000_0000_0000_0000hl, 0F000_0000_FFFF_FFFChl,
0F000_0000_0000_0000hl,  3000_7FEE_0000_0000hl,
 0000_0000_F000_0000hl,  3000_7FDE_FFFF_FFFAhl,
 0000_0000_F000_0000hl,  3000_7FBE_FFFF_FFF9hl,
 0000_0000_F000_0000hl,  3000_7F7E_FFFF_FFF8hl,
 0000_0000_F000_0000hl,  3000_7EFE_FFFF_FFF7hl,
 0000_0000_F000_0000hl,  3000_7DFE_FFFF_FFF6hl,
 0000_0000_F000_0000hl,  3000_7BFE_FFFF_FFF5hl,
 0000_0000_F000_0000hl,  3000_77FE_FFFF_FFF4hl,
 0000_0000_F000_0000hl,  3000_6FFE_FFFF_FFF3hl,
 0000_0000_F000_0000hl,  3000_5FFE_FFFF_FFF2hl,
 0000_0000_F000_0000hl,  3000_3FFE_FFFF_FFF1hl,
 
 3000_3FFE_FFFF_FFF1hl,
 3000_5FFE_FFFF_FFF2hl,
 3000_6FFE_FFFF_FFF3hl,
 3000_77FE_FFFF_FFF4hl,
 3000_7BFE_FFFF_FFF5hl,
 3000_7DFE_FFFF_FFF6hl,
 3000_7EFE_FFFF_FFF7hl,
 3000_7F7E_FFFF_FFF8hl,
 3000_7FBE_FFFF_FFF9hl,
 3000_7FDE_FFFF_FFFAhl,
 3000_7FEE_0000_0000hl,
0F000_0000_FFFF_FFFChl,
 3000_7FFA_FFFF_FFFDhl,
 3000_7FFC_FFFF_FFFEhl,
0FFFF_FFFF_3000_7FFEhl 
	 ) ;

save_stack : word = 0;

end data_test_res;

			     	global test_1_2 : label;
			     		 global start : label;	
						    weak mytest : label;
	    START_PROGRAM, LOOPI  : label;
Loop, Normal, Error, Finish : label;
								 
// The boot loader (address 0h)							 
begin ".text.init"
<test_1_2>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

				ar0 = START_PROGRAM;                        // Set start address of source
				ar1 = START_PROGRAM + 0000_8000h;						// Set start address of destination (IMU2)
				gr0 = 536;                                  // Move up to 536 instructions to internal global memory
				with gr0 = gr0 - 1; 												// Set flags for conditional jump	
				
<LOOPI>
				if > delayed skip LOOPI with gr0 = gr0 - 1;	// Go to LOOPI
				ar2,gr2 = [ar0++];													// Delayed instruction 1 
				[ar1++] = ar2,gr2; 											    // Delayed instruction 2 																						// 

.repeat 20;																					// The WBUF clearning
				nul;
.endrepeat;
								
				pc = START_PROGRAM + 0000_8000h;						// Jump to IMU2
				vnul;																				// Delayed instruction 1
				vnul;																				// Delayed instruction 2
				
// ********************* MAIN PROGRAM (PART 2) ********************************************
<START_PROGRAM>			 			  		
					
				ar6 = test_result + 8000h;									// Set start address for test result 
				
// **************************** The first half of test *****************************************

				ar0 = 3000_0000h;  										// Set start address 3000_0000h
				gr0 = 0h;  														// Set const = 0h				
				ar1 = 0FFF_FFFFh;  						  			// Set const = 0FFF_FFFFh
				gr1 = 0FFFF_FFFFh;  								  // Set const = FFFF_FFFFh
				
// First check (address = 3000_000(0/1)h)				
				ar2 = 3000_0000h;  										// Set even start address 
				ar3 = 3000_0001h;  										// Set odd start address
				
				[ar3] = ar2;    
				[ar2] = ar2;
				[ar3] = ar3;								
				[ar2] = ar0,gr0;
				[ar2] = gr1;
				[ar3] = ar1;
				ar4,gr4 = [ar2];
				[ar2] = ar0,gr0 with gr0 = gr0 + 1;
				ar5,gr5 = [ar2];
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;
				
// Next check (address = 3000_000(2/3)h)								
				ar2 = 3000_0002h;  										// Set even current address 
				ar3 = 3000_0003h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;						
				[ar2] = ar0;
				[ar3] = gr0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar2] = gr0 with gr0 = gr0 + 1;
				[ar3] = ar2; 				
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;

// Next check (address = 3000_000(4/5)h)								
				ar2 = 3000_0004h;  										// Set even current address 
				ar3 = 3000_0005h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;						
				[ar2] = ar0;
				[ar3] = ar1;
				[ar2] = gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;

// Next check (address = 3000_000(8/9)h)								
				ar2 = 3000_0008h;  										// Set even current address 
				ar3 = 3000_0009h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar3] = ar0;
				[ar3] = ar1;
				[ar2] = gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;		
				
// Next check (address = 3000_001(0/1)h)								
				ar2 = 3000_0010h;  										// Set even current address 
				ar3 = 3000_0011h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;
        [ar2] = ar0;
				[ar3] = ar1;
				[ar2] = gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
 				[ar3] = ar2; 						
				with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;					
				
// Next check (address = 3000_002(0/1)h)								
				ar2 = 3000_0020h;  										// Set even current address 
				ar3 = 3000_0021h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;				
								
// Next check (address = 3000_004(0/1)h)								
				ar2 = 3000_0040h;  										// Set even current address 
				ar3 = 3000_0041h;  										// Set odd current address
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;		

// Next check (address = 3000_008(0/1)h)								
				ar2 = 3000_0080h;  										// Set even current address 
				ar3 = 3000_0081h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;

// Next check (address = 3000_010(0/1)h)								
				ar2 = 3000_0100h;  										// Set even current address 
				ar3 = 3000_0101h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;

// Next check (address = 3000_020(0/1)h)								
				ar2 = 3000_0200h;  										// Set even current address 
				ar3 = 3000_0201h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;			
				
// Next check (address = 3000_040(0/1)h)								
				ar2 = 3000_0400h;  										// Set even current address 
				ar3 = 3000_0401h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;						

// Next check (address = 3000_080(0/1)h)								
				ar2 = 3000_0800h;  										// Set even current address 
				ar3 = 3000_0801h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;		
				
// Next check (address = 3000_100(0/1)h)								
				ar2 = 3000_1000h;  										// Set even current address 
				ar3 = 3000_1001h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;		
							
// Next check (address = 3000_200(0/1)h)								
				ar2 = 3000_2000h;  										// Set even current address 
				ar3 = 3000_2001h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;
				
// Next check (address = 3000_400(0/1)h)								
				ar2 = 3000_4000h;  										// Set even current address 
				ar3 = 3000_4001h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 + 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;	
		
// Read the results from memory	(not from WBUF) and write to external memory

.repeat 20;																			// The WBUF clearning
				nul;
.endrepeat;
				
				ar0 = 3000_0000h;			
				ar1 = 3000_0002h;
				ar2 = 3000_0004h;
				ar3 = 3000_0008h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							

				ar0 = 3000_0010h;			
				ar1 = 3000_0020h;
				ar2 = 3000_0040h;
				ar3 = 3000_0080h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3000_0100h;			
				ar1 = 3000_0200h;
				ar2 = 3000_0400h;
				ar3 = 3000_0800h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3000_1000h;			
				ar1 = 3000_2000h;
				ar2 = 3000_4000h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;							
				
// **************************** The second half of test *****************************************

				ar0 = 3000_7FFEh;  										// Set start address 3000_7FFEh
				gr0 = 0FFFF_FFFFh;  									// Set const = -1				
				ar1 = 0F000_0000h;  						  		// Set const = F000_0000h
				gr1 = 0h;  								  					// Set const = 0h
				
// First check (address = 3000_7FF(E/F)h)				
				ar2 = 3000_7FFEh;  										// Set even start address 
				ar3 = 3000_7FFFh;  										// Set odd start address 
				
				[ar3] = ar2;				
				[ar2] = ar2;
				[ar3] = ar3;				
				[ar2] = ar0,gr0;
				[ar2] = gr1;
				[ar3] = ar1;
				ar4,gr4 = [ar2];
				[ar2] = ar0,gr0 with gr0 = gr0 - 1;
				ar5,gr5 = [ar2];
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;
				
// Next check (address = 3000_7FF(C/D)h)								
				ar2 = 3000_7FFCh;  										// Set even current address 
				ar3 = 3000_7FFDh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = gr0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar2] = gr0 with gr0 = gr0 - 1;
				[ar3] = ar2; 				
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;

// Next check (address = 3000_7FF(A/B)h)								
				ar2 = 3000_7FFAh;  										// Set even current address 
				ar3 = 3000_7FFBh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar1;
				[ar2] = gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;

// Next check (address = 3000_FFF(6/7)h)								
				ar2 = 3000_7FF6h;  										// Set even current address 
				ar3 = 3000_7FF7h;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar3] = ar0;
				[ar3] = ar1;
				[ar2] = gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;		
				
// Next check (address = 3000_7FE(E/F)h)								
				ar2 = 3000_7FEEh;  										// Set even current address 
				ar3 = 3000_7FEFh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar1;
				[ar2] = gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
 				[ar3] = ar2; 						
				with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;					
				
// Next check (address = 3000_7FD(E/F)h)								
				ar2 = 3000_7FDEh;  										// Set even current address 
				ar3 = 3000_7FDFh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;				
								
// Next check (address = 3000_7FB(E/F)h)								
				ar2 = 3000_7FBEh;  										// Set even current address 
				ar3 = 3000_7FBFh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;		

// Next check (address = 3000_7F7(E/F)h)								
				ar2 = 3000_7F7Eh;  										// Set even current address 
				ar3 = 3000_7F7Fh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;

// Next check (address = 3000_7EF(E/F)h)								
				ar2 = 3000_7EFEh;  										// Set even current address 
				ar3 = 3000_7EFFh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;

// Next check (address = 3000_7DF(E/F)h)								
				ar2 = 3000_7DFEh;  										// Set even current address 
				ar3 = 3000_7DFFh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;			
				
// Next check (address = 3000_7BF(E/F)h)								
				ar2 = 3000_7BFEh;  										// Set even current address 
				ar3 = 3000_7BFFh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;						

// Next check (address = 3000_77F(E/F)h)								
				ar2 = 3000_77FEh;  										// Set even current address 
				ar3 = 3000_77FFh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;		
				
// Next check (address = 3000_6FF(E/F)h)								
				ar2 = 3000_6FFEh;  										// Set even current address 
				ar3 = 3000_6FFFh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;		
							
// Next check (address = 3000_5FF(E/F)h)								
				ar2 = 3000_5FFEh;  										// Set even current address 
				ar3 = 3000_5FFFh;  										// Set odd current address
				 
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;
				
// Next check (address = 3000_3FF(E/F)h)								
				ar2 = 3000_3FFEh;  										// Set even current address 
				ar3 = 3000_3FFFh;  										// Set odd current address 
				
				[ar2] = ar2;
				[ar3] = ar3;					
				[ar2] = ar0;
				[ar3] = ar0;
				[ar2] = ar1,gr1;
				[ar0] = ar0;
				ar4,gr4 = [ar2];
				[ar3] = ar2; 						
				[ar2] = gr0 with gr0 = gr0 - 1;		
				ar5,gr5 = [ar2];				
				[ar6++] = ar4,gr4;
				[ar6++] = ar5,gr5;		
			
// Read the results from memory	(not from WBUF) and write to external memory

.repeat 20;																			// The WBUF clearning
				nul;
.endrepeat;
												
				ar0 = 3000_3FFEh;			
				ar1 = 3000_5FFEh;
				ar2 = 3000_6FFEh;
				ar3 = 3000_77FEh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							

				ar0 = 3000_7BFEh;			
				ar1 = 3000_7DFEh;
				ar2 = 3000_7EFEh;
				ar3 = 3000_7F7Eh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3000_7FBEh;			
				ar1 = 3000_7FDEh;
				ar2 = 3000_7FEEh;
				ar3 = 3000_7FF6h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3000_7FFAh;			
				ar1 = 3000_7FFCh;
				ar2 = 3000_7FFEh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;	

// compare the results with the originals 

	ar0 = test_result + 8000h;
	ar1 = contr_result + 8000h;
  gr0 = 180;

<Loop>
	gr1 = [ ar0++ ];
	gr2 = [ ar1++ ];
	with gr2 - gr1;
	if <>0 skip Error;
	with gr0--;
	if <>0 skip Loop;

<Normal>   
				rumboot_exit(0_16800h, 0);
//	      ar0 = STOP_SIMULATION with gr1 = false;
//	      [ar0] = gr1;												 // Set 00h into the location 1_6800h (IM3), if the test is correct
	      skip Finish;
<Error>
				rumboot_exit(0_16800h, 1);
				//gr1 = 01h;
	      //ar0 = STOP_SIMULATION;
	      //[ar0] = gr1;	                       // Set 01h into the location 1_6800h (IM3), if the test is incorrect
	  
<Finish>
		    halt;										               // Wait (test finished)
		    
end ".text.init";		
