// Test 4.4. Scalar Shift Checking (Followed by Test 4.3) 

//import from macros640x;

import from "platform/rumboot.mlb";

const STOP_SIMULATION = 01_6800h;                 // Address to write to stop simulation

data data_test
// The data operand dimension
array : word[6] = (
				0fe0f0e32h,	01f0f1cdh,	0ffffffffh,
				7fffffffh,	80000001h,	0
			 );

// The test result dimension
test_result : word[341] = ( -1 dup 341);
pswr_result : word[15] = ( -1 dup 15);
// The control value dimension
contr_result : word[341] = (
//	11111110000011110000111000110010b = fe0f0e32h
	0,
	0,						// 0 bit cyclic shift left = NOP
	11111100000111100001110001100101b,	// 1 bit cyclic shift left 
	11111000001111000011100011001011b,	// 2 bit cyclic shift left 
	11110000011110000111000110010111b,	// 3 bit cyclic shift left 
	11100000111100001110001100101111b,	// 4 bit cyclic shift left 
	11000001111000011100011001011111b,	// 5 bit cyclic shift left 
	10000011110000111000110010111111b,	// 6 bit cyclic shift left 
	00000111100001110001100101111111b,	// 7 bit cyclic shift left 
	00001111000011100011001011111110b,	// 8 bit cyclic shift left 
	00011110000111000110010111111100b,	// 9 bit cyclic shift left 
	00111100001110001100101111111000b,	// 10 bit cyclic shift left 
	01111000011100011001011111110000b,	// 11 bit cyclic shift left 
	11110000111000110010111111100000b,	// 12 bit cyclic shift left 
	11100001110001100101111111000001b,	// 13 bit cyclic shift left 
	11000011100011001011111110000011b,	// 14 bit cyclic shift left 
	10000111000110010111111100000111b,	// 15 bit cyclic shift left 
	00001110001100101111111000001111b,	// 16 bit cyclic shift left 
	00011100011001011111110000011110b,	// 17 bit cyclic shift left 
	00111000110010111111100000111100b,	// 18 bit cyclic shift left 
	01110001100101111111000001111000b,	// 19 bit cyclic shift left 
	11100011001011111110000011110000b,	// 20 bit cyclic shift left 
	11000110010111111100000111100001b,	// 21 bit cyclic shift left 
	10001100101111111000001111000011b,	// 22 bit cyclic shift left 
	00011001011111110000011110000111b,	// 23 bit cyclic shift left 
	00110010111111100000111100001110b,	// 24 bit cyclic shift left 
	01100101111111000001111000011100b,	// 25 bit cyclic shift left 
	11001011111110000011110000111000b,	// 26 bit cyclic shift left 
	10010111111100000111100001110001b,	// 27 bit cyclic shift left 
	00101111111000001111000011100011b,	// 28 bit cyclic shift left 
	01011111110000011110000111000110b,	// 29 bit cyclic shift left 
	10111111100000111100001110001100b,	// 30 bit cyclic shift left 
	01111111000001111000011100011001b,	// 31 bit cyclic shift left 


	11111100000111100001110001100101b,

	11111100000111100001110001100101b,	// 0 bit cyclic shift right 
	01111111000001111000011100011001b,	// 1 bit cyclic shift right 
	10111111100000111100001110001100b,	// 2 bit cyclic shift right 
	01011111110000011110000111000110b,	// 3 bit cyclic shift right 
	00101111111000001111000011100011b,	// 4 bit cyclic shift right 
	10010111111100000111100001110001b,	// 5 bit cyclic shift right 
	11001011111110000011110000111000b,	// 6 bit cyclic shift right 
	01100101111111000001111000011100b,	// 7 bit cyclic shift right 
	00110010111111100000111100001110b,	// 8 bit cyclic shift right 
	00011001011111110000011110000111b,	// 9 bit cyclic shift right 
	10001100101111111000001111000011b,	// 10 bit cyclic shift right 
	11000110010111111100000111100001b,	// 11 bit cyclic shift right 
	11100011001011111110000011110000b,	// 12 bit cyclic shift right 
	01110001100101111111000001111000b,	// 13 bit cyclic shift right 
	00111000110010111111100000111100b,	// 14 bit cyclic shift right 
	00011100011001011111110000011110b,	// 15 bit cyclic shift right 
	00001110001100101111111000001111b,	// 16 bit cyclic shift right 
	10000111000110010111111100000111b,	// 17 bit cyclic shift right 
	11000011100011001011111110000011b,	// 18 bit cyclic shift right 
	11100001110001100101111111000001b,	// 19 bit cyclic shift right 
	11110000111000110010111111100000b,	// 20 bit cyclic shift right 
	01111000011100011001011111110000b,	// 21 bit cyclic shift right 
	00111100001110001100101111111000b,	// 22 bit cyclic shift right 
	00011110000111000110010111111100b,	// 23 bit cyclic shift right 
	00001111000011100011001011111110b,	// 24 bit cyclic shift right 
	00000111100001110001100101111111b,	// 25 bit cyclic shift right 
	10000011110000111000110010111111b,	// 26 bit cyclic shift right 
	11000001111000011100011001011111b,	// 27 bit cyclic shift right 
	11100000111100001110001100101111b,	// 28 bit cyclic shift right 
	11110000011110000111000110010111b,	// 29 bit cyclic shift right 
	11111000001111000011100011001011b,	// 30 bit cyclic shift right 
	11111100000111100001110001100101b,	// 31 bit cyclic shift right 

//	00000001111100001111000111001101b = 01f0f1cdh
	0,
	0,						// 0 bit cyclic shift right = NOP

	10000000111110000111100011100110b,	// 1 bit cyclic shift right 
	01000000011111000011110001110011b,	// 2 bit cyclic shift right 
	10100000001111100001111000111001b,	// 3 bit cyclic shift right 
	11010000000111110000111100011100b,	// 4 bit cyclic shift right 
	01101000000011111000011110001110b,	// 5 bit cyclic shift right 
	00110100000001111100001111000111b,	// 6 bit cyclic shift right 
	10011010000000111110000111100011b,	// 7 bit cyclic shift right 
	11001101000000011111000011110001b,	// 8 bit cyclic shift right 
	11100110100000001111100001111000b,	// 9 bit cyclic shift right 
	01110011010000000111110000111100b,	// 10 bit cyclic shift right 
	00111001101000000011111000011110b,	// 11 bit cyclic shift right 
	00011100110100000001111100001111b,	// 12 bit cyclic shift right 
	10001110011010000000111110000111b,	// 13 bit cyclic shift right 
	11000111001101000000011111000011b,	// 14 bit cyclic shift right 
	11100011100110100000001111100001b,	// 15 bit cyclic shift right 
	11110001110011010000000111110000b,	// 16 bit cyclic shift right 
	01111000111001101000000011111000b,	// 17 bit cyclic shift right 
	00111100011100110100000001111100b,	// 18 bit cyclic shift right 
	00011110001110011010000000111110b,	// 19 bit cyclic shift right 
	00001111000111001101000000011111b,	// 20 bit cyclic shift right 
	10000111100011100110100000001111b,	// 21 bit cyclic shift right 
	11000011110001110011010000000111b,	// 22 bit cyclic shift right 
	11100001111000111001101000000011b,	// 23 bit cyclic shift right 
	11110000111100011100110100000001b,	// 24 bit cyclic shift right 
	11111000011110001110011010000000b,	// 25 bit cyclic shift right 
	01111100001111000111001101000000b,	// 26 bit cyclic shift right 
	00111110000111100011100110100000b,	// 27 bit cyclic shift right 
	00011111000011110001110011010000b,	// 28 bit cyclic shift right 
	00001111100001111000111001101000b,	// 29 bit cyclic shift right 
	00000111110000111100011100110100b,	// 30 bit cyclic shift right 
	00000011111000011110001110011010b,	// 31 bit cyclic shift right 

	10000000111110000111100011100110b,	// 1 bit cyclic shift right 

	10000000111110000111100011100110b,	// 0 bit cyclic shift left 
	00000011111000011110001110011010b,	// 1 bit cyclic shift left 
	00000111110000111100011100110100b,	// 2 bit cyclic shift left 	
	00001111100001111000111001101000b,	// 3 bit cyclic shift left 
	00011111000011110001110011010000b,	// 4 bit cyclic shift left 
	00111110000111100011100110100000b,	// 5 bit cyclic shift left 
	01111100001111000111001101000000b,	// 6 bit cyclic shift left 
	11111000011110001110011010000000b,	// 7 bit cyclic shift left 
	11110000111100011100110100000001b,	// 8 bit cyclic shift left 
	11100001111000111001101000000011b,	// 9 bit cyclic shift left 
	11000011110001110011010000000111b,	// 10 bit cyclic shift left 
	10000111100011100110100000001111b,	// 11 bit cyclic shift left 
	00001111000111001101000000011111b,	// 12 bit cyclic shift left 
	00011110001110011010000000111110b,	// 13 bit cyclic shift left 
	00111100011100110100000001111100b,	// 14 bit cyclic shift left 
	01111000111001101000000011111000b,	// 15 bit cyclic shift left 
	11110001110011010000000111110000b,	// 16 bit cyclic shift left 
	11100011100110100000001111100001b,	// 17 bit cyclic shift left 
	11000111001101000000011111000011b,	// 18 bit cyclic shift left 
	10001110011010000000111110000111b,	// 19 bit cyclic shift left 
	00011100110100000001111100001111b,	// 20 bit cyclic shift left 
	00111001101000000011111000011110b,	// 21 bit cyclic shift left 
	01110011010000000111110000111100b,	// 22 bit cyclic shift left 
	11100110100000001111100001111000b,	// 23 bit cyclic shift left 
	11001101000000011111000011110001b,	// 24 bit cyclic shift left 
	10011010000000111110000111100011b,	// 25 bit cyclic shift left 
	00110100000001111100001111000111b,	// 26 bit cyclic shift left 
	01101000000011111000011110001110b,	// 27 bit cyclic shift left 
	11010000000111110000111100011100b,	// 28 bit cyclic shift left 
	10100000001111100001111000111001b,	// 29 bit cyclic shift left 
	01000000011111000011110001110011b,	// 30 bit cyclic shift left 
	10000000111110000111100011100110b,	// 31 bit cyclic shift left 

	0,
//	11111111111111111111111111111111b = ffffffffh
	0,						// 0 bit logic shift left = NOP
	11111111111111111111111111111110b,	// 1 bit logic shift left 
	11111111111111111111111111111100b,	// 2 bit logic shift left 
	11111111111111111111111111111000b,	// 3 bit logic shift left 
	11111111111111111111111111110000b,	// 4 bit logic shift left 
	11111111111111111111111111100000b,	// 5 bit logic shift left 
	11111111111111111111111111000000b,	// 6 bit logic shift left 
	11111111111111111111111110000000b,	// 7 bit logic shift left 
	11111111111111111111111100000000b,	// 8 bit logic shift left 
	11111111111111111111111000000000b,	// 9 bit logic shift left 
	11111111111111111111110000000000b,	// 10 bit logic shift left 
	11111111111111111111100000000000b,	// 11 bit logic shift left 
	11111111111111111111000000000000b,	// 12 bit logic shift left 
	11111111111111111110000000000000b,	// 13 bit logic shift left 
	11111111111111111100000000000000b,	// 14 bit logic shift left 
	11111111111111111000000000000000b,	// 15 bit logic shift left 
	11111111111111110000000000000000b,	// 16 bit logic shift left 
	11111111111111100000000000000000b,	// 17 bit logic shift left 
	11111111111111000000000000000000b,	// 18 bit logic shift left 
	11111111111110000000000000000000b,	// 19 bit logic shift left 
	11111111111100000000000000000000b,	// 20 bit logic shift left 
	11111111111000000000000000000000b,	// 21 bit logic shift left 
	11111111110000000000000000000000b,	// 22 bit logic shift left 
	11111111100000000000000000000000b,	// 23 bit logic shift left 
	11111111000000000000000000000000b,	// 24 bit logic shift left 
	11111110000000000000000000000000b,	// 25 bit logic shift left 
	11111100000000000000000000000000b,	// 26 bit logic shift left 
	11111000000000000000000000000000b,	// 27 bit logic shift left 
	11110000000000000000000000000000b,	// 28 bit logic shift left 
	11100000000000000000000000000000b,	// 29 bit logic shift left 
	11000000000000000000000000000000b,	// 30 bit logic shift left 
	10000000000000000000000000000000b,	// 31 bit logic shift left 

	10000000000000000000000000000000b,	// 0 bit logic shift right 
	01111111111111111111111111111111b,	// 1 bit logic shift right 
	00111111111111111111111111111111b,	// 2 bit logic shift right 
	00011111111111111111111111111111b,	// 3 bit logic shift right 
	00001111111111111111111111111111b,	// 4 bit logic shift right 
	00000111111111111111111111111111b,	// 5 bit logic shift right 
	00000011111111111111111111111111b,	// 6 bit logic shift right 
	00000001111111111111111111111111b,	// 7 bit logic shift right 
	00000000111111111111111111111111b,	// 8 bit logic shift right 
	00000000011111111111111111111111b,	// 9 bit logic shift right 
	00000000001111111111111111111111b,	// 10 bit logic shift right 
	00000000000111111111111111111111b,	// 11 bit logic shift right 
	00000000000011111111111111111111b,	// 12 bit logic shift right 
	00000000000001111111111111111111b,	// 13 bit logic shift right 
	00000000000000111111111111111111b,	// 14 bit logic shift right 
	00000000000000011111111111111111b,	// 15 bit logic shift right 
	00000000000000001111111111111111b,	// 16 bit logic shift right 
	00000000000000000111111111111111b,	// 17 bit logic shift right 
	00000000000000000011111111111111b,	// 18 bit logic shift right 
	00000000000000000001111111111111b,	// 19 bit logic shift right 
	00000000000000000000111111111111b,	// 20 bit logic shift right 
	00000000000000000000011111111111b,	// 21 bit logic shift right 
	00000000000000000000001111111111b,	// 22 bit logic shift right 
	00000000000000000000000111111111b,	// 23 bit logic shift right 
	00000000000000000000000011111111b,	// 24 bit logic shift right 
	00000000000000000000000001111111b,	// 25 bit logic shift right 
	00000000000000000000000000111111b,	// 26 bit logic shift right 
	00000000000000000000000000011111b,	// 27 bit logic shift right 
	00000000000000000000000000001111b,	// 28 bit logic shift right 
	00000000000000000000000000000111b,	// 29 bit logic shift right 
	00000000000000000000000000000011b,	// 30 bit logic shift right 
	00000000000000000000000000000001b,	// 31 bit logic shift right 

//	01111111111111111111111111111111b = 7fffffffh
	0,
	0,						// 0 bit arithmetic shift right = NOP
	00111111111111111111111111111111b,	// 1 bit arithmetic shift right 
	00011111111111111111111111111111b,	// 2 bit arithmetic shift right 
	00001111111111111111111111111111b,	// 3 bit arithmetic shift right 
	00000111111111111111111111111111b,	// 4 bit arithmetic shift right 
	00000011111111111111111111111111b,	// 5 bit arithmetic shift right 
	00000001111111111111111111111111b,	// 6 bit arithmetic shift right 
	00000000111111111111111111111111b,	// 7 bit arithmetic shift right 
	00000000011111111111111111111111b,	// 8 bit arithmetic shift right 
	00000000001111111111111111111111b,	// 9 bit arithmetic shift right 
	00000000000111111111111111111111b,	// 10 bit arithmetic shift right 
	00000000000011111111111111111111b,	// 11 bit arithmetic shift right 
	00000000000001111111111111111111b,	// 12 bit arithmetic shift right 
	00000000000000111111111111111111b,	// 13 bit arithmetic shift right 
	00000000000000011111111111111111b,	// 14 bit arithmetic shift right 
	00000000000000001111111111111111b,	// 15 bit arithmetic shift right 
	00000000000000000111111111111111b,	// 16 bit arithmetic shift right 
	00000000000000000011111111111111b,	// 17 bit arithmetic shift right 
	00000000000000000001111111111111b,	// 18 bit arithmetic shift right 
	00000000000000000000111111111111b,	// 19 bit arithmetic shift right 
	00000000000000000000011111111111b,	// 20 bit arithmetic shift right 
	00000000000000000000001111111111b,	// 21 bit arithmetic shift right 
	00000000000000000000000111111111b,	// 22 bit arithmetic shift right 
	00000000000000000000000011111111b,	// 23 bit arithmetic shift right 
	00000000000000000000000001111111b,	// 24 bit arithmetic shift right 
	00000000000000000000000000111111b,	// 25 bit arithmetic shift right 
	00000000000000000000000000011111b,	// 26 bit arithmetic shift right 
	00000000000000000000000000001111b,	// 27 bit arithmetic shift right 
	00000000000000000000000000000111b,	// 28 bit arithmetic shift right 
	00000000000000000000000000000011b,	// 29 bit arithmetic shift right 
	00000000000000000000000000000001b,	// 30 bit arithmetic shift right 
	00000000000000000000000000000000b,	// 31 bit arithmetic shift right 

	0b,						// 0 bit arithmetic shift left = NOP
//    01111111111111111111111111111110b, // 1 bit arithmetic shift left 
//	01111111111111111111111111111100b,	// 2 bit arithmetic shift left 
//	01111111111111111111111111111000b,	// 3 bit arithmetic shift left 
//	01111111111111111111111111110000b,	// 4 bit arithmetic shift left 
//	01111111111111111111111111100000b,	// 5 bit arithmetic shift left 
//	01111111111111111111111111000000b,	// 6 bit arithmetic shift left 
//	01111111111111111111111110000000b,	// 7 bit arithmetic shift left 
//	01111111111111111111111100000000b,	// 8 bit arithmetic shift left 
//	01111111111111111111111000000000b,	// 9 bit arithmetic shift left 
//	01111111111111111111110000000000b,	// 10 bit arithmetic shift left 
//	01111111111111111111100000000000b,	// 11 bit arithmetic shift left 
//	01111111111111111111000000000000b,	// 12 bit arithmetic shift left 
//	01111111111111111110000000000000b,	// 13 bit arithmetic shift left 
//	01111111111111111100000000000000b,	// 14 bit arithmetic shift left 
//	01111111111111111000000000000000b,	// 15 bit arithmetic shift left 
//	01111111111111110000000000000000b,	// 16 bit arithmetic shift left 
//	01111111111111100000000000000000b,	// 17 bit arithmetic shift left 
//	01111111111111000000000000000000b,	// 18 bit arithmetic shift left 
//	01111111111110000000000000000000b,	// 19 bit arithmetic shift left 
//	01111111111100000000000000000000b,	// 20 bit arithmetic shift left 
//	01111111111000000000000000000000b,	// 21 bit arithmetic shift left 
//	01111111110000000000000000000000b,	// 22 bit arithmetic shift left 
//	01111111100000000000000000000000b,	// 23 bit arithmetic shift left 
//	01111111000000000000000000000000b,	// 24 bit arithmetic shift left 
//	01111110000000000000000000000000b,	// 25 bit arithmetic shift left 
//	01111100000000000000000000000000b,	// 26 bit arithmetic shift left 
//	01111000000000000000000000000000b,	// 27 bit arithmetic shift left 
//	01110000000000000000000000000000b,	// 28 bit arithmetic shift left 
//	01100000000000000000000000000000b,	// 29 bit arithmetic shift left 
//	01000000000000000000000000000000b,	// 30 bit arithmetic shift left 
//	00000000000000000000000000000000b	// 31 bit arithmetic shift left 

	01111111111111111111111111111110b,	// 1 bit arithmetic shift left 
	01111111111111111111111111111100b,	// 2 bit arithmetic shift left 
	01111111111111111111111111111000b,	// 3 bit arithmetic shift left 
	01111111111111111111111111110000b,	// 4 bit arithmetic shift left 
	01111111111111111111111111100000b,	// 5 bit arithmetic shift left 
	01111111111111111111111111000000b,	// 6 bit arithmetic shift left 
	01111111111111111111111110000000b,	// 7 bit arithmetic shift left 
	01111111111111111111111100000000b,	// 8 bit arithmetic shift left 
	01111111111111111111111000000000b,	// 9 bit arithmetic shift left 
	01111111111111111111110000000000b,	// 10 bit arithmetic shift left 
	01111111111111111111100000000000b,	// 11 bit arithmetic shift left 
	01111111111111111111000000000000b,	// 12 bit arithmetic shift left 
	01111111111111111110000000000000b,	// 13 bit arithmetic shift left 
	01111111111111111100000000000000b,	// 14 bit arithmetic shift left 
	01111111111111111000000000000000b,	// 15 bit arithmetic shift left 
	01111111111111110000000000000000b,	// 16 bit arithmetic shift left 
	01111111111111100000000000000000b,	// 17 bit arithmetic shift left 
	01111111111111000000000000000000b,	// 18 bit arithmetic shift left 
	01111111111110000000000000000000b,	// 19 bit arithmetic shift left 
	01111111111100000000000000000000b,	// 20 bit arithmetic shift left 
	01111111111000000000000000000000b,	// 21 bit arithmetic shift left 
	01111111110000000000000000000000b,	// 22 bit arithmetic shift left 
	01111111100000000000000000000000b,	// 23 bit arithmetic shift left 
	01111111000000000000000000000000b,	// 24 bit arithmetic shift left 
	01111110000000000000000000000000b,	// 25 bit arithmetic shift left 
	01111100000000000000000000000000b,	// 26 bit arithmetic shift left 
	01111000000000000000000000000000b,	// 27 bit arithmetic shift left 
	01110000000000000000000000000000b,	// 28 bit arithmetic shift left 
	01100000000000000000000000000000b,	// 29 bit arithmetic shift left 
	01000000000000000000000000000000b,	// 30 bit arithmetic shift left 
	00000000000000000000000000000000b,	// 31 bit arithmetic shift left 

//	11111111111111111111111111111111b = ffffffffh
	0,
	0,						// 0 bit arithmetic shift right = NOP
	11111111111111111111111111111111b,	// 1 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 2 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 3 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 4 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 5 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 6 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 7 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 8 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 9 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 10 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 11 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 12 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 13 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 14 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 15 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 16 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 17 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 18 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 19 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 20 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 21 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 22 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 23 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 24 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 25 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 26 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 27 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 28 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 29 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 30 bit arithmetic shift right 
	11111111111111111111111111111111b,	// 31 bit arithmetic shift right 

	11111111111111111111111111111111b,	// 0 bit arithmetic shift left = NOP

	11111111111111111111111111111110b,	// 1 bit arithmetic shift left  
	11111111111111111111111111111100b,	// 2 bit arithmetic shift left  
	11111111111111111111111111111000b,	// 3 bit arithmetic shift left  
	11111111111111111111111111110000b,	// 4 bit arithmetic shift left  
	11111111111111111111111111100000b,	// 5 bit arithmetic shift left  
	11111111111111111111111111000000b,	// 6 bit arithmetic shift left  
	11111111111111111111111110000000b,	// 7 bit arithmetic shift left  
	11111111111111111111111100000000b,	// 8 bit arithmetic shift left  
	11111111111111111111111000000000b,	// 9 bit arithmetic shift left  
	11111111111111111111110000000000b,	// 10 bit arithmetic shift left  
	11111111111111111111100000000000b,	// 11 bit arithmetic shift left  
	11111111111111111111000000000000b,	// 12 bit arithmetic shift left  
	11111111111111111110000000000000b,	// 13 bit arithmetic shift left  
	11111111111111111100000000000000b,	// 14 bit arithmetic shift left  
	11111111111111111000000000000000b,	// 15 bit arithmetic shift left  
	11111111111111110000000000000000b,	// 16 bit arithmetic shift left  
	11111111111111100000000000000000b,	// 17 bit arithmetic shift left  
	11111111111111000000000000000000b,	// 18 bit arithmetic shift left  
	11111111111110000000000000000000b,	// 19 bit arithmetic shift left  
	11111111111100000000000000000000b,	// 20 bit arithmetic shift left  
	11111111111000000000000000000000b,	// 21 bit arithmetic shift left  
	11111111110000000000000000000000b,	// 22 bit arithmetic shift left  
	11111111100000000000000000000000b,	// 23 bit arithmetic shift left  
	11111111000000000000000000000000b,	// 24 bit arithmetic shift left  
	11111110000000000000000000000000b,	// 25 bit arithmetic shift left  
	11111100000000000000000000000000b,	// 26 bit arithmetic shift left  
	11111000000000000000000000000000b,	// 27 bit arithmetic shift left  
	11110000000000000000000000000000b,	// 28 bit arithmetic shift left  
	11100000000000000000000000000000b,	// 29 bit arithmetic shift left  
	11000000000000000000000000000000b,	// 30 bit arithmetic shift left  
	10000000000000000000000000000000b,	// 31 bit arithmetic shift left  

	0ffffffffh,	// 1 bit logic shift left with carry
	 00000002h,	// 1 bit logic shift left with carry 
	0bfffffffh,	// 1 bit logic shift right with carry
	0c0000000h,	// 1 bit logic shift right with carry
	0c0000000h,	// witout write to a register
	0bfffffffh,	// noflags
	0bfffffffh,	// witout write to a register 

	 80f878e6h,	// gr0
	 01f0f1cdh,	// gr1
	0bfffffffh,	// gr2
	 80000000h,	// gr4
	0ffffffffh,	// gr5
	 80000001h,	// gr6
	0bfffffffh	// gr7
		 ) ;


