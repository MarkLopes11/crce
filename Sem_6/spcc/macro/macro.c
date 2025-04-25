#include <stdio.h>
#include <stdlib.h>
#include<string.h>

# define MAX_LINE_LENGTH 100
# define MAX_LINES 50

void definition_table(char definition[MAX_LINES][MAX_LINE_LENGTH], const char *filename)
{
    FILE *file = fopen(filename,"r"); 
    if (!file)
    {
        printf("Error opening file.\n");
    }

    char line[MAX_LINE_LENGTH];
    int index = 0; 

    printf("Index\t | Definition\n");
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && index < MAX_LINES) {
        line[strcspn(line, "\n")] = '\0';
        
        strcpy(definition[index], line);
        if(index>0)
            printf("%i\t | %s\n", index+1, definition[index]);
        index++;
    }
    fclose(file);
}



typedef struct {
    char name[MAX_LINE_LENGTH];
    int param_count;
    int start_index;
} MNTEntry;

typedef struct {
    char param_name[MAX_LINE_LENGTH];
    int index;
} ALAEntry;

void ala_table(char definition[MAX_LINES][MAX_LINE_LENGTH], int start_line)
{
    ALAEntry ala[MAX_LINES];
    int ala_index = 0;

    if(start_line + 1 >= MAX_LINES)
        return;

    char temp_line[MAX_LINE_LENGTH];
    strcpy(temp_line, definition[start_line + 1]);

    char *token = strtok(temp_line, " ,");
    // Skip macro name
    if(token != NULL)
        token = strtok(NULL, " ,");

    while(token != NULL)
    {
        strcpy(ala[ala_index].param_name, token);
        ala[ala_index].index = ala_index + 1;
        ala_index++;
        token = strtok(NULL, " ,");
    }

    // Print ALA table
    printf("ALA Table for macro starting at line %d:\n", start_line + 2);
    printf("Index\tParameter Name\n");
    for(int i = 0; i < ala_index; i++)
    {
        printf("%d\t%s\n", ala[i].index, ala[i].param_name);
    }
}

void mnt_table(char definition[MAX_LINES][MAX_LINE_LENGTH], int num_lines) 
{
    MNTEntry mnt[MAX_LINES];
    int macro_index = 0;

    for(int i = 0; i < num_lines; i++)
    {
        if (strstr(definition[i], "Macro")) 
        {
            // Extract macro name and parameters from the next line
            if(i + 1 < num_lines)
            {
                char temp_line[MAX_LINE_LENGTH];
                strcpy(temp_line, definition[i + 1]);
                char *token = strtok(temp_line, " ,");
                if(token != NULL)
                {
                    strcpy(mnt[macro_index].name, token);
                    // Count parameters
                    int param_count = 0;
                    while(token != NULL)
                    {
                        param_count++;
                        token = strtok(NULL, " ,");
                    }
                    mnt[macro_index].param_count = param_count - 1; // exclude macro name itself
                    mnt[macro_index].start_index = i;
                    macro_index++;
                }
            }
        }
    }

    // Print MNT table
    printf("MNT Table:\n");
    printf("Index\tName\tParam Count\tStart Line\n");
    for(int i = 0; i < macro_index; i++)
    {
        printf("%d\t%s\t%d\t\t%d\n", i+1, mnt[i].name, mnt[i].param_count, mnt[i].start_index+2);
        // Call ala_table for each macro
        ala_table(definition, mnt[i].start_index);
    }
}

int main()
{
    char definition[MAX_LINES][MAX_LINE_LENGTH];
    definition_table(definition, "input.txt");

    mnt_table(definition, MAX_LINES);

    return 0;
}
