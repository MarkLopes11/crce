#include <stdio.h>

int binarySearch(int arr[], int n, int target) {
    int low = 0;
    int high = n - 1;
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // If the target is found at the middle return
        if (arr[mid] == target)
            return mid;
        
        // If the target is greater than mid, go to the right half of array
        else if (arr[mid] < target)
            low = mid + 1;
        
        // If the target is smaller than mid, go to the left half of the array
        else
            high = mid - 1;
    }
    
    // Target not found
    return -1;
}

int main() {
    int n;
    printf("Enter the size of the array: ");
    scanf("%d", &n);
    
    int arr[n];
    printf("Enter the elements of the array in sorted order: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    
    int target;
    printf("Enter the target value to search: ");
    scanf("%d", &target);
    
    int result = binarySearch(arr, n, target);
    if (result != -1)
        printf("Element found at index %d\n", result);
    else
        printf("Element not found in the array\n");
    
    return 0;
}
