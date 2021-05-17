/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Page Priority Unit (Cache) Testing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	1. Fetch 8 instruction blocks from memory + open 8 pages in cache + load cache.
//  2. Fethch 9-th block (replace 1-th block)from memory + load cache, go to 2-th block, 3-th block, ... 9-th block (are fetched from cache).
//  3. Fethch 10-th block (replace 2-th block)from memory + load cache, go to 3-th block, 4-th block, ... 10-th block (are fetched from cache).
//  					 ...
//  8. Fethch 16-th block (replace 8-th block)from memory + load cache, go to 9-th block, 10-th block, ... 15-th block (are fetched from cache).
//
//	gr0 store the number of the successefully executed instruction blocks.
//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                    // Address to write to stop simulation

const EMI_BASE = 4000_0000h;                         // Start address of external memory

const CSR_ADDRESS = 03_8C00h;  									     // Cache Control Regiser address
const CSR_SET_ADDRESS = 3_8C02h;  							     // Set bits for Cache Control Regiser address
const CSR_CLR_ADDRESS = 03_8C04h;  							     // Reset bits for Cache Control Regiser address
const PDA_ADDRESS = 03_8C08h;  									     // Cache GiperPage Register address
const CACHE_ENABLE = 001b;                           // Constant for CSR (cache enable) 
const CACHE_CLEAR = 100b;                            // Constant for CSR (cache clear) 
const CACHE_DISABLE = 000b;                          // Constant for CSR (cache disable) 
const PDA_WR_DISABLE = 010b;

global test_8_6 : label;
   global start : label;
		weak mytest : label;
 Normal, Error, Finish : label;

 M1, M2, M3, M4, M5, M6, M7, M8, M9 : label;
	M10, M11, M12, M13, M14, M15, M16 : label;

// This section should be placed with start address 0h (internal memory bank 0)
begin ".text.init"
<test_8_6>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);
 
  ar0 = 0FFFF_FFFFh;
  [PDA_ADDRESS] = ar0;									// Determine GiperPage Address register	

	ar7 = 0001_0000h;											// Determine system stack (int. memory bank 3)		
	
	gr0 = 0h;
	gr1 = 0h;
	gr2 = 0h;
	gr3 = 0h;								
	gr4 = 0h; 														
	gr5 = 0h;
	gr6 = 0h;
	gr7 = 0h;		
	
// Write program to external memory 
        ar0 = M1;
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
	
	// Go to external memory 
	call EMI_BASE;
	gr1 = 80;
	with gr0 - gr1;
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
		    halt;										              // Wait (test finished)
		    
// 1-th instruction block
<M1>
						with gr0 = gr0 + 1;
						nul;
						nul;
						nul;
						skip M2;
.repeat 124;
						nul;
						nul;
.endrepeat;						
						
// 2-th instruction block
<M2>
						with gr0 = gr0 + 1;
						nul;
						nul;
						nul;
						skip M3;
.repeat 124;
						nul;
						nul;
.endrepeat;								
						
// 3-th instruction block
<M3>
						with gr0 = gr0 + 1;
						nul;
						nul;
						nul;
						skip M4;
.repeat 124;
						nul;
						nul;
.endrepeat;								
						
// 4-th instruction block
<M4>
						with gr0 = gr0 + 1;
						nul;
						nul;
						nul;
						skip M5;
.repeat 124;
						nul;
						nul;
.endrepeat;								
						
// 5-th instruction block
<M5>
						with gr0 = gr0 + 1;
						nul;
						nul;
						nul;
						skip M6;
.repeat 124;
						nul;
						nul;
.endrepeat;											
						
// 6-th instruction block
<M6>
						with gr0 = gr0 + 1;
						nul;
						nul;
						nul;
						skip M7;
.repeat 124;
						nul;
						nul;
.endrepeat;								
						
// 7-th instruction block
<M7>
						with gr0 = gr0 + 1;
						nul;
						nul;
						nul;
						skip M8;
.repeat 124;
						nul;
						nul;
.endrepeat;					

// 8-th instruction block
<M8>
						with gr0 = gr0 + 1;
						nul;
						nul;
						nul;
						skip M9;
.repeat 124;
						nul;
						nul;
.endrepeat;								
						
// 9-th instruction block
<M9>
						gr1 = 9 with gr0 = gr0 + 1;
						with gr1 - gr0;
						nul;
						if =0 skip M2;
						skip M10;
.repeat 122;
						nul;
						nul;
.endrepeat;					

// 10-th instruction block
<M10>
						gr1 = 18 with gr0 = gr0 + 1;
						with gr1 - gr0;
						nul;
						if =0 skip M3;
						skip M11;
.repeat 122;
						nul;
						nul;
.endrepeat;			

// 11-th instruction block
<M11>
						gr1 = 27 with gr0 = gr0 + 1;
						with gr1 - gr0;
						nul;
						if =0 skip M4;
						skip M12;
.repeat 122;
						nul;
						nul;
.endrepeat;			

// 12-th instruction block
<M12>
						gr1 = 36 with gr0 = gr0 + 1;
						with gr1 - gr0;
						nul;
						if =0 skip M5;
						skip M13;
.repeat 122;
						nul;
						nul;
.endrepeat;			

// 13-th instruction block
<M13>
						gr1 = 45 with gr0 = gr0 + 1;
						with gr1 - gr0;
						nul;
						if =0 skip M6;
						skip M14;
.repeat 122;
						nul;
						nul;
.endrepeat;			

// 14-th instruction block
<M14>
						gr1 = 54 with gr0 = gr0 + 1;
						with gr1 - gr0;
						nul;
						if =0 skip M7;
						skip M15;
.repeat 122;
						nul;
						nul;
.endrepeat;			

// 15-th instruction block
<M15>
						gr1 = 63 with gr0 = gr0 + 1;
						with gr1 - gr0;
						nul;
						if =0 skip M8;
						skip M16;
.repeat 122;
						nul;
						nul;
.endrepeat;			

//16-th instruction block
<M16>
						gr1 = 72 with gr0 = gr0 + 1;
						with gr1 - gr0;
						nul;
						if =0 skip M9;
						return;
.repeat 122;
						nul;
						nul;
.endrepeat;			

end ".text.init";
