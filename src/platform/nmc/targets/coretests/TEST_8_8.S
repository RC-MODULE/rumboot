// Test for NMC4 instruction cache (1K*64) check
// When cache is enabled, all read/writes should be prohibited!
// The test checks up to 1K*64 bit of cache as follows:
// 1. For each address from 0000_0000h up to 0000_03FFh 
// 		write "000...0" to location.
// 2. Cache enable. CRDL = 0h, CRDH = 0h.   
// 3. For each address from 0000_0000h up to 0000_03FFh
//    write "111...1"b to location;
//		read  "000...0"b from CRDL, CRDH.
// 4. Cache disable. CRDL = "111...1"b, CRDH = "111...1"b. 
// 5. For each address from 0000_03FFh up to 0000_0000h
//		read  "000...0"b from location;
// *****************************************************************
// The check sum is stored in gr1 and allways has to be FFFF_FFFFh

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                   // Address to write to stop simulation

const CYCLE = 1 * 1024; 														// The size of memory to be checked (1K *64 bit) 
const START_ADDR = 0000_0000h;  								    // Start address of cache to be checked 
const FINISH_ADDRESS = 0000_03FFh; 								  // Last address of cache to be checked 

const CCSR_ADDRESS = 03_8C00h;  									 // Cache Control Regiser address
const CSR_SET_ADDRESS = 3_8C02h;  							 // set bits for Cache Control Regiser address
const CSR_CLR_ADDRESS = 03_8C04h;  							 // Reset bits for Cache Control Regiser address
const PDA_ADDRESS = 03_8C08h;  									 // Cache GiperPage Register address
const START_ADDRESS = 03_8C10h;  								 // Cache Start Register address (Start Memomory Operation)
const CPRC_ADDRESS = 03_8C12h;  									 // Cache Operation Register address (not increment/increment, read/write)
const CPAR_ADDRESS = 03_8C14h;  									 // Cache Address Register address
const CWDL_ADDRESS = 03_8C18h;  									 // Cache Write Data Register address (low 32-bit part of 64-bit word)
const CWDH_ADDRESS = 03_8C1Ah;  									 // Cache Write Data Register address (high 32-bit part of 64-bit word)
const CRDL_ADDRESS = 03_8C1Ch;  									 // Cache read Data Register address (low 32-bit part of 64-bit word)
const CRDH_ADDRESS = 03_8C1Eh;  									 // Cache read Data Register address (high 32-bit part of 64-bit word)

const CACHE_ENABLE = 001b;                           // Constant for CSR (cache enable) 
const CACHE_CLEAR = 100b;                            // Constant for CSR (cache clear) 
const CACHE_DISABLE = 000b;                          // Constant for CSR (cache disable) 
const PDA_WR_DISABLE = 010b; 												 // Constant for CSR (PDA hardware write disable) 
const READ_CACHE = 00b;												       // Constant for CPRC (read from cache)
const READ_CACHE_W_INC = 10b;									       // Constant for CPRC (read from cache with address autoincrement)
const WRITE_CACHE = 01b;												     // Constant for CPRC (write to cache)
const WRITE_CACHE_W_INC = 11b;											 // Constant for CPRC (write to cachewith address autoincrement)
const START_RW_CACHE = 1b;                           // Constant for START register (start read/write operation)
 
// The test result dimension
data data_test_res
test_result : word[32] = (-1 dup 32);
// The control value dimension
contr_result : word[32] = (
                           0h dup 2, 
                           0f0f0_0000h, 2h, 3fch, 0h,
                           1234_5678h, 5678_1234h, 0EDCB_A987h, 0A987_EDCBh,
                           0f00_0000h,  1h, 3h, 0h,
                           0EDCB_A987h, 0A987_EDCBh, 1234_5678h, 5678_1234h,
                           0h, 0ffff_ffffh,
                           0h, 0ffff_ffffh,
                           3ffh, 0ffff_ffffh, 
                           0f00_0000h, 0h, 3ffh, 0h, 0ffff_ffffh dup 2, 0h dup 2);
end data_test_res;

begin ".text.init"

				global test_1_1 : label;
				   global start : label;				
						weak mytest : label;

				LOOP11, LOOP12, LOOP13, LOOP14, LOOP15 : label;
				
				START_PROGRAM, LOOP21, LOOP22, LOOP23, LOOP24, LOOP25, LOOP26 : label;
                                                Normal, Error, Finish : label;
                                                           Loop_Check : label;
