#include "headers.h"

void signal_handler(int pid, int signal)
{
    if (kill(pid, 0) == -1)
    {
        fprintf(stderr, "\x1b[31mNo such process found\x1b[0m\n");
        return;
    }

    signal = signal % 32;
    int result = kill(pid, signal);

    if (result == 0)
        printf("Sent signal %d to process with pid %d\n", signal, pid);
    else
        fprintf(stderr, "\x1b[31mSignal not sent\x1b[0m\n");
}