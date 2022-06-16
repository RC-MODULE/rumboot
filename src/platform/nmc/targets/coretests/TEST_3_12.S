// Test 3.12. Instruction Queue and Source Buffer (9 * 32 bits each) Testing
// Adress Register Counters and General Purpose Register Counters (4 bits each) Testing
// NZVC Counter and EN Bit Counter (4 bits each) Testing
// The result of the test you can see only on shared memory

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation


data data_test
// The test result dimension 

test_result : long[34] = ( -1l dup 34);

// The control value dimension 
contr_res : long[34] = (
         0000fff7_00000366hl,
         0000fff6_0000fff1hl,
         0000fff5_0000fff2hl,
         0000fff4_0000fff3hl,
         0000fff3_0000fff4hl,  
         0000fff2_0000fff5hl,
         0000fff1_0000fff6hl,
         0000fff0_0000fff7hl, 
        0ffff000f_00000376hl,
        0ffff000e_ffff0008hl,
        0ffff000d_ffff0009hl,         
        0ffff000c_ffff000ahl,         
        0ffff000b_ffff000bhl,         
        0ffff000a_ffff000chl,         
        0ffff0009_ffff000dhl,         
        0ffff0008_ffff000ehl,                                       
         000000aa_000000achl,
        0FFFF00DE_000000d0hl, 
         00000030_00000030hl,
        0FFFF000C_00000040hl,
         0000013a_0000013chl,         
        0FFFF0168_0000015ehl,         
         00000070_00000070hl,         
        0FFFF0008_00000080hl,
         00000FF0_00000FF0hl,
         00000FF1_00000FF1hl,
         00000FF2_00000FF2hl,
         00000FF3_00000FF3hl,
         0000FF0A_0000FF0Ahl,
         0000FF0B_0000FF0Bhl,
         0000FF0B_0000FF0Bhl,
         0000FF0C_0000FF0Chl, 
         0000001f_000002a8hl,
         03000400_03000500hl	
           ) ;
end data_test;

const STACK_ADDRESS =  00000_B100h; 									// Top address of system stack

global test_3_12 : label;
    global start : label;
	   weak mytest : label;
	
Normal, Error, Finish : label;

M1, M3, M4: label;

R2, R4, R5 : label;
 
// The boot loader								 
begin ".text.init"
<test_3_12>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

// 		Instruction queue and source buffer testing (try to write to all triggers of the queue and the buffer 0 and 1) 			
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;																						
            ar0 = 0000_FFF0h set;										// Fill the queue and the source buffer 
      			ar1 = 0000_FFF1h set;										// Write constants to address registers
 			      ar2 = 0000_FFF2h set;										
 		        ar3 = 0000_FFF3h set;
 	        	ar4 = 0000_FFF4h set;
 		        ar5 = 0000_FFF5h set;
 		        ar6 = 0000_FFF6h set;
          	ar7 = 0000_FFF7h set;

      			gr0 = ar7;															// Move ari to gri
      			gr1 = ar6;
        		gr2 = ar5;
          	gr3 = ar4;
      			gr4 = ar3;
      			gr5 = ar2;
      			gr6 = ar1;
       			gr7 = ar0;

         		ar0 = test_result set;								// Set address of test result
        		[ar0++] = ar0, gr0;											// Wtrite result to memory
        		[ar0++] = ar1, gr1;
        		[ar0++] = ar2, gr2;
        		[ar0++] = ar3, gr3;
        		[ar0++] = ar4, gr4;
        		[ar0++] = ar5, gr5;
        		[ar0++] = ar6, gr6;
         		[ar0++] = ar7, gr7;
                   
      			gr0 = 0FFFF_000Fh set;									// Write constants to GP registers
      			gr1 = 0FFFF_000Eh set;									
      			gr2 = 0FFFF_000Dh set;									
      			gr3 = 0FFFF_000Ch set;
      			gr4 = 0FFFF_000Bh set;
      			gr5 = 0FFFF_000Ah set;
      			gr6 = 0FFFF_0009h set;
        		gr7 = 0FFFF_0008h set;

      			ar1 = gr7 addr;									  			// Move gri to ari
      			ar2 = gr6 addr;
      			ar3 = gr5 addr;
      			ar4 = gr4 addr;
      			ar5 = gr3 addr;
      			ar6 = gr2 addr;
      			ar7 = gr1 addr;

          	[ar0++] = ar0, gr0;											// Wtrite result to memory
        		[ar0++] = ar1, gr1;
        		[ar0++] = ar2, gr2;
        		[ar0++] = ar3, gr3;
        		[ar0++] = ar4, gr4;
        		[ar0++] = ar5, gr5;
        		[ar0++] = ar6, gr6;
        		[ar0++] = ar7, gr7;

