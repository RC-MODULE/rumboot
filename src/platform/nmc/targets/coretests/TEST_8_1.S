/////////////////////////////////////////////////////////////////////////
//	Insctruction Cache Testing
/////////////////////////////////////////////////////////////////////////
//	Instruction block is executed twice, in the last case - from cash.
//	The size of the block is the size of the cache.
//	Every block checks some bits.
//	All blocks should check corectness of "0" and "1" store for every bit.
//	All bits exept 31-th are tested by control sum comutation, 
//	and the 31-th bit is tested by time of execution in clock cycles.
//
//	In comments you can determine the bits that are tested.

//	Legend:			. - bit is not tested; 
//				+ - bit has been tested;  
//				1 - bit, conteined "1", is tested;
//				0 - bit, conteined "0", is tested;
//				_ - used only to group bits for more clarity
//	P.S.:
//	gr7 store the difference between execution time for instruction blocks with null and vnull;
//	gr5 store the number of the last successefully executed stage.

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                    // Address to write to stop simulation

const M = 0AAAAAAAAh;
const W = 055555555h;
const C0 = 000000000h;
const C1 = 0FFFFFFFFh;

const TMC0_ADDRESS = 0003_8800h;  									 // Timer T0 address
const TMM0_ADDRESS = 0003_8802h;  									 // Timer T0 Control Register address
const START_TIMER0 = 01b;                            // Constant for TMM0 (start T0 in single-pass mode)
const STOP_TIMER0 =  00b;                            // Constant for TMM0 (stop T0) 
const T0_PERIOD = 0;                                 // Clear T0 value

const TMC1_ADDRESS = 0003_8804h;  									 // Timer T1 address
const TMM1_ADDRESS = 0003_8806h;  									 // Timer T1 Control Register address
const START_TIMER1 = 01b;                            // Constant for TMM1 (start T1 in single-pass mode)
const STOP_TIMER1 =  00b;                            // Constant for TMM1 (stop T1) 

const CSR_ADDRESS = 03_8C00h;  									     // Cache Control Regiser address
const CSR_SET_ADDRESS = 3_8C02h;  							     // Set bits for Cache Control Regiser address
const CSR_CLR_ADDRESS = 03_8C04h;  							     // Reset bits for Cache Control Regiser address
const PDA_ADDRESS = 03_8C08h;  									     // Cache GiperPage Register address
const CACHE_ENABLE = 001b;                           // Constant for CSR (cache enable) 
const CACHE_CLEAR =  100b;                           // Constant for CSR (cache clear) 
const CACHE_DISABLE = 000b;                          // Constant for CSR (cache disable) 

global test_8_1 : label;
   global start : label;
		weak mytest : label;
		cache1, cache2, cache3, cache4 : label;
						 Normal, Error, Finish : label;
		   	  
weak mytest_additional : label;

// This section should be placed with start address 0h (0-th internal memory bank)
begin ".text.init"
<test_8_1>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

  ar0 = 0FFFF_FFFFh;
  [PDA_ADDRESS] = ar0;										   // Determine GiperPage Address register	
	ar7 = 0001_7000h;													 // Determine system stack (IMU3)	

	gr0 = 0h;
	gr1 = 0h;
	gr2 = 0h;
	gr3 = 0h;								
	gr4 = 0h; 														
	gr5 = 0h;
	gr6 = 0h;
	gr7 = 0h;		
	
	//	Turn on the cache
	ar0 = CACHE_ENABLE;
	[CSR_ADDRESS] = ar0;
	ar0 = STOP_TIMER0;
	[TMM0_ADDRESS] = ar0;
  ar0 = STOP_TIMER1;
	[TMM1_ADDRESS] = ar0;
	
//	1

	//	Initialization
	ar2 = 0;
	gr0 = 3;
	gr7 = 3;
	pswr clear 10h;
	call in_cache1 + 3000_0000h;
	//	Execute the control sum
	gr0 = ar2 with gr1 = gr7 xor gr0; 
	gr1 = gr1 xor gr0; 

	//	Initialization
	ar2 = 0;
	gr0 = 3;
	gr7 = 3;
	pswr clear 10h;
	//	Now the same from cache
	call in_cache1 + 3000_0000h;
	//	Execute the control sum
	gr0 = ar2 with gr2 = gr7 xor gr0; 
	gr2 = gr2 xor gr0; 
	with gr2 - gr1;
	if <>0 goto Error;
		
	gr5 = 1;
