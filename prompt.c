#include "headers.h"

char extra[4096] = {'\0'};

void prompt(char code_dir[4096])
{
    int uid = getuid();

    struct passwd *info;
    info = getpwuid(uid);

    struct utsname name;
    uname(&name);

    char dir[4096];
    char right_now[4096];
    getcwd(right_now, 4096);

    int length = strlen(code_dir);
    int other_length = strlen(right_now);

    if (length > other_length)
        strcpy(dir, right_now);
    else
        strcpy(dir, right_now + length);

    printf("\033[32m<%s@%s\033[0m:\033[34m~%s\033[0m %s> ", info->pw_name, name.nodename, dir, extra);

    int leng = strlen(extra);
    for (int i = 0; i < leng; i++)
        extra[i] = '\0';
}