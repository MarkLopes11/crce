#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define MAX 100

typedef struct{
    char symbol;
    char type[20];
} Symbol;

Symbol symbolTable[MAX];
int symbolCount = 0;

int search(char symbol)
{
    for (int i = 0; i < symbolCount; i++)
    {
        if (symbolTable[i].symbol == symbol){
            return i; // found
        }
    }
    return -1; // not found
}

void insert(char symbol, char *type)
{
    if (search(symbol) != -1)
    {
        printf("Duplicate Symbol: %c \n", symbol);
        return;
    }
    symbolTable[symbolCount].symbol = symbol;
    strcpy(symbolTable[symbolCount].type, type);
    symbolCount++;
}

void createSymbolTable(char *expr)
{
    printf("The expression is: %s \n", expr);

    for (int i = 0; expr[i] != '\0'; i++)
    {
        if (expr[i] == '$')
        {
            break; // expression terminated
        }
        if (isalpha(expr[i]))  // If the character is an alphabet (identifier)
        {
            insert(expr[i], "identifier");
        }
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '=' || expr[i] == '*')  // If it's an operator
        {
            insert(expr[i], "operator");
        }
        else if (isdigit(expr[i]))  // If it's a digit (constant)
        {
            insert(expr[i], "constant");
        }
    }
    printf("Table is created.");
}

void display_table()
{
    printf("\nSymbol Table\n");
    printf("Symbol      Type\n");
    for(int i  = 0; i < symbolCount; i++)
    {
        printf("%c      %s\n", symbolTable[i].symbol, symbolTable[i].type);
    }
}

void delete_Symbol(char symbol)
{
    int pos = search(symbol);
    if(pos == -1){
        printf("Symbol not found.");
        return;
    }
    for(int i = pos; i < symbolCount -1; i++)
    {
        symbolTable[i] = symbolTable[i+1];
    }
    symbolCount--;
    printf("Symbol %c is deleted successfully.\n", symbol);
}
int main()
{
    char expr[MAX];
    int choice;

    while(1) {
        printf("Enter your choice: \n1. Enter your expression\n2. Create Table\n3. Display Table\n4. Delete Symbol\n5.Exit\n");
        scanf("%d", &choice);
        
        // Clear the input buffer
        while(getchar() != '\n'); 

        switch (choice)
        {
        case 1:
            printf("Enter your expression: ");
            fgets(expr, MAX, stdin);  // Read the expression

            // Remove newline character if present
            expr[strcspn(expr, "\n")] = 0;
            break;

        case 2:
            createSymbolTable(expr);
            break;

        case 3:
            display_table();
            break;

        case 4:
            char n;
            printf("Enter the symbol to be deleted");
            scanf("%c",&n);
            delete_Symbol(n);
            break;  

        case 5:
            printf("Exiting program.....");
            return;

        default:
            printf("Invalid choice. Please enter a valid option.\n");
            break;
        }
    }
}
