/* -*- rumboot-test-labels: broken -*- */

// Test 3.2. PC (Program Counter) Incrementor Check 
// The test should be started at addreess 0h (local internal memory)
// We make jump to address 00...010 bin, 00...0110 bin, 00...01110 bin, 00...011110 bin, ... , 001...10 bin.
// The delaeyd instruction for the jump is fetched from the address PC + 2. 
// Thus we check the incrementor of PC (almost all 31 bits, except for LSB, which always is zero) 

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

begin ".text.init"

global test_3_2 : label;
   global start : label;
	  weak mytest : label;

Program_fragment_1 : label;
Program_fragment_2 : label;

Normal, Error, Finish : label;	

<test_3_2>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);    		   																					 
		gr0 = 22;																						  // Set gr0 to 22 (counter)
		gr2 = 2h;																							// Set gr2 to 2
		
		gr3 = 0h;																							// Set gr3 to 0		
		gr4 = 0h;																							// Set gr4 to 0		
		gr5 = 0h;																							// Set gr5 to 0		
		gr6 = 0h;																							// Set gr6 to 0		
		gr7 = 0h;																							// Set gr7 to 0		
			
		ar0 = 0001_0010h;																			// Set address of test result dimension
		gr1 = 0000_00FEh;																			// Set initial address for jump
		
		ar3,gr3 = [Program_fragment_1];											  // Load to register pair two first instructions of Program_fragment_1 block
		ar4,gr4 = [Program_fragment_1 + 2];										// Load to register pair two last instructions of Program_fragment_1 block
		
.repeat 9;																								// Copy Program_fragment block to addresses: 00FEh, 01FEh, 03FEh,
		[ar1=gr1] = ar3,gr3 with gr1 = gr1 A<<1;							// 07FEh, ... , 0_FFFEh
		[ar1 += 2] = ar4,gr4 with gr1 = gr1 + gr2;
.endrepeat;

		gr1 = 1_7FFEh;																		  	// Set initial address for finish
		
		ar3,gr3 = [Program_fragment_2];											  // Load to register pair two first instructions of Program_fragment_2 block
		ar4,gr4 = [Program_fragment_2 + 2];										// Load to register pair two last instructions of Program_fragment_2 block
 
		[ar1=gr1] = ar3,gr3;
		[ar1 += 2] = ar4,gr4;
		
		gr1 = 0000_01FEh;																			// Set address for second jump
		with gr0 = gr0 - 1;
		
nul 0h;																								    // Clear write buffers 
nul 0h;
nul 0h;
nul 0h;

      	pc = 0000_00FEh;																	// First jump to address 8000_0002h
				[ar0++] = pc,pswr;																// First delayed instruction (pc -> Memory)
				[ar0++] = pc,pswr;																// Second delayed instruction (pc -> Memory)
				
				goto Error;

<Program_fragment_1>
		if <>0 delayed goto gr1 with gr1 = gr1 A<<1;					// Current jump
		[ar0++] = pc with gr1 = gr1 + gr2 noflags;          	// First delayed instruction (pc -> Memory)
		[ar0++] = pswr with gr0 = gr0 - 1;							      // Second delayed instruction (pc -> Memory)
		[ar0++] = gr0;	                                      // Third delayed instruction (gr0 -> Memory)
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
		    halt;										               // Wait (test finished)
end ".text.init";    