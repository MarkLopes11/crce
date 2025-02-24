#include <stdio.h>
#include <string.h>

#define MAX_LEN 20

char S1[MAX_LEN] = "MARK";
char S2[MAX_LEN] = "ARCEUS";
int LCS_table[MAX_LEN][MAX_LEN];

void Lcs()
{
    int m = strlen(S1);
    int n = strlen(S2);

    // Fill LCS_table with 0's
    for (int i = 0; i <= m; i++)
        LCS_table[i][0] = 0;
    for (int i = 0; i <= n; i++)
        LCS_table[0][i] = 0;

    // Fill LCS_table based on LCS algorithm
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (S1[i - 1] == S2[j - 1])
                LCS_table[i][j] = LCS_table[i - 1][j - 1] + 1;
            else if (LCS_table[i - 1][j] >= LCS_table[i][j - 1])
                LCS_table[i][j] = LCS_table[i - 1][j];
            else
                LCS_table[i][j] = LCS_table[i][j - 1];
        }
    }

    // Finding the LCS string
    int length = LCS_table[m][n];
    char lcsAlgo[length + 1];
    int i = m, j = n;
    lcsAlgo[length] = '\0';

    while (i > 0 && j > 0)
    {
        if (S1[i - 1] == S2[j - 1])
        {
            lcsAlgo[length - 1] = S1[i - 1];
            i--;
            j--;
            length--;
        }
        else if (LCS_table[i - 1][j] >= LCS_table[i][j - 1])
        {
            i--;
        }
        else
        {
            j--;
        }
    }

    // Printing the results
    printf("S1 : %s\nS2 : %s\n", S1, S2);
    printf("LCS: %s\n", lcsAlgo);
}

int main()
{
    Lcs();
    return 0;
}
