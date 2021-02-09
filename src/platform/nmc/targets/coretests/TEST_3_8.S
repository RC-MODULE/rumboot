// Test 3.8. Address ALU (AU3) for Delta between Neighbour Calls Testing
// All Address Methods of Calls Testing
// The result of the test you can see only on shared memory

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test
// The control value dimension 
contr_result : long[40] = (
  //  Data    Address
  0d54_1000_0000_A555hl,
 0FAA9_9000_0000_BAABhl,
 0F800_9000_0000_AFFFhl,  
  02AA_1000_0000_AAABhl,  
 0FD54_9000_0000_B555hl,
 0F7FF_9000_0000_B001hl,
  07FF_1000_0000_A001hl, 
 0FFFF_9000_0000_BFFFhl, 
  0A99_1000_0000_A021hl,  
 0F70E_9000_0000_B203hl,
  0111_1000_0000_AFDFhl,    
 0F8F0_9000_0000_BDFDhl,  
  0BC2_1000_0000_A7F7hl, 
 0FFFF_9000_0000_BF7Dhl,
 0FFFF_9000_0000_AFBDhl,
 0F9E0_9000_0000_BBFBhl, 
  0683_1000_0000_AEF3hl, 
 0F782_9000_0000_BFEDhl,  
  07FF_1000_0000_AFEDhl,   
 0F87C_9000_0000_BEF3hl,    
  077B_1000_0000_AFFBhl,  
 0F802_9000_0000_BFF5hl, 
  07FF_1000_0000_A005hl,  
 0F7FC_9000_0000_B00Bhl,  
  0404_1000_0000_A801hl,   
 0FBF8_9000_0000_B08Fhl,  
  0806_1000_0000_A081hl,   
 0F438_9000_0000_B80Fhl,  
  03C6_1000_0000_A881hl,     
 0FFFF_9000_0000_B00Fhl,   
 0FFFA_9000_0000_A00Bhl,     
 0FFBF_9000_0000_A08Bhl,    
 0F842_9000_0000_B005hl,   
  07FF_1000_0000_A005hl,  
  07FF_1000_0000_A007hl,    
 0F7FF_9000_0000_B007hl,   
  07FF_1000_0000_A007hl,    
 0F7FF_9000_0000_B007hl,     
  0801_1000_0000_A003hl, 
 0F7FF_9000_0000_B003hl);
  
end data_test;

global test_3_8 : label;
   global start : label;
		weak mytest : label;
		
Loop, Normal, Error, Finish : label;

M1, M2, M3, M4, M5, M6, M7, M8, M9, M10 : label;
M11, M12, M13, M14, M15, M16, M17, M18, M19, M20 : label;
M21, M22, M23, M24, M25, M26, M27, M28, M29, M30 : label;
M31, M32, M33, M34, M35, M36, M37, M38, M39, M40 : label;
M41, M42, M43, M44, M45, M46, M47, M48, M49, M50 : label;
M51, M52, M53, M54, M55, M56, M57, M58, M59, M60 : label;
M61, M62, M63, M64, M65, M66, M67, M68, M69, M70 : label;
M71, M72, M73, M74, M75, M76, M77, M78, M79, M80 : label;
M81, M82, M83, M84, M85, M86, M87, M88, M89, M90 : label;
          M91, M92, M93, M94, M95, M96, M97, M98 : label;

// The main program								 
begin ".text.init"
<test_3_1>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

						pswr set 1000h;													// Set bit EN
						gr7 = 0AAAAh;
						
// Delta Testing (AU3 Testing) 
// 1-th step
						ar7 = 0_A000h;
						call M1;															
						goto Error;

<M1>				ar7 = 0_BFFEh;						
						callrel 6;														
						goto Error;
						
// 2-th step
<M2>				ar7 = 0_A554h;
						ar0 = M3;
						gr0 = 0FFFF_FFFFh;
						call ar0;															
						goto Error;

<M3>				ar7 = 0_BAAAh;	
						ar1 = 100h;
						gr1 = M4;					
						call gr1;															
						goto Error;						

// 3-th step
<M4>				ar7 = 0_AFFEh;
						ar2 = M5;
						gr2 = 0h;
						call ar2 + gr2;												
						goto Error;

<M5>				ar7 = 0_B000h;	
						ar3 = 1FFh;
						gr3 = 6;					
						callrel gr3;											
						goto Error;		

// 4-th step
<M6>				ar7 = 0_AAAAh;
						ar4 = M7 - 8;
						gr4 = 0FFFFh;
						call ar4 + 8;											
						goto Error;

