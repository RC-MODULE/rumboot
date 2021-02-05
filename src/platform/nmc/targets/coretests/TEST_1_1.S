// Test for NMC4 IMU1-IMU3 internal memory check (3Mbit)
// Part 1.
// The test checks up to 32K*64 bit of internal memory (IMU2, IMU3) as follows:
// 1. For each address from 0000_8000h up to 0001_7FFEh 
// 		write "000...0" to location.
// 2. For each address from 0000_8000h up to 0001_7FFEh
//		read  "000...0"b from location;
//    write "111...1"b to location;
//		read  "111...1"b from location.
// 3. For each address from 0001_7FFEh up to 0000_8000h
//		read  "111...1"b from location;
//    write "000...0"b to location;
//		read  "000...0"b from location.
// 4. For each address from 0001_7FFEh up to 0000_8000h
//		read  "000...0"b from location;
// *****************************************************************
// The check sum is stored in gr1 and allways has to be FFFF_FFFFh

// Part 2.
// The test checks up to 16K*64 bit of internal memory (IMU1) as follows:
// 1. For each address from 0000_0000h up to 0000_7FFEh 
// 		write "000...0" to location.
// 2. For each address from 0000_0000h up to 0000_7FFEh
//		read  "000...0"b from location;
//    write "111...1"b to location;
//		read  "111...1"b from location.
// 3. For each address from 0000_7FFEh up to 0000_0000h
//		read  "111...1"b from location;
//    write "000...0"b to location;
//		read  "000...0"b from location.
// 4. For each address from 0000_7FFEh up to 0000_0000h
//		read  "000...0"b from location;
// *****************************************************************
// The check sum is stored in gr1 and allways has to be FFFF_FFFFh

// !!! This programm should be started from IMU1 of internal memory with initial address 0h !!!

//import from macros640x;
import from "platform/rumboot.mlb";

const CYCLE11 = 29 * 1024; 													  // The size of memory to be checked (29K *64 bit)(for Part 1)
const START_ADDRESS11 = 0000_8000h;  									// Start address of memory to be checked (for Part 1)
const FINISH_ADDRESS11 = 0001_6800h; 									// Last address of memory to be checked plus 2 (for Part 1)
const CYCLE12 = 3 * 1024 - 8; 											  // The size of memory to be checked (3K *64 bit - 8)(for Part 1)
const START_ADDRESS12 =  0001_6810h;  								// Start address of memory to be checked (for Part 1)
const FINISH_ADDRESS12 = 0001_8000h; 									// Last address of memory to be checked plus 2 (for Part 1)

const CYCLE2 = 16 * 1024; 													  // The size of memory to be checked (32K *64 bit)(for Part 2)
const START_ADDRESS2 = 0000_0000h;  									// Start address of memory to be checked (for Part 2)
const FINISH_ADDRESS2 = 0000_8000h; 									// Last address of memory to be checked plus 2 (for Part 2)

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

				global test_1_1 : label;
				   global start : label;				
						weak mytest : label;
  weak mytest_additional: label;	
				LOOP11, LOOP21, LOOP31, LOOP41: label;
				LOOP12, LOOP22, LOOP32, LOOP42: label;				
				LOOP5, LOOP6, LOOP7, LOOP8 : label;
				LOOPI, START_PROGRAM : label;
        Normal, Error, Finish : label;
        
begin ".text.init"
<test_1_1>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

// PART 1.
// As the memory cells with addresses from 016800h up to 01680Fh are located for system purposes,
// we check the memory as to parts, the first with addresses from 8000h up to 0167FEh,
// the second with addresses from 01_6810h up to 01_7FFEh
				
				gr0 = 0h;																		// Determine GP registers
				gr1 = 0h;
				gr2 = 0h;
				gr3 = 0h;
				gr4 = 0h;
				gr5 = gr0;
				gr6 = gr0;
				gr7 = gr0;
				
