// Test 4.5 (followed by Test 4.4). 
// Execute conditional jump, call, return from subroutine, return from interrupt

//import from macros640x;

import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test
// The operand data dimension
array : word[12] = (
	0,
	0111b,	0100b,	1001b,	0010b,	1101b,
	0001b,	1110b,	0000b,	0111b,	1000b,
	1111b
			 );
// The test result dimension
pswr_result : word[5] = ( -1 dup 5);
//The control value dimension
control_pswr : word[5] = ( 
	1000b,	1000b,	1000b,	1000b,	1000b	
		);

save_stack : word = 0;
end data_test;


begin ".text.init"
global test_4_5 : label;
   global start : label;
   
	weak mytest  : label;
	weak Label1  : label;
	weak Label2  : label;
	weak Label3  : label;
	weak Label4  : label;
	weak Label5  : label;
	weak Label6  : label;
	weak Label7  : label;
	weak Label8  : label;
	weak Label9  : label;
	weak Label10 : label;
	weak Label11 : label;
	weak Label12 : label;
	weak Label13 : label;
	weak Label14 : label;
	weak Label15 : label;
	weak Label16 : label;
	weak Label17 : label;
	weak Label18 : label;
	weak Label19 : label;
	weak Label20 : label;
	weak Label21 : label;
	weak Return  : label;
	weak Ireturn : label;
	
Loop,  Normal, Error, Finish : label;	



<test_4_5>
<start>
<mytest>
rumboot_nmc_init_core();
rumboot_runtime_init(016800h);

ar7 = save_stack + 2;									      // Deternine ar7 (stack pointer)

ar3 = pswr_result;


gr7 = Ireturn;
gr6 = 1000b;
push gr7;
push gr6;

ireturn;		// Return from interrupt (instruction 4.3)

goto Label1;	// If the instruction ireturn has not been executed

<Ireturn>
    goto Label22;
    nul;
    nul;
    goto Label1;

<Label22>
gr7 = Return;
push gr7;
push gr6;

return;		// Return from subroutine (instruction 4.3)
goto Label1;

goto Label1;

<Return>

[ar3] = pswr;

ar1 = Label1;
ar4 = Label1;
ar7 = [save_stack];

if false goto ar1;	// Instruction 4.1, condition 1111, ma 00, ar1 -> PC

ar3 = pswr_result[1];
[ar3] = pswr;

if false call ar4;	// Instruction 4.1, condition 1111, ma 00, ar4 -> PC

ar3 = pswr_result[2];
[ar3] = pswr;

if false ireturn;		// Instruction 4.3, condition 1111

ar3 = pswr_result[3];
[ar3] = pswr;

if false return;		// Instruction 4.3, condition 1111

ar3 = pswr_result[4];
[ar3] = pswr;		// pswr = 1000



if >= goto Label1;// Instruction 4.2, ma 10, condition n=0 (jump has not been executed)

if < goto Label2;	// Instruction 4.2, ma 10, condition n=1 (jump has been executed)

goto Label1;		 	// If jump has not been executed 

<Label2>
	pswr = [array[1]];	// Set pswr = 0111
if >= goto Label3;		// Instruction 4.2, ma 10, condition n=0 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label3>
if < goto Label1;		// Instruction 4.2, ma 10, condition n=1 (jump has not been executed)



	pswr = [array[2]];	// Set pswr = 0100
if <>0 goto Label1;		// Instruction 4.2, ma 10, condition z=0 (jump has not been executed)

if =0 goto Label4;		// Instruction 4.2, ma 10, condition z=1 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label4>
pswr = [array[3]];		// Set pswr = 1001
if <>0 goto Label5;		// Instruction 4.2, ma 10, condition z=0 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label5>
if =0 goto Label1;		// Instruction 4.2, ma 10, condition z=1 (jump has not been executed)



	pswr = [array[4]];	// Set pswr = 0010
