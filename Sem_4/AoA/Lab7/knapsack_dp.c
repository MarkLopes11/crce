#include <stdio.h>

int max(int a, int b) //calculate max of two integers
{
    return (a > b) ? a : b;
}

int knapsack(int W, int wt[], int val[], int n)
{
    int mat[n + 1][W + 1];

    // Initialize matrix with 0
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= W; j++)
        {
            if (i == 0 || j == 0)
                mat[i][j] = 0;
            else
            {
                int maxValWithoutCurr = mat[i - 1][j];
                int maxValWithCurr = 0;

                if (j >= wt[i - 1])
                {
                    maxValWithCurr = val[i - 1];
                    int remainingCapacity = j - wt[i - 1];
                    maxValWithCurr += mat[i - 1][remainingCapacity];
                }

                mat[i][j] = max(maxValWithoutCurr, maxValWithCurr);
            }
        }
    }

    return mat[n][W]; 
}

int main()
{
    int W = 10;                   // Max weight
    int n = 4;                    // Number of items
    int val[] = {10, 40, 30, 50}; // Values of items
    int wt[] = {5, 4, 6, 3};      // Weights of items

    int maxValue = knapsack(W, wt, val, n);

    printf("Maximum value that can be obtained: %d\n", maxValue);

    return 0;
}
