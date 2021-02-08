// Test 4.6. Flag V Checking (Shift Instructions)

//import from macros640x;

import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test
// The operand data dimension
array : word[138] = (							//Shift
	 0h,   0f0f0f0f0h,   0fffffffeh,   1h,   0fffffffch,   3h,	// 31
	0f0f0f0f0h,	0fffffffeh,	        1h,	0fffffffch,	// 30
	        3h,	0fffffffch,	        3h,	0fffffff8h,	// 29
	        7h,	0fffffff8h,	        7h,	0fffffff0h,	// 28
	       0fh,	0fffffff0h,	       0fh,	0ffffffe0h,	// 27
	       1fh,	0ffffffe0h,	       1fh,	0ffffffc0h,	// 26
	       3fh,	0ffffffc0h,	       3fh,	0ffffff80h,	// 25
	       7fh,	0ffffff80h,	       7fh,	0ffffff00h,	// 24
	      0ffh,	0ffffff00h,	      0ffh,	0fffffe00h,	// 23
	      1ffh,	0fffffe00h,	      1ffh,	0fffffc00h,	// 22
	      3ffh,	0fffffc00h,	      3ffh,	0fffff800h,	// 21
	      7ffh,	0fffff800h,	      7ffh,	0fffff000h,	// 20
	     0fffh,	0fffff000h,	     0fffh,	0ffffe000h,	// 19
	     1fffh,	0ffffe000h,	     1fffh,	0ffffc000h,	// 18
	     3fffh,	0ffffc000h,	     3fffh,	0ffff8000h,	// 17
	     7fffh,	0ffff8000h,	     7fffh,	0ffff0000h,	// 16
	    0ffffh,	0ffff0000h,	    0ffffh,	0fffe0000h,	// 15
	    1ffffh,	0fffe0000h,	    1ffffh,	0fffc0000h,	// 14
	    3ffffh,	0fffc0000h,	    3ffffh,	0fff80000h,	// 13
	    7ffffh,	0fff80000h,	    7ffffh,	0fff00000h,	// 12
	   0fffffh,	0fff00000h,	   0fffffh,	0ffe00000h,	// 11
	   1fffffh,	0ffe00000h,	   1fffffh,	0ffc00000h,	// 10
	   3fffffh,	0ffc00000h,	   3fffffh,	0ff800000h,	//  9
	   7fffffh,	0ff800000h,	   7fffffh,	0ff000000h,	//  8
	  0ffffffh,	0ff000000h,	  0ffffffh,	0fe000000h,	//  7
	  1ffffffh,	0fe000000h,	  1ffffffh,	0fc000000h,	//  6
	  3ffffffh,	0fc000000h,	  3ffffffh,	0f8000000h,	//  5
	  7ffffffh,	0f8000000h,	  7ffffffh,	0f0000000h,	//  4
	 0fffffffh,	0f0000000h,	 0fffffffh,	0e0000000h,	//  3
	 1fffffffh,	0e0000000h,	 1fffffffh,	0c0000000h,	//  2
	 3fffffffh,	0c0000000h,	 3fffffffh,	 80000000h,	//  1
	 7fffffffh,	 80000000h,	 7fffffffh,	        0h,	//  0
	0ffffffffh,	        0h,	0ffffffffh,	        0h,	//  0
	0ffffffffh,	        0h,	0ffffffffh,	        0h	// No Shift 
	);

// The test result dimension
test_result : word[137] = ( -1 dup 137);
pswr_result : word[137] = ( -1 dup 137);

// The control value dimensions
contr_result : word[137] = (
	0,	        80000000h,      80000000h,      0,     80000000h,   0,
	80000000h,	80000000h,	40000000h,	80000000h,	// 30
	60000000h,	80000000h,	60000000h,	80000000h,	// 29
	70000000h,	80000000h,	70000000h,	80000000h,	// 28
	78000000h,	80000000h,	78000000h,	80000000h,	// 27 
	7c000000h,	80000000h,	7c000000h,	80000000h,	// 26
	7e000000h,	80000000h,	7e000000h,	80000000h,	// 25
	7f000000h,	80000000h,	7f000000h,	80000000h,	// 24
	7f800000h,	80000000h,	7f800000h,	80000000h,	// 23
	7fc00000h,	80000000h,	7fc00000h,	80000000h,	// 22
	7fe00000h,	80000000h,	7fe00000h,	80000000h,	// 21
	7ff00000h,	80000000h,	7ff00000h,	80000000h,	// 20
	7ff80000h,	80000000h,	7ff80000h,	80000000h,	// 19
	7ffc0000h,	80000000h,	7ffc0000h,	80000000h,	// 18
	7ffe0000h,	80000000h,	7ffe0000h,	80000000h,	// 17
	7fff0000h,	80000000h,	7fff0000h,	80000000h,	// 16
	7fff8000h,	80000000h,	7fff8000h,	80000000h,	// 15
	7fffc000h,	80000000h,	7fffc000h,	80000000h,	// 14
	7fffe000h,	80000000h,	7fffe000h,	80000000h,	// 13
	7ffff000h,	80000000h,	7ffff000h,	80000000h,	// 12
	7ffff800h,	80000000h,	7ffff800h,	80000000h,	// 11
	7ffffc00h,	80000000h,	7ffffc00h,	80000000h,	// 10
	7ffffe00h,	80000000h,	7ffffe00h,	80000000h,	//  9
	7fffff00h,	80000000h,	7fffff00h,	80000000h,	//  8
	7fffff80h,	80000000h,	7fffff80h,	80000000h,	//  7
	7fffffc0h,	80000000h,	7fffffc0h,	80000000h,	//  6
	7fffffe0h,	80000000h,	7fffffe0h,	80000000h,	//  5
	7ffffff0h,	80000000h,	7ffffff0h,	80000000h,	//  4
	7ffffff8h,	80000000h,	7ffffff8h,	80000000h,	//  3
	7ffffffch,	80000000h,	7ffffffch,	80000000h,	//  2
	7ffffffeh,	80000000h,	7ffffffeh,	80000000h,	//  1
	80000000h,	80000000h,	80000000h,	80000000h,	//  0
	80000000h,	80000000h,	80000000h,	80000000h,	//  0
	80000000h,	80000000h,	80000000h	// No Shift 
	 ) ;

