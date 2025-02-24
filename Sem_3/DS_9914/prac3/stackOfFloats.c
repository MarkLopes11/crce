// Stack of floats using arrays
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10 //
// Structure for a stack of array of floats
typedef struct
{
    float arr[SIZE]; // array of floats
    int tos;         // the index of the last elements of the stack
} Stack;

/*
pushes an element to the top of the stack
inputs the Stack as a pointer:have direct access to the stack.
inputs the element: to push on tos
*/
void push(Stack *stk, float ele)
{
    // -> used to access the element through structure's pointer
    if (stk->tos == SIZE - 1)
    {
        printf("Stack Overflow\n\n");
    }
    else
    {
        // increment the tos by one
        stk->tos++;
        stk->arr[stk->tos] = ele; // push at tos
        printf("The pushed element is %f\nCurrently the tos is %d\n\n", ele,
               stk->tos);
    }
}

/*
peeks the last element of the stack. and decrements the tos.
if we push an element then the element will directly get replaced so we
don't have to worry about deleting the element.
hence only the stack is referenced through the address since we don't need
the element
*/
float pop(Stack *stk)
{
    // checks whether the stack is empty or not
    if (stk->tos == -1)
    {
        printf("Stack Underflow\n\n");
        return (0);
    }
    else
    {
        printf("The popped element is %f\nNow the tos is %d\n\n",
               stk->arr[stk->tos], stk->tos - 1);
        return (stk->arr[stk->tos--]);
    }
}
/*
returns the element at the top of the stack
stack is not referenced by address since we only want to view the value and
not edit it
*/
float peek(Stack stk)
{
    return stk.arr[stk.tos]; // return top element of stack
}
/*
Displays all the elements of the stack
stack is not referenced by address since we only want to view the value and
not edit it
*/
void display(Stack stk)
{

    int i;
    printf("Elements of the stack are:\n\n");
    for (i = stk.tos; i >= 0; i--)
    {
        printf("\t|%f|\n", stk.arr[i]);
    }
}
// Main starts
int main()
{
    Stack s1; // initializing the stack
    float element, dataAtTos;
    int option;
    s1.tos = -1; // top of stack initialized to -1
    // while true
    while (1)
    {
        printf("Enter option of the choice 1.Push 2.Pop 3.Peek 4.Display 5.Close\n");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("Enter the element to push:\t\n");
            scanf("%f", &element);
            push(&s1, element);
            break;
        case 2:
            dataAtTos = pop(&s1);
            break;
        case 3: // peeks the top of stack
            printf("The element at tos is :\t%f\n\n", peek(s1));
            break;
        case 4:
            display(s1);
            break;
        case 5:
            exit(0); // exit from program with exit status 0
        }
    }
    return 0;
}