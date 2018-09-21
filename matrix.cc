#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
using namespace std;

int main() 
{
    // Opens a file a binary
    ifstream file;
    file.open("test.txt", ios::binary);
    if(file.is_open()) 
    {
    	char byte;
        int num_bytes = 0;
        int matrix_pos = 0;
        int matrix_index = -1;
        // List of 4x4 Matrices
        vector< vector < vector<char> > > matrix_list;
    	while(file.get(byte)) {
            /*
            bitset<8> bits(byte);
            cout << byte << ", " << bits << endl;
            //*/

            // Keeps track of the position in the matrix the next byte should be
            // If it is 0, it means we should start a new matrix
            if(matrix_pos == 0) {
                vector< vector<char> > matrix(4, vector<char> (4, 0));
                matrix_list.push_back(matrix);
                // Update the current matrix
                ++matrix_index;
            }

            // Converts the matrix_pos range from [0-15] to row/column indexing
            // Makes this column-order instead of row-order as specified in the document
            int row = matrix_pos % 4;
            int col = matrix_pos / 4;

            matrix_list[matrix_index][row][col] = byte;
            
            // Updates the next position in the matrix & the number of bytes collectd
            matrix_pos = (matrix_pos+1) % 16;
            ++num_bytes;            
    	}
    	cout << "num_bytes: " << num_bytes << endl;
        int rem = num_bytes % 16;
        if (rem == 0) {
            // If we recieved a factor of 16 bytes, we add a new padding matrix
            cout << "ANOTHER" << endl;
            vector< vector<char> > matrix(4, vector<char> (4, 16));
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

    	file.close();
    }
    
    return 0;
}