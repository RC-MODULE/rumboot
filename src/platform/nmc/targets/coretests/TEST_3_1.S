// Test 3.1. Write constant to all programable registers, read its contents
//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test

// The test result dimension 

test_result : long[77] = ( -1l dup 77);

// The control value dimension 
contr_res_1 : long[77] = (

	0efefefefh, -1, 0eeeeeeeeh, -1, // gr0-gr7 in block 1   [0],[1]
	0ededededh, -1, 0ecececech, -1, // --                  [2],[3]
	0ebebebebh, -1, 0eaeaeaeah, -1, // --                  [4],[5]
	0e9e9e9e9h, -1, 0e8e8e8e8h, -1, // --
	-1, -1,                 -1, -1, // Reserved
	0e8e8e8e8h, -1, 0e9e9e9e9h, -1, // gr7-gr0 in block 1
	0eaeaeaeah, -1, 0ebebebebh, -1, //
	0ecececech, -1, 0ededededh, -1, //
	0eeeeeeeeh, -1, 0efefefefh, -1, //

    00000000h, 0efefefefh, 01010101h, 0eeeeeeeeh,  // pair ari,gri in block 2 [18],[19]
    02020202h, 0ededededh, 03030303h, 0ecececech,  // --
    04040404h, 0ebebebebh, 05050505h, 0eaeaeaeah,  // --
    06060606h, 0e9e9e9e9h, 07070707h, 0e8e8e8e8h,  // --
    -1, -1,                                        // Reserved

	00000000h, 10101010h, 01010101h, 11111111h,  // pair ari,gri in block 3
	02020202h, 12121212h, 03030303h, 13131313h,  // --
	04040404h, 14141414h, 05050505h, 15151515h,  // --
	06060606h, 16161616h, 07070707h, 17171717h,  // --
	-1, -1,                    									 // Reserved

	00000000h, 10101010h, 01010101h, 11111111h,  // pair ari,gri in block 4 [36],[37]
	02020202h, 12121212h, 03030303h, 13131313h,  // --
	04040404h, 14141414h, 05050505h, 15151515h,  // --
	06060606h, 16161616h, 07070707h, 17171717h,  // --

	17171717h, -1,         														// Reserved [44],[45]
    0ffffffffh, -1, 0fefefefeh, -1, // register ari in block 5
    0fdfdfdfdh, -1, 0fcfcfcfch, -1, // --
    0fbfbfbfbh, -1, 0fafafafah, -1, // --
    0f9f9f9f9h, -1, 0f8f8f8f8h, -1, // --           [51],[52]

    0f8f8f8f8h, -1, 0f9f9f9f9h, -1, // register ari in block 6
    0fafafafah, -1, 0fbfbfbfbh, -1, // --
    0fcfcfcfch, -1, 0fdfdfdfdh, -1, // --
    0fefefefeh, -1, 0ffffffffh, -1, // --           [59],[60]

    87878787h, -1,  8f8f8f8fh, -1,  // register ari in block 7
    88888888h, -1,  8e8e8e8eh, -1,  //
    89898989h, -1,  8d8d8d8dh, -1,  //
    8a8a8a8ah, -1,  8c8c8c8ch, -1,  // [67],[68]

	8f8f8f8fh, 10101010h, 8e8e8e8eh, 8e8e8e8eh,  // pair ari,gri in block 8
	12121212h, 12121212h, 8c8c8c8ch, 13131313h,  // --
	13131313h, 13131313h, 8c8c8c8ch, 13131313h,  // --
	8d8d8d8dh, 8d8d8d8dh, 8f8f8f8fh, 10101010h   // -- [75],[76]
           ) ;

save_stack : word = 0;

end data_test;

global test_3_1 : label;
   global start : label;
		weak mytest : label;
	  weak mytest : label;
	  	
Loop, Normal, Error, Finish : label;

// The boot loader								 
begin ".text.init"
<test_3_1>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);
// Block 1 (lines 99-147), address method 00, const -> gri -> test_result

