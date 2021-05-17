// Test 9.8. DMA Controller (DMAC) Testing
// 1. load 1 arrays with data - in IMU1.
// 2. Start DMA channels three times:
//                                             (0) IMU1 -> IMU3;  
//                                             (1) IMU3 -> EXT_MEM;
//                                             (2) EXT_MEM -> IMU2.
// 3. Wait, until DMA channels finish it's work.
// 4. Compare the corresponding initial arrays and  arrays written by DMA. If all corresponding initial and obteined arrays are equal, test is
// finished successfully.

//import from macros640x;
import from "platform/rumboot.mlb";

// The first array to transmit (8K*64bits), placed in internal memory bank 1 
begin text_dma0
begin_DMA : word[2] = (0h dup 2);
				var N = 0;
				var M = 0;
.repeat 16384;
				M = (not N) and 0FFFFh;
				code (N << 16) or M;
				N = N + 1;
.endrepeat;
end text_dma0;

// The test result dimension
data data_test_res
test_result : word[33] = (-1 dup 33);
// The control value dimension
contr_result : word[33] = (
                           0h dup 2, 100h, 2_0000h, 0h dup 4,
                           0h dup 8, 
                           0h, 0300_0400h, 0h, 1_0000h, 0h dup 4,
                           0h, 0C005_0000h, 0h dup 4, 3h, 0h,
                           0ffffffffh
                            );
end data_test_res;

//const SOURCE0_ADDRESS = 0000_4000h;               // DMA Channel 0 Source Address (IMU1)
const SOURCE1_ADDRESS = 0001_0000h;                 // DMA Channel 1 Source Address (IMU3)
const SOURCE1_BYTE_ADDRESS = 0C006_0000h;           // DMA Channel 1 Source Byte Address (IMU3)
const DESTIN0_ADDRESS = 0001_0000h;                 // DMA Channel 0 Destination Address (IMU3)
const DESTIN0_BYTE_ADDRESS = 0C006_0000h;           // DMA Channel 0 Destination Byte Address (IMU3)
const DESTIN2_ADDRESS = 0000_8000h;                 // DMA Channel 2 Destination Address (IMU2)
const DESTIN2_BYTE_ADDRESS = 0C004_0000h;           // DMA Channel 2 Destination Byte Address (IMU2)
const EXT_MEM_ADDRESS = 4000_0000h;                 // Ext. Memory Address 
const EXT_MEM_BYTE_ADDRESS = 0000_0000h;            // Ext. Memory Byte Address 

const IRRL_ADDRESS = 0003_8000h;  									// External Interrupt 31-0 Request Register address  
const IRRL_SET_ADDRESS = 0003_8002h;  						  // External Interrupt 31-0 Request Register address for Bit set  
const IRRL_RESET_ADDRESS = 0003_8004h;  						// External Interrupt 31-0 Request Register address for Bit Reset  
const IMRL_ADDRESS = 0003_8008h;  									// External Interrupt 31-0 Mask Register address  
const IMRL_SET_ADDRESS = 0003_800Ah;  							// External Interrupt 31-0 Mask Register address for Bit set  
const IMRL_RESET_ADDRESS = 0003_800Ch;  						// External Interrupt 31-0 Mask Register address for Bit Reset  
const IMRL_NULL_ADDRESS = 0003_800Eh;  						  // External Interrupt 31-0 Mask Register address for All Bits Reset (Low Part)
const IRPL_ADDRESS = 0003_8010h;  									// External Interrupt 31-0 Priority Register address (Low Part)
const IRPL_SET_ADDRESS = 0003_8012h;  							// External Interrupt 31-0 Priority Register address for Bit set (Low Part)
const IRPL_RESET_ADDRESS = 0003_8014h;  						// External Interrupt 31-0 Priority Register address for Bit Reset (Low Part)
const IRPL_NULL_ADDRESS = 0003_8016h;  						  // External Interrupt 31-0 Priority Register address for All Bits Reset (Low Part)
const IASL_ADDRESS = 0003_8018h;  									// External Interrupt 31-0 Acknowledge&Status Register address (Low Part)
const IASL_RESET_ADDRESS = 0003_801Ch;  						// External Interrupt 31-0 Acknowledge&Status Register address for Bit Reset (Low Part)

