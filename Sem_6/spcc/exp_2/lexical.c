#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 100
#define MAX_TOKENS 1000

// Token type arrays
const char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

const char operators[] = {
    '+', '-', '*', '/', '%',  // arithmetic
    '=', '<', '>', '!',       // relational
    '&', '|', '^', '~',       // bitwise
    ':', '?'                  // others
};

const char delimiters[] = {
    '(', ')', '{', '}', '[', ']',
    ';', ',', '.'
};

const char* data_types[] = {
    "int", "char", "float", "double", "void",
    "long", "short", "signed", "unsigned"
};

// Arrays to store unique tokens
char identifiers[MAX_TOKENS][MAX_TOKEN_LEN];
char numbers[MAX_TOKENS][MAX_TOKEN_LEN];
char operators_found[MAX_TOKENS];
char delimiters_found[MAX_TOKENS];

int id_count = 0, num_count = 0, op_count = 0, del_count = 0;

// Helper function to check if a string is in an array
int exists_in_array(char arr[MAX_TOKENS][MAX_TOKEN_LEN], int count, char* str) {
    for (int i = 0; i < count; i++) {
        if (strcmp(arr[i], str) == 0) return 1;
    }
    return 0;
}

// Helper function to check if a character is in an array
int exists_in_char_array(char arr[MAX_TOKENS], int count, char ch) {
    for (int i = 0; i < count; i++) {
        if (arr[i] == ch) return 1;
    }
    return 0;
}

