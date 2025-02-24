#include <stdio.h>

// Function to perform a recursive binary search
int binary_search_rec(int a[], int low, int high, int ele)
{
    if (low > high)
        return -1; // Element not found

    int mid = (low + high) / 2;

    if (a[mid] == ele)
        return (mid + 1); // Element found, return its position

    if (ele < a[mid])
        return binary_search_rec(a, low, mid - 1, ele); // Search in the left half of the array
    else
        return binary_search_rec(a, mid + 1, high, ele); // Search in the right half of the array
}

int main()
{
    int n, ele;

    // Input the number of elements in the sorted array
    printf("Enter the number of elements in the sorted array: ");
    scanf("%d", &n);

    // Declare an array of size 'n'
    int a[n];

    // Input the sorted array elements
    printf("Enter %d sorted elements:\n", n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    // Input the element to search for
    printf("Enter the element to search for: ");
    scanf("%d", &ele);

    // Call the binary search function and store the result
    int result = binary_search_rec(a, 0, n - 1, ele);

    // Check if the element was found or not, and print the result
    if (result == -1)
    {
        printf("Element %d not found in the array.\n", ele);
    }
    else
    {
        printf("Element %d found at position %d.\n", ele, result);
    }

    return 0;
}

