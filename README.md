# AES
## CS361 AES Project

Name1:
 eid1:

Name2: Jong-Min Richard Kim
 eid2: jrk2826


Usage
./program --keysize [128/256] --keyfile [key] --inputfile [input] --outputfile [output] --mode [encrypt]
Example:
./program --keysize 256 --keyfile key --inputfile input --outputfile output --mode encrypt

This program should be compiled using C++11.

g++ -std=c++17 aes.cc -o program

= Algorithm Description

Input for the key simply reads the key file byte by byte into an array.

The input file is read into a list of matrices of bytes (the state) in 
column-major order. This is done by tracking the position in a matrix the next 
byte should be in, starting at 0. Whenever that position is 0, we add a new 
matrix to that list and begin adding to it. The matrix position is in a range 
from 0 to 15, representing the 16 elements in a 4x4 matrix. To insert into the 
matrix in column order, the matrix position is divided by 4, saving the 
remainder. The quotient represents the column the next byte should be in, 
while the remainder represents the row the byte should be in (row order can be 
done by making the quotient the row and the remainder the column). As bytes are 
read in, the matrix position is incremented by 1. Modulo division is also 
performed on it with 16 as the divisor such that it remains within the range of 
0 to 15.

After all of the bytes from the file are read for an encryption operation, 
modulo division is performed on the number of bytes read using 16 as the 
divisor. If the number of bytes is not a factor 16, we take the remainder and 
subtract it from 16. That difference is the used to fill in the rest of the 
matrix. If the number of bytes is a factor of 16, we add a new 4x4 matrix 
filled with 16s as padding.

Then, Key Expansion is performed on the key. The key schedule that results is 
represented as a matrix whose size is determined by the key size. Specifically, 
it consists of 4 rows and 4*(n_r+1) columns, where n_r represents the number of 
rounds that will be performed in Rijndael's Algorithm (10 for 128 bit keys, 14 
for 256 bit keys). First, the key is transferred to the beginning of the key 
schedule as is in column-major order. This is done by taking every 4 bytes of 
the key and placing them into the first unfilled column in the key schedule. 
Then, for the rest of the key schedule, the following algorithm derived from 
the NIST document is followed:

	We iterate through the rest of the unfilled columns of the key schedule. 
	First, a temporary copy (temp) is made of the previous column in the key 
	schedule. The copy is an array of 4 bytes, starting with the row 0 byte of 
	the column and ending with the row 3 byte of the column. 

	The value of temp is conditionally manipulated:

		If the column's index (where numbering starts at 0)	is a factor of n_k, 
		the number of 32 bit words in the key (4 for 128 bit keys, 8 for 256 
		bit keys), reassign temp with the result of a series of functions
			temp = XorWords(SubWord(RotWord(temp)), rcon_vec);
		1. RotWord returns a copy of the temp array with its elements shifted 
		   to the left by 1:
			element 0 is assigned element 1's original value
			element 1 is assigned element 2's original value
			element 2 is assigned element 3's original value
			element 3 is assigned element 0's original value
		2. SubWord returns a copy of the temp array that substitutes each 
		   element in the temp array using a lookup table representing the 
		   S-Box of the Rijndael Algorithm. The first nibble of each element 
		   is used as the row index into the lookup table, and the second 
		   nibble is used as the column index into the lookup table.
		3. The rcon_vec is contains [rcon[index], 0, 0, 0], where 
		   index = column index/n_k. rcon is a lookup table (array)
		   representing x^(index-1), powers of x (denoted {02}) in the field 
		   GF(2^8). Since index should begin at 1 for the algorithm, a padding 
		   0x00 is placed as the zeroth element of the rcon array
		4. XorWords returns an array obtained by xor'ing corresponding 
		   elements in the two input arrays

		If we are dealing with a 256 bit key (n_k = 8) and 
		(column index % n_k = 4) is a factor of n_k, reassign temp with the 
		result of SubWord(temp)

		Otherwise, temp remains unchanged

	We then make a copy of the column n_k indices before the current column 
	index (column index - n_k) in a fashion similar to temp (a 4 byte array). 
	We xor this copy and temp, and transfer the values of the resulting array 
    into the current column index in column-major order (the first element is 
    placed into row 0 of the column, the last element is placed into row 3 of 
    the	column) 

