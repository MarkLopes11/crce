#include <stdio.h>
#include <math.h>

typedef struct{
	int x;
	int y;

}Point;  // point datatype created

float computedDist(Point p1,Point p2){
	return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

int main(){
	Point p1, p2;
	float dist;
	
	printf("enter two points");
	scanf("%d %d %d %d", &p1.x, &p1.y, &p2.x, &p2.y);
	
	dist = computedDist(p1,p2);
	printf("the distance between the points is %f",dist);
}