control_pswr : word[137] = (
	4,	0ah,	0ah,	07h,	0ah,	4,
	0ah,	09h,	0h,	0ah,	// A<<30
	0h,	09h,	0h,	0ah,	// A<<29
	0h,	09h,	0h,	0ah,	// A<<28
	0h,	09h,	0h,	0ah,	// A<<27
	0h,	09h,	0h,	0ah,	// A<<26
	0h,	09h,	0h,	0ah,	// A<<25
	0h,	09h,	0h,	0ah,	// A<<24
	0h,	09h,	0h,	0ah,	// A<<23
	0h,	09h,	0h,	0ah,	// A<<22
	0h,	09h,	0h,	0ah,	// A<<21
	0h,	09h,	0h,	0ah,	// A<<20
	0h,	09h,	0h,	0ah,	// A<<19
	0h,	09h,	0h,	0ah,	// A<<18
	0h,	09h,	0h,	0ah,	// A<<17
	0h,	09h,	0h,	0ah,	// A<<16
	0h,	09h,	0h,	0ah,	// A<<15
	0h,	09h,	0h,	0ah,	// A<<14
	0h,	09h,	0h,	0ah,	// A<<13
	0h,	09h,	0h,	0ah,	// A<<12
	0h,	09h,	0h,	0ah,	// A<<11
	0h,	09h,	0h,	0ah,	// A<<10
	0h,	09h,	0h,	0ah,	// A<<9
	0h,	09h,	0h,	0ah,	// A<<8
	0h,	09h,	0h,	0ah,	// A<<7
	0h,	09h,	0h,	0ah,	// A<<6
	0h,	09h,	0h,	0ah,	// A<<5
	0h,	09h,	0h,	0ah,	// A<<4
	0h,	09h,	0h,	0ah,	// A<<3
	0h,	09h,	0h,	0ah,	// A<<2
	0h,	09h,	0h,	0ah,	// A<<1
	0ah,	0ah,	0ah,	0ah,	// A<<0
	0ah,	0ah,	0ah,	0ah,	// A<<0
	0ah,	0ah,	0ah		// No Shift 
	 ) ;

save_stack : word = 0;
end data_test;

global test_4_6 : label;
   global start : label;
		weak mytest : label;
		
						 weak mytest_additional: label;		
Loop, Loop1, Normal, Error, Finish : label;

// The boot loader								 
begin ".text.init"
<test_4_6>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

    		gr0 = 0h;															     // GP registers definition
    		gr1 = 0h;
    		gr2 = 0h;
    		gr3 = 0h;
    		gr4 = 0h;
    		gr5 = 0h;
    		gr6 = 0h;
    		gr7 = 0h;      		
				goto mytest_additional;								      // Jump to internal memory bank 1
				
// The main program 
<mytest_additional>
ar7 = array;
ar6 = test_result;
ar5 = pswr_result;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A>> 31;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 31;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 31;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 31;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 31;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A>> 31;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 30;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 30;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 30;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 30;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 29;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 29;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 29;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 29;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 28;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 28;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 28;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 28;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 27;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 27;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 27;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 27;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 26;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 26;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 26;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 26;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 25;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 25;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 25;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 25;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 24;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 24;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 24;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 24;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 23;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 23;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 23;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 23;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 22;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 22;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 22;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 22;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 21;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 21;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 21;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 21;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 20;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 20;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 20;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 20;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 19;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 19;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 19;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 19;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 18;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 18;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 18;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 18;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 17;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 17;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 17;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 17;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 16;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 16;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 16;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 16;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 15;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 15;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 15;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 15;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 14;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 14;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 14;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 14;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 13;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 13;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 13;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 13;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 12;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 12;
   [ar6++] = gr1;
   [ar5++] = pswr;



   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 12;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 12;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 11;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 11;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 11;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 11;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 10;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 10;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 10;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 10;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 9;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 9;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 9;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 9;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 8;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 8;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 8;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 8;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 7;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 7;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 7;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 7;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 6;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 6;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 6;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 6;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 5;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 5;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 5;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 5;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 4;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 4;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 4;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 4;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 3;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 3;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 3;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 3;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 2;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 2;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 2;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 2;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 1;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 1;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 1;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 1;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 0;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 0;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 0;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 0;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 0;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 0;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 0;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul  with gr1 = gr0 A<< 0;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul;
   [ar6++] = gr1;
   [ar5++] = pswr;

   gr0 = [ar7++];
   nul;

// The program of given and control dimension comparison 

	ar0 = test_result;
	ar1 = contr_result;
  	gr0 = 137;

<Loop>
	gr1 = [ ar0++ ];
	gr2 = [ ar1++ ];
	with gr2 - gr1;
	if <>0 goto Error;
	with gr0--;
	if <>0 goto Loop;

// The program of given and control pswr dimension comparison 

		ar0 = pswr_result;
		ar1 = control_pswr;
  		gr0 = 137;
		gr3 = 1fh;

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
