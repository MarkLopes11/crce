#include <stdio.h>
#include <stdlib.h>

// checks whether there is memory available or not for the dynamic array
int checkMemory(float *a)
{
    if (a == NULL)
    {
        printf("Error!!!: Insufficient memory. Cannot proceed with the execution\n");
        exit(1);
    }
}

// sets the elements of the dynamic array
void setElements(float *a, int n, int arrNum)
{
    for (int i = 0; i < n; i++)
    {
        printf("Enter the %d element of the array %d:\t", i + 1,
               arrNum);
        scanf("%f", &a[i]);
    }
}

/*
definition: adding two arrays and storing it in another array
useCase: addingBothArrays(float *sum, float *a, float *b, int n)
---------------------------
here sum[i] = a[i] + b[i] occurs n times
*/
void addingBothArrays(float *sum, float *a, float *b, int n)
{
    for (int i = 0; i < n; i++)
    {
        sum[i] = a[i] + b[i];
    }
}

// prints the elements of the dynamic array
void getElements(float *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("The %d element of the summed array is %f\n", i + 1,
               a[i]);
    }
}
int main()
{
    int n; // number of elements in the arrays
    printf("Enter the number of elements in the array:\t");
    scanf("%d", &n);
    float *arr1 = (float *)malloc(n * sizeof(float));   // dynamic array 1
    float *arr2 = (float *)calloc(n, sizeof(float));    // dynamic arr 2
    float *sumArr = (float *)malloc(n * sizeof(float)); // dynamic sum of arr1 and arr2
    checkMemory(arr1);

    checkMemory(arr2);
    setElements(arr1, n, 1);
    setElements(arr2, n, 1);
    addingBothArrays(sumArr, arr1, arr2, n);

    getElements(sumArr, n);
    free(arr1);
    free(arr2);
    free(sumArr);
    return 0;
}