//	2

	//	Initialization
	
	ar5 = 0;
	gr0 = 3;
	gr7 = 3;
	pswr clear 10h;
	call in_cache2 + 3000_0000h;
	//	Execute the control sum
	gr0 = ar5 with gr1 = gr7 xor gr0; 
	gr1 = gr1 xor gr0; 

	//	Initialization
	ar5 = 0;
	gr0 = 3;
	gr7 = 3;
	pswr clear 10h;
	//	Now the same from cache
	call in_cache2 + 3000_0000h;
	//	Execute the control sum
	gr0 = ar5 with gr2 = gr7 xor gr0; 
	gr2 = gr2 xor gr0; 
	with gr2 - gr1;
	if <>0 goto Error;
		
	gr5 = 2;
//	3

	//	Initialization
	
	gr0 = 0;
	gr3 = 1;
	ar1, gr1 = 0;
	call in_cache3 + 3000_0000h;
	
	//	Initialization
	gr3 = 1;
	ar1, gr1 = 0;
	//	Now the same from cache
	call in_cache3 + 3000_0000h with true;
	gr2 = gr3;
	//	Check the flag storage
	if =0 goto Error;
	with gr2 - gr3;
	if <>0 goto Error;
		
	gr5 = 3;
//	4

	//	Initialization
	
	gr0 = -1;
	gr3 = 1;
	gr7 = 1;
	ar1, gr1 = 0;
	[TMC0_ADDRESS] = gr1;
	ar0 = START_TIMER0;	
	[TMM0_ADDRESS] = ar0;
	ar0 = STOP_TIMER0;
	[TMM0_ADDRESS] = ar0;
	
	call in_cache4 + 3000_0000h;
	//	Execute the control sum
	gr2 = ar1;
	gr2 = gr2 + gr7;

	//	Initialization
	//	Now the same from cache
	gr3 = 1;
	gr7 = 1;
	ar1, gr1 = 0;
	call in_cache4 + 3000_0000h;
	gr4 = ar1;
	gr4 = gr4 + gr7;
	with gr2 - gr4;
	//	Execute the control sum
	if <>0 goto Error;
		
	gr5 = 4;
//	5,6

	//	31-th bit testing
	ar0 = 0h;
	[TMC1_ADDRESS] = ar0;
	ar0 = START_TIMER1;
	[TMM1_ADDRESS] = ar0;
	call in_cache8 + 3000_0000h;
	ar0 = STOP_TIMER1;
	[TMM1_ADDRESS] = ar0;	
	gr2 = [TMC1_ADDRESS];
	ar0 = 0h;
	[TMC1_ADDRESS] = ar0;
	ar0 = START_TIMER1;
	[TMM1_ADDRESS] = ar0;	
	call in_cache8 + 3000_0000h;	
	ar0 = STOP_TIMER1;
	[TMM1_ADDRESS] = ar0;		
	gr6 = [TMC1_ADDRESS];
	ar0 = 0h;
	[TMC1_ADDRESS] = ar0;	
	ar0 = START_TIMER1;
	[TMM1_ADDRESS] = ar0;	
	call in_cache9 + 3000_0000h;
	ar0 = STOP_TIMER1;
	[TMM1_ADDRESS] = ar0;		
	gr0 = [TMC1_ADDRESS];
	ar0 = 0h;
	[TMC1_ADDRESS] = ar0;		
	ar0 = START_TIMER1;
	[TMM1_ADDRESS] = ar0;	
	call in_cache9 + 3000_0000h;
	ar0 = STOP_TIMER1;
	[TMM1_ADDRESS] = ar0;				
	gr4 = [TMC1_ADDRESS];

	gr2 - gr6;
	if <= goto Error;
	
	gr0 - gr4;
	if <= goto Error;	
	
	//	It is supposed that the version with vnul is more fast (instructions are fetched from cache)	
	gr6 - gr4;
	if <= goto Error;

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
//	1
.repeat 843;																						// Align address (should be 800h!)
	nul 0h;
.endrepeat;

<in_cache1>
//	The bits are tested:
//	4i+0 :	.100_0110_1001_.010_.111_0011_1100_0000
//	4i+1 :	1010_1010_1010_1010_1010_1010_1010_1010
//	4i+2 :	.100_0110_1001_.010_.000_1100_0011_1111
//	4i+3 :	1010_1010_1010_1010_1010_1010_1010_1010
.repeat 505;
	ar2 = ar2 + M with gr0 = gr0 * gr7;
	ar2 = ar2 + M with gr7 = gr7 R>> 16;
.endrepeat;
return;

.repeat 12;																						// Align address
	nul 0h;
.endrepeat;
		
//	2