//t0 = 0f3f3f3f3h;            // set the value of t0 
//t1 = 0e3e3e3e3h;            // set the value of t1

gr0 = 0efefefefh;            	// set the value of gr0
[test_result] = gr0;         	// write the gr0 content to result dimension

gr1 = 0eeeeeeeeh;           	// Instruction 2.2
[test_result[1]] = gr1;      	// Instruction 1.2, address method 00

gr2 = 0ededededh;
[test_result[2]] = gr2;

gr3 = 0ecececech;
[test_result[3]] = gr3;

gr4 = 0ebebebebh;
[test_result[4]] = gr4;

gr5 = 0eaeaeaeah;
[test_result[5]] = gr5;

gr6 = 0e9e9e9e9h;
[test_result[6]] = gr6;

gr7 = 0e8e8e8e8h;
[test_result[7]] = gr7;

//[test_result[8]] = t0; 		// write the t0 content to result dimension 
//[test_result[9]] = t1;    // write the t1 content to result dimension 

[ar6 = test_result[10]] = gr7;
[ar7 = test_result[11]] = gr6;
[ar0 = test_result[12]] = gr5;
[ar1 = test_result[13]] = gr4;
[ar2 = test_result[14]] = gr3;
[ar3 = test_result[15]] = gr2;
[ar4 = test_result[16]] = gr1;
[ar5 = test_result[17]] = gr0;

// Block 2 (lines 1-27), const -> ari; pair  ari,gri -> test_result

//t0 = 0c0c0c0ch;                   // set the value of t0

ar0 = 00000000h;                  // set the value of ar0 (instruction 2.2)
[test_result[18]] = ar0,gr0;      // pair ar0,gr0 is written to result dimension

ar1 = 01010101h;                  // Instruction 2.2
[test_result[19]] = ar1,gr1;      // Instruction 1.2, address method 00

ar2 = 02020202h;
[test_result[20]] = ar2,gr2;

ar3 = 03030303h;
[test_result[21]] = ar3,gr3;

ar4 = 04040404h;
[test_result[22]] = ar4,gr4;

ar5 = 05050505h;
[test_result[23]] = ar5,gr5;

ar6 = 06060606h;
[test_result[24]] = ar6,gr6;

ar7 = 07070707h;
[test_result[25]] = ar7,gr7;

//[test_result[26]] = t1;          // t1 is written to result dimension 

// Block 3 (lines 28-54), const -> gri; paiir ari,gri -> test_result

//t1 = 1c1c1c1ch;                   // set the value of t1

gr0 = 10101010h;                  // set the value of gr0
[test_result[27]] = ar0,gr0;      // pair ar0,gr0 is written to result dimension 

gr1 = 11111111h;                  // Instruction 2.2 
[test_result[28]] = ar1,gr1;      // Instruction 1.2, address method 00
gr2 = 12121212h;
[test_result[29]] = ar2,gr2;

gr3 = 13131313h;
[test_result[30]] = ar3,gr3;

gr4 = 14141414h;
[test_result[31]] = ar4,gr4;

gr5 = 15151515h;
[test_result[32]] = ar5,gr5;

gr6 = 16161616h;
[test_result[33]] = ar6,gr6;

gr7 = 17171717h;
[test_result[34]] = ar7,gr7;

// [test_result[35]] = t1;      	// t1 is written to result dimension 
// Block 4 (lines 58-69), pair ari,gri -> results, simultaneously address -> ari

[ar0 = test_result[36]] = ar0,gr0;   // pair ari,gri is written on address, loaded into ari
[ar1 = test_result[37]] = ar1,gr1;   // Instruction 1.2, address method 10
[ar2 = test_result[38]] = ar2,gr2;
[ar3 = test_result[39]] = ar3,gr3;
[ar4 = test_result[40]] = ar4,gr4;
[ar5 = test_result[41]] = ar5,gr5;
[ar6 = test_result[42]] = ar6,gr6;
[ar7 = test_result[43]] = ar7,gr7;