<test_8_7>
<start>
<mytest>		  
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

				gr0 = 0h;																	// Determine GP registers
				gr1 = gr0;
				gr2 = gr0;
				gr3 = gr0;
				gr4 = gr0;
				gr5 = gr0;
				gr6 = gr0;
				gr7 = gr0;

				ar7 = test_result;									  // Set result address					
				ar1 = [CCSR_ADDRESS];							
				gr1 = [CPRC_ADDRESS];
									
// Reading the cache registers after System Reset						
				[ar7++] = ar1,gr1;										       // Read CPRC, CCSR
// Check cache registers				
				gr1 = 0F0FF_FFFCh;
				[PDA_ADDRESS] = gr1;
				gr1 = 0FFFF_FFFEh;
				[CPRC_ADDRESS] = gr1;
				gr1 = 0FFFF_FFFCh;				
				[CPAR_ADDRESS] = gr1;
				gr1 = 1234_5678h;
				[CWDL_ADDRESS] = gr1;
				gr1 = 5678_1234h;
				[CWDH_ADDRESS] = gr1;				
				gr1 = 0EDCB_A987h;
				[CRDL_ADDRESS] = gr1;
				gr1 = 0A987_EDCBh;
				[CRDH_ADDRESS] = gr1;				
				 				 				
				ar1 = [PDA_ADDRESS];
				gr1 = [CPRC_ADDRESS];
				ar2 = [CPAR_ADDRESS];
				gr2 = [CCSR_ADDRESS];		
				ar3 = [CWDL_ADDRESS];
				gr3 = [CWDH_ADDRESS];
				ar4 = [CRDL_ADDRESS];
				gr4 = [CRDH_ADDRESS];
				
				[ar7++] = ar1,gr1;										       // Read CPRC, PDA
				[ar7++] = ar2,gr2;										       // Read CCSR, CPAR	
				[ar7++] = ar3,gr3;										       // Read CWDH, CWDL
				[ar7++] = ar4,gr4;										       // Read CRDH, CRDL		
									
				gr1 = 5678_1234h;
				[CRDH_ADDRESS] = gr1;	
				gr1 = 1234_5678h;
				[CRDL_ADDRESS] = gr1;			
				gr1 = 0A987_EDCBh;
				[CWDH_ADDRESS] = gr1;						
				gr1 = 0EDCB_A987h;
				[CWDL_ADDRESS] = gr1;			
				gr1 = 03h;				
				[CPAR_ADDRESS] = gr1;			
				gr1 = 0000_0001h;
				[CPRC_ADDRESS] = gr1;												
				gr1 = 0F00_0003h;
				[PDA_ADDRESS] = gr1;
		 				
				ar1 = [PDA_ADDRESS];
				gr1 = [CPRC_ADDRESS];
				ar2 = [CPAR_ADDRESS];
				gr2 = [CCSR_ADDRESS];		
				ar3 = [CWDL_ADDRESS];
				gr3 = [CWDH_ADDRESS];
				ar4 = [CRDL_ADDRESS];
				gr4 = [CRDH_ADDRESS];
				
				[ar7++] = ar1,gr1;										       // Read CPRC, PDA
				[ar7++] = ar2,gr2;										       // Read CCSR, CPAR	
				[ar7++] = ar3,gr3;										       // Read CWDH, CWDL
				[ar7++] = ar4,gr4;										       // Read CRDH, CRDL
						
        sir = START_ADDR;    						
				[CPAR_ADDRESS] = sir with gr1 = true; 			// Set start address and check sum
				sir = 0h;
				[CWDL_ADDRESS] = sir;												// set 0 to CWDL
				[CWDH_ADDRESS] = sir;												// set 0 to CWDH
				sir = WRITE_CACHE_W_INC;
				[CPRC_ADDRESS] = sir;                       // set write with autoincrement mode				
				gr0 = CYCLE;														    // Set cycle count for the first step
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				gr2 = START_RW_CACHE;
// The first step of test (write all ziroes to cache)				
<LOOP11>
if > delayed goto LOOP11 with gr0 = gr0 - 1;        // Go to LOOP11
				[START_ADDRESS] = gr2;                      // Delayed instruction - start read/write operation to cache 

// The first write of the check sum	
				ar1 = [CPAR_ADDRESS];
  		  [ar7++] = ar1, gr1;
  		  
