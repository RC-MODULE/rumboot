// Test 3.10. Core Interrupt Testing with EN_SSH = 1 (Hardware Stack Top Enable)
// Masck (PSWR) Counter (4 bits) Testing
// The boot loader and interrupt handlers should be placed with start address 0h (top of internal memory) !!!
// The test should be executed only after system reset !!!
//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                   // Address to write to stop simulation

const STACK_ADDRESS = 00001_0000h; 									 // Top address of system stack (IMU3)

const TMC0_ADDRESS = 0003_8800h;  									// Timer T0 address
const TMM0_ADDRESS = 0003_8802h;  									// Timer T0 Control Register address
const TMC1_ADDRESS = 0003_8804h;  									// Timer T1 address
const TMM1_ADDRESS = 0003_8806h;  									// Timer T1 Control Register address

const START_TIMER0 = 01b;                           // Constant for TMM0 (start T0 in single-pass mode)
const STOP_TIMER0 = 0b;                             // Constant for TMM0 (stop T0) 
const START_TIMER1 = 01b;                           // Constant for TMM1 (start T1 in single-pass mode)
const STOP_TIMER1 = 0b;                             // Constant for TMM1 (stop T1) 

const IRRL_ADDRESS = 0003_8000h;  									// External Interrupt 31-0 Request Register address  
const IRRL_SET_ADDRESS = 0003_8002h;  						  // External Interrupt 31-0 Request Register address for Bit set  
const IRRL_RESET_ADDRESS = 0003_8004h;  						// External Interrupt 31-0 Request Register address for Bit Reset  
const IMRL_ADDRESS = 0003_8008h;  									// External Interrupt 31-0 Mask Register address  
const IMRL_SET_ADDRESS = 0003_800Ah;  							// External Interrupt 31-0 Mask Register address for Bit set  
const IMRL_RESET_ADDRESS = 0003_800Ch;  						// External Interrupt 31-0 Mask Register address for Bit Reset  

data data_test1
// The test result dimension 
test_result_1 : long[28] = ( -1l dup 28);
// The control value dimension 
contr_result_1 : long[28] = (                 
                 00000000_0000015Chl,  00000000_03000400hl,
                 03000400_03000500hl,  03000580_03000500hl, 
                 03000580_03000500hl,  03000580_03000500hl,
                 03000580_03000500hl,  03000580_03000500hl,
                 03000580_03000500hl,  03000580_03000500hl,
                 03000580_03000500hl,  03000580_03000500hl,
                 03000580_03000500hl,  03000580_03000500hl,
                 03000580_03000500hl,  03000580_030005cahl, 
                 03000580_030005cbhl,  00000005_030005cahl, 
                 00000001_000003f2hl,  00000060_0000050ahl,
                 00000000_00000000hl, 0fffffff1_fffffff1hl,
                0fffffff2_fffffff2hl, 0fffffff3_fffffff3hl,
                0fffffff5_fffffff5hl,  7fffffff_00010000hl,
                 00001004_0000033ahl,  00000000_03000400hl
           ) ;
end data_test1;

data data_test2
// The test result dimension 
test_result_2 : long[102] = ( -1l dup 102);
// The control value dimension 
contr_result_2: long[102] = (
 000011e4_0000039chl,  0000100b_030004cahl,  0fffffff1_fffffff1hl,
 000011e4_000003a6hl,  0000100b_0300044ahl,  0fffffff2_fffffff2hl, 
 000011e4_000003bahl,  0000100b_0300044bhl,  0fffffff4_fffffff4hl,
 000011e4_000003c4hl,  0000100b_03000400hl,  0fffffff5_fffffff5hl,
(000011e4_000003b0hl,  0000100b_03000400hl,  0fffffff3_fffffff3hl) dup 30
           ) ;
end data_test2;

		 global test_3_10 : label;
         global start : label;
					weak mytest : label;
	
weak mytest_additional: label;	
Normal, Error, Finish : label;

M1, M2, M3, M4, M5, M6, M7, M8, M9, M10 : label;
           M11, M12, M13, M14, M15, M16 : label;
           M17, M18, M19, M20, M21, M22 : label;
           M23, M24, M25, M26, M27, M28 : label;
                               M29, M30 : label;
                           Loop1, Loop2 : label;
NMI, OV, EI, ST : label;
EX_INT0, EX_INT1, EX_INT2, EX_INT3, EX_INT4, EX_INT5, EX_INT6, EX_INT7, EX_INT8, EX_INT9 : label;
EX_INT10, EX_INT11, EX_INT12, EX_INT13, EX_INT14, EX_INT15 : label;
EX_INT16, EX_INT17, EX_INT18, EX_INT19, EX_INT20, EX_INT21, EX_INT22, EX_INT23 : label;
START_PROGRAM, START_NMI, START_OV, START_EI, START_ST, START_T0, START_T1 : label;

