#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OLD_SIZE 20
#define NEW_SIZE 50

// checks whether there is space available or not
void checkMemory(char *ptr)
{
    if (ptr == NULL)
    {
        printf("Error!!!: Insufficient memory. Cannot proceed with the execution\n");

        exit(1);
    }
}

int main()
{
    char *a = (char *)malloc(OLD_SIZE * sizeof(char)); // assigned 'a' a location dynamically
    checkMemory(a);
    strcpy(a, "Data Reallocation"); // puts the right string in the left pointer as as cannot assign a string the normal way
    printf("Value of a before reallocation :%s\nSize :\t%d\n\n", a, OLD_SIZE);
    a = (char *)realloc(a, NEW_SIZE * sizeof(char)); // reallocated a new size memory to 'a'
    checkMemory(a);
    strcat(a, " with realloc(datatype, new size)"); //
    printf("Value of a after reallocation :%s\nSize :\t%d\n", a,
           NEW_SIZE);
    free(a); // free memory
    return 0;
}