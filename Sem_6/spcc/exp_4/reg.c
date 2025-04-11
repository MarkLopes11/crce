#include <stdio.h>
#include <string.h>

#define MAX_REGISTERS 8
#define MAX_EXPRESSIONS 100

// Register status
int reg_status[MAX_REGISTERS] = {0}; // 0 = free, non-zero = holds a variable

// Expression structure
typedef struct {
    char x;
    char y;
    char z;
    char op;
} Expression;

Expression expressions[MAX_EXPRESSIONS];  
int expression_count = 0;                

// Map operator to instruction
const char* map_operator(char op) {
    switch (op) {
        case '+': return "ADD";
        case '-': return "SUB";
        case '*': return "MUL";
        case '/': return "DIV";
        default: return "INVALID";
    }
}

// Get available register or reuse one
int getreg(char var) {
    // Step 1: If y is already in a register and no other variables are there, use it
    for (int i = 0; i < MAX_REGISTERS; i++) {
        if (reg_status[i] == var) {
            return i;
        }
    }

    // Step 2: Find an empty register
    for (int i = 0; i < MAX_REGISTERS; i++) {
        if (reg_status[i] == 0) {
            reg_status[i] = var;
            return i;
        }
    }

    // Step 3: If no empty register, evict the first one (simple replacement strategy)
    printf("MOV R0, MEM\n"); // Store the value of R0 to memory
    reg_status[0] = var;
    return 0;
}

// Generate code for each expression
void generate_code() {
    int reg_count = 0;

    printf("\nGenerated Code:\n");
    for (int i = 0; i < expression_count; i++) {
        const char *operation = map_operator(expressions[i].op);

        if (strcmp(operation, "INVALID") == 0) {
            printf("Invalid operator in expression %d!\n", i + 1);
            continue;
        }

        int reg_y = getreg(expressions[i].y);
        int reg_z = getreg(expressions[i].z);

        // Load y into register if not already loaded
        printf("MOV %c, R%d\n", expressions[i].y, reg_y);
        
        // Perform operation
        printf("%s %c, R%d\n", operation, expressions[i].z, reg_y);
        
        // Store result in x
        printf("MOV R%d, %c\n", reg_y, expressions[i].x);

        // Update register status
        reg_status[reg_y] = expressions[i].x;
    }
}

int main() {
    printf("Enter number of expressions: ");
    scanf("%d", &expression_count);

    for (int i = 0; i < expression_count; i++) {
        printf("\nEnter statement %d in the form x = y op z:\n", i + 1);
        scanf(" %c = %c %c %c", &expressions[i].x, &expressions[i].y, 
                                 &expressions[i].op, &expressions[i].z);
    }

    generate_code();

    return 0;
}
