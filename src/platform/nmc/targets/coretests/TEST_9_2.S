// Test 9.2. Timers 0 and 1 Testing

//import from macros640x;
import from "platform/rumboot.mlb";

const TMC0_ADDRESS = 0003_8800h;  									// Timer T0 address
const TMM0_ADDRESS = 0003_8802h;  									// Timer T0 Control Register address

const TMC1_ADDRESS = 0003_8804h;  									// Timer T1 address
const TMM1_ADDRESS = 0003_8806h;  									// Timer T1 Control Register address

const START_TIMER_CYCLICALLY = 11b;                 // Constant for TMMi (start Ti in multi-pass mode)
const START_TIMER = 01b;                            // Constant for TMMi (start Ti in single-pass mode)
const STOP_TIMER = 0b;                              // Constant for TMMi (stop Ti) 

const IRRL_ADDRESS = 0003_8000h;  									// External Interrupt 31-0 Request Register address  
const IRRL_SET_ADDRESS = 0003_8002h;  						  // External Interrupt 31-0 Request Register address for Bit set  
const IRRL_RESET_ADDRESS = 0003_8004h;  						// External Interrupt 31-0 Request Register address for Bit Reset  
const IMRL_ADDRESS = 0003_8008h;  									// External Interrupt 31-0 Mask Register address  
const IMRL_SET_ADDRESS = 0003_800Ah;  							// External Interrupt 31-0 Mask Register address for Bit set  
const IMRL_RESET_ADDRESS = 0003_800Ch;  						// External Interrupt 31-0 Mask Register address for Bit Reset  
const STOP_SIMULATION = 01_6800h;                   // Address to write to stop simulation

// The test result dimension
data data_test_res

test_result : word[90] = (-1 dup 90);

// The control value dimension

contr_result : word[90] = (
                            // Registers after reset
                               0 dup 4,
                            // Pass 1.
                               2h, 3h, 3h, 5h, 5h, 9h, 9h, 11h, 11h, 21h,
                               21h, 41h, 41h, 81h, 81h, 101h, 101h, 201h,
                               201h, 401h, 401h, 801h, 801h, 1001h, 1001h, 2001h,
                               2001h, 4001h, 4001h, 8001h, 8001h, 10001h, 10001h, 20001h,
                               20001h, 40001h, 40001h, 80001h, 80001h, 100001h, 100001h, 200001h,
                               200001h, 400001h, 400001h, 800001h, 800001h, 1000001h, 1000001h, 2000001h,
                               2000001h, 4000001h, 4000001h, 8000001h, 8000001h, 10000001h, 10000001h, 20000001h,
                               20000001h, 40000001h, 40000001h, 80000001h, 80000001h, 0h, 0h, 2h,
                               0300_0400h, 0h, 0300_0400h, 800h,  0300_0400h, 0C00h, 
                            // Pass 2. 
                               0300_0400h, 0h, 0FFFF_FFFEh, 0FFFF_FFFFh, 3h dup 2, 0300_0400h, 0C00h,
                               0 dup 4, 0300_0400h, 0C00h                                                                                          
                            );
save_stack : word = 0;
end data_test_res;
                  global test_9_2 : label;
                     global start : label;
	                    weak mytest : label;
	  
Loop_Check, Normal, Error, Finish : label;
  		         		 
// The boot loader	+ 	the main program (placed in memory with start address 0h -  internal memory) 						 
begin ".text.init"

<test_9_2>
<start>
<mytest>		
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);
	  
				ar0 = test_result;									    // Set result address (external memory) 
				ar1 = [TMC0_ADDRESS];										// Store timer 0 value after reset
				gr1 = [TMC1_ADDRESS];										// Store timer 1 value after reset
				ar2 = [TMM0_ADDRESS];										// Store timer 0 control register value after reset
				gr2 = [TMM1_ADDRESS];										// Store timer 1 control register value after reset				
				[ar0++] = ar1,gr1;										  // Write stored values to memory								
				[ar0++] = ar2,gr2;

	      
