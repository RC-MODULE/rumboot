// Tests for NMC4 Write Buffer (WBUF) of internal memory bank 3!!!
// **************************************************************************************************

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test_res

// The test result dimension
test_result : long[90] = (0 dup 180);

// The control value dimension

contr_result : long[90] = (
	0FFF_FFFF_FFFF_FFFFhl,  0000_0000_3001_8000hl,
 0FFFF_FFFF_0FFF_FFFFhl,  3001_8002_0000_0001hl,
	0FFF_FFFF_FFFF_FFFFhl,  3001_8004_0000_0002hl,
	0FFF_FFFF_FFFF_FFFFhl,  0FFF_FFFF_0000_0003hl,
	0FFF_FFFF_FFFF_FFFFhl,  3001_8010_FFFF_FFFFhl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_8020_0000_0005hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_8040_0000_0006hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_8080_0000_0007hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_8100_0000_0008hl,		
 0FFFF_FFFF_0FFF_FFFFhl,  3001_8200_0000_0009hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_8400_0000_000Ahl,		
 0FFFF_FFFF_0FFF_FFFFhl,  3001_8800_0000_000Bhl,
 0FFFF_FFFF_0FFF_FFFFhl,  3001_9000_0000_000Chl,		
 0FFFF_FFFF_0FFF_FFFFhl,  3001_A000_0000_000Dhl,	
 0FFFF_FFFF_0FFF_FFFFhl,  3001_C000_0000_000Ehl,	

 0000_0000_3001_8000hl,
 3001_8002_0000_0001hl,
 3001_8004_0000_0002hl,
 0FFF_FFFF_0000_0003hl,
 3001_8010_FFFF_FFFFhl,	
 3001_8020_0000_0005hl,	
 3001_8040_0000_0006hl,	
 3001_8080_0000_0007hl,	
 3001_8100_0000_0008hl,		
 3001_8200_0000_0009hl,	
 3001_8400_0000_000Ahl,		
 3001_8800_0000_000Bhl,
 3001_9000_0000_000Chl,		
 3001_A000_0000_000Dhl,	
 3001_C000_0000_000Ehl,	

0F000_0000_0000_0000hl, 0FFFF_FFFF_3001_FFFEhl, 
 0000_0000_F000_0000hl,  3001_FFFC_FFFF_FFFEhl,
0F000_0000_0000_0000hl,  3001_FFFA_FFFF_FFFDhl,
0F000_0000_0000_0000hl, 0F000_0000_FFFF_FFFChl,
0F000_0000_0000_0000hl,  3001_FFEE_0000_0000hl,
 0000_0000_F000_0000hl,  3001_FFDE_FFFF_FFFAhl,
 0000_0000_F000_0000hl,  3001_FFBE_FFFF_FFF9hl,
 0000_0000_F000_0000hl,  3001_FF7E_FFFF_FFF8hl,
 0000_0000_F000_0000hl,  3001_FEFE_FFFF_FFF7hl,
 0000_0000_F000_0000hl,  3001_FDFE_FFFF_FFF6hl,
 0000_0000_F000_0000hl,  3001_FBFE_FFFF_FFF5hl,
 0000_0000_F000_0000hl,  3001_F7FE_FFFF_FFF4hl,
 0000_0000_F000_0000hl,  3001_EFFE_FFFF_FFF3hl,
 0000_0000_F000_0000hl,  3001_DFFE_FFFF_FFF2hl,
 0000_0000_F000_0000hl,  3001_BFFE_FFFF_FFF1hl,
 
 3001_BFFE_FFFF_FFF1hl,
 3001_DFFE_FFFF_FFF2hl,
 3001_EFFE_FFFF_FFF3hl,
 3001_F7FE_FFFF_FFF4hl,
 3001_FBFE_FFFF_FFF5hl,
 3001_FDFE_FFFF_FFF6hl,
 3001_FEFE_FFFF_FFF7hl,
 3001_FF7E_FFFF_FFF8hl,
 3001_FFBE_FFFF_FFF9hl,
 3001_FFDE_FFFF_FFFAhl,
 3001_FFEE_0000_0000hl,
0F000_0000_FFFF_FFFChl,
 3001_FFFA_FFFF_FFFDhl,
 3001_FFFC_FFFF_FFFEhl,
0FFFF_FFFF_3001_FFFEhl 
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

				ar0 = 3001_8000h;  										// Set start address 0000_0000h
				gr0 = 0h;  														// Set const = 0h				
				ar1 = 0FFF_FFFFh;  						  			// Set const = 0FFF_FFFFh
				gr1 = 0FFFF_FFFFh;  								  // Set const = FFFF_FFFFh
				
// First check (address = 3001_800(0/1)h)				
				ar2 = 3001_8000h;  										// Set even start address 
				ar3 = 3001_8001h;  										// Set odd start address
				
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
				
// Next check (address = 3001_800(2/3)h)								
				ar2 = 3001_8002h;  										// Set even current address 
				ar3 = 3001_8003h;  										// Set odd current address 
				
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

// Next check (address = 3001_800(4/5)h)								
				ar2 = 3001_8004h;  										// Set even current address 
				ar3 = 3001_8005h;  										// Set odd current address 
				
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

// Next check (address = 3001_800(8/9)h)								
				ar2 = 3001_8008h;  										// Set even current address 
				ar3 = 3001_8009h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_801(0/1)h)								
				ar2 = 3001_8010h;  										// Set even current address 
				ar3 = 3001_8011h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_802(0/1)h)								
				ar2 = 3001_8020h;  										// Set even current address 
				ar3 = 3001_8021h;  										// Set odd current address 
				
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
								
