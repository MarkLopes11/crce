#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_REGISTERS 4
#define MAX_VARIABLES 26  // One for each letter of the alphabet
#define MAX_STATEMENTS 50
#define MAX_EXPR_LEN 100

// Register descriptor - stores which variable is held in which register
char registers[MAX_REGISTERS][10];
bool is_register_free[MAX_REGISTERS];

// Address descriptor - stores the current location of each variable
typedef struct {
    char location[10]; // "MEMORY" or "REGISTER"
    int register_num;  // Register number if in register
} AddressDesc;

AddressDesc address_descriptor[MAX_VARIABLES];

// Structure to store the statements
typedef struct {
    char x[10];
    char y[10];
    char op[10];
    char z[10];
} Statement;

// Array to store all statements
Statement statements[MAX_STATEMENTS];

// Utility functions
int get_register_for(char *var) {
    // Check if var is a single letter
    if (strlen(var) == 1 && var[0] >= 'a' && var[0] <= 'z') {
        int idx = var[0] - 'a';
        if (strcmp(address_descriptor[idx].location, "REGISTER") == 0) {
            return address_descriptor[idx].register_num;
        }
    }
    
    // Alternative method: check the register descriptor
    for (int i = 0; i < MAX_REGISTERS; i++) {
        if (strcmp(registers[i], var) == 0) {
            return i;
        }
    }
    return -1;
}

int get_free_register() {
    for (int i = 0; i < MAX_REGISTERS; i++) {
        if (is_register_free[i]) {
            return i;
        }
    }
    return -1;
}

// Get index in address_descriptor for a variable
int get_var_index(char *var) {
    // Simple mapping: 'a' -> 0, 'b' -> 1, etc.
    if (strlen(var) == 1 && var[0] >= 'a' && var[0] <= 'z') {
        return var[0] - 'a';
    }
    return -1;  // Invalid variable name
}

// Map operator to assembly instruction
void map_operator(char *op, char *assembly_op) {
    if (strcmp(op, "-") == 0) {
        strcpy(assembly_op, "SUB");
    } else if (strcmp(op, "+") == 0 || strcmp(op, "=") == 0) {
        strcpy(assembly_op, "ADD");
    } else if (strcmp(op, "*") == 0) {
        strcpy(assembly_op, "MUL");
    } else if (strcmp(op, "/") == 0) {
        strcpy(assembly_op, "DIV");
    } else {
        // Default case - use the operator as is
        strcpy(assembly_op, op);
    }
}

int getreg(char *x, char *y, char *z) {
    // Check if y is in a register
    int y_reg = get_register_for(y);
    if (y_reg != -1) {
        return y_reg;
    }
    
    // Try to find a free register
    int free_reg = get_free_register();
    if (free_reg != -1) {
        return free_reg;
    }
    
    // If no free registers, make one free by spilling
    // (For simplicity, just use R0)
    return 0;
}

void generate_code(char *x, char *y, char *op, char *z) {
    int reg = getreg(x, y, z);
    int y_reg = get_register_for(y);
    int z_reg = get_register_for(z);
    int x_index = get_var_index(x);
    
    char assembly_op[10];
    map_operator(op, assembly_op);
    
    // Handle Y operand
    if (y_reg == -1) {
        // Y is not in a register, load it
        printf("MOV %s, R%d\n", y, reg);
    } else {
        // Y is already in register, use that register
        reg = y_reg;
    }
    
    // Handle operation with Z operand
    if (z_reg == -1) {
        printf("%s %s, R%d\n", assembly_op, z, reg);
    } else {
        printf("%s R%d, R%d\n", assembly_op, z_reg, reg);
    }
    
    // Update descriptors
    if (x_index >= 0 && x_index < MAX_VARIABLES) {
        strcpy(registers[reg], x);
        strcpy(address_descriptor[x_index].location, "REGISTER");
        address_descriptor[x_index].register_num = reg;
        is_register_free[reg] = false;
    }
}

// Parse an expression like "t = a - b" into components
bool parse_expression(char *expr, Statement *stmt) {
    char *token;
    char *saveptr;
    
    // Make a copy of the expression to tokenize
    char expr_copy[MAX_EXPR_LEN];
    strcpy(expr_copy, expr);
    
    // Get the left side (result variable)
    token = strtok_r(expr_copy, "=", &saveptr);
    if (token == NULL) return false;
    
    // Trim spaces
    while (*token == ' ') token++;
    char *end = token + strlen(token) - 1;
    while (end > token && *end == ' ') end--;
    *(end + 1) = '\0';
    
    strcpy(stmt->x, token);
    
    // Get the right side of the expression
    token = strtok_r(NULL, "", &saveptr);
    if (token == NULL) return false;
    
    // Trim spaces
    while (*token == ' ') token++;
    
    // Find the operator
    char *op_pos = NULL;
    if (strchr(token, '+') != NULL) op_pos = strchr(token, '+');
    else if (strchr(token, '-') != NULL) op_pos = strchr(token, '-');
    else if (strchr(token, '*') != NULL) op_pos = strchr(token, '*');
    else if (strchr(token, '/') != NULL) op_pos = strchr(token, '/');
    
    if (op_pos == NULL) {
        // No operator found - just a single variable on the right
        strcpy(stmt->y, token);
        strcpy(stmt->op, "="); // assignment
        strcpy(stmt->z, "0");  // dummy value
        return true;
    }
    
    // Extract the operands and operator
    char op = *op_pos;
    *op_pos = '\0'; // Split the string at the operator
    
    // Get the first operand
    char *first_operand = token;
    while (*first_operand == ' ') first_operand++;
    end = first_operand + strlen(first_operand) - 1;
    while (end > first_operand && *end == ' ') end--;
    *(end + 1) = '\0';
    
    // Get the second operand
    char *second_operand = op_pos + 1;
    while (*second_operand == ' ') second_operand++;
    end = second_operand + strlen(second_operand) - 1;
    while (end > second_operand && *end == ' ') end--;
    *(end + 1) = '\0';
    
    strcpy(stmt->y, first_operand);
    stmt->op[0] = op;
    stmt->op[1] = '\0';
    strcpy(stmt->z, second_operand);
    
    return true;
}

int main() {
    // Initialize registers and address descriptors
    for (int i = 0; i < MAX_REGISTERS; i++) {
        is_register_free[i] = true;
        registers[i][0] = '\0';
    }
    for (int i = 0; i < MAX_VARIABLES; i++) {
        strcpy(address_descriptor[i].location, "MEMORY");
        address_descriptor[i].register_num = -1;
    }

    int num_statements;
    printf("Enter number of statements: ");
    scanf("%d", &num_statements);
    getchar(); // Consume the newline
    
    // First collect all statements
    printf("\n=== Input Expressions ===\n");
    for (int i = 0; i < num_statements; i++) {
        char expr[MAX_EXPR_LEN];
        printf("Enter expression %d (e.g., 't = a - b'): ", i + 1);
        fgets(expr, MAX_EXPR_LEN, stdin);
        expr[strcspn(expr, "\n")] = 0; // Remove newline
        
        if (!parse_expression(expr, &statements[i])) {
            printf("Error parsing expression: %s\n", expr);
            i--; // Try again
            continue;
        }
    }

    // Then generate code for all statements
    printf("\n=== Generated Assembly Code ===\n");
    for (int i = 0; i < num_statements; i++) {
        generate_code(statements[i].x, statements[i].y, statements[i].op, statements[i].z);
    }

    return 0;
}