// Address register 0 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;	
        		ar7 = ar0 addr;													// Save ar0 (address of test result)
        																						// Fill the queue and increment the ar0 counter
       			ar0 = 1h set;														
						ar0 = 2h set;														
						ar0 = 3h set;
						ar0 = 4h set;
						ar0 = 5h set;
						ar0 = 6h set;
						ar0 = 7h set;
						ar0 = 8h set;
						ar0 = 9h set;
						ar0 = 10h set;
						
         		ar0 = pc + 0h;														
						[ar7++] = pc;														// Wtrite result to memory (pc)
						[ar7++] = ar0;													// Wtrite result to memory (ar0 = pc - 2)

						ar0 = ar7 addr;													// Restore ar0

// Address register 1 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;	    	
       			ar1 = 11h set;														// Stop the queue until the vectot instruction will be executed (up to 32 clock cycles)
						ar1 = 12h set;														// Fill the queue and increment the ar1 counter
						ar1 = 13h set;
						ar1 = 14h set;
						ar1 = 15h set;
						ar1 = 16h set;
						ar1 = 17h set;
						ar1 = 18h set;
						ar1 = 19h set;
						ar1 = 20h set;
						
         		ar1 = pc + gr1;														
						[ar0++] = pc;														// Wtrite result to memory (pc)
						[ar0++] = ar1;													// Wtrite result to memory (ar1 = pc + FFFF_000Eh - 2)

// Address register 2 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;	                 	
      			ar2 = 21h set;														// Stop the queue until the vectot instruction will be executed (up to 32 clock cycles)
						ar2 = 22h set;														// Fill the queue and increment the ar2 counter
						ar2 = 23h set;
						ar2 = 24h set;
						ar2 = 25h set;
						ar2 = 26h set;
						ar2 = 27h set;
						ar2 = 28h set;
						ar2 = 29h set;
						ar2 = 30h set;
						
        		ar1 = ar2 addr;														
						[ar0++] = ar2;													// Wtrite result to memory (ar2 = 30h)
						[ar0++] = ar1;													// Wtrite result to memory (ar1 = 30h)

// Address register 3 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;	       			
      			ar3 = 31h set;														// Stop the queue until the vectot instruction will be executed (up to 32 clock cycles)
						ar3 = 32h set;														// Fill the queue and increment the ar3 counter
						ar3 = 33h set;
						ar3 = 34h set;
						ar3 = 35h set;
						ar3 = 36h set;
						ar3 = 37h set;
						ar3 = 38h set;
						ar3 = 39h set;
						ar3 = 40h set;
						
        	 [ar0++] = ar3, gr3;												// Wtrite result to memory (gr3 = FFFF_000Ch || ar3 = 40h)

// Address register 4 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;	      			
       			ar4 = 41h set;														// Stop the queue until the vectot instruction will be executed (up to 32 clock cycles)
						ar4 = 42h set;														// Fill the queue and increment the ar4 counter
						ar4 = 43h set;
						ar4 = 44h set;
						ar4 = 45h set;
						ar4 = 46h set;
						ar4 = 47h set;
						ar4 = 48h set;
						ar4 = 49h set;
						ar4 = 50h set;
						
        		ar4 = pc + 0h;														
						[ar0++] = pc;														 // Wtrite result to memory (pc)
						[ar0++] = ar4;													 // Wtrite result to memory (ar4 = pc - 2)

