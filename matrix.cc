#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
using namespace std;

void addRoundKeys(vector< vector < vector<unsigned char> > > &matrix_list, vector < vector<unsigned char> > key_schedule, int round_num) {
    for (int i = 0; i < matrix_list.size(); ++i) {
        int key_column = 4*round_num;
        for(int col = 0; col < 4; ++col) {
            for(int row = 0; row < 4; ++row) {
                matrix_list[i][row][col] ^= key_schedule[row][key_column];
            }
            ++key_column;
        }
    }
}

vector <unsigned char> SubWord(vector <unsigned char> word) {
    unsigned char lookup_table[16][16] =
    {
        0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
    };

    for(int i = 0; i < 4; ++i) {
        unsigned char value = word[i];
        int lookup_row = (value & 0xF0) >> 4;
        int lookup_col = value & 0x0F;
        word[i] = lookup_table[lookup_row][lookup_col];
    }
    return word;
}

vector <unsigned char> RotWord(vector <unsigned char> word) {
    unsigned char temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
    return word;
}

vector <unsigned char> XorWords(vector <unsigned char> v1, vector <unsigned char> v2) {
    vector<unsigned char> return_vector(4,0);
    for(int i = 0; i < 4; ++i) {
        return_vector[i] = v1[i] ^ v2[i];
    }
    return return_vector;
}

vector < vector<unsigned char> > KeyExpansion(vector<unsigned char> key, int key_length) {
    int n_k = key_length / 32;
    int n_r = 0;
    if(key_length == 128) {
        n_r = 10;
    } else if(key_length == 256) {
        n_r = 14;
    }

    vector < vector<unsigned char> > key_schedule(4, vector<unsigned char> (4*(n_r+1), 0));
    
    unsigned char rcon[] = {0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36};

    // Add the original key into the key_schedule
    for(int i = 0; i < n_k; ++i) {
        key_schedule[0][i] = key[4*i];
        key_schedule[1][i] = key[4*i+1];
        key_schedule[2][i] = key[4*i+2];
        key_schedule[3][i] = key[4*i+3];
    }

    // Create the expanded part of the key_schedule
    vector<unsigned char> temp(4,0);
    for(int i = n_k; i < 4*(n_r+1); ++i) {
        temp[0] = key_schedule[0][i-1];
        temp[1] = key_schedule[1][i-1];
        temp[2] = key_schedule[2][i-1];
        temp[3] = key_schedule[3][i-1];

        if(i % n_k == 0) {
            vector<unsigned char> rcon_vec(4, 0x00);
            rcon_vec[0] = rcon[i/n_k];
            temp = XorWords(SubWord(RotWord(temp)), rcon_vec);
        } else if (n_k == 8 && ((i % n_k) == 4)) {
            temp = SubWord(temp);
        }

        vector<unsigned char> past_column(4, 0x00);
        past_column[0] = key_schedule[0][i - n_k];
        past_column[1] = key_schedule[1][i - n_k];
        past_column[2] = key_schedule[2][i - n_k];
        past_column[3] = key_schedule[3][i - n_k];

        temp = XorWords(past_column, temp);
        key_schedule[0][i] = temp[0];
        key_schedule[1][i] = temp[1];
        key_schedule[2][i] = temp[2];
        key_schedule[3][i] = temp[3];
    }

    return key_schedule;
}
    
