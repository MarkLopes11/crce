#include <stdio.h>

// Function to perform Insertion Sort
void insertionSort(int arr[], int n)
{
    int i, key, j;

    // Iterate through the array starting from the second element (index 1)
    for (i = 1; i < n; i++)
    {
        key = arr[i]; // Current element to be inserted in the sorted array
        j = i - 1;    // Index of the previous element

        // compare the value of key(the element to be sorted) and the element in the sorted array
        while (j >= 0 && key < arr[j])
        {
            arr[j + 1] = arr[j]; // when key(element to be inserted) is smaller than element in sorted array, replace key by element in sorted array
            j = j - 1;           // decrement the value of j to compare to the previous element(in sorted array) in next iteration
        }
        arr[j + 1] = key; // Place the key at its correct position
    }
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

// Function to print an array
void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main()
{
    int arr[] = {34, 67, 99, 56, 29};
    int n = getArraySize(arr);

    printf("Original array: ");
    printArray(arr, n);

    insertionSort(arr, n);

    printf("Sorted array: ");
    printArray(arr, n);

    return 0;
}