// The second step of test	
				sir = 0h;
				[CRDL_ADDRESS] = sir;												// Clear CRDL
				[CRDH_ADDRESS] = sir;												// Clear CRDH	
				gr0 = CACHE_ENABLE;		
				[CCSR_ADDRESS] = gr0;                       // Cache enable!!!
				gr0 = [CCSR_ADDRESS];                       // Synchronization


        sir = START_ADDR;    		                    // Set start address and check sum						
				[CPAR_ADDRESS] = sir;
				sir = 0FFFF_FFFFh;
				[CWDL_ADDRESS] = sir;												// set all ones to CWDL
				[CWDH_ADDRESS] = sir;												// setall ones CWDH				
				gr0 = CYCLE;														    // Set cycle count for the second step		
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				
// The third step of test			
<LOOP12>

				sir = WRITE_CACHE;
				[CPRC_ADDRESS] = sir;                       // set write
				sir = START_RW_CACHE; 
				[START_ADDRESS] = sir;                      // start write operation to cache 						 				

				sir = READ_CACHE_W_INC;
				[CPRC_ADDRESS] = sir;                       // set read
				sir = START_RW_CACHE;
				[START_ADDRESS] = sir;                      // start read operation from cache 
				gr2 = [CRDL_ADDRESS];                       // read CRDL (should be all zeroes)
				gr3 = [CRDH_ADDRESS]                        // read CRDH (should be all zeroes)	
				with gr1 = gr1  and  not gr2 noflags;				// calculate check sum								
				with gr1 = gr1  and  not gr3 noflags;       // calculate check sum
				if >  goto LOOP12 with gr0 = gr0 - 1;				// Go to LOOP12

// The second write of the check sum	
				ar1 = [CPAR_ADDRESS];
  		  [ar7++] = ar1, gr1;	
  		  		
// The fourth step of test
				sir = 0FFFF_FFFFh;
				[CRDL_ADDRESS] = sir;												// Clear CRDL
				[CRDH_ADDRESS] = sir;												// Clear CRDH	
				gr0 = CACHE_DISABLE;		
				[CCSR_ADDRESS] = gr0;                       // Cache disable!!!
				gr0 = [CCSR_ADDRESS];                       // Synchronization						


				sir = FINISH_ADDRESS;
				[CPAR_ADDRESS] = sir;  				            // Set last address				
				ar0 = FINISH_ADDRESS - 1;  								// Set last address minus 1
				gr0 = CYCLE;													    // Set cycle count for the fourth step
				with gr0 = gr0 - 1; 											// Set flags for conditional jump
				
// The fifth step of test (read all ziroes and calculate check sum)		
<LOOP13>
				sir = READ_CACHE;
				[CPRC_ADDRESS] = sir;                       // set read
				sir = START_RW_CACHE;
				[START_ADDRESS] = sir;                      // start read operation from cache 
				gr2 = [CRDL_ADDRESS];                       // read CRDL (should be all zeroes)
				gr3 = [CRDH_ADDRESS];                       // read CRDH (should be all zeroes)	
				with gr1 = gr1  and  not gr2 noflags;       // calculate check sum
				with gr1 = gr1  and  not gr3 noflags;       // calculate check sum					
        [CPAR_ADDRESS] = ar0;                       // set new address
				if > delayed goto LOOP13 with gr0 = gr0 - 1;						// Go to LOOP13
				ar0 = ar0 - 1;										                      // Delayed instruction 1 (calculate check sum)
				nul;									                                  // Delayed instruction 2 (calculate check sum)			
				
// The third write of the check sum	
				ar1 = [CPAR_ADDRESS];
  		  [ar7++] = ar1, gr1;		
				with gr1 = gr1 + 1;													             // Should be all ones if the test is correct, plus 1
				
// Final part (reading of all registers)

				ar1 = [PDA_ADDRESS];
				gr1 = [CPRC_ADDRESS];
				ar2 = [CPAR_ADDRESS];
				gr2 = [CCSR_ADDRESS];		
				ar3 = [CWDL_ADDRESS];
				gr3 = [CWDH_ADDRESS];
				ar4 = [CRDL_ADDRESS];
				gr4 = [CRDH_ADDRESS];
				
				[ar7++] = ar1,gr1;										       // Read CPRC, PDA
				[ar7++] = ar2,gr2;										       // Read CCSR, CPAR	
				[ar7++] = ar3,gr3;										       // Read CWDH, CWDL
				[ar7++] = ar4,gr4;										       // Read CRDH, CRDL		

				if <>0 skip Error;
				
// Check the result    	
       ar0 = test_result;
	     ar1 = contr_result;
       gr0 = 32;

<Loop_Check>
	gr1 = [ar0++];
	gr2 = [ar1++];
	with gr2 - gr1;
	if <>0 skip Error;
	with gr0--;
	if <>0 skip Loop_Check;				
													
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
		    halt;										             // Wait (test finished)
		    
end ".text.init";