// The boot loader and interrupt handlers	(placed in memory with start address 0h)						 
begin ".text.init"

<test_3_10>
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
							[ar1++] = pc, pswr;										 				// First delayed instruction - write result to memory (pswr  || pc = START_OV)
							[ar1++] = intr;																// Second delayed instruction - write result to memory (intr)
							nul;
							nul;
							nul;
							nul;																					// Align address

// Error Instruction (EI) Handler (Address = 10h)
<EI>
							delayed goto START_EI;												// Go to EI handler
							[ar1++] = pc, pswr;										 				// First delayed instruction - write result to memory (pswr  || pc = START_EI)
							[ar1++] = intr;																// Second delayed instruction - write result to memory (intr)
							nul;
							nul;
							nul;
							nul;																					// Align address
							
// Step-by-step Interrupt (ST) Handler (Address = 18h)
<ST>
							delayed goto START_ST;												// Go to ST handler
							[ar1++] = pc, pswr;										 				// First delayed instruction - write result to memory (pswr  || pc = START_ST)
							[ar1++] = intr;																// Second delayed instruction - write result to memory (intr)
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
							[ar1++] = pc, pswr;										 				// First delayed instruction - write result to memory (pswr  || pc = START_T1)
							[ar1++] = intr;																// Second delayed instruction - write result to memory (intr)
							nul;																					// Align address
							nul;
							nul;
							nul;								
							
							
// External Interrupt#11 Handler (Address = 78h) - Timer T1 handler
<EX_INT11>
							delayed goto START_T1;												// Go to T1 handler
							[ar1++] = pc, pswr;										 				// First delayed instruction - write result to memory (pswr  || pc = START_T0)
							[ar1++] = intr;																// Second delayed instruction - write result to memory (intr)
							nul;
							nul;
							nul;
							nul;																					// Align address							
	
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

// External Interrupt#21 Handler (Address = C8h) - Timer T1 handler
<EX_INT21>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		
						
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

// External Interrupt#28 Handler (Address = 100h) - NOT USED HERE
<EX_INT28>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		
		
// External Interrupt#29 Handler (Address = 108h) - NOT USED HERE
<EX_INT29>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		

// External Interrupt#30 Handler (Address = 110h) - NOT USED HERE
<EX_INT30>
							goto Error;
.repeat 4;
							nul;																					// Align address
.endrepeat;		
										
// Boot program body
<START_BOOT>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

							pc = mytest_additional;								// Jump to external memory 
							nul;
							nul;

<mytest_additional>		

							ar0 = test_result_1;													// Set address for test results
							ar7 = STACK_ADDRESS;													// Set address of system stack
							[ar0++] = pc,pswr;														// Write result to memory (pswr = 0h || pc = START_PROGRAM)
							[ar0++] = intr;																// Write result to memory (intr = 0h)
							[ar0++] = ar1;																// Write result to memory (ar1)

// Check conditions to set overflow interrupt request 

							gr7 = 7FFF_FFFFh;															// Write max positive value to gr7
							gr0 = 8000_0000h;															// Write max negative value to gr0
							with true;																		// Set N = 1, Z = V = C = 0
							with gr7 + gr7;																// Set N = 1, Z = 0, V = 1, C = 0
							
              if vfalse goto Error;                         // Clear the instruction pipeline

							[ar0++] = intr; 															// Read intr = 100h (OV inrerrupt reguest)
							intr clear 100h;															// Reset OV inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 0h (no OV inrerrupt reguest)	
							with false;																		// Set N = 0, Z = 1, V = 0, C = 0
							with gr0 = gr0 A<<1;													// Set N = 1, Z = 0, V = 1, C = 0
							
              if vfalse goto Error; 												// Clear the instruction pipeline

							[ar0++] = intr; 															// Read intr = 100h (OV inrerrupt reguest), not reset the request 

// Check conditions to set error instruction interrupt request
// A. Vector error instructions 

//						rep 32 with data;													   	// 1. Usage of data from memory in Operation Unit, but there is no read operation from memory
              code 0003_F518h;	
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)
							
							rep 32 [ar0++] = afifo;												// 2. Read from empty AFIFO
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)								

							rep 16 with vtrue;														// Write all ones (16*64 bits)to AFIFO
							rep 1 with vfalse;														// 3. Write to not empty AFIFO
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)	
							
							rep 8 ram = [ar1++];													// Write to RAM 8 long words
							rep 16 with ram + afifo;											// 4. Usage RAM and AFIFO as operands with different size of data (RAM - 8*64 bits, AFIFO - 16*64)
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)	
							
							rep 32 data = [ar1++] with data - ram;				// 5. Usage data from memory and RAM as operands with different size of data (data - 32*64 bits, RAM - 8*64)
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)	
							
							rep 15 data = [ar1++] with data and afifo;		// 6. Usage data from memory and AFIFO as operands with different size of data (data - 15*64 bits, AFIFO - 16*64)
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)	
							
