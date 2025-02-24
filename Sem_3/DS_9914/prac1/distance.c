#include <stdio.h>
#include <math.h>

// Structure to store the x and y coordinates
typedef struct
{
  float x;
  float y;
} Point;

// sets the values of the coordinates
void setData(Point *a)
{
  printf("Please enter the coordinates of the point :");
  scanf("%f %f", &(*a).x, &(*a).y); // we can use &a->x and &a->y to access the addresses too
}

// calculates the distance between two points and returns the distance
double calDistance(Point a, Point b)
{
  //distance = sqrt((x1-x2)^2 + (y1-y2)^2)
  double setDistance = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  return setDistance;
}
// outputs the distance obtained from calDistance
void display(double a)
{
  printf("The distance between the two points is %.3lf", a);
}

int main()
{
  Point p1, p2;
  setData(&p1);
  setData(&p2);
  double distance = calDistance(p1, p2);
  display(distance);
  // can reduce this by typing display(calDistance(p1, p2))
  return 0;
}
