// Test 4.3 Scalar Multiply Testing (Followed by 4.2)

//import from macros640x;

import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test
// The operand data dimension
array : word[5] = (
										0,	0ffffffffh,
						1e61f752h,	270fdd48h,
						0f7521e61h
									);

// The test result dimension
test_result : word[8] = ( -1 dup 8 );	// The register contents
pswr_result : word[4] = ( 0 dup 4 );	// The pswr contents

// The control value dimension
contr_result : word[8] = (		// The control values of registers
	0h,	0h,	0h,	0h,
	04a2ceaeh,	19dd5910h,
	0feacf8f0h,	02014848h
	);

contr_pswr : word[3] = (		// The control values of pswr
												04h,	14h,	04h, 14h
											 );
save_stack : word = 0;
end data_test;


begin ".text.init"

global test_4_3 : label;
   global start : label;
	  weak mytest : label;
	
Loop, Loop1, Normal, Error, Finish : label;

<test_4_3>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

gr0 = 0h;							

ar3 = array;				                         // What the value of pswr ???
pswr = [ar3];

gr7 = [ar3];
gr0 = [ar3];

gr2 = gr0 *: gr7;
.repeat 15;
gr2 = gr0 * gr7;
.endrepeat;

ar3 = test_result;
[ar3] = gr2;
ar3 = test_result[1];
[ar3] = gr7;

ar3 = array[1];
gr7 = [ar3];

ar3 = pswr_result;
[ar3] = pswr;

gr2 = gr0 *: gr7;
.repeat 15;
gr2 = gr0 * gr7;
.endrepeat;

ar3 = test_result[2];
[ar3] = gr2;
ar3 = test_result[3];
[ar3] = gr7;

ar3 = pswr_result[1];
[ar3] = pswr;

ar3 = array[2];
gr7 = [ar3];

ar3 = array[3];
gr1 = [ar3];

gr0 = gr1 *: gr7 noflags;
.repeat 15;
gr0 = gr1 * gr7 noflags;
.endrepeat;

ar3 = test_result[4];
[ar3] = gr0;
ar3 = test_result[5];
[ar3] = gr7;

ar3 = array[4];
gr7 = [ar3];

ar3 = pswr_result[2];
[ar3] = pswr;

gr0 = gr1 *: gr7 noflags;
.repeat 15;
gr0 = gr1 * gr7 noflags;
.endrepeat;

ar3 = test_result[6];
[ar3] = gr0;
ar3 = test_result[7];
[ar3] = gr7;

ar3 = pswr_result[3];
[ar3] = pswr;

// ar3 = ??;
// [ar3] = intr;

ar3 = array;
pswr = [ar3];


ar7 = [save_stack];


// The program of given end control dimension compare

	ar0 = test_result;
	ar1 = contr_result;
  gr0 = 8;

<Loop>
	gr1 = [ ar0++ ];
	gr2 = [ ar1++ ];
	with gr2 - gr1;
	if <>0 goto Error;
	with gr0--;
	if <>0 goto Loop;

		ar0 = pswr_result;
		ar1 = contr_pswr;
	  	gr0 = 3;
		gr3 = 1Fh;

	<Loop1>
		gr1 = [ ar0++ ];
		gr2 = [ ar1++ ];
		with gr1 = gr1 and gr3;
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