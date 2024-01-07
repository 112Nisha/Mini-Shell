#include "headers.h"

void find_input(char *string_taken, char *current, char prevdir[4096])
{
    char original_Command[4096];
    strcpy(original_Command, string_taken);

    char *command_1 = (char *)malloc(sizeof(char) * 1000);
    char *command_2 = (char *)malloc(sizeof(char) * 1000);

    char *sub_string = strtok_r(string_taken, ";", &string_taken);
    int count = 0;
    int num_pipe = 0;
    int redirection = 0;

    int num_and = 0;
    while (sub_string != NULL)
    {
        num_and = 0;
        strcpy(command_1, sub_string);
        for (int i = 0; i < strlen(command_1); i++)
        {
            if (command_1[i] == '&')
                num_and++;
        }

        process_pointer *p = (process_pointer *)malloc(sizeof(process_pointer) * (num_and + 1));
        for (int i = 0; i <= num_and; i++)
        {
            p[i] = (process_pointer)malloc(sizeof(process));
            strcpy(p[i]->commands[0], "NULL");
        }

        int processth = 0;
        int n = num_and;
        // To fix: make the code combine & and | together

        char *sub_sub_string = strtok_r(command_1, "&", &command_1);
        while (sub_sub_string != NULL)
        {
            for (int i = 0; i < strlen(sub_sub_string); i++)
            {
                if (sub_sub_string[i] == '|')
                    num_pipe++;
            }
            if (num_pipe > 0)
            {
                piping_process(sub_sub_string, current, prevdir, original_Command, p, n, processth, num_pipe);
            }
            else
            {
                char *token = (char *)malloc(sizeof(char) * 4096);
                strcpy(token, sub_sub_string);
                int redirection_count = 0;
                for (int i = 0; i < strlen(token); i++)
                {
                    if (token[i] == '<' || token[i] == '>')
                        redirection_count++;
                }

                redirection = redirection_count;

                if (redirection_count)
                {
                    // printf("redirection\n");
                    redirection_exec(sub_sub_string, current, prevdir, original_Command, p, n, processth);
                    break;
                }

                strcpy(command_2, sub_sub_string);
                num_and--;
                if (num_and < 0)
                {
                    // foreground
                    p[processth]->ground = 1;
                    int count = 0;
                    char *sub_space_string = strtok_r(command_2, " \t", &command_2);
                    while (sub_space_string != NULL)
                    {
                        strcpy(p[processth]->commands[count++], sub_space_string);
                        sub_space_string = strtok_r(NULL, " \t", &command_2);
                    }
                    strcpy(p[processth]->commands[count++], "NULL");
                    processth++;
                }
                else
                {
                    p[processth]->ground = 0;
                    int count = 0;
                    char *sub_space_string = strtok_r(command_2, " \t", &command_2);
                    while (sub_space_string != NULL)
                    {
                        strcpy(p[processth]->commands[count++], sub_space_string);
                        sub_space_string = strtok_r(NULL, " \t", &command_2);
                    }
                    strcpy(p[processth]->commands[count++], "NULL");
                    processth++;
                }
            }
            sub_sub_string = strtok_r(NULL, "&", &command_1);
        }
        if (num_pipe == 0 && redirection == 0)
        {
            for (int i = 0; i <= n; i++)
            {
                if (strcmp(p[i]->commands[0], "NULL") != 0)
                    execute_instr(p[i]->commands, current, prevdir, original_Command, p, n + 1, i);
            }
        }

        for (int i = 0; i < n; i++)
            free(p[i]);
        free(p);
        sub_string = strtok_r(NULL, ";", &string_taken);
    }

    return;
}

