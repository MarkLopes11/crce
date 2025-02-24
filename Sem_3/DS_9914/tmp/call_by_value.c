//increment 2 variables
#include <stdio.h>

#define PI 3.14

void readme(int *p1,int *p2){
//gets the vlaue of the integers from the user
printf("Enter any two numbers");
	scanf("%d %d",p1,p2);
}

void increment(int *x,int *y){
//increases the integers by 1
	++(*x);
	++(*y);
}

void display(int x, int y){
//displays the new value of the integer
printf("\n The new value of a= %d and b = %d\n", x, y);
}

void compute(float radius, float *area, float *perimeter){
	*area = radius * radius * PI;
	*perimeter = 2 * radius * PI;
}


int main(){
	int a,b;
	readme(&a,&b);
	increment(&a,&b);
	display(a,b);
	return 0;
}

