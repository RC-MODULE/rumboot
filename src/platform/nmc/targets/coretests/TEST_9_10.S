// Test 9.10. Test to check the access of processor core to prohibited memory regions

//import from macros640x;
import from "platform/rumboot.mlb";

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

const CSR_ADDRESS = 3_9000h;                    // Control Register Address (AXI - System Integrator Bridge)
const IRQ_MASK_ADDRESS = 3_9002h;               // Mask Interrupt Register Address (AXI - System Integrator Bridge)
const A_READ_ADDRESS = 3_9004h;                 // Read Address Register Address (AXI - System Integrator Bridge)
const A_WRITE_ADDRESS = 3_9006h;								// Write Address Register Address (AXI - System Integrator Bridge)

// The test result dimension
data data_test_res
test_result : word[128] = (-1 dup 128);

// The control value dimension
contr_result : word[128] = (       
 														 0h,  0h, 
    											 0C0h,  0h, 7h, 0C04h,
    											 0C0h,  0h, 7h, 0C04h, 
    											   0h,  0h, 7h, 0C00h, 
    											   80h, 0h, 7h, 0C04h, 
    											   0h,  7h, 0h, 0C00h,   
    											   80h, 0h, 7h, 0C04h, 
    											   0h,  7h, 0h, 0C00h,  											   
    											   0h dup 2, 2000_0000h dup 2, 
    											   80h,  0h, 7h, 0FC3h,    											   
    											   0h dup 4, 3000_8800h dup 2, 
    											   80h,  0h, 7h, 0FC3h,
    											   0h dup 4, 3000_B400h dup 2, 
    											   80h,  0h, 7h, 0FC3h,
    											   0h dup 4, 3000_C800h dup 2, 
    											   80h,  0h,  7h, 0FC3h,   
    											   0h dup 4, 3000_F400h dup 2, 
    											   80h, 0h,  7h, 0FC3h,    											   
    											   0h dup 4, 3002_0000h dup 2, 
    											   80h,  0h,  7h, 0FC3h,    											   
    											   0h dup 4, 300C_2000h dup 2, 
    											   80h,  0h,  7h, 0FC3h, 
    											   0h dup 4, 3014_0000h dup 2, 
    											   80h,  0h,  7h, 0FC3h,  
    											   0h dup 4, 6000_0000h dup 2, 
    											   80h,  0h,  7h, 0FC3h, 
    											   0h dup 4, 8000_0000h dup 2, 
    											   80h,  0h,  7h, 0FC3h     				
                            );
end data_test_res;

// The boot loader	and main program (placed in memory with start address 0h (internal memory bank 0) 							 
begin ".text.init"

                 global test_9_10 : label;
                     global start : label;
		                  weak mytest : label;
          L1, L2, L3, L4, L5, L6  : label;
                     Next1, Next2 : label; 
Loop_Check, Normal, Error, Finish : label;
        				 
<test_9_10>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

 				ar3 = [IRQ_MASK_ADDRESS];
 				gr3 = [CSR_ADDRESS];
// 				ar4 = [A_READ_ADDRESS];
// 				gr4 = [A_WRITE_ADDRESS];
 				  
			  ar0 = test_result;									  // Set result address	
			  gr7 = 0FFFF_FFFFh;
			  ar7 = 0FFFF_FFFFh;
			   
//	Pass 1. Read instruction from	peripheral memory region
        sir = 7h;
 				[IRQ_MASK_ADDRESS] = sir;             // Enable interrupts with read/write data errors
 				sir = [IRQ_MASK_ADDRESS];
 				
// 			delayed goto 1_8000h;                 // Go to the peripheral memory region  
        code 4827_0000h;
        code 0001_8000h;
 				goto L1;                             
<L1> 				
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	
				
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IRRH_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];
 				
 				[ar0++] = ar3,gr3;                    // read CSR,  IRQ_MASK 	after reset	
// 			[ar0++] = ar4,gr4;                    // read A_WR, A_RD 	after reset		
 				[ar0++] = ar1,gr1;                    // read IRRH, IRRL
 				[ar0++] = ar2,gr2;                    // read CSR,  IRQ_MASK 				
 				
 				
        sir = 4h; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR
 				gr0 = [CSR_ADDRESS]; 	
 				sir = 80h;                             
 				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL   							
 				sir = 0C0h;                             
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS];
 				
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	 
	
// 			delayed goto 1FFF_FFFEh;             // Go to peripheral memory region  
        code 4827_0000h;
        code 1FFF_FFFEh;			
 				goto L2;                                                        