void execute_instr(char commands[1000][1000], char *current, char prevdir[4096], char *string_taken, process_pointer *p, int num_and_plus_one, int i)
{
    int exec_errors = 0;
    if (strcmp(commands[0], "warp") == 0 || strcmp(commands[0], "warp\n") == 0)
    {
        exec_errors = warp(commands, current, prevdir);
        pastevents_store(commands, current);
    }
    else if (strcmp(commands[0], "peek") == 0 || strcmp(commands[0], "peek\n") == 0)
    {
        exec_errors = peek(commands, current, prevdir);
        pastevents_store(commands, current);
    }
    else if (strcmp(commands[0], "pastevents\n") == 0)
        display_pastevents(current);
    else if (strcmp(commands[0], "pastevents") == 0 && strcmp(commands[1], "purge\n") == 0)
        delete_pastevents(current);
    else if (strcmp(commands[0], "pastevents") == 0 && strcmp(commands[1], "execute") == 0)
        execute_pastevents(atoi(commands[2]), current, prevdir);
    else if (strcmp(commands[0], "neonate") == 0 && strcmp(commands[1], "-n") == 0)
    {
        if (commands[2][strlen(commands[2]) - 1] == '\n')
            commands[2][strlen(commands[2]) - 1] = '\0';
        neonate(atoi(commands[2]));
        pastevents_store(commands, current);
    }
    else if (strcmp(commands[0], "fg") == 0)
    {
        if (commands[1][strlen(commands[1]) - 1] == '\n')
            commands[1][strlen(commands[1]) - 1] = '\0';
        change_to_fg(atoi(commands[1]));
        pastevents_store(commands, current);
    }
    else if (strcmp(commands[0], "bg") == 0)
    {
        if (commands[1][strlen(commands[1]) - 1] == '\n')
            commands[1][strlen(commands[1]) - 1] = '\0';
        change_to_bg(atoi(commands[1]));
        pastevents_store(commands, current);
    }
    else if (strcmp(commands[0], "proclore") == 0 || strcmp(commands[0], "proclore\n") == 0)
    {
        proclore(commands);
        pastevents_store(commands, current);
    }
    else if (strcmp(commands[0], "iMan") == 0)
    {
        iMan(commands);
        pastevents_store(commands, current);
    }
    else if (strcmp(commands[0], "activities") == 0 || strcmp(commands[0], "activities\n") == 0)
    {
        activites();
        pastevents_store(commands, current);
    }
    else if (strcmp(commands[0], "seek") == 0 || strcmp(commands[0], "seek\n") == 0)
    {
        exec_errors = seek(commands, current, prevdir);
        pastevents_store(commands, current);
    }
    else if (strcmp(commands[0], "ping") == 0)
    {
        if (commands[2][strlen(commands[2]) - 1] == '\n')
            commands[2][strlen(commands[2]) - 1] = '\0';
        signal_handler(atoi(commands[1]), atoi(commands[2]));
        pastevents_store(commands, current);
    }
    else if (strcmp(commands[0], "\n") == 0)
        return;
    else
    {
        int num_comm = 0;
        char **command_new = (char **)malloc(sizeof(char *) * 1000);
        for (int j = 0; j < 1000; j++)
        {
            num_comm++;
            command_new[j] = (char *)malloc(sizeof(char) * 1000);
            if (strcmp(commands[j], "NULL") == 0)
            {
                if (commands[j - 1][strlen(commands[j - 1]) - 1] == '\n')
                    commands[j - 1][strlen(commands[j - 1]) - 1] = '\0';
                strcpy(command_new[j - 1], commands[j - 1]);
                command_new[j] = NULL;
                break;
            }
            else
                strcpy(command_new[j], commands[j]);
        }

        time_t currentTime;
        currentTime = time(NULL);

        if (strcmp(p[i]->commands[0], "NULL") == 0 || p[i]->commands[0][0] == '\n')
            return;
        int id = fork();
        if (id == 0)
        {
            if (p[i]->ground == 0) // bg
                setpgid(id, 0);
            int var = execvp(command_new[0], command_new);
            if (var == -1)
            {
                fprintf(stderr, "\x1b[31m%s is an invalid command!\n\x1b[0m", p[i]->commands[0]);
                exit(EXIT_FAILURE);
                return;
            }
        }
        else if (id > 0)
        {
            if (p[i]->ground == 0) // bg
            {
                int status;
                waitpid(id, &status, WNOHANG);
                if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_FAILURE)
                {
                    p[i]->pid = id;
                    printf("(%d)\n", id);
                    add_node(MakeNode(*p[i]));
                }
            }
            else // fg
            {
                int status;
                waitpid(id, &status, 0);
                time_t finish_time;
                finish_time = time(NULL);

                int diff = finish_time - currentTime;

                if (diff > 2)
                {
                    strcpy(extra, p[i]->commands[0]);
                    strcat(extra, ": ");
                    char commandzz[4096];
                    sprintf(commandzz, "%d", diff);
                    strcat(extra, commandzz);
                    strcat(extra, "s");
                }
            }
        }

        for (int j = 0; j < num_comm; j++)
            free(command_new[j]);
        free(command_new);

        pastevents_store(commands, current);
    }
}

