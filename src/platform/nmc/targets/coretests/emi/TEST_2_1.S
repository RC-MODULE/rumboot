// Tests for NMC4 Write Buffer (WBUF) of external memory bank 0!!!
// **************************************************************************************************

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test_res

// The test result dimension
test_result : long[108] = (0l dup 108);

// The control value dimension

contr_result : long[108] = (
	0FFF_FFFF_FFFF_FFFFhl,  0000_0000_4000_0000hl,
 0FFFF_FFFF_0FFF_FFFFhl,  4000_0002_0000_0001hl,
	0FFF_FFFF_FFFF_FFFFhl,  4000_0004_0000_0002hl,
	0FFF_FFFF_FFFF_FFFFhl,  0FFF_FFFF_0000_0003hl,
	0FFF_FFFF_FFFF_FFFFhl,  4000_0010_FFFF_FFFFhl,	
 0FFFF_FFFF_0FFF_FFFFhl,  4000_0020_0000_0005hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  4000_0040_0000_0006hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  4000_0080_0000_0007hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  4000_0100_0000_0008hl,		
 0FFFF_FFFF_0FFF_FFFFhl,  4000_0200_0000_0009hl,	
 0FFFF_FFFF_0FFF_FFFFhl,  4000_0400_0000_000Ahl,		
 0FFFF_FFFF_0FFF_FFFFhl,  4000_0800_0000_000Bhl,
 0FFFF_FFFF_0FFF_FFFFhl,  4000_1000_0000_000Chl,		
 0FFFF_FFFF_0FFF_FFFFhl,  4000_2000_0000_000Dhl,	
 0FFFF_FFFF_0FFF_FFFFhl,  4000_4000_0000_000Ehl, 
 0FFFF_FFFF_0FFF_FFFFhl,  4000_8000_0000_000Fhl, 
 0FFFF_FFFF_0FFF_FFFFhl,  4001_0000_0000_0010hl, 
 0FFFF_FFFF_0FFF_FFFFhl,  4002_0000_0000_0011hl, 	

 0000_0000_4000_0000hl,
 4000_0002_0000_0001hl,
 4000_0004_0000_0002hl,
 0FFF_FFFF_0000_0003hl,
 4000_0010_FFFF_FFFFhl,	
 4000_0020_0000_0005hl,	
 4000_0040_0000_0006hl,	
 4000_0080_0000_0007hl,	
 4000_0100_0000_0008hl,		
 4000_0200_0000_0009hl,	
 4000_0400_0000_000Ahl,		
 4000_0800_0000_000Bhl,
 4000_1000_0000_000Chl,		
 4000_2000_0000_000Dhl,	
 4000_4000_0000_000Ehl,	
 4000_8000_0000_000Fhl, 
 4001_0000_0000_0010hl, 
 4002_0000_0000_0011hl,

0F000_0000_0000_0000hl, 0FFFF_FFFF_4003_FFFEhl, 
 0000_0000_F000_0000hl,  4003_FFFC_FFFF_FFFEhl,
0F000_0000_0000_0000hl,  4003_FFFA_FFFF_FFFDhl,
0F000_0000_0000_0000hl, 0F000_0000_FFFF_FFFChl,
0F000_0000_0000_0000hl,  4003_FFEE_0000_0000hl,
 0000_0000_F000_0000hl,  4003_FFDE_FFFF_FFFAhl,
 0000_0000_F000_0000hl,  4003_FFBE_FFFF_FFF9hl,
 0000_0000_F000_0000hl,  4003_FF7E_FFFF_FFF8hl,
 0000_0000_F000_0000hl,  4003_FEFE_FFFF_FFF7hl,
 0000_0000_F000_0000hl,  4003_FDFE_FFFF_FFF6hl,
 0000_0000_F000_0000hl,  4003_FBFE_FFFF_FFF5hl,
 0000_0000_F000_0000hl,  4003_F7FE_FFFF_FFF4hl,
 0000_0000_F000_0000hl,  4003_EFFE_FFFF_FFF3hl,
 0000_0000_F000_0000hl,  4003_DFFE_FFFF_FFF2hl,
 0000_0000_F000_0000hl,  4003_BFFE_FFFF_FFF1hl,  
 0000_0000_F000_0000hl,  4003_7FFE_FFFF_FFF0hl,
 0000_0000_F000_0000hl,  4002_FFFE_FFFF_FFEFhl,
 0000_0000_F000_0000hl,  4001_FFFE_FFFF_FFEEhl,
 
 4001_FFFE_FFFF_FFEEhl,
 4002_FFFE_FFFF_FFEFhl,
 4003_7FFE_FFFF_FFF0hl,
 4003_BFFE_FFFF_FFF1hl,
 4003_DFFE_FFFF_FFF2hl,
 4003_EFFE_FFFF_FFF3hl,
 4003_F7FE_FFFF_FFF4hl,
 4003_FBFE_FFFF_FFF5hl,
 4003_FDFE_FFFF_FFF6hl,
 4003_FEFE_FFFF_FFF7hl,
 4003_FF7E_FFFF_FFF8hl,
 4003_FFBE_FFFF_FFF9hl,
 4003_FFDE_FFFF_FFFAhl,
 4003_FFEE_0000_0000hl,
0F000_0000_FFFF_FFFChl,
 4003_FFFA_FFFF_FFFDhl,
 4003_FFFC_FFFF_FFFEhl,
0FFFF_FFFF_4003_FFFEhl 
	 ) ;