// 1. Write code 0...01, 0...011, ... , 1...1 to timer 0, timer 1.
//    Start timers, stop timers, store timer 0 and timer 1	values.
        gr2 = 0h;			
        [TMC0_ADDRESS] = gr2;
        gr3 = 1h;
        [TMC1_ADDRESS] = gr3;
        ar7 = START_TIMER;
        gr7 = STOP_TIMER;
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          
					
				ar1 = [TMC0_ADDRESS];										// Store timer 0 value
				gr1 = [TMC1_ADDRESS];										// Store timer 1 value

        gr4 = 1h;			
        [TMC0_ADDRESS] = gr4;
        gr5 = 3h;
        [TMC1_ADDRESS] = gr5;				     
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          

				ar2 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr2 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 3h;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 7h;
        [TMC1_ADDRESS] = gr6;        
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    
				
				ar3 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr3 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 7h;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 0Fh;
        [TMC1_ADDRESS] = gr6;				     
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    

				ar4 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr4 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 0Fh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 1Fh;
        [TMC1_ADDRESS] = gr6;				
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    

				ar5 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr5 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 1Fh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 3Fh;
        [TMC1_ADDRESS] = gr6;							      
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0 

				ar6 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr6 = [TMC1_ADDRESS];									 // Store timer 1 value

				[ar0++] = ar1,gr1;										// Write result to memory								
				[ar0++] = ar2,gr2;
				[ar0++] = ar3,gr3;
				[ar0++] = ar4,gr4;
				[ar0++] = ar5,gr5;
				[ar0++] = ar6,gr6;
				
        gr6 = 3Fh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 7Fh;
        [TMC1_ADDRESS] = gr6;			
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          
					
				ar1 = [TMC0_ADDRESS];										// Store timer 0 value
				gr1 = [TMC1_ADDRESS];										// Store timer 1 value
        gr6 = 7Fh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 0FFh;
        [TMC1_ADDRESS] = gr6;							     
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          

				ar2 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr2 = [TMC1_ADDRESS];										// Store timer 1 value
        gr6 = 0FFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 1FFh;
        [TMC1_ADDRESS] = gr6;			   
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    
				
				ar3 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr3 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 1FFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 3FFh;
        [TMC1_ADDRESS] = gr6;							    
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    

				ar4 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr4 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 3FFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 7FFh;
        [TMC1_ADDRESS] = gr6;							     
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    

				ar5 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr5 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 7FFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 0FFFh;
        [TMC1_ADDRESS] = gr6;							
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0 

				ar6 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr6 = [TMC1_ADDRESS];									 // Store timer 1 value

				[ar0++] = ar1,gr1;										// Write result to memory								
				[ar0++] = ar2,gr2;
				[ar0++] = ar3,gr3;
				[ar0++] = ar4,gr4;
				[ar0++] = ar5,gr5;
				[ar0++] = ar6,gr6;
				
        gr6 = 0FFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 1FFFh;
        [TMC1_ADDRESS] = gr6;			
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          
					
				ar1 = [TMC0_ADDRESS];										// Store timer 0 value
				gr1 = [TMC1_ADDRESS];										// Store timer 1 value
				
        gr6 = 1FFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 3FFFh;
        [TMC1_ADDRESS] = gr6;						     
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          

				ar2 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr2 = [TMC1_ADDRESS];										// Store timer 1 value
				
        gr6 = 3FFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 7FFFh;
        [TMC1_ADDRESS] = gr6;		     
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    
				
				ar3 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr3 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 7FFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 0FFFFh;
        [TMC1_ADDRESS] = gr6;						      
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    

				ar4 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr4 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 0FFFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 1FFFFh;
        [TMC1_ADDRESS] = gr6;						  
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    

				ar5 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr5 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 1FFFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 3FFFFh;
        [TMC1_ADDRESS] = gr6;							       
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0 

				ar6 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr6 = [TMC1_ADDRESS];									 // Store timer 1 value

				[ar0++] = ar1,gr1;										// Write result to memory								
				[ar0++] = ar2,gr2;
				[ar0++] = ar3,gr3;
				[ar0++] = ar4,gr4;
				[ar0++] = ar5,gr5;
				[ar0++] = ar6,gr6;
				
        gr6 = 3FFFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 7FFFFh;
        [TMC1_ADDRESS] = gr6;			
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          
					
				ar1 = [TMC0_ADDRESS];										// Store timer 0 value
				gr1 = [TMC1_ADDRESS];										// Store timer 1 value
				
        gr6 = 7FFFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 0FFFFFh;
        [TMC1_ADDRESS] = gr6;							      
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          

				ar2 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr2 = [TMC1_ADDRESS];										// Store timer 1 value

        gr6 = 0FFFFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 1FFFFFh;
        [TMC1_ADDRESS] = gr6;			     
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    
				
				ar3 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr3 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 1FFFFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 3FFFFFh;
        [TMC1_ADDRESS] = gr6;							       
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    

				ar4 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr4 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 3FFFFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 7FFFFFh;
        [TMC1_ADDRESS] = gr6;							      
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    

				ar5 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr5 = [TMC1_ADDRESS];									 // Store timer 1 value
				
        gr6 = 7FFFFFh;			
        [TMC0_ADDRESS] = gr6;
        gr6 = 0FFFFFFh;
        [TMC1_ADDRESS] = gr6;							      
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0 

				ar6 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr6 = [TMC1_ADDRESS];									 // Store timer 1 value

				[ar0++] = ar1,gr1;										// Write result to memory								
				[ar0++] = ar2,gr2;
				[ar0++] = ar3,gr3;
				[ar0++] = ar4,gr4;
				[ar0++] = ar5,gr5;
				[ar0++] = ar6,gr6;

        gr6 = 0FF_FFFFh;		
        [TMC0_ADDRESS] = gr6;
        gr6 = 1FF_FFFFh;
        [TMC1_ADDRESS] = gr6;			
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          
					
				ar1 = [TMC0_ADDRESS];										// Store timer 0 value
				gr1 = [TMC1_ADDRESS];										// Store timer 1 value

        gr6 = 1FF_FFFFh;		
        [TMC0_ADDRESS] = gr6;
        gr6 = 3FF_FFFFh;
        [TMC1_ADDRESS] = gr6;					  
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          

				ar2 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr2 = [TMC1_ADDRESS];										// Store timer 1 value

        gr6 = 3FF_FFFFh;		
        [TMC0_ADDRESS] = gr6;
        gr6 = 7FF_FFFFh;
        [TMC1_ADDRESS] = gr6;	
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    
				
				ar3 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr3 = [TMC1_ADDRESS];									 // Store timer 1 value

        gr6 = 7FF_FFFFh;		
        [TMC0_ADDRESS] = gr6;
        gr6 = 0FFF_FFFFh;
        [TMC1_ADDRESS] = gr6;					       
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 1    

				ar4 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr4 = [TMC1_ADDRESS];									 // Store timer 1 value

        gr6 = 0FFF_FFFFh;		
        [TMC0_ADDRESS] = gr6;
        gr6 = 1FFF_FFFFh;
        [TMC1_ADDRESS] = gr6;					       
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0    

				ar5 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr5 = [TMC1_ADDRESS];									 // Store timer 1 value

        gr6 = 1FFF_FFFFh;		
        [TMC0_ADDRESS] = gr6;
        gr6 = 3FFF_FFFFh;
        [TMC1_ADDRESS] = gr6;					        
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0 

				ar6 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr6 = [TMC1_ADDRESS];									 // Store timer 1 value

				[ar0++] = ar1,gr1;										// Write result to memory								
				[ar0++] = ar2,gr2;
				[ar0++] = ar3,gr3;
				[ar0++] = ar4,gr4;
				[ar0++] = ar5,gr5;
				[ar0++] = ar6,gr6;
        gr6 = 3FFF_FFFFh;		
        [TMC0_ADDRESS] = gr6;
        gr6 = 7FFF_FFFFh;
        [TMC1_ADDRESS] = gr6;	
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 0          
					
				ar1 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr1 = [TMC1_ADDRESS];									 // Store timer 1 value
				
				ar4 = intr;													   // No interrupt reguests				
				gr4 = [IRRL_ADDRESS];
				with gr4 = gr4 >>11;
				if carry goto Error;                    // If timer 0 interrupt, go to Error
				with gr4 = gr4 >>1;
				if carry goto Error;                    // If timer 1 interrupt, go to Error				
				gr4 = 0h;										
				 
        gr6 = 7FFF_FFFFh;		
        [TMC0_ADDRESS] = gr6;
        gr6 = 0FFFF_FFFFh;
        [TMC1_ADDRESS] = gr6;	
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 1 
								
				ar2 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr2 = [TMC1_ADDRESS];									 // Store timer 1 value
											
				ar5 = intr;											
				gr5 = [IRRL_ADDRESS];									 // Timer 1 interrupt request
				with gr5 = gr5 >>11;
				if carry goto Error;                    // If timer 0 interrupt, go to Error
				with gr5 = gr5 >>1;
				if not carry goto Error;                // If not timer 1 interrupt, go to Error				
				gr5 = 800h;			
				
        gr6 = 0FFFF_FFFFh;		
        [TMC0_ADDRESS] = gr6;
        gr6 = 0h;
        [TMC1_ADDRESS] = gr6;	
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 1 
								
				ar3 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr3 = [TMC1_ADDRESS];									 // Store timer 1 value			

				[ar0++] = ar1,gr1;										 // Write result to memory								
				[ar0++] = ar2,gr2;
				[ar0++] = ar3,gr3;
				[ar0++] = ar4,gr4;
				[ar0++] = ar5,gr5;
				[ar0++] = intr;		
				
				gr0 = [IRRL_ADDRESS];		
				with gr0 = gr0 >>11;
				if not carry goto Error;                // If not timer 0 interrupt, go to Error
				with gr0 = gr0 >>1;
				if not carry goto Error;                // If not timer 1 interrupt, go to Error				
				gr0 = 0C00h;			
				[ar0++] = gr0;												  // Timer 0 and timer 1 interrupt requests
        gr0 = 1100_0000_0000b;
	      [IRRL_RESET_ADDRESS] = gr0;             // Reset timer 0 and 1 external interrupts
	      
