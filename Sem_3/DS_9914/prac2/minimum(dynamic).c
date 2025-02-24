#include <stdio.h>
#include <stdlib.h>

// checks whether there is space available or not
void checkSpace(float *arr)
{
	if (arr == NULL)
	{
		printf("Error: Insufficient memory. Cannot proceed with the execution\n");
		exit(1);
	}
}

// enters the elements in an array
void setElements(float *arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("Enter the %d element :\t", i + 1);
		scanf("%f", &arr[i]);
	}
}

// return the minimum value in the given array
float setMin(float *arr, int n)
{
	float min = arr[0];
	for (int i = 1; i < n; i++)

	{
		if (arr[i] < min)
		{
			min = arr[i];
		}
	}
	return min;
}

int main()
{

	int n; // number of elements of the array
	printf("Enter the number of elements of the array :\t");
	scanf("%d", &n);
	float *arr = (float *)malloc(n * sizeof(float)); // allocating a dynamically created array to arr[n]
	checkSpace(arr);
	setElements(arr, n);
	float minimum = setMin(arr, n);
	printf("The minimum from the elements you is %f", minimum);
	free(arr); // frees the allocated memory
	return 0;
}