control_pswr : word[15] = (
	1fh,		//     [0]
	1fh,		//     [1]
	1fh,		//     [2]
	1fh,		//     [3]
	1001b,		//  [4]
//	1011b,		//  [4] old odd value Ch.A.
	1000b,		//  [5]
	0001b,		//  [6]
	1001b,		//  [7]
	1001b,		//  [8]
	0000b,		//  [9]
	0000b,		// [10]
	0000b,		// [11]
    0000b,      // [12]
    0000b,      // [13]
    0000b       // [14]
//  0011b,      // [12]
//  0011b,      // [13]
//  0011b       // [14]
		) ;

save_stack : word = 0;
end data_test;

global test_4_4 : label;
   global start : label;
		weak mytest : label;
		
						 weak mytest_additional: label;	
Loop, Loop1, Normal, Error, Finish : label;
// The boot loader								 
begin ".text.init"
<test_4_4>
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

				goto mytest_additional;								      // Jump to internal memory bank 2

<mytest_additional>

pswr clear 0fh;			//  Reset flags in pswr

// 0-31 bit cyclic shift left. The result is wtitting at the next step

ar3 = array;
gr0 = [ar3];

ar3 = array[5];
gr1 = [ar3];

ar3 = test_result;
[ar3] = gr1 with gr1 = gr0 R<<0;	// 0 bit cyclic shift left 

