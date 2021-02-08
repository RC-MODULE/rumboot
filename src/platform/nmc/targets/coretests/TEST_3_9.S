// Test 3.9. Calls and Returns with and without SSH
// Test is executed after system reset
// The result of the test you can see only on IMU3 int. memory

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation
const STACK_ADDRESS = 0_1100h; 										// Top address of system stack
 
begin ".text.init"

global test_3_9 : label;
   global start : label;
	  weak mytest : label;
	
Normal, Error, Finish : label;

M1, M2, M3, M4, M6, M9, M10 : label;
R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11 : label;



<test_3_9>
<start>	
<mytest>					
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

						gr0 = 0h;																// Set gr0 to 0
						gr1 = 0h;																// Set gr1 to 0
						gr2 = 0h;																// Set gr2 to 0		
						gr3 = 0h;								 								// Set gr3 to 0		
						gr4 = 0h;																// Set gr4 to 0		
						gr5 = 0h;																// Set gr5 to 0		
						gr6 = 0h;																// Set gr6 to 0		
						gr7 = 0h;																// Set gr7 to 0		
 
<M1>				ar7 = STACK_ADDRESS;										// Set system stack address
						if <>0 call M2; 												// 1-th call (write to stack: pswr = 0, EN = 0, DELTA = xxxxh; DEL_VALID = 0 on address 0A_0100h)
						pswr set 1000h;													// Set bit EN (ireturn from first call)						
						if <>0 call M3; 												// 3-th call (write to stack: pswr = 0, EN = 1, DELTA = xxxxh; DEL_VALID = 0 on address 0A_0100h)						
						call M6;																// 6-th call (write to stack: pswr = 1Ah, EN = 1, DELTA = xxxxh; DEL_VALID = 0 on address 0A_0100h)				
						call M9;																// 9-th call (write to stack: pswr = 1Ah, EN = 1, DELTA = xxxxh; DEL_VALID = 0 on address 0A_0100h)
						call Normal;														// Last call (write to stack: pswr = 1Ah, EN = 1, DELTA = xxxxh; DEL_VALID = 0 on address 0A_0100h)												
						
<M2>				pswr set 1Fh;														// Set flags Y, N, Z, V, C							
						if <>0 call Error;                      // The call should not be executed
						if =0 call R2; 													// 2-th call (write to stack: pswr = 1Fh, EN = 0, DELTA = xxxxh; DEL_VALID = 0 on address 0A_0102h)
<R1>				if =0 call Error;                      	// The call should not be executed						
						if <>0 ireturn;													// Return to instruction after 1-th call	from memory					
						  						
<R2>				pswr clear 5h;													// Reset flags Z, C		
						if =0 call Error;                      	// The call should not be executed				
						if <>0 return; 													// Return to instruction after 2-th call from memory

<M3>				pswr set 1Ah;														// Set flags Y, N, V
						if vfalse call Error;                   // The call should not be executed						
						if vtrue call M4; 											// 4-th call (write to stack: pswr = 1Ah, EN = 1, DELTA = FFFEh; DEL_VALID = 0 on address 0A_0102h)
<R3>				return;																	// Return to instruction after 3-th call from memory

<M4>				ar7 = 0_1102h; 											    // Set system stack address with large delta
						call R5;																// 5-th call (write to stack: pswr = 1Ah, EN = 1, DELTA = 7FFFh; DEL_VALID = 1 on address 0B_0102h)
<R4>				return;																	// Return to instruction after 4-th call from memory

<R5>				delayed return;													// Return to instruction after 5-th call from SSH, not from memory
						nul;																		// First delayed instruction (32 bits)
						ar7 = 0_1104h;												// Second delayed instruction (64 bits) - restore ar7 (stack pointer)

<M6>				call R7; 																// 7-th call (write to stack: pswr = 1Ah, EN = 1, DELTA = FFFEh; DEL_VALID = 1 on address 0A_0102h)
						call R8;																// 8-th call (write to stack: pswr = 1Ah, EN = 1, DELTA = xxxxh; DEL_VALID = 0 on address 0A_0102h)
<R6>				return;																	// Return to instruction after 6-th call from memory

<R7>				pswr clear 1000h;												// Reset bit EN 
						pswr set 1000h;													// Set bit EN
						return; 																// Return to instruction after 7-th call from memory

<R8>				return;																	// Return to instruction after 8-th call from SSH, not from memory


<M9>				call M10;																// 10-th call (write to stack: pswr = 1Ah, EN = 1, DELTA = FFFEh; DEL_VALID = 1 on address 0A_0102h)
<R9>				return;																	// Return to instruction after 9-th call from SSH, no read from memory

<M10>				pswr = 1FC5h;														// Change pswr (to check SSH_PSWR)
						call R11;																// 11-th call (write to stack: pswr = 1FC5h, EN = 1, DELTA = FFFEh; DEL_VALID = 1 on address 0A_0104h)
<R10>				[ar7++] = pc, pswr;											// Write pswr = 1FC5h on address 0A_0104h
						ar7 -= 2; 															// Restore ar7
						ireturn;																// Return to instruction after 10-th call from SSH, read to SSH from memory (address - 0A_0100h) 

<R11>				pswr = 1000h;														// Change pswr (to check SSH_PSWR)
						ireturn;																// Return to instruction after 11-th call from SSH, read to SSH from memory (address - 0A_0102h)
	
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