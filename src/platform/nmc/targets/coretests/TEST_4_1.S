// Test 4.1. Logic Unit (LU) of Scalar Processor Testing

//import from macros640x;

import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test
// The test result dimension
	test_result : word[132] = ( 0 dup 132);
// The control value dimension
	contr_result : word[132] = ( 
				0h,	// pswr after clearning
	0h,	4h, 	-1h,	8h,	// pair (register)-(pswr)
	0h,	4h,  	-1h,	8h,	// pair (register)-(pswr)
	0h,	4h,	-1h,	8h,	// pair (register)-(pswr)
	0h,	4h,	-1h,	8h,	// pair (register)-(pswr)
	0h,	4h,	-1h,	8h,	// pair (register)-(pswr)
	0h,	4h,	-1h,	8h,	// pair (register)-(pswr)
	0h,	4h,	-1h,	8h,	// pair (register)-(pswr)
	0h,	4h,	-1h,		// pair (register)-(pswr)

0,					// Not loaded location 

	0h,	8h,	0h,	4h,	// (register)-(pswr of previous operation)
	-1h,	4h,	-1h,	8h,	// (register)-(pswr of previous operation)
	0h,	8h,	0h,	4h,	// (register)-(pswr of previous operation)
	-1h,	4h,	-1h,	8h,	// (register)-(pswr of previous operation)
	0h,	8h,	0h,	4h,	// (register)-(pswr of previous operation)
	-1h,	4h,	-1h,	8h,	// (register)-(pswr of previous operation)
	0h,	8h,	0h,	4h,	// (register)-(pswr of previous operation)
	-1h,	4h,	-1h,	8h,
		
0,					// Not loaded location 

				8h,	// pswr of previous operation 
	0h,	4h,	0h,	4h,	// pair (register)-(pswr) 
	0h,	4h,	0h,	4h,	// pair (register)-(pswr)
	-1h,	8h,	-1h,	8h,	// pair (register)-(pswr)
	-1h,	8h,	-1h,	8h,	// pair (register)-(pswr)
	0h,	4h,	0h,	4h,	// pair (register)-(pswr)
	0h,	4h,	0h,	4h,	// pair (register)-(pswr)
	-1h,	8h,	-1h,	8h,	// pair (register)-(pswr)
	-1h,	8h,	-1h,

0,					// Not loaded location 

	0h,	8h,	0h,	4h,	// (register)-(pswr of previous operation)
	0h,	4h,	0h,	4h,	// (register)-(pswr of previous operation)
	0h,	4h,	0h,	4h,	// (register)-(pswr of previous operation)
	0h,	4h,	0h,	4h,	// (register)-(pswr of previous operation)
	-1h,	4h,	-1h,	8h,	// (register)-(pswr of previous operation)
	-1h,	8h,	-1h,	8h,	// (register)-(pswr of previous operation)
	-1h,	8h,	-1h,	8h,	// (register)-(pswr of previous operation)
	-1h,	8h,	-1h,	8h,	// (register)-(pswr of previous operation)

0					// Not loaded location 
			) ;

save_stack : word = 0;

end data_test;


begin ".text.init"

global test_4_1 : label;
   global start : label;
	  weak mytest : label;

Loop, Normal, Error, Finish : label;



<test_4_1>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

		gr0 = 0h;							// Register definition
		gr1 = 0ffffffffh;			//
		
		pswr clear 0fh;				// Clear four LSBs of pswr

// Execute all logical functions with gr0 and not gr0 as operands 
// Address method ari++

	ar0 = test_result;
	ar1 = test_result[2];
	ar2 = test_result[4];
	ar3 = test_result[6];
	ar4 = test_result[8];
	ar5 = test_result[10];
	ar6 = test_result[12];
	ar7 = test_result[14];

[ar0++] = pswr with gr2 = false;			// 0
[ar0++] = gr2;                                 //
																								// __   __
[ar1++] = pswr with gr3 = not gr0 and not gr0;	// r0 & r0
[ar1++] = gr3;                                 //
																						//      __
[ar2++] = pswr with gr4 = gr0 and not gr0;	// r0 & r0
[ar2++] = gr4;                                 //
																			// __
[ar3++] = pswr with gr5 = not gr0;		// r0
[ar3++] = gr5;					//
																								 // __
[ar4++] = pswr with gr6 = not gr0 and gr0;       // r0 & r0
[ar4++] = gr6;					//
																			// __
[ar5++] = pswr with gr7 = not gr0;		// r0
[ar5++] = gr7;					//
							//
[ar6++] = pswr with gr2 = gr0 xor gr0;		// r0 xor r0
[ar6++] = gr2;					//
																							// __   __
