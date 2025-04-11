#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tempCount = 1; // Counter for temporary variables

void generateTAC(char *expr) {
    char op, left[10], right[10];
    int i, j = 0, k = 0;
    char tempVar[10];
    char tac[10][30];
    int tacCount = 0;

    for (i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            op = expr[i];
            strncpy(left, expr, i);
            left[i] = '\0';
            strcpy(right, expr + i + 1);
            break;
        }
    }
    
    snprintf(tempVar, sizeof(tempVar), "t%d", tempCount++);
    snprintf(tac[tacCount++], sizeof(tac[0]), "%s = %s %c %s", tempVar, left, op, right);
    
    printf("Three Address Code:\n");
    for (i = 0; i < tacCount; i++) {
        printf("%s\n", tac[i]);
    }
}

int main() {
    char expr[50];
    printf("Enter an arithmetic expression (e.g., a+b, x*y, etc.): ");
    scanf("%s", expr);
    generateTAC(expr);
    return 0;
}