// Check if token is a keyword
int is_keyword(char* str) {
    for (int i = 0; i < sizeof(keywords) / sizeof(char*); i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

// Check if token is a data type
int is_data_type(char* str) {
    for (int i = 0; i < sizeof(data_types) / sizeof(char*); i++) {
        if (strcmp(str, data_types[i]) == 0) return 1;
    }
    return 0;
}

// Check if character is an operator
int is_operator(char ch) {
    for (int i = 0; i < sizeof(operators); i++) {
        if (ch == operators[i]) return 1;
    }
    return 0;
}

// Check if character is a delimiter
int is_delimiter(char ch) {
    for (int i = 0; i < sizeof(delimiters); i++) {
        if (ch == delimiters[i]) return 1;
    }
    return 0;
}

// Helper function to print a line separator
void print_separator(int width, char style) {
    printf("+");
    for (int i = 0; i < width-1; i++) {
        printf("%c", style);
    }
    printf("+");
    for (int i = 0; i < width-1; i++) {
        printf("%c", style);
    }
    printf("+\n");
}

// Helper function to print wrapped text in a column
void print_wrapped_text(const char* text, int width) {
    int len = strlen(text);
    int pos = 0;
    int chars_remaining = len;
    int first_line = 1;

    while (chars_remaining > 0) {
        int chars_to_print = chars_remaining;
        if (chars_to_print > width - 2) {
            chars_to_print = width - 2;
            // Find last space before width limit
            while (chars_to_print > 0 && text[pos + chars_to_print] != ' ' && text[pos + chars_to_print] != ',') {
                chars_to_print--;
            }
            if (chars_to_print == 0) chars_to_print = width - 2; // If no space found, force break
        }

        if (first_line) {
            first_line = 0;
        } else {
            printf("| %*s | ", width-2, "");
        }

        printf("%-*.*s%*s |\n", 
               chars_to_print, chars_to_print, 
               text + pos,
               width - chars_to_print - 2, "");

        pos += chars_to_print;
        while (pos < len && (text[pos] == ' ' || text[pos] == ',')) pos++;
        chars_remaining = len - pos;
    }
}

int main() {
    printf("################################################################################################\n");
    printf("##########################           LEXICAL ANALYSIS            ###############################\n");
    printf("################################################################################################\n");
    FILE *file_ptr;
    char ch, token[MAX_TOKEN_LEN];
    int i = 0;

    file_ptr = fopen("fibo.c", "r");
    if (NULL == file_ptr) {
        printf("File can't be opened\n");
        return EXIT_FAILURE;
    }

    while ((ch = fgetc(file_ptr)) != EOF) {
        if (isspace(ch)) continue;

        if (isalpha(ch) || ch == '_') {  
            i = 0;
            token[i++] = ch;
            while ((ch = fgetc(file_ptr)) != EOF && (isalnum(ch) || ch == '_')) {
                token[i++] = ch;
            }
            token[i] = '\0';
            ungetc(ch, file_ptr);

            if (!is_keyword(token) && !is_data_type(token) && !exists_in_array(identifiers, id_count, token)) {
                strcpy(identifiers[id_count++], token);
            }
        }
        else if (isdigit(ch)) {  
            i = 0;
            token[i++] = ch;
            while ((ch = fgetc(file_ptr)) != EOF && (isdigit(ch) || ch == '.')) {
                token[i++] = ch;
            }
            token[i] = '\0';
            ungetc(ch, file_ptr);

            if (!exists_in_array(numbers, num_count, token)) {
                strcpy(numbers[num_count++], token);
            }
        }
        else if (is_operator(ch)) {  
            if (!exists_in_char_array(operators_found, op_count, ch)) {
                operators_found[op_count++] = ch;
            }
        }
        else if (is_delimiter(ch)) {  
            if (!exists_in_char_array(delimiters_found, del_count, ch)) {
                delimiters_found[del_count++] = ch;
            }
        }
    }

    fclose(file_ptr);

    int column_width = 50;

    // Print table header with dashes
    print_separator(column_width, '-');
    printf("| %-*s | %-*s |\n", column_width-2, "Category", column_width-2, "Value");
    print_separator(column_width, '_');  // Using underscore after Category

    // Print IDENTIFIER
    printf("| %-*s | ", column_width-2, "IDENTIFIER");
    char id_buffer[MAX_TOKENS * MAX_TOKEN_LEN] = {0};
    for (int j = 0; j < id_count; j++) {
        strcat(id_buffer, identifiers[j]);
        if (j < id_count - 1) strcat(id_buffer, ", ");
    }
    print_wrapped_text(id_buffer, column_width);
    print_separator(column_width, '_');  // Add separator after each category

    // Print OPERATOR
    printf("| %-*s | ", column_width-2, "OPERATOR");
    char op_buffer[MAX_TOKENS * 2] = {0};
    for (int j = 0; j < op_count; j++) {
        char temp[3] = {operators_found[j], 0};
        strcat(op_buffer, temp);
        if (j < op_count - 1) strcat(op_buffer, ", ");
    }
    print_wrapped_text(op_buffer, column_width);
    print_separator(column_width, '_');  // Add separator after each category

    // Print NUMBER
    printf("| %-*s | ", column_width-2, "NUMBER");
    char num_buffer[MAX_TOKENS * MAX_TOKEN_LEN] = {0};
    for (int j = 0; j < num_count; j++) {
        strcat(num_buffer, numbers[j]);
        if (j < num_count - 1) strcat(num_buffer, ", ");
    }
    print_wrapped_text(num_buffer, column_width);
    print_separator(column_width, '_');  // Add separator after each category

    // Print DELIMITER
    printf("| %-*s | ", column_width-2, "DELIMITER");
    char del_buffer[MAX_TOKENS * 2] = {0};
    for (int j = 0; j < del_count; j++) {
        char temp[3] = {delimiters_found[j], 0};
        strcat(del_buffer, temp);
        if (j < del_count - 1) strcat(del_buffer, ", ");
    }
    print_wrapped_text(del_buffer, column_width);
    print_separator(column_width, '-');  // Using dash for final separator

    // Calculate and print total tokens
    int keyword_count = sizeof(keywords) / sizeof(keywords[0]);
    int datatype_count = sizeof(data_types) / sizeof(data_types[0]);
    int total_tokens = id_count + num_count + op_count + del_count + keyword_count + datatype_count;
    printf("\nTotal number of tokens: %d\n", total_tokens);

    return 0;
}