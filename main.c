#include "headers.h"

NodePtr process_list = NULL;
char current[4096];
pid_t pid_of_shell;
char previous_dir[4096];

int main()
{
    getcwd(current, sizeof(current));
    char prevdir[4096];
    prevdir[0] = '\0';
    pid_of_shell = getpid();

    struct sigaction sigterm_action;
    memset(&sigterm_action, 0, sizeof(sigterm_action));
    sigterm_action.sa_handler = &handleCtrlC;
    sigterm_action.sa_flags = 0;
    sigfillset(&sigterm_action.sa_mask);
    sigaction(SIGINT, &sigterm_action, NULL);

    struct sigaction sigstop_action;
    memset(&sigstop_action, 0, sizeof(sigstop_action));
    sigstop_action.sa_handler = &handleCtrlZ;
    sigstop_action.sa_flags = 0;
    sigfillset(&sigstop_action.sa_mask);
    sigaction(SIGTSTP, &sigstop_action, NULL);

    NodePtr process_list = NULL;

    make_loop(current, prevdir);
}
// current = pseudo home directory

void make_loop(char *current, char *prevdir)
{
    strcpy(previous_dir, prevdir);

    while (1)
    {
        prompt(current);
        char input[4096];
        time_t currentTime;
        currentTime = time(NULL);
        fgets(input, 4096, stdin);
        if (feof(stdin))
            handleCtrlD();
        remove_node();
        find_input(input, current, prevdir);
    }
}

void handleCtrlC()
{
    if (pid_of_shell == getpid()) // fix
        printf("\n");
    else
        kill(getpid(), SIGINT);
}

void handleCtrlD()
{
    char commands[1000][1000];
    strcpy(commands[0], "exit");
    strcpy(commands[1], "NULL");
    pastevents_store(commands, current);

    delete_all();

    int pid = getpid();
    int result = kill(pid, SIGKILL);
    if (result != 0)
        fprintf(stderr, "\x1b[31mError\x1b[0m\n");
    exit(0);
}

void handleCtrlZ()
{
    getpid();
    if (pid_of_shell == getpid())
        printf("\n");
    else
    {
        if (kill(getpid(), SIGTSTP))
            fprintf(stderr, "\x1b[31mError\x1b[0m\n");
    }
}