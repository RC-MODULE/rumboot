////////////////////////////////////
//	All Eight Sequensers of Insctruction Cache Testing
////////////////////////////////////
//	Instruction block is executed three times:
//  at the first time  instructions are fetched from memory,
//  at the second time - partialy from cache and from memory,
//  at the last time - only from cache.

//	gr5 store the number of the last successefully executed stage.

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                    // Address to write to stop simulation

const M = 0AAAAAAAAh;
const W = 055555555h;

const CSR_ADDRESS = 03_8C00h;  									     // Cache Control Regiser address
const CSR_SET_ADDRESS = 3_8C02h;  							     // Set bits for Cache Control Regiser address
const CSR_CLR_ADDRESS = 03_8C04h;  							     // Reset bits for Cache Control Regiser address
const PDA_ADDRESS = 03_8C08h;  									     // Cache GiperPage Register address
const CACHE_ENABLE = 001b;                           // Constant for CSR (cache enable) 
const CACHE_CLEAR = 100b;                            // Constant for CSR (cache clear) 
const CACHE_DISABLE = 000b;                          // Constant for CSR (cache disable) 
const PDA_WR_DISABLE = 010b;

const EMI_BASE = 4000_0000h;                            // Start address of external memory (IMU3)

global test_8_4 : label;
   global start : label;
		weak mytest : label;
	Loop, cache1, cache2 : label;
 Normal, Error, Finish : label;
		   	  
				weak mytest_additional : label;
M2, M3, M4, M5, M6, M7, M8, M9 : label;
L1, L2, L3, L4, L5, L6, L7, L8 : label;

// This section should be placed with start address 0h (internal memory bank 0)
begin ".text.init"
<test_8_4>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);
// Determine GiperPage Address register

  ar0 = 02FF0_0000h;
  [PDA_ADDRESS] = ar0;												          	
  	
	gr1 = 0h;
	gr2 = 0h;
	gr3 = 0h;								
	gr4 = 0h; 														
	gr5 = 0h;
	gr6 = 0h;
	gr7 = 0h;		
	ar7 = 0_8000h;                              //  set system stack on IMU2            


 // Write program to external memory 
        ar0 = mytest_additional;
        ar3 = EMI_BASE;
        gr0 = 1024;
<Loop>
	      ar1, gr1 = [ar0++];
        ar2, gr2 = [ar0++];
        [ar3++] = ar1, gr1;
        [ar3++] = ar2, gr2; 
	      with gr0--;
	      if <>0 goto Loop;
	      
	//	Turn on the cache
	ar0 = CACHE_ENABLE;
  [CSR_SET_ADDRESS] = ar0;   
   
//	First half part of the test (sequential filling of the the sequensers)

	//	Initialization	
	ar2 = 0;
	gr0 = 3;
	gr7 = 3;
	pswr clear 10h;
	// The first time of instruction block execution (fetch half of instructions from memory + load half of the sequensers)
	call EMI_BASE with gr5 = gr5 + 1;
	
	//	Initialization	
	ar2 = 0;
	gr0 = 3;
	gr7 = 3;
	pswr clear 10h;
	// The second time of instruction block execution (fetch other half of instructions from memory + fill the sequensers)
	call EMI_BASE with gr5 = gr5 + 1;	
	//	Execute the control sum
	gr0 = ar2 with gr1 = gr7 xor gr0; 
	gr1 = gr1 xor gr0; 

	//	Initialization
	ar2 = 0;
	gr0 = 3;
	gr7 = 3;
	pswr clear 10h;
	// The third time of instruction block execution (fetch all instructions from cahe)
	call EMI_BASE with gr5 = gr5 + 1;
	//	Execute the control sum
	gr0 = ar2 with gr2 = gr7 xor gr0; 
	gr2 = gr2 xor gr0; 
	with gr2 - gr1;
	if <>0 goto Error;
	
