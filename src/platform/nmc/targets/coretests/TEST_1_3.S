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
	0FFF_FFFF_FFFF_FFFFhl,  0000_0000_0000_8000hl,
 0FFFF_FFFF_0FFF_FFFFhl,  0000_8002_0000_0001hl,
	0FFF_FFFF_FFFF_FFFFhl,  0000_8004_0000_0002hl,
	0FFF_FFFF_FFFF_FFFFhl,  0FFF_FFFF_0000_0003hl,
	0FFF_FFFF_FFFF_FFFFhl,  0000_8010_FFFF_FFFFhl,	
 0FFFF_FFFF_0FFF_FFFFhl,  0000_8020_0000_0005hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  0000_8040_0000_0006hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  0000_8080_0000_0007hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  0000_8100_0000_0008hl,		
 0FFFF_FFFF_0FFF_FFFFhl,  0000_8200_0000_0009hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  0000_8400_0000_000Ahl,		
 0FFFF_FFFF_0FFF_FFFFhl,  0000_8800_0000_000Bhl,
 0FFFF_FFFF_0FFF_FFFFhl,  0000_9000_0000_000Chl,		
 0FFFF_FFFF_0FFF_FFFFhl,  0000_A000_0000_000Dhl,	
 0FFFF_FFFF_0FFF_FFFFhl,  0000_C000_0000_000Ehl,	

 0000_0000_0000_8000hl,
 0000_8002_0000_0001hl,
 0000_8004_0000_0002hl,
 0FFF_FFFF_0000_0003hl,
 0000_8010_FFFF_FFFFhl,	
 0000_8020_0000_0005hl,	
 0000_8040_0000_0006hl,	
 0000_8080_0000_0007hl,	
 0000_8100_0000_0008hl,		
 0000_8200_0000_0009hl,	
 0000_8400_0000_000Ahl,		
 0000_8800_0000_000Bhl,
 0000_9000_0000_000Chl,		
 0000_A000_0000_000Dhl,	
 0000_C000_0000_000Ehl,	

0F000_0000_0000_0000hl, 0FFFF_FFFF_0000_FFFEhl, 
 0000_0000_F000_0000hl,  0000_FFFC_FFFF_FFFEhl,
0F000_0000_0000_0000hl,  0000_FFFA_FFFF_FFFDhl,
0F000_0000_0000_0000hl, 0F000_0000_FFFF_FFFChl,
0F000_0000_0000_0000hl,  0000_FFEE_0000_0000hl,
 0000_0000_F000_0000hl,  0000_FFDE_FFFF_FFFAhl,
 0000_0000_F000_0000hl,  0000_FFBE_FFFF_FFF9hl,
 0000_0000_F000_0000hl,  0000_FF7E_FFFF_FFF8hl,
 0000_0000_F000_0000hl,  0000_FEFE_FFFF_FFF7hl,
 0000_0000_F000_0000hl,  0000_FDFE_FFFF_FFF6hl,
 0000_0000_F000_0000hl,  0000_FBFE_FFFF_FFF5hl,
 0000_0000_F000_0000hl,  0000_F7FE_FFFF_FFF4hl,
 0000_0000_F000_0000hl,  0000_EFFE_FFFF_FFF3hl,
 0000_0000_F000_0000hl,  0000_DFFE_FFFF_FFF2hl,
 0000_0000_F000_0000hl,  0000_BFFE_FFFF_FFF1hl,
 
 0000_BFFE_FFFF_FFF1hl,
 0000_DFFE_FFFF_FFF2hl,
 0000_EFFE_FFFF_FFF3hl,
 0000_F7FE_FFFF_FFF4hl,
 0000_FBFE_FFFF_FFF5hl,
 0000_FDFE_FFFF_FFF6hl,
 0000_FEFE_FFFF_FFF7hl,
 0000_FF7E_FFFF_FFF8hl,
 0000_FFBE_FFFF_FFF9hl,
 0000_FFDE_FFFF_FFFAhl,
 0000_FFEE_0000_0000hl,
