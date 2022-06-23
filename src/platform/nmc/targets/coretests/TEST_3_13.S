// Test 3.13. Halt instruction execution
// The test should be executed only after system reset !!!
//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                    // Address to write to stop simulation
const TMC0_ADDRESS = 3_8800h;  									 		 // Timer T0 address
const TMM0_ADDRESS = 3_8802h;  									 		 // Timer T0 Control Register address
const START_TIMER0 = 0101b;                          // Constant for TMM0 (start T0 in single-pass mode)
const STOP_TIMER0 = 0100b;                           // Constant for TMM0 (stop T0) 
const T0_PERIOD = 0;                                 // Clear T0 value

const TMC1_ADDRESS = 3_8804h;  									     // Timer T1 address
const TMM1_ADDRESS = 3_8806h;  									     // Timer T1 Control Register address
const START_TIMER1 = 0111b;                          // Constant for TMM1 (start T1 in multi-pass mode)
const STOP_TIMER1 = 0100b;                           // Constant for TMM1 (stop T1) 
const T1_PERIOD = -40;                               //  T1 = -40

const IRRL_ADDRESS = 3_8000h;  									     // External Interrupt 31-0 Request Register address
const IRRL_RESET_ADDRESS = 3_8004h;  						     // External Interrupt 31-0 Request Register address for Bit Reset
const IMRL_ADDRESS = 3_8008h;  									     // External Interrupt 31-0 Mask Register address
const IMRL_SET_ADDRESS = 3_800Ah;  							     // External Interrupt 31-0 Mask Register address for Bit set
const IMRL_RESET_ADDRESS = 3_800Ch;  						     // External Interrupt 31-0 Mask Register address for Bit Reset

data data_test
// The test result dimension 

test_result : word[104] = ( -1 dup 104);
// The control value dimension 

contr_result : word[104] = (
                            0h, 0h, 15ah, 1040h, 0300_0400h, 1d6h, 0300_0400h, 100bh, 0fffffff5h dup 2,
    												0h, 1h, 15eh, 1040h, 0300_0400h, 1d6h, 0300_0400h, 100bh, 0fffffff5h dup 2,
    												0h, 2h, 162h, 1040h, 0300_0400h, 1d6h, 0300_0400h, 100bh, 0fffffff5h dup 2,
    												0h, 3h, 166h, 1040h, 0300_0400h, 1d6h, 0300_0400h, 100bh, 0fffffff5h dup 2,    
    												0h, 4h, 16ah, 1040h, 0300_0400h, 1d6h, 0300_0400h, 100bh, 0fffffff5h dup 2,
    												0h, 5h, 16eh, 1040h, 0300_0400h, 1d6h, 0300_0400h, 100bh, 0fffffff5h dup 2,
    												0h, 6h, 172h, 1040h, 0300_0400h, 1d6h, 0300_0400h, 100bh, 0fffffff5h dup 2,
    												0h, 7h, 176h, 1040h, 0300_0400h, 1d6h, 0300_0400h, 100bh, 0fffffff5h dup 2,
    												0h, 8h, 17ah, 1040h, 0300_0400h, 1d6h, 0300_0400h, 100bh, 0fffffff5h dup 2,
    												0h, 9h, 17eh, 1040h, 0300_0400h, 1d6h, 0300_0400h, 100bh, 0fffffff5h dup 2,
    												0300_0400h, 0ah, 0ffff_ffd8h, 0000_01c3h);

save_stack : long = 0l;

end data_test;

		 global test_3_13 : label;
		     global start : label;
					weak mytest : label;
Normal, Error, Finish : label;

M1, M2, M3, M4, M5, M6, M7, M8, M9, M10 : label;
M11, M12, M13, M14, M15, M16 : label;

NMI, OV, EI, ST : label;
EX_INT0, EX_INT1, EX_INT2, EX_INT3, EX_INT4, EX_INT5, EX_INT6, EX_INT7, EX_INT8, EX_INT9 : label;
EX_INT10, EX_INT11, EX_INT12, EX_INT13, EX_INT14, EX_INT15 : label;
START_PROGRAM, START_NMI, START_OV, START_EI, START_ST, START_T0, START_T1 : label;

