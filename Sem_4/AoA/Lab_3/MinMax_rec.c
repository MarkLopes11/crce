#include <stdio.h>

void findMaxMin(int a[], int low, int high, int *max, int *min) {
    int mid;

    if (low == high) {
        *min = a[low];
        *max = a[low];
    } else if (low + 1 == high) {
        if (a[low] < a[high]) {
            *min = a[low];
            *max = a[high];
        } else {
            *min = a[high];
            *max = a[low];
        }
    } else {
        mid = (low + high) / 2;

        int leftMax, leftMin, rightMax, rightMin;
        findMaxMin(a, low, mid, &leftMax, &leftMin);
        findMaxMin(a, mid + 1, high, &rightMax, &rightMin);

        // Combine results from left and right subarrays
        if (leftMax > rightMax)
            *max = leftMax;
        else
            *max = rightMax;

        if (leftMin < rightMin)
            *min = leftMin;
        else
            *min = rightMin;
    }
}

int main() {
    int n;
    printf("Enter size of array:");
    scanf("%d", &n);

    int a[n];

    printf("Enter data in the array:");
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    int max, min;
    findMaxMin(a, 0, n - 1, &max, &min);

    printf("The max is %d and the min is %d\n", max, min);

    return 0;
}