save_stack : word = 0;

end data_test_res;

			     	global test_2_1 : label;
			     	   global start : label;
						    weak mytest : label;
	    weak mytest_additional: label;
Loop, Normal, Error, Finish : label;
								 
// The boot loader (address 0h)							 
begin ".text.init"
<test_2_1>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);
			
				ar6 = test_result;									  // Set start address for test result (external memory)
				
// **************************** The first half of test *****************************************

				ar0 = 4000_0000h;  										// Set start address 4000_0000h
				gr0 = 0h;  														// Set const = 0h				
				ar1 = 0FFF_FFFFh;  						  			// Set const = 0FFF_FFFFh
				gr1 = 0FFFF_FFFFh;  								  // Set const = FFFF_FFFFh
				
// First check (address = 4000_000(0/1)h)				
				ar2 = 4000_0000h;  										// Set even start address 
				ar3 = 4000_0001h;  										// Set odd start address
				
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
				
// Next check (address = 4000_000(2/3)h)								
				ar2 = 4000_0002h;  										// Set even current address 
				ar3 = 4000_0003h;  										// Set odd current address 
				
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

// Next check (address = 4000_000(4/5)h)								
				ar2 = 4000_0004h;  										// Set even current address 
				ar3 = 4000_0005h;  										// Set odd current address 
				
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

// Next check (address = 4000_000(8/9)h)								
				ar2 = 4000_0008h;  										// Set even current address 
				ar3 = 4000_0009h;  										// Set odd current address 
				
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
				
// Next check (address = 4000_001(0/1)h)								
				ar2 = 4000_0010h;  										// Set even current address 
				ar3 = 4000_0011h;  										// Set odd current address 
				
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
				
// Next check (address = 4000_002(0/1)h)								
				ar2 = 4000_0020h;  										// Set even current address 
				ar3 = 4000_0021h;  										// Set odd current address 
				
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
								
// Next check (address = 4000_004(0/1)h)								
				ar2 = 4000_0040h;  										// Set even current address 
				ar3 = 4000_0041h;  										// Set odd current address
				
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

// Next check (address = 4000_008(0/1)h)								
				ar2 = 4000_0080h;  										// Set even current address 
				ar3 = 4000_0081h;  										// Set odd current address 
				
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

// Next check (address = 4000_010(0/1)h)								
				ar2 = 4000_0100h;  										// Set even current address 
				ar3 = 4000_0101h;  										// Set odd current address 
				
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