0F000_0000_FFFF_FFFChl,
 0000_FFFA_FFFF_FFFDhl,
 0000_FFFC_FFFF_FFFEhl,
0FFFF_FFFF_0000_FFFEhl 
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

				ar0 = 0000_8000h;  										// Set start address 0000_0000h
				gr0 = 0h;  														// Set const = 0h				
				ar1 = 0FFF_FFFFh;  						  			// Set const = 0FFF_FFFFh
				gr1 = 0FFFF_FFFFh;  								  // Set const = FFFF_FFFFh
				
// First check (address = 0000_800(0/1)h)				
				ar2 = 0000_8000h;  										// Set even start address 
				ar3 = 0000_8001h;  										// Set odd start address
				
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
				
// Next check (address = 0000_800(2/3)h)								
				ar2 = 0000_8002h;  										// Set even current address 
				ar3 = 0000_8003h;  										// Set odd current address 
				
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

// Next check (address = 0000_800(4/5)h)								
				ar2 = 0000_8004h;  										// Set even current address 
				ar3 = 0000_8005h;  										// Set odd current address 
				
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

// Next check (address = 0000_800(8/9)h)								
				ar2 = 0000_8008h;  										// Set even current address 
				ar3 = 0000_8009h;  										// Set odd current address 
				
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
				
// Next check (address = 0000_801(0/1)h)								
				ar2 = 0000_8010h;  										// Set even current address 
				ar3 = 0000_8011h;  										// Set odd current address 
				
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
				
// Next check (address = 0000_802(0/1)h)								
				ar2 = 0000_8020h;  										// Set even current address 
				ar3 = 0000_8021h;  										// Set odd current address 
				
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
								
// Next check (address = 0000_804(0/1)h)								
				ar2 = 0000_8040h;  										// Set even current address 
				ar3 = 0000_8041h;  										// Set odd current address
				
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

// Next check (address = 0000_808(0/1)h)								
				ar2 = 0000_8080h;  										// Set even current address 
				ar3 = 0000_8081h;  										// Set odd current address 
				
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

// Next check (address = 0000_810(0/1)h)								
				ar2 = 0000_8100h;  										// Set even current address 
				ar3 = 0000_8101h;  										// Set odd current address 
				
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

// Next check (address = 0000_820(0/1)h)								
				ar2 = 0000_8200h;  										// Set even current address 
				ar3 = 0000_8201h;  										// Set odd current address 
				
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
				
// Next check (address = 0000_840(0/1)h)								
				ar2 = 0000_8400h;  										// Set even current address 
				ar3 = 0000_8401h;  										// Set odd current address 
				
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

// Next check (address = 0000_880(0/1)h)								
				ar2 = 0000_8800h;  										// Set even current address 
				ar3 = 0000_8801h;  										// Set odd current address 
				
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
				
// Next check (address = 0000_900(0/1)h)								
				ar2 = 0000_9000h;  										// Set even current address 
				ar3 = 0000_9001h;  										// Set odd current address 
				
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
							
// Next check (address = 0000_A00(0/1)h)								
				ar2 = 0000_A000h;  										// Set even current address 
				ar3 = 0000_A001h;  										// Set odd current address 
				
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
				
// Next check (address = 0000_C00(0/1)h)								
				ar2 = 0000_C000h;  										// Set even current address 
				ar3 = 0000_C001h;  										// Set odd current address 
				
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
				
				ar0 = 0000_8000h;			
				ar1 = 0000_8002h;
				ar2 = 0000_8004h;
				ar3 = 0000_8008h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							

				ar0 = 0000_8010h;			
				ar1 = 0000_8020h;
				ar2 = 0000_8040h;
				ar3 = 0000_8080h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 0000_8100h;			
				ar1 = 0000_8200h;
				ar2 = 0000_8400h;
				ar3 = 0000_8800h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 0000_9000h;			
				ar1 = 0000_A000h;
				ar2 = 0000_C000h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;							
				