const IRRH_ADDRESS = 0003_8040h;  									// External Interrupt 39-32 Request Register address (High Part)
const IRRH_SET_ADDRESS = 0003_8042h;  						  // External Interrupt 39-32 Request Register address for Bit set (High Part)
const IRRH_RESET_ADDRESS = 0003_8044h;  						// External Interrupt 39-32 Request Register address for Bit Reset (High Part)
const IMRH_ADDRESS = 0003_8048h;  									// External Interrupt 39-32 Mask Register address (High Part)
const IMRH_SET_ADDRESS = 0003_804Ah;  							// External Interrupt 39-32 Mask Register address for Bit set (High Part)
const IMRH_RESET_ADDRESS = 0003_804Ch;  						// External Interrupt 39-32 Mask Register address for Bit Reset (High Part)
const IMRH_NULL_ADDRESS = 0003_804Eh;  						  // External Interrupt 39-32 Mask Register address for All Bits Reset (High Part)
const IRPH_ADDRESS = 0003_8050h;  									// External Interrupt 39-32 Priority Register address (High Part)
const IRPH_SET_ADDRESS = 0003_8052h;  							// External Interrupt 39-32 Priority Register address for Bit set (High Part)
const IRPH_RESET_ADDRESS = 0003_8054h;  						// External Interrupt 39-32 Priority Register address for Bit Reset (High Part)
const IRPH_NULL_ADDRESS = 0003_8056h;  						  // External Interrupt 39-32 Priority Register address for All Bits Reset (High Part)
const IASH_ADDRESS = 0003_8058h;  									// External Interrupt 39-32 Acknowledge&Status Register address (High Part)
const IASH_RESET_ADDRESS = 0003_805Ch;  						// External Interrupt 39-32 Acknowledge&Status Register address for Bit Reset (High Part)

const BASE_DMAC_ADDRESS = 0300C_1800h;              // Base DMAC Address

// Addresses of DMAC Registers (Transmit Part)
const DMA_MainCounter_ADDRESS = BASE_DMAC_ADDRESS;
const DMATR_ADDRESS_REGISTER_ADDRESS = BASE_DMAC_ADDRESS + 1h;
const DMATR_BIAS_ADDRESS = BASE_DMAC_ADDRESS + 2h;
const DMATR_RowCounter_ADDRESS = BASE_DMAC_ADDRESS + 3h;
const DMATR_AddressMode_ADDRESS = BASE_DMAC_ADDRESS + 4h;
const DMA_ControlRegister_ADDRESS = BASE_DMAC_ADDRESS + 5h;

// Addresses of DMAC Registers (Receive Part)
const DMARC_ADDRESS_REGISTER_ADDRESS = BASE_DMAC_ADDRESS + 9h;
const DMARC_BIAS_ADDRESS = BASE_DMAC_ADDRESS + 0Ah;
const DMARC_RowCounter_ADDRESS = BASE_DMAC_ADDRESS + 0Bh;
const DMARC_AddressMode_ADDRESS = BASE_DMAC_ADDRESS + 0Ch;
const DMA_InterruptMask_ADDRESS = BASE_DMAC_ADDRESS + 0Eh;
const DMA_StateRegister_ADDRESS = BASE_DMAC_ADDRESS + 0Fh;

// Constants for DMAC Control
const DMA_SUSPEND = 4h;
const DMA_START = 1h;
const DMA_STOP = 0h;
const DMA_OneDimentionAddressMode = 0h;
const DMA_TwoDimentionAddressMode = 1h;
const DMA_InterruptEnable = 0h;
const DMA_InterruptDisable = 3h;

global test_9_8  : label;
    global start : label;
		 weak mytest : label;
			
            L1, L2, L3, L4, L5, L6, L7, L8 : label; 
                    L9, L10, L11, L12, L13 : label;  	
Stop_DMA0, Stop_DMA1, Stop_DMA2, Stop_DMA3 : label;
                     Normal, Error, Finish : label;
       
// The boot loader and main program (placed in memory with start address 0h (internal memory bank 0) 								 
begin ".text.init"
<test_9_8>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

				ar0 = test_result;									// Set result address					
				ar1 = [IRRL_ADDRESS];							
				gr1 = [IMRL_ADDRESS];
				ar3 = [DMA_MainCounter_ADDRESS];
				gr3 = [DMATR_ADDRESS_REGISTER_ADDRESS];				
				ar4 = [DMATR_BIAS_ADDRESS];
				gr4 = [DMATR_RowCounter_ADDRESS];
				ar5 = [DMATR_AddressMode_ADDRESS];
				gr5 = [DMA_ControlRegister_ADDRESS];
				