ar3 = test_result[1];
[ar3] = gr1 with gr1 = gr0 R<<1;	// 1 bit cyclic shift left 

ar3 = test_result[2];
[ar3] = gr1 with gr1 = gr0 R<<2;	// 2 bit cyclic shift left 

ar3 = test_result[3];
[ar3] = gr1 with gr1 = gr0 R<<3;	// 3 bit cyclic shift left 

ar3 = test_result[4];
[ar3] = gr1 with gr1 = gr0 R<<4;	// 4 bit cyclic shift left 

ar3 = test_result[5];
[ar3] = gr1 with gr1 = gr0 R<<5;	// 5 bit cyclic shift left 

ar3 = test_result[6];
[ar3] = gr1 with gr1 = gr0 R<<6;	// 6 bit cyclic shift left 

ar3 = test_result[7];
[ar3] = gr1 with gr1 = gr0 R<<7;	// 7 bit cyclic shift left 

ar3 = test_result[8];
[ar3] = gr1 with gr1 = gr0 R<<8;	// 8 bit cyclic shift left 

ar3 = test_result[9];
[ar3] = gr1 with gr1 = gr0 R<<9;	// 9 bit cyclic shift left 

ar3 = test_result[10];
[ar3] = gr1 with gr1 = gr0 R<<10;	// 10 bit cyclic shift left 

