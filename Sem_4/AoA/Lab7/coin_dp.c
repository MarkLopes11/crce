#include <stdio.h>

// Define the available coin denominations and the target sum
int coins[] = {1, 2, 3};
int targetSum = 4;
int numCoins = 3;

// Function to initialize the dynamic programming table
void initializeTable(int table[][5]) {
    // Initialize the first row to 0 (base case)
    for (int i = 0; i <= targetSum; i++) {
        table[0][i] = 0;
    }

    // Initialize the first column to 1 (base case)
    for (int i = 0; i <= numCoins; i++) {
        table[i][0] = 1;
    }
}

// Function to calculate the number of ways to reach the target sum
int countWays(int table[][5]) {
    for (int coinIndex = 1; coinIndex <= numCoins; coinIndex++) {
        for (int currentSum = 1; currentSum <= targetSum; currentSum++) {
            if (coins[coinIndex - 1] > currentSum) {
                // If the coin value is greater than the current sum, exclude it
                table[coinIndex][currentSum] = table[coinIndex - 1][currentSum];
            } else {
                // otherwise include the current coin in the count
                table[coinIndex][currentSum] = table[coinIndex - 1][currentSum] +
                                                table[coinIndex][currentSum - coins[coinIndex - 1]];
            }
        }
    }

    // Return the final count of ways to reach the target sum
    return table[numCoins][targetSum];
}

int main() {
    // Create a 2D array for the dynamic programming table
    int dpTable[numCoins + 1][5];

    // Initialize the dynamic programming table
    initializeTable(dpTable);

    // Calculate and print the total number of ways to reach the target sum
    printf("Total Ways: %d\n", countWays(dpTable));

    return 0;
}
