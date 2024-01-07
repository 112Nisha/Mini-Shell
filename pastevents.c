#include "headers.h"

void pastevents_store(char command[1000][1000], char *directory)
{
    char dir[4096];
    strcpy(dir, directory);
    strcat(dir, "/pastevents.txt");

    int len = 0;
    for (int i = 0; i < 1000; i++)
    {
        if (strcmp(command[i], "NULL") == 0)
        {
            len = i;
            break;
        }
    }
    int i;
    for (i = 0; i < strlen(command[len - 1]); i++)
    {
        if (command[len - 1][i] == '\n')
            break;
    }
    command[len - 1][i] = '\n';
    command[len - 1][i + 1] = '\0';

    char command_1[4096] = {'\0'};
    for (int i = 0; i < 1000; i++)
    {
        if (strcmp(command[i], "NULL") == 0)
            break;
        strcat(command_1, command[i]);
        strcat(command_1, " ");
    }
    command_1[strlen(command_1) - 1] = '\0';

    FILE *store = fopen(dir, "r");
    if (store == NULL)
    {
        fprintf(stderr, "\x1b[31mError opening file\x1b[0m\n");
        return;
    }

    char lines[15][4096] = {'\0'};
    int lineCount = 0;
    while (fgets(lines[lineCount], 4096, store) != NULL && lineCount < 16)
        lineCount++;
    fclose(store);

    strcpy(dir, directory);
    strcat(dir, "/pastevents.txt");

    store = fopen(dir, "w");

    if (lineCount < 15)
    {
        for (int i = 0; i < lineCount; i++)
            fputs(lines[i], store);
        if (lineCount > 0 && (strcmp(lines[lineCount - 1], command_1) != 0))
            fputs(command_1, store);
        else if (lineCount == 0)
            fputs(command_1, store);
    }
    else
    {
        if (strcmp(lines[lineCount - 1], command_1) != 0)
        {
            for (int i = 1; i < lineCount; i++)
                fputs(lines[i], store);
            fputs(command_1, store);
        }
        else
        {
            for (int i = 0; i < lineCount; i++)
                fputs(lines[i], store);
        }
    }

    fclose(store);

    return;
}

void display_pastevents(char *directory)
{
    char dir[4096];
    strcpy(dir, directory);
    strcat(dir, "/pastevents.txt");

    FILE *store = fopen(dir, "r");
    if (store == NULL)
    {
        fprintf(stderr, "\x1b[31mError opening file\x1b[0m\n");
        return;
    }

    char one_line[4096];
    while (fgets(one_line, 4096, store) != NULL)
        printf("%s", one_line);
    fclose(store);
}

void delete_pastevents(char *directory)
{
    char dir[4096];
    strcpy(dir, directory);
    strcat(dir, "/pastevents.txt");

    FILE *store = fopen(dir, "w");
    if (store == NULL)
    {
        fprintf(stderr, "\x1b[31mError opening file\x1b[0m\n");
        return;
    }
    fclose(store);
}

void execute_pastevents(int index, char *directory, char prevdir[4096])
{
    char dir[4096];
    strcpy(dir, directory);
    strcat(dir, "/pastevents.txt");

    FILE *store = fopen(dir, "r");
    if (store == NULL)
    {
        fprintf(stderr, "\x1b[31mError opening file\x1b[0m\n");
        return;
    }

    char lines[15][4096] = {'\0'};
    int lineCount = 0;
    while (fgets(lines[lineCount], 4096, store) != NULL && lineCount < 16)
        lineCount++;
    fclose(store);

    find_input(lines[lineCount - index], directory, prevdir);
}