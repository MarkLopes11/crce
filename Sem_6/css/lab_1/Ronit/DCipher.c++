#include <iostream> // For input/output
#include <fstream>  // For file handling

using namespace std;

// Function to encrypt the plaintext using a key
string encrypt(const string &plaintext, const string &key) {
    int keyLength = key.length();
    int plaintextLength = plaintext.length();
    int rows = (plaintextLength + keyLength - 1) / keyLength; // Calculate rows needed

    // Create a 2D array (matrix) to hold the plaintext
    char matrix[rows][keyLength];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < keyLength; j++) {
            matrix[i][j] = ' '; // Initialize with spaces
        }
    }

    // Fill the matrix row-wise with plaintext
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < keyLength; j++) {
            if (index < plaintextLength) {
                matrix[i][j] = plaintext[index++];
            }
        }
    }

    // Determine column order based on the key
    int column_order[keyLength];
    for (int i = 0; i < keyLength; i++) {
        column_order[i] = i;
    }
    // Sort columns based on the key (simple bubble sort)
    for (int i = 0; i < keyLength; i++) {
        for (int j = i + 1; j < keyLength; j++) {
            if (key[column_order[i]] > key[column_order[j]]) {
                swap(column_order[i], column_order[j]);
            }
        }
    }

    // Build encrypted text by reading columns in the new order
    string encrypted_text;
    for (int i = 0; i < keyLength; i++) {
        for (int j = 0; j < rows; j++) {
            if (matrix[j][column_order[i]] != ' ') {
                encrypted_text += matrix[j][column_order[i]];
            }
        }
    }

    return encrypted_text;
}

int main() {
    string plaintext;

    // Read plaintext from file
    ifstream inFile("ciphertext.txt");
    if (inFile.is_open()) {
        getline(inFile, plaintext);
        inFile.close();
    } else {
        cerr << "Error opening file for reading!" << endl;
        return 1;
    }

    string key1 = "BAD";
    string key2 = "DOG";

    // Encrypt with first key
    string encrypted_text = encrypt(plaintext, key1);
    cout << "Encrypted with key1: " << encrypted_text << endl;

    // Encrypt with second key
    encrypted_text = encrypt(encrypted_text, key2);
    cout << "Encrypted with key2: " << encrypted_text << endl;

    // Write encrypted text back to file
    ofstream outFile("ciphertext.txt");
    if (outFile.is_open()) {
        outFile << encrypted_text;
        outFile.close();
    } else {
        cerr << "Error opening file for writing!" << endl;
        return 1;
    }

    return 0;
}