// Address register 5 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;	                      	
      			ar5 = 51h set;														
						ar5 = 52h set;														// Fill the queue and increment the ar5 counter
						ar5 = 53h set;
						ar5 = 54h set;
						ar5 = 55h set;
						ar5 = 56h set;
						ar5 = 57h set;
						ar5 = 58h set;
						ar5 = 59h set;
						ar5 = 60h set;
						
         		ar5 = pc + gr5;														
						[ar0++] = pc;														 // Wtrite result to memory (pc)
						[ar0++] = ar5;													 // Wtrite result to memory (ar5 = pc + FFFF_000Ah)

// Address register 6 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;	  			
      			ar6 = 61h set;													 // Fill the queue and increment the ar6 counter
						ar6 = 62h set;													 
						ar6 = 63h set;
						ar6 = 64h set;
						ar6 = 65h set;
						ar6 = 66h set;
						ar6 = 67h set;
						ar6 = 68h set;
						ar6 = 69h set;
						ar6 = 70h set;
						
        		ar1 = ar6 addr;														
						[ar0++] = ar6;													 // Wtrite result to memory (ar6 = 70h)
						[ar0++] = ar1;													 // Wtrite result to memory (ar1 = 70h)

// Address register 7 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;	         	
      			ar7 = 71h set;														// Fill the queue and increment the ar7 counter
						ar7 = 72h set;														
						ar7 = 73h set;
						ar7 = 74h set;
						ar7 = 75h set;
						ar7 = 76h set;
						ar7 = 77h set;
						ar7 = 78h set;
						ar7 = 79h set;
						ar7 = 80h set;
						
        	 [ar0++] = ar7, gr7;												// Wtrite result to memory (gr7 = FFFF_0008h || ar7 = 80h)

// GP register 0 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									  // Clear the instruction queue 												
.endrepeat;	                       	
        		with gr0 = gr7 R<< 31;										// Fill the queue and increment the gr0 counter
						with gr0 = gr1 C<<1;											
						with gr0 = true;
 						with gr0 = false;
						with gr0 = gr1 - gr2;
						with gr0 = gr7 + gr7;
						with gr0 = gr3 *: gr7;
						with gr0 = gr4 * gr7;
						with gr0 = gr2 - gr5;
						with gr0 = gr3 - 1;

			 			gr0 = 0FF0h with gr0 = gr0 - gr0;        	// Write 0FF0h to gr0
        		ar1 = gr0 addr;													 	// Use gr0 as source for address modification
						[ar0++] = ar1;													 	// Wtrite result to memory (ar1 = 0FF0h)
						[ar0++] = gr0;														// Wtrite result to memory (gr0 = 0FF0h)

// GP register 1 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									  // Clear the instruction queue 												
.endrepeat;	             																					
      			with gr1 = gr7 R<< 31;										// Fill the queue and increment the gr1 counter
						with gr1 = gr0 C<<1;											
						with gr1 = true;
 						with gr1 = false;
						with gr1 = gr0 - gr2;
						with gr1 = gr7 + gr7;
						with gr1 = gr3 *: gr7;
						with gr1 = gr4 * gr7;
						with gr1 = gr2 - gr5;
						with gr1 = gr3 - 1;

			 			gr1 = 0FF1h with gr1 = gr1 - gr1;        	// Write 0FF1h to gr1
        		ar1 = gr1 addr;													 	// Use gr1 as source for address modification
						[ar0++] = ar1, gr1;												// Wtrite result to memory (gr1 = 0FF1h || ar1 = 0FF1h)

// GP register 2 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									  // Clear the instruction queue 												
.endrepeat;	            		
      			with gr2 = gr7 R<< 31;										// Fill the queue and increment the gr2 counter
						with gr2 = gr0 C<<1;											
						with gr2 = true;
 						with gr2 = false;
						with gr2 = gr0 - gr1;
						with gr2 = gr7 + gr7;
						with gr2 = gr3 *: gr7;
						with gr2 = gr4 * gr7;
						with gr2 = gr1 - gr5;
						with gr2 = gr3 - 1;

			 			gr2 = 0FF2h with gr2 = gr2 - gr2;        	// Write 0FF2h to gr2
        		ar2 = gr2 addr;													 	// Use gr2 as source for address modification
						[ar0++] = ar2, gr2;												// Wtrite result to memory (gr2 = 0FF2h || ar2 = 0FF2h)