// Check the memory part 1, with addresses from 8000h up to 0167FEh				
				ar0 = START_ADDRESS11 with gr1 = true; 			// Set start address and check sum
				gr0 = CYCLE11 / 2;													// Set cycle count for the first step
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				ar2,gr2 = 0h;                               // Clear register pair
				        
// The first step of test (write all ziroes)				
<LOOP11>
				if > delayed goto LOOP11 with gr0 = gr0 - 1;// Go to LOOP11
				[ar0++] = ar2,gr2;													// Delayed instruction 1 (Memory_cell[i] <- 000...0)
				[ar0++] = ar2,gr2; 											    // Delayed instruction 2 (Memory_cell[i+1] <- 000...0)

				ar0 = START_ADDRESS11;  										// Set start address
				gr0 = CYCLE11 / 2;													// Set cycle count for the second step
				ar1 = START_ADDRESS11;  										// Set start address again			
				ar2,gr2 = 0FFFF_FFFFh;											// Write all ones to register pair (ar2,gr2)
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				
// The second step of test			
<LOOP21>				
				ar3,gr3 = [ar0];																				// Read all zeroes from i-th location
				[ar0++] = ar2,gr2 with gr1 = gr1  and  not gr3 noflags; // Write all ones to i-th location, calculate check sum
				ar4,gr4 = [ar0];																				// Read all zeroes from (i+1)-th location
				[ar0++] = ar2,gr2 with gr1 = gr1  and  not gr4 noflags; // Write all ones to (i+1)-th location, calculate check sum
				gr7 = ar3;																							// Move ar3 to gr7
				ar5,gr5 = [ar1++] with gr1 = gr1  and  not gr7 noflags;	// Read all ones from i-th location, calculate check sum
				gr7 = ar4;																							// Move ar4 to gr7
				ar6,gr6 = [ar1++] with gr1 = gr1  and  not gr7 noflags;	// Read all ones from (i+1)-th location, calculate check sum
				gr7 = ar5 with gr1 = gr1 and gr5 noflags;				      	// Move ar5 to gr7, calculate check sum
				gr7 = ar6 with gr1 = gr1 and gr7 noflags;				      	// Move ar6 to gr7, calculate check sum
				if > delayed goto LOOP21 with gr0 = gr0 - 1;							// Go to LOOP21
				with gr1 = gr1 and gr6 noflags;													// Delayed instruction 1 (calculate check sum)
				with gr1 = gr1 and gr7 noflags;													// Delayed instruction 2 (calculate check sum)	
								
				ar0 = FINISH_ADDRESS11;  										// Set last address
				gr0 = CYCLE11 / 2;													// Set cycle count for the third step
				ar1 = FINISH_ADDRESS11;  										// Set last address again			
				ar2,gr2 = 0000_0000h;												// Write all zeroes to register pair (ar2,gr2)
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				
// The third step of test			
<LOOP31>				
				ar3,gr3 = [--ar0];																		  // Read all ones from i-th location
				[ar0] = ar2,gr2 with gr1 = gr1 and gr3 noflags; 				// Write all zeroes to i-th location, calculate check sum
				ar4,gr4 = [--ar0];																			// Read all ones from (i+1)-th location
				[ar0] = ar2,gr2 with gr1 = gr1 and gr4 noflags; 				// Write all zeroes to (i+1)-th location, calculate check sum
				gr7 = ar3;																							// Move ar3 to gr7
				ar5,gr5 = [--ar1] with gr1 = gr1 and gr7 noflags;				// Read all zeroes from i-th location, calculate check sum
				gr7 = ar4;																							// Move ar4 to gr7
				ar6,gr6 = [--ar1] with gr1 = gr1 and gr7 noflags;				// Read all zeroes from (i+1)-th location, calculate check sum
				gr7 = ar5 with gr1 = gr1  and  not gr5 noflags;				  // Move ar5 to gr7, calculate check sum
				gr7 = ar6 with gr1 = gr1  and  not gr7 noflags;				  // Move ar6 to gr7, calculate check sum
				if > delayed goto LOOP31 with gr0 = gr0 - 1;							// Go to LOOP31
				with gr1 = gr1  and  not gr6 noflags;										// Delayed instruction 1 (calculate check sum)
				with gr1 = gr1  and  not gr7 noflags;									  // Delayed instruction 2 (calculate check sum)	
				
								
				ar0 = FINISH_ADDRESS11;  									// Set last address
				gr0 = CYCLE11 / 4;												// Set cycle count for the fourth step
				with gr0 = gr0 - 1; 											// Set flags for conditional jump
				        