<L2> 			 				
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	
	
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IRRH_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];
 				
 				[ar0++] = ar1,gr1;                    // read IRRH, IRRL
 				[ar0++] = ar2,gr2;                    // read CSR,  IRQ_MASK 	
 							
        sir = 100b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR 	
 				gr0 = [CSR_ADDRESS]; 					 				
 				sir = 0C0h;                             
 				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                          
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL  		
 				sir = [IRRL_ADDRESS];		
		
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IRRH_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];
 				
 				[ar0++] = ar1,gr1;                    // read IRRH,IRRL
 				[ar0++] = ar2,gr2;                    // read CSR, IRQ_MASK				

//	Pass 2. Read instruction from	prohibited (reserved) memory region

// 			delayed goto 2000_0000h;                 // Go to prohibited (reserved) memory region    
        code 4827_0000h;
        code 2000_0000h;			
 				goto L3;                                                        
<L3> 			 				
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	
	
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IRRH_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];
 				
 				[ar0++] = ar1,gr1;                    // read IRRH, IRRL
 				[ar0++] = ar2,gr2;                    // read CSR,  IRQ_MASK 	
 				
        sir = 100b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR 							
  			gr0 = [CSR_ADDRESS];				
 				sir = 80h;                             
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL 
 				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL 		
 				sir = [IRRL_ADDRESS];		
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IRQ_MASK_ADDRESS];
 				ar2 = [IASL_ADDRESS];
 				gr2 = [CSR_ADDRESS];
 				
 				[ar0++] = ar1,gr1;                    // read IRQ_MASK,IRRL
 				[ar0++] = ar2,gr2;                    // read CSR,     IASL				

// 			delayed goto 8000_0000h;                 // Go to prohibited (reserved) memory region    
        code 4827_0000h;
        code 8000_0000h;			
 				goto L4;                                                        
<L4> 			 				
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	
	
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];
 				
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read CSR,  IRQ_MASK 	
 				
        sir = 100b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR 							
  			gr0 = [CSR_ADDRESS];
  			 				
 				sir = 80h; 
 				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL                             
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL 		
 				sir = [IRRL_ADDRESS];		
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IRQ_MASK_ADDRESS];
 				ar2 = [IASL_ADDRESS];
 				gr2 = [CSR_ADDRESS];
 				
 				[ar0++] = ar1,gr1;                    // read IRQ_MASK,IRRL
 				[ar0++] = ar2,gr2;                    // read CSR,     IASL			


//	Pass 3. read/write data from memory region 2000_0000h - 2FFF_FFFFh (prohibited (reserved) memory region);

         ar1 = 2000_0000h;
         gr1 = 2FFF_FFFFh;
         [ar1] = gr7;
         [gr1] = gr7;
         ar5 = [ar1];                         // Should be all zeroes
         gr5 = [gr1];                         // Should be all zeroes         
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	

        gr6 = [A_WRITE_ADDRESS];             // Should be 2000_0000h 
        ar6 = [A_READ_ADDRESS];              // Should be 2000_0000h        

 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];

        [ar0++] = ar5,gr5;                    // read 2*(all zeroes)  
        [ar0++] = ar6,gr6;                    // read 2*(2000_0000h)
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read  CSR, IRQ_MASK 				
 				 				 				
        sir = 11b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR 				
 				gr0 = [CSR_ADDRESS];
 				sir = 80h;  
				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                           
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS];

//	Pass 4. read/write data from memory region 3000_8800h - 3000_9FFEh (prohibited (reserved) memory region);

         ar1 = 3000_8800h;
         gr1 = 3000_9FFEh;
         [ar1] = ar7,gr7;
         [gr1] = ar7,gr7;
         ar4,gr4 = [ar1];                     // Should be all zeroes
         ar5,gr5 = [gr1];                     // Should be all zeroes
         
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	
         
        gr6 = [A_WRITE_ADDRESS];             // Should be 3000_8800h
        ar6 = [A_READ_ADDRESS];              // Should be 3000_8800h
                  
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];

        [ar0++] = ar4,gr4;                    // read 2*(all zeroes)
        [ar0++] = ar5,gr5;                    // read 2*(all zeroes)  
        [ar0++] = ar6,gr6;                    // read 2*(3000_8800h)
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read  CSR, IRQ_MASK 				
 				 				 				
        sir = 11b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR
 				gr0 = [CSR_ADDRESS];
 				sir = 80h;  
				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                           
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS];
 				