ar3 = test_result[11];
[ar3] = gr1 with gr1 = gr0 R<<11;	// 11 bit cyclic shift left 

ar3 = test_result[12];
[ar3] = gr1 with gr1 = gr0 R<<12;	// 12 bit cyclic shift left 

ar3 = test_result[13];
[ar3] = gr1 with gr1 = gr0 R<<13;	// 13 bit cyclic shift left 

ar3 = test_result[14];
[ar3] = gr1 with gr1 = gr0 R<<14;	// 14 bit cyclic shift left 

ar3 = test_result[15];
[ar3] = gr1 with gr1 = gr0 R<<15;	// 15 bit cyclic shift left 

ar3 = test_result[16];
[ar3] = gr1 with gr1 = gr0 R<<16;	// 16 bit cyclic shift left 

ar3 = test_result[17];
[ar3] = gr1 with gr1 = gr0 R<<17;	// 17 bit cyclic shift left 

ar3 = test_result[18];
[ar3] = gr1 with gr1 = gr0 R<<18;	// 18 bit cyclic shift left 

ar3 = test_result[19];
[ar3] = gr1 with gr1 = gr0 R<<19;	// 19 bit cyclic shift left 

ar3 = test_result[20];
[ar3] = gr1 with gr1 = gr0 R<<20;	// 20 bit cyclic shift left 

ar3 = test_result[21];
[ar3] = gr1 with gr1 = gr0 R<<21;	// 21 bit cyclic shift left 

ar3 = test_result[22];
[ar3] = gr1 with gr1 = gr0 R<<22;	// 22 bit cyclic shift left 

ar3 = test_result[23];
[ar3] = gr1 with gr1 = gr0 R<<23;	// 23 bit cyclic shift left 

ar3 = test_result[24];
[ar3] = gr1 with gr1 = gr0 R<<24;	// 24 bit cyclic shift left 

ar3 = test_result[25];
[ar3] = gr1 with gr1 = gr0 R<<25;	// 25 bit cyclic shift left 

ar3 = test_result[26];
[ar3] = gr1 with gr1 = gr0 R<<26;	// 26 bit cyclic shift left 

ar3 = test_result[27];
[ar3] = gr1 with gr1 = gr0 R<<27;	// 27 bit cyclic shift left 

ar3 = test_result[28];
[ar3] = gr1 with gr1 = gr0 R<<28;	// 28 bit cyclic shift left 

ar3 = test_result[29];
[ar3] = gr1 with gr1 = gr0 R<<29;	// 29 bit cyclic shift left 

ar3 = test_result[30];
[ar3] = gr1 with gr1 = gr0 R<<30;	// 30 bit cyclic shift left 

ar3 = test_result[31];
[ar3] = gr1 with gr1 = gr0 R<<31;	// 31 bit cyclic shift left 

ar3 = test_result[32];
[ar3] = gr1 with gr1 = gr0 R<<1;	// gr2 - ?
//[ar3] = gr2 with gr1 = gr0 R<<1;	// gr2 - ?


// 0-31 bit cyclic shift rigth. The result is wtitting at the next step

ar3 = test_result[33];
[ar3] = gr1 with gr1 = gr0 R>>0;	// 0 bit cyclic shift rigth 

ar3 = test_result[34];
[ar3] = gr1 with gr1 = gr0 R>>1;	// 1 bit cyclic shift rigth 

ar3 = test_result[35];
[ar3] = gr1 with gr1 = gr0 R>>2;	// 2 bit cyclic shift rigth 

ar3 = test_result[36];
[ar3] = gr1 with gr1 = gr0 R>>3;	// 3 bit cyclic shift rigth 

ar3 = test_result[37];
[ar3] = gr1 with gr1 = gr0 R>>4;	// 4 bit cyclic shift rigth 

ar3 = test_result[38];
[ar3] = gr1 with gr1 = gr0 R>>5;	// 5 bit cyclic shift rigth 

ar3 = test_result[39];
[ar3] = gr1 with gr1 = gr0 R>>6;	// 6 bit cyclic shift rigth 

ar3 = test_result[40];
[ar3] = gr1 with gr1 = gr0 R>>7;	// 7 bit cyclic shift rigth 

ar3 = test_result[41];
[ar3] = gr1 with gr1 = gr0 R>>8;	// 8 bit cyclic shift rigth 

ar3 = test_result[42];
[ar3] = gr1 with gr1 = gr0 R>>9;	// 9 bit cyclic shift rigth 

ar3 = test_result[43];
[ar3] = gr1 with gr1 = gr0 R>>10;	// 10 bit cyclic shift rigth 

ar3 = test_result[44];
[ar3] = gr1 with gr1 = gr0 R>>11;	// 11 bit cyclic shift rigth 

ar3 = test_result[45];
[ar3] = gr1 with gr1 = gr0 R>>12;	// 12 bit cyclic shift rigth 

ar3 = test_result[46];
[ar3] = gr1 with gr1 = gr0 R>>13;	// 13 bit cyclic shift rigth 

ar3 = test_result[47];
[ar3] = gr1 with gr1 = gr0 R>>14;	// 14 bit cyclic shift rigth 

ar3 = test_result[48];
[ar3] = gr1 with gr1 = gr0 R>>15;	// 15 bit cyclic shift rigth 

ar3 = test_result[49];
[ar3] = gr1 with gr1 = gr0 R>>16;	// 16 bit cyclic shift rigth 

ar3 = test_result[50];
[ar3] = gr1 with gr1 = gr0 R>>17;	// 17 bit cyclic shift rigth 

ar3 = test_result[51];
[ar3] = gr1 with gr1 = gr0 R>>18;	// 18 bit cyclic shift rigth 

ar3 = test_result[52];
[ar3] = gr1 with gr1 = gr0 R>>19;	// 19 bit cyclic shift rigth 

ar3 = test_result[53];
[ar3] = gr1 with gr1 = gr0 R>>20;	// 20 bit cyclic shift rigth 

ar3 = test_result[54];
[ar3] = gr1 with gr1 = gr0 R>>21;	// 21 bit cyclic shift rigth 

ar3 = test_result[55];
[ar3] = gr1 with gr1 = gr0 R>>22;	// 22 bit cyclic shift rigth 

ar3 = test_result[56];
[ar3] = gr1 with gr1 = gr0 R>>23;	// 23 bit cyclic shift rigth 

ar3 = test_result[57];
[ar3] = gr1 with gr1 = gr0 R>>24;	// 24 bit cyclic shift rigth 

ar3 = test_result[58];
[ar3] = gr1 with gr1 = gr0 R>>25;	// 25 bit cyclic shift rigth 

ar3 = test_result[59];
[ar3] = gr1 with gr1 = gr0 R>>26;	// 26 bit cyclic shift rigth 

ar3 = test_result[60];
[ar3] = gr1 with gr1 = gr0 R>>27;	// 27 bit cyclic shift rigth 

ar3 = test_result[61];
[ar3] = gr1 with gr1 = gr0 R>>28;	// 28 bit cyclic shift rigth 

ar3 = test_result[62];
[ar3] = gr1 with gr1 = gr0 R>>29;	// 29 bit cyclic shift rigth 

ar3 = test_result[63];
[ar3] = gr1 with gr1 = gr0 R>>30;	// 30 bit cyclic shift rigth 

ar3 = test_result[64];
[ar3] = gr1 with gr1 = gr0 R>>31;	// 31 bit cyclic shift rigth 

ar3 = test_result[65];
[ar3] = gr1;			// gr2 - ?
//[ar3] = gr2;			// gr2 - ?

ar3 = pswr_result;
//[ar3] = pswr;



// 0-31 bit cyclic shifts right-left 

ar3 = array[1];
gr1 = [ar3];

ar3 = array[5];
gr0 = [ar3];

ar3 = test_result[66];
[ar3] = gr0 with gr0 = gr1 R>>0;	// 0 bit cyclic shift rigth

ar3 = test_result[67];
[ar3] = gr0 with gr0 = gr1 R>>1;	// 1 bit cyclic shift rigth

ar3 = test_result[68];
[ar3] = gr0 with gr0 = gr1 R>>2;	// 2 bit cyclic shift rigth

ar3 = test_result[69];
[ar3] = gr0 with gr0 = gr1 R>>3;	// 3 bit cyclic shift rigth 

ar3 = test_result[70];
[ar3] = gr0 with gr0 = gr1 R>>4;	// 4 bit cyclic shift rigth 