// The fourth step of test (read all ziroes and calculate vector check sum)		
				
<LOOP41>
				ar3,gr3 = [--ar0];																		  // Read all zeroes from i-th location
				with gr1 = gr1  and  not gr3 noflags;                   // Calculate check sum
				ar4,gr4 = [--ar0];																			// Read all zeroes from (i-1)-th location
				with gr1 = gr1  and  not gr4 noflags;                   // Calculate check sum
				gr7 = ar3;																							// Move ar3 to gr7
				ar5,gr5 = [--ar0] with gr1 = gr1  and  not gr7 noflags;	// Read all zeroes from (i-2)-th location, calculate check sum
				gr7 = ar4;																							// Move ar4 to gr7
				ar6,gr6 = [--ar0] with gr1 = gr1  and  not gr7 noflags;	// Read all ones from (i-3)-th location, calculate check sum
				gr7 = ar5 with gr1 = gr1 and not gr5 noflags;				    // Move ar5 to gr7, calculate check sum
				gr7 = ar6 with gr1 = gr1 and not gr7 noflags;				    // Move ar6 to gr7, calculate check sum
				if > delayed goto LOOP41 with gr0 = gr0 - 1;						// Go to LOOP4
				with gr1 = gr1 and not gr6 noflags;											// Delayed instruction 1 (calculate check sum)
				with gr1 = gr1 and not gr7 noflags;											// Delayed instruction 2 (calculate check sum)	

// As the memory cells with addresses from 016800h up to 01680Fh are located for system purposes,
// we check the memory as to parts, the first with addresses from 8000h up to 0167FFh,
// the second with addresses from 01_6810h up to 01_7FFEh

// Check the memory part 2, with addresses from 01_6810h up to 01_7FFEh	
				ar0 = START_ADDRESS12; 			                // Set start address and check sum
				gr0 = CYCLE12 / 2;													// Set cycle count for the first step
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				ar2,gr2 = 0h;                               // Clear register pair
				        
