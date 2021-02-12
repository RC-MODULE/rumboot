// Test 9.1. External Interrupt Requests Testing !!! Program only for NMC0 !!!
// 1. After reset read registers: INTR, IRR, IMR, IRP, IAS.
// 2. set all ones in IRR, in IMR (enable external interrupts), disable external interrupts in PSWR.
// 3. Read intr (internal interrupt request with interrupt address-vector), read IRR. Clear the internal interrupt request in intr by program.
// 4. Read intr with next internal interrupt request, read irr with minus one interrupt request, clear the internal interrupt request in intr.
// 5. Repeat item 4, until there are interrupt requests in irr.
// 6. Read IAS, clear IAS, clear IMR.
// 7. Write 0101...01 into IRP. Repeat 2-6. 
// 8. Write 1010...10 into IRP. Repeat 2-6.

//import from macros640x;
import from "platform/rumboot.mlb";

const STACK_ADDRESS  = 1_0000h; 									  // Top address of system stack

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

const ID_ADDRESS = 0003_8400h;                      // ID Register Address (System Controller)

const STOP_SIMULATION = 01_6800h;                   // Address to write to stop simulation

// The test result dimension
data data_test_res

test_result : word[414] = (-1 dup 414);

// The control value dimension

contr_result : word[414] = (
    // Registers after reset
       0300_0400h, 0h dup 9,  1_0000h, 333254h,
    // First pass (IRP = 0...0)
       0300_0446h, 0FFFF_FC80h, 0300_0447h, 0FFFF_FC00h, 
       0300_044Ah, 0FFFF_F800h, 0300_044Bh, 0FFFF_F000h, 0300_044Ch, 0FFFF_E000h, 0300_044Dh, 0FFFF_C000h,
       0300_044Eh, 0FFFF_8000h, 0300_044Fh, 0FFFF_0000h, 0300_0450h, 0FFFE_0000h, 0300_0451h, 0FFFC_0000h,                   
       0300_0452h, 0FFF8_0000h, 0300_0453h, 0FFF0_0000h, 0300_0454h, 0FFE0_0000h, 0300_0455h, 0FFC0_0000h,
       0300_0456h, 0FF80_0000h, 0300_0457h, 0FF00_0000h, 0300_0458h, 0FE00_0000h, 0300_0459h, 0FC00_0000h,
       0300_045Ah, 0F800_0000h, 0300_045Bh, 0F000_0000h, 0300_045Ch, 0E000_0000h, 0300_045Dh, 0C000_0000h, 
       0300_045Eh, 08000_0000h, 0300_045Fh,  0000_0000h,   
       0300_0460h,  0003_FFFEh, 0300_0461h,  0003_FFFCh, 0300_0462h,  0003_FFF8h, 0300_0463h,  0003_FFF0h,
       0300_0464h,  0003_FFE0h, 0300_0465h,  0003_FFC0h, 0300_0466h,  0003_FF80h, 0300_0467h,  0003_FF00h,
       0300_0468h,  0003_FE00h, 0300_0469h,  0003_FC00h, 0300_046Ah,  0003_F800h, 0300_046Bh,  0003_F000h,
       0300_046Ch,  0003_E000h, 0300_046Dh,  0003_C000h, 0300_046Eh,  0003_8000h, 0300_046Fh,  0003_0000h,              
       0300_0470h,  0002_0000h, 0300_0471h,  0000_0000h,       
       0300_0400h,   0h dup 3,  0FFFF_FCC0h,    3_FFFFh,   0h dup 2, 0FFFF_E080h, 01FFFFh,
       0h dup 6,
    // Second pass (IRP = 0101...01)
       0300_0447h, 0FFFF_FC40h, 0300_044Bh, 0FFFF_F440h, 0300_044Dh, 0FFFF_D440h,
       0300_044Fh, 0FFFF_5440h, 0300_0451h, 0FFFD_5440h, 0300_0453h, 0FFF5_5440h, 0300_0455h, 0FFD5_5440h,
       0300_0457h, 0FF55_5440h, 0300_0459h, 0FD55_5440h, 0300_045Bh, 0F555_5440h, 0300_045Dh, 0D555_5440h,
       0300_045Fh, 05555_5440h,
       0300_0461h, 3_FFFDh, 0300_0463h, 3_FFF5h, 0300_0465h, 3_FFD5h, 0300_0467h, 3_FF55h,
       0300_0469h, 3_FD55h, 0300_046Bh, 3_F555h, 0300_046Dh, 3_D555h, 0300_046Fh, 3_5555h, 
       0300_0471h, 1_5555h,         
       0300_0446h, 05555_5400h, 0300_044Ah, 05555_5000h, 0300_044Ch, 05555_4000h,
       0300_044Eh, 05555_0000h, 0300_0450h, 05554_0000h, 0300_0452h, 05550_0000h, 0300_0454h, 05540_0000h,
       0300_0456h, 05500_0000h, 0300_0458h, 05400_0000h, 0300_045Ah, 05000_0000h, 0300_045Ch, 04000_0000h,
       0300_045Eh, 00000_0000h,
       0300_0460h, 1_5554h, 0300_0462h, 1_5550h, 0300_0464h, 1_5540h, 0300_0466h, 1_5500h, 
       0300_0468h, 1_5400h, 0300_046Ah, 1_5000h, 0300_046Ch, 1_4000h, 0300_046Eh, 1_0000h,
       0300_0470h,      0h,       
       0300_0400h,   0h dup 3,  0FFFF_FCC0h,  3_FFFFh, 5555_5440h, 1_5555h, 0FFFF_E080h, 01FFFFh,
       0h dup 6,
    // Third pass (IRP = 1010...10)	
       0300_0446h, 0FFFF_FC80h, 0300_044Ah, 0FFFF_F880h, 0300_044Ch, 0FFFF_E880h, 0300_044Eh, 0FFFF_A880h, 
       0300_0450h, 0FFFE_A880h, 0300_0452h, 0FFFA_A880h, 0300_0454h, 0FFEA_A880h, 0300_0456h, 0FFAA_A880h,
       0300_0458h, 0FEAA_A880h, 0300_045Ah, 0FAAA_A880h, 0300_045Ch, 0EAAA_A880h, 0300_045Eh, 0AAAA_A880h, 
       0300_0460h, 3_FFFEh, 0300_0462h, 3_FFFAh, 0300_0464h,3_FFEAh, 0300_0466h, 3_FFAAh,  
       0300_0468h, 3_FEAAh, 0300_046Ah, 3_FAAAh, 0300_046Ch,3_EAAAh, 0300_046Eh, 3_AAAAh, 
       0300_0470h, 2_AAAAh,          
       0300_0447h, 0AAAA_A800h, 0300_044Bh, 0AAAA_A000h, 0300_044Dh, 0AAAA_8000h, 0300_044Fh, 0AAAA_0000h,
       0300_0451h, 0AAA8_0000h, 0300_0453h, 0AAA0_0000h, 0300_0455h, 0AA80_0000h, 0300_0457h, 0AA00_0000h,
       0300_0459h, 0A800_0000h, 0300_045Bh, 0A000_0000h, 0300_045Dh, 08000_0000h, 0300_045Fh, 00000_0000h,    
			 0300_0461h, 2_AAA8h, 0300_0463h, 2_AAA0h, 0300_0465h, 2_AA80h, 0300_0467h, 2_AA00h,
			 0300_0469h, 2_A800h, 0300_046Bh, 2_A000h, 0300_046Dh, 2_8000h, 0300_046Fh, 2_0000h,
			 0300_0471h,      0h,
       0300_0400h,   0h dup 3,  0FFFF_FCC0h,  3_FFFFh, 0AAAA_A880h, 2_AAAAh, 0FFFF_E080h, 01FFFFh,
       0h dup 6,			 
		// Fourth pass (IRP = 1111...1)	
       0300_0446h, 0FFFF_FC80h, 0300_0447h, 0FFFF_FC00h, 
       0300_044Ah, 0FFFF_F800h, 0300_044Bh, 0FFFF_F000h, 0300_044Ch, 0FFFF_E000h, 0300_044Dh, 0FFFF_C000h,
       0300_044Eh, 0FFFF_8000h, 0300_044Fh, 0FFFF_0000h, 0300_0450h, 0FFFE_0000h, 0300_0451h, 0FFFC_0000h,                   
       0300_0452h, 0FFF8_0000h, 0300_0453h, 0FFF0_0000h, 0300_0454h, 0FFE0_0000h, 0300_0455h, 0FFC0_0000h,
       0300_0456h, 0FF80_0000h, 0300_0457h, 0FF00_0000h, 0300_0458h, 0FE00_0000h, 0300_0459h, 0FC00_0000h,
       0300_045Ah, 0F800_0000h, 0300_045Bh, 0F000_0000h, 0300_045Ch, 0E000_0000h, 0300_045Dh, 0C000_0000h, 
       0300_045Eh, 08000_0000h, 0300_045Fh,  0000_0000h,   
       0300_0460h,  0003_FFFEh, 0300_0461h,  0003_FFFCh, 0300_0462h,  0003_FFF8h, 0300_0463h,  0003_FFF0h,
       0300_0464h,  0003_FFE0h, 0300_0465h,  0003_FFC0h, 0300_0466h,  0003_FF80h, 0300_0467h,  0003_FF00h,
       0300_0468h,  0003_FE00h, 0300_0469h,  0003_FC00h, 0300_046Ah,  0003_F800h, 0300_046Bh,  0003_F000h,
       0300_046Ch,  0003_E000h, 0300_046Dh,  0003_C000h, 0300_046Eh,  0003_8000h, 0300_046Fh,  0003_0000h,              
       0300_0470h,  0002_0000h, 0300_0471h,  0000_0000h,                     
       0300_0400h,   0h dup 3, 0FFFF_FCC0h,  3_FFFFh, 0FFFF_FCC0h, 3_FFFFh, 0FFFF_E080h, 01FFFFh,
       0AAAA_5440h, 2_5555h, 0h dup 6			 
                            );