ar3 = test_result[71];
[ar3] = gr0 with gr0 = gr1 R>>5;	// 5 bit cyclic shift rigth 

ar3 = test_result[72];
[ar3] = gr0 with gr0 = gr1 R>>6;	// 6 bit cyclic shift rigth 

ar3 = test_result[73];
[ar3] = gr0 with gr0 = gr1 R>>7;	// 7 bit cyclic shift rigth 

ar3 = test_result[74];
[ar3] = gr0 with gr0 = gr1 R>>8;	// 8 bit cyclic shift rigth 

ar3 = test_result[75];
[ar3] = gr0 with gr0 = gr1 R>>9;	// 9 bit cyclic shift rigth 

ar3 = test_result[76];
[ar3] = gr0 with gr0 = gr1 R>>10;	// 10 bit cyclic shift rigth 

ar3 = test_result[77];
[ar3] = gr0 with gr0 = gr1 R>>11;	// 11 bit cyclic shift rigth 

ar3 = test_result[78];
[ar3] = gr0 with gr0 = gr1 R>>12;	// 12 bit cyclic shift rigth 

ar3 = test_result[79];
[ar3] = gr0 with gr0 = gr1 R>>13;	// 13 bit cyclic shift rigth 

ar3 = test_result[80];
[ar3] = gr0 with gr0 = gr1 R>>14;	// 14 bit cyclic shift rigth 

ar3 = test_result[81];
[ar3] = gr0 with gr0 = gr1 R>>15;	// 15 bit cyclic shift rigth 

ar3 = test_result[82];
[ar3] = gr0 with gr0 = gr1 R>>16;	// 16 bit cyclic shift rigth 

ar3 = test_result[83];
[ar3] = gr0 with gr0 = gr1 R>>17;	// 17 bit cyclic shift rigth 

ar3 = test_result[84];
[ar3] = gr0 with gr0 = gr1 R>>18;	// 18 bit cyclic shift rigth 

ar3 = test_result[85];
[ar3] = gr0 with gr0 = gr1 R>>19;	// 19 bit cyclic shift rigth 

ar3 = test_result[86];
[ar3] = gr0 with gr0 = gr1 R>>20;	// 20 bit cyclic shift rigth 

ar3 = test_result[87];
[ar3] = gr0 with gr0 = gr1 R>>21;	// 21 bit cyclic shift rigth 

ar3 = test_result[88];
[ar3] = gr0 with gr0 = gr1 R>>22;	// 22 bit cyclic shift rigth 

ar3 = test_result[89];
[ar3] = gr0 with gr0 = gr1 R>>23;	// 23 bit cyclic shift rigth 

ar3 = test_result[90];
[ar3] = gr0 with gr0 = gr1 R>>24;	// 24 bit cyclic shift rigth 

ar3 = test_result[91];
[ar3] = gr0 with gr0 = gr1 R>>25;	// 25 bit cyclic shift rigth 

ar3 = test_result[92];
[ar3] = gr0 with gr0 = gr1 R>>26;	// 26 bit cyclic shift rigth 

ar3 = test_result[93];
[ar3] = gr0 with gr0 = gr1 R>>27;	// 27 bit cyclic shift rigth 

ar3 = test_result[94];
[ar3] = gr0 with gr0 = gr1 R>>28;	// 28 bit cyclic shift rigth 

ar3 = test_result[95];
[ar3] = gr0 with gr0 = gr1 R>>29;	// 29 bit cyclic shift rigth 

ar3 = test_result[96];
[ar3] = gr0 with gr0 = gr1 R>>30;	// 30 bit cyclic shift rigth 

ar3 = test_result[97];
[ar3] = gr0 with gr0 = gr1 R>>31;	// 31 bit cyclic shift rigth 

ar3 = test_result[98];
[ar3] = gr0 with gr0 = gr1 R>>1;	//


ar3 = test_result[99];
[ar3] = gr0 with gr0 = gr1 R<<0;	// 0 bit cyclic shift left 

ar3 = test_result[100];
[ar3] = gr0 with gr0 = gr1 R<<1;	// 1 bit cyclic shift left 

ar3 = test_result[101];
[ar3] = gr0 with gr0 = gr1 R<<2;	// 2 bit cyclic shift left 

ar3 = test_result[102];
[ar3] = gr0 with gr0 = gr1 R<<3;	// 3 bit cyclic shift left 

ar3 = test_result[103];
[ar3] = gr0 with gr0 = gr1 R<<4;	// 4 bit cyclic shift left 

ar3 = test_result[104];
[ar3] = gr0 with gr0 = gr1 R<<5;	// 5 bit cyclic shift left 

ar3 = test_result[105];
[ar3] = gr0 with gr0 = gr1 R<<6;	// 6 bit cyclic shift left 

ar3 = test_result[106];
[ar3] = gr0 with gr0 = gr1 R<<7;	// 7 bit cyclic shift left 

ar3 = test_result[107];
[ar3] = gr0 with gr0 = gr1 R<<8;	// 8 bit cyclic shift left 

ar3 = test_result[108];
[ar3] = gr0 with gr0 = gr1 R<<9;	// 9 bit cyclic shift left 

ar3 = test_result[109];
[ar3] = gr0 with gr0 = gr1 R<<10;	// 10 bit cyclic shift left 

ar3 = test_result[110];
[ar3] = gr0 with gr0 = gr1 R<<11;	// 11 bit cyclic shift left 

ar3 = test_result[111];
[ar3] = gr0 with gr0 = gr1 R<<12;	// 12 bit cyclic shift left 

ar3 = test_result[112];
[ar3] = gr0 with gr0 = gr1 R<<13;	// 13 bit cyclic shift left 

ar3 = test_result[113];
[ar3] = gr0 with gr0 = gr1 R<<14;	// 14 bit cyclic shift left 

ar3 = test_result[114];
[ar3] = gr0 with gr0 = gr1 R<<15;	// 15 bit cyclic shift left 

ar3 = test_result[115];
[ar3] = gr0 with gr0 = gr1 R<<16;	// 16 bit cyclic shift left 

ar3 = test_result[116];
[ar3] = gr0 with gr0 = gr1 R<<17;	// 17 bit cyclic shift left 

ar3 = test_result[117];
[ar3] = gr0 with gr0 = gr1 R<<18;	// 18 bit cyclic shift left 

ar3 = test_result[118];
[ar3] = gr0 with gr0 = gr1 R<<19;	// 19 bit cyclic shift left 

ar3 = test_result[119];
[ar3] = gr0 with gr0 = gr1 R<<20;	// 20 bit cyclic shift left 

ar3 = test_result[120];
[ar3] = gr0 with gr0 = gr1 R<<21;	// 21 bit cyclic shift left 

ar3 = test_result[121];
[ar3] = gr0 with gr0 = gr1 R<<22;	// 22 bit cyclic shift left 

ar3 = test_result[122];
[ar3] = gr0 with gr0 = gr1 R<<23;	// 23 bit cyclic shift left 

ar3 = test_result[123];
[ar3] = gr0 with gr0 = gr1 R<<24;	// 24 bit cyclic shift left 

ar3 = test_result[124];
[ar3] = gr0 with gr0 = gr1 R<<25;	// 25 bit cyclic shift left 

ar3 = test_result[125];
[ar3] = gr0 with gr0 = gr1 R<<26;	// 26 bit cyclic shift left 

ar3 = test_result[126];
[ar3] = gr0 with gr0 = gr1 R<<27;	// 27 bit cyclic shift left 

ar3 = test_result[127];
[ar3] = gr0 with gr0 = gr1 R<<28;	// 28 bit cyclic shift left 

ar3 = test_result[128];
[ar3] = gr0 with gr0 = gr1 R<<29;	// 29 bit cyclic shift left 

ar3 = test_result[129];
[ar3] = gr0 with gr0 = gr1 R<<30;	// 30 bit cyclic shift left 

ar3 = test_result[130];
[ar3] = gr0 with gr0 = gr1 R<<31;	// 31 bit cyclic shift left 

ar3 = test_result[131];
[ar3] = gr0;

ar3 = pswr_result[1];
//[ar3] = pswr;



// 0-31 bit logical shifts left-right 

ar3 = array[2];
gr2 = [ar3];

ar3 = array[3];
gr4 = [ar3];

ar3 = array[5];
gr3 = [ar3];
gr5 = [ar3];

ar3 = test_result[132];
[ar3] = gr3 with gr3 = gr2 << 0;	// 0 bit logical shift left 

ar3 = test_result[133];
[ar3] = gr3 with gr3 = gr2 << 1;	// 1 bit logical shift left 

ar3 = test_result[134];
[ar3] = gr3 with gr3 = gr2 << 2;	// 2 bit logical shift left 

ar3 = test_result[135];
[ar3] = gr3 with gr3 = gr2 << 3;	// 3 bit logical shift left 

ar3 = test_result[136];
[ar3] = gr3 with gr3 = gr2 << 4;	// 4 bit logical shift left 

ar3 = test_result[137];
[ar3] = gr3 with gr3 = gr2 << 5;	// 5 bit logical shift left 

ar3 = test_result[138];
[ar3] = gr3 with gr3 = gr2 << 6;	// 6 bit logical shift left 

ar3 = test_result[139];
[ar3] = gr3 with gr3 = gr2 << 7;	// 7 bit logical shift left 

