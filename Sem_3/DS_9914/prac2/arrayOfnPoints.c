#include <stdio.h>
#include <stdlib.h>

// Point structure
typedef struct
{
    int x;
    int y;
} Point;

// checks whether there is space available or not
void checkSpace(Point *points)
{
    if (points == NULL)
    {
        printf("Error!!!: Insufficient memory. Cannot proceed with the execution\n");
        exit(1);
    }
}

// sets the elements of the array
void setElements(Point *points, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Enter the x and y coordinates of %d element :\t", i +
                                                                      1);
        // &(points[i].x) == &(*(points + i)).x == &((points + i)->x)
        // x->y indicates *x.y since y is linked to the value of x and not the address of x
        // x[i].y indicates (*(x + i)).y (this is a value) increment address x by i(size of datatype is automatically determined) then get its value and then get the x element linkedto its value which returns a value.

        scanf("%d%d", &(points[i].x), &(points[i].y));
    }
}

// prints the elements of the array
void getElements(Point *points, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("The Point %d is (%d, %d)\n", i + 1, points[i].x,
               (points[i].y));
    }
}
int main()
{
    int n; // number of elements of the array
    printf("Enter the number of elements of the array:\t");
    scanf("%d", &n);

    // (datatype *)calloc(number of elements, size of datatype)
    Point *points = (Point *)calloc(n, sizeof(Point)); // allocating a dynamically created array to arr[n]
    checkSpace(points);

    setElements(points, n);
    getElements(points, n);

    free(points);
    return 0;
}