// Next check (address = 3001_804(0/1)h)								
				ar2 = 3001_8040h;  										// Set even current address 
				ar3 = 3001_8041h;  										// Set odd current address
				
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

// Next check (address = 3001_808(0/1)h)								
				ar2 = 3001_8080h;  										// Set even current address 
				ar3 = 3001_8081h;  										// Set odd current address 
				
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

// Next check (address = 3001_810(0/1)h)								
				ar2 = 3001_8100h;  										// Set even current address 
				ar3 = 3001_8101h;  										// Set odd current address 
				
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

// Next check (address = 3001_820(0/1)h)								
				ar2 = 3001_8200h;  										// Set even current address 
				ar3 = 3001_8201h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_840(0/1)h)								
				ar2 = 3001_8400h;  										// Set even current address 
				ar3 = 3001_8401h;  										// Set odd current address 
				
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

// Next check (address = 3001_880(0/1)h)								
				ar2 = 3001_8800h;  										// Set even current address 
				ar3 = 3001_8801h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_900(0/1)h)								
				ar2 = 3001_9000h;  										// Set even current address 
				ar3 = 3001_9001h;  										// Set odd current address 
				
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
							
// Next check (address = 3001_A00(0/1)h)								
				ar2 = 3001_A000h;  										// Set even current address 
				ar3 = 3001_A001h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_C00(0/1)h)								
				ar2 = 3001_C000h;  										// Set even current address 
				ar3 = 3001_C001h;  										// Set odd current address 
				
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
				
				ar0 = 3001_8000h;			
				ar1 = 3001_8002h;
				ar2 = 3001_8004h;
				ar3 = 3001_8008h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							

				ar0 = 3001_8010h;			
				ar1 = 3001_8020h;
				ar2 = 3001_8040h;
				ar3 = 3001_8080h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3001_8100h;			
				ar1 = 3001_8200h;
				ar2 = 3001_8400h;
				ar3 = 3001_8800h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3001_9000h;			
				ar1 = 3001_A000h;
				ar2 = 3001_C000h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;							
				
