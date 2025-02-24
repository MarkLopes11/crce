#include <stdio.h>

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
    for (i = 0; i < n; ++i)
    {
        if (arr[i] == x)
        {
            printf("Element found at index %d", i);
            return 0;
        }
    }
    printf("Element not found");
    return 0;
}
