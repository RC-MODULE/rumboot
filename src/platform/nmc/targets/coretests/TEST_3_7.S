// Test 3.7. Address ALU (AU1) & Index Address Mode Checking

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test
// The control value dimension 
contr_result : long[46] = (
  //  Data    Address
0000_8000_0001_0000hl,
  0000_8000_0000_AAAAhl,
  0000_AAAA_0000_7FFEhl,
  0000_7FFE_0001_5550hl,
  0001_5550_0001_7FFFhl,
  0001_7FFF_0001_7FFChl,
  0001_7FFC_0001_5554hl,
  0001_5554_0001_5AA9hl,
  0001_0000_0001_5553hl,
  0000_AAAA_0001_2AA8hl,
  0000_7FFE_0001_1E1Dhl,
  0001_5550_0001_71E2hl,
  0001_7FF0_0001_7879hl,
  0001_7FFC_0001_7785hl,
  0001_5554_0001_3C3Dhl,
  0001_5553_0001_C3C1hl,
  0001_2AA8_0001_0F05hl,
  0001_1E1D_0000_F0F9hl,
  0001_71E2_0000_F005hl,
  0001_7879_0001_0FF9hl,
  0001_6F7D_0001_0770hl,
  0000_FBFB_0001_7870hl,
  0001_BFBD_0001_7874hl,
  0000_FEF3_0001_078Ehl,
 0FFFF_EFED_0001_008Ehl,
 0FFFF_EFFB_0001_7F7Fhl,
  0000_1004_0001_7FF5hl,
  0000_0800_0001_7FFEhl,
  0001_7870_0000_7FFFhl,
  0001_7874_0001_7FFFhl,
  
  0001_010C_0001_000Bhl,
  0000_000A_0001_000Ahl,
  0001_000A_0001_0007hl,
  0000_0005_0001_0006hl,
  0000_FFF5_0000_9007hl,
  0000_8006_0000_9006hl,
  0009_FF70_0001_0017hl,
  0000_0006_0001_0016hl,
  0000_0808_0000_8005hl,
  0000_8003_0000_8004hl,
  0000_4042_0001_7877hl,
  0007_F874_0001_7876hl,
  0000_0404_0001_178Fhl,
  0001_078E_0001_178Ehl,
  0000_1012_0001_018Fhl,
  0001_008E_0001_018Ehl);
  
end data_test;

// The  program, which placed in memory with start address 0h (IMU1) 								 
begin ".text.init"

global test_3_7 : label;
   global start : label;
		weak mytest : label;
			
Loop, Normal, Error, Finish : label;
       	 
<test_3_7>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);
 
// 1-th step
ar0 = 0_8000h;
gr0 = 0_8000h;
[ar0 +=	gr0] = ar0;											// Write 0_8000h on address 1_0000h

// 2-th step
ar1 = 0_5555h;
gr1 = 0_5555h;
[ar1 +=	gr1] = ar0;											// Write 0_8000h on address 0_AAAAh

//3-th step
ar2 = 0_3FFFh;
gr2 = 0_3FFFh;
[ar2 +=	gr2] = ar1;											// Write 0_AAAAh on address 0_7FFEh
  									  
//4-th step
ar3 = 0_AAA8h;
gr3 = 0_AAA8h;
[ar3 +=	gr3] = ar2;											// Write 0_7FFEh on address 1_5550h
  									  
//5-th step
ar4 = 0h;
gr4 = 01_7FF0h;
[ar4 +=	gr4] = ar3;											// Write 1_5550h on address 1_7FF0h

//6-th step
ar5 = 01_7FFCh;
gr5 = 0h;
[ar5 +=	gr5] = ar4;											// Write 1_7FF0h on address 1_7FFCh

//7-th step
ar6 =     1_5555h;
gr6 = 0FFFF_FFFFh;
[ar6 +=	gr6] = ar5;											// Write 1_7FFCh on address 1_5554h

//8-th step
ar7 =    01_5AAAh;
gr7 = 0FFFF_FFFFh;
[ar7 +=	gr7] = ar6;											// Write 1_5554h on address 1_5AA9h

//9-th step
ar7 = 0FFFF_FFFFh;
gr7 = 1_5554h;
[ar7 +=	gr7] = ar0;											// Write 1_0000h on address 1_5553h

//10-th step
ar0 = 0FFFF_FFFEh;
gr0 =    01_2AAAh;
[ar0 +=	gr0] = ar1;											// Write 0_AAAAh on address 1_2AA8h

//11-th step
ar1 =  0_2020h;
gr1 =  0_FDFDh;
[ar1 +=	gr1] = ar2;											// Write 0_7FFEh on address 1_1E1Dh

