// Test 9.7. Communication ports 0 and 1 Testing with Hamming Code!
// 1. During reset force the com. port 0 to output mode and the com. port 1 to input mode.
// 2. Start com. port 0 in output mode, com. port 1 in input mode (transmit from internal memory bank 1 and receive to internal memory bank 2)
// in two dimension mode (RowCounter = 2, Bias = FFFCh).
// 3. When com. ports finish it's work, start com. port 1 in output mode, com. port 0 in input mode (transmit from internal memory bank 2 and 
// receive to internal memory bank 3) in two dimention mode (RowCounter = 7FFFh, Bias = 2h).
// 4. Wait, until com. ports finish it's work,.
// 5. Compare the array in internal memory bank 1 with the arraies in internal memory banks 2 and 3. If all three arrays are equal, test is
// finished successfully.

//import from macros640x;
import from "platform/rumboot.mlb";

// The data to transmit (8K*64bits),  placed in IM1 
begin text_dma
begin_DMA : word[2] = (0h dup 2);
				var N = 0;
				var M = 0;
.repeat 16384;
				M = (not N) and 0FFFFh;
				code (N << 16) or M;
				N = N + 1;
.endrepeat;
end text_dma;

// The test result dimension
data data_test_res
test_result : word[34] = (-1 dup 34);
// The control value dimension
contr_result : word[34] = (
 													 3000400h, 0h dup 3, 
 													 3000400h, 06_0000h, 0h, 0h, 
 													 0h, 0C001_06c0h, 8h, 07FFh, 1h, 0h dup 3, 		
 													 21h, 1h,	0h dup 2, 														 										  
 													 0h, 0C007_8000h, 8h, 07FFh, 1h, 0h dup 3,
 													 21h,  1h, 0h dup 2, 
 													 0h, 0ffffffffh
                            );
end data_test_res;

//const ARRAY1_ADDRESS = 0001_0000h;                    // Address of array in internal memory bank 1 (initial array)
const ARRAY2_ADDRESS = 0000_C000h;                      // Address of array in internal memory bank 2 (secondary array)
const ARRAY2_CP_ADDRESS = 0C005_0000h;                  // Byte Address of array in internal memory bank 2 (secondary array)
const ARRAY3_ADDRESS = 0001_2000h;                      // Address of array in internal memory bank 3 (third array)
const ARRAY3_CP_ADDRESS = 0C006_8000h;                  // Byte Address of array in internal memory bank 3 (third array)

const IRRL_ADDRESS = 0003_8000h;  											// External Interrupt 31-0 Request Register address  
const IRRL_SET_ADDRESS = 0003_8002h;  						  		// External Interrupt 31-0 Request Register address for Bit set  
const IRRL_RESET_ADDRESS = 0003_8004h;  								// External Interrupt 31-0 Request Register address for Bit Reset  
const IMRL_ADDRESS = 0003_8008h;  											// External Interrupt 31-0 Mask Register address  

const IRRH_ADDRESS = 0003_8040h;  											// External Interrupt 39-32 Request Register address (High Part)
const IRRH_SET_ADDRESS = 0003_8042h;  						 			// External Interrupt 39-32 Request Register address for Bit set (High Part)
const IRRH_RESET_ADDRESS = 0003_8044h;  								// External Interrupt 39-32 Request Register address for Bit Reset (High Part)
const IMRH_ADDRESS = 0003_8048h;  											// External Interrupt 39-32 Mask Register address (High Part)

const BASE_CP0_ADDRESS = 300C_1000h;                    // Base Address for Com. Port 0
const BASE_CP1_ADDRESS = 300C_1400h;                   	// Base Address for Com. Port 1

