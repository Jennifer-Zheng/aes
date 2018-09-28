# AES
## CS361 AES Project

This program should be compiled using C++11.

SubBytes is written using a lookup table. For each element in the state,
get the value, then use the first nibble of the value for the row index into
the lookup table and the second nibble for the column index. Using these
indices, we index into the lookup table and replace the state element with the
value found in the lookup table.

ShiftRows is written by first making a copy of the state then referencing that
copy when getting the value after shifting. For each element in the state, grab
the element found by adding the row number to the column index modulo 4 (which
will shift the row by the row number) in the copied state. We then replace the
state element with the value we grabbed in the copied state.

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