// The first step of test (write all ziroes)				
<LOOP12>
				if > delayed goto LOOP12 with gr0 = gr0 - 1;// Go to LOOP12
				[ar0++] = ar2,gr2;													// Delayed instruction 1 (Memory_cell[i] <- 000...0)
				[ar0++] = ar2,gr2; 											    // Delayed instruction 2 (Memory_cell[i+1] <- 000...0)

				ar0 = START_ADDRESS12;  										// Set start address
				gr0 = CYCLE12 / 2;													// Set cycle count for the second step
				ar1 = START_ADDRESS12;  										// Set start address again			
				ar2,gr2 = 0FFFF_FFFFh;											// Write all ones to register pair (ar2,gr2)
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				
// The second step of test			
<LOOP22>				
				ar3,gr3 = [ar0];																				// Read all zeroes from i-th location
				[ar0++] = ar2,gr2 with gr1 = gr1  and  not gr3 noflags; // Write all ones to i-th location, calculate check sum
				ar4,gr4 = [ar0];																				// Read all zeroes from (i+1)-th location
				[ar0++] = ar2,gr2 with gr1 = gr1  and  not gr4 noflags; // Write all ones to (i+1)-th location, calculate check sum
				gr7 = ar3;																							// Move ar3 to gr7
				ar5,gr5 = [ar1++] with gr1 = gr1  and  not gr7 noflags;	// Read all ones from i-th location, calculate check sum
				gr7 = ar4;																							// Move ar4 to gr7
				ar6,gr6 = [ar1++] with gr1 = gr1  and  not gr7 noflags;	// Read all ones from (i+1)-th location, calculate check sum
				gr7 = ar5 with gr1 = gr1 and gr5 noflags;				      	// Move ar5 to gr7, calculate check sum
				gr7 = ar6 with gr1 = gr1 and gr7 noflags;				      	// Move ar6 to gr7, calculate check sum
				if > delayed goto LOOP22 with gr0 = gr0 - 1;							// Go to LOOP22
				with gr1 = gr1 and gr6 noflags;													// Delayed instruction 1 (calculate check sum)
				with gr1 = gr1 and gr7 noflags;													// Delayed instruction 2 (calculate check sum)	
								
				ar0 = FINISH_ADDRESS12;  										// Set last address
				gr0 = CYCLE12 / 2;													// Set cycle count for the third step
				ar1 = FINISH_ADDRESS12;  										// Set last address again			
				ar2,gr2 = 0000_0000h;												// Write all zeroes to register pair (ar2,gr2)
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				
// The third step of test			
<LOOP32>				
				ar3,gr3 = [--ar0];																		  // Read all ones from i-th location
				[ar0] = ar2,gr2 with gr1 = gr1 and gr3 noflags; 				// Write all zeroes to i-th location, calculate check sum
				ar4,gr4 = [--ar0];																			// Read all ones from (i+1)-th location
				[ar0] = ar2,gr2 with gr1 = gr1 and gr4 noflags; 				// Write all zeroes to (i+1)-th location, calculate check sum
				gr7 = ar3;																							// Move ar3 to gr7
				ar5,gr5 = [--ar1] with gr1 = gr1 and gr7 noflags;				// Read all zeroes from i-th location, calculate check sum
				gr7 = ar4;																							// Move ar4 to gr7
				ar6,gr6 = [--ar1] with gr1 = gr1 and gr7 noflags;				// Read all zeroes from (i+1)-th location, calculate check sum
				gr7 = ar5 with gr1 = gr1  and  not gr5 noflags;				  // Move ar5 to gr7, calculate check sum
				gr7 = ar6 with gr1 = gr1  and  not gr7 noflags;				  // Move ar6 to gr7, calculate check sum
				if > delayed goto LOOP32 with gr0 = gr0 - 1;						// Go to LOOP32
				with gr1 = gr1  and  not gr6 noflags;										// Delayed instruction 1 (calculate check sum)
				with gr1 = gr1  and  not gr7 noflags;									  // Delayed instruction 2 (calculate check sum)	
				
								
				ar0 = FINISH_ADDRESS12;  									// Set last address
				gr0 = CYCLE12 / 4;												// Set cycle count for the fourth step
				with gr0 = gr0 - 1; 											// Set flags for conditional jump
				        
// The fourth step of test (read all ziroes and calculate vector check sum)		
				
<LOOP42>
				ar3,gr3 = [--ar0];																		  // Read all zeroes from i-th location
				with gr1 = gr1  and  not gr3 noflags;                   // Calculate check sum
				ar4,gr4 = [--ar0];																			// Read all zeroes from (i-1)-th location
				with gr1 = gr1  and  not gr4 noflags;                   // Calculate check sum
				gr7 = ar3;																							// Move ar3 to gr7
				ar5,gr5 = [--ar0] with gr1 = gr1  and  not gr7 noflags;	// Read all zeroes from (i-2)-th location, calculate check sum
				gr7 = ar4;																							// Move ar4 to gr7
				ar6,gr6 = [--ar0] with gr1 = gr1  and  not gr7 noflags;	// Read all ones from (i-3)-th location, calculate check sum
				gr7 = ar5 with gr1 = gr1 and not gr5 noflags;				    // Move ar5 to gr7, calculate check sum
				gr7 = ar6 with gr1 = gr1 and not gr7 noflags;				    // Move ar6 to gr7, calculate check sum
				if > delayed goto LOOP42 with gr0 = gr0 - 1;						// Go to LOOP42
				with gr1 = gr1 and not gr6 noflags;											// Delayed instruction 1 (calculate check sum)
				with gr1 = gr1 and not gr7 noflags;	