// Addresses of Com. Port 0 Registers (Transmit Part)
const CP0TR_MainCounter_ADDRESS = BASE_CP0_ADDRESS;
const CP0TR_ADDRESS_REGISTER_ADDRESS = BASE_CP0_ADDRESS + 1h;
const CP0TR_BIAS_ADDRESS = BASE_CP0_ADDRESS + 2h;
const CP0TR_RowCounter_ADDRESS = BASE_CP0_ADDRESS + 3h;
const CP0TR_AddressMode_ADDRESS = BASE_CP0_ADDRESS + 4h;
const CP0TR_ControlRegister_ADDRESS = BASE_CP0_ADDRESS + 5h;
const CP0TR_InterruptMask_ADDRESS = BASE_CP0_ADDRESS + 6h;
const CP0TR_StateRegister_ADDRESS = BASE_CP0_ADDRESS + 7h;

// Addresses of Com. Port 0 Registers (Receive Part)
const CP0RC_MainCounter_ADDRESS = BASE_CP0_ADDRESS + 8h;
const CP0RC_ADDRESS_REGISTER_ADDRESS = BASE_CP0_ADDRESS + 9h;
const CP0RC_BIAS_ADDRESS = BASE_CP0_ADDRESS + 0Ah;
const CP0RC_RowCounter_ADDRESS = BASE_CP0_ADDRESS + 0Bh;
const CP0RC_AddressMode_ADDRESS = BASE_CP0_ADDRESS + 0Ch;
const CP0RC_ControlRegister_ADDRESS = BASE_CP0_ADDRESS + 0Dh;
const CP0RC_InterruptMask_ADDRESS = BASE_CP0_ADDRESS + 0Eh;
const CP0RC_StateRegister_ADDRESS = BASE_CP0_ADDRESS + 0Fh;

// Addresses of Congiguration Com. Port 0 Registers
const CP0_HEM_EN_ADDRESS = BASE_CP0_ADDRESS +10h;            // [0], 0/1 - hamming dis/en
const CP0_PHY_CONFIG_ADDRESS = BASE_CP0_ADDRESS + 11h;       // [5-0], [5] - MSB, [4] - CDS, [3-0] - DIV
const CP0_ERR1_CNT_ADDRESS = BASE_CP0_ADDRESS + 1ah;         // [31-0], [31] - overflow ERR1, [30-16] = 00...0, [15:0] - ERR1_CNT
const CP0_ERR2_CNT_ADDRESS = BASE_CP0_ADDRESS + 1bh;         // [31-0], [31] - overflow ERR1, [30-16] = 00...0, [15:0] - ERR1_CNT

// Addresses of Com. Port 1 Registers (Transmit Part)
const CP1TR_MainCounter_ADDRESS = BASE_CP1_ADDRESS;
const CP1TR_ADDRESS_REGISTER_ADDRESS = BASE_CP1_ADDRESS + 1h;
const CP1TR_BIAS_ADDRESS = BASE_CP1_ADDRESS + 2h;
const CP1TR_RowCounter_ADDRESS = BASE_CP1_ADDRESS + 3h;
const CP1TR_AddressMode_ADDRESS = BASE_CP1_ADDRESS + 4h;
const CP1TR_ControlRegister_ADDRESS = BASE_CP1_ADDRESS + 5h;
const CP1TR_InterruptMask_ADDRESS = BASE_CP1_ADDRESS + 6h;
const CP1TR_StateRegister_ADDRESS = BASE_CP1_ADDRESS + 7h;

// Addresses of Com. Port 1 Registers (Receive Part)
const CP1RC_MainCounter_ADDRESS = BASE_CP1_ADDRESS + 8h;
const CP1RC_ADDRESS_REGISTER_ADDRESS = BASE_CP1_ADDRESS + 9h;
const CP1RC_BIAS_ADDRESS = BASE_CP1_ADDRESS + 0Ah;
const CP1RC_RowCounter_ADDRESS = BASE_CP1_ADDRESS + 0Bh;
const CP1RC_AddressMode_ADDRESS = BASE_CP1_ADDRESS + 0Ch;
const CP1RC_ControlRegister_ADDRESS = BASE_CP1_ADDRESS + 0Dh;
const CP1RC_InterruptMask_ADDRESS = BASE_CP1_ADDRESS + 0Eh;
const CP1RC_StateRegister_ADDRESS = BASE_CP1_ADDRESS + 0Fh;