ar3 = test_result[140];
[ar3] = gr3 with gr3 = gr2 << 8;	// 8 bit logical shift left 

ar3 = test_result[141];
[ar3] = gr3 with gr3 = gr2 << 9;	// 9 bit logical shift left 

ar3 = test_result[142];
[ar3] = gr3 with gr3 = gr2 << 10;	// 10 bit logical shift left 

ar3 = test_result[143];
[ar3] = gr3 with gr3 = gr2 << 11;	// 11 bit logical shift left 

ar3 = test_result[144];
[ar3] = gr3 with gr3 = gr2 << 12;	// 12 bit logical shift left 

ar3 = test_result[145];
[ar3] = gr3 with gr3 = gr2 << 13;	// 13 bit logical shift left 

ar3 = test_result[146];
[ar3] = gr3 with gr3 = gr2 << 14;	// 14 bit logical shift left 

ar3 = test_result[147];
[ar3] = gr3 with gr3 = gr2 << 15;	// 15 bit logical shift left 

ar3 = test_result[148];
[ar3] = gr3 with gr3 = gr2 << 16;	// 16 bit logical shift left 

ar3 = test_result[149];
[ar3] = gr3 with gr3 = gr2 << 17;	// 17 bit logical shift left 

ar3 = test_result[150];
[ar3] = gr3 with gr3 = gr2 << 18;	// 18 bit logical shift left 

ar3 = test_result[151];
[ar3] = gr3 with gr3 = gr2 << 19;	// 19 bit logical shift left 

ar3 = test_result[152];
[ar3] = gr3 with gr3 = gr2 << 20;	// 20 bit logical shift left 

ar3 = test_result[153];
[ar3] = gr3 with gr3 = gr2 << 21;	// 21 bit logical shift left 

ar3 = test_result[154];
[ar3] = gr3 with gr3 = gr2 << 22;	// 22 bit logical shift left 

ar3 = test_result[155];
[ar3] = gr3 with gr3 = gr2 << 23;	// 23 bit logical shift left 

ar3 = test_result[156];
[ar3] = gr3 with gr3 = gr2 << 24;	// 24 bit logical shift left 

ar3 = test_result[157];
[ar3] = gr3 with gr3 = gr2 << 25;	// 25 bit logical shift left 

ar3 = test_result[158];
[ar3] = gr3 with gr3 = gr2 << 26;	// 26 bit logical shift left 

ar3 = test_result[159];
[ar3] = gr3 with gr3 = gr2 << 27;	// 27 bit logical shift left 

ar3 = test_result[160];
[ar3] = gr3 with gr3 = gr2 << 28;	// 28 bit logical shift left 

ar3 = test_result[161];
[ar3] = gr3 with gr3 = gr2 << 29;	// 29 bit logical shift left 

ar3 = test_result[162];
[ar3] = gr3 with gr3 = gr2 << 30;	// 30 bit logical shift left 

ar3 = test_result[163];
[ar3] = gr3 with gr3 = gr2 << 31;	// 31 bit logical shift left 


ar3 = test_result[164];
[ar3] = gr3 with gr3 = gr2 >> 0;	// 0 bit logical shift right 

ar3 = test_result[165];
[ar3] = gr3 with gr3 = gr2 >> 1;	// 1 bit logical shift right 

ar3 = test_result[166];
[ar3] = gr3 with gr3 = gr2 >> 2;	// 2 bit logical shift right 

ar3 = test_result[167];
[ar3] = gr3 with gr3 = gr2 >> 3;	// 3 bit logical shift right 

ar3 = test_result[168];
[ar3] = gr3 with gr3 = gr2 >> 4;	// 4 bit logical shift right 

ar3 = test_result[169];
[ar3] = gr3 with gr3 = gr2 >> 5;	// 5 bit logical shift right 

ar3 = test_result[170];
[ar3] = gr3 with gr3 = gr2 >> 6;	// 6 bit logical shift right 

ar3 = test_result[171];
[ar3] = gr3 with gr3 = gr2 >> 7;	// 7 bit logical shift right 

ar3 = test_result[172];
[ar3] = gr3 with gr3 = gr2 >> 8;	// 8 bit logical shift right 

ar3 = test_result[173];
[ar3] = gr3 with gr3 = gr2 >> 9;	// 9 bit logical shift right 

ar3 = test_result[174];
[ar3] = gr3 with gr3 = gr2 >> 10;	// 10 bit logical shift right 

ar3 = test_result[175];
[ar3] = gr3 with gr3 = gr2 >> 11;	// 11 bit logical shift right 

ar3 = test_result[176];
[ar3] = gr3 with gr3 = gr2 >> 12;	// 12 bit logical shift right 

ar3 = test_result[177];
[ar3] = gr3 with gr3 = gr2 >> 13;	// 13 bit logical shift right 

ar3 = test_result[178];
[ar3] = gr3 with gr3 = gr2 >> 14;	// 14 bit logical shift right 

ar3 = test_result[179];
[ar3] = gr3 with gr3 = gr2 >> 15;	// 15 bit logical shift right 

ar3 = test_result[180];
[ar3] = gr3 with gr3 = gr2 >> 16;	// 16 bit logical shift right 

ar3 = test_result[181];
[ar3] = gr3 with gr3 = gr2 >> 17;	// 17 bit logical shift right 

ar3 = test_result[182];
[ar3] = gr3 with gr3 = gr2 >> 18;	// 18 bit logical shift right 

ar3 = test_result[183];
[ar3] = gr3 with gr3 = gr2 >> 19;	// 19 bit logical shift right 

ar3 = test_result[184];
[ar3] = gr3 with gr3 = gr2 >> 20;	// 20 bit logical shift right 

ar3 = test_result[185];
[ar3] = gr3 with gr3 = gr2 >> 21;	// 21 bit logical shift right 

ar3 = test_result[186];
[ar3] = gr3 with gr3 = gr2 >> 22;	// 22 bit logical shift right 

ar3 = test_result[187];
[ar3] = gr3 with gr3 = gr2 >> 23;	// 23 bit logical shift right 

ar3 = test_result[188];
[ar3] = gr3 with gr3 = gr2 >> 24;	// 24 bit logical shift right 

ar3 = test_result[189];
[ar3] = gr3 with gr3 = gr2 >> 25;	// 25 bit logical shift right 

ar3 = test_result[190];
[ar3] = gr3 with gr3 = gr2 >> 26;	// 26 bit logical shift right 

ar3 = test_result[191];
[ar3] = gr3 with gr3 = gr2 >> 27;	// 27 bit logical shift right 

ar3 = test_result[192];
[ar3] = gr3 with gr3 = gr2 >> 28;	// 28 bit logical shift right 

ar3 = test_result[193];
[ar3] = gr3 with gr3 = gr2 >> 29;	// 29 bit logical shift right 

ar3 = test_result[194];
[ar3] = gr3 with gr3 = gr2 >> 30;	// 30 bit logical shift right 

ar3 = test_result[195];
[ar3] = gr3 with gr3 = gr2 >> 31;	// 31 bit logical shift right 

ar3 = test_result[196];
[ar3] = gr3;

ar3 = pswr_result[2];
//[ar3] = pswr;



// 0-31 bit arithmetic shifts right-left 

ar3 = test_result[197];
[ar3] = gr5 with gr5 = gr4 A>> 0;	// 0 bit arithmetic shift right 

ar3 = test_result[198];
[ar3] = gr5 with gr5 = gr4 A>> 1;	// 1 bit arithmetic shift right 

ar3 = test_result[199];
[ar3] = gr5 with gr5 = gr4 A>> 2;	// 2 bit arithmetic shift right 

ar3 = test_result[200];
[ar3] = gr5 with gr5 = gr4 A>> 3;	// 3 bit arithmetic shift right 

ar3 = test_result[201];
[ar3] = gr5 with gr5 = gr4 A>> 4;	// 4 bit arithmetic shift right 

ar3 = test_result[202];
[ar3] = gr5 with gr5 = gr4 A>> 5;	// 5 bit arithmetic shift right 

ar3 = test_result[203];
[ar3] = gr5 with gr5 = gr4 A>> 6;	// 6 bit arithmetic shift right 

ar3 = test_result[204];
[ar3] = gr5 with gr5 = gr4 A>> 7;	// 7 bit arithmetic shift right 

ar3 = test_result[205];
[ar3] = gr5 with gr5 = gr4 A>> 8;	// 8 bit arithmetic shift right 

ar3 = test_result[206];
[ar3] = gr5 with gr5 = gr4 A>> 9;	// 9 bit arithmetic shift right 

ar3 = test_result[207];
[ar3] = gr5 with gr5 = gr4 A>> 10;	// 10 bit arithmetic shift right 

ar3 = test_result[208];
[ar3] = gr5 with gr5 = gr4 A>> 11;	// 11 bit arithmetic shift right 

ar3 = test_result[209];
[ar3] = gr5 with gr5 = gr4 A>> 12;	// 12 bit arithmetic shift right 

ar3 = test_result[210];
[ar3] = gr5 with gr5 = gr4 A>> 13;	// 13 bit arithmetic shift right 

ar3 = test_result[211];
[ar3] = gr5 with gr5 = gr4 A>> 14;	// 14 bit arithmetic shift right 