// *********************************************************************************************
// PART 2.	
				ar0 = START_PROGRAM;                        // Set start address of source
				ar1 = START_PROGRAM + 0001_0000h;						// Set start address of destination (IMU3)
				gr0 = 96;                                   // Move up to 96 instructions to internal global memory
				with gr0 = gr0 - 1; 												// Set flags for conditional jump	
				
<LOOPI>
				if > delayed skip LOOPI with gr0 = gr0 - 1;	// Go to LOOPI
				ar2,gr2 = [ar0++];													// Delayed instruction 1 
				[ar1++] = ar2,gr2; 											    // Delayed instruction 2 			

.repeat 20;																					// The WBUF clearning
				nul;
.endrepeat;
				
				
				pc = START_PROGRAM + 0001_0000h;						// Jump to IMU3
				vnul;																				// Delayed instruction 1
				vnul;																				// Delayed instruction 2
				
// ********************* MAIN PROGRAM (PART 2) ********************************************
<START_PROGRAM>				
				ar0 = START_ADDRESS2; 			                // Set start address and check sum
				gr0 = CYCLE2 / 2;														// Set cycle count for the first step
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				ar2,gr2 = 0h;                               // Clear register pair
				        
// The first step of test (write all ziroes)				
<LOOP5>
				if > delayed skip LOOP5 with gr0 = gr0 - 1;	// Go to LOOP1
				[ar0++] = ar2,gr2;													// Delayed instruction 1 (Memory_cell[i] <- 000...0)
				[ar0++] = ar2,gr2; 											    // Delayed instruction 2 (Memory_cell[i+1] <- 000...0)

				ar0 = START_ADDRESS2;  											// Set start address
				gr0 = CYCLE2 / 2;														// Set cycle count for the second step
				ar1 = START_ADDRESS2;  											// Set start address again			
				ar2,gr2 = 0FFFF_FFFFh;											// Write all ones to register pair (ar2,gr2)
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				
// The second step of test			
<LOOP6>				
				ar3,gr3 = [ar0];																				// Read all zeroes from i-th location
				[ar0++] = ar2,gr2 with gr1 = gr1  and  not gr3 noflags; // Write all ones to i-th location, calculate check sum
				ar4,gr4 = [ar0];																				// Read all zeroes from (i+1)-th location
				[ar0++] = ar2,gr2 with gr1 = gr1  and  not gr4 noflags; // Write all ones to (i+1)-th location, calculate check sum
				gr7 = ar3;																							// Move ar3 to gr7
				ar5,gr5 = [ar1++] with gr1 = gr1  and  not gr7 noflags;	// Read all ones from i-th location, calculate check sum
				gr7 = ar4;																							// Move ar4 to gr7
				ar6,gr6 = [ar1++] with gr1 = gr1  and  not gr7 noflags;	// Read all ones from (i+1)-th location, calculate check sum
				gr7 = ar5 with gr1 = gr1 and gr5 noflags;				      	// Move ar5 to gr7, calculate check sum
				gr7 = ar6 with gr1 = gr1 and gr7 noflags;				      	// Move ar6 to gr7, calculate check sum
				if > delayed skip LOOP6 with gr0 = gr0 - 1;							// Go to LOOP2
				with gr1 = gr1 and gr6 noflags;													// Delayed instruction 1 (calculate check sum)
				with gr1 = gr1 and gr7 noflags;													// Delayed instruction 2 (calculate check sum)	
								
				ar0 = FINISH_ADDRESS2;  										// Set last address
				gr0 = CYCLE2 / 2;														// Set cycle count for the third step
				ar1 = FINISH_ADDRESS2;  										// Set last address again			
				ar2,gr2 = 0000_0000h;												// Write all zeroes to register pair (ar2,gr2)
				with gr0 = gr0 - 1; 												// Set flags for conditional jump
				
