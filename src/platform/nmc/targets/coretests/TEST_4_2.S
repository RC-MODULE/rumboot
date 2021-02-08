// Test 4.2. ALU of Scalar Processor Testing (After Test 4.1)

//import from macros640x;

import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test
// The operand data dimension
array : word[41] = (
	0f0h,	0f1h,	0f2h,	0f3h,	0f4h,	0f5h,	0f6h,	0f7h,
	0,		55555555h,
	00202020h,	0dffdfdfdh,
	20020202h,	0ffdfdfdfh,
	80808082h,	0eff7f7f7h,
	10080808h,	7f7f7f7dh,
	40404042h,	0f7fdfbfbh,
	08040404h,	0dfbfbfbdh,
	80101014h,	0bbfefef5h,
	4401010ah,	7fefefebh,
	0fdffeffbh,	0c010000ah,
	3feffff5h,	02001004h,
	0fff7ff7fh,	01000800h,
	0fefff7ffh,	00080080h,
	0ebeff77fh,	20081000h,
	0bff7efffh,	14100880h,
	0ffffffffh,	00000080h,
	0ffffff7fh
		);

// The test result dimension

test_result : word[84] = ( 0 dup 84 );
long_result : long[8] = ( 0 dup 16 );


// The control value dimension

contr_result : word[84] = (
	0h,	0h,
	4h,	0aaaaaaaah,
	0ah,	55555554h,
	3h,	0fffffffeh,
	9h,	0ffffffffh,
	8h,	0ffffffffh,
	8h,	55555554h,
	1h,	55555554h,
	1h,	55555554h,
	1h,	0aaaaaaa9h,
	9h,	 0,
	9h,	0e01e1e1dh,
	8h,	1fe1e1e1h,
	1h,	70787879h,
	3h,	8f878785h,
	 0,	383e3c3dh,
	 0,	0e7c3c3c1h,
	 0,	3c0f0f09h,
	 0,	0c3f0f0f5h,
	 0,	0be0ff005h,
	 0,	41f00ff9h,
	 0,	00f8077fh,
	 0,	0ff07f87fh,
	 0,	0bf8077fh,
	 0,	0d407f87fh,
	 0,	0000007fh,
	 0,	0ffffff7fh,
	 0,	0h,
	5h,	0ffffffffh,
	8h,	0h,
	4h,	0ffffff7fh,
	8h,	0ffffff80h,
	8h,	0ffffff7fh,
	9h,	0ffffff80h,
	8h,	0fffffffeh,
	8h,	0ffffff10h,
	8h,	0fffffffeh,
	8h,	000000f1h,
	1h,	0fffffffeh,
	8h,	000000f2h,
	1h,	000000f2h,
	 0,	0ffffff0eh
	 ) ;

control_long : long[8] = (
	0ffffff10h,	0ffffff10h,
	0fffffffeh,	0fffffffeh,
	0fffffffeh,	0fffffffeh,
	long_result[3],	0ffffff0eh,
	0fffffffeh,	0fffffffeh,
	0d407f87fh,	0d407f87fh,
	0ffffffffh,	0ffffffffh,
	0ffffff80h,	0ffffff80h
	 );


save_stack : word = 0;

end data_test;

global test_4_2 : label;
   global start : label;
		weak mytest : label;
		
						 weak mytest_additional: label;		
Loop, Loop1, Normal, Error, Finish : label;	

// The boot loader								 
begin ".text.init"
<test_4_2>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

    		gr0 = 0h;															// GP registers definition
    		gr1 = 0h;
    		gr2 = 0h;
    		gr3 = 0h;
    		gr4 = 0h;
    		gr5 = 0h;
    		gr6 = 0h;
    		gr7 = 0h; 
    			
				goto mytest_additional;								// Jump to internal memory bank 0

// The main program (placed in memory with start address 8000h (internal memory bank 0) 

<mytest_additional>