int main() 
{
    ifstream key_file;
    int key_length = 128;
    vector<unsigned char> key;

    key_file.open("key", ios::binary);
    if(key_file.is_open()) 
    {
        char byte;
        int key_bytes = 0;;
        
        while(key_file.get(byte))
        {
            key.push_back((unsigned char)byte);
            ++key_bytes;
        }

        if((8 * key_bytes) != key_length) {
            cout << "Your key is not " << key_length << "bits long " << endl;
            cout << "Your key is " << (8 * key_bytes) << "bits long " << endl;
            return 0;
        }
    } else {
        cout << "Error with opening the key file" << endl;
        return 0;
    }

    int n_k = key_length / 32;
    int n_r = 0;
    if(key_length == 128) {
        n_r = 10;
    } else if(key_length == 256) {
        n_r = 14;
    }
    vector < vector<unsigned char> > key_schedule = KeyExpansion(key,key_length);

    
    // Opens a file as binary
    ifstream file;
    vector< vector < vector<unsigned char> > > matrix_list;
    int num_bytes = 0;
    file.open("Pingu3D.png", ios::binary);
    if(file.is_open()) 
    {
    	char byte;
        int matrix_pos = 0;
        int matrix_index = -1;
        // List of 4x4 Matrices
        
    	while(file.get(byte)) {
            // Keeps track of the position in the matrix the next byte should be
            // If it is 0, it means we should start a new matrix
            if(matrix_pos == 0) {
                vector< vector<unsigned char> > matrix(4, vector<unsigned char> (4, 0));
                matrix_list.push_back(matrix);
                // Update the current matrix
                ++matrix_index;
            }

            // Converts the matrix_pos range from [0-15] to row/column indexing
            // Makes this column-order instead of row-order as specified in the document
            int row = matrix_pos % 4;
            int col = matrix_pos / 4;

            matrix_list[matrix_index][row][col] = (unsigned char)byte;
            
            // Updates the next position in the matrix & the number of bytes collectd
            matrix_pos = (matrix_pos+1) % 16;
            ++num_bytes;            
    	}
    	cout << "num_bytes: " << num_bytes << endl;
        int rem = num_bytes % 16;
        if (rem == 0) {
            // If we recieved a factor of 16 bytes, we add a new padding matrix
            cout << "ANOTHER" << endl;
            vector< vector<unsigned char> > matrix(4, vector<unsigned char> (4, 16));
            matrix_list.push_back(matrix);
        } else {
            // Otherwise, we pad the rest of the current matrix
            cout << "ADD " << 16-rem << " PAD CELLS"<< endl;
            for(int i = matrix_pos; i < 16; ++i) {
                int row = i % 4;
                int col = i / 4;
                matrix_list[matrix_index][row][col] = (16-rem);
            }
        }
        file.close();
        /*
        // Prints Matrix
        for(int i = 0; i < matrix_list.size(); ++i) 
        {
            cout << "= MATRIX " << i+1 << " =" << endl;
            for(int j = 0; j < 4; ++j) 
            {
                for(int k = 0; k < 4; ++k) {
                    byte = matrix_list[i][j][k];
                    bitset<8> bits(byte);
                    if(byte == '\n') {
                        cout << "\\n";
                    } else {
                        cout << byte;
                    }
                    cout << " (" << bits << ")";
                    if(k != 3) {
                        cout << ",";
                    }
                }
                cout << endl;
            }
            cout << "===" << endl;
        }
        //*/
        // addRoundKeys(matrix_list, key_schedule, 0);

        // Create Output
        ofstream output("cipherOutput");
        int num_printed_bytes = 0; // compare against num_bytes;
        for(int i = 0; i < matrix_list.size(); ++i) {
            for(int c = 0; c < 4; ++c) {
                for(int r = 0; r < 4; ++r) {
                    if(num_printed_bytes < num_bytes){
                        char byte = matrix_list[i][r][c];
                        output.put(byte);
                    } else {
                        break;
                    }
                    ++num_printed_bytes;
                }
            }
        }
        
        output.close();
    } else {
        cout << "Error with opening the input file" << endl;
    }
    
    return 0;
}

void inverseShiftRows (vector<vector<vector<unsigned char>>> &input_array) {
   // make a copy of the input vector to reference when shifting values
   vector<vector<vector<unsigned char>>> old_array = input_array;

   for (int i = 0; i < input_array.size(); i++) {
      for(int row = 1; row < input_array[i].size(); row++) {
         for(int col = 0; col < input_array[i][row].size(); col++) {
            // replace value with the value found by shifting row bytes
            int shift = (col-row)%4;
            if(shift < 0) {
                shift += 4;
            }
            input_array[i][row][col] = old_array[i][row][shift];
         }
      }
   }
}

void inverseSubBytes(vector<vector<vector<unsigned char>>> &input_array) {
    unsigned char lookup_table[16][16] =
    { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
      0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
      0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
      0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
      0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
      0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
      0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
      0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
      0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
      0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
      0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
      0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
      0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
      0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
      0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
      0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
    };

    for (int i = 0; i < input_array.size(); i++) {
        for(int row = 0; row < input_array[i].size(); row++) {
            for(int col = 0; col < input_array[i][row].size(); col++) {
                // get value to use as index into lookup table
                unsigned char value = input_array[i][row][col];
                // get first nibble for row index
                int lookup_row = (value & 0xF0) >> 4;
                // get second nibble for column index
                int lookup_col = value & 0x0F;
                // replace value with the value in lookup table
                input_array[i][row][col] = lookup_table[lookup_row][lookup_col];
            }
        }
    }
}