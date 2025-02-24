#include <stdio.h>
#include <stdlib.h>

#define SIZE 10 

void mergelist(int a[], int lb1, int ub1, int lb2, int ub2);

void mergesort(int a[], int low, int high) {
    if (low == high)
        return;
    else {
        int mid = (low + high) / 2;
        mergesort(a, low, mid);     // left subarray
        mergesort(a, mid + 1, high); // right subarray
        mergelist(a, low, mid, mid + 1, high);
    }
}

void mergelist(int a[], int lb1, int ub1, int lb2, int ub2) {
    int i = lb1;
    int j = lb2;
    int k = 0;
    int c[SIZE];

    while (i <= ub1 && j <= ub2) {
        if (a[i] < a[j])
            c[k++] = a[i++];
        else
            c[k++] = a[j++];
    }

    while (j <= ub2) {
        c[k++] = a[j++];
    }
    while (i <= ub1)
        c[k++] = a[i++];

    for (i = lb1, k = 0; i <= ub2; i++, k++)
        a[i] = c[k];
}

int main() {
    int arr[] = {12, 11, 13, 5, 6, 7};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    printf("Given array is \n");
    for (int i = 0; i < arr_size; i++)
        printf("%d ", arr[i]);
    printf("\n");

    mergesort(arr, 0, arr_size - 1);

    printf("Sorted array is \n");
    for (int i = 0; i < arr_size; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
