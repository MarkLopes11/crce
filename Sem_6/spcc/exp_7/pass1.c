#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define MAX 100

typedef struct{
    char label[20];
    int address;
} Symbol;

Symbol symbolTable[MAX];
int symbolCount = 0;

typedef struct{
    char opcode[10];
    char machineCode[10];
} Opcode;

Opcode opcodeTable[MAX];
int opcodeCount = 0;
int locationCounter = 0;

int searchSymbol(char *label)
{
    for (int i = 0; i < symbolCount; i++)
    {
        if (strcmp(symbolTable[i].label, label) == 0){
            return i; // found
        }
    }
    return -1; // not found
}

void insertSymbol(char *label, int address)
{
    if (searchSymbol(label) != -1)
    {
        printf("Duplicate Symbol: %s \n", label);
        return;
    }
    strcpy(symbolTable[symbolCount].label, label);
    symbolTable[symbolCount].address = address;
    symbolCount++;
}

void insertOpcode(char *opcode, char *machineCode)
{
    strcpy(opcodeTable[opcodeCount].opcode, opcode);
    strcpy(opcodeTable[opcodeCount].machineCode, machineCode);
    opcodeCount++;
}

void createOpcodeTable()
{
    insertOpcode("L", "58");
    insertOpcode("A", "5A");
    insertOpcode("ST", "50");
}

void displaySymbolTable()
{
    printf("\nSymbol Table\n");
    printf("Label      Address\n");
    for(int i  = 0; i < symbolCount; i++)
    {
        printf("%s      %d\n", symbolTable[i].label, symbolTable[i].address);
    }
}

void displayOpcodeTable()
{
    printf("\nOpcode Table\n");
    printf("Opcode      Machine Code\n");
    for(int i = 0; i < opcodeCount; i++)
    {
        printf("%s      %s\n", opcodeTable[i].opcode, opcodeTable[i].machineCode);
    }
}

void passOne()
{
    FILE *file = fopen("input.txt", "r");
    if (!file)
    {
        printf("Error opening file.\n");
        return;
    }
    
    char line[MAX], label[20], opcode[10], operand[20];
    int startSet = 0;
    
    while (fgets(line, MAX, file))
    {
        int count = sscanf(line, "%s %s %s", label, opcode, operand);
        
        if (count == 2) { // No label
            strcpy(operand, opcode);
            strcpy(opcode, label);
            label[0] = '\0';
        }
        
        if (strcmp(opcode, "START") == 0)
        {
            locationCounter = atoi(operand);
            startSet = 1;
            continue;
        }
        
        if (!startSet) {
            printf("Error: START directive missing.\n");
            return;
        }
        
        if (label[0] != '\0') {
            insertSymbol(label, locationCounter);
        }
        
        if (strcmp(opcode, "L") == 0 || strcmp(opcode, "A") == 0 || strcmp(opcode, "ST") == 0)
        {
            locationCounter += 4; // Each instruction is 4 bytes
        }
        else if (strcmp(opcode, "DC") == 0)
        {
            


       
    }
    fclose(file);
}

int main()
{
    int choice;
    createOpcodeTable();
    while(1) {
        printf("Enter your choice: \n1. Pass One\n2. Display Symbol Table\n3. Display Opcode Table\n4. Exit\n");
        scanf("%d", &choice);
        
        while(getchar() != '\n');

        switch (choice)
        {
        case 1:
            passOne();
            break;
        case 2:
            displaySymbolTable();
            break;
        case 3:
            displayOpcodeTable();
            break;
        case 4:
            printf("Exiting program.....");
            return 0;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
            break;
        }
    }
}