// **************************** The second half of test *****************************************

				ar0 = 0000_FFFEh;  										// Set start address 0000_7FFEh
				gr0 = 0FFFF_FFFFh;  									// Set const = -1				
				ar1 = 0F000_0000h;  						  		// Set const = F000_0000h
				gr1 = 0h;  								  					// Set const = 0h
				
// First check (address = 0000_FFF(E/F)h)				
				ar2 = 0000_FFFEh;  										// Set even start address 
				ar3 = 0000_FFFFh;  										// Set odd start address 
				
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
				
// Next check (address = 0000_FFF(C/D)h)								
				ar2 = 0000_FFFCh;  										// Set even current address 
				ar3 = 0000_FFFDh;  										// Set odd current address 
				
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

// Next check (address = 0000_FFF(A/B)h)								
				ar2 = 0000_FFFAh;  										// Set even current address 
				ar3 = 0000_FFFBh;  										// Set odd current address 
				
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

// Next check (address = 0000_FFF(6/7)h)								
				ar2 = 0000_FFF6h;  										// Set even current address 
				ar3 = 0000_FFF7h;  										// Set odd current address 
				
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
				
// Next check (address = 0000_FFE(E/F)h)								
				ar2 = 0000_FFEEh;  										// Set even current address 
				ar3 = 0000_FFEFh;  										// Set odd current address 
				
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
				
// Next check (address = 0000_FFD(E/F)h)								
				ar2 = 0000_FFDEh;  										// Set even current address 
				ar3 = 0000_FFDFh;  										// Set odd current address 
				
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
								
// Next check (address = 0000_FFB(E/F)h)								
				ar2 = 0000_FFBEh;  										// Set even current address 
				ar3 = 0000_FFBFh;  										// Set odd current address 
				
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

// Next check (address = 0000_FF7(E/F)h)								
				ar2 = 0000_FF7Eh;  										// Set even current address 
				ar3 = 0000_FF7Fh;  										// Set odd current address 
				
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

// Next check (address = 0000_FEF(E/F)h)								
				ar2 = 0000_FEFEh;  										// Set even current address 
				ar3 = 0000_FEFFh;  										// Set odd current address 
				
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

// Next check (address = 0000_FDF(E/F)h)								
				ar2 = 0000_FDFEh;  										// Set even current address 
				ar3 = 0000_FDFFh;  										// Set odd current address 
				
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
				
// Next check (address = 0000_FBF(E/F)h)								
				ar2 = 0000_FBFEh;  										// Set even current address 
				ar3 = 0000_FBFFh;  										// Set odd current address 
				
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

// Next check (address = 0000_F7F(E/F)h)								
				ar2 = 0000_F7FEh;  										// Set even current address 
				ar3 = 0000_F7FFh;  										// Set odd current address 
				
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
				
// Next check (address = 0000_EFF(E/F)h)								
				ar2 = 0000_EFFEh;  										// Set even current address 
				ar3 = 0000_EFFFh;  										// Set odd current address 
				
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
							
// Next check (address = 0000_DFF(E/F)h)								
				ar2 = 0000_DFFEh;  										// Set even current address 
				ar3 = 0000_DFFFh;  										// Set odd current address
				 
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
				
// Next check (address = 0000_BFF(E/F)h)								
				ar2 = 0000_BFFEh;  										// Set even current address 
				ar3 = 0000_BFFFh;  										// Set odd current address 
				
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
												
				ar0 = 0000_BFFEh;			
				ar1 = 0000_DFFEh;
				ar2 = 0000_EFFEh;
				ar3 = 0000_F7FEh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							

				ar0 = 0000_FBFEh;			
				ar1 = 0000_FDFEh;
				ar2 = 0000_FEFEh;
				ar3 = 0000_FF7Eh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 0000_FFBEh;			
				ar1 = 0000_FFDEh;
				ar2 = 0000_FFEEh;
				ar3 = 0000_FFF6h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 0000_FFFAh;			
				ar1 = 0000_FFFCh;
				ar2 = 0000_FFFEh;
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
