#include <stdio.h>
#include <stdlib.h>

// Node struct
typedef struct node
{
    int data;
    struct node *next;
} Node;

// StackLL structure
typedef struct
{
    Node *tos;
} StackLL;

// push method
void push(StackLL *s, int x)
{
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    p->data = x;
    p->next = s->tos; // whether it is NULL or not it doesn't matter
    s->tos = p;
}

// pop method
void pop(StackLL *s)
{
    Node *p;
    int x;
    if (s->tos == NULL)
    {
        printf("Stack underflow\n");
        return;
    }
    else
    {
        p = s->tos;
        x = p->data;
        s->tos = p->next;
        free(p);
        printf("The popped element is:%d\n", x);
    }
}

// display the elements of the stack
void display(StackLL l)
{
    Node *p;
    p = l.tos;
    printf("The Stack is:\n");
    while (p != NULL) // while node exists
    {
        printf("%d\n", p->data);
        p = p->next;
    }
}

// main function
int main()
{
    StackLL l1;
    l1.tos = NULL;
    int option, x, ele;
    // menu driven
    while (1)
    {
        printf("Enter option: 1.Push  2.Pop  3.Display  4.Exit :\t");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("Enter element to be pushed :\t");
            scanf("%d", &x);
            push(&l1, x);
            break;
        case 2:
            pop(&l1);
            break;
        case 3:
            display(l1);
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("Enter a valid option\n");
        }
    }
    return 0;
}
