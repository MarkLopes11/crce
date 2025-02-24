#include <stdio.h>

// Function to perform a recursive linear search
int linear_search_rec(int a[], int n, int x)
{
    // Base case: If the array is empty (n == 0), the element is not found
    if (n == 0)
        return -1;
    // If the last element in the array matches the target element 'x', return its position 'n'
    else if (a[n - 1] == x)
        return n;
    // If the last element does not match, recursively search the rest of the array
    else
        return linear_search_rec(a, n - 1, x);
}

int main()
{
    int n, x;

    // Input the number of elements in the array
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);

    // Declare an array of size 'n'
    int a[n];

    // Input the array elements
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    // Input the element to search for
    printf("Enter the element to search for: ");
    scanf("%d", &x);

    // Call the linear search function and store the result
    int result = linear_search_rec(a, n, x);

    // Check if the element was found or not, and print the result
    if (result == -1)
    {
        printf("Element %d not found in the array.\n", x);
    }
    else
    {
        printf("Element %d found at position %d.\n", x, result);
    }

    return 0;
}