[ar7++] = pswr with gr3 = not gr0 or not gr0;	// r0 + r0
[ar7++] = gr3;						//
								//
	ar0 = test_result[16];
	ar1 = test_result[18];
	ar2 = test_result[20];
	ar3 = test_result[22];
	ar4 = test_result[24];
	ar5 = test_result[26];
	ar6 = test_result[28];
	ar7 = test_result[30];

[ar0++] = pswr with gr4 = gr0 and gr0;		// r0 & r0
[ar0++] = gr4;					//
																			//        __
[ar1++] = pswr with gr5 = not gr0 xor gr0;	 // r0 xor r0
[ar1++] = gr5;					//
						//
[ar2++] = pswr with gr6 = gr0;			// r0
[ar2++] = gr6;					//
																					//      __
[ar3++] = pswr with gr7 = gr0 or not gr0;	// r0 + r0
[ar3++] = gr7;					//
						//
[ar4++] = pswr with gr2 = gr0;			// r0
[ar4++] = gr2;					//
																					// __
[ar5++] = pswr with gr3 = not gr0 or gr0;	// r0 + r0
[ar5++] = gr3;					//
						//
[ar6++] = pswr with gr4 = gr0 or gr0;		// r0 + r0
[ar6++] = gr4;					//
						//
[ar7++] = pswr with gr5 = true;			// -1
[ar7++] = gr5;					//
						//
// Execute all logical functions with gr0 and gr1 as operands 
// Address method --ari

	ar0 = test_result[35];
	ar1 = test_result[37];
	ar2 = test_result[39];
	ar3 = test_result[41];
	ar4 = test_result[43];
	ar5 = test_result[45];
	ar6 = test_result[47];
	ar7 = test_result[49];

[--ar0] = pswr with gr2 = false;		// 0
[--ar0] = gr2;					//
																								// __   __
[--ar1] = pswr with gr3 = not gr0 and not gr1;	// r0 & r1
[--ar1] = gr3;					 //
							 												 //      __
[--ar2] = pswr with gr4 = not gr0 and gr1;	// r0 & r1
[--ar2] = gr4;					//
																			// __
[--ar3] = pswr with gr5 = not gr0;		// r0
[--ar3] = gr5;					//
								     														 // __
[--ar4] = pswr with gr6 = gr0 and not gr1;	// r0 & r1
[--ar4] = gr6;					//
																			// __
[--ar5] = pswr with gr7 = not gr1;		// r1
[--ar5] = gr7;					//
						//
[--ar6] = pswr with gr2 = gr0 xor gr1;		// r0 xor r1
[--ar6] = gr2;					//
																							// __   __
[--ar7] = pswr with gr3 = not gr0 or not gr1;	// r0 + r1
[--ar7] = gr3;					//
						//
	ar0 = test_result[51];
	ar1 = test_result[53];
	ar2 = test_result[55];
	ar3 = test_result[57];
	ar4 = test_result[59];
	ar5 = test_result[61];
	ar6 = test_result[63];
	ar7 = test_result[65];

[--ar0] = pswr with gr4 = gr0 and gr1;		// r0 & r1
[--ar0] = gr4;					//
																			//        __
[--ar1] = pswr with gr5 = not gr0 xor gr1;	 // r0 xor r1
[--ar1] = gr5;					//
						      //
[--ar2] = pswr with gr6 = gr1;			// r1
[--ar2] = gr6;				  //
						  												//      __
[--ar3] = pswr with gr7 = not gr0 or gr1;	 // r0 + r1
[--ar3] = gr7;					 //
						       //
[--ar4] = pswr with gr2 = gr0;			// r0
[--ar4] = gr2;					//
																								// __
[--ar5] = pswr with gr3 = gr0 or not gr1;	 // r0 + r1
[--ar5] = gr3;					 //
						       //
[--ar6] = pswr with gr4 = gr0 or gr1;		// r2 + r1
[--ar6] = gr4;					//
						//
[--ar7] = pswr with gr5 = true;			// -1
[--ar7] = gr5;					//

// Execute all logical functions with gr1 and gr0 as operands 
// Address method ari++ 

	ar0 = test_result[66];
	ar1 = test_result[68];
	ar2 = test_result[70];
	ar3 = test_result[72];
	ar4 = test_result[74];
	ar5 = test_result[76];
	ar6 = test_result[78];
	ar7 = test_result[80];

[ar0++] = pswr with gr2 = false;		// 0
[ar0++] = gr2;					//
																								// __   __
[ar1++] = pswr with gr3 = not gr1 and not gr0;	// r1 & r0
[ar1++] = gr3;					 //
																			 //      __