// Addresses of Congiguration Com. Port 1 Registers
const CP1_HEM_EN_ADDRESS = BASE_CP1_ADDRESS + 10h;           // [0], 0/1 - hamming dis/en
const CP1_PHY_CONFIG_ADDRESS = BASE_CP1_ADDRESS + 11h;       // [5-0], [5] - MSB, [4] - CDS, [3-0] - DIV
const CP1_ERR1_CNT_ADDRESS = BASE_CP1_ADDRESS + 1ah;         // [31-0], [31] - overflow ERR1, [30-16] = 00...0, [15:0] - ERR1_CNT
const CP1_ERR2_CNT_ADDRESS = BASE_CP1_ADDRESS + 1bh;         // [31-0], [31] - overflow ERR1, [30-16] = 00...0, [15:0] - ERR1_CNT

// Constants for Com. Port Control
const CP_START = 1h;
const CP_STOP = 0h;
const CP_OneDimentionAddressMode = 0h;
const CP_TwoDimentionAddressMode = 1h;
const CP_InterruptEnable = 0h;
const CP_InterruptDisable = 3h;

global test_9_7 : label;
   global start : label;
		weak mytest : label;
			
 L1, L2, L3, L4, L5, L6, L7, L8 : label; 	
        	Normal, Error, Finish : label;
        	           Loop_Check : label;
        	                  
// The boot loader and main program (placed in memory with start address 0h (internal memory bank 0) 								 
begin ".text.init"
<test_9_7>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

				ar0 = test_result;									  // Set result address					
				ar1 = intr;							
				gr1 = [IRRL_ADDRESS];
				ar2 = [IRRH_ADDRESS];
				gr2 = [IMRL_ADDRESS];
							
				[ar0++] = ar1,gr1;										// Read  irrl, intr
				[ar0++] = ar2,gr2;										// Read cp rate, imrl	

// Com. Port 1 Initialization (Receive Mode)
        gr7 = ARRAY2_CP_ADDRESS;
        [CP1RC_ADDRESS_REGISTER_ADDRESS] = gr7;
        gr7 = 4;
        [CP1RC_MainCounter_ADDRESS] = gr7;
        gr7 = 2;
        [CP1RC_RowCounter_ADDRESS] = gr7;
        gr7 = 7FE8h;
        [CP1RC_BIAS_ADDRESS] = gr7;       
        gr7 = CP_TwoDimentionAddressMode;
        [CP1RC_AddressMode_ADDRESS] = gr7;
        gr7 = CP_InterruptEnable;
        [CP1RC_InterruptMask_ADDRESS] = gr7;
        
// Com. Port 0 Initialization (Transmit Mode)
        gr7 = begin_DMA + 3000_0002h;
        with gr7 = gr7 << 2;
        [CP0TR_ADDRESS_REGISTER_ADDRESS] = gr7;
        gr7 = 4;
        [CP0TR_MainCounter_ADDRESS] = gr7;
        gr7 = 2;
        [CP0TR_RowCounter_ADDRESS] = gr7;
        gr7 = 7FE8h;
        [CP0TR_BIAS_ADDRESS] = gr7;         
        gr7 = CP_TwoDimentionAddressMode;
        [CP0TR_AddressMode_ADDRESS] = gr7;
        gr7 = CP_InterruptEnable;
        [CP0TR_InterruptMask_ADDRESS] = gr7;   

				gr7= 1h;
				[CP0_HEM_EN_ADDRESS] = gr7;
				[CP1_HEM_EN_ADDRESS] = gr7;    
				  
