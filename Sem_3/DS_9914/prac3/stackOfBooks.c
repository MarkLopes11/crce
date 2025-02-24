// Stack of floats using arrays
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10        // size of stack
#define STRING_SIZE 50 // max size of strings
// Structure of book
typedef struct
{
    int id;
    char title[STRING_SIZE];
    char author[STRING_SIZE];
    int price;
} Book;
// Structure for a stack of array of floats
typedef struct
{
    Book books[SIZE]; // array of Book struct
    int tos;          // the index of the last elements of the stack
} Stack;
/*
pushes an element to the top of the stack
inputs the Stack as a pointer:have direct access to the stack.
inputs the element: to push on tos
*/

void push(Stack *stk, Book curBook)
{
    // -> used to access the element through structure's pointer
    // Note:pre-increment required.
    stk->tos++;
    stk->books[stk->tos] = curBook; // push at tos
    printf("\nThe pushed book is:\n\tBook Name :\t%s\n\tBook Author :\t%s\n\tBook Id :\t%d\n\tBook Price :\tRs.%d/-\nCurrently the tos is :\t%d\n\n", curBook.title, curBook.author, curBook.id, curBook.price, stk->tos);
}
/*
peeks the last book of the stack. and decrements the tos.
if we push an element then the element will directly get replaced so we
don't have to worry about deleting the element.
hence only the stack is referenced through the address since we don't need
the element
*/
Book pop(Stack *stk)
{
    // checks whether the stack is empty or not
    if (stk->tos == -1)
    {
        printf("Stack Underflow\n\n");
        Book nullBook = {.id = 0, .title = "", .author = "", .price = 0};
        return nullBook;
    }
    else
    {
        // expansion of stk->books[stk->tos]title is(*stk).(*(books+(*stk).tos)).name; here name doesn't require an arrow operator because books are not in address format.

        printf("\nThe popped book is:\n\tBook Name :\t%s\n\tBook Author :\t%s\n\tBook Id :\t%d\n\tBook Price :\tRs.%d/-\nCurrently the tos is :\t%d\n\n", stk->books[stk->tos].title, stk->books[stk->tos].author, stk->books[stk->tos].id, stk->books[stk->tos].price, stk->tos - 1);
        return (stk->books[stk->tos--]);
    }
}
/*
returns the book at the top of the stack
stack is not referenced by address since we only want to view the value and
not edit it
*/
Book peek(Stack stk)
{
    return stk.books[stk.tos]; // return top element of stack
}

/*
Displays all the books of the stack
stack is not referenced by address since we only want to view the value and
not edit it
*/
void display(Stack stk)
{
    int i;
    printf("\nThe stack of books is:\n\n");
    for (i = stk.tos; i >= 0; i--)
    {
        printf("\tBook Name :\t%s\n\tBook Author :\t%s\n\tBook Id :\t%d\n\tBook Price :\tRs.%d/-\n", stk.books[i].title, stk.books[i].author,
               stk.books[i].id, stk.books[i].price);
    }
    printf("\n");
}
int main()
{
    Stack s1;       // initializing the stack
    Book curBook;   // the book to be passed at various functions is stored here
    Book dataAtTos; // last book of the stack
    int option;     // selection of operation on the stack
    s1.tos = -1;    // top of stack initialized to -1. If not done then you get segmentation fault
    // while true
    while (1)
    {
        printf("Enter the option of the choice 1.Push 2.Pop 3.Peek 4.Display 5.Close\n");
        scanf("%d", &option);
        switch (option)
        {
        case 1:

            // defined here so that the user doesn't have to enter all the details and then see that the stack has overflown.
            if (s1.tos == SIZE - 1)
            {
                printf("Stack Overflow\n\n");
            }
            else
            {
                printf("\nEnter the name of the book:\t");
                scanf("%s", &curBook.title);
                printf("Enter the author of the book:\t");
                scanf(" %s", &curBook.author);
                printf("Enter the id of the book:\t");
                scanf("%d", &curBook.id);
                printf("Enter the price of the book in Rs:\t");
                scanf("%d", &curBook.price);
                push(&s1, curBook); // pushes the book on the stack
                break;
            }
        case 2:
            dataAtTos = pop(&s1); // stores the return value of pop for future use
            break;
        case 3: // peeks the top of stack
            dataAtTos = peek(s1);
            printf("The book at tos is:\n\tBook Name :\t%s\n\tBook Author :\t%s\n\tBook Id :\t%d\n\tBook Price :\tRs.%d/-\nCurrently the tos is :\t%d\n\n", dataAtTos.title, dataAtTos.author, dataAtTos.id, dataAtTos.price, s1.tos);
            break;
        case 4:
            display(s1); // displays all elements of the stack
            break;
        case 5:
            exit(0); // exit from program with exit status 0
        default:
            printf("Enter a valid option\n");
        }
    }
    return 0;
}