<M7>				ar7 = 0_B554h;	
						ar5 = M8 - 10;
						gr5 = 10;					
						call ar5 + gr5;										
						goto Error;		

// 5-th step
<M8>				ar7 = 0_A000h;
						ar6 = M9 - 16h;
						gr6 = 5555h;
						call ar6 + 16h;										
						goto Error;

<M9>				ar7 = 0_A000h;	
						ar6 = M10 + 10;
						gr6 = -10;					
						call ar6 + gr6;										
						goto Error;		

// 6-th step
<M10>				ar7 = 0_BFFEh;
						call M11;													
						goto Error;

<M11>				ar7 = 0_BFFEh;					
						call M12;													
						goto Error;		

// 7-th step
<M12>				ar7 = 0_A554h;
						call M13;															
						goto Error;

<M13>				ar7 = 0_B000h;					
						call M14;															
						goto Error;		

// 8-th step
<M14>				ar7 = 0_AAAAh;
						call M15;															
						goto Error;

<M15>				ar7 = 0_B000h;					
						call M16;															
						goto Error;		

// 9-th step
<M16>				ar7 = 0_AFFEh;
						call M17;															
						goto Error;

<M17>				ar7 = 0_BAAAh;					
						call M18;															
						goto Error;		

// 10-th step
<M18>				ar7 = 0_AFFEh;
						call M19;															
						goto Error;

<M19>				ar7 = 0_B554h;					
						call M20;															
						goto Error;		

// 11-th step
<M20>				ar7 = 0_A020h;
						call M21;															
						goto Error;

<M21>				ar7 = 0_B202h;					
						call M22;														
						goto Error;		

// 12-th step
<M22>				ar7 = 0_AFDEh;
						call M23;														
						goto Error;

<M23>				ar7 = 0_BDFCh;					
						call M24;														
						goto Error;		

// 13-th step
<M24>				ar7 = 0_A7F6h;
						call M25;														
						goto Error;

<M25>				ar7 = 0_BF7Ch;					
						call M26;														
						goto Error;		

// 14-th step
<M26>				ar7 = 0_BF7Ch;
						call M27;														
						goto Error;

<M27>				ar7 = 0_A7F6h;					
						call M28;														
						goto Error;		

// 15-th step
<M28>				ar7 = 0_BBFAh;
						call M29;														
						goto Error;

<M29>				ar7 = 0_AFBCh;					
						call M30;															
						goto Error;		

// 16-th step
<M30>				ar7 = 0_AFBCh;
						call M31;															
						goto Error;

<M31>				ar7 = 0_BBFAh;					
						call M32;															
						goto Error;		

// 17-th step
<M32>				ar7 = 0_AEF2h;
						call M33;															
						goto Error;

<M33>				ar7 = 0_BFECh;					
						call M34;															
						goto Error;		

// 18-th step
<M34>				ar7 = 0_AFECh;
						call M35;															
						goto Error;

<M35>				ar7 = 0_BEF2h;					
						call M36;															
						goto Error;		

// 19-th step
<M36>				ar7 = 0_AFFAh;
						call M37;															
						goto Error;

<M37>				ar7 = 0_BFF4h;					
						call M38;															
						goto Error;		

// 20-th step
<M38>				ar7 = 0_A004h;
						call M39;															
						goto Error;

<M39>				ar7 = 0_B00Ah;					
						call M40;															
						goto Error;		

// 21-th step
<M40>				ar7 = 0_A800h;
						call M41;															
						goto Error;

<M41>				ar7 = 0_B08Eh;					
						call M42;															
						goto Error;		

// 22-th step
<M42>				ar7 = 0_A080h;
						call M43;															
						goto Error;

<M43>				ar7 = 0_B80Eh;					
						call M44;															
						goto Error;		

// 23-th step
<M44>				ar7 = 0_A880h;
						call M45;															
						goto Error;

<M45>				ar7 = 0_B00Eh;					
						call M46;															
						goto Error;		

// 24-th step
<M46>				ar7 = 0_B00Eh;
						call M47;															
						goto Error;

<M47>				ar7 = 0_A880h;					
						call M48;															
						goto Error;		

// 25-th step
<M48>				ar7 = 0_B08Eh;
						call M49;															
						goto Error;

<M49>				ar7 = 0_A000h;					
						call M50;															
						goto Error;		

// 26-th step
<M50>				ar7 = 0_A00Ah;
						call M51;															
						goto Error;

<M51>				ar7 = 0_A08Ah;					
						call M52;															
						goto Error;		

// 27-th step
<M52>				ar7 = 0_B004h;
						call M53;															 
						goto Error;

<M53>				ar7 = 0_A004h;					
						call M54;																
						goto Error;		