// Reading the DMAC Registers after System Reset			
// Reading DMAC Registers (Transmit Part)					
				[ar0++] = ar1,gr1;										// Read IMRL, IRRL
				[ar0++] = ar3,gr3;										// Read ADDRESS_REGISTER, MainCounter				
				[ar0++] = ar4,gr4;										// Read RowCounter, BIAS					
				[ar0++] = ar5,gr5;										// Read ControlRegister, AddressMode						
						
				ar3 = [IASL_ADDRESS];
				gr3 = [DMARC_ADDRESS_REGISTER_ADDRESS];				
				ar4 = [DMARC_BIAS_ADDRESS];
				gr4 = [DMARC_RowCounter_ADDRESS];
				ar5 = [DMARC_AddressMode_ADDRESS];
				gr5 = [IASL_ADDRESS];
				ar6 = [DMA_InterruptMask_ADDRESS];
				gr6 = [DMA_StateRegister_ADDRESS];
				
// Reading DMAC Registers (Receive Part) 								

				[ar0++] = ar3,gr3;										// Read ADDRESS_REGISTER, IASL					
				[ar0++] = ar4,gr4;										// Read RowCounter, BIAS					
				[ar0++] = ar5,gr5;										// Read IASL, addressMode					
				[ar0++] = ar6,gr6;										// Read StateRegister, InterruptMask		

// DMAC Initialization 0
 
// DMA Initialization (Transmit Mode)
        gr7 = begin_DMA + 3000_0002h;
        with gr7 = gr7 << 2;
        [DMATR_ADDRESS_REGISTER_ADDRESS] = gr7;
        gr7 = 8 * 1024;
        [DMA_MainCounter_ADDRESS] = gr7;
        gr7 = DMA_OneDimentionAddressMode;
        [DMATR_AddressMode_ADDRESS] = gr7;
        
// DMA Initialization (Receive Mode)
        gr7 = DESTIN0_BYTE_ADDRESS;
        [DMARC_ADDRESS_REGISTER_ADDRESS] = gr7;
        gr7 = DMA_OneDimentionAddressMode;
        [DMARC_AddressMode_ADDRESS] = gr7;
        gr7 = DMA_InterruptDisable;
        [DMA_InterruptMask_ADDRESS] = gr7;            
        
// Start DMA
        gr7 =  DMA_START;
        [DMA_ControlRegister_ADDRESS] = gr7;
    
//  Wait until DMA is finished         
<Stop_DMA0>        
        gr7 = 2h;
        gr0 = [DMA_ControlRegister_ADDRESS];   
        with gr7 = gr7 and gr0;
        if =0 goto Stop_DMA0; 
        
        gr7 = DMA_STOP;
        [DMA_ControlRegister_ADDRESS] = gr7;
//  DMAC Initialization 1

// DMA Initialization (Transmit Mode)
        gr7 = SOURCE1_BYTE_ADDRESS;
        [DMATR_ADDRESS_REGISTER_ADDRESS] = gr7;
        gr7 = 8 * 1024;
        [DMA_MainCounter_ADDRESS] = gr7;
        gr7 = DMA_OneDimentionAddressMode;
        [DMATR_AddressMode_ADDRESS] = gr7;
        
// DMA Initialization (Receive Mode)
        gr7 = EXT_MEM_BYTE_ADDRESS;
        [DMARC_ADDRESS_REGISTER_ADDRESS] = gr7;
        gr7 = DMA_OneDimentionAddressMode;
        [DMARC_AddressMode_ADDRESS] = gr7;
        gr7 = DMA_InterruptDisable;
        [DMA_InterruptMask_ADDRESS] = gr7;            
        
// Start DMA
        gr7 =  DMA_START;
        [DMA_ControlRegister_ADDRESS] = gr7;

//  Wait until DMA is finished         
<Stop_DMA1>        
        gr7 = 2h;
        gr0 = [DMA_ControlRegister_ADDRESS];   
        with gr7 = gr7 and gr0;
        if =0 goto Stop_DMA1; 
        
        gr7 = DMA_STOP;
        [DMA_ControlRegister_ADDRESS] = gr7;
         
//  DMAC Initialization 2

// DMA Initialization (Transmit Mode)
        gr7 = EXT_MEM_BYTE_ADDRESS;
        [DMATR_ADDRESS_REGISTER_ADDRESS] = gr7;
        gr7 = 8 * 1024;
        [DMA_MainCounter_ADDRESS] = gr7;
        gr7 = DMA_OneDimentionAddressMode;
        [DMATR_AddressMode_ADDRESS] = gr7;
        
