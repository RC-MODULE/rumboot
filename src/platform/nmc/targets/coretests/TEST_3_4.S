// Test 3.4. Address ALU checking. Analog of the test 3.3.

//import from macros640x;
import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test

// Operand data dimension

array : word[2] = (0ffffffffh dup 2);

// Test result dimension

test_result : word[34] = ( -1 dup 34);

// The control value dimension

contr_result : word[34] = ( 
	00000000h dup 2,          // Lines 1-4
	55555555h, 0aaaaaaaah,    // Lines 5-9
	0ffffffffh, 0fffffffeh,   // Lines 11-14
	0aaaaaaaah, 55555554h,    // Lines 16-19
	0fffffffeh dup 4,         // Lines 21-35
  55555554h, 0aaaaaaa9h,    // Lines 40 - 45
	0f01e1e1dh, 0fe1e1e2h,    // Lines 49 - 54
	0bc787879h, 43878785h,    // Lines 58 - 63
	1e3c7c3dh, 0e1c3c3c1h,    // Lines 67 - 72
	0ff0f0f05h, 00f0f0f9h,    // Lines 76 - 81
	000ff005h, 0fff00ff9h,    // Lines 85 - 90
	07f80770h, 0f807f870h,    // Lines 94 - 99
	0007f870h, 0fff8078eh,    // Lines 103 - 108
	0000008eh, 0ffffff7fh,    // Lines 112 - 117
	00ff7fffh, 0ff007fffh,    // Lines 121 - 126
	007f7fffh, 0ff807fffh     // Lines 130 - 135
	 ) ;

save_stack : word = 0;

end data_test;

global test_3_4 : label;
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
// Lines 1-4

ar4 = 0 addr;                     // Instruction 3.2, address method 10
ar7 = test_result  addr;
ar5 = ar4 + 0;                    // Instruction 3.2, address method 01
[ar7 ++] = ar4;                   // Instruction 1.1, address method 111
[ar7 ++] = ar5;                   // Instruction 1.1, address method 111

// Lines 5-9

ar5 = 55555555h addr;             // Instruction 3.2, address method 10
ar6 = test_result[2] addr;    
[ar6] = ar5;                      // Instruction 1.1, address method 100
gr6 = test_result[3] + 55555556h;
ar6 = ar5 + 55555555h;            // Instruction 3.2, address method 01
[ar6 += gr6] = ar6;               // Instruction 1.1, address method 001

// Lines 11-14

ar6 = 0ffffffffh addr;            // Instruction 3.2, address method 10
ar7 = test_result[4];    
[ar7] = ar6;                      // Instruction 1.1, address method 100
ar7 = ar6 + 0ffffffffh;           // Instruction 3.2, address method 01
ar6 = test_result[5] addr;
[ar6] = ar7;                      // Instruction 1.1, address method 100

// Lines 16-19

ar7 = 0aaaaaaaah addr;
ar4 = ar7 + 0aaaaaaaah;           // Instruction 3.2, address method 01
gr7 = test_result[6] + 55555556h;
[ar7 += gr7] = ar7;               // Instruction 1.1, address method 001
ar7 = test_result[7] addr; 
[ar7 ++ gr7] = ar4;               // Instruction 1.1, address method 101

// Lines 21-23

gr4,ar4 = [array];                // Instruction 1.2, address method 00
ar5 = ar4 + gr4;                  // Instruction 3.1, address method 01
gr4 = test_result[8];        
[gr4] = ar5;                      // Instruction 1.1, address method 000

// Lines 24-27

gr5,ar5 = [array];                // Instruction 1.2, address method 00
ar6 = ar5 + gr5;                  // Instruction 3.1, address method 01
ar5 = test_result[9] addr;   
[ar5] = ar6;                      // Instruction 1.1, address method 100

// Lines 28-31

gr6,ar6 = [array];                // Instruction 1.2, address method 00
ar7 = ar6 + gr6;                  // Instruction 3.1, address method 01
gr6 = test_result[10];   
[gr6] = ar7;                      // Instruction 1.1, address method 000

// Lines 32-35

gr7,ar7 = [array];                // Instruction 1.2, address method 00 
ar4 = ar7 + gr7;                  // Instruction 3.1, address method 01
ar7 = test_result[11] addr;            // ar4 - ???
[ar7] = ar4;                      // Instruction 1.1, address method 100

// Lines 36-40

ar7 = 0ffffffffh addr;            // Instruction 3.2, address method 10
ar6 = ar7 + 55555555h;            // Instruction 3.2, address method 01
[test_result[12]] = ar6;          // Instruction 1.2, address method 00

// Lines 42-45

ar6 = 0ffffffffh addr;            // Instruction 3.2, address method 10
ar5 = ar6 + 0aaaaaaaah;           // Instruction 3.2, address method 01
[test_result[13]] = ar5;          // Instruction 1.2, address method 00

// Lines 46-49

ar5 = 00202020h addr;             // Instruction 3.2, address method 10
ar4 = ar5 + 0effdfdfdh;           // Instruction 3.2, address method 01
[test_result[14]] = ar4;          // Instruction 1.2, address method 00

