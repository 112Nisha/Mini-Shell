#include "headers.h"

void redirection_exec(char *command, char *current, char *prevdir, char *original_Command, process_pointer *p, int n, int processth)
{
    // must write into the input file or smth
    // make flag 1 so that no further processing??
    char *temp = (char *)malloc(sizeof(char) * 4096);
    strcpy(temp, command);
    char *exec_command = (char *)malloc(sizeof(char) * 4096);

    int input_flag = 0;
    int output_rewrite_flag = 0;
    int output_append_flag = 0;

    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] == '<')
            input_flag = 1;
        else if (command[i] == '>')
        {
            if (command[++i] == '>')
                output_append_flag = 1;
            else
                output_rewrite_flag = 1;
        }
    }

    // I have found if I need to give input, output or both
    // (input == 1, output == 1); (input == 1, append == 1); (output == 1); (append == 1); (input == 1)

    int index = 0;
    if (input_flag == 1 && output_rewrite_flag == 1) // cat < a.txt > b.txt
    {
        char *command_copy = (char *)malloc(sizeof(char) * 4096);
        char input_file[4096];
        char output_file[4096];

        int i = 0;
        for (i = 0; i < strlen(command); i++)
        {
            if (command[i] == '<')
                break;
            if (command[i] == ' ')
                continue;
            command_copy[i] = command[i];
        }
        command_copy[i] = '\0';

        int index = 0;
        for (int j = i; j < strlen(command); j++)
        {
            if (command[j] == '>')
            {
                i = j;
                break;
            }
            if (command[j] == ' ')
                continue;
            if (command[j] == '<')
                continue;
            input_file[index++] = command[j];
        }
        input_file[index + 1] = '\0';

        index = 0;
        for (int j = i; j < strlen(command); j++)
        {
            if (command[j] == ' ')
                continue;
            if (command[j] == '>')
                continue;
            output_file[index++] = command[j];
        }
        if (output_file[index - 1] == '\n')
            output_file[index - 1] = '\0';
        else
            output_file[i] = '\0';

        index = 0;
        char *token = strtok_r(command_copy, " ", &command_copy);
        while (token != NULL)
        {
            strcpy(p[processth]->commands[index++], token);
            token = strtok_r(NULL, " ", &command_copy);
        }
        strcpy(p[processth]->commands[index++], "NULL");

        int input_fd = open(input_file, O_RDONLY);
        int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (input_fd == -1)
        {
            fprintf(stderr, "\x1b[31mError: Unable to open file; File does not exist\x1b[0m\n");
            return;
        }
        if (output_fd == -1)
        {
            fprintf(stderr, "\x1b[31mError: Unable to open file\x1b[0m\n");
            return;
        }

        int original_in_file_desc = dup(STDIN_FILENO);
        int original_out_file = dup(STDOUT_FILENO);
        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);

        index = 0;
        while (strcmp(p[processth]->commands[index], "NULL") != 0)
            index++;

        p[processth]->ground = 1;

        execute_instr(p[processth]->commands, current, prevdir, original_Command, p, n, processth);

        dup2(original_in_file_desc, STDIN_FILENO);
        dup2(original_out_file, STDOUT_FILENO);

        close(output_fd);
        close(input_fd);
    }
    else if (input_flag == 1 && output_append_flag == 1)
    {
        char *command_copy = (char *)malloc(sizeof(char) * 4096);
        char input_file[4096];
        char output_file[4096];

        int i = 0;
        for (i = 0; i < strlen(command); i++)
        {
            if (command[i] == '<')
                break;
            if (command[i] == ' ')
                continue;
            command_copy[i] = command[i];
        }
        command_copy[i] = '\0';

        int index = 0;
        for (int j = i; j < strlen(command); j++)
        {
            if (command[j] == '>')
            {
                i = j;
                break;
            }
            if (command[j] == ' ')
                continue;
            if (command[j] == '<')
                continue;
            input_file[index++] = command[j];
        }
        input_file[index + 1] = '\0';

        index = 0;
        for (int j = i; j < strlen(command); j++)
        {
            if (command[j] == ' ')
                continue;
            if (command[j] == '>')
                continue;
            output_file[index++] = command[j];
        }
        if (output_file[index - 1] == '\n')
            output_file[index - 1] = '\0';
        else
            output_file[i] = '\0';

        index = 0;
        char *token = strtok_r(command_copy, " ", &command_copy);
        while (token != NULL)
        {
            strcpy(p[processth]->commands[index++], token);
            token = strtok_r(NULL, " ", &command_copy);
        }
        strcpy(p[processth]->commands[index++], "NULL");

        int input_fd = open(input_file, O_RDONLY);
        int output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);

        if (input_fd == -1)
        {
            fprintf(stderr, "\x1b[31mError: Unable to open file; File does not exist\x1b[0m\n");
            return;
        }
        if (output_fd == -1)
        {
            fprintf(stderr, "\x1b[31mError: Unable to open file\x1b[0m\n");
            return;
        }

        int original_in_file_desc = dup(STDIN_FILENO);
        int original_out_file = dup(STDOUT_FILENO);
        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);

        index = 0;
        while (strcmp(p[processth]->commands[index], "NULL") != 0)
            index++;

        p[processth]->ground = 1;

        execute_instr(p[processth]->commands, current, prevdir, original_Command, p, n, processth);

        dup2(original_in_file_desc, STDIN_FILENO);
        dup2(original_out_file, STDOUT_FILENO);

        close(output_fd);
        close(input_fd);
    }
    else if (input_flag == 1) // cat < a.txt
    {
        char *token = strtok_r(temp, "<", &temp);
        strcpy(exec_command, token);
        char *sub_token;
        while (token != NULL)
        {
            sub_token = strtok_r(exec_command, " ", &exec_command);
            while (sub_token != NULL)
            {
                strcpy(p[processth]->commands[index++], sub_token);
                sub_token = strtok_r(NULL, " ", &exec_command);
            }
            sub_token = strtok_r(NULL, "<", &temp);
            break;
        }
        char *file_name = strtok_r(sub_token, " ", &sub_token);

        char file_name2[4096];
        strcpy(file_name2, file_name);
        if (file_name2[strlen(file_name2) - 1] == '\n')
            file_name2[strlen(file_name2) - 1] = '\0';

        int input_fd = open(file_name2, O_RDONLY);
        if (input_fd == -1)
        {
            fprintf(stderr, "\x1b[31mError: !!   Unable to open file\x1b[0m\n");
            return;
        }

        strcpy(p[processth]->commands[index], "NULL");
        int original = dup(STDIN_FILENO);
        dup2(input_fd, STDIN_FILENO);

        p[processth]->ground = 1;
        int output_fd;
        int original_out_file;
        execute_instr(p[processth]->commands, current, prevdir, original_Command, p, n, processth);
        dup2(original, STDIN_FILENO);
        close(input_fd);
    }
    else if (output_append_flag == 1) // cat < a.txt | cat >> b.txt
    {
        int original;
        int input_fd;
        char *token = strtok_r(temp, ">", &temp);
        strcpy(exec_command, token);
        char *sub_token;
        while (token != NULL)
        {
            sub_token = strtok_r(exec_command, " ", &exec_command);
            while (sub_token != NULL)
            {
                strcpy(p[processth]->commands[index++], sub_token);
                sub_token = strtok_r(NULL, " ", &exec_command);
            }
            strcpy(p[processth]->commands[index], "NULL");
            sub_token = strtok_r(NULL, ">", &temp);
            break;
        }
        if (strcmp(sub_token, "") == 0)
        {
            sub_token = strtok_r(NULL, ">", &temp);
        }

        char *file_name = strtok_r(sub_token, " ", &sub_token);

        char file_name2[4096];
        strcpy(file_name2, file_name);
        if (file_name2[strlen(file_name2) - 1] == '\n')
            file_name2[strlen(file_name2) - 1] = '\0';

        int output_fd = open(file_name2, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (output_fd == -1)
        {
            fprintf(stderr, "\x1b[31mError: Unable to open file\x1b[0m\n");
            return;
        }
        int original_out_file = dup(1);
        dup2(output_fd, STDOUT_FILENO);

        p[processth]->ground = 1;
        execute_instr(p[processth]->commands, current, prevdir, original_Command, p, n, processth);

        dup2(original_out_file, 1);
        close(output_fd);
    }
    else if (output_rewrite_flag == 1) // cat < a.txt | cat > b.txt
    {
        int original;
        int input_fd;
        char *token = strtok_r(temp, ">", &temp);
        strcpy(exec_command, token);
        char *sub_token;
        while (token != NULL)
        {
            sub_token = strtok_r(exec_command, " ", &exec_command);
            while (sub_token != NULL)
            {
                strcpy(p[processth]->commands[index++], sub_token);
                sub_token = strtok_r(NULL, " ", &exec_command);
            }
            strcpy(p[processth]->commands[index], "NULL");
            sub_token = strtok_r(NULL, ">", &temp);
            break;
        }
        char *file_name = strtok_r(sub_token, " ", &sub_token);

        char file_name2[4096];
        strcpy(file_name2, file_name);
        if (file_name2[strlen(file_name2) - 1] == '\n')
            file_name2[strlen(file_name2) - 1] = '\0';

        int output_fd = open(file_name2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd == -1)
        {
            fprintf(stderr, "\x1b[31mError: Unable to open file\x1b[0m\n");
            return;
        }
        int original_out_file = dup(1);
        dup2(output_fd, STDOUT_FILENO);

        p[processth]->ground = 1;
        execute_instr(p[processth]->commands, current, prevdir, original_Command, p, n, processth);

        dup2(original_out_file, 1);
        close(output_fd);
    }
}