// Lines 1 - 24

pswr clear 0fh;			// Clear four LSBs of pswr

ar4 = array[4];
gr4 = [ar4];

ar5 = array[5];
gr5 = [ar5];

ar6 = array[6];
gr6 = [ar6];

ar7 = array[7];
gr7 = [ar7];

ar4 = array[8];
gr2 = [ar4];

ar5 = array[9];
gr3 = [ar5];


ar6 = test_result;
ar7 = test_result[1];
[ar6] = pswr with gr0 = gr2 + gr2;
[ar7] = gr0;

ar6 = test_result[2];
ar7 = test_result[3];
[ar6] = pswr with gr1 = gr3 + gr3;
[ar7] = gr1;

ar6 = test_result[4];
ar7 = array[38];
[ar6] = pswr with gr1 = gr1 + gr1;
gr2 = [ar7];
ar4 = test_result[5];
[ar4 ++ gr4] = gr1;				// Address method 101

ar5 = test_result[6];
ar6 = test_result[7];
[ar5 ++ gr5] = pswr with gr1 = gr2 + gr2;	// Address method 101
[ar6 ++ gr6] = gr1;				// Address method 101

ar7 = test_result[8];
ar4 = test_result[9] + 0ffffff0ch;			// -0f4h
[ar7 ++ gr7] = pswr with gr1 = gr0 + gr2;	// Address method 101
[ar4 += gr4] = gr1;				// Address method 001

ar5 = test_result[10] + 0ffffff0bh;			// -0f5h
ar6 = test_result[11] + 0ffffff0ah;			// -0f6h
[ar5 += gr5] = pswr with gr0 = gr2 + gr0;	// Address method 001
[ar6 += gr6] = gr0;				// Address method 001

ar7 = test_result[12] + 0ffffff09h;			// -0f7h
ar4 = test_result[13];
[ar7 += gr7] = pswr with gr0 = gr3 + gr2;	// Address method 001
[ar4] = gr0 with gr1 = not gr3 noflags;		// Address method 100

ar5 = test_result[14];
ar6 = test_result[15];
[ar5] = pswr with gr0 = gr3 + gr2;		// Address method 100
[ar6] = gr0;					// Address method 100

ar7 = test_result[16];
ar4 = test_result[17];
[ar7] = pswr with gr3 = gr2 + gr3;		// Address method 100
[ar4] = gr3;					// Address method 100

ar5 = test_result[18];
ar6 = test_result[19];
[ar5] = pswr with gr2 = gr2 + gr1;		// Address method 100
[ar6] = gr2;					// Address method 100

ar7 = test_result[20];
[ar7] = pswr;					// Address method 100


// Lines 26 - 43


ar0 = array;
gr0 = [ar0];

ar1 = array[1];
gr1 = [ar1];

ar2 = array[2];
gr2 = [ar2];

ar3 = array[3];
gr3 = [ar3];

ar4 = array[10];
gr4 = [ar4];

ar5 = array[11];
gr5 = [ar5];


ar2 = test_result[22];
ar3 = array[12];
[ar2] = pswr with gr4 = gr4 + gr5;
gr5 = [ar3];

ar0 = test_result[23];
ar1 = array[13];
[ar0 ++ gr0] = gr4;				// Address method 101
gr6 = [ar1 ++ gr1];				// Address method 101

ar2 = test_result[24];
ar3 = array[14];
[ar2 ++ gr2] = pswr with gr5 = gr5 + gr6;	// Address method 101
gr6 = [ar3 ++ gr3];				// Address method 101

ar0 = test_result[25] + 0ffffff10h;			// - 0f0h
ar1 = array[15] + 0ffffff0fh;				// - 0f1h
[ar0 += gr0] = gr5;				// Address method 001
gr7 = [ar1 += gr1];				// Address method 001