end data_test_res;

// The boot loader	and main program (placed in memory with start address 0h (IMU1) 							 
begin ".text.init"

      global test_9_1 : label;
         global start : label;
		      weak mytest : label;
		
 		       Loop_Check : label; 	
Normal, Error, Finish : label;
        				 
<test_9_1>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);
				ar7 = STACK_ADDRESS;
			  				
				ar0 = test_result;									  // Set result address	
				ar1 = intr;							
				gr1 = pswr;
				ar2 = [IRRL_ADDRESS];
				gr2 = [IRRH_ADDRESS];
				ar3 = [IMRL_ADDRESS];
				gr3 = [IMRH_ADDRESS];				
				ar4 = [IRPL_ADDRESS];
				gr4 = [IRPH_ADDRESS];
				ar5 = [IASL_ADDRESS];
				gr5 = [IASH_ADDRESS];
				gr7 = [ID_ADDRESS];
				
				[ar0++] = ar1,gr1;										// Read  pswr, intr
				[ar0++] = ar2,gr2;										// Read irrh, irrl				
				[ar0++] = ar3,gr3;										// Read imrh, imrl				
				[ar0++] = ar4,gr4;										// Read irrh, irrl				
				[ar0++] = ar5,gr5;										// Read imrh, imrl
				[ar0++] = ar7,gr7;                    // Read id, ar7
												
