#include "headers.h"

void change_to_fg(int pid)
{
    time_t currentTime;
    currentTime = time(NULL);

    remove_node_with_pid(pid);

    int status;
    waitpid(pid, &status, 0);
    time_t finish_time;
    finish_time = time(NULL);

    int diff = finish_time - currentTime;

    if (diff > 2)
    {
        strcpy(extra, "fg ");
        char commandzzz[4096];
        snprintf(commandzzz, sizeof(commandzzz), "%d", pid);
        strcat(extra, commandzzz);
        strcat(extra, ": ");
        sprintf(commandzzz, "%d", diff);
        strcat(extra, commandzzz);
        strcat(extra, "s");
    }
}

void remove_node_with_pid(int pid)
{
    NodePtr traveller = process_list;
    NodePtr prev = NULL;
    while (traveller != NULL)
    {
        if (traveller->pid == pid)
            break;
        prev = traveller;
        traveller = traveller->next;
    }

    NodePtr temp = traveller;
    if (traveller == NULL)
    {
        fprintf(stderr, "\x1b[31mNo such process exists\x1b[0m\n");
        return;
    }
    if (prev == NULL)
        process_list = traveller->next;
    else
        prev->next = traveller->next;
    free(temp);
}

void change_to_bg(int pid)
{
    if (kill(pid, SIGCONT) == 0)
        printf("Process with PID %d has been resumed\n", pid);
    else
        printf("Error resuming process with PID %d\n", pid);
}