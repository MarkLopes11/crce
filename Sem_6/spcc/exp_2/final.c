#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

// List of keywords
const char *keywords[] = {
    "auto", "break", "case", "char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register", "return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

// List of operators
const char operators[] = "+-/%=<>!&|";

// Function to check if a given string is a keyword
int isKeyword(char *word)
{
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Function to check if a character is an operator
int isOperator(char ch)
{
    return strchr(operators, ch) != NULL;
}

// Function to classify tokens
void tokenize(char *sourceCode)
{
    char token[MAX_TOKEN_LENGTH];
    int tokenIndex = 0;

    printf("TOKEN TYPE         |         VALUE\n");
    printf("-------------------+--------------------\n");

    for (int i = 0; sourceCode[i] != '\0'; i++)
    {
        char ch = sourceCode[i];

        // Skip whitespace
        if (isspace(ch)) continue;

        // Identifiers and keywords
        if (isalpha(ch) || ch == '_')
        {
            tokenIndex = 0;
            while (isalnum(ch) || ch == '_')
            {
                token[tokenIndex++] = ch;
                ch = sourceCode[++i];
            }
            token[tokenIndex] = '\0';
            i--;

            if (isKeyword(token))
                printf("[KEYWORD]          |%16s\n", token);
            else
                printf("[IDENTIFIER]       |%16s\n", token);
        }

        // Numbers (integers and floating-point)
        else if (isdigit(ch))
        {
            tokenIndex = 0;
            while (isdigit(ch) || ch == '.')
            {
                token[tokenIndex++] = ch;
                ch = sourceCode[++i];
            }
            token[tokenIndex] = '\0';
            i--;
            printf("[NUMBER]       |%16s\n", token);
        }

        // Operators
        else if (isOperator(ch))
        {
            printf("[OPERATOR]         |%16c\n", ch);
        }

        // String Literals
        else if (ch == '\"')
        {
            tokenIndex = 0;
            token[tokenIndex++] = ch;
            ch = sourceCode[++i];

            while (ch != '\"' && ch != '\0')
            {
                token[tokenIndex++] = ch;
                ch = sourceCode[++i];
            }
            token[tokenIndex++] = '\"';
            token[tokenIndex] = '\0';
            printf("[STRING LITERAL]   |%16s\n", token);
        }

        // Special symbols (e.g., parentheses, semicolons)
        else if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ',' || ch == ';' || ch == '.' || ch == '#')
        {
            printf("[SPECIAL SYMBOL]   |%16c\n", ch);
        }

        // Undefined tokens
        else
        {
            printf("[ERROR]       |       Undefined token: %c\n", ch);
        }
    }
}

// Main function
int main()
{
    FILE *file_ptr;
    char buffer[1024];
    size_t bytes_read;

    file_ptr = fopen("./input.txt", "r");
    if (NULL == file_ptr)
    {
        printf("File can't be opened\n");
        return EXIT_FAILURE;
    }

    // Read file content
    bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file_ptr);
    buffer[bytes_read] = '\0';

    // Print and analyze
    printf("File contents:\n%s\n", buffer);
    printf("\nLexical Analysis Output:\n");
    tokenize(buffer);

    fclose(file_ptr);

    return 0;
}