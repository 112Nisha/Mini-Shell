#include "headers.h"

int warp(char function_string[1000][1000], char *current, char prevdir[4096])
{
    strcpy(previous_dir, prevdir);
    int error = 0;
    int index = 1;
    char directory[4096];

    char right_now[4096];
    getcwd(right_now, 4096);

    if (strcmp(function_string[index], "NULL") == 0)
    {
        strcpy(prevdir, right_now);
        chdir(current);
    }

    while (function_string[index][0] != '\n' && function_string[index][0] != '\0' && strcmp(function_string[index], "NULL") != 0)
    {
        char *working_on_path = strtok(function_string[index], "/");
        while (working_on_path != NULL && working_on_path[0] != '\n' && working_on_path[0] != '\0' && strcmp(working_on_path, "NULL") != 0)
        {
            char curr[4096];
            getcwd(curr, 4096);

            int length = strlen(curr);

            if (strcmp(working_on_path, "~\n") == 0 || strcmp(working_on_path, "~") == 0)
            {
                strcpy(prevdir, right_now);
                chdir(current);
            }
            else if (strcmp(working_on_path, "-\n") == 0 || strcmp(working_on_path, "-") == 0)
            {
                if (prevdir[0] != '\0')
                {
                    char temp[4096];
                    strcpy(temp, prevdir);
                    getcwd(prevdir, 4096);
                    chdir(temp);
                }
                else
                {
                    fprintf(stderr, "\x1b[31mOLDPWD not set\x1b[0m\n"); // assuming this isnt an error
                    error = 1;
                }
            }
            else
            {
                strcpy(directory, curr);
                strcpy(directory + length, "/");
                strcpy(directory + 1 + length, working_on_path);
                if (directory[strlen(directory) - 1] == '\n')
                    directory[strlen(directory) - 1] = '\0';

                int worked = 1;
                worked = chdir(directory);

                if (worked != 0)
                {
                    char temp[4096];
                    strcpy(temp, working_on_path);
                    temp[strlen(temp) - 1] = '\0';
                    fprintf(stderr, "\x1b[31m%s is not a valid directory\x1b[0m\n", working_on_path);
                    error = 1;
                    break;
                }
                else
                {
                    strcpy(prevdir, right_now);
                }
            }
            getcwd(curr, 4096);
            working_on_path = strtok(NULL, "/");
        }
        index += 1;
    }
    char curr[4096];
    getcwd(curr, 4096);
    printf("%s\n", curr);

    strcpy(previous_dir, prevdir);
    return error;
}