// First pass (IRP = 0...0)								
				pswr clear 40h;												// Disable external interrupts in processor core
				gr7 = 0FFFF_FFFFh;
				[IRRH_SET_ADDRESS] = gr7;				      // set external interrupts (high part)
				[IRRL_SET_ADDRESS] = gr7;             // set external interrupts (low part)
				
				gr7 = 0FFFF_FFFFh;
				[IMRL_SET_ADDRESS] = gr7;             // Enable external interrupts (low part)				
				[IMRH_SET_ADDRESS] = gr7;				      // Enable external interrupts (high part)
								
.repeat 20;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;

.repeat 24;
				ar1 = intr;							
				gr1 = [IRRL_ADDRESS];
.repeat 12;																		// Wait 12 clock cycles
				nul 0h;
.endrepeat;				
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 12 clock cycles
				nul 0h;
.endrepeat;						
        [ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

.repeat 18;
				ar1 = intr;							
				gr1 = [IRRH_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;				
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;						
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

				ar1 = intr;							
				gr1 = pswr;
				ar2 = [IRRL_ADDRESS];
				gr2 = [IRRH_ADDRESS];
				ar3 = [IMRL_ADDRESS];
				gr3 = [IMRH_ADDRESS];				
				ar4 = [IRPL_ADDRESS];
				gr4 = [IRPH_ADDRESS];
				ar5 = [IASL_ADDRESS];
				gr5 = [IASH_ADDRESS];
				
				[ar0++] = ar1,gr1;										// Read  pswr, intr
				[ar0++] = ar2,gr2;										// Read irrh, irrl				
				[ar0++] = ar3,gr3;										// Read imrh, imrl				
				[ar0++] = ar4,gr4;										// Read irph, irpl				
				[ar0++] = ar5,gr5;										// Read iash, iasl	
				
				gr7 = 0h;
				[IMRH_NULL_ADDRESS] = gr7;            // Disable external interrupts (high part)
				[IMRL_NULL_ADDRESS] = gr7;            // Disable internal interrupts (low part)
        gr7 = 1111_1111_1111_1111_1110_0000_1000_0000b; 
        [IRRL_SET_ADDRESS] = gr7;
        [IASL_RESET_ADDRESS] = gr7;
        gr7 = 0000_0000_0000_0001_1111_1111_1111_1111b; 
        [IRRH_SET_ADDRESS] = gr7;
        [IASH_RESET_ADDRESS] = gr7;
				ar2 = [IRRL_ADDRESS];
				gr2 = [IRRH_ADDRESS]; 
				ar3 = [IMRL_ADDRESS];
				gr3 = [IMRH_ADDRESS];	       
				ar5 = [IASL_ADDRESS];
				gr5 = [IASH_ADDRESS];
				[ar0++] = ar2,gr2;										// Read irrh = 0, irrl = 0!		
				[ar0++] = ar3,gr3;										// Read imrh = 0, imrl = 0!								
				[ar0++] = ar5,gr5;										// Read iash = 0, iasl = 0!

// Second pass (IRP = 0101...01)								
        gr7 = 5555_5555h;
        [IRPH_SET_ADDRESS] = gr7;             // set IRPH
        [IRPL_SET_ADDRESS] = gr7;             // set IRPL
				gr7 = 0FFFF_FFFFh;
				[IRRH_SET_ADDRESS] = gr7;				      // set external interrupts (high part)
				[IRRL_SET_ADDRESS] = gr7;             // set external interrupts (low part)
				
				gr7 = 0FFFF_FFFFh;
				[IMRL_ADDRESS] = gr7;                 // Enable external interrupts (low part)				
				[IMRH_ADDRESS] = gr7;				          // Enable external interrupts (high part)		
						
.repeat 20;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;

.repeat 12;
				ar1 = intr;							
				gr1 = [IRRL_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

.repeat 9;
				ar1 = intr;							
				gr1 = [IRRH_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

.repeat 12;
				ar1 = intr;							
				gr1 = [IRRL_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

.repeat 9;
				ar1 = intr;							
				gr1 = [IRRH_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

				ar1 = intr;							
				gr1 = pswr;
				ar2 = [IRRL_ADDRESS];
				gr2 = [IRRH_ADDRESS];
				ar3 = [IMRL_ADDRESS];
				gr3 = [IMRH_ADDRESS];				
				ar4 = [IRPL_ADDRESS];
				gr4 = [IRPH_ADDRESS];
				ar5 = [IASL_ADDRESS];
				gr5 = [IASH_ADDRESS];
				
				[ar0++] = ar1,gr1;										// Read  pswr, intr
				[ar0++] = ar2,gr2;										// Read irrh, irrl				
				[ar0++] = ar3,gr3;										// Read imrh, imrl				
				[ar0++] = ar4,gr4;										// Read irph, irpl				
				[ar0++] = ar5,gr5;										// Read iash, iasl	

				gr7 = 0FFFF_FFFFh;
				[IMRH_RESET_ADDRESS] = gr7;           // Disable external interrupts (high part)
				[IMRL_RESET_ADDRESS] = gr7;           // Disable internal interrupts (low part)
        gr7 = 1111_1111_1111_1111_1110_0000_1000_0000b; 
        [IRRL_SET_ADDRESS] = gr7;
        [IASL_RESET_ADDRESS] = gr7;
        gr7 = 0000_0000_0000_0001_1111_1111_1111_1111b; 
        [IRRH_SET_ADDRESS] = gr7;
        [IASH_RESET_ADDRESS] = gr7;
				ar2 = [IRRL_ADDRESS];
				gr2 = [IRRH_ADDRESS]; 
				ar3 = [IMRL_ADDRESS];
				gr3 = [IMRH_ADDRESS];	       
				ar5 = [IASL_ADDRESS];
				gr5 = [IASH_ADDRESS];
				[ar0++] = ar2,gr2;										// Read irrh = 0, irrl = 0!		
				[ar0++] = ar3,gr3;										// Read imrh = 0, imrl = 0!								
				[ar0++] = ar5,gr5;										// Read iash = 0, iasl = 0!

// Third pass (IRP = 1010...10)	
        gr7 = 0h;
        [IRPH_NULL_ADDRESS]	= gr7;            // Reset IRPH
        [IRPL_NULL_ADDRESS]	= gr7;        		// Reset IRPL				
        gr7 = 0AAAA_AAAAh;
        [IRPH_ADDRESS] = gr7;                 // set IRPH
        [IRPL_ADDRESS] = gr7;                 // set IRPL
				gr7 = 0FFFF_FFFFh;
				[IRRH_SET_ADDRESS] = gr7;				      // set external interrupts (high part)
				[IRRL_SET_ADDRESS] = gr7;             // set external interrupts (low part)
				
				gr7 = 0FFFF_FFFFh;
				[IMRL_ADDRESS] = gr7;                 // Enable external interrupts (low part)				
				[IMRH_ADDRESS] = gr7;				          // Enable external interrupts (high part)			
					
.repeat 20;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;

.repeat 12;
				ar1 = intr;							
				gr1 = [IRRL_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

.repeat 9;
				ar1 = intr;							
				gr1 = [IRRH_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

.repeat 12;
				ar1 = intr;							
				gr1 = [IRRL_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

.repeat 9;
				ar1 = intr;							
				gr1 = [IRRH_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

				ar1 = intr;							
				gr1 = pswr;
				ar2 = [IRRL_ADDRESS];
				gr2 = [IRRH_ADDRESS];
				ar3 = [IMRL_ADDRESS];
				gr3 = [IMRH_ADDRESS];				
				ar4 = [IRPL_ADDRESS];
				gr4 = [IRPH_ADDRESS];
				ar5 = [IASL_ADDRESS];
				gr5 = [IASH_ADDRESS];
				
				[ar0++] = ar1,gr1;										// Read  pswr, intr
				[ar0++] = ar2,gr2;										// Read irrh, irrl				
				[ar0++] = ar3,gr3;										// Read imrh, imrl				
				[ar0++] = ar4,gr4;										// Read irph, irpl				
				[ar0++] = ar5,gr5;										// Read iash, iasl	

				gr7 = 0FFFF_FFFFh;
				[IMRH_NULL_ADDRESS] = gr7;           // Disable external interrupts (high part)
				[IMRL_NULL_ADDRESS] = gr7;           // Disable internal interrupts (low part)
        gr7 = 1111_1111_1111_1111_1110_0000_1000_0000b; 
        [IRRL_SET_ADDRESS] = gr7;
        [IASL_RESET_ADDRESS] = gr7;
        gr7 = 0000_0000_0000_0001_1111_1111_1111_1111b; 
        [IRRH_SET_ADDRESS] = gr7;
        [IASH_RESET_ADDRESS] = gr7;
				ar2 = [IRRL_ADDRESS];
				gr2 = [IRRH_ADDRESS]; 
				ar3 = [IMRL_ADDRESS];
				gr3 = [IMRH_ADDRESS];	       
				ar5 = [IASL_ADDRESS];
				gr5 = [IASH_ADDRESS];
				[ar0++] = ar2,gr2;										// Read irrh = 0, irrl = 0!		
				[ar0++] = ar3,gr3;										// Read imrh = 0, imrl = 0!								
				[ar0++] = ar5,gr5;										// Read iash = 0, iasl = 0!

// Fourth pass (IRP = 1111...1)	
        gr7 = 0AAAA_AAAAh;
        [IRPH_RESET_ADDRESS]	= gr7;          // Reset IRPH
        [IRPL_RESET_ADDRESS]	= gr7;          // Reset IRPL				
        gr7 = 0FFFF_FFFFh;
        [IRPH_SET_ADDRESS] = gr7;             // set IRPH
        [IRPL_SET_ADDRESS] = gr7;             // set IRPL
				[IRRH_SET_ADDRESS] = gr7;				      // set external interrupts (high part)
				[IRRL_SET_ADDRESS] = gr7;             // set external interrupts (low part)
				[IMRL_ADDRESS] = gr7;                 // Enable external interrupts (low part)					
				[IMRH_ADDRESS] = gr7;				          // Enable external interrupts (high part)
							
.repeat 20;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;

.repeat 24;
				ar1 = intr;							
				gr1 = [IRRL_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

.repeat 18;
				ar1 = intr;							
				gr1 = [IRRH_ADDRESS];
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				intr clear 40h;												// Clear external interrupt request
.repeat 12;																		// Wait 20 clock cycles
				nul 0h;
.endrepeat;					
				[ar0++] = ar1,gr1;										// Read previous intr, irr
.endrepeat;

				ar1 = intr;							
				gr1 = pswr;
				ar2 = [IRRL_ADDRESS];
				gr2 = [IRRH_ADDRESS];
				ar3 = [IMRL_ADDRESS];
				gr3 = [IMRH_ADDRESS];				
				ar4 = [IRPL_ADDRESS];
				gr4 = [IRPH_ADDRESS];
				ar5 = [IASL_ADDRESS];
				gr5 = [IASH_ADDRESS];
				
				[ar0++] = ar1,gr1;										// Read  pswr, intr
				[ar0++] = ar2,gr2;										// Read irrh, irrl				
				[ar0++] = ar3,gr3;										// Read imrh, imrl				
				[ar0++] = ar4,gr4;										// Read irph, irpl				
				[ar0++] = ar5,gr5;										// Read iash, iasl	

				gr7 = 0FFFF_FFFFh;
				[IMRH_NULL_ADDRESS] = gr7;           // Disable external interrupts (high part)
				[IMRL_NULL_ADDRESS] = gr7;           // Disable internal interrupts (low part)
        gr7 = 1111_1111_1111_1111_1110_0000_1000_0000b; 
        [IRRL_SET_ADDRESS] = gr7;
        [IASL_RESET_ADDRESS] = gr7;
        gr7 = 0000_0000_0000_0001_1111_1111_1111_1111b;  
        [IRRH_SET_ADDRESS] = gr7;
        [IASH_RESET_ADDRESS] = gr7;
        
        gr7 = 0AAAA_5555h;
				[IRRH_SET_ADDRESS] = gr7;				      // set external interrupts (high part)
				[IRRL_SET_ADDRESS] = gr7;             // set external interrupts (low part)
				
				ar2 = [IRRL_ADDRESS];
				gr2 = [IRRH_ADDRESS]; 
				ar3 = [IMRL_ADDRESS];
				gr3 = [IMRH_ADDRESS];	       
				ar5 = [IASL_ADDRESS];
				gr5 = [IASH_ADDRESS];
				
				[IRRH_RESET_ADDRESS] = gr7;				    // Reset external interrupts (high part)
				[IRRL_RESET_ADDRESS] = gr7;           // Reset external interrupts (low part)
				ar6 = [IRRL_ADDRESS];
				gr6 = [IRRH_ADDRESS]; 							
				
				[ar0++] = ar2,gr2;										// Read irrh, irrl		
				[ar0++] = ar3,gr3;										// Read imrh = 0, imrl = 0!								
				[ar0++] = ar5,gr5;										// Read iash = 0, iasl = 0!
				[ar0++] = ar6,gr6;										// Read irrh = 0, irrl = 0!	

// Check the result    	
       ar0 = test_result;
	     ar1 = contr_result;
       gr0 = 414;

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
				
