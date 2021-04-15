// Test 8.2. Reduced Test 3.2 with Cache Enable 
// The test check the Giper Page Address changing  
// All instructions should be fetched from external memory, not from cache ! 

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                    // Address to write to stop simulation

const CSR_ADDRESS = 03_8C00h;  									     // Cache Control Regiser address
const CSR_SET_ADDRESS = 3_8C02h;  							     // Set bits for Cache Control Regiser address
const CSR_CLR_ADDRESS = 03_8C04h;  							     // Reset bits for Cache Control Regiser address
const PDA_ADDRESS = 03_8C08h;  									     // Cache GiperPage Register address
const CACHE_ENABLE = 001b;                           // Constant for CSR (cache enable) 
const CACHE_CLEAR = 100b;                            // Constant for CSR (cache clear) 
const CACHE_DISABLE = 000b;                          // Constant for CSR (cache disable) 

global test_8_2 : label;
   global start : label;
	  weak mytest : label;	
Program_fragment_1 : label;
Program_fragment_2 : label;	  
Normal, Error, Finish : label;
  
begin ".text.init"
// The instruction section should be placed with start address 0h (local memory bank 0)
<test_8_2>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);
 
   ar0 = 0h;
   [PDA_ADDRESS] = ar0;										 // Determine GiperPage Address register	
   ar0 = CACHE_ENABLE;
	 [CSR_ADDRESS] = ar0;                    //	Turn on the cache																		
		ar7,gr7 = 0h;													 // Set ar7 and gr7 to 0	
		ar6,gr6 = 0h;													 // Set ar6 and gr6 to 0		
		ar5,gr5 = 0h;													 // Set ar5 and gr5 to 0	
		ar4,gr4 = 0h;													 // Set ar4 and gr4 to 0		
		ar3,gr3 = 0h;													 // Set ar3 and gr3 to 0	
		ar2,gr2 = 0h;													 // Set ar2 and gr2 to 0		
		ar1,gr1 = 0h;													 // Set ar1 and gr1 to 0	
		ar0,gr0 = 0h;													 // Set ar0 and gr0 to 0		
		gr0 = 8;															 // Set gr0 to 8 (counter)
		gr2 = 3001_0002h;											 // Set gr2	
		gr6 = 2h;	

		ar0 = 0001_0010h;																			// Set address of test result dimension
		gr1 = 3001_00FEh;																			// Set initial address for jump
		gr5 = 0FEh;                                           // Set value for shift
		
		ar3,gr3 = [Program_fragment_1];											  // Load to register pair two first instructions of Program_fragment_1 block
		ar4,gr4 = [Program_fragment_1 + 2];										// Load to register pair two last instructions of Program_fragment_1 block
		
.repeat 8;																								// Copy Program_fragment block to addresses: 00FEh, 01FEh, 03FEh,
		[ar1=gr1] = ar3,gr3 with gr5 = gr5 A<<1;							// 07FEh, ... , 0_7FFEh
		                    with gr5 = gr5 or gr6;
		[ar1 += 2] = ar4,gr4 with gr1 = gr5 or gr2;
.endrepeat;

		gr1 = 3001_8002h;																		  // Set initial address for finish
		
		ar3,gr3 = [Program_fragment_2];											  // Load to register pair two first instructions of Program_fragment_2 block
		ar4,gr4 = [Program_fragment_2 + 2];										// Load to register pair two last instructions of Program_fragment_2 block
 
		[ar1=gr1] = ar3,gr3;
		[ar1 += 2] = ar4,gr4;
		
		gr1 = 3001_01FEh;																			// Set address for second jump
		gr5 = 1FEh;                                           // Set value for shift
				
		with gr0 = gr0 - 1;
		
nul 0h;																								    // Clear write buffers 
nul 0h;
nul 0h;
nul 0h;

      	pc = 3001_00FEh;																	// First jump to address in IM2
				[ar0++] = pc,pswr;																// First delayed instruction (pc -> Memory)
				[ar0++] = pc,pswr;																// Second delayed instruction (pc -> Memory)
				
				goto Error;

<Program_fragment_1>
		if <>0 delayed goto gr1 with gr5 = gr5 A<<1;					// Current jump
		[ar0++] = pc with gr5 = gr5 or gr6 noflags;          	// First delayed instruction (pc -> Memory)
		[ar0++] = pswr with gr0 = gr0 - 1;							      // Second delayed instruction (pc -> Memory)
		[ar0++] = gr0 with gr1 = gr5 or gr2 noflags;	        // Third delayed instruction (gr0 -> Memory)
		
<Program_fragment_2>		
		goto Normal;
		
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
