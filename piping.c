#include "headers.h"

void piping_process(char *sub_sub_string, char *current, char *prevdir, char *original_Command, process_pointer *p, int n, int processth, int pipe_count)
{
    p[processth]->ground = 1;
    int index = 0;
    int command_index = 0;
    int command_count = 0;

    char **commands = (char **)malloc(1000 * sizeof(char *));
    for (int i = 0; i < 1000; i++)
    {
        commands[i] = (char *)malloc(1000 * sizeof(char));
        strcpy(commands[i], "NULL");
    }
    char *temp = (char *)malloc(4096 * sizeof(char));

    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);

    int num_pipes = 0; // handle incorrect use
    for (int i = 0; i < strlen(sub_sub_string); i++)
    {
        if (sub_sub_string[i] == '|')
            num_pipes++;
    }


    int i = 0;
    char *token = strtok_r(sub_sub_string, "|", &sub_sub_string);
    while (token != NULL)
    {
        strcpy(commands[i++], token);
        token = strtok_r(NULL, "|", &sub_sub_string);
    }
    int command_no = 0;

    int inp[2];
    int outp[2];

    for (int i = 0; i <= num_pipes; i++)
    {
        int worked = pipe(inp);

        dup2(outp[0], STDIN_FILENO);

        if (num_pipes != i)
        {
            dup2(inp[1], STDOUT_FILENO);
            close(inp[1]);
        }

        if (i == num_pipes)
            dup2(original_stdout, STDOUT_FILENO);

        find_input(commands[i], current, prevdir);

        if(i != num_pipes)
            close(outp[0]);

        outp[0] = inp[0];
    }

    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
}