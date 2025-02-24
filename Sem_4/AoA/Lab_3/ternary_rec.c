#include<stdio.h>

int ternarySearchRec(int a[], int low, int high, int ele) {
    if (low > high)
        return -1; // If the element is not found
    
    int mid1 = low + (high - low) / 3;    //mid1 calculation
    int mid2 = high - (high - low) / 3;    //mid2 calculation
    
    if (a[mid1] == ele)    //if ele == element at mid1
        return mid1;
    else if (a[mid2] == ele)    //if ele == element at mid2
        return mid2;
    else if (a[mid1] > ele)    //part 1 of array
        return ternarySearchRec(a, low, mid1 - 1, ele);
    else if (ele > a[mid2])    //part 3
        return ternarySearchRec(a, mid2 + 1, high, ele);
    else    //part 2, middle part
        return ternarySearchRec(a, mid1 + 1, mid2 - 1, ele);
}

int main() {
    int n;
    printf("Enter the size of the array: ");
    scanf("%d", &n);
    
    int myNumbers[n];
    
    printf("Enter the elements of the array in sorted order: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &myNumbers[i]);
    
    int target;
    printf("Enter the target value to search: ");
    scanf("%d", &target);
    
    int index = ternarySearchRec(myNumbers, 0, n - 1, target);
    if (index != -1)
        printf("Element found at index %d\n", index);
    else
        printf("Element not found in the array\n");
        
    return 0;
}
