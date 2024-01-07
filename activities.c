#include "headers.h"

void activites()
{
    int my_pid = getpid();

    struct dirent **namelist;
    int n;

    n = scandir("/proc", &namelist, NULL, alphasort);

    if (n == -1)
    {
        perror("\x1b[31mError scanning /proc directory\x1b[0m\n");
        return;
    }

    activity_pointer head = NULL;
    for (int i = 0; i < n; i++)
    {
        if (namelist[i]->d_type == DT_DIR && atoi(namelist[i]->d_name))
        {
            char filename[300];
            snprintf(filename, sizeof(filename), "/proc/%s/status", namelist[i]->d_name);

            FILE *statusfile = fopen(filename, "r");
            if (statusfile == NULL)
            {
                fprintf(stderr, "\x1b[31mError opening status file\x1b[0m\n");
                return;
            }
            char line[256];
            char name[256];
            char pid[256];
            char state[256];
            char ppid[256];

            while (fgets(line, sizeof(line), statusfile))
            {
                int flag = 0;
                if (strncmp(line, "Name:", 5) == 0)
                {
                    sscanf(line + 5, "%s", name);
                    flag = 1;
                    fgets(line, sizeof(line), statusfile); // umask
                    fgets(line, sizeof(line), statusfile);
                    if (strncmp(line, "State:", 6) == 0)
                    {
                        sscanf(line + 6, "%s", state);
                        sscanf(line + 9, "%s", state);
                    }
                    fgets(line, sizeof(line), statusfile); // tgid
                    fgets(line, sizeof(line), statusfile); // ngid
                    fgets(line, sizeof(line), statusfile);
                    if (strncmp(line, "Pid:", 4) == 0)
                        sscanf(line + 4, "%s", pid);
                    fgets(line, sizeof(line), statusfile);
                    if (strncmp(line, "PPid:", 5) == 0)
                        sscanf(line + 5, "%s", ppid);
                    if (atoi(ppid) == my_pid)
                    {
                        activity_pointer new = MakeANode(name, pid, state);
                        head = AddANode(new, head);
                    }
                }
            }
            fclose(statusfile);
        }
        free(namelist[i]);
    }
    free(namelist);

    head = sort_by_name(head);
    print_activities(head);
}

void print_activities(activity_pointer head)
{
    activity_pointer temp = head;
    while (temp != NULL)
    {
        printf("%s: %s - %s\n", temp->name, temp->pid, temp->state);
        temp = temp->next;
    }
}

activity_pointer MakeANode(char name[256], char pid[256], char state[256])
{
    activity_pointer new = (activity_pointer)malloc(sizeof(activity));
    strcpy(new->name, name);
    strcpy(new->pid, pid);
    strcpy(new->state, state);
    new->next = NULL;
    return new;
}

activity_pointer AddANode(activity_pointer new_node, activity_pointer head)
{
    if (head == NULL)
    {
        head = new_node;
        return head;
    }

    new_node->next = head->next;
    head->next = new_node;

    return head;
}

void DeleteAllNode(activity_pointer head)
{
    if (head == NULL)
        return;
    while (head != NULL)
    {
        activity_pointer temp = head->next;
        free(head);
        head = temp;
    }
}

activity_pointer sort_by_name(activity_pointer head)
{
    activity_pointer temp = head;
    while (temp != NULL)
    {
        activity_pointer temp2 = temp->next;
        while (temp2 != NULL)
        {
            if (strcmp(temp->name, temp2->name) > 0)
            {
                char temp_name[256];
                char temp_pid[256];
                char temp_state[256];
                strcpy(temp_name, temp->name);
                strcpy(temp_pid, temp->pid);
                strcpy(temp_state, temp->state);
                strcpy(temp->name, temp2->name);
                strcpy(temp->pid, temp2->pid);
                strcpy(temp->state, temp2->state);
                strcpy(temp2->name, temp_name);
                strcpy(temp2->pid, temp_pid);
                strcpy(temp2->state, temp_state);
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    return head;
}