// set Com. Port Rate = 25 MB/sec, MSB First!
        gr7 = 10_0011b;
        [CP0_PHY_CONFIG_ADDRESS] = gr7;         
        [CP1_PHY_CONFIG_ADDRESS] = gr7;             
        
// Start Com. Ports 0 and 1
        gr7 =  CP_START;
        [CP0TR_ControlRegister_ADDRESS] = gr7;
        [CP1RC_ControlRegister_ADDRESS] = gr7;      

//  Wait until Com. Port Transmition is finished         
<L1>        
      gr7 = 2h;
      gr0 = [CP0TR_ControlRegister_ADDRESS];   
      gr2 = [CP1RC_ControlRegister_ADDRESS] with gr7 = gr7 and gr0;
      with gr7 = gr7 and gr2; 
      if =0 goto L1;		
      gr7 = CP_STOP;
      [CP0TR_ControlRegister_ADDRESS] = gr7;
      [CP1RC_ControlRegister_ADDRESS] = gr7;       					  
            				       
 // Com. Port 1 Initialization (Receive Mode)
        gr7 = ARRAY3_CP_ADDRESS;
        [CP1RC_ADDRESS_REGISTER_ADDRESS] = gr7;
        gr7 = 8 * 1024;
        [CP1RC_MainCounter_ADDRESS] = gr7;
        gr7 = 07FFh;
        [CP1RC_RowCounter_ADDRESS] = gr7;
        gr7 = 8h;
        [CP1RC_BIAS_ADDRESS] = gr7;         
        gr7 = CP_TwoDimentionAddressMode;
        [CP1RC_AddressMode_ADDRESS] = gr7;
        gr7 = CP_InterruptEnable;
        [CP1RC_InterruptMask_ADDRESS] = gr7;
        
// Com. Port 0 Initialization (Transmit Mode)
        gr7 = begin_DMA + 3000_0002h;
        with gr7 = gr7 << 2;
        [CP0TR_ADDRESS_REGISTER_ADDRESS] = gr7;
        gr7 = 8 * 1024;
        [CP0TR_MainCounter_ADDRESS] = gr7;
        gr7 = 07FFh;
        [CP0TR_RowCounter_ADDRESS] = gr7;
        gr7 = 8h;
        [CP0TR_BIAS_ADDRESS] = gr7;         
        gr7 = CP_TwoDimentionAddressMode;
        [CP0TR_AddressMode_ADDRESS] = gr7;
        gr7 = CP_InterruptEnable;
        [CP0TR_InterruptMask_ADDRESS] = gr7;   
        
// set Com. Port Rate = 50 MB/sec, MSB First!
        gr7 = 10_0001b;
        [CP0_PHY_CONFIG_ADDRESS] = gr7;         
        [CP1_PHY_CONFIG_ADDRESS] = gr7;               
        
// Start Com. Ports 0 and 1
        gr7 =  CP_START;
        [CP1RC_ControlRegister_ADDRESS] = gr7;           
        [CP0TR_ControlRegister_ADDRESS] = gr7;
   
// Check the correctness of the test (compare 1-th and 2-th transfer of data)               

				ar1  = begin_DMA + 2h;                      // set 1-th array address
				ar2  = ARRAY2_ADDRESS;                      // set 2-th array address