// 2. Cyclical mode of timers testing (observe on TIMER0 and TIMER1 pins)

				[ar0++] = intr;		
				gr0 = [IRRL_ADDRESS];		
				with gr0 = gr0 >>11;
				if carry goto Error;                // If timer 0 interrupt, go to Error
				with gr0 = gr0 >>1;
				if carry goto Error;                // If timer 1 interrupt, go to Error				
				gr0 = 0h;			
				[ar0++] = gr0;	        
       
        ar1 = 0FFFF_FFFBh;
        gr1 = 0FFFF_FFFAh; 
        [TMC0_ADDRESS] = ar1;                 // Timer 0 = -5
        [TMC1_ADDRESS] = gr1;                 // Timer 1 = -6	
        gr0 = START_TIMER_CYCLICALLY;         
        [TMM0_ADDRESS] = gr0;                 // Start timer 0 and timer 1 (multi-pass mode, Type = 01)
        [TMM1_ADDRESS] = gr0;        
	
// Wait 27 clock cycles
.repeat 27;                
        with gr1 = gr1 noflags;     
.endrepeat;		

				ar3 = [TMM0_ADDRESS];									// Store timer 0 state
				gr3 = [TMM1_ADDRESS];									// Store timer 1 state	

        [TMM0_ADDRESS] = gr7;                  // Stop Timer 0
        [TMM1_ADDRESS] = gr7;                  // Stop Timer 1
        
				ar1 = [TMC0_ADDRESS];									 // Store timer 0 value
				gr1 = [TMC1_ADDRESS];									 // Store timer 1 value	               
				[ar0++] = ar1,gr1;										 // Write result to memory	
				[ar0++] = ar3,gr3;				        
				[ar0++] = intr;	
								
				gr0 = [IRRL_ADDRESS];	
				with gr0 = gr0 >>11;
				if not carry goto Error;                // If not timer 0 interrupt, go to Error
				with gr0 = gr0 >>1;
				if not carry goto Error;                // If not timer 1 interrupt, go to Error				
				gr0 = 0C00h;			
				[ar0++] = gr0;												// Timer 0 and timer 1 interrupt requests        
        gr0 = 1100_0000_0000b;
	      [IRRL_RESET_ADDRESS] = gr0;           // Reset timer 0 and 1 external interrupts                        
        
        [TMC0_ADDRESS] = ar1;                 // Timer 0 = -5
        [TMC1_ADDRESS] = gr1;                 // Timer 1 = -6	
