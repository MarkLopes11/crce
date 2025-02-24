#include <stdio.h>

int linearSearch(int arr[], int n, int x)
{
    if (n < 0)
        return -1;
    else if (arr[n] == x)
        return n;
    else
        return linearSearch(arr, n - 1, x);
}

int main()
{
    int n, i, x;
    printf("How many elements? ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter array elements: ");
    for (i = 0; i < n; ++i)
    {
        scanf("%d", &arr[i]);
    }
    printf("Enter element to search: ");
    scanf("%d", &x);
    int result = linearSearch(arr, n - 1, x);
    if (result == -1)
        printf("Element not found");
    else
        printf("Element found at index %d", result);
    return 0;
}