//12-th step
ar2 = 1_6FDFh;
gr2 = 0_0203h;
[ar2 +=	gr2] = ar3;											// Write 1_5550h on address 1_71E2h

//13-th step
ar3 = 0_F7F7h;
gr3 = 0_8082h;
[ar3 +=	gr3] = ar4;											// Write 1_7FF0h on address 1_7879h

//14-th step
ar4 = 1_6F7Dh;
gr4 = 0_0808h;
[ar4 +	gr4] = ar5;											// Write 1_7FFCh on address 1_7785h

//15-th step
ar5 =  0_FBFBh;
gr5 =  0_4042h;
[ar5 +	gr5] = ar6;											// Write 1_5554h on address 1_3C3Dh

//16-th step
ar6 = 1_BFBDh;
gr6 = 0_0404h;
[ar6 +	gr6] = ar7;											// Write 1_5553h on address 1_C3C1h

//17-th step
ar7 =  0_FEF3h;
gr7 =  0_1012h;
[ar7 +	gr7] = ar0;											// Write 1_2AA8h on address 1_0F05h

//18-th step
ar0 = 0FFFF_EFEDh;
gr0 =     1_010Ch;
[ar0 +	gr0] = ar1;											// Write 1_1E1Dh on address 0_F0F9h

//19-th step
ar1 = 0FFFF_EFFBh;
gr1 =     1_000Ah;
[ar1 +	gr1] = ar2;											// Write 1_71E2h on address 0_F005h

//20-th step
ar2 = 0000_1004h;
gr2 =    0_FFF5h;
[ar2 +	gr2] = ar3;											// Write 1_7879h on address 1_0FF9h

//21-th step
ar3 = 0_0800h;
gr3 = 0_FF70h;
[ar3 +	gr3] = ar4;											// Write 1_6F7Dh on address 1_0770h

//22-th step
ar4 = 0000_0080h;
[ar4 +=	 1_77F0h] = ar5;							  // Write 0_FBFBh on address 1_7870h

//23-th step
ar5 =   0_0880h;
[ar5 +=	1_6FF4h] = ar6;							    // Write 1_BFBDh on address 1_7874h

//24-th step
ar6 =   0_100Fh;
[ar6 +=	0_F77Fh] = ar7;							    // Write  0_FEF3h on address 1_078Eh

//25-th step
ar7 =       1_008Fh;
[ar7 +=	0FFFF_FFFFh] = ar0;							// Write 0FFFF_EFEDh on address 1_008Eh

//26-th step
ar0 =  0_000Ah;
[ar0 + 1_7F75h] = ar1;							    // Write 0FFFF_EFFBh on address 1_7F7Fh

//27-th step
ar1 =  0_0005h;
[ar1 + 1_7FF0h] = ar2;							    // Write 0000_1004h on address 1_7FF5h

//28-th step
ar2 =  0_8006h;
[ar2 + 0_FFF8h] = ar3;							    // Write 0_0800h on address 1_7FFEh

//29-th step
ar3 =  0_0006h;
[ar3 + 0_7FF9h] = ar4;							    // Write 1_7870h on address 0_7FFFh

//30-th step
ar4 =  0_8003h;
[ar4 + 0_FFFCh] = ar5;							    // Write 1_7874h on address 1_7FFFh


// Write context of all address registers and general purpose registers

[ar0 + 1_0000h] = ar0, gr0;					    // Write 1_010Ch || 0000_000Ah on address 1_000Ah

[ar1 + 1_0001h] = ar1, gr1;					    // Write 1_000Ah || 0_0005h on address 1_0006h

[ar2 + 0_1000h] = ar2, gr2;					    // Write 0_FFF5h || 0_8006h on address 0_9006h

[ar3 + 1_0010h] = ar3, gr3;					    // Write 0_FF70h || 0_0006h on address 1_0016h

[ar4 + 0_0001h] = ar4, gr4;					    // Write 0_0808h || 0_8003h on address 0_8004h

[ar5 + 0_0002h] = ar5, gr5;					    // Write 0_4042h || 1_7874h on address 1_7876h

[ar6 + 0_1000h] = ar6, gr6;					    // Write 0_0404h || 1_078Eh on address 1_178Eh	  

[ar7 + 0_0100h] = ar7, gr7;					    // Write 0_1012h || 1_008Eh on address 1_018Eh

// compare the results with the originals 

	ar0 = contr_result;
  gr0 = 46;

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
		    halt;										             // Wait (test finished)
		    
end ".text.init";