// 28-th step
<M54>				ar7 = 0_A006h;
						call M55;															 
						goto Error;

<M55>				ar7 = 0_B006h;					
						call M56;																
						goto Error;		

// 29-th step
<M56>				ar7 = 0_A006h;
						call M57;															 
						goto Error;

<M57>				ar7 = 0_B006h;					
						call M58;																
						goto Error;		

// 30-th step
<M58>				ar7 = 0_A002h;
						call M59;															 
						goto Error;

<M59>				ar7 = 0_B002h;					
						call M60;																
						goto Error;		

// DEL_VALID Testing (DEL_VALID = SSH_VALID & DEL<31> & DEL<30> & ... & DEL<17>)
<M60>				pswr clear 1000h;												// Reset bit EN
// 1-th step
						ar7 = 0_A000h;
						call M61;															
						goto Error;

<M61>				pswr set 1000h;													// Set bit EN
						ar7 = 0_BFFEh;					
						call M62;																
						goto Error;					

// 2-th step
<M62>				ar7 = 0_A000h;
						call M63;															 	
						goto Error;

<M63>				ar7 = 0_AFFEh;					
						call M64;																
						goto Error;					

// 3-th step
<M64>				ar7 = 0_BFFEh;
						call M65;															 	
						goto Error;

<M65>				ar7 = 0_BFFEh;					
						call M66;																
						goto Error;					

// 4-th step
<M66>				ar7 = 0_A000h;
						call M67;															 	
						goto Error;

<M67>				ar7 = 0_A000h;					
						call M68;																
						goto Error;					

// 5-th step
<M68>				ar7 = 0_B000h;
						call M69;															
						goto Error;

<M69>				ar7 = 0_A000h;					
						call M70;															
						goto Error;					

// 6-th step
<M70>				ar7 = 0_B000h;
						call M71;															
						goto Error;

<M71>				ar7 = 0_A000h;					
						call M72;															
						goto Error;					

// 7-th step
<M72>				ar7 = 0_B000h;
						call M73;															
						goto Error;

<M73>				ar7 = 0_A000h;					
						call M74;															
						goto Error;					

// 8-th step
<M74>				ar7 = 0_B000h;
						call M75;															
						goto Error;

<M75>				ar7 = 0_B000h;					
						call M76;															
						goto Error;					

// 9-th step
<M76>				ar7 = 0_A000h;
						call M77;														
						goto Error;

<M77>				ar7 = 0_B000h;					
						call M78;														
						goto Error;					

// 10-th step
<M78>				ar7 = 0_A000h;
						call M79;														
						goto Error;

<M79>				ar7 = 0_A000h;					
						call M80;														
						goto Error;					

// 11-th step
<M80>				ar7 = 0_B000h;
						call M81;														
						goto Error;

<M81>				ar7 = 0_A000h;					
						call M82;														
						goto Error;					

// 12-th step
<M82>				ar7 = 0_B000h;
						call M83;														
						goto Error;

<M83>				ar7 = 0_A000h;					
						call M84;															
						goto Error;					

// 13-th step
<M84>				ar7 = 0_B000h;
						call M85;															
						goto Error;

<M85>				ar7 = 0_A000h;					
						call M86;															
						goto Error;					

// 14-th step
<M86>				ar7 = 0_B000h;
						call M87;															
						goto Error;

<M87>				ar7 = 0_A000h;					
						call M88;															
						goto Error;					

// 15-th step
<M88>				ar7 = 0_B000h;
						call M89;															
						goto Error;

<M89>				ar7 = 0_A000h;					
						call M90;															
						goto Error;					

// 16-th step
<M90>				ar7 = 0_B000h;
						call M91;															
						goto Error;

<M91>				ar7 = 0_A000h;					
						call M92;															
						goto Error;					

// 17-th step
<M92>				ar7 = 0_B000h;
						call M93;															
						goto Error;

<M93>				ar7 = 0_A000h;					
						call M94;															
						goto Error;					

// 18-th step
<M94>				ar7 = 0_B000h;
						call M95;															
						goto Error;

<M95>				ar7 = 0_A000h;					
						call M96;															
						goto Error;					

// 19-th step
<M96>				ar7 = 0_B000h;											  
						call M97;															
						goto Error;

<M97>				ar7 = 0_A000h;					
						call M98;														  
						goto Error;					
<M98>
// compare the results with the originals 

	ar0 = contr_result;
  gr0 = 40;

<Loop>
	ar1,gr1 = [ ar0++ ];
	gr2 = [ ar1 ];
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
		    halt;										               // Wait (test finished)
		    
end ".text.init";
