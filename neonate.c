#include "headers.h"

struct termios orig_termios;

void die(const char *s)
{
    perror(s);
    exit(1);
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

void neonate(int time_to_loop)
{
    int flags = fcntl(STDOUT_FILENO, F_GETFL, 0);
    if (flags == -1)
    {
        fprintf(stderr, "\x1b[31mError getting flags\x1b[0m\n");
        return;
    }

    if (fcntl(STDOUT_FILENO, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        fprintf(stderr, "\x1b[31mError setting no block\x1b[0m\n");
        return;
    }

    setbuf(stdout, NULL);
    enableRawMode();
    char c;
    time_t start = time(NULL);
    while (1)
    {
        read(STDIN_FILENO, &c, 1);

        if (c == 120)
        {
            printf("%c", c);
            break;
        }

        time_t current_time = time(NULL);
        if (current_time - start >= time_to_loop)
        {
            print();
            start = current_time;
        }
    }
    if (fcntl(STDOUT_FILENO, F_SETFL, flags & ~O_NONBLOCK) == -1)
    {
        fprintf(stderr, "\x1b[31mError\x1b[0m\n");
        return;
    }
    disableRawMode();
}

void print()
{
    FILE *file = fopen("/proc/sys/kernel/ns_last_pid", "r");
    int pid;
    fscanf(file, "%d", &pid);
    fclose(file);
    printf("%d\n", pid);
}