// DMA Initialization (Receive Mode)
        gr7 = DESTIN2_BYTE_ADDRESS;
        [DMARC_ADDRESS_REGISTER_ADDRESS] = gr7;
        gr7 = DMA_OneDimentionAddressMode;
        [DMARC_AddressMode_ADDRESS] = gr7;
        gr7 = DMA_InterruptDisable;
        [DMA_InterruptMask_ADDRESS] = gr7;            
        
// Start DMA
        gr7 =  DMA_START;
        [DMA_ControlRegister_ADDRESS] = gr7;
        
//  Wait until DMA is finished 
<Stop_DMA2>        
        gr7 = 2h;
        gr0 = [DMA_ControlRegister_ADDRESS];   
        with gr7 = gr7 and gr0;
        if =0 goto Stop_DMA2;
        
        gr7 = DMA_STOP;
        [DMA_ControlRegister_ADDRESS] = gr7;
 
// Check the correctness of the test (compare Source 0  and Destination 0 arrays of DMA channel 0)               

				ar1 = begin_DMA + 2h;							    // Set address of Source 0 array
				ar2 = DESTIN0_ADDRESS;							  // Set address of Destination 0 array
				gr2 = (8 * 1024)/ 2;								  // Set cycle count 
				with gr2 = gr2 - 1; 									// Set flags for conditional jump
				gr1 = 0h;                             // Set initial check sum
// Calculating the check sum 
<L1>				
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
				if > goto L1 with gr2 = gr2 - 1;			// Go to L1
	
// Check the correctness of the test (compare Source 1  and Destination 1 arrays of DMA channel 1)               
			
				ar1 = SOURCE1_ADDRESS;							  // Set address of Source 1 array
				ar2 = EXT_MEM_ADDRESS;							  // Set address of Destination 1 array
				gr2 = (8 * 1024)/ 2;								  // Set cycle count 
				with gr2 = gr2 - 1; 									// Set flags for conditional jump
// Calculating the check sum 
<L2>				
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
				if > goto L2 with gr2 = gr2 - 1;			// Go to L2
				
// Check the correctness of the test (compare Source 2  and Destination 2 arrays of DMA channel 2)               

				ar1 = EXT_MEM_ADDRESS;							  // Set address of Source 2 array
				ar2 = DESTIN2_ADDRESS;							  // Set address of Destination 2 array
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

				ar1 = [IRRL_ADDRESS];							
				gr1 = intr;
				ar3 = [DMA_MainCounter_ADDRESS];
				gr3 = [DMATR_ADDRESS_REGISTER_ADDRESS];				
				ar4 = [DMATR_BIAS_ADDRESS];
				gr4 = [DMATR_RowCounter_ADDRESS];
				ar5 = [DMATR_AddressMode_ADDRESS];
				gr5 = [DMA_ControlRegister_ADDRESS];
						
// Reading DMAC Registers (Transmit Part)					
				[ar0++] = ar1,gr1;										// Read INTR, IRRL
				[ar0++] = ar3,gr3;										// Read ADDRESS_REGISTER, MainCounter				
				[ar0++] = ar4,gr4;										// Read RowCounter, BIAS					
				[ar0++] = ar5,gr5;										// Read ControlRegister, AddressMode						
						
				ar3 = [IMRL_ADDRESS];
				gr3 = [DMARC_ADDRESS_REGISTER_ADDRESS];				
				ar4 = [DMARC_BIAS_ADDRESS];
				gr4 = [DMARC_RowCounter_ADDRESS];
				ar5 = [DMARC_AddressMode_ADDRESS];
				gr5 = [IASL_ADDRESS];
				ar6 = [DMA_InterruptMask_ADDRESS];
				gr6 = [DMA_StateRegister_ADDRESS];
				
// Reading DMAC Registers (Receive Part) 								

				[ar0++] = ar3,gr3;										// Read ADDRESS_REGISTER, IMRL					
				[ar0++] = ar4,gr4;										// Read RowCounter, BIAS					
				[ar0++] = ar5,gr5;										// Read IASL, addressMode					
				[ar0++] = ar6,gr6;										// Read StateRegister, InterruptMask			

				ar7 = gr0;							
				[ar0++] = ar7;					              // Read check sum = FFFF_FFFFh, if test is correct
				with not gr0;
				if <>0 goto Error;
				
// Check the result    	
       ar0 = test_result;
	     ar1 = contr_result;
       gr0 = 33;

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