void delete_all()
{
    while (process_list != NULL)
    {
        int result = kill(process_list->pid, SIGKILL);

        if (result != 0)
            fprintf(stderr, "\x1b[31mError\x1b[0m\n");
        NodePtr temp = process_list;
        free(temp->command);
        free(temp);
        process_list = process_list->next;
    }
}

NodePtr MakeNode(process p)
{
    NodePtr new = (NodePtr)malloc(sizeof(Node));
    new->command = (char *)malloc(sizeof(char) * 4096);
    new->pid = p.pid;
    char *temp;
    strcpy(new->command, p.commands[0]);
    new->next = NULL;

    return new;
}

void add_node(NodePtr new_node)
{
    if (process_list == NULL)
    {
        process_list = new_node;
        return;
    }

    new_node->next = process_list->next;
    process_list->next = new_node;

    return;
}

char *reconstruct(char *final, char command[1000][1000])
{
    char *final_command = (char *)malloc(1000 * sizeof(char));
    memset(final_command, '\0', 1000);

    for (int i = 0; i < 1000; i++)
    {
        if (strcmp(command[i], "NULL") != 0)
        {
            strcat(final_command, command[i]);
            strcat(final_command, " ");
        }
        else
            break;
    }
    return final_command;
}

void remove_node()
{
    NodePtr traveller = process_list;
    if (traveller == NULL)
        return;
    NodePtr prev = NULL;
    while (traveller != NULL)
    {
        int status;
        pid_t result = waitpid(traveller->pid, &status, WNOHANG); // fix
        if (result == -1)
        {
            fprintf(stderr, "\x1b[31mError with command %s\x1b[0m\n", traveller->command);
            if (prev != NULL)
                prev->next = traveller->next;
            else
                process_list = traveller->next;
            NodePtr temp = traveller;
            traveller = traveller->next;
            free(temp);
        }
        else if (result == 0) // running
        {
            prev = traveller;
            traveller = traveller->next;
        }
        else
        {
            if (WIFEXITED(status))
            {
                fprintf(stdout, "%s exited normally (%d)\n", traveller->command, traveller->pid); // must for all bg processes
                NodePtr temp = traveller->next;
                if (prev != NULL)
                    prev->next = traveller->next;
                else
                    process_list = traveller->next;
                free(traveller->command);
                free(traveller);
                traveller = temp;
            }
            else if (WIFSIGNALED(status))
            {
                fprintf(stdout, "%s exited abnormally (%d)\n", traveller->command, traveller->pid); // must for all bg processes
                NodePtr temp = traveller->next;
                if (prev != NULL)
                    prev->next = traveller->next;
                else
                    process_list = traveller->next;
                free(traveller->command);
                free(traveller);
                traveller = temp;
            }
        }
    }
}

void print_list(NodePtr List)
{
    NodePtr traveller = List;
    while (traveller != NULL)
    {
        printf("printing: %d\n", traveller->pid);
        traveller = traveller->next;
    }
}