// Lines 51-54

ar4 = 0ffdfdfdfh addr;            // Instruction 3.2, address method 10
ar7 = ar4 + 10020203h;            // Instruction 3.2, address method 01
[test_result[15]] = ar7;          // Instruction 1.2, address method 00

// Lines 55-58

ar4 = 0fbf7f7f7h addr;            // Instruction 3.2, address method 10
ar4 += 0c0808082h;                // Instruction 3.2, address method 01
[test_result[16]] = ar4;          // Instruction 1.2, address method 00

// Lines 60-63

ar5 = 3f7f7f7dh addr;             // Instruction 3.2, address method 10
ar5 += 04080808h;                 // Instruction 3.2, address method 01
[test_result[17]] = ar5;          // Instruction 1.2, address method 00

// Lines 64-67

ar6 = 3dfbfbfbh addr;             // Instruction 3.2, address method 10
ar6 += 0e0408042h;                // Instruction 3.2, address method 01
[test_result[18]] = ar6;          // Instruction 1.2, address method 00

// Lines 69-72

ar7 = 0dfbfbfbdh addr;            // Instruction 3.2, address method 10
ar7 += 02040404h;                 // Instruction 3.2, address method 01
[test_result[19]] = ar7;          // Instruction 1.2, address method 00

// Lines 73-76

ar4 = 3efefef3h addr;             // Instruction 3.2, address method 10
ar4 += 0c0101012h;                // Instruction 3.2, address method 01
[test_result[20]] = ar4;          // Instruction 1.2, address method 00

// Lines 78-81

ar5 = 0ffefefedh addr;            // Instruction 3.2, address method 10
ar5 += 0101010ch;                 // Instruction 3.2, address method 01
[test_result[21]] = ar5;          // Instruction 1.2, address method 00

// Lines 82-85

ar6 = 3fffeffbh addr;             // Instruction 3.2, address method 10
ar6 += 0c010000ah;                // Instruction 3.2, address method 01
[test_result[22]] = ar6;          // Instruction 1.2, address method 00

// Lines 87-90

ar7 = 00001004h addr;             // Instruction 3.2, address method 10
ar7 += 0ffeffff5h;                // Instruction 3.2, address method 01
[test_result[23]] = ar7;          // Instruction 1.2, address method 00

// Lines 91-94

ar7 = 08000800h addr;             // Instruction 3.2, address method 10
ar7 += 0fff7ff70h;                // Instruction 3.2, address method 01
[test_result[24]] = ar7;          // Instruction 1.2, address method 00

// Lines 96-99

ar6 = 00080080h addr;             // Instruction 3.2, address method 10
ar6 += 0f7fff7f0h;                // Instruction 3.2, address method 01
[test_result[25]] = ar6;          // Instruction 1.2, address method 00

// Lines 100-103

ar5 = 00100880h addr;             // Instruction 3.2, address method 10
ar5 += 0fff7eff0h;                // Instruction 3.2, address method 01
[test_result[26]] = ar5;          // Instruction 1.2, address method 00

// Lines 105-108

ar4 = 0008100fh addr;             // Instruction 3.2, address method 10 
ar4 += 0ffeff77fh;                // Instruction 3.2, address method 01
[test_result[27]] = ar4;          // Instruction 1.2, address method 00 

// Lines 109-112

ar4 = 0000008fh addr;             // Instruction 3.2, address method 10 
ar4 += 0ffffffffh;                // Instruction 3.2, address method 01
[test_result[28]] = ar4;          // Instruction 1.2, address method 00 

// Lines 114-117

ar5 = 0000000ah addr;             // Instruction 3.2, address method 10 
ar5 += 0ffffff75h;                // Instruction 3.2, address method 01
[test_result[29]] = ar5;          // Instruction 1.2, address method 00

// Lines 118-121

ar6 = 01000005h addr;             // Instruction 3.2, address method 10 
ar6 += 0ffff7ffah;                // Instruction 3.2, address method 01
[test_result[30]] = ar6;          // Instruction 1.2, address method 00 

// Lines 123-126

ar7 = 00008006h addr;             // Instruction 3.2, address method 10 
ar7 += 0fefffff9h;                // Instruction 3.2, address method 01
[test_result[31]] = ar7;          // Instruction 1.2, address method 00 

// Lines 127-130

ar7 = 01000006h addr;             // Instruction 3.2, address method 10 
ar7 += 0ff7f7ff9h;                // Instruction 3.2, address method 01
[test_result[32]] = ar7;          // Instruction 1.2, address method 00 

// Lines 132-135

ar6 = 00808003h addr;             // Instruction 3.2, address method 10 
ar6 += 0fefffffch;                // Instruction 3.2, address method 01
[test_result[33]] = ar6;          // Instruction 1.2, address method 00 

nul 17;                           // Instruction 3.4, line 137


ar7 = [save_stack];


// Program of given and control dimension comparison

	ar0 = test_result;
	ar1 = contr_result;
  	gr0 = 34;

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