//	Pass 5. read/write data from memory region 3000_B400h - 3000_BFFEh (prohibited (reserved) memory region);

         ar1 = 3000_B400h;
         gr1 = 3000_BFFEh;
         [ar1] = ar7,gr7;
         [gr1] = ar7,gr7;
         ar4,gr4 = [ar1];                     // Should be all zeroes
         ar5,gr5 = [gr1];                     // Should be all zeroes
         
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	
         
         gr6 = [A_WRITE_ADDRESS];             // Should be 3000_B400h          
         ar6 = [A_READ_ADDRESS];              // Should be 3000_B400h
        
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];

        [ar0++] = ar4,gr4;                    // read 2*(all zeroes)
        [ar0++] = ar5,gr5;                    // read 2*(all zeroes)  
        [ar0++] = ar6,gr6;                    // read 2*(3000_B400h)
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read  CSR, IRQ_MASK 				
 				 				 				
        sir = 11b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR
 				gr0 = [CSR_ADDRESS];
 				sir = 80h;  
				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                           
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS];
 				 				

//	Pass 6. read/write data from memory region 3000_C800h - 3000_DFFEh (prohibited (reserved) memory region);

         ar1 = 3000_C800h;
         gr1 = 3000_DFFEh;
         [ar1] = ar7,gr7;
         [gr1] = ar7,gr7;
         ar4,gr4 = [ar1];                     // Should be all zeroes
         ar5,gr5 = [gr1];                     // Should be all zeroes
         
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	

        gr6 = [A_WRITE_ADDRESS];             // Should be 3000_C800h                  
        ar6 = [A_READ_ADDRESS];              // Should be 3000_C800h

 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];

        [ar0++] = ar4,gr4;                    // read 2*(all zeroes)
        [ar0++] = ar5,gr5;                    // read 2*(all zeroes)  
        [ar0++] = ar6,gr6;                    // read 2*(3000_C800h)
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read  CSR, IRQ_MASK 				
 				 				 				
        sir = 11b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR
 				gr0 = [CSR_ADDRESS];
 				sir = 80h;  
				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                           
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS];

//	Pass 7. read/write data from memory region 3000_F400h - 3000_FFFEh (prohibited (reserved) memory region);

         ar1 = 3000_F400h;
         gr1 = 3000_FFFEh;
         [ar1] = ar7,gr7;
         [gr1] = ar7,gr7;
         ar4,gr4 = [ar1];                     // Should be all zeroes
         ar5,gr5 = [gr1];                     // Should be all zeroes
         
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	
         
        gr6 = [A_WRITE_ADDRESS];             // Should be 3000_F400h            
        ar6 = [A_READ_ADDRESS];              // Should be 3000_F400h
      
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];

        [ar0++] = ar4,gr4;                    // read 2*(all zeroes)
        [ar0++] = ar5,gr5;                    // read 2*(all zeroes)  
        [ar0++] = ar6,gr6;                    // read 2*(3000_F400h)
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read  CSR, IRQ_MASK 				
 				 				 				
        sir = 11b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR
 				gr0 = [CSR_ADDRESS];
 				sir = 80h;  
				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                           
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS]; 				

//	Pass 8. read/write data from memory region 3002_0000h - 3003_FFFEh (prohibited (reserved) memory region);

         ar1 = 3002_0000h;
         gr1 = 3003_FFFEh;
         [ar1] = ar7,gr7;
         [gr1] = ar7,gr7;
         ar4,gr4 = [ar1];                     // Should be all zeroes
         ar5,gr5 = [gr1];                     // Should be all zeroes
         
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	
         
        gr6 = [A_WRITE_ADDRESS];             // Should be 3002_0000h                  
        ar6 = [A_READ_ADDRESS];              // Should be 3002_0000h

 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];

        [ar0++] = ar4,gr4;                    // read 2*(all zeroes)
        [ar0++] = ar5,gr5;                    // read 2*(all zeroes)  
        [ar0++] = ar6,gr6;                    // read 2*(3002_0000h)
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read  CSR, IRQ_MASK 				
 				 				 				
        sir = 11b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR
 				gr0 = [CSR_ADDRESS];
 				sir = 80h;  
				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                           
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS]; 				