// GP register 3 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									  // Clear the instruction queue 												
.endrepeat;	                           	
      			with gr3 = gr7 R<< 31;										// Fill the queue and increment the gr3 counter
						with gr3 = gr0 C<<1;											
						with gr3 = true;
 						with gr3 = false;
						with gr3 = gr0 - gr1;
						with gr3 = gr7 + gr7;
						with gr3 = gr2 *: gr7;
						with gr3 = gr4 * gr7;
						with gr3 = gr1 - gr5;
						with gr3 = gr2 - 1;

			 			gr3 = 0FF3h with gr3 = gr3 - gr3;        	// Write 0FF2h to gr3
        		ar3 = gr3 addr;													 	// Use gr2 as source for address modification
						[ar0++] = ar3, gr3;												// Wtrite result to memory (gr3 = 0FF3h || ar3 = 0FF3h)

// GP register 4 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									  // Clear the instruction queue 												
.endrepeat;	                		
      			gr4 = 0FF00h;															// Fill the queue and increment the gr4 counter
						with gr4 = gr4 + 1 noflags;								
						with gr4 = gr4 + 1 noflags;
						with gr4 = gr4 + 1 noflags;
						with gr4 = gr4 + 1 noflags;
						with gr4 = gr4 + 1 noflags;
						with gr4 = gr4 + 1 noflags;
						with gr4 = gr4 + 1 noflags;
						with gr4 = gr4 + 1 noflags;
						with gr4 = gr4 + 1 noflags;						

			 			with gr4 = gr4 + 1;        								// Write 0FF0Ah to gr2
        		ar4 = gr4 addr;													 	// Use gr4 as source for address modification
						[ar0++] = ar4, gr4;												// Wtrite result to memory (gr4 = 0FF0Ah || ar4 = 0FF0Ah)

// GP register 5 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									  // Clear the instruction queue 												
.endrepeat;	              				
       			gr5 = 0FF01h;															// Fill the queue and increment the gr5 counter
						with gr5 = gr5 + 1 noflags;								
						with gr5 = gr5 + 1 noflags;
						with gr5 = gr5 + 1 noflags;
						with gr5 = gr5 + 1 noflags;
						with gr5 = gr5 + 1 noflags;
						with gr5 = gr5 + 1 noflags;
						with gr5 = gr5 + 1 noflags;
						with gr5 = gr5 + 1 noflags;
						with gr5 = gr5 + 1 noflags;						

			 			with gr5 = gr5 + 1;        								// Write 0FF0Bh to gr2
        		ar5 = gr5 addr;													 	// Use gr5 as source for address modification
						[ar0++] = ar5, gr5;												// Wtrite result to memory (gr5 = 0FF0Bh || ar5 = 0FF0Bh)

// GP register 6 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									  // Clear the instruction queue 												
.endrepeat;	                  			
      			gr6 = 0FF01h;															// Fill the queue and increment the gr6 counter
						with gr6 = gr6 + 1 noflags;								
						with gr6 = gr6 + 1 noflags;
						with gr6 = gr6 + 1 noflags;
						with gr6 = gr6 + 1 noflags;
						with gr6 = gr6 + 1 noflags;
						with gr6 = gr6 + 1 noflags;
						with gr6 = gr6 + 1 noflags;
						with gr6 = gr6 + 1 noflags;
						with gr6 = gr6 + 1 noflags;						

			 			with gr6 = gr6 + 1;        								// Write 0FF0Bh to gr2
        		ar6 = gr6 addr;													 	// Use gr5 as source for address modification
						[ar0++] = ar6, gr6;												// Wtrite result to memory (gr6 = 0FF0Bh || ar6 = 0FF0Bh)

// GP register 7 counter testing
.repeat 12;
						with gr0 = gr0 noflags;									  // Clear the instruction queue 												
.endrepeat;	                     			
       			with gr7 = gr7 R<< 31;										// Fill the queue and increment the gr7 counter
						with gr7 = gr0 C<<1;											
						with gr7 = true;
 						with gr7 = false;
						with gr7 = gr0 - gr1;
						with gr7 = gr6 + gr6;
						with gr7 = gr3 *: gr7;
						with gr7 = gr4 * gr7;
						with gr7 = gr1 - gr5;
						with gr7 = gr2 - 1;

			 			gr7 = 0FF0Ch with gr7 = gr7 - gr7;       // Write 0FF0Ch to gr7
        		ar7 = gr7 addr;													 // Use gr7 as source for address modification
						[ar0++] = ar7, gr7;											 // Wtrite result to memory (gr7 = 0FF0Ch || ar7 = 0FF0Ch)