ar3 = test_result[212];
[ar3] = gr5 with gr5 = gr4 A>> 15;	// 15 bit arithmetic shift right 

ar3 = test_result[213];
[ar3] = gr5 with gr5 = gr4 A>> 16;	// 16 bit arithmetic shift right 

ar3 = test_result[214];
[ar3] = gr5 with gr5 = gr4 A>> 17;	// 17 bit arithmetic shift right 

ar3 = test_result[215];
[ar3] = gr5 with gr5 = gr4 A>> 18;	// 18 bit arithmetic shift right 

ar3 = test_result[216];
[ar3] = gr5 with gr5 = gr4 A>> 19;	// 19 bit arithmetic shift right 

ar3 = test_result[217];
[ar3] = gr5 with gr5 = gr4 A>> 20;	// 20 bit arithmetic shift right 

ar3 = test_result[218];
[ar3] = gr5 with gr5 = gr4 A>> 21;	// 21 bit arithmetic shift right 

ar3 = test_result[219];
[ar3] = gr5 with gr5 = gr4 A>> 22;	// 22 bit arithmetic shift right 

ar3 = test_result[220];
[ar3] = gr5 with gr5 = gr4 A>> 23;	// 23 bit arithmetic shift right 

ar3 = test_result[221];
[ar3] = gr5 with gr5 = gr4 A>> 24;	// 24 bit arithmetic shift right 

ar3 = test_result[222];
[ar3] = gr5 with gr5 = gr4 A>> 25;	// 25 bit arithmetic shift right 

ar3 = test_result[223];
[ar3] = gr5 with gr5 = gr4 A>> 26;	// 26 bit arithmetic shift right 

ar3 = test_result[224];
[ar3] = gr5 with gr5 = gr4 A>> 27;	// 27 bit arithmetic shift right 

ar3 = test_result[225];
[ar3] = gr5 with gr5 = gr4 A>> 28;	// 28 bit arithmetic shift right 

ar3 = test_result[226];
[ar3] = gr5 with gr5 = gr4 A>> 29;	// 29 bit arithmetic shift right 

ar3 = test_result[227];
[ar3] = gr5 with gr5 = gr4 A>> 30;	// 30 bit arithmetic shift right 

ar3 = test_result[228];
[ar3] = gr5 with gr5 = gr4 A>> 31;	// 31 bit arithmetic shift right 


ar3 = test_result[229];
[ar3] = gr5 with gr5 = gr4 A<< 0;	// 0 bit arithmetic shift left

ar3 = test_result[230];
[ar3] = gr5 with gr5 = gr4 A<< 1;	// 1 bit arithmetic shift left

ar3 = test_result[231];
[ar3] = gr5 with gr5 = gr4 A<< 2;	// 2 bit arithmetic shift left 

ar3 = test_result[232];
[ar3] = gr5 with gr5 = gr4 A<< 3;	// 3 bit arithmetic shift left 

ar3 = test_result[233];
[ar3] = gr5 with gr5 = gr4 A<< 4;	// 4 bit arithmetic shift left 

ar3 = test_result[234];
[ar3] = gr5 with gr5 = gr4 A<< 5;	// 5 bit arithmetic shift left 

ar3 = test_result[235];
[ar3] = gr5 with gr5 = gr4 A<< 6;	// 6 bit arithmetic shift left 

ar3 = test_result[236];
[ar3] = gr5 with gr5 = gr4 A<< 7;	// 7 bit arithmetic shift left 

ar3 = test_result[237];
[ar3] = gr5 with gr5 = gr4 A<< 8;	// 8 bit arithmetic shift left 

ar3 = test_result[238];
[ar3] = gr5 with gr5 = gr4 A<< 9;	// 9 bit arithmetic shift left 

ar3 = test_result[239];
[ar3] = gr5 with gr5 = gr4 A<< 10;	// 10 bit arithmetic shift left 

ar3 = test_result[240];
[ar3] = gr5 with gr5 = gr4 A<< 11;	// 11 bit arithmetic shift left 

ar3 = test_result[241];
[ar3] = gr5 with gr5 = gr4 A<< 12;	// 12 bit arithmetic shift left 

ar3 = test_result[242];
[ar3] = gr5 with gr5 = gr4 A<< 13;	// 13 bit arithmetic shift left 

ar3 = test_result[243];
[ar3] = gr5 with gr5 = gr4 A<< 14;	// 14 bit arithmetic shift left 

ar3 = test_result[244];
[ar3] = gr5 with gr5 = gr4 A<< 15;	// 15 bit arithmetic shift left 

ar3 = test_result[245];
[ar3] = gr5 with gr5 = gr4 A<< 16;	// 16 bit arithmetic shift left 

ar3 = test_result[246];
[ar3] = gr5 with gr5 = gr4 A<< 17;	// 17 bit arithmetic shift left 

ar3 = test_result[247];
[ar3] = gr5 with gr5 = gr4 A<< 18;	// 18 bit arithmetic shift left 

ar3 = test_result[248];
[ar3] = gr5 with gr5 = gr4 A<< 19;	// 19 bit arithmetic shift left 

ar3 = test_result[249];
[ar3] = gr5 with gr5 = gr4 A<< 20;	// 20 bit arithmetic shift left 

ar3 = test_result[250];
[ar3] = gr5 with gr5 = gr4 A<< 21;	// 21 bit arithmetic shift left 

ar3 = test_result[251];
[ar3] = gr5 with gr5 = gr4 A<< 22;	// 22 bit arithmetic shift left 

ar3 = test_result[252];
[ar3] = gr5 with gr5 = gr4 A<< 23;	// 23 bit arithmetic shift left 

ar3 = test_result[253];
[ar3] = gr5 with gr5 = gr4 A<< 24;	// 24 bit arithmetic shift left 

ar3 = test_result[254];
[ar3] = gr5 with gr5 = gr4 A<< 25;	// 25 bit arithmetic shift left 

ar3 = test_result[255];
[ar3] = gr5 with gr5 = gr4 A<< 26;	// 26 bit arithmetic shift left 

ar3 = test_result[256];
[ar3] = gr5 with gr5 = gr4 A<< 27;	// 27 bit arithmetic shift left 

ar3 = test_result[257];
[ar3] = gr5 with gr5 = gr4 A<< 28;	// 28 bit arithmetic shift left 

ar3 = test_result[258];
[ar3] = gr5 with gr5 = gr4 A<< 29;	// 29 bit arithmetic shift left 

ar3 = test_result[259];
[ar3] = gr5 with gr5 = gr4 A<< 30;	// 30 bit arithmetic shift left 

ar3 = test_result[260];
[ar3] = gr5 with gr5 = gr4 A<< 31;	// 31 bit arithmetic shift left 

ar3 = test_result[261];
[ar3] = gr5;

ar3 = pswr_result[3];
//[ar3] = pswr;


ar3 = array[2];
gr5 = [ar3];

ar3 = array[3];
gr3 = [ar3];

ar3 = array[5];
gr4 = [ar3];

ar3 = test_result[262];
[ar3] = gr4 with gr4 = gr5 A>> 0;	// 0 bit arithmetic shift right

ar3 = test_result[263];
[ar3] = gr4 with gr4 = gr5 A>> 1;	// 1 bit arithmetic shift right 

ar3 = test_result[264];
[ar3] = gr4 with gr4 = gr5 A>> 2;	// 2 bit arithmetic shift right 

ar3 = test_result[265];
[ar3] = gr4 with gr4 = gr5 A>> 3;	// 3 bit arithmetic shift right 

ar3 = test_result[266];
[ar3] = gr4 with gr4 = gr5 A>> 4;	// 4 bit arithmetic shift right 

ar3 = test_result[267];
[ar3] = gr4 with gr4 = gr5 A>> 5;	// 5 bit arithmetic shift right 

ar3 = test_result[268];
[ar3] = gr4 with gr4 = gr5 A>> 6;	// 6 bit arithmetic shift right 

ar3 = test_result[269];
[ar3] = gr4 with gr4 = gr5 A>> 7;	// 7 bit arithmetic shift right 

ar3 = test_result[270];
[ar3] = gr4 with gr4 = gr5 A>> 8;	// 8 bit arithmetic shift right 

ar3 = test_result[271];
[ar3] = gr4 with gr4 = gr5 A>> 9;	// 9 bit arithmetic shift right 

ar3 = test_result[272];
[ar3] = gr4 with gr4 = gr5 A>> 10;	// 10 bit arithmetic shift right 

ar3 = test_result[273];
[ar3] = gr4 with gr4 = gr5 A>> 11;	// 11 bit arithmetic shift right 

ar3 = test_result[274];
[ar3] = gr4 with gr4 = gr5 A>> 12;	// 12 bit arithmetic shift right 

ar3 = test_result[275];
[ar3] = gr4 with gr4 = gr5 A>> 13;	// 13 bit arithmetic shift right 

ar3 = test_result[276];
[ar3] = gr4 with gr4 = gr5 A>> 14;	// 14 bit arithmetic shift right 

ar3 = test_result[277];
[ar3] = gr4 with gr4 = gr5 A>> 15;	// 15 bit arithmetic shift right 

ar3 = test_result[278];
[ar3] = gr4 with gr4 = gr5 A>> 16;	// 16 bit arithmetic shift right 