<in_cache2>
//	The bits are tested:
//	4i+0 :	.100_0111_0101_.101_.000_1100_0011_1111
//	4i+1 :	0101_0101_0101_0101_0101_0101_0101_0101
//	4i+2 :	.100_0111_0101_.101_.111_0011_1100_0000
//	4i+3 :	0101_0101_0101_0101_0101_0101_0101_0101
.repeat 505;
	ar5 = ar5 + W with gr7 = gr7 R>> 16;
	ar5 = ar5 + W with gr0 = gr0 * gr7;
.endrepeat;
return;

.repeat 12;																						// Align address
	nul 0h;
.endrepeat;

//	Now the bits are tested:
//	4i+0 :	.100_011+_++01_.+++_.+++_++++_++++_++++
//	4i+1 :	++++_++++_++++_++++_++++_++++_++++_++++
//	4i+2 :	.100_011+_++01_.+++_.+++_++++_++++_++++
//	4i+3 :	++++_++++_++++_++++_++++_++++_++++_++++

//	3

<in_cache3>
//	The bits are tested:
//	4i+0 :	..11_1..+_++.._.+++_0+++_++++_++++_++++
//	4i+1 :	++++_++++_++++_++++_++++_++++_++++_++++
//	4i+2 :	...._...+_++1._1+++_0+++_++++_++++_++++
//	4i+3 :	++++_++++_++++_++++_++++_++++_++++_++++
.repeat 252;
	[TMC0_ADDRESS] = gr3 with gr0 = gr0 + gr0 noflags;
	with gr3++ noflags;
	nul;
	gr1 = [TMC0_ADDRESS] with gr0 = gr0 + gr0 noflags;
	ar1 += gr1;
	nul;
.endrepeat;
return;

.repeat 14;																						// Align address
	nul 0h;
.endrepeat;

//	4

<in_cache4>
//	The bits are tested:
//	4i+0 :	...._...+_++1._1+++_1+++_++++_++++_++++
//	4i+1 :	++++_++++_++++_++++_++++_++++_++++_++++
//	4i+2 :	..11_1..+_++.._.+++_1+++_++++_++++_++++
//	4i+3 :	++++_++++_++++_++++_++++_++++_++++_++++
.repeat 252;
	gr1 = [TMC0_ADDRESS] with gr0 + 1;
	ar1 += gr1 with gr7 = gr7 + carry noflags;
	nul;
	[TMC0_ADDRESS] = gr3 with gr0 + 1;
	gr3 = gr7 with gr7 = gr7 + carry noflags;
	nul;
.endrepeat;
return;

.repeat 14;																						// Align address
	nul 0h;
.endrepeat;

//	Now the bits are tested:
//	4i+0 :	.1++_+11+_+++1_1+++_++++_++++_++++_++++
//	4i+1 :	++++_++++_++++_++++_++++_++++_++++_++++
//	4i+2 :	.1++_+11+_+++1_1+++_++++_++++_++++_++++
//	4i+3 :	++++_++++_++++_++++_++++_++++_++++_++++
	
//	5

<in_cache8>
//	The bits are tested:
//	4i+0 :	1.++_+..+_+++._.+++_++++_++++_++++_++++
//	4i+1 :	++++_++++_++++_++++_++++_++++_++++_++++
//	4i+2 :	1.++_+..+_+++._.+++_++++_++++_++++_++++
//	4i+3 :	++++_++++_++++_++++_++++_++++_++++_++++
.repeat 505;
	nul;
	nul;
	nul;
	nul;
.endrepeat;
return;

.repeat 12;																						// Align address
	nul 0h;
.endrepeat;
		
//	6

<in_cache9>
//	The bits are tested:
//	4i+0 :	00++_+00+_+++0_0+++_++++_++++_++++_++++
//	4i+1 :	++++_++++_++++_++++_++++_++++_++++_++++
//	4i+2 :	00++_+00+_+++0_0+++_++++_++++_++++_++++
//	4i+3 :	++++_++++_++++_++++_++++_++++_++++_++++
.repeat 1010;
        vnul;
				vnul;
.endrepeat;
return;

.repeat 12;																						// Align address
	nul 0h;
.endrepeat;

//	Now the bits are tested:
//	4i+0 :	++++_++++_++++_++++_++++_++++_++++_++++
//	4i+1 :	++++_++++_++++_++++_++++_++++_++++_++++
//	4i+2 :	++++_++++_++++_++++_++++_++++_++++_++++
//	4i+3 :	++++_++++_++++_++++_++++_++++_++++_++++

end ".text.init";