//  NZVC (FLAGS) counter testing (for jumps/calls)

						ar7 = STACK_ADDRESS;										 // Set stack pointer
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;	                    		
       			pswr = 0h;															 // Fill the queue and increment the NZVC counter	
						pswr set 0h;															
						pswr clear 0FFFh;
						with gr7 - gr7;
						with gr6 or gr5;
						with gr2 and gr1;
						with gr0 - 1;
						with gr3 + 1;
						with false;
						with true;
						
						pswr set 1Fh with gr1 - 1;								// Set flags Y, N, Z, V, C
						if vfalse goto Error;											// Error, if V=0
						if <>0 goto Error;												// Error, if Z=0
						if u< call Error;													// Error, if C=0
						if >= return;															// Error, if N=0
						
						[ar0++] = pc, pswr;												// Write result to memory (pswr = 1fh || pc)
						
//  NZVC (FLAGS) counter testing (for intr)
						
.repeat 12;
						with gr0 = gr0 noflags;									// Clear the instruction queue 												
.endrepeat;	               		
       			with gr0 + 1; 														// Fill the queue and increment the NZVC counter
						with gr1 - 1; 														
						with gr2 + 1;														
						with false;
						with gr3 - 1;
						with false;
						with gr4 + 1;
						with false;
						with gr5 - 1;
						with false ;
						with gr5 + 1;
						with false;
						with gr6 - 1;
						with false;
						[ar0++] = intr with gr7 = gr7 A<<31;			// Set V=1, wtrite result to memory (intr = 100h)
      			intr clear 100h;													// Reset overflow interrupt request						
.repeat 30;																						// Wait 
						nul;
.endrepeat;						
						[ar0++] = intr;														// Wtrite result to memory (intr = 0h) 

// EN bit counter testing
						pswr set 1000h;														// EN = 1
						call M1;																	// First call (write to stack: pswr = 1016h, EN = 1, DELTA = xxxxh; DEL_VALID = 0 on address 0B_0100h)
<M1>												
.repeat 12;
						with gr0 = gr0 noflags;									  // Clear the instruction queue 												
.endrepeat;	                    		
       			pswr = 0h;															  // Fill the queue and increment the EN bit counter	
						pswr clear 1000h;													
						pswr set 0h;
						pswr = 0h;
						pswr = 0h;
						pswr = 0h;																
						pswr clear 1000h;													
						pswr set 0h;
						pswr = 0h;
						pswr = 0h;															
						pswr clear 1000h;													
						pswr set 1000h;														// EN = 1
						call R2;																	// Second call (write to stack: pswr = 1000h, EN = 1, DELTA = xxxxh; DEL_VALID = ? on address 0_B102h)
						call M3;																	// Third  call (write to stack: pswr = 1000h, EN = 1, DELTA = xxxxh; DEL_VALID = 0 on address 0_B0102h)
						goto Error;																// Error (previous SSH was not rewrite)

<R2>				return;																		// Return to instruction after second call from SSH !!!, not from memory

<M3>				call R4;																	// Fourth call (write to stack: pswr = 9000h, EN = 1, DELTA = FFFEh; DEL_VALID = 1 on address 0_B104h)
						call R5;																	// Fifth call (write to stack: pswr = 9000h, EN = 1, DELTA = FFFEh; DEL_VALID = 1 on address 0_B104h)
						goto M4;															// Go to test finish

<R4>				return;																		// Return to instruction after fourth call from SSH, read to SSH from memory (address - 0_B102h)																		

<R5>
						return;																		// Return to instruction after fifth call from SSH, read to SSH from memory (address - 0_B0102h)												
<M4>
// The program of the given and control dimension comparison 
	ar0 = test_result;
	ar1 = contr_res;
	gr0 = 68;

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
		    halt;										             // Wait (test finished)
		    
end ".text.init";