// Start timer 0 and timer 1 (single-pass  mode, Type = 01)
        [TMM0_ADDRESS] = ar7;                  // Start Timer 0
        [TMM1_ADDRESS] = ar7;                  // Start Timer 1      

// Wait 27 clock cycles
.repeat 27;                
        with gr1 = gr1 noflags;     
.endrepeat;		

				ar2 = [TMC0_ADDRESS];									// Store timer 0 value
				gr2 = [TMC1_ADDRESS];									// Store timer 1 value	
				ar3 = [TMM0_ADDRESS];									// Store timer 0 state
				gr3 = [TMM1_ADDRESS];									// Store timer 1 state	
				gr4 = [IRRL_ADDRESS];
				with gr4 = gr4 >>11;
				if not carry goto Error;                // If not timer 0 interrupt, go to Error
				with gr4 = gr4 >>1;
				if not carry goto Error;                // If not timer 1 interrupt, go to Error				
				gr4 = 0C00h;			
				ar4 = intr; 				
														 								
				[ar0++] = ar2,gr2;                    // Write result to memory
				[ar0++] = ar3,gr3;
				[ar0++] = ar4,gr4;	
							
// Check the result    	
       ar0 = test_result;
	     ar1 = contr_result;
       gr0 = 90;

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
	      goto Finish;
<Error>
				rumboot_exit(0_16800h, 1);
				//gr1 = 01h;
	      //ar0 = STOP_SIMULATION;
	      //[ar0] = gr1;	                       // Set 01h into the location 1_6800h (IM3), if the test is incorrect
	  
<Finish>
		    halt;										             // Wait (test finished)
		    
end ".text.init";
