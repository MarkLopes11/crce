#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int tempCount = 1; // Counter for temporary variables
char tac[50][30]; // Array to store TAC instructions
int tacCount = 0; // Counter for TAC instructions

// Function to generate a new temporary variable name
char* generateTempVar() {
    char tempVarName[10];
    snprintf(tempVarName, sizeof(tempVarName), "t%d", tempCount++);
    char *tempVar = strdup(tempVarName); // Allocate memory for the string
    return tempVar;
}

// Function to check if a string is a number
int isNumber(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    char *endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

// Function to generate TAC recursively
char* generateTAC_recursive(char *expr) {
    int plusMinusIndex = -1;
    int mulDivIndex = -1;
    int parenLevel = 0;
    int lastPlusMinusIndex = -1;
    int lastMulDivIndex = -1;

    // Find the last '+' or '-' at the lowest precedence level (outside parentheses)
    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == '(') {
            parenLevel++;
        } else if (expr[i] == ')') {
            parenLevel--;
        } else if (parenLevel == 0) {
            if (expr[i] == '+' || expr[i] == '-') {
                lastPlusMinusIndex = i;
            }
        }
    }
    if (lastPlusMinusIndex != -1) {
        plusMinusIndex = lastPlusMinusIndex;
    } else {
        // If no '+' or '-', find the last '*' or '/' at the next precedence level (outside parentheses)
        parenLevel = 0;
        for (int i = 0; expr[i] != '\0'; i++) {
            if (expr[i] == '(') {
                parenLevel++;
            } else if (expr[i] == ')') {
                parenLevel--;
            } else if (parenLevel == 0) {
                if (expr[i] == '*' || expr[i] == '/') {
                    lastMulDivIndex = i;
                }
            }
        }
        if (lastMulDivIndex != -1) {
            mulDivIndex = lastMulDivIndex;
        }
    }

    if (plusMinusIndex != -1) {
        char op = expr[plusMinusIndex];
        char leftExpr[50];
        char rightExpr[50];

        strncpy(leftExpr, expr, plusMinusIndex);
        leftExpr[plusMinusIndex] = '\0';
        strcpy(rightExpr, expr + plusMinusIndex + 1);

        char *leftResult = generateTAC_recursive(leftExpr);
        char *rightResult = generateTAC_recursive(rightExpr);
        char *tempVar = generateTempVar();

        snprintf(tac[tacCount++], sizeof(tac[0]), "%s = %s %c %s", tempVar, leftResult, op, rightResult);
        free(leftResult);
        free(rightResult);
        return tempVar;
    } else if (mulDivIndex != -1) {
        char op = expr[mulDivIndex];
        char leftExpr[50];
        char rightExpr[50];

        strncpy(leftExpr, expr, mulDivIndex);
        leftExpr[mulDivIndex] = '\0';
        strcpy(rightExpr, expr + mulDivIndex + 1);

        char *leftResult = generateTAC_recursive(leftExpr);
        char *rightResult = generateTAC_recursive(rightExpr);
        char *tempVar = generateTempVar();

        snprintf(tac[tacCount++], sizeof(tac[0]), "%s = %s %c %s", tempVar, leftResult, op, rightResult);
        free(leftResult);
        free(rightResult);
        return tempVar;
    } else {
        // Base case: No operators found (single variable or number)
        char *operand = strdup(expr);
        return operand;
    }
}

void generateTAC(char *expr) {
    tacCount = 0; // Reset TAC counter for each new expression
    tempCount = 1; // Reset temp variable counter
    char *resultVar = generateTAC_recursive(expr);

    printf("Three Address Code:\n");
    for (int i = 0; i < tacCount; i++) {
        printf("%s\n", tac[i]);
    }
    if (tacCount > 0) {
        printf("%s\n", resultVar); // Print the final result variable if needed.
    } else {
        printf("%s\n", resultVar); // For single variable input.
    }
    free(resultVar);
}

int main() {
    char expr[50];
    printf("Enter an arithmetic expression: ");
    scanf("%s", expr);
    generateTAC(expr);
    return 0;
}