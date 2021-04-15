// Test 8.3. The test is modification of Test 3.2. with additional work of instruction cache.
// The test should be started at addreess 0h (internal memory bank 0).
// The test is multipass:
// 0-th pass: set the GiperPage Address register (GPA=FFFx_xxxxh) by program, disable GPA modification, enable cache, 
// run (instruction fetch from internal/external memory, load cache)
// 1-th pass: not change the GPA, run (instruction fetch from internal/external memory)
// 2-th pass: clear the cache, run (instruction fetch from internal/external memory, not from the cache)
// 3-th pass: rewrite GPA (clear the cache), run (instruction fetch from internal/external memory, not from the cache, load the cache)
// 4-th pass: repeat 1-th pass
// 5-th pass: stop the test
// GR5 store the pass number minus 1  

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
const PDA_WR_DISABLE = 010b;

global test_8_3 : label;
   global start : label;
	  weak mytest : label;
	  
				Program_fragment : label;	  
	    				All_passes : label;
	   						Pass_345 : label;
Start_3_pass, Start_pass : label;
					        Finish : label;
Program_fragment_1 : label;
Program_fragment_2 : label;	  			        
					
begin ".text.init"
<test_8_3>
<start>
<mytest>
// 0-th pass	
		nul 0h;																								// 64-bit nop 
   	if carry goto All_passes;    
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

		ar2, gr2 = 0h;																				// Clear ar2,gr2 		
		ar6, gr6 = 0h;																				// Clear ar5,gr5		
		ar7, gr7 = 0h;																				// Clear ar7,gr7 		

		gr0 = 8;															                // Set gr0 to 8 (counter)
		gr2 = 3001_0002h;															        // Set gr2 		
		gr6 = 2h;	
		
		ar0 = 0001_0010h;																			// Set address of test result dimension
		gr1 = 3001_00FEh;																			// Set initial address for jump
		gr7 = 0FEh;                                           // Set value for shift
				
		ar3,gr3 = [Program_fragment_1];											  // Load to register pair two first instructions of Program_fragment_1 block
		ar4,gr4 = [Program_fragment_1 + 2];										// Load to register pair two last instructions of Program_fragment_1 block
		
.repeat 8;																								// Copy Program_fragment block to addresses: 00FEh, 01FEh, 03FEh,
		[ar1=gr1] = ar3,gr3 with gr7 = gr7 A<<1;							// 07FEh, ... , 1FFF_FFFEh
		                    with gr7 = gr7 or gr6;		
		[ar1 += 2] = ar4,gr4 with gr1 = gr7 or gr2;
.endrepeat;

		gr1 = 3001_8002h;																			// Set initial address for jump
		
		ar3,gr3 = [Program_fragment_2];											  // Load to register pair two first instructions of Program_fragment_2 block
		ar4,gr4 = [Program_fragment_2 + 2];										// Load to register pair two last instructions of Program_fragment_2 block
 
		[ar1=gr1] = ar3,gr3;
		[ar1 += 2] = ar4,gr4;
				
nul 0h;																								    // Clear write buffers 
nul 0h;
nul 0h;
nul 0h;

//************************************************************	    
		    ar5, gr5 = 0h;																				// Clear ar5, gr5  - pass number
        ar0 = 1FF0_0000h;
        [PDA_ADDRESS] = ar0;												          // Determine GiperPage Address register			
        ar0 = PDA_WR_DISABLE;		                             
        [CSR_SET_ADDRESS] = ar0;                              // Disable GiperPage Address register	hardware modification  
	      ar0 = CACHE_ENABLE;
        [CSR_SET_ADDRESS] = ar0;                              // Enable cache	  

<All_passes>																				 			// Common part of all passes
		    with gr4 = gr5 - 1;																		// Set flags to determine the pass number
		    if <= goto Start_pass;																// If the pass number is 0 or 1 - go to Start_pass
		
		    with gr4 = gr4 - 1;																		// Set flags to determine the pass number
		    if > goto Pass_345;																		// If the pass number is 3, 4 or 5 - go to Pass_345
        ar0 = CACHE_CLEAR;                                    // Pass 2 (clear the cache, go to Start_pass)
        [CSR_SET_ADDRESS] = ar0;		
        [CSR_CLR_ADDRESS] = ar0;																				 
		    goto Start_pass;																			
		
<Pass_345>
		    with gr4 = gr4 - 1;																		// Set flags to determine the pass number
		    if <=  goto Start_3_pass;															// If the pass number is 3 - go to Start_pass	with	 
		
		    with gr4 = gr4 - 1;																		// Set flags to determine the pass number
		    if <= goto Start_pass;																// If the pass number is 4 - go to Start_pass

<Finish>																									    // 5-th pass
				rumboot_exit(0_16800h, 0);
//	    ar0 = STOP_SIMULATION with gr1 = false;
//	    [ar0] = gr1;												                   // Set 00h into the location 1_6800h (IM3), if the test is correct
	      halt;										                               // Wait (test finished)
		
<Start_3_pass>		
        ar0 = 1FFF_FFFFh;                                       // Start pass 3 (rewrite GiperPage Address, i.e. clear the cache)	
        [PDA_ADDRESS] = ar0;																
		
<Start_pass>																							      // Start passes 0 - 2, 4, continue pass 3
		    gr1 = 3001_01FEh with gr5 = gr5 + 1;									  // Set address for second jump
		    gr0 = 8 ;
		    gr7 = 1FEh;															                  // Set gr0 to 22 (counter)
		    with gr0 = gr0 - 1;
.repeat 12;																		// The WBUF clearning
				nul 0h;
.endrepeat;
								
				pc = 3001_00FEh;						          // Jump to external memory 
				vnul;																	// Delayed instruction 1
				vnul;																	// Delayed instruction 2	

<Program_fragment_1>
		if <>0 delayed goto gr1 with gr7 = gr7 A<<1;					// Current jump
		with gr7 = gr7 + gr6 noflags;          	  // First delayed instruction 
		with gr0 = gr0 - 1;							          // Second delayed instruction 
		with gr1 = gr7 or gr2 noflags;	          // Third delayed instruction 
<Program_fragment_2>		
    delayed goto 0h; 
		pswr = 1h;    

end ".text.init";
		                   													