ar3 = test_result[279];
[ar3] = gr4 with gr4 = gr5 A>> 17;	// 17 bit arithmetic shift right 

ar3 = test_result[280];
[ar3] = gr4 with gr4 = gr5 A>> 18;	// 18 bit arithmetic shift right 

ar3 = test_result[281];
[ar3] = gr4 with gr4 = gr5 A>> 19;	// 19 bit arithmetic shift right 

ar3 = test_result[282];
[ar3] = gr4 with gr4 = gr5 A>> 20;	// 20 bit arithmetic shift right 

ar3 = test_result[283];
[ar3] = gr4 with gr4 = gr5 A>> 21;	// 21 bit arithmetic shift right 

ar3 = test_result[284];
[ar3] = gr4 with gr4 = gr5 A>> 22;	// 22 bit arithmetic shift right 

ar3 = test_result[285];
[ar3] = gr4 with gr4 = gr5 A>> 23;	// 23 bit arithmetic shift right 

ar3 = test_result[286];
[ar3] = gr4 with gr4 = gr5 A>> 24;	// 24 bit arithmetic shift right 

ar3 = test_result[287];
[ar3] = gr4 with gr4 = gr5 A>> 25;	// 25 bit arithmetic shift right 

ar3 = test_result[288];
[ar3] = gr4 with gr4 = gr5 A>> 26;	// 26 bit arithmetic shift right 

ar3 = test_result[289];
[ar3] = gr4 with gr4 = gr5 A>> 27;	// 27 bit arithmetic shift right 

ar3 = test_result[290];
[ar3] = gr4 with gr4 = gr5 A>> 28;	// 28 bit arithmetic shift right 

ar3 = test_result[291];
[ar3] = gr4 with gr4 = gr5 A>> 29;	// 29 bit arithmetic shift right 

ar3 = test_result[292];
[ar3] = gr4 with gr4 = gr5 A>> 30;	// 30 bit arithmetic shift right 

ar3 = test_result[293];
[ar3] = gr4 with gr4 = gr5 A>> 31;	// 31 bit arithmetic shift right 


ar3 = test_result[294];
[ar3] = gr4 with gr4 = gr5 A<< 0;	// 0 bit arithmetic shift left

ar3 = test_result[295];
[ar3] = gr4 with gr4 = gr5 A<< 1;	// 1 bit arithmetic shift left 

ar3 = test_result[296];
[ar3] = gr4 with gr4 = gr5 A<< 2;	// 2 bit arithmetic shift left 

ar3 = test_result[297];
[ar3] = gr4 with gr4 = gr5 A<< 3;	// 3 bit arithmetic shift left 

ar3 = test_result[298];
[ar3] = gr4 with gr4 = gr5 A<< 4;	// 4 bit arithmetic shift left 

ar3 = test_result[299];
[ar3] = gr4 with gr4 = gr5 A<< 5;	// 5 bit arithmetic shift left 

ar3 = test_result[300];
[ar3] = gr4 with gr4 = gr5 A<< 6;	// 6 bit arithmetic shift left 

ar3 = test_result[301];
[ar3] = gr4 with gr4 = gr5 A<< 7;	// 7 bit arithmetic shift left 

ar3 = test_result[302];
[ar3] = gr4 with gr4 = gr5 A<< 8;	// 8 bit arithmetic shift left 

ar3 = test_result[303];
[ar3] = gr4 with gr4 = gr5 A<< 9;	// 9 bit arithmetic shift left 

ar3 = test_result[304];
[ar3] = gr4 with gr4 = gr5 A<< 10;	// 10 bit arithmetic shift left 

ar3 = test_result[305];
[ar3] = gr4 with gr4 = gr5 A<< 11;	// 11 bit arithmetic shift left 

ar3 = test_result[306];
[ar3] = gr4 with gr4 = gr5 A<< 12;	// 12 bit arithmetic shift left 

ar3 = test_result[307];
[ar3] = gr4 with gr4 = gr5 A<< 13;	// 13 bit arithmetic shift left 

ar3 = test_result[308];
[ar3] = gr4 with gr4 = gr5 A<< 14;	// 14 bit arithmetic shift left 

ar3 = test_result[309];
[ar3] = gr4 with gr4 = gr5 A<< 15;	// 15 bit arithmetic shift left 

ar3 = test_result[310];
[ar3] = gr4 with gr4 = gr5 A<< 16;	// 16 bit arithmetic shift left 

ar3 = test_result[311];
[ar3] = gr4 with gr4 = gr5 A<< 17;	// 17 bit arithmetic shift left 

ar3 = test_result[312];
[ar3] = gr4 with gr4 = gr5 A<< 18;	// 18 bit arithmetic shift left 

ar3 = test_result[313];
[ar3] = gr4 with gr4 = gr5 A<< 19;	// 19 bit arithmetic shift left 

ar3 = test_result[314];
[ar3] = gr4 with gr4 = gr5 A<< 20;	// 20 bit arithmetic shift left 

ar3 = test_result[315];
[ar3] = gr4 with gr4 = gr5 A<< 21;	// 21 bit arithmetic shift left 

ar3 = test_result[316];
[ar3] = gr4 with gr4 = gr5 A<< 22;	// 22 bit arithmetic shift left 

ar3 = test_result[317];
[ar3] = gr4 with gr4 = gr5 A<< 23;	// 23 bit arithmetic shift left 

ar3 = test_result[318];
[ar3] = gr4 with gr4 = gr5 A<< 24;	// 24 bit arithmetic shift left 

ar3 = test_result[319];
[ar3] = gr4 with gr4 = gr5 A<< 25;	// 25 bit arithmetic shift left 

ar3 = test_result[320];
[ar3] = gr4 with gr4 = gr5 A<< 26;	// 26 bit arithmetic shift left 

ar3 = test_result[321];
[ar3] = gr4 with gr4 = gr5 A<< 27;	// 27 bit arithmetic shift left 

ar3 = test_result[322];
[ar3] = gr4 with gr4 = gr5 A<< 28;	// 28 bit arithmetic shift left 

ar3 = test_result[323];
[ar3] = gr4 with gr4 = gr5 A<< 29;	// 29 bit arithmetic shift left 

ar3 = test_result[324];
[ar3] = gr4 with gr4 = gr5 A<< 30;	// 30 bit arithmetic shift left 

ar3 = test_result[325];
[ar3] = gr4 with gr4 = gr5 A<< 31;	// 31 bit arithmetic shift left 

ar3 = test_result[326];
[ar3] = gr4;

ar3 = pswr_result[4];
[ar3] = pswr;


//  Logical shift with carry 

ar3 = array[4];
gr6 = [ar3] with gr2 = gr3 C<<1;

ar3 = test_result[327];
[ar3] = gr2;

ar3 = pswr_result[5];
[ar3] = pswr with gr7 = gr6 C<<1;

ar3 = test_result[328];
[ar3] = gr7;

ar3 = pswr_result[6];
[ar3] = pswr with gr2 = gr3 C>>1;

ar3 = test_result[329];
[ar3] = gr2;

ar3 = pswr_result[7];
[ar3] = pswr with gr7 = gr6 C>>1;

ar3 = test_result[330];
[ar3] = gr7;

ar3 = pswr_result[8];
[ar3] = pswr;




// Instructions without result into register writing and without flags changing


with -gr7;				// Flags change,
					// Result is not wrote

ar3 = test_result[331];
[ar3] = gr7;

ar3 = pswr_result[9];
[ar3] = pswr with gr7 = -gr7 noflags;	// Flags are not changed

ar3 = pswr_result[10];
[ar3] = pswr;

ar3 = pswr_result[11];
[ar3] = pswr with gr7;        // Result is not saved 

ar3 = test_result[332];
[ar3] = gr2;

ar3 = pswr_result[12];
[ar3] = pswr with gr7 = not gr7 noflags;	// Flags are not changed 

ar3 = test_result[333];
[ar3] = gr7;

ar3 = pswr_result[13];
[ar3] = pswr;

ar3 = test_result[334];
[ar3] = gr0 with gr1 = gr0 R<< 0;

ar3 = test_result[335];
[ar3] = gr1 with gr2 = gr1 R>> 32;

ar3 = test_result[336];
[ar3] = gr2 with gr4 = gr3 << 0;

ar3 = test_result[337];
[ar3] = gr4 with gr5 = gr4 >> 32;

ar3 = test_result[338];
[ar3] = gr5 with gr6 = gr5 A<< 0;

ar3 = test_result[339];
[ar3] = gr6 with gr7 = gr6 A>> 32;

ar3 = test_result[340];
[ar3] = gr7;

ar3 = pswr_result[14];
[ar3] = pswr;

ar7 = [save_stack];

// The program of given and control dimension compare

	ar0 = test_result;
	ar1 = contr_result;
  	gr0 = 341;

<Loop>
	gr1 = [ ar0++ ];
	gr2 = [ ar1++ ];
	with gr2 - gr1;
	if <>0 goto Error;
	with gr0--;
	if <>0 goto Loop;

		ar0 = pswr_result;
		ar1 = control_pswr;
	  gr0 = 15;
		gr3 = 1fh;

	<Loop1>
		gr1 = [ ar0++ ];
		with gr1 = gr1 and gr3;
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




