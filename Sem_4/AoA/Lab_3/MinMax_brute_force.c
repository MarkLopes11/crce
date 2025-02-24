#include<stdio.h>

void findMaxMin(int a[], int n, int *max, int *min) {
    *min = a[0];
    *max = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] > *max)
            *max = a[i];
        if (a[i] < *min)
            *min = a[i];
    }

    printf("The max is %d and the min is %d\n", *max, *min);
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
    findMaxMin(a, n, &max, &min);

    return 0;
}