// Calculating the final check sum 
				
				ar3,gr3 = [ar1++];																		  // Read first element of array 1
				ar4,gr4 = [ar2++]; 			                                // Read first element of array 2
				ar5,gr5 = [ar1] with gr1 = gr3 xor gr4;									// Read second element of array 1, compare first elements of 1 and 2 arrays (high part)
				ar6,gr6 = [ar2]; 			                                  // Read second element of array 2
        gr3 = ar3       with gr2 = gr5 xor gr6;                 // Compare second elements of 1 and 2 arrays (high part)
        gr4 = ar4       with gr1 = gr1 or gr2;                  // Calculate partial check sum
        gr5 = ar5       with gr2 = gr3 xor gr4;                 // Compare first elements of 1 and 2 arrays (low part)             
        gr6 = ar6       with gr1 = gr1 or gr2;                  // Calculate partial check sum
        ar3,gr3 = [ar1+= 1FFAh]  with gr2 = gr5 xor gr6;			  // Read third element of array 1, compare second elements of 1 and 2 arrays (low part)
        ar4,gr4 = [ar2+= 1FFAh]  with gr1 = gr1 or gr2; 			  // Read third element of array 2, calculate partial check sum
				ar5,gr5 = [ar1 + 2h]     with gr2 = gr3 xor gr4;				// Read fourth element of array 1, compare third elements of 1 and 2 arrays (high part)
				ar6,gr6 = [ar2 + 2h]     with gr1 = gr1 or gr2; 			  // Read fourth element of array 2, calculate partial check sum
        gr3 = ar3       with gr2 = gr5 xor gr6;                 // Compare fourth elements of 1 and 2 arrays (high part)
        gr4 = ar4       with gr1 = gr1 or gr2;                  // Calculate partial check sum
        gr5 = ar5       with gr2 = gr3 xor gr4;                 // Compare third elements of 1 and 2 arrays (low part)
        gr6 = ar6       with gr1 = gr1 or gr2;                  // Calculate partial check sum
                        with gr2 = gr5 xor gr6;                 // Compare fourth elements of 1 and 2 arrays (low part)
                        with gr1 = gr1 or gr2;                  // Calculate check sum

//  Wait until Com. Port Transmition is finished         
<L2>        
      gr7 = 2h;
      gr0 = [CP0TR_ControlRegister_ADDRESS];   
      gr2 = [CP1RC_ControlRegister_ADDRESS] with gr7 = gr7 and gr0;
      with gr7 = gr7 and gr2; 
      if =0 goto L2;		
      gr7 = CP_STOP;
      [CP0TR_ControlRegister_ADDRESS] = gr7;
      [CP1RC_ControlRegister_ADDRESS] = gr7;       					  
            				  
// Check the correctness of the test (compare 1-th and 3-th arrays of data)               
             
				ar1 = begin_DMA + 2h;							    // Set address of 1-th array
				ar2 = ARRAY3_ADDRESS;							    // Set address of 3-th array
				gr2 = (8 * 1024)/ 2;								  // Set cycle count 
				with gr2 = gr2 - 1; 									// Set flags for conditional jump
// Calculating the check sum 
<L3>				
				ar3,gr3 = [ar1++];																		  
				ar4,gr4 = [ar2++]; 			                                
				ar5,gr5 = [ar1++] with gr3 = gr3 xor gr4 noflags;				
				ar6,gr6 = [ar2++] with gr1 = gr1 or gr3 noflags;				
        gr3 = ar3         with gr5 = gr5 xor gr6 noflags;	
        gr4 = ar4         with gr1 = gr1 or gr5 noflags;
        gr5 = ar5         with gr3 = gr3 xor gr4 noflags;
        gr6 = ar6         with gr1 = gr1 or gr3 noflags;
                          with gr5 = gr5 xor gr6 noflags;
                          with gr1 = gr1 or gr5 noflags;
				if > goto L3 with gr2 = gr2 - 1;			// Go to L3
												
				with gr0 = not gr1;                                     // set check sum to all ones, if test is correct
				
// Final part (reading of all registers)
				ar1 = intr;							
				gr1 = [IRRL_ADDRESS];
				ar2 = [IRRH_ADDRESS];
				gr2 = [IMRL_ADDRESS];
				ar3 = [CP0TR_MainCounter_ADDRESS];
				gr3 = [CP0TR_ADDRESS_REGISTER_ADDRESS];				
				ar4 = [CP0TR_BIAS_ADDRESS];
				gr4 = [CP0TR_RowCounter_ADDRESS];
				ar5 = [CP0TR_AddressMode_ADDRESS];
				gr5 = [CP0TR_ControlRegister_ADDRESS];
				ar6 = [CP0TR_InterruptMask_ADDRESS];
				gr6 = [CP0TR_StateRegister_ADDRESS];
								
				[ar0++] = ar1,gr1;										// Read  irrl, intr
				[ar0++] = ar2,gr2;										// Read cp rate, imrl	
