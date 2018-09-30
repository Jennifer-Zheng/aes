#ifndef AES_H_
#define AES_H_

void AddRoundKey (std::vector<std::vector<std::vector<unsigned char>>> &matrix_list,
                  std::vector<std::vector<unsigned char>> key_schedule, int round_num);
void SubBytes (std::vector<std::vector<std::vector<unsigned char>>> &input_array);
void ShiftRows (std::vector<std::vector<std::vector<unsigned char>>> &input_array);
void MixColumns (std::vector<std::vector<std::vector<unsigned char>>> &input_array);
void InverseMixColumns (std::vector<std::vector<std::vector<unsigned char>>> &input_array);
void InverseSubBytes (std::vector<std::vector<std::vector<unsigned char>>> &input_array);
void InverseShiftRows (std::vector<std::vector<std::vector<unsigned char>>> &input_array);

std::vector<unsigned char> SubWord (std::vector<unsigned char> word);
std::vector<unsigned char> RotWord (std::vector<unsigned char> word);
std::vector<unsigned char> XorWords (std::vector<unsigned char> v1, std::vector<unsigned char> v2);
std::vector<std::vector<unsigned char>> KeyExpansion (std::vector<unsigned char> key, int key_length);

#endif