The following functions are used in Rijndael's Algorithm

AddRoundKey takes the state, the key schedule, and the current round number. 
A round key represents 4 columns in the key schedule: columns 4*round_num to
4*round_num+3. This round key is added (xor'd) to each matrix in the state
by adding (xor'ing) each column of the 4x4 matrix with the corresponding 
column in the round key.

SubBytes is written using a lookup table (S-Box). For each element in the 
state, get the value, then use the first nibble of the value for the row index 
into the lookup table and the second nibble for the column index. Using these
indices, we index into the lookup table and replace the state element with the
value found in the lookup table.

InverseSubBytes functions similarly, but its lookup table represents the
inverse S-Box.

ShiftRows is written by first making a copy of the state then referencing that
copy when getting the value after shifting. For each element in the state, grab
the element found by adding the row number to the column index modulo 4 (which
will shift the row by the row number) in the copied state. We then replace the
state element with the value we grabbed in the copied state.

InverseShiftRows functions similarly, but it subtracts the row number instead 
of adding it. If the resulting index is less than 0, 4 is added to it in order
to loop around to the end of the row.

MixColumns is written using a lookup multiplication table of 2 and 3 and a copy
of the state. For each column in the state, perform the following:
- for row 0 of the column, set the value to row 0's value in the mul2 table xor
row 1's value in the mul3 table xor row 2's value xor row 3's value
- for row 1 of the column, set the value to row 0's value xor row 1's value in
the mul2 table xor row 2's value in the mul3 table xor row 3's value
- for row 2 of the column, set the value to row 0's value xor row 1's value xor
row 2's value in the mul2 table xor row 3's value in the mul3 table
- for row 3 of the column, set the value to row 0's value in the mul3 table xor
row 1's value xor row 2's value xor row 3's value in the mul2 table

InverseMixColumns is written using a lookup multiplication table of 9, 11, 13,
and 14 and a copy of the state. For each column in the state, perform the
following:
- for row 0 of the column, set the value to row 0's value in the mul14 table xor
row 1's value in the mul11 table xor row 2's value in the mul13 table xor row
3's value in the mul9 table
- for row 1 of the column, set the value to row 0's value in the mul9 table xor
row 1's value in the mul14 table xor row 2's value in the mul11 table xor row
3's value in the mul13 table
- for row 2 of the column, set the value to row 0's value in the mul13 table xor
row 1's value in the mul9 table xor row 2's value in the mul14 table xor row 3's
value in the mul11 table
- for row 3 of the column, set the value to row 0's value in the mul11 table xor
row 1's value in the mul13 table xor row 2's value in the mul9 table xor row 3's
value in the mul14 table

To encrypt using the initial state, we perform the Rijndael's algorithm:
	First, we call AddRoundKey on the initial state using the key schedule for
	round 0.

	Then, for round 1 to round n_r - 1, we do the following
		1. Call SubBytes on the state
		2. Call ShiftRows on the state
		3. Call MixColumns on the state
		4. Call AddRoundKey on the state using the key schedule and the current 
		   round number

	Finally, repeat the above steps one more time for round n_r, excluding the
	MixColumns Step.

To decrypt using the initial state, we do the following:
	First, we call AddRoundKey on the initial state using the key schedule for 
	round n_r.

	Then, for round n_r-1 to round 1, we do the following
		
		1. Call InverseShiftRows on the state
		2. Call InverseSubBytes on the state
		3. Call AddRoundKey on the state using the key schedule and the current 
		   round number
		4. Call InverseMixColumns on the state

	Finally, repeat the above steps one more time for round 0, excluding the
	InverseMixColumns Step.


The final state is the outputted to a file in column-major order, byte by byte. 
However, if the file is being decrypted, the number of padding bytes added in 
the encryption step is obtained from the last element in the last matrix in the 
final state. The number of bytes to write for decryption is the number of bytes 
in the input (encrypted) file minus the number of padding bytes. Thus, the 
padding bytes are not written.