// Com. Port 0 State (Transmit Part)		
				[ar0++] = ar3,gr3;										// Read ADDRESS_REGISTER, MainCounter				
				[ar0++] = ar4,gr4;										// Read RowCounter, BIAS				
				[ar0++] = ar5,gr5;										// Read ControlRegister, addressMode						
				[ar0++] = ar6,gr6;										// Read StateRegister, InterruptMask					
				
				ar1 = [CP0_PHY_CONFIG_ADDRESS];							
				gr1 = [CP0_HEM_EN_ADDRESS];
				ar2 = [CP0_ERR1_CNT_ADDRESS];
				gr2 = [CP0_ERR2_CNT_ADDRESS];
// Com. Port 0 Configuration	
				[ar0++] = ar1,gr1;										// Read CP0_HEM_EN, CP0_PHY_CONFIG
				[ar0++] = ar2,gr2;										// Read CP0_ERR2_CNT, CP0_ERR1_CNT				
								
				ar3 = [CP1RC_MainCounter_ADDRESS];
				gr3 = [CP1RC_ADDRESS_REGISTER_ADDRESS];				
				ar4 = [CP1RC_BIAS_ADDRESS];
				gr4 = [CP1RC_RowCounter_ADDRESS];
				ar5 = [CP1RC_AddressMode_ADDRESS];
				gr5 = [CP1RC_ControlRegister_ADDRESS];
				ar6 = [CP1RC_InterruptMask_ADDRESS];
				gr6 = [CP1RC_StateRegister_ADDRESS];
								
// Com. Port 1 State (Receive Part)		
				[ar0++] = ar3,gr3;										// Read ADDRESS_REGISTER, MainCounter				
				[ar0++] = ar4,gr4;										// Read RowCounter, BIAS				
				[ar0++] = ar5,gr5;										// Read ControlRegister, ddressMode						
				[ar0++] = ar6,gr6;					          // Read StateRegister, InterruptMask	

				ar1 = [CP1_PHY_CONFIG_ADDRESS];							
				gr1 = [CP1_HEM_EN_ADDRESS];
				ar2 = [CP1_ERR1_CNT_ADDRESS];
				gr2 = [CP1_ERR2_CNT_ADDRESS];
// Com. Port 1 Configuration	
				[ar0++] = ar1,gr1;										// Read CP1_HEM_EN, CP1_PHY_CONFIG
				[ar0++] = ar2,gr2;										// Read CP1_ERR2_CNT, CP1_ERR1_CNT
				
        gr1=06_0000h;				
				[IRRL_RESET_ADDRESS] = gr1;           // Reset IRRL					
				
        gr1 = [IRRL_ADDRESS];                 // Read IRRL
        [ar0++] = gr1;
        [ar0++] = gr0;        	              // Read check sum = FFFF_FFFFh, if test is correct 
				with not gr0;
				if <>0 goto Error;
				
				gr1 = 0h;				
				[CP1RC_BIAS_ADDRESS] = gr1;           // Reset Biases and Row Counters
				[CP1RC_RowCounter_ADDRESS] = gr1;
				[CP0TR_BIAS_ADDRESS] = gr1;
				[CP0TR_RowCounter_ADDRESS] = gr1;
																			
// Check the result    	
       ar0 = test_result;
	     ar1 = contr_result;
       gr0 = 34;

<Loop_Check>
	gr1 = [ar0++];
	gr2 = [ar1++];
	with gr2 - gr1;
	if <>0 goto Error;
	with gr0--;
	if <>0 goto Loop_Check;		
			
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
				
