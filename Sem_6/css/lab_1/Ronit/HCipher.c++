#include <iostream>
#include <fstream>
using namespace std;

// Following function generates the key matrix for the key string
void getKeyMatrix(string key, int keyMatrix[][3]) {
    int k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            keyMatrix[i][j] = (key[k]) % 65;
            k++;
        }
    }
}

// Following function encrypts the message
void encrypt(int cipherMatrix[][1], int keyMatrix[][3], int messageVector[][1]) {
    int x, i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 1; j++) {
            cipherMatrix[i][j] = 0;
            for (x = 0; x < 3; x++) {
                cipherMatrix[i][j] += keyMatrix[i][x] * messageVector[x][j];
            }
            cipherMatrix[i][j] = cipherMatrix[i][j] % 26;
        }
    }
}

// Function to implement Hill Cipher
string HillCipher(string message, string key) {
    int keyMatrix[3][3];
    getKeyMatrix(key, keyMatrix);

    int messageVector[3][1];
    for (int i = 0; i < 3; i++)
        messageVector[i][0] = (message[i]) % 65;

    int cipherMatrix[3][1];
    encrypt(cipherMatrix, keyMatrix, messageVector);

    string CipherText;
    for (int i = 0; i < 3; i++)
        CipherText += cipherMatrix[i][0] + 65;

    return CipherText;
}

// Driver function
int main() {

    string message;

    ifstream inFile("ciphertext.txt");
    if (inFile.is_open()) {
        getline(inFile, message);
        inFile.close();
    } else {
        cerr << "Error opening file for reading!" << endl;
        return 1;
    }

    
    string key = "GYBNQKURP";
    string result;

    for (size_t i = 0; i < message.length(); i += 3) {
        string chunk = message.substr(i, 3);
        while (chunk.length() < 3) {
            chunk += 'X';
        }
        result += HillCipher(chunk, key);
    }

    cout << "Ciphertext: " << result << endl;

    // Write encrypted text back to file
    ofstream outFile("ciphertext.txt");
    if (outFile.is_open()) {
        outFile << result;
        outFile.close();
    } else {
        cerr << "Error opening file for writing!" << endl;
        return 1;
    }

    return 0;
}