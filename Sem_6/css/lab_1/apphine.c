//affin and single column combination

//#define A 17
//#define B 19
#define Alphabets 26

#include<stdio.h>
#include<string.h>


int modInverse(int a, int m) 
{
    int i;
    for (i = 0; i < m; i++) 
    {
        if ((a * i) % m == 1)  
            return i;  


    }
    return -1; 
}


void encrypt_affine(char word[], char cipher[], int A, int B)
{
    //int l = strlen(word);
    //char cipher[l];
    int i;

    for(i=0; word[i]!='\0'; i++)
    {
        if(word[i]!= ' ')
        {
            cipher[i] = ((A * (word[i] - 'A') + B) % 26) + 'A';
        }
        else{
            cipher[i] = ' ';
        }
    }
    cipher[i] = '\0';
    printf("The ciphered text is %s\n", cipher);

}

void decrypt_affine(char cipher[], char decrypted[], int A, int B)
{
    int A_inv = modInverse(A,Alphabets);
    int i;
    //if A = -1

    for(i=0; cipher[i]!='\0'; i++)
    {
        if (cipher[i]!= ' ')
        {
            decrypted[i] = ((A_inv*(cipher[i] - 'A' - B + Alphabets) % Alphabets) + 'A');
        }
        else{
            decrypted[i] = ' ';
        }
    }
    decrypted[strlen(cipher)] = '\0';
    printf("The decrypted text is %s", decrypted);

}

int main()
{   
    char word[100];
    int A;
    int B;

    printf("Enter your string: ");
    fgets(word, sizeof(word), stdin);

    word[strcspn(word, "\n")] = 0;
    

    printf("Enter the value of A and B: ");
    scanf("%d %d", &A, &B);
    //char word[] = "MARK LOPES";
    char cipher[strlen(word)+1];
    char decrypted_word[strlen(word)+1];

    encrypt_affine(word, cipher, A, B);
    decrypt_affine(cipher, decrypted_word, A, B);

    return 1;
}
