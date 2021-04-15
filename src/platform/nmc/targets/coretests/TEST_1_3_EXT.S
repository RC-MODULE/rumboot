// Tests for NMC4 Write Buffer (WBUF) of internal memory bank 2!!!
// **************************************************************************************************

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test_res

// The test result dimension
test_result : long[90] = (0 dup 180);

// The control value dimension

contr_result : long[90] = (
	0FFF_FFFF_FFFF_FFFFhl,  0000_0000_3001_0000hl,
 0FFFF_FFFF_0FFF_FFFFhl,  3001_0002_0000_0001hl,
	0FFF_FFFF_FFFF_FFFFhl,  3001_0004_0000_0002hl,
	0FFF_FFFF_FFFF_FFFFhl,  0FFF_FFFF_0000_0003hl,
	0FFF_FFFF_FFFF_FFFFhl,  3001_0010_FFFF_FFFFhl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_0020_0000_0005hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_0040_0000_0006hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_0080_0000_0007hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_0100_0000_0008hl,		
 0FFFF_FFFF_0FFF_FFFFhl,  3001_0200_0000_0009hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_0400_0000_000Ahl,		
 0FFFF_FFFF_0FFF_FFFFhl,  3001_0800_0000_000Bhl,
 0FFFF_FFFF_0FFF_FFFFhl,  3001_1000_0000_000Chl,		
 0FFFF_FFFF_0FFF_FFFFhl,  3001_2000_0000_000Dhl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_4000_0000_000Ehl,	

 0000_0000_3001_0000hl,
 3001_0002_0000_0001hl,
 3001_0004_0000_0002hl,
 0FFF_FFFF_0000_0003hl,
 3001_0010_FFFF_FFFFhl,	
 3001_0020_0000_0005hl,	
 3001_0040_0000_0006hl,	
 3001_0080_0000_0007hl,	
 3001_0100_0000_0008hl,		
 3001_0200_0000_0009hl,	
 3001_0400_0000_000Ahl,		
 3001_0800_0000_000Bhl,
 3001_1000_0000_000Chl,		
 3001_2000_0000_000Dhl,	
 3001_4000_0000_000Ehl,	

0F000_0000_0000_0000hl, 0FFFF_FFFF_3001_7FFEhl, 
 0000_0000_F000_0000hl,  3001_7FFC_FFFF_FFFEhl,
0F000_0000_0000_0000hl,  3001_7FFA_FFFF_FFFDhl,
0F000_0000_0000_0000hl, 0F000_0000_FFFF_FFFChl,
0F000_0000_0000_0000hl,  3001_7FEE_0000_0000hl,
 0000_0000_F000_0000hl,  3001_7FDE_FFFF_FFFAhl,
 0000_0000_F000_0000hl,  3001_7FBE_FFFF_FFF9hl,
 0000_0000_F000_0000hl,  3001_7F7E_FFFF_FFF8hl,
 0000_0000_F000_0000hl,  3001_7EFE_FFFF_FFF7hl,
 0000_0000_F000_0000hl,  3001_7DFE_FFFF_FFF6hl,
 0000_0000_F000_0000hl,  3001_7BFE_FFFF_FFF5hl,
 0000_0000_F000_0000hl,  3001_77FE_FFFF_FFF4hl,
 0000_0000_F000_0000hl,  3001_6FFE_FFFF_FFF3hl,
 0000_0000_F000_0000hl,  3001_5FFE_FFFF_FFF2hl,
 0000_0000_F000_0000hl,  3001_3FFE_FFFF_FFF1hl,
 
 3001_3FFE_FFFF_FFF1hl,
 3001_5FFE_FFFF_FFF2hl,
 3001_6FFE_FFFF_FFF3hl,
 3001_77FE_FFFF_FFF4hl,
 3001_7BFE_FFFF_FFF5hl,
 3001_7DFE_FFFF_FFF6hl,
 3001_7EFE_FFFF_FFF7hl,
 3001_7F7E_FFFF_FFF8hl,
 3001_7FBE_FFFF_FFF9hl,
 3001_7FDE_FFFF_FFFAhl,
 3001_7FEE_0000_0000hl,
0F000_0000_FFFF_FFFChl,
 3001_7FFA_FFFF_FFFDhl,
 3001_7FFC_FFFF_FFFEhl,
0FFFF_FFFF_3001_7FFEhl 
	 ) ;

save_stack : word = 0;

