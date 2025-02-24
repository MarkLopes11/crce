#include <stdio.h>

void selectionSort(int arr[], int n)
{
    int pass, j, minIndex;

    for (pass = 0; pass < n - 1; pass++) //iterate through every pass
    {
        minIndex = pass;

        // Find the index of the minimum element in the unsorted part of the array
        for (j = pass + 1; j < n; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }

        // Swap the found minimum element with the first element and the minimum element is now in sorted part of array
        int temp = arr[pass];
        arr[pass] = arr[minIndex];
        arr[minIndex] = temp;
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
    int arr[] = {34,76,50,1,48};
    int n = getArraySize(arr);

    printf("Original array: ");
    printArray(arr, n);

    selectionSort(arr, n);

    printf("Sorted array: ");
    printArray(arr, n);

    return 0;
}