// B. Scalar error instructions
																		
//						fp0_flags = 0h;																// 1. Move constant to vector register (FPU)
              code 0C0000000h;
              code 00000000h;
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)	
														
//						vr = 0h;																			// 2. Move constant to vector register (VU)
              code 49000000h;
              code 00000000h;
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)	
								
//  					[ar0++] = vr;																	// 3. Write vector register to memory	
		      		code 3E24_0000h;	
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)	
									
//  					f1cr = gr0;															    	// 4.	Move RGrisc to RGvu	
              code 7890_0000h;						
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)				

//            gr0  = f2cr;															    // 5.	Move RGvu	to RGrisc		
							code 7023_0000h;
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest)
							intr clear 80h;																// Reset EI inrerrupt reguest
							[ar0++] = intr; 															// Read intr = 100h (only OV inrerrupt reguest)			

              sir = 3h;							
							sb = sir;
							sir = 0h;																			// Set sb register
							nb1 = sir;																			// Set nb register
							rep 2 wfifo = [ar1++], ftw;										// Write to WFIFO, load shadow matrix from WFIFO
							sb = sir;																			// 6. Changing of sb register between ftw and wtw operations
							[ar0++] = intr; 															// Read intr = 180h (OV inrerrupt reguest, EI inrerrupt reguest), not reset the requests 

// Set/reset T0 reguest
              gr0 = 0100_0000_0000b;
              [IMRL_ADDRESS] = gr0;                         // Enable T0 interrupt in IMRL
              gr0 = 0FFFF_FFFFh;
              [TMC0_ADDRESS] = gr0;                         // Determine T0 (all ones)
              gr0 = START_TIMER0;
							[TMM0_ADDRESS] = gr0;                         // Start T0

.repeat 16;																									// Wait 16 clocks
      				nul 0h;
.endrepeat;													

							[ar0++] = intr; 															// Read intr = 1CAh (OV inrerrupt reguest, EI inrerrupt reguest, T0 interrupt reguest)											
							
							intr clear 40h;																// Reset T0 inrerrupt reguest
							nul 0h;
							[ar0++] = intr; 															// Read intr = 180h (only OV and EI inrerrupt reguests)									

// Set/reset T1 reguest
              gr0 = 1000_0000_0000b;
              [IMRL_SET_ADDRESS] = gr0;                     // Enable T1 interrupt in IMRL
              gr0 = 0FFFF_FFFFh;
              [TMC1_ADDRESS] = gr0;                         // Determine T1 (all ones)
              gr0 = START_TIMER1;
							[TMM1_ADDRESS] = gr0;                         // Start T11

.repeat 16;																									// Wait 16 clocks
      				nul 0h;
.endrepeat;													

							[ar0++] = intr; 															// Read intr = 1D1h (OV inrerrupt reguest, EI inrerrupt reguest, T1 interrupt reguest)											
							
							intr clear 40h;																// Reset T1 inrerrupt reguest
							nul 0h;
							[ar0++] = intr; 															// Read intr = 180h (only OV and EI inrerrupt reguests)		

// set T0 and T1 reguests
              gr0 = 0FFFF_FFFFh;
              [TMC0_ADDRESS] = gr0;                         // Determine T0 (all ones)
              [TMC1_ADDRESS] = gr0;                         // Determine T1 (all ones)   
              gr0 = START_TIMER0;           
							[TMM0_ADDRESS] = gr0;                         // Start T0
							[TMM1_ADDRESS] = gr0;                         // Start T1
							
.repeat 16;																									// Wait 16 clocks
        			nul 0h;
.endrepeat;		

							[ar0++] = intr; 															// Read intr = 1C4h (OV inrerrupt reguest, EI inrerrupt reguest, T0 interrupt reguest), not reset the requests 	
              gr1 = [IRRL_ADDRESS];	
              with gr1 = gr1 >>12;
              [ar0++] = pswr;                               // read PSWR (T1 interrupt reguest)		

.repeat 12;
						with gr0 = gr0 noflags;									  // Clear the instruction queue 												
.endrepeat;	  

              ar1 = test_result_2;												
.repeat 10;																									// Check Mask counter (all interrupts are disable, excluding NMI, until this block is finished)
      				pswr = 11E4h;																	// Enalbe all interrupts, set Z glag and EN bit, reset Y, N, V and C flags