end data_test_res;

			     	global test_1_3 : label;
			     	   global start : label;
						    weak mytest : label;
	    weak mytest_additional: label;
Loop, Normal, Error, Finish : label;
								 
// The boot loader (address 0h)							 
begin ".text.init"
<test_1_3>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);
        ar0 = mytest_additional;  
				pc = ar0;             											// Jump to external memory memory 
				vnul;																				// Delayed instruction 1
				vnul;																				// Delayed instruction 2

<mytest_additional>			 			  		
							
				ar6 = test_result;									  // Set start address for test result (external memory)
				
// **************************** The first half of test *****************************************

				ar0 = 3001_0000h;  										// Set start address 0000_0000h
				gr0 = 0h;  														// Set const = 0h				
				ar1 = 0FFF_FFFFh;  						  			// Set const = 0FFF_FFFFh
				gr1 = 0FFFF_FFFFh;  								  // Set const = FFFF_FFFFh
				
// First check (address = 3001_000(0/1)h)				
				ar2 = 3001_0000h;  										// Set even start address 
				ar3 = 3001_0001h;  										// Set odd start address
				
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
				
// Next check (address = 3001_000(2/3)h)								
				ar2 = 3001_0002h;  										// Set even current address 
				ar3 = 3001_0003h;  										// Set odd current address 
				
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

// Next check (address = 3001_000(4/5)h)								
				ar2 = 3001_0004h;  										// Set even current address 
				ar3 = 3001_0005h;  										// Set odd current address 
				
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

// Next check (address = 3001_000(8/9)h)								
				ar2 = 3001_0008h;  										// Set even current address 
				ar3 = 3001_0009h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_001(0/1)h)								
				ar2 = 3001_0010h;  										// Set even current address 
				ar3 = 3001_0011h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_002(0/1)h)								
				ar2 = 3001_0020h;  										// Set even current address 
				ar3 = 3001_0021h;  										// Set odd current address 
				
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
								
// Next check (address = 3001_004(0/1)h)								
				ar2 = 3001_0040h;  										// Set even current address 
				ar3 = 3001_0041h;  										// Set odd current address
				
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

// Next check (address = 3001_008(0/1)h)								
				ar2 = 3001_0080h;  										// Set even current address 
				ar3 = 3001_0081h;  										// Set odd current address 
				
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

// Next check (address = 3001_010(0/1)h)								
				ar2 = 3001_0100h;  										// Set even current address 
				ar3 = 3001_0101h;  										// Set odd current address 
				
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

// Next check (address = 3001_020(0/1)h)								
				ar2 = 3001_0200h;  										// Set even current address 
				ar3 = 3001_0201h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_040(0/1)h)								
				ar2 = 3001_0400h;  										// Set even current address 
				ar3 = 3001_0401h;  										// Set odd current address 
				
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

// Next check (address = 3001_080(0/1)h)								
				ar2 = 3001_0800h;  										// Set even current address 
				ar3 = 3001_0801h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_100(0/1)h)								
				ar2 = 3001_1000h;  										// Set even current address 
				ar3 = 3001_1001h;  										// Set odd current address 
				
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
							
// Next check (address = 3001_200(0/1)h)								
				ar2 = 3001_2000h;  										// Set even current address 
				ar3 = 3001_2001h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_400(0/1)h)								
				ar2 = 3001_4000h;  										// Set even current address 
				ar3 = 3001_4001h;  										// Set odd current address 
				
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
				
				ar0 = 3001_0000h;			
				ar1 = 3001_0002h;
				ar2 = 3001_0004h;
				ar3 = 3001_0008h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							

				ar0 = 3001_0010h;			
				ar1 = 3001_0020h;
				ar2 = 3001_0040h;
				ar3 = 3001_0080h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3001_0100h;			
				ar1 = 3001_0200h;
				ar2 = 3001_0400h;
				ar3 = 3001_0800h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3001_1000h;			
				ar1 = 3001_2000h;
				ar2 = 3001_4000h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;							
				
// **************************** The second half of test *****************************************

				ar0 = 3001_7FFEh;  										// Set start address 0000_7FFEh
				gr0 = 0FFFF_FFFFh;  									// Set const = -1				
				ar1 = 0F000_0000h;  						  		// Set const = F000_0000h
				gr1 = 0h;  								  					// Set const = 0h
				
// First check (address = 3001_7FF(E/F)h)				
				ar2 = 3001_7FFEh;  										// Set even start address 
				ar3 = 3001_7FFFh;  										// Set odd start address 
				
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
				
