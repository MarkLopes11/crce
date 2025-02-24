//increment 2 variables
#include <stdio.h>

#define PI 3.14

void readMe(float *rad){
//get the value of the radius
	printf("Enter the radius of the circle: ");
	scanf("%f", rad);
}

void calcArea(float radius, float *area){
//display the area of the circle
	*area = radius * radius * PI;
}

void calcPerimeter(float radius, float *perimeter){
//display the perimeter of the circle 
	*perimeter = 2 * radius * PI;
}

void display(float rad, float area, float perimeter){
//displays the new value of the integer
printf("\n The Area of te circle with radius %.2f is %.2f\nThe perimeter of the circle with radius %.2f is %.2f", rad,area,rad,perimeter);
}

int main(){
	float rad,area,perimeter;
	readMe(&rad);
	calcPerimeter(rad, &perimeter);
	calcArea(rad, &area);
	display(rad, area, perimeter);
	return 0;
}

