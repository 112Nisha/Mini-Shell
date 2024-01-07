#include "headers.h"

int proclore(char commands[1000][1000])
{
    int error = 0;
    pid_t pid;
    if (strcmp(commands[1], "NULL") == 0)
        pid = getpid();
    else
        pid = atoi(commands[1]);

    printf("pid : %d\n", pid);

    error += print_state(pid);
    if (error == 1)
        return 1;
    error += print_pgid(pid);
    if (error == 1)
        return 1;
    error += Virtual_Memory(pid);
    if (error == 1)
        return 1;

    char path[1024];
    snprintf(path, sizeof(path), "/proc/%d/exe", pid);

    char buffer[1024];
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len != -1)
    {
        buffer[len] = '\0';
        printf("Executable path : %s\n", buffer);
    }
    else
    {
        fprintf(stderr, "\x1b[31mError reading process path\x1b[0m\n");
        return 1;
    }

    return error;
}

int Virtual_Memory(int pid)
{
    char status_path[50];
    snprintf(status_path, sizeof(status_path), STATUS_FILE_PATH, pid);

    FILE *status_file = fopen(status_path, "r");
    if (status_file == NULL)
    {
        fprintf(stderr, "\x1b[31mError opening status file\x1b[0m\n");
        return 1;
    }

    int i = 0;
    char *vm = (char *)malloc(sizeof(char) * 256);
    char word[256] = {'\0'};
    while (fgets(word, sizeof(word), status_file))
    {
        i++;
        if (i == 18) // on line 18 vm
        {
            strcpy(vm, word);
            break;
        }
    }
    i = 0;
    char *temp = strtok_r(vm, " \t", &vm);
    while (i < 2)
    {
        strcpy(word, temp);
        temp = strtok_r(NULL, " \t", &vm);
        i++;
    }
    printf("Virtual memory: %s\n", word);
    fclose(status_file);
}

int print_pgid(int pid)
{
    char stat_path[50];
    snprintf(stat_path, sizeof(stat_path), STAT_FILE_PATH, pid);

    FILE *stat_file = fopen(stat_path, "r");
    if (stat_file == NULL)
    {
        fprintf(stderr, "\x1b[31mError opening stat file\x1b[0m\n");
        return 1;
    }
    int i = 0;
    char pgrp_read[256];
    char word[256] = {'\0'};
    while (fscanf(stat_file, "%s", word) != EOF) // on line 5 pgid
    {
        i++;
        if (i == 4)
        {
            strcpy(pgrp_read, word);
            break;
        }
    }

    printf("Process Group : %s\n", pgrp_read);

    return 0;
}

int print_state(int pid)
{
    char status_path[50];
    snprintf(status_path, sizeof(status_path), STATUS_FILE_PATH, pid);

    pid_t pgid = tcgetpgrp(STDIN_FILENO);
    FILE *status_file = fopen(status_path, "r");
    if (status_file == NULL)
    {
        fprintf(stderr, "\x1b[31mError opening status file or invalid pid\x1b[0m\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), status_file) != NULL)
    {
        if (strncmp(line, "State:", 6) == 0)
        {
            char temp[100];
            strcpy(temp, line + 7);
            if (pid == pgid)
                printf("process Status : %c+\n", temp[0]);
            else
                printf("process Status : %c\n", temp[0]);
            break;
        }
    }
    fclose(status_file);

    return 0;
}