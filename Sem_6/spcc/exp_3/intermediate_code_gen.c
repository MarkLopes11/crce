#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100

typedef enum { ID, NUM, PLUS, MINUS, MULT, DIV, ASSIGN, LBRACKET, RBRACKET, LPAREN, RPAREN, END, ERROR } TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_LEN];
} Token;

typedef struct {
    char op[10], arg1[MAX_LEN], arg2[MAX_LEN], result[MAX_LEN];
} Quadruple;

typedef struct {
    char op[10], arg1[MAX_LEN], arg2[MAX_LEN];
} Triple;

char *input;
int pos = 0, tempVar = 0, quadIdx = 0, tripleIdx = 0;
Token token;
Quadruple quads[MAX_LEN];
Triple triples[MAX_LEN];

char* create_temp() {
    char* temp = (char*)malloc(10);
    sprintf(temp, "t%d", tempVar++);
    return temp;
}

void fetch_token() {
    while (input[pos] && isspace(input[pos])) pos++;
    if (!input[pos]) { token.type = END; strcpy(token.lexeme, "EOF"); return; }
    
    if (isalpha(input[pos])) {
        int i = 0;
        while (isalnum(input[pos])) token.lexeme[i++] = input[pos++];
        token.lexeme[i] = '\0';
        token.type = ID;
    } else if (isdigit(input[pos])) {
        int i = 0;
        while (isdigit(input[pos])) token.lexeme[i++] = input[pos++];
        token.lexeme[i] = '\0';
        token.type = NUM;
    } else {
        switch (input[pos++]) {
            case '+': token.type = PLUS; strcpy(token.lexeme, "+"); break;
            case '-': token.type = MINUS; strcpy(token.lexeme, "-"); break;
            case '*': token.type = MULT; strcpy(token.lexeme, "*"); break;
            case '/': token.type = DIV; strcpy(token.lexeme, "/"); break;
            case '=': token.type = ASSIGN; strcpy(token.lexeme, "="); break;
            case '[': token.type = LBRACKET; strcpy(token.lexeme, "["); break;
            case ']': token.type = RBRACKET; strcpy(token.lexeme, "]"); break;
            case '(': token.type = LPAREN; strcpy(token.lexeme, "("); break;
            case ')': token.type = RPAREN; strcpy(token.lexeme, ")"); break;
            default: token.type = ERROR; token.lexeme[0] = input[pos - 1]; token.lexeme[1] = '\0';
        }
    }
}

void generate_quad(char* op, char* arg1, char* arg2, char* result) {
    strcpy(quads[quadIdx].op, op);
    strcpy(quads[quadIdx].arg1, arg1);
    strcpy(quads[quadIdx].arg2, arg2);
    strcpy(quads[quadIdx].result, result);
    quadIdx++;
}

void generate_triple(char* op, char* arg1, char* arg2) {
    strcpy(triples[tripleIdx].op, op);
    strcpy(triples[tripleIdx].arg1, arg1);
    strcpy(triples[tripleIdx].arg2, arg2);
    tripleIdx++;
}

char* parse_expression();

char* parse_factor() {
    char *res;
    if (token.type == ID) {
        res = strdup(token.lexeme);
        fetch_token();
        if (token.type == LBRACKET) {
            fetch_token();
            char* index = parse_expression();
            if (token.type != RBRACKET) { printf("Syntax Error: Expected ']'\n"); exit(1); }
            fetch_token();
            char* temp = create_temp();
            generate_quad("[]", res, index, temp);
            generate_triple("[]", res, index);
            free(res);
            free(index);
            return temp;
        }
        return res;
    } else if (token.type == NUM) {
        res = strdup(token.lexeme);
        fetch_token();
        return res;
    } else if (token.type == LPAREN) {
        fetch_token();
        res = parse_expression();
        if (token.type != RPAREN) { printf("Syntax Error: Expected ')'\n"); exit(1); }
        fetch_token();
        return res;
    }
    printf("Syntax Error: Unexpected token %s\n", token.lexeme);
    exit(1);
}

char* parse_term() {
    char* left = parse_factor();
    while (token.type == MULT || token.type == DIV) {
        TokenType op = token.type;
        fetch_token();
        char* right = parse_factor();
        char* temp = create_temp();
        generate_quad(op == MULT ? "*" : "/", left, right, temp);
        generate_triple(op == MULT ? "*" : "/", left, right);
        free(left);
        free(right);
        left = temp;
    }
    return left;
}

char* parse_expression() {
    char* left = parse_term();
    while (token.type == PLUS || token.type == MINUS) {
        TokenType op = token.type;
        fetch_token();
        char* right = parse_term();
        char* temp = create_temp();
        generate_quad(op == PLUS ? "+" : "-", left, right, temp);
        generate_triple(op == PLUS ? "+" : "-", left, right);
        free(left);
        free(right);
        left = temp;
    }
    return left;
}

int main() {
    char buffer[MAX_LEN];
    printf("Enter an arithmetic expression: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    input = buffer;
    pos = tempVar = quadIdx = tripleIdx = 0;
    
    fetch_token();
    char* left = parse_factor();
    if (token.type != ASSIGN) { printf("Syntax Error: Expected '='\n"); exit(1); }
    fetch_token();
    char* right = parse_expression();
    generate_quad("=", right, "", left);
    generate_triple("=", right, "");
    free(left);
    free(right);
    
    printf("\nThree-Address Code:\n");
    for (int i = 0; i < quadIdx; i++) {
        printf("%s = %s %s %s\n", quads[i].result, quads[i].arg1, quads[i].op, quads[i].arg2);
    }

    printf("\nQuadruples:\n");
    printf("| Index | Op  | Arg1  | Arg2  | Result |\n");
    printf("|-------|-----|-------|-------|--------|\n");
    for (int i = 0; i < quadIdx; i++) {
        printf("| %5d | %-3s | %-5s | %-5s | %-6s |\n", i, quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
    }

    printf("\nTriples:\n");
    printf("| Index | Op  | Arg1  | Arg2  |\n");
    printf("|-------|-----|-------|-------|\n");
    for (int i = 0; i < tripleIdx; i++) {
        printf("| %5d | %-3s | %-5s | %-5s |\n", i, triples[i].op, triples[i].arg1, triples[i].arg2);
    }

    return 0;
}
