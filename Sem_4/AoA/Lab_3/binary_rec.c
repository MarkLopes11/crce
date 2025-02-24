#include<stdio.h>

int binary_rec(int a[], int low, int high, int x) {
    if (low > high)
        return 0;  // Element not found
    else {
        int mid = (low + high) / 2;
        if (a[mid] == x)
            return mid + 1;  // Element found, returning position (index + 1)
        else if (a[mid] > x)
            return binary_rec(a, low, mid - 1, x);
        else
            return binary_rec(a, mid + 1, high, x);
    }
}

int main() {
    int n, x, answer, low = 0;
    printf("Enter size of array:");
    scanf("%d", &n);

    int a[n];
    int high = n - 1;

    printf("Enter data in the array:");
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    printf("Enter the data to be searched:");
    scanf("%d", &x);

    answer = binary_rec(a, low, high, x);

    if (answer != 0) {
        printf("The data %d is at position %d\n", x, answer);
    } else {
        printf("The data %d is not present in the array\n", x);
    }

    return 0;
}