// The third step of test			
<LOOP7>				
				ar3,gr3 = [--ar0];																		  // Read all ones from i-th location
				[ar0] = ar2,gr2 with gr1 = gr1 and gr3 noflags; 				// Write all zeroes to i-th location, calculate check sum
				ar4,gr4 = [--ar0];																			// Read all ones from (i+1)-th location
				[ar0] = ar2,gr2 with gr1 = gr1 and gr4 noflags; 				// Write all zeroes to (i+1)-th location, calculate check sum
				gr7 = ar3;																							// Move ar3 to gr7
				ar5,gr5 = [--ar1] with gr1 = gr1 and gr7 noflags;				// Read all zeroes from i-th location, calculate check sum
				gr7 = ar4;																							// Move ar4 to gr7
				ar6,gr6 = [--ar1] with gr1 = gr1 and gr7 noflags;				// Read all zeroes from (i+1)-th location, calculate check sum
				gr7 = ar5 with gr1 = gr1  and  not gr5 noflags;				  // Move ar5 to gr7, calculate check sum
				gr7 = ar6 with gr1 = gr1  and  not gr7 noflags;				  // Move ar6 to gr7, calculate check sum
				if > delayed skip LOOP7 with gr0 = gr0 - 1;							// Go to LOOP3
				with gr1 = gr1  and  not gr6 noflags;										// Delayed instruction 1 (calculate check sum)
				with gr1 = gr1  and  not gr7 noflags;									  // Delayed instruction 2 (calculate check sum)	
				
								
				ar0 = FINISH_ADDRESS2;  									// Set last address
				gr0 = CYCLE2 / 4;													// Set cycle count for the fourth step
				with gr0 = gr0 - 1; 											// Set flags for conditional jump
				        
// The fourth step of test (read all ziroes and calculate vector check sum)		
				
<LOOP8>
				ar3,gr3 = [--ar0];																		  // Read all zeroes from i-th location
				with gr1 = gr1  and  not gr3 noflags;                   // Calculate check sum
				ar4,gr4 = [--ar0];																			// Read all zeroes from (i-1)-th location
				with gr1 = gr1  and  not gr4 noflags;                   // Calculate check sum
				gr7 = ar3;																							// Move ar3 to gr7
				ar5,gr5 = [--ar0] with gr1 = gr1  and  not gr7 noflags;	// Read all zeroes from (i-2)-th location, calculate check sum
				gr7 = ar4;																							// Move ar4 to gr7
				ar6,gr6 = [--ar0] with gr1 = gr1  and  not gr7 noflags;	// Read all ones from (i-3)-th location, calculate check sum
				gr7 = ar5 with gr1 = gr1 and not gr5 noflags;				    // Move ar5 to gr7, calculate check sum
				gr7 = ar6 with gr1 = gr1 and not gr7 noflags;				    // Move ar6 to gr7, calculate check sum
				if > delayed skip LOOP8 with gr0 = gr0 - 1;							// Go to LOOP2
				with gr1 = gr1 and not gr6 noflags;											// Delayed instruction 1 (calculate check sum)
				with gr1 = gr1 and not gr7 noflags;											// Delayed instruction 2 (calculate check sum)

				with not gr1;
				if <>0 skip Error;
				
<Normal>   
				rumboot_exit(0_16800h, 0);
//	      ar0 = STOP_SIMULATION with gr1 = false;
//	      [ar0] = gr1;												 // Set 00h into the location 1_6800h (IM3), if the test is correct
	      skip Finish;
<Error>
				rumboot_exit(0_16800h, 1);
				//gr1 = 01h;
	      //ar0 = STOP_SIMULATION;
	      //[ar0] = gr1;	                       // Set 01h into the location 1_6800h (IM3), if the test is incorrect
	  
<Finish>
		    halt;										               // Wait (test finished)
		    
end ".text.init";

