#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_LINES 50
#define MAX_ARGS 10

typedef struct {
    int macro_no;
    char macro_name[50];
    int definition_index;
} MNTEntry;

typedef struct {
    int arg_index;
    char arg_name[50];
    char macro_name[50];
} ALAEntry;

void definition_table(char definition[MAX_LINES][MAX_LINE_LENGTH], const char *filename, int *num_lines)
{
    FILE *file = fopen(filename,"r"); 
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int index = 0; 

    printf("Index\t| Definition\n");
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && index < MAX_LINES) {
        line[strcspn(line, "\n")] = '\0';
        strcpy(definition[index], line);
        if (index > 0)
            printf("%d\t| %s\n", index + 1, definition[index]);
        index++;
    }

    *num_lines = index;
    fclose(file);
}

void parse_macro(const char *line, char *macro_name, char args[MAX_ARGS][50], int *arg_count) {
    *arg_count = 0;

    char temp[MAX_LINE_LENGTH];
    strcpy(temp, line);

    char *token = strtok(temp, " ,");  // Tokenize using space or comma

    // First token is label (optional), second is macro name
    if (token != NULL) {
        token = strtok(NULL, " ,");  // Get macro name (second token)
        if (token != NULL) {
            strcpy(macro_name, token);  // Macro name (INCR1)

            // Get arguments
            token = strtok(NULL, " ,");
            while (token != NULL && *arg_count < MAX_ARGS) {
                strcpy(args[(*arg_count)++], token);
                token = strtok(NULL, " ,");
            }
        }
    }
}


void build_mnt_ala(char definition[MAX_LINES][MAX_LINE_LENGTH], int num_lines)
{
    MNTEntry mnt[MAX_LINES];
    ALAEntry ala[MAX_LINES];
    int mnt_count = 0, ala_count = 0;

    for (int i = 0; i < num_lines; i++) {
        if (strcmp(definition[i], "Macro") == 0 && i + 1 < num_lines) {
            char macro_line[MAX_LINE_LENGTH];
            strcpy(macro_line, definition[i + 1]);

            char macro_name[50];
            char args[MAX_ARGS][50];
            int arg_count = 0;

            parse_macro(macro_line, macro_name, args, &arg_count);

            // Add to MNT
            mnt[mnt_count].macro_no = mnt_count + 1;
            strcpy(mnt[mnt_count].macro_name, macro_name);
            mnt[mnt_count].definition_index = i + 2;
            mnt_count++;

            // Add to ALA
            for (int j = 0; j < arg_count; j++) {
                ala[ala_count].arg_index = ala_count + 1;
                strcpy(ala[ala_count].arg_name, args[j]);
                strcpy(ala[ala_count].macro_name, macro_name);
                ala_count++;
            }
        }
    }

    printf("\n=== MNT Table ===\n");
    printf("No.\tMacro Name\tDefinition Index\n");
    for (int i = 0; i < mnt_count; i++) {
        printf("%d\t%s\t\t%d\n", mnt[i].macro_no, mnt[i].macro_name, mnt[i].definition_index);
    }

    printf("\n=== ALA Table ===\n");
    printf("Index\tArgument\tMacro Name\n");
    for (int i = 0; i < ala_count; i++) {
        printf("%d\t%s\t\t%s\n", ala[i].arg_index, ala[i].arg_name, ala[i].macro_name);
    }
}

int main()
{
    char definition[MAX_LINES][MAX_LINE_LENGTH];
    int num_lines = 0;

    definition_table(definition, "input.txt", &num_lines);
    build_mnt_ala(definition, num_lines);

    return 0;
}