// The boot loader and interrupt handlers	(placed in memory with start address 0h)						 
begin ".text.init"

<test_3_13>
<start>
<mytest>

// Non Mask Interrupt (NMI) Handler (Address = 0h)
<NMI>
							if <>0 delayed goto START_BOOT;								// Go to boot program after system reset (not NMI handler)						
							nul;																					// First delayed instruction
							nul;																					// Second delayed instruction
							goto Error;								                  	

// Overflow (OV) Handler (Address = 8h)
<OV>
							delayed goto START_OV;												// Go to OV handler
							[ar0++] = pc, pswr;										 				// First delayed instruction - write result to memory (pswr  || pc = START_OV)
							[ar0++] = intr;																// Second delayed instruction - write result to memory (intr)
							nul;
							nul;
							nul;
							nul;																					// Align address

// Error Instruction (EI) Handler (Address = 10h)
<EI>
							delayed goto START_EI;												// Go to EI handler
							[ar0++] = pc, pswr;										 				// First delayed instruction - write result to memory (pswr  || pc = START_EI)
							[ar0++] = intr;																// Second delayed instruction - write result to memory (intr)
							nul;
							nul;
							nul;
							nul;																					// Align address
							
// Step-by-step Interrupt (ST) Handler (Address = 18h)
<ST>
							delayed goto START_ST;												// Go to ST handler
							[ar0++] = pc, pswr;										 				// First delayed instruction - write result to memory (pswr  || pc = START_ST)
							[ar0++] = intr;																// Second delayed instruction - write result to memory (intr)
							nul;
							nul;
							nul;
							nul;																					// Align address

// External Interrupt#0 Handler (Address = 20h) - NOT USED HERE
<EX_INT0>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#1 Handler (Address = 28h) - NOT USED HERE
<EX_INT1>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#2 Handler (Address = 30h) - NOT USED HERE
<EX_INT2>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#3 Handler (Address = 38h) - NOT USED HERE
<EX_INT3>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#4 Handler (Address = 40h) - NOT USED HERE
<EX_INT4>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#5 Handler (Address = 48h) - NOT USED HERE
<EX_INT5>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#6 Handler (Address = 50h) - NOT USED HERE
<EX_INT6>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#7 Handler (Address = 58h) - NOT USED HERE
<EX_INT7>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#8 Handler (Address = 60h) - NOT USED HERE
<EX_INT8>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#9 Handler (Address = 68h) - NOT USED HERE
<EX_INT9>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#10 Handler (Address = 70h) - Timer T0 handler
<EX_INT10>
							delayed goto START_T0;												// Go to T0 handler
							[ar0++] = pc;										 				      // First delayed instruction - write result to memory (pswr  || pc = START_T1)
							[ar0++] = intr;																// Second delayed instruction - write result to memory (intr)
							nul;																					// Align address
							nul;
							nul;
							nul;			

// External Interrupt#4 Handler (Address = 78h) - Timer T1 handler
<EX_INT11>
							delayed goto START_T1;												// Go to T1 handler
							[ar0++] = pc;										 				      // First delayed instruction - write result to memory (pswr  || pc = START_T0)
							[ar0++] = intr;																// Second delayed instruction - write result to memory (intr)
							nul;																					// Align address
							nul;
							nul;
							nul;																												
	
// External Interrupt#12 Handler (Address = 80h) - NOT USED HERE
<EX_INT12>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#13 Handler (Address = 88h) - NOT USED HERE
<EX_INT13>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#14 Handler (Address = 90h) - NOT USED HERE
<EX_INT14>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#15 Handler (Address = 98h) - NOT USED HERE
<EX_INT15>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#16 Handler (Address = A0h) - NOT USED HERE
<EX_INT16>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#17 Handler (Address = A8h) - NOT USED HERE
<EX_INT17>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#18 Handler (Address = B0h) - NOT USED HERE
<EX_INT18>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#19 Handler (Address = B1h) - NOT USED HERE
<EX_INT19>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#20 Handler (Address = C0h) - NOT USED HERE
<EX_INT20>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#21 Handler (Address = C8h) - - Timer T1 handler
<EX_INT21>
							delayed goto START_T1;												// Go to T1 handler
							[ar0++] = pc;										 				      // First delayed instruction - write result to memory (pswr  || pc = START_T1)
							[ar0++] = intr;																// Second delayed instruction - write result to memory (intr)
							nul;																					// Align address
							nul;
							nul;
							nul;		