ar2 = test_result[26] + 0ffffff0eh;			// - 0f2h
ar3 = array[16] + 0ffffff0dh;				// - 0f3h
[ar2 += gr2] = pswr with gr6 = gr6 + gr7;	// Address method 001
gr4 = [ar3 += gr3];				// Address method 001

ar0 = test_result[27];
ar1 = array[17];
[ar0] = gr6;					// Address method 100
gr7 = [ar1];					// Address method 100

ar2 = test_result[28];
ar3 = array[18];
[ar2] = pswr with gr7 = gr7 + gr4;		// Address method 100
gr4 = [ar3];					// Address method 100

// Lines 44 - 61

	ar0 = test_result[29];
	[ar0] = gr7;				// Address method 100
ar0 = array[19];
gr5 = [ar0];					// Address method 100
ar0 = array[20];
gr5 = [ar0] with gr4 = gr4 + gr5;
	ar0 = test_result[31];
	[ar0] = gr4;

ar0 = array[21];
gr6 = [ar0];
ar0 = array[22];
gr6 = [ar0] with gr5 = gr5 + gr6;

ar0 = array[23];
gr7 = [ar0];
	ar0 = test_result[33];
	[ar0] = gr5 with gr6 = gr6 + gr7;
ar0 = array[24];
gr7 = [ar0];

ar0 = array[25];
gr4 = [ar0];
	ar0 = test_result[35];
	[ar0] = gr6 with gr7 = gr7 + gr4;
ar0 = array[26];
gr4 = [ar0];

ar0 = array[27];
gr5 = [ar0];
	ar0 = test_result[37];
	[ar0] = gr7 with gr4 = gr4 + gr5;
ar0 = array[28];
gr5 = [ar0];

ar0 = array[29];
gr6 = [ar0];
	ar0 = test_result[39];
	[ar0] = gr4 with gr5 = gr5 + gr6;
ar0 = array[30];
gr6 = [ar0];

// Lines 62 - 78

ar0 = array[31];
gr7 = [ar0];
	ar1 = test_result[41];
	[ar1] = gr5 with gr6 = gr6 + gr7;
ar0 = array[32];
gr7 = [ar0];

ar0 = array[33];
gr4 = [ar0];
	ar2 = test_result[43];
	[ar2] = gr6 with gr7 = gr7 + gr4;
ar0 = array[34];
gr4 = [ar0];

ar0 = array[35];
gr5 = [ar0];
	ar3 = test_result[45];
	[ar3] = gr7 with gr4 = gr4 + gr5;
ar0 = array[36];
gr5 = [ar0];

ar0 = array[37];
gr6 = [ar0];
	ar4 = test_result[47];
	[ar4] = gr4 with gr5 = gr5 + gr6;
ar0 = array[38];
gr6 = [ar0];

ar0 = array[39];
gr7 = [ar0];
	ar5 = test_result[49];
	[ar5] = gr5 with gr6 = gr6 + gr7;
ar0 = array[40];
gr7 = [ar0];

ar0 = array[8];
gr4 = [ar0];

	ar6 = test_result[51];
	[ar6] = gr6 with gr7 = gr7 + gr4;
	ar7 = test_result[53];
	[ar7] = gr7;