//	Pass 9. read/write data from memory region 300C_2000h - 300F_FFFEh (prohibited (reserved) memory region);

         ar1 = 300C_2000h;
         gr1 = 300F_FFFEh;
         [ar1] = ar7,gr7;
         [gr1] = ar7,gr7;
         ar4,gr4 = [ar1];                     // Should be all zeroes
         ar5,gr5 = [gr1];                     // Should be all zeroes
         
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	

        gr6 = [A_WRITE_ADDRESS];             // Should be 300C_2000h                  
        ar6 = [A_READ_ADDRESS];              // Should be 300C_2000h

 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];

        [ar0++] = ar4,gr4;                    // read 2*(all zeroes)
        [ar0++] = ar5,gr5;                    // read 2*(all zeroes)  
        [ar0++] = ar6,gr6;                    // read 2*(300C_2000h)
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read  CSR, IRQ_MASK 				
 				 				 				
        sir = 11b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR
 				gr0 = [CSR_ADDRESS];
 				sir = 80h;  
				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                           
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS]; 				

//	Pass 10. read/write data from memory region 3014_0000h - 3FFF_FFFEh (prohibited (reserved) memory region);

         ar1 = 3014_0000h;
         gr1 = 3FFF_FFFEh;
         [ar1] = ar7,gr7;
         [gr1] = ar7,gr7;
         ar4,gr4 = [ar1];                     // Should be all zeroes
         ar5,gr5 = [gr1];                     // Should be all zeroes
         
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	         
          
        gr6 = [A_WRITE_ADDRESS];             // Should be 3014_0000h                 
        ar6 = [A_READ_ADDRESS];              // Should be 3014_0000h

 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];

        [ar0++] = ar4,gr4;                    // read 2*(all zeroes)
        [ar0++] = ar5,gr5;                    // read 2*(all zeroes)  
        [ar0++] = ar6,gr6;                    // read 2*(3014_0000h)
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read  CSR, IRQ_MASK 				
 				 				 				
        sir = 11b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR
 				gr0 = [CSR_ADDRESS];
 				sir = 80h;  
				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                           
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS]; 				


//	Pass 11. read/write data from memory region 6000_0000h - 7FFF_FFFEh (prohibited (reserved) memory region);

         ar1 = 6000_0000h;
         gr1 = 7FFF_FFFEh;
         [ar1] = ar7,gr7;
         [gr1] = ar7,gr7;
         ar4,gr4 = [ar1];                     // Should be all zeroes
         ar5,gr5 = [gr1];                     // Should be all zeroes
         
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	
                  
        ar6 = [A_READ_ADDRESS];              // Should be 6000_0000h
        gr6 = [A_WRITE_ADDRESS];             // Should be 6000_0000h         

 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];

        [ar0++] = ar4,gr4;                    // read 2*(all zeroes)
        [ar0++] = ar5,gr5;                    // read 2*(all zeroes)  
        [ar0++] = ar6,gr6;                    // read 2*(6000_0000h)
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read  CSR, IRQ_MASK 				
 				 				 				
        sir = 11b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR
 				gr0 = [CSR_ADDRESS];
 				sir = 80h;  
				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                           
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS]; 				

//	Pass 12. read/write data from memory region 8000_0000h - 0FFFF_FFFEh (prohibited (reserved) memory region);

         ar1 = 8000_0000h;
         gr1 = 0FFFF_FFFEh;
         [ar1] = ar7,gr7;
         [gr1] = ar7,gr7;
         ar4,gr4 = [ar1];                     // Should be all zeroes
         ar5,gr5 = [gr1];                     // Should be all zeroes
         
.repeat 20;				                 
				gr0 = sir;                            // Wait 20 clock cycles
.endrepeat;	
        
        gr6 = [A_WRITE_ADDRESS];             // Should be 8000_0000h          
        ar6 = [A_READ_ADDRESS];              // Should be 8000_0000h
        
 				ar1 = [IRRL_ADDRESS];
 				gr1 = [IASL_ADDRESS];
 				ar2 = [IRQ_MASK_ADDRESS];
 				gr2 = [CSR_ADDRESS];

        [ar0++] = ar4,gr4;                    // read 2*(all zeroes)
        [ar0++] = ar5,gr5;                    // read 2*(all zeroes)  
        [ar0++] = ar6,gr6;                    // read 2*(8000_0000h)
 				[ar0++] = ar1,gr1;                    // read IASL, IRRL
 				[ar0++] = ar2,gr2;                    // read  CSR, IRQ_MASK 				
 				 				 				
        sir = 11b; 				
 				[CSR_ADDRESS] = sir;                  // Clear error bits of CSR
 				gr0 = [CSR_ADDRESS];
 				sir = 80h;  
				[IASL_RESET_ADDRESS] = sir;           // Clear bits of IASL  				                           
 				[IRRL_RESET_ADDRESS] = sir;           // Clear bits of IRRL
 				sir = [IRRL_ADDRESS]; 				


// Check the result    	
       ar0 = test_result;
	     ar1 = contr_result;
       gr0 = 128;

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