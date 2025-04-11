#include <stdio.h>
#include <string.h>

int current_register = 0; 

const char* map_operator(char op) {
    switch (op) {
        case '+': return "ADD";
        case '-': return "SUB";
        case '*': return "MUL";
        default: return "INVALID";
    }
}

void generate_code(char *x, char op, char *y) {
    const char *operation = map_operator(op);

    if (strcmp(operation, "INVALID") == 0) {
        printf("Invalid operator!\n");
        return;
    }

    printf("MOV %s, R%d\n", x, current_register);
    printf("%s %s, R%d\n", operation, y, current_register);

    current_register = (current_register + 1);
}

int main() {
    int num_expressions;
    printf("Enter number of expressions: ");
    scanf("%d", &num_expressions);

    for (int i = 0; i < num_expressions; i++) {
        char x[10], y[10], z[10];
        char op;

        printf("\nEnter statement %d in the form x + y = z:\n", i + 1);
        scanf("%s %c %s = %s", x, &op, y, z);

        generate_code(x, op, y);
    }

    return 0;
}