// External Interrupt#22 Handler (Address = D0h) - NOT USED HERE
<EX_INT22>

							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#23 Handler (Address = D8h) - NOT USED HERE
<EX_INT23>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;	

// External Interrupt#24 Handler (Address = E0h) - NOT USED HERE
<EX_INT24>							
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#25 Handler (Address = E8h) - NOT USED HERE
<EX_INT25>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#26 Handler (Address = F0h) - NOT USED HERE
<EX_INT26>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#27 Handler (Address = F8h) - NOT USED HERE
<EX_INT27>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		
		
// Boot program body
<START_BOOT>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);				  
							ar0 = test_result;													// Set address for test results
							ar7 = save_stack;													  // Set address of system stack
							gr2 = 0;                                    // Reset counter

// Start T0 and T1 timers
              gr0 = T0_PERIOD;
              [TMC0_ADDRESS] = gr0;                         // Determine T0 (all zeroes)
              gr1 = T1_PERIOD;             
              [TMC1_ADDRESS] = gr1;                         // Determine T1 (-40)   
              gr0 = START_TIMER0;           
							[TMM0_ADDRESS] = gr0;                         // Start T0
              gr1 = START_TIMER1; 							
							[TMM1_ADDRESS] = gr1;                         // Start T1
              gr0 = 0_0000_0000_0000_1100_0000_0000b;
              [IMRL_SET_ADDRESS] = gr0;                     // Enable T0 and T1 interrupts in IMRL					
              pswr set 1040h;																// Enalbe internal interrupts (SSH enable) 
              
.repeat 10;
.align;              
              delayed  halt;             
              [ar0++] = ar2,gr2 with gr2 = gr2 + 1;
              [ar0++] = pswr, pc;
              [ar0++] = intr;         
.endrepeat;	
		
              pswr clear 40h;	                               // Disable interrupts (SSH enable)
              gr0 = STOP_TIMER0;
              [TMM0_ADDRESS] = gr0;                          // Stop T0
              [TMM1_ADDRESS] = gr0;                          // Stop T1
						  [ar0++] = intr;				
						  [ar0++] = gr2;
						  gr2 = [TMC0_ADDRESS];
						  ar2 = [TMC1_ADDRESS];
						  [ar0++] = ar2, gr2;
// The program of the given and control dimension comparison 
	ar0 = test_result;
	ar1 = contr_result;
	gr0 = 104;

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

// NMI handler body
<START_NMI>	
              goto Error;			
//						pswr = 100Bh;
//						[ar0++] = pswr;
//						ar2, gr2 = 0FFFF_FFF0h;
//						[ar0++] = ar2, gr2;
//						ireturn;

// OV handler body
<START_OV>
              goto Error;
//						pswr = 100Bh;
//						[ar0++] = pswr;
//						ar3, gr3 = 0FFFF_FFF1h;
//						[ar0++] = ar3, gr3;
//						ireturn;
						
// EI handler body
<START_EI>
              goto Error;
//						pswr = 100Bh;
//						[ar0++] = pswr;
//						ar4, gr4 = 0FFFF_FFF2h;
//						[ar0++] = ar4, gr4;
//						ireturn;
						
// ST handler body
<START_ST>
              goto Error;
//						pswr = 100Bh;
//						[ar0++] = pswr;
//						ar5, gr5 = 0FFFF_FFF3h;
//						[ar0++] = ar5, gr5;
//						ireturn;
						
// T0 handler body
<START_T0>
              goto Error;
//						pswr = 100Bh;
//						[ar0++] = pswr;
//						ar6, gr6 = 0FFFF_FFF4h;
//						[ar0++] = ar6, gr6;
//						ireturn;

<START_T1>
						pswr = 100Bh;
						[ar0++] = pswr;
						ar6, gr6 = 0FFFF_FFF5h;
						[ar0++] = ar6, gr6;
						ireturn;

end ".text.init";