if vfalse goto Label1;		// Instruction 4.2, ma 10, condition v=0 (jump has not been executed)

if vtrue goto Label6;		// Instruction 4.2, ma 10, condition v=1 (jump has been executed)

goto Label1;			// If jump has not executed 

<Label6>
	pswr = [array[5]];	// Set pswr = 1101
if vfalse goto Label7;		// Instruction 4.2, ma 10, condition v=0 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label7>
if vtrue goto Label1;		// Instruction 4.2, ma 10, condition v=1 (jump has not been executed)




	pswr = [array[6]];	// Set pswr = 0001
if not carry goto Label1;	// Instruction 4.2, ma 10, condition c=0 (jump has not been executed)

if carry goto Label8;		// Instruction 4.2, ma 10, condition c=1 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label8>
	pswr = [array[7]];	// Set pswr = 1110
if not carry goto Label9;	// Instruction 4.2, ma 10, condition c=0 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label9>
if carry goto Label1;		// Instruction 4.2, ma 10, condition c=1 (jump has not been executed)




pswr = [array[8]];		// Set pswr = 0000
if > goto Label10;		// n+z = 0 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label10>
if v> goto Label11;		// (n xor v) +z = 0 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label11>
if v>= goto Label12;		// n xor v = 0 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label12>
if <= goto Label1;		// n+z = 1 (jump has not been executed)
if v<= goto Label1;		// (n xor v) +z = 1 (jump has not been executed)
if v< goto Label1;		// n xor v = 1 (jump has not been executed)


	pswr = [array[9]];	// Set pswr = 0111
if > goto Label1;			// n+z = 0 (jump has not been executed)
if v> goto Label1;		// (n xor v) +z = 0 (jump has not been executed)
if v>= goto Label1;		// n xor v = 0 (jump has not been executed)

if <= goto Label13;		// n+z = 1 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label13>
if v<= goto Label14;		// (n xor v) +z = 1 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label14>
if v< goto Label15;		// n xor v = 1 (jump has been executed)
goto Label1;			// If jump has not been executed 


<Label15>
pswr = [array[10]];		// Set pswr = 1000
if > goto Label1;			// n+z = 0 (jump has not been executed)
if v> goto Label1;		// (n xor v) +z = 0 (jump has not been executed)
if v>= goto Label1;		// n xor v = 0 (jump has not been executed)

if <= goto Label16;		// n+z = 1 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label16>
if v<= goto Label17;		// (n xor v) +z = 1 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label17>
if v< goto Label18;		// n xor v = 1 (jump has been executed)
goto Label1;			// If jump has not been executed 


<Label18>
	pswr = [array[11]];	// Set pswr = 1111
if > goto Label1;			// n+z = 0 (jump has not been executed)
if v> goto Label1;		// (n xor v) +z = 0 (jump has not been executed)
if v>= goto Label19;		// n xor v = 0 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label19>
if <= goto Label20;		// n+z = 1 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label20>
if v<= goto Label21;		// (n xor v) +z = 1 (jump has been executed)
goto Label1;			// If jump has not been executed 

<Label21>
if v< goto Label1;		// n xor v = 1 (jump has not been executed)

//ar7 = [save_stack];

// The program of given and control dimension comparison

	ar0 = pswr_result;
	ar1 = control_pswr;
  	gr0 = 5;
	gr3 = 0fh;

<Loop>
	gr1 = [ ar0++ ];
	with gr1 = gr1 and gr3;
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
	      
<Label1>
		    nul;
		    nul;
		    nul;
		    nul;

<Error>
				rumboot_exit(0_16800h, 1);
				//gr1 = 01h;
	      //ar0 = STOP_SIMULATION;
	      //[ar0] = gr1;	                       // Set 01h into the location 1_6800h (IM3), if the test is incorrect
	  
<Finish>
		    halt;										             // Wait (test finished)
     
end ".text.init";


