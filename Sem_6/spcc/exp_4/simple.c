#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_REGISTERS 4
#define MAX_VAR 26

// Register descriptor
char registers[MAX_REGISTERS][2]; 
bool reg_free[MAX_REGISTERS];

// Address descriptor
typedef struct {
    bool in_register;
    int reg_num;
} VarLocation;

VarLocation var_locations[MAX_VAR];

// Check if variable is in a register
int get_reg(char var) {
    int idx = var - 'a';
    if (idx >= 0 && idx < MAX_VAR && var_locations[idx].in_register) {
        return var_locations[idx].reg_num;
    }
    return -1;
}

// Find a free register
int get_free_reg() {
    for (int i = 0; i < MAX_REGISTERS; i++) {
        if (reg_free[i]) return i;
    }
    return 0; // If no free register, return R0 
}

// Generate assembly code for an expression
void generate_code(char x, char y, char op, char z) {
    int reg, y_reg, z_reg;
    
    // Get register for result
    y_reg = get_reg(y);
    if (y_reg != -1) {
        reg = y_reg;
    } else {
        reg = get_free_reg();
        printf("MOV %c, R%d\n", y, reg);
    }
    
    // Perform operation
    z_reg = get_reg(z);
    if (op == '=') {
        // Simple assignment
        if (z_reg != -1) {
            printf("MOV R%d, R%d\n", z_reg, reg);
        } else {
            printf("MOV %c, R%d\n", z, reg);
        }
    } else {
        // Arithmetic operation
        char *instr;
        switch(op) {
            case '+': instr = "ADD"; break;
            case '-': instr = "SUB"; break;
            case '*': instr = "MUL"; break;
            case '/': instr = "DIV"; break;
            default:  instr = "???"; break;
        }
        
        if (z_reg != -1) {
            printf("%s R%d, R%d\n", instr, z_reg, reg);
        } else {
            printf("%s %c, R%d\n", instr, z, reg);
        }
    }
    
    // Update descriptors
    int x_idx = x - 'a';
    if (x_idx >= 0 && x_idx < MAX_VAR) {
        registers[reg][0] = x;
        registers[reg][1] = '\0';
        var_locations[x_idx].in_register = true;
        var_locations[x_idx].reg_num = reg;
        reg_free[reg] = false;
    }
}

int main() {
    // Initialize data structures
    for (int i = 0; i < MAX_REGISTERS; i++) {
        registers[i][0] = '\0';
        reg_free[i] = true;
    }
    
    for (int i = 0; i < MAX_VAR; i++) {
        var_locations[i].in_register = false;
        var_locations[i].reg_num = -1;
    }
    
    // Process expressions
    int n;
    printf("Enter number of expressions: ");
    scanf("%d", &n);
    getchar(); // Consume newline
    
    printf("\n=== Generated Assembly ===\n");
    for (int i = 0; i < n; i++) {
        char x, y, op, z;
        char expr[100];
        
        printf("Expression %d (e.g., 't=a+b'): ", i+1);
        fgets(expr, sizeof(expr), stdin);
        
        // Simple parser for expressions of form "x=y" or "x=y+z"
        sscanf(expr, " %c = %c", &x, &y);
        
        // Check for arithmetic operations
        char *ops = strchr(expr, '+');
        if (!ops) ops = strchr(expr, '-');
        if (!ops) ops = strchr(expr, '*');
        if (!ops) ops = strchr(expr, '/');
        
        if (ops) {
            op = *ops;
            sscanf(ops+1, " %c", &z);
        } else {
            op = '=';
            z = ' '; // Not used
        }
        
        generate_code(x, y, op, z);
    }
    
    return 0;
}