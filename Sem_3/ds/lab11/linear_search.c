#include <stdio.h>

// Function to perform a linear search
int linear_search(int a[], int n, int x)
{
    int i = 0;

    // Iterate through the array until the end or until the element is found
    while (i < n)
    {
        if (x == a[i])
            break; // Exit the loop if the element is found
        i++;
    }

    // If 'i' reaches 'n', the element was not found; return -1, otherwise, return the position (+1) where it was found
    if (i == n)
        return -1;
    else
        return (i + 1);
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
    int result = linear_search(a, n, x);

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
