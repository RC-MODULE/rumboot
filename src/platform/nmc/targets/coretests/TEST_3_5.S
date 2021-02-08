// Test 3.5. Instruction format 3.1, 3.2 checking

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test

//  Operand data dimension

array : long[8] = (
	0f0f0f0f00f0f0f0fhl, 0f1f1f1f11f1f1f1fhl,
	0f2f2f2f22f2f2f2fhl, 0f3f3f3f33f3f3f3fhl,
	0f4f4f4f44f4f4f4fhl, 0f5f5f5f55f5f5f5fhl,
	0f6f6f6f66f6f6f6fhl, 0f7f7f7f77f7f7f7fhl
		);

// The test result dimension

test_result : long[24] = ( -1 dup 48);

// The control value dimension

contr_result : long[24] = ( 
	3f3f3f3fh, -1, 3f3f3f3fh, -1,
	1f1f1f1fh, -1, 2f2f2f2fh, -1,
	7f7f7f7fh, -1, 7f7f7f7fh, -1,
	5f5f5f5fh, -1, 6f6f6f6fh, -1,
	
	0f7f7f7f7h, 0f7f7f7f7h,
	0f6f6f6f6h, 0f6f6f6f6h,
	0f5f5f5f5h, 0f5f5f5f5h,
	0f4f4f4f4h, 0f4f4f4f4h,
	0f3f3f3f3h, 0f3f3f3f3h,
	0f2f2f2f2h, 0f2f2f2f2h,
	0f1f1f1f1h, 0f1f1f1f1h,
	0f0f0f0f0h, 0f0f0f0f0h,
	
	0f1f1f1f1h, 0f0f0f0f0h,
	0f2f2f2f2h, 0f1f1f1f1h, 
	0f3f3f3f3h, 0f2f2f2f2h, 
	0f1f1f1f1h, 0f3f3f3f3h, 
	0f5f5f5f5h, 0f4f4f4f4h, 
	0f6f6f6f6h, 0f5f5f5f5h, 
	0f7f7f7f7h, 0f6f6f6f6h, 
	0f5f5f5f5h, 0f7f7f7f7h
	 ) ;

save_stack : word = 0;

end data_test;

global test_3_5 : label;
   global start : label;
		weak mytest : label;
	
			weak mytest_additional: label;
Loop, Normal, Error, Finish : label;

// The boot loader								 
begin ".text.init"
<test_3_4>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

				goto mytest_additional;								      // Jump to external memory  

// The main program 

<mytest_additional>
gr0,ar0 = [array];           // Instruction 1.2, address method 00
gr1,ar1 = [array[1]];        // Instruction 1.2, address method 00
gr2,ar2 = [array[2]];        // Instruction 1.2, address method 00
gr3,ar3 = [array[3]];        // Instruction 1.2, address method 00
gr4,ar4 = [array[4]];        // Instruction 1.2, address method 00
gr5,ar5 = [array[5]];        // Instruction 1.2, address method 00
gr6,ar6 = [array[6]];        // Instruction 1.2, address method 00
gr7,ar7 = [array[7]];        // Instruction 1.2, address method 00

ar0 = ar3 addr;              // Instruction 3.2, address method 00 
ar3 = ar2 addr;              // Instruction 3.2, address method 00 
ar2 = ar1 addr;              // Instruction 3.2, address method 00 
ar1 = ar0 addr;              // Instruction 3.2, address method 00 
ar4 = ar7 addr;              // Instruction 3.2, address method 00 
ar7 = ar6 addr;              // Instruction 3.2, address method 00 
ar6 = ar5 addr;              // Instruction 3.2, address method 00 
ar5 = ar4 addr;              // Instruction 3.2, address method 00 

[test_result]    = ar0;      // Instruction 1.2, address method 00
[test_result[1]] = ar1;      // Instruction 1.2, address method 00
[test_result[2]] = ar2;      // Instruction 1.2, address method 00
[test_result[3]] = ar3;      // Instruction 1.2, address method 00
[test_result[4]] = ar4;      // Instruction 1.2, address method 00
[test_result[5]] = ar5;      // Instruction 1.2, address method 00
[test_result[6]] = ar6;      // Instruction 1.2, address method 00
[test_result[7]] = ar7;      // Instruction 1.2, address method 00

ar0 = gr0 addr;              // Instruction 3.1, address method 10 
ar1 = gr1 addr;              // Instruction 3.1, address method 10 
ar2 = gr2 addr;              // Instruction 3.1, address method 10
ar3 = gr3 addr;              // Instruction 3.1, address method 10 
ar4 = gr4 addr;              // Instruction 3.1, address method 10 
ar5 = gr5 addr;              // Instruction 3.1, address method 10 
ar6 = gr6 addr;              // Instruction 3.1, address method 10 
ar7 = gr7 addr;              // Instruction 3.1, address method 10 

gr0 = test_result[8];
[gr0] = ar7,gr7;             // Instruction 1.1, address method 000

gr1 = test_result[9];
[gr1]  = ar6,gr6;            // Instruction 1.1, address method 000

gr2 = test_result[10];
[gr2] = ar5,gr5;             // Instruction 1.1, address method 000

gr3 = test_result[11];     
[gr3] = ar4,gr4;             // Instruction 1.1, address method 000

gr0 = ar0;                   // Restore registers
gr1 = ar1;                   // Restore registers 
gr2 = ar2;                   // Restore registers 
gr3 = ar3;                   // Restore registers 

gr4 = test_result[12];
[gr4] = ar3,gr3;             // Instruction 1.1, address method 000

gr5 = test_result[13];  
[gr5] = ar2,gr2;             // Instruction 1.1, address method 000

gr6 = test_result[14];
[gr6] = ar1,gr1;             // Instruction 1.1, address method 000

gr7 = test_result[15];
[gr7] = ar0,gr0;             // Instruction 1.1, address method 000

gr4 = ar4;                   // Restore registers 
gr5 = ar5;                   // Restore registers 
gr6 = ar6;                   // Restore registers 
gr7 = ar7;                   // Restore registers 

ar0 = ar1 addr;              // Instruction 3.1, address method 00 
ar1 = ar2 addr;              // Instruction 3.1, address method 00 
ar2 = ar3 addr;              // Instruction 3.1, address method 00 
ar3 = ar0 addr;              // Instruction 3.1, address method 00 

ar4 = test_result[16];     
[ar4] = gr0,ar0;             // Instruction 1.1, address method 100
ar4 = ar5;                   // Restore registers 

ar5 = test_result[17];    
[ar5] = gr1,ar1;             // Instruction 1.1, address method 100
ar5 = ar6;                   // Restore registers 

ar6 = test_result[18];   
[ar6] = gr2,ar2;             // Instruction 1.1, address method 100
ar6 = ar7;                   // Restore registers 

ar7 = test_result[19];    
[ar7] = gr3,ar3;             // Instruction 1.1, address method 100
ar7 = ar4;                   // Restore registers 


ar3 = test_result[20];  
[ar3] = gr4,ar4;             // Instruction 1.1, address method 100   

ar2 = test_result[21];  
[ar2] = gr5,ar5;             // Instruction 1.1, address method 100

ar1 = test_result[22];    
[ar1] = gr6,ar6;             // Instruction 1.1, address method 100

ar0 = test_result[23];    
[ar0] = gr7,ar7;             // Instruction 1.1, address method 100

ar7 = [save_stack];

// The program of given and control dimension comparison 
	ar0 = test_result;
	ar1 = contr_result;
  	gr0 = 48;

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