// **************************** The second half of test *****************************************

				ar0 = 3001_FFFEh;  										// Set start address 0000_7FFEh
				gr0 = 0FFFF_FFFFh;  									// Set const = -1				
				ar1 = 0F000_0000h;  						  		// Set const = F000_0000h
				gr1 = 0h;  								  					// Set const = 0h
				
// First check (address = 3001_FFF(E/F)h)				
				ar2 = 3001_FFFEh;  										// Set even start address 
				ar3 = 3001_FFFFh;  										// Set odd start address 
				
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
				
// Next check (address = 3001_FFF(C/D)h)								
				ar2 = 3001_FFFCh;  										// Set even current address 
				ar3 = 3001_FFFDh;  										// Set odd current address 
				
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

// Next check (address = 3001_FFF(A/B)h)								
				ar2 = 3001_FFFAh;  										// Set even current address 
				ar3 = 3001_FFFBh;  										// Set odd current address 
				
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

// Next check (address = 3001_FFF(6/7)h)								
				ar2 = 3001_FFF6h;  										// Set even current address 
				ar3 = 3001_FFF7h;  										// Set odd current address 
				
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
				
// Next check (address = 3001_FFE(E/F)h)								
				ar2 = 3001_FFEEh;  										// Set even current address 
				ar3 = 3001_FFEFh;  										// Set odd current address 
				
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
				
// Next check (address = 3001_FFD(E/F)h)								
				ar2 = 3001_FFDEh;  										// Set even current address 
				ar3 = 3001_FFDFh;  										// Set odd current address 
				
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
								
// Next check (address = 3001_FFB(E/F)h)								
				ar2 = 3001_FFBEh;  										// Set even current address 
				ar3 = 3001_FFBFh;  										// Set odd current address 
				
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

// Next check (address = 3001_FF7(E/F)h)								
				ar2 = 3001_FF7Eh;  										// Set even current address 
				ar3 = 3001_FF7Fh;  										// Set odd current address 
				
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

// Next check (address = 3001_FEF(E/F)h)								
				ar2 = 3001_FEFEh;  										// Set even current address 
				ar3 = 3001_FEFFh;  										// Set odd current address 
				
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

// Next check (address = 3001_FDF(E/F)h)								
				ar2 = 3001_FDFEh;  										// Set even current address 
				ar3 = 3001_FDFFh;  										// Set odd current address 
				
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
				
// Next check (address = 3001_FBF(E/F)h)								
				ar2 = 3001_FBFEh;  										// Set even current address 
				ar3 = 3001_FBFFh;  										// Set odd current address 
				
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

// Next check (address = 3001_F7F(E/F)h)								
				ar2 = 3001_F7FEh;  										// Set even current address 
				ar3 = 3001_F7FFh;  										// Set odd current address 
				
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
				
// Next check (address = 3001_EFF(E/F)h)								
				ar2 = 3001_EFFEh;  										// Set even current address 
				ar3 = 3001_EFFFh;  										// Set odd current address 
				
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
							
// Next check (address = 3001_DFF(E/F)h)								
				ar2 = 3001_DFFEh;  										// Set even current address 
				ar3 = 3001_DFFFh;  										// Set odd current address
				 
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
				
// Next check (address = 3001_BFF(E/F)h)								
				ar2 = 3001_BFFEh;  										// Set even current address 
				ar3 = 3001_BFFFh;  										// Set odd current address 
				
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
												
				ar0 = 3001_BFFEh;			
				ar1 = 3001_DFFEh;
				ar2 = 3001_EFFEh;
				ar3 = 3001_F7FEh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							

				ar0 = 3001_FBFEh;			
				ar1 = 3001_FDFEh;
				ar2 = 3001_FEFEh;
				ar3 = 3001_FF7Eh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3001_FFBEh;			
				ar1 = 3001_FFDEh;
				ar2 = 3001_FFEEh;
				ar3 = 3001_FFF6h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 3001_FFFAh;			
				ar1 = 3001_FFFCh;
				ar2 = 3001_FFFEh;
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