// Next check (address = 4000_020(0/1)h)								
				ar2 = 4000_0200h;  										// Set even current address 
				ar3 = 4000_0201h;  										// Set odd current address 
				
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
				
// Next check (address = 4000_040(0/1)h)								
				ar2 = 4000_0400h;  										// Set even current address 
				ar3 = 4000_0401h;  										// Set odd current address 
				
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

// Next check (address = 4000_080(0/1)h)								
				ar2 = 4000_0800h;  										// Set even current address 
				ar3 = 4000_0801h;  										// Set odd current address 
				
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
				
// Next check (address = 4000_100(0/1)h)								
				ar2 = 4000_1000h;  										// Set even current address 
				ar3 = 4000_1001h;  										// Set odd current address 
				
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
							
// Next check (address = 4000_200(0/1)h)								
				ar2 = 4000_2000h;  										// Set even current address 
				ar3 = 4000_2001h;  										// Set odd current address 
				
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
				
// Next check (address = 4000_400(0/1)h)								
				ar2 = 4000_4000h;  										// Set even current address 
				ar3 = 4000_4001h;  										// Set odd current address 
				
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

// Next check (address = 4000_800(0/1)h)								
				ar2 = 4000_8000h;  										// Set even current address 
				ar3 = 4000_8001h;  										// Set odd current address 
				
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

// Next check (address = 4001_000(0/1)h)								
				ar2 = 4001_0000h;  										// Set even current address 
				ar3 = 4001_0001h;  										// Set odd current address 
				
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

// Next check (address = 4002_000(0/1)h)								
				ar2 = 4002_0000h;  										// Set even current address 
				ar3 = 4002_0001h;  										// Set odd current address 
				
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
				
				ar0 = 4000_0000h;			
				ar1 = 4000_0002h;
				ar2 = 4000_0004h;
				ar3 = 4000_0008h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							

				ar0 = 4000_0010h;			
				ar1 = 4000_0020h;
				ar2 = 4000_0040h;
				ar3 = 4000_0080h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 4000_0100h;			
				ar1 = 4000_0200h;
				ar2 = 4000_0400h;
				ar3 = 4000_0800h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 4000_1000h;			
				ar1 = 4000_2000h;
				ar2 = 4000_4000h;
				ar3 = 4000_8000h;				
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;							
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;
				
				ar0 = 4001_0000h;			
				ar1 = 4002_0000h;		
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;				
							
// **************************** The second half of test *****************************************

				ar0 = 4003_FFFEh;  										// Set start address 4003_FFFEh
				gr0 = 0FFFF_FFFFh;  									// Set const = -1				
				ar1 = 0F000_0000h;  						  		// Set const = F000_0000h
				gr1 = 0h;  								  					// Set const = 0h
				
// First check (address = 4003_FFF(E/F)h)				
				ar2 = 4003_FFFEh;  										// Set even start address 
				ar3 = 4003_FFFFh;  										// Set odd start address 
				
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
				
// Next check (address = 4003_FFF(C/D)h)								
				ar2 = 4003_FFFCh;  										// Set even current address 
				ar3 = 4003_FFFDh;  										// Set odd current address 
				
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

// Next check (address = 4003_FFF(A/B)h)								
				ar2 = 4003_FFFAh;  										// Set even current address 
				ar3 = 4003_FFFBh;  										// Set odd current address 
				
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

// Next check (address = 4003_FFF(6/7)h)								
				ar2 = 4003_FFF6h;  										// Set even current address 
				ar3 = 4003_FFF7h;  										// Set odd current address 
				
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
				
// Next check (address = 4003_FFE(E/F)h)								
				ar2 = 4003_FFEEh;  										// Set even current address 
				ar3 = 4003_FFEFh;  										// Set odd current address 
				
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
				
// Next check (address = 4003_FFD(E/F)h)								
				ar2 = 4003_FFDEh;  										// Set even current address 
				ar3 = 4003_FFDFh;  										// Set odd current address 
				
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
								