// Lines 80 - 108.1

	gr7 = test_result[55];
	with gr4 = - gr4;
	[gr7] = gr4;				// Address method 010
	gr7 = [array[40]];

	gr5 = test_result[56];
	ar5 = test_result[57];
	[gr5] = pswr with gr6 = gr7 + gr6 + carry;
	[ar5] = gr6;
		gr5 = [test_result[49]];	// Restore gr5
		ar5 = gr5;			// Restore ar5

	gr6 = test_result[58];
	ar6 = test_result[59];
	[gr6] = pswr with gr4 = gr4 + gr4 + carry;
	[ar6] = gr4;
		gr6 = [test_result[57]];	// Restore gr6
		ar6 = gr6;			// Restore ar6

	ar7 = test_result[60];
	[ar7] = pswr with gr7 = gr7 + carry;
	ar7 = test_result[61];
	[ar7] = gr7;

	ar7 = test_result[62];
	[ar7] = pswr with gr7 = gr7 + 1;
	ar7 = test_result[63];
	[ar7] = gr7;

	ar7 = test_result[64];
	[ar7] = pswr with gr7 = gr7 - 1;
	ar7 = test_result[65];
	[ar7] = gr7;

	ar7 = test_result[66];
	[ar7] = pswr with gr7 = gr7 + carry;
	ar7 = test_result[67];			// gr7
	[ar7] = gr7;				// [gr7]

	ar7 = test_result[68];
	[ar7] = pswr with gr4 = gr1 - gr2 - 1 + carry;
	ar7 = test_result[69];
	[ar7] = gr4;
		ar7 = gr7;			// Restore ar7

	gr4 = test_result[70];
	[gr4] = pswr with gr0 = -gr0;
	ar4 = test_result[71];
	[ar4] = gr0;
		gr4 = [test_result[69]];	// Restore gr4
		ar4 = gr4;			// Restore ar4

	gr0 = test_result[72];
	[gr0] = pswr with gr1 = gr1 - gr3;
	ar0 = test_result[73];
	[ar0] = gr1;
		gr0 = [test_result[71]];	// Restore gr0
		ar0 = gr0;			// Restore ar0

	gr1 = test_result[74];
	[gr1] = pswr with gr2 = gr2 - 1 + carry;
	ar1 = test_result[75];
	[ar1] = gr2;
		gr1 = [test_result[73]];	// Restore gr1

	ar1 = test_result[76];
	[ar1] = pswr with gr2 = gr2 - gr3 - 1 + carry;
	ar1 = test_result[77];
	[ar1] = gr2;
		ar1 = gr1;			// Restore ar1

	gr2 = test_result[78];
	[gr2] = pswr with gr3 = gr3 - 1;
	ar2 = test_result[79];
	[ar2] = gr3;
		gr2 = [test_result[77]];	// Restore gr2

	ar2 = test_result[80];
	[ar2] = pswr with gr3 = gr3 - 1 + carry;
	ar2 = test_result[81];
	[ar2] = gr3 with gr3 = -gr3;
	ar2 = test_result[83];
	[ar2] = gr3;
		ar2 = gr2;			// Restore ar2		

// Lines 108.2 - 117

ar3 = long_result;
[ar3] = ar0,gr0;

ar3 = long_result[1];
[ar3] = ar1,gr1;

ar3 = long_result[2];
[ar3] = ar2,gr2;

ar3 = long_result[3];		// ar3
[ar3] = ar3,gr3;			// ar3

ar3 = long_result[4];
[ar3] = ar4,gr4;

ar3 = long_result[5];
[ar3] = ar5,gr5;

ar3 = long_result[6];
[ar3] = ar6,gr6;

ar3 = long_result[7];
[ar3] = ar7,gr7;

ar7 = [save_stack];

// The program of given and control dimension compare

	ar0 = test_result;
	ar1 = contr_result;
  gr0 = 84;
	gr3 = 1fh;

<Loop>
	gr1 = [ ar0++ ];
	with gr1 = gr1 and gr3;
	gr2 = [ ar1++ ];
	with gr2 = gr2;
	with gr2 - gr1;
	if <>0 goto Error;
	with gr0--;
		gr1 = [ ar0++ ];
		gr2 = [ ar1++ ];
		with gr2 - gr1;
		if <>0 goto Error;
		with gr0--;
		if <>0 goto Loop;

	ar0 = long_result;
	ar1 = control_long;
	gr0 = 16;

<Loop1>
	gr1 = [ ar0++ ];
	gr2 = [ ar1++ ];
	with gr2 - gr1;
	if <>0 goto Error;
	with gr0--;
	if <>0 goto Loop1;

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


