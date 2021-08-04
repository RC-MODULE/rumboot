// Power Consumption Test for NMC4                                         

//import from macros640x;
import from "platform/rumboot.mlb";

const BASE_IM_ADDRESS = 4000h;                          // Initial Address for NMC4
const COUNT = 1000h;																		// Counter for NMC4

const BASE_SCRB_ADDRESS = 0300C_1C00h;                  // Base Address for Scraiber Controller
const SCRB_CR_ADDRESS = BASE_SCRB_ADDRESS;              // Int. Mem. Scriber Control Register address (6 bits)
const SCRB_TM_ADDRESS = BASE_SCRB_ADDRESS + 2h;         // Scriber Timer Register address (32 bits)
const SCR_ECNT1_ADDRESS = BASE_SCRB_ADDRESS + 4h;       // IM1 Scriber Error Counter  address
const SCR_ECNT2_ADDRESS = BASE_SCRB_ADDRESS + 6h;       // IM2 Scriber Error Counter address
const SCR_ECNT3_ADDRESS = BASE_SCRB_ADDRESS + 8h;       // IM3 Scriber Error Counter address
const SCR_ALLCNT_ADDRESS = BASE_SCRB_ADDRESS + 0Ch;     // All Scriber Error Counter  address
const SCR_FCNT1_ADDRESS = BASE_SCRB_ADDRESS + 10h;      // Last IM1 Scriber Error Counter address 
const SCR_FCNT2_ADDRESS = BASE_SCRB_ADDRESS + 12h;      // Last IM2 Scriber Error Counter address 
const SCR_FCNT3_ADDRESS = BASE_SCRB_ADDRESS + 14h;      // Last IM3 Scriber Error Counter address 
const SCR_FREF_ADDRESS = BASE_SCRB_ADDRESS + 18h;       // Scriber Error Threshold Register address 
const SCR_IMR_ADDRESS = BASE_SCRB_ADDRESS + 1Ch;        // Scriber Interrupt Mask Register address 
const SCR_IRR_ADDRESS = BASE_SCRB_ADDRESS + 1Eh;        // Scriber Interrupt Request Register address 

global test_11_1  : label;
    global start : label;
		 weak mytest : label;
			
      LOOP1, LOOP2 : label;
      START_PROGRAM, Normal : label;
 	       
// The boot loader and main program (placed in memory with start address 0h (internal memory bank 0) 								 
begin ".text.init"
<test_11_1>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

// IM1, IM2 and IM3 Scraiber Enable
				gr1 = -100;			
			  [SCR_FREF_ADDRESS] = gr1 with gr1 = gr1 - 1;   // set SCR_FREF						  
			  [SCR_ECNT1_ADDRESS] = gr1 with gr1 = gr1 - 1;  // set SCR_ECNT1
			  [SCR_ECNT2_ADDRESS] = gr1 with gr1 = gr1 - 1;  // set SCR_ECNT2			  
			  [SCR_ECNT3_ADDRESS] = gr1 with gr1 = gr1 - 1;  // set SCR_ECNT3				
			  [SCR_ALLCNT_ADDRESS] = gr1 with gr1 = gr1 - 1; // SCR_ALLCNT
			  [SCR_FCNT1_ADDRESS] = gr1 with gr1 = gr1 - 1;  // set SCR_FCNT1
			  [SCR_FCNT2_ADDRESS] = gr1 with gr1 = gr1 - 1;  // set SCR_FCNT2			  
			  [SCR_FCNT3_ADDRESS] = gr1 with gr1 = gr1 - 1;  // set SCR_FCNT3	
			  						  
			  gr1 = 8h;
			  [SCRB_TM_ADDRESS] = gr1;											  // set time register = 16 clock cycles
			  gr1 = 0h;
			  [SCR_IMR_ADDRESS] = gr1;											  // Disalble Interrupts		  

				gr7 = 17h;
			  [SCRB_CR_ADDRESS] = gr7;                     // IM1-IM3 Scribe Enable, Priority is Low, Scribe All Cells Mode

<START_PROGRAM>
// Initialization
        gr0 = 0h;
        ar0 = 0h;
        gr1 = 0_ffff_ffffh;
        ar1 = 0_ffff_ffffh;
        
        ar2 = BASE_IM_ADDRESS;              				// Set start address 
        gr2 = COUNT;                        				// Set cycle count
				with gr2 = gr2 - 1; 												// Set flags for conditional jump        
        
// First step of test        
<LOOP1>
				if > delayed goto LOOP1 with gr2 = gr2 - 1 ;        // Go to LOOP1
				[ar2++] = ar0,gr0 with gr4 = gr0 + gr1 noflags;			// Delayed instruction 1 (Memory_cell[i] <- 000...0)
				[ar2++] = ar1,gr1 with gr5 = gr1 + gr0 noflags; 		// Delayed instruction 2 (Memory_cell[i+1] <- FFF...F)
				
        gr2 = COUNT;                        				// Set cycle count
				with gr2 = gr2 - 1; 												// Set flags for conditional jump        
        
// Second step of test        
<LOOP2>
				if > delayed goto LOOP2 with gr2 = gr2 - 1;  			  // Go to LOOP2
				[--ar2] = ar0,gr0 with gr4 = gr0 + gr1 noflags;     // Delayed instruction 1 (Memory_cell[i+1] <- 000...0)
				[--ar2] = ar1,gr1 with gr5 = gr1 + gr0 noflags; 		// Delayed instruction 2 (Memory_cell[i] <- FFF...F)				
				        			
<Normal>   
				rumboot_exit(0_16800h, 0);
	      skip START_PROGRAM;

end ".text.init";

