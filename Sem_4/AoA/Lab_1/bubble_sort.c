#include <stdio.h>

// Function to perform Bubble Sort
void bubbleSort(int arr[], int n)
{
    for (int pass = 0; pass < n - 1; pass++) // iterate through all the passes
    {
        for (int i = 0; i < n - pass - 1; i++) // iterate through all comparisons
        {
            
            if (arr[i] > arr[i + 1]) // if element is greater then swap
            {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
            }
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
    int arr[] = {34,677,87,2,45};
    int n = getArraySize(arr);

    printf("Original array: ");
    printArray(arr, n);

    bubbleSort(arr, n);

    printf("Sorted array: ");
    printArray(arr, n);

    return 0;
}