// Next check (address = 4003_FFB(E/F)h)								
				ar2 = 4003_FFBEh;  										// Set even current address 
				ar3 = 4003_FFBFh;  										// Set odd current address 
				
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

// Next check (address = 4003_FF7(E/F)h)								
				ar2 = 4003_FF7Eh;  										// Set even current address 
				ar3 = 4003_FF7Fh;  										// Set odd current address 
				
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

// Next check (address = 4003_FEF(E/F)h)								
				ar2 = 4003_FEFEh;  										// Set even current address 
				ar3 = 4003_FEFFh;  										// Set odd current address 
				
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

// Next check (address = 4003_FDF(E/F)h)								
				ar2 = 4003_FDFEh;  										// Set even current address 
				ar3 = 4003_FDFFh;  										// Set odd current address 
				
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
				
// Next check (address = 4003_FBF(E/F)h)								
				ar2 = 4003_FBFEh;  										// Set even current address 
				ar3 = 4003_FBFFh;  										// Set odd current address 
				
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

// Next check (address = 4003_F7F(E/F)h)								
				ar2 = 4003_F7FEh;  										// Set even current address 
				ar3 = 4003_F7FFh;  										// Set odd current address 
				
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
				
// Next check (address = 4003_EFF(E/F)h)								
				ar2 = 4003_EFFEh;  										// Set even current address 
				ar3 = 4003_EFFFh;  										// Set odd current address 
				
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
							
// Next check (address = 4003_DFF(E/F)h)								
				ar2 = 4003_DFFEh;  										// Set even current address 
				ar3 = 4003_DFFFh;  										// Set odd current address
				 
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
				
// Next check (address = 4003_BFF(E/F)h)								
				ar2 = 4003_BFFEh;  										// Set even current address 
				ar3 = 4003_BFFFh;  										// Set odd current address 
				
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

// Next check (address = 4003_7FF(E/F)h)								
				ar2 = 4003_7FFEh;  										// Set even current address 
				ar3 = 4003_7FFFh;  										// Set odd current address 
				
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

// Next check (address = 4002_FFF(E/F)h)								
				ar2 = 4002_FFFEh;  										// Set even current address 
				ar3 = 4002_FFFFh;  										// Set odd current address 
				
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

// Next check (address = 4001_FFF(E/F)h)								
				ar2 = 4001_FFFEh;  										// Set even current address 
				ar3 = 4001_FFFFh;  										// Set odd current address 
				
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
	
				ar1 = 4001_FFFEh;
				ar2 = 4002_FFFEh;
				ar3 = 4003_7FFEh;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							
												
				ar0 = 4003_BFFEh;			
				ar1 = 4003_DFFEh;
				ar2 = 4003_EFFEh;
				ar3 = 4003_F7FEh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;							

				ar0 = 4003_FBFEh;			
				ar1 = 4003_FDFEh;
				ar2 = 4003_FEFEh;
				ar3 = 4003_FF7Eh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 4003_FFBEh;			
				ar1 = 4003_FFDEh;
				ar2 = 4003_FFEEh;
				ar3 = 4003_FFF6h;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;				
				ar5,gr5 = [ar3];
				[ar6++] = ar5,gr5;		

				ar0 = 4003_FFFAh;			
				ar1 = 4003_FFFCh;
				ar2 = 4003_FFFEh;
				ar5,gr5 = [ar0];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar1];
				[ar6++] = ar5,gr5;
				ar5,gr5 = [ar2];
				[ar6++] = ar5,gr5;	

// compare the results with the originals 

	ar0 = test_result;
	ar1 = contr_result;
  gr0 = 216;

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
	      skip Finish;
<Error>
				rumboot_exit(0_16800h, 1);
				//gr1 = 01h;
	      //ar0 = STOP_SIMULATION;
	      //[ar0] = gr1;	                       // Set 01h into the location 1_6800h (IM3), if the test is incorrect
	  
<Finish>
		    halt;										               // Wait (test finished)
		    
end ".text.init";		