// Next check (address = 3001_7FF(C/D)h)								
				ar2 = 3001_7FFCh;  										// Set even current address 
				ar3 = 3001_7FFDh;  										// Set odd current address 
				
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

// Next check (address = 3001_7FF(A/B)h)								
				ar2 = 3001_7FFAh;  										// Set even current address 
				ar3 = 3001_7FFBh;  										// Set odd current address 
				
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

// Next check (address = 3001_7FF(6/7)h)								
				ar2 = 3001_7FF6h;  										// Set even current address 
				ar3 = 3001_7FF7h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_7FE(E/F)h)								
				ar2 = 3001_7FEEh;  										// Set even current address 
				ar3 = 3001_7FEFh;  										// Set odd current address 
				
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
				
// Next check (address = 3001_7FD(E/F)h)								
				ar2 = 3001_7FDEh;  										// Set even current address 
				ar3 = 3001_7FDFh;  										// Set odd current address 
				
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
								
// Next check (address = 3001_7FB(E/F)h)								
				ar2 = 3001_7FBEh;  										// Set even current address 
				ar3 = 3001_7FBFh;  										// Set odd current address 
				
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

// Next check (address = 3001_7F7(E/F)h)								
				ar2 = 3001_7F7Eh;  										// Set even current address 
				ar3 = 3001_7F7Fh;  										// Set odd current address 
				
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

// Next check (address = 3001_7EF(E/F)h)								
				ar2 = 3001_7EFEh;  										// Set even current address 
				ar3 = 3001_7EFFh;  										// Set odd current address 
				
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

// Next check (address = 3001_7DF(E/F)h)								
				ar2 = 3001_7DFEh;  										// Set even current address 
				ar3 = 3001_7DFFh;  										// Set odd current address 
				
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
				
// Next check (address = 3001_7BF(E/F)h)								
				ar2 = 3001_7BFEh;  										// Set even current address 
				ar3 = 3001_7BFFh;  										// Set odd current address 
				
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

// Next check (address = 3001_77F(E/F)h)								
				ar2 = 3001_77FEh;  										// Set even current address 
				ar3 = 3001_77FFh;  										// Set odd current address 
				
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
				
// Next check (address = 3001_6FF(E/F)h)								
				ar2 = 3001_6FFEh;  										// Set even current address 
				ar3 = 3001_6FFFh;  										// Set odd current address 
				
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
							
// Next check (address = 3001_5FF(E/F)h)								
				ar2 = 3001_5FFEh;  										// Set even current address 
				ar3 = 3001_5FFFh;  										// Set odd current address
				 
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
				
// Next check (address = 3001_3FF(E/F)h)								
				ar2 = 3001_3FFEh;  										// Set even current address 
				ar3 = 3001_3FFFh;  										// Set odd current address 
				
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
												
				ar0 = 3001_3FFEh;			
				ar1 = 3001_5FFEh;
				ar2 = 3001_6FFEh;
				ar3 = 3001_77FEh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							

				ar0 = 3001_7BFEh;			
				ar1 = 3001_7DFEh;
				ar2 = 3001_7EFEh;
				ar3 = 3001_7F7Eh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3001_7FBEh;			
				ar1 = 3001_7FDEh;
				ar2 = 3001_7FEEh;
				ar3 = 3001_7FF6h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3001_7FFAh;			
				ar1 = 3001_7FFCh;
				ar2 = 3001_7FFEh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;	

// compare the results with the originals 

	ar0 = test_result;
	ar1 = contr_result;
  gr0 = 180;

<Loop>
	gr1 = [ ar0++ ];
	gr2 = [ ar1++ ];
	with gr2 - gr1;
	if <>0 goto Error;
	with gr0--;
	if <>0 goto Loop;
	
<Normal>   
				rumboot_exit(0_16800h, 0);
//	      ar0 = STOP_SIMULATION with gr1 = false;
//	      [ar0] = gr1;												 // Set 00h into the location 1_6800h (IM3), if the test is correct
	      goto Finish;
<Error>
				rumboot_exit(0_16800h, 1);
				//gr1 = 01h;
	      //ar0 = STOP_SIMULATION;
	      //[ar0] = gr1;	                       // Set 01h into the location 1_6800h (IM3), if the test is incorrect
	  
<Finish>
		    halt;										               // Wait (test finished)
		    
end ".text.init";		