.endrepeat;
														
							pswr clear 1E0h;															// Disalbe all interrupts												 
.repeat 10;																									// Wait 10 clocks
        			nul;
.endrepeat;										
							pswr set 1E0h;																// Enalbe all interrupts
										
							
// The program that we will be interrupt				
							gr1 = 0h;																			// Write 0 to gr1
							
							if =0 delayed goto M1 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 +1 noflags;										// Second delayed instruction
							
<M1>					if <>0 goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 +1 noflags;										// Second delayed instruction

							if u< delayed goto M2 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M2>					if u>= goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if vfalse delayed goto M3 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M3>					if vtrue goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if >= delayed goto M4 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M4>					if < goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if =0 delayed goto M5 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
							
<M5>					if <>0 goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if u< delayed goto M6 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M6>					if u>= goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if vfalse delayed goto M7 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M7>					if vtrue goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if >= delayed goto M8 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M8>					if < goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if =0 delayed goto M9 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 +1 noflags;										// Second delayed instruction
							
<M9>					if <>0 goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if u< delayed goto M10 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M10>					if u>= goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if vfalse delayed goto M11 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M11>					if vtrue goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if >= delayed goto M12 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M12>					if < goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if =0 delayed goto M13 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
							
<M13>					if <>0 goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if u< delayed goto M14 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M14>					if u>= goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if vfalse delayed goto M15 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M15>					if vtrue goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction

							if >= delayed goto M16 with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
<M16>					if < goto Error with gr1 = gr1 + 1 noflags;
							with gr1 = gr1 + 1 noflags;										// First delayed instruction
							with gr1 = gr1 + 1 noflags;										// Second delayed instruction
							
							pswr clear 1E0h;															// Disalbe all interrupts	
						  [ar0++] = ar0, gr0;
						  [ar0++] = ar1, gr1;
						  [ar0++] = ar2, gr2;						
						  [ar0++] = ar3, gr3;						
						  [ar0++] = ar4, gr4;						
						  [ar0++] = ar5, gr5;						
						  [ar0++] = ar6, gr6;												
						  [ar0++] = ar7, gr7;
						  [ar0++] = pc, pswr;
						  [ar0++] = intr;	
						  gr0 = [IRRL_ADDRESS];
							with gr0 = gr0 >>11;
							if carry goto Error;                					// If timer 0 interrupt, go to Error
							with gr0 = gr0 >>1;
							if carry goto Error;                					// If timer 1 interrupt, go to Error									  
						  [ar0++] = gr0;			
						  
// The program of the given and control dimension comparison (1)
	ar0 = test_result_1;
	ar1 = contr_result_1;
	gr0 = 56;

	<Loop1>
	  gr1 = [ ar0++ ];
	  gr2 = [ ar1++ ];
	  with gr2 - gr1;
	  if <>0 goto Error;
	  with gr0--;
	  if <>0 goto Loop1;
	  
// The program of the given and control dimension comparison (2)
	ar0 = test_result_2;
	ar1 = contr_result_2;
	gr0 = 204;

	<Loop2>
	  gr1 = [ ar0++ ];
	  gr2 = [ ar1++ ];
	  with gr2 - gr1;
	  if <>0 goto Error;
	  with gr0--;
	  if <>0 goto Loop2;	  
	  						  		
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
						pswr = 100Bh;
						[ar1++] = pswr;
						ar2, gr2 = 0FFFF_FFF0h;
						[ar1++] = ar2, gr2;
						ireturn;

// OV handler body
<START_OV>
						pswr = 100Bh;
						[ar1++] = pswr;
						ar3, gr3 = 0FFFF_FFF1h;
						[ar1++] = ar3, gr3;
						ireturn;
						
// EI handler body
<START_EI>
						pswr = 100Bh;
						[ar1++] = pswr;
						ar4, gr4 = 0FFFF_FFF2h;
						[ar1++] = ar4, gr4;
						ireturn;
						
// ST handler body
<START_ST>
						pswr = 100Bh;
						[ar1++] = pswr;
						ar5, gr5 = 0FFFF_FFF3h;
						[ar1++] = ar5, gr5;
						ireturn;
						
// T0 handler body
<START_T0>
						pswr = 100Bh;
						[ar1++] = pswr;
						ar6, gr6 = 0FFFF_FFF4h;
						[ar1++] = ar6, gr6;
						ireturn;

<START_T1>
						pswr = 100Bh;
						[ar1++] = pswr;
						ar6, gr6 = 0FFFF_FFF5h;
						[ar1++] = ar6, gr6;
						ireturn;

end ".text.init";
