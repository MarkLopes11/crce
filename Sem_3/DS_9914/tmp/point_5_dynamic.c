#include <stdio.h>
#include <stdlib.h>

//#define NULL 0  
//we cannot define this because it is already defined in the stdlib library.e an use #define NULL ((void *)0) to equate it to 0.


typedef struct{
	float x;
	float y;
}Point;


//checks if there is a exiting memory or not
void checkValidity(Point *a){
	if(a == NULL){
		printf("Insufficient memory");
		exit(0);
	}
	
}
//loops the array to enter the element 
void setElements(int n, Point *p){
	for(int i = 0; i < n; i++){
		printf("Enter the coordinates of %d point", i + 1);
		scanf("%f%f",&(p[i].x), &(p[i].y));
	}
}

//loops the array to print its data
void getElements(int n, Point *p){
	for(int i = 0; i < n; i++){
		printf("Point at % is (x, y)=(%f, %f)", i + 1, p[i].x, p[i].y);
	}
}

int main(){
	int n;
	printf("Enter the number of elements :  ");
	scanf("%d",&n);
	
	//defines a dynamic pointer array 'ptr' 
	Point *p = (Point *)malloc(n * sizeof(float));

	checkValidity(p);
	
	setElements(n, p);
	getElements(n, p);
	
	//frees the dynamic pointer
	free(p);
	
	printf("\n");
	return 0;
}
