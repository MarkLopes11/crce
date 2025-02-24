#include <stdio.h>
#include <stdlib.h>

struct Item
{
    int Item_no;
    int profit;
    float weight;
    float ratio;
} arr[50], arr1[50];

void sort(struct Item *arr1, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr1[j].ratio < arr1[j + 1].ratio)
            {
                struct Item temp = arr1[j];
                arr1[j] = arr1[j + 1];
                arr1[j + 1] = temp;
            }
        }
    }
}

void fractional_knapsack(struct Item *arr, int n, int M)
{
    float total = 0;
    float x[50] = {0}; // Initialize array

    int u = M;
    int i = 0;

    while (u != 0 && i < n)
    {
        if (arr[i].weight <= u)
        {
            u -= arr[i].weight;
            total += arr[i].profit;
            x[arr[i].Item_no] = 1;
        }
        else
        {
            x[arr[i].Item_no] = u / arr[i].weight;
            total += x[arr[i].Item_no] * arr[i].profit;
            u = 0;
        }
        printf("\n Remaining capacity after sorted element %d: %d\n", i, u);
        i++;
    }
    printf("Total Profit: %f\n", total);
    printf("The solution vector is: ");
    for (i = 0; i < n; i++)
    {
        printf("%.2f ", x[i]);
    }
    printf("\n");
}

int main()
{
    int M, n, i;

    printf("Enter the total items: ");
    scanf("%d", &n);

    printf("Enter the total capacity: ");
    scanf("%d", &M);

    for (i = 0; i < n; i++)
    {
        printf("Enter the profit and weight for item %d: ", i + 1);
        scanf("%d %f", &arr[i].profit, &arr[i].weight);
        arr[i].Item_no = i;
        arr1[i] = arr[i];
    }

    for (i = 0; i < n; i++)
    {
        arr1[i].ratio = (float)arr[i].profit / arr[i].weight;
    }

    sort(arr1, n);

    printf("The sorted ratios: \n");
    for (i = 0; i < n; i++)
    {
        printf("%f ", arr1[i].ratio);
    }
    printf("\n");

    fractional_knapsack(arr1, n, M);

    return 0;
}
