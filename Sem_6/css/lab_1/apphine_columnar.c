// Single Columnar Transposition Cipher with Affine Encryption
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Affine Encryption Function
void encrypt_affine(char word[], char cipher[], int A, int B) {
    int i;
    for(i = 0; word[i] != '\0'; i++) {
        if(word[i] != ' ') {
            cipher[i] = ((A * (word[i] - 'A') + B) % 26) + 'A';
        } else {
            cipher[i] = ' ';
        }
    }
    cipher[i] = '\0';
}

// Columnar Transposition Encryption Function
char* Encryption(int no_rows, int len_key, int len_msg, char msg[], int col_val[]) {
    int x = 0;
    char enc_mat[no_rows + 1][len_key];
    char* cipher = (char*)malloc((len_msg + (no_rows + 1) * len_key - len_msg + 1) * sizeof(char));
    int cipher_index = 0;

    // Creating the matrix
    for (int i = 0; i < no_rows + 1; i++) {
        for (int j = 0; j < len_key; j++) {
            if (x >= len_msg) {
                enc_mat[i][j] = '_';
            } else {
                enc_mat[i][j] = msg[x];
            }
            x++;
        }
    }

    int t = 1;
    // Finding the cipher text according to the value of col_val array
    while (t <= len_key) {
        for (int i = 0; i < len_key; i++) {
            int k = col_val[i];
            if (k == t) {
                for (int j = 0; j < no_rows + 1; j++) {
                    cipher[cipher_index++] = enc_mat[j][i];
                }
                t++;
            }
        }
    }
    cipher[cipher_index] = '\0'; // Null-terminate the string
    return cipher;
}

int main() {
    // Original Message
    // char msg[] = "Mark Lopes";
    char msg[100];
    printf("Enter your string: ");
    fgets(msg, sizeof(msg), stdin);

    char affine_cipher[100];

    int A,B;
    printf("Enter the value of A and B: ");
    scanf("%d %d", &A, &B);
    getchar(); // Clear the newline character from the buffer


    // Apply Affine Encryption first
    encrypt_affine(msg, affine_cipher, A, B);

    // Key for Columnar Transposition
    //char key[] = "HACK";
    char key[100];
    printf("Enter the key for columnar transposition: ");
    fgets(key, sizeof(key), stdin);

    int len_key = strlen(key);
    int len_msg = strlen(affine_cipher);

    int val = 1, count = 0, ind;
    int col_val[len_key];

    // Initializing col_val array with 0
    memset(col_val, 0, sizeof(col_val));

    // Numbering the key alphabets according to their ASCII values
    while (count < len_key) {
        int min = 999;
        for (int i = 0; i < len_key; i++) {
            if ((min > (int)key[i]) && (col_val[i] == 0)) {
                min = (int)key[i];
                ind = i;
            }
        }
        col_val[ind] = val;
        count++;
        val++;
    }

    int no_rows = len_msg / len_key;
    if (len_msg % len_key != 0) no_rows++; // Adjusting for leftover characters

    // Apply Columnar Transposition Encryption on Affine Cipher Text
    char* cipher_text = Encryption(no_rows, len_key, len_msg, affine_cipher, col_val);

    // Display Results
    printf("Affine Ciphered Text: %s\n", affine_cipher);
    printf("Final Encrypted Message: %s\n", cipher_text);

    free(cipher_text); // Free allocated memory

    return 1;
}
