#include <stdio.h>
#include <stdbool.h>

void ModifiedBubbleSort(int arr[], int n)
{
    int pass, j;
    bool exchange; // flag to check if any exchange or swaps are made between passes

    for (pass = 0; pass < n - 1; pass++)
    {
        exchange = false; // No swaps made yet in this pass

        for (j = 0; j < n - pass - 1; j++)
        {

            if (arr[j] > arr[j + 1])  // if element is greater then swap
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                exchange = true; // Set the flag to indicate a swap
            }
        }

        // If no swaps were made, the array is already sorted
        if (!exchange)
        {
            break;
        }
    }
}

// Function to print an array
void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int getArraySize(int arr[])
{
    int size = 0;

    // Iterate through the array until the end marker is found
    while (arr[size] != '\0')
    {
        size++;
    }

    return size;
}

int main()
{
    int arr[] = {2,56,34,90,67};
    int n = getArraySize(arr);

    printf("Original array: ");
    printArray(arr, n);

    ModifiedBubbleSort(arr, n);

    printf("Sorted array: ");
    printArray(arr, n);

    return 0;
}
