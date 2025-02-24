#include <stdio.h>

int binary_search(int a[], int low, int high, int ele)
{
    int mid;
    
    while (low <= high)
    {
        // Calculate the middle index
        mid = (low + high) / 2;
        
        // Check if the middle element is the target element
        if (ele == a[mid])
            return (mid + 1); // Element found, return its position
        
        // If the target element is less than the middle element,
        // search in the left half of the array
        else if (ele < a[mid])
            high = mid - 1;
        
        // If the target element is greater than the middle element,
        // search in the right half of the array
        else
            low = mid + 1;
    }
    
    // If the element is not found, return -1
    return -1;
}

int main()
{
    int n, ele;

    printf("Enter the number of elements in the sorted array: ");
    scanf("%d", &n);

    int a[n]; // Declare an array of size 'n'

    printf("Enter %d sorted elements:\n", n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    printf("Enter the element to search for: ");
    scanf("%d", &ele);

    int result = binary_search(a, 0, n - 1, ele);

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