[ar2++] = pswr with gr4 = not gr1 and gr0;	// r1 & r0
[ar2++] = gr4;					//
																			// __
[ar3++] = pswr with gr5 = not gr1;		// r1
[ar3++] = gr5;					//
																									// __
[ar4++] = pswr with gr6 = gr1 and not gr0;	 // r1 & r0
[ar4++] = gr6;					//
																			// __
[ar5++] = pswr with gr7 = not gr0;		// r0
[ar5++] = gr7;					//
						//
[ar6++] = pswr with gr2 = gr1 xor gr0;		// r1 xor r0
[ar6++] = gr2;					//
																							// __   __
[ar7++] = pswr with gr3 = not gr1 or not gr0;	// r1 + r0
[ar7++] = gr3;					//
					
	ar0 = test_result[82];
	ar1 = test_result[84];
	ar2 = test_result[86];
	ar3 = test_result[88];
	ar4 = test_result[90];
	ar5 = test_result[92];
	ar6 = test_result[94];
	ar7 = test_result[96];

[ar0++] = pswr with gr4 = gr1 and gr0;		// r1 & r0
[ar0++] = gr4;					//
																			//        __
[ar1++] = pswr with gr5 = not gr1 xor gr0;	 // r1 xor r0
[ar1++] = gr5;					//
						//
[ar2++] = pswr with gr6 = gr0;			// r0
[ar2++] = gr6;					//
						 												 //      __
[ar3++] = pswr with gr7 = not gr1 or gr0;	// r1 + r0
[ar3++] = gr7;					//
						//
[ar4++] = pswr with gr2 = gr1;			// r1
[ar4++] = gr2;					//
								     														 // __
[ar5++] = pswr with gr3 = gr1 or not gr0; 	// r1 + r0
[ar5++] = gr3;					//
						//
[ar6++] = pswr with gr4 = gr1 or gr0;		// r1 + r0
[ar6++] = gr4;					//
						//
[ar7++] = pswr with gr5 = true;			// -1
[ar7++] = gr5;					//

// Execute all logical functions with gr1 and not gr1 as operands 
// Address method --ari

	ar0 = test_result[101];
	ar1 = test_result[103];
	ar2 = test_result[105];
	ar3 = test_result[107];
	ar4 = test_result[109];
	ar5 = test_result[111];
	ar6 = test_result[113];
	ar7 = test_result[115];

[--ar0] = pswr with gr2 = false; 
[--ar0] = gr2;

[--ar1] = pswr with gr3 = not gr1 and not gr1;
[--ar1] = gr3;

[--ar2] = pswr with gr4 = gr1 and not gr1;
[--ar2] = gr4;

[--ar3] = pswr with gr5 = not gr1;
[--ar3] = gr5;

[--ar4] = pswr with gr6 = not gr1 and gr1;
[--ar4] = gr6;

[--ar5] = pswr with gr7 = not gr1;
[--ar5] = gr7;

[--ar6] = pswr with gr2 = gr1 xor gr1;              
[--ar6] = gr2;

[--ar7] = pswr with gr3 = not gr1 or not gr1;
[--ar7] = gr3;

	ar0 = test_result[117];
	ar1 = test_result[119];
	ar2 = test_result[121];
	ar3 = test_result[123];
	ar4 = test_result[125];
	ar5 = test_result[127];
	ar6 = test_result[129];
	ar7 = test_result[131];

[--ar0] = pswr with gr4 = gr1 and gr1;
[--ar0] = gr4;

[--ar1] = pswr with gr5 = not gr1 xor gr1;
[--ar1] = gr5;

[--ar2] = pswr with gr6 = gr1;
[--ar2] = gr6;

[--ar3] = pswr with gr7 = gr1 or not gr1;
[--ar3] = gr7;

[--ar4] = pswr with gr2 = gr1;
[--ar4] = gr2;

[--ar5] = pswr with gr3 = not gr1 or gr1;
[--ar5] = gr3;

[--ar6] = pswr with gr4 = gr1 or gr1;
[--ar6] = gr4;

[--ar7] = pswr with gr5 = true;
[--ar7] = gr5;

    ar7 = [ save_stack ];

// Program of given and control dimension comparison

	ar0 = test_result;
	ar1 = contr_result;
	gr0 = 132;
	gr3 = 1fh;

	<Loop>
	  gr1 = [ ar0++ ];
	  with gr1 = gr1 and gr3;
	  gr2 = [ ar1++ ];
	  with gr2 - gr1;
	  if <>0 goto Error;
	with gr0--;
	gr1 = [ ar0++ ];
	gr2 = [ ar1++ ];
	with gr2 -gr1;
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