//	Last half part of the test (reverce order of sequensers filling)	
	
	gr5 = 0;

	//	Initialization
	ar5 = 0;
	gr0 = 3;
	gr7 = 3;
	pswr clear 10h;
	// The first time of instruction block execution (fetch half of instructions from memory + load half of the sequensers)
	call (EMI_BASE + 800h) with gr5 = gr5 + 1;
	
	//	Initialization
	ar5 = 0;
	gr0 = 3;
	gr7 = 3;
	pswr clear 10h;
	// The second time of instruction block execution (fetch other half of instructions from memory + fill the sequensers)
	call (EMI_BASE + 800h) with gr5 = gr5 + 1;
	//	Execute the control sum
	gr0 = ar5 with gr1 = gr7 xor gr0; 
	gr1 = gr1 xor gr0; 

	//	Initialization
	ar5 = 0;
	gr0 = 3;
	gr7 = 3;
	pswr clear 10h;
	//	The third time of instruction block execution (fetch all instructions from cahe)
	call (EMI_BASE + 800h) with gr5 = gr5 + 1;
	//	Execute the control sum
	gr0 = ar5 with gr2 = gr7 xor gr0; 
	gr2 = gr2 xor gr0; 
	with gr2 - gr1;
	if <>0 goto Error;
		
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
		    
<mytest_additional>	

//	Sequential filling of all eight sequensers (first pass - load first half of each sequenser, second pass - load remained half of each sequenser)
<in_cache1>
// Fill the 0-th sequenser
.repeat 32;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;
	nul;
	with gr5 - 1;
	if =0 skip M2;
	nul;
	nul;
.repeat 30;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;

// Fill the 1-th sequenser
<M2>
.repeat 32;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;
	nul;
	with gr5 - 1;
	if =0 skip M3;
	nul;
	nul;
.repeat 30;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;

// Fill the 2-th sequenser
<M3>
.repeat 32;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;
	nul;
	with gr5 - 1;
	if =0 skip M4;
	nul;
	nul;
.repeat 30;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;

// Fill the 3-th sequenser
<M4>
.repeat 32;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;
	nul;
	with gr5 - 1;
	if =0 skip M5;
	nul;
	nul;
.repeat 30;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;

// Fill the 4-th sequenser
<M5>
.repeat 32;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;
	nul;
	with gr5 - 1;
	if =0 skip M6;
	nul;
	nul;
.repeat 30;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;

// Fill the 5-th sequenser
<M6>
.repeat 32;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;
	nul;
	with gr5 - 1;
	if =0 skip M7;
	nul;
	nul;
.repeat 30;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;

// Fill the 6-th sequenser
<M7>
.repeat 32;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;
	nul;
	with gr5 - 1;
	if =0 skip M8;
	nul;
	nul;
.repeat 30;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;

// Fill the 7-th sequenser at a time (because of tail of the cache the 7-th sequenser should not  be full)
<M8>
.repeat 56;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;

<M9>
	return;

.repeat 14;
	nul 0h;
.endrepeat;

// Reverce order of sequensers filling (first pass - load second half of each sequenser, second pass - load remained half of each sequenser)	
<in_cache2>
// Fill the 0-th sequenser
	nul;
	with gr5 - 1;
	if =0 skip L1;
	nul;
	nul;
.repeat 30;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

<L1>
.repeat 32;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

// Fill the 1-th sequenser
	nul;
	with gr5 - 1;
	if =0 skip L2;
	nul;
	nul;
.repeat 30;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

<L2>
.repeat 32;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

// Fill the 2-th sequenser
	nul;
	with gr5 - 1;
	if =0 skip L3;
	nul;
	nul;
.repeat 30;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

<L3>
.repeat 32;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

// Fill the 3-th sequenser
	nul;
	with gr5 - 1;
	if =0 skip L4;
	nul;
	nul;
.repeat 30;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

<L4>
.repeat 32;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

// Fill the 4-th sequenser
	nul;
	with gr5 - 1;
	if =0 skip L5;
	nul;
	nul;
.repeat 30;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

<L5>
.repeat 32;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

// Fill the 5-th sequenser
	nul;
	with gr5 - 1;
	if =0 skip L6;
	nul;
	nul;
.repeat 30;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

<L6>
.repeat 32;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

// Fill the 6-th sequenser
	nul;
	with gr5 - 1;
	if =0 skip L7;
	nul;
	nul;
.repeat 30;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

<L7>
.repeat 32;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

// Fill the 7-th sequenser at a time (because of tail of the cache the 7-th sequenser should not  be full)
	
.repeat 56;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;

	return;
	
.repeat 14;
	nul 0h;
.endrepeat;

end ".text.init";