// Block 5 (Lines 72-98) 

ar0 = test_result[44];	     // Set address in ari
ar1 = test_result[45];
ar2 = test_result[46];
ar3 = test_result[47];
ar4 = test_result[48];
ar5 = test_result[49];
ar6 = test_result[50];
ar7 = test_result[51];

[ar0 += 0] = gr7;          // gr7 adressed with displasment =0 
ar0 = 0ffffffffh;          // Set address in ar0
[ar1 += 0] = ar0;          // Instruction 1.2, address method 01

ar1 = 0fefefefeh;
[ar2 += 0] = ar1;

ar2 = 0fdfdfdfdh;
[ar3 += 0] = ar2;

ar3 = 0fcfcfcfch;
[ar4 += 0] = ar3;

ar4 = 0fbfbfbfbh;
[ar5 += 0] = ar4;

ar5 = 0fafafafah;
[ar6 += 0] = ar5;

ar6 = 0f9f9f9f9h;
[ar7 += 0] = ar6;

ar7 = 0f8f8f8f8h;
[test_result[52]] = ar7;

// Block 6 (Lines 149-159) 

[ar7 = test_result[53]] = ar7;   // Instruction 1.2, address method 10
[ar6 = test_result[54]] = ar6;   // The content of the register is written on address 
[ar5 = test_result[55]] = ar5;   // and simultaneously the address is set to the register
[ar4 = test_result[56]] = ar4;
[ar3 = test_result[57]] = ar3;
[ar2 = test_result[58]] = ar2;
[ar1 = test_result[59]] = ar1;
[ar0 = test_result[60]] = ar0;

// Block 7 (lines 161-171)     [arj+Cm] = ari

ar0 = test_result[61] + 1;
ar7 = 87878787h;            // The content of the ar7
[ar0 += 0ffffffffh] = ar7;  // is written on address, based on [ar0] with displasment ffffffffh

ar7 = test_result[62] + 1;
ar0 = 8f8f8f8fh;           
[ar7 += 0ffffffffh] = ar0;  // Instruction 1.2, address method 01

ar1 = test_result[63] + 1;
ar6 = 88888888h;
[ar1 += 0ffffffffh] = ar6;

ar6 = test_result[64] + 1;
ar1 = 8e8e8e8eh;
[ar6 += 0ffffffffh] = ar1;

ar2 = test_result[65] + 1;
ar5 = 89898989h;
[ar2 += 0ffffffffh] = ar5;

ar5 = test_result[66] + 1;
ar2 = 8d8d8d8dh;
[ar5 += 0ffffffffh] = ar2;

ar3 = test_result[67] + 1;
ar4 = 8a8a8a8ah;
[ar3 += 0ffffffffh] = ar4;

ar4 = test_result[68] + 1;
ar3 = 8c8c8c8ch;
[ar4 += 0ffffffffh] = ar3;

// Block 8 (Lines 180-200)

ar7 = ar0;                 // Instruction 2.1, ar7 is modified         
gr1 = ar1;
ar6,gr6 = ar2;
ar5,gr5 = ar3,gr3;
gr7 = gr0;
ar2 = gr2;
ar4,gr4 = gr3;
ar0,gr0 = ar0,gr0;

[test_result[69]] = ar0,gr0;
[test_result[70]] = ar1,gr1;
[test_result[71]] = ar2,gr2;
[test_result[72]] = ar3,gr3;
[test_result[73]] = ar4,gr4;
[test_result[74]] = ar5,gr5;
[test_result[75]] = ar6,gr6;
[test_result[76]] = ar7,gr7;

    ar7 = [ save_stack ];

// The program of the given and control dimension comparison 
	ar0 = test_result;
	ar1 = contr_res_1;
	gr0 = 154;

	<Loop>
	  gr1 = [ ar0++ ];
	  gr2 = [ ar1++ ];
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
