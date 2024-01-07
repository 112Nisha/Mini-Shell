#include "headers.h"

int peek(char function_string[1000][1000], char *current, char prevdir[4096])
{
    int i;
    for (i = 0; i < 5; i++)
    {
        if (strcmp(function_string[i], "NULL") == 0)
            break;
    }
    if (function_string[i - 1][strlen(function_string[i - 1]) - 1] == '\n')
        function_string[i - 1][strlen(function_string[i - 1]) - 1] = '\0';
    // make \n -> \0

    int error = 0;
    int a = 0;
    int l = 0;
    int other_random = 0;
    int start_of_if_path = 1;
    int flag[26] = {0};

    if (function_string[1][0] == '-' && function_string[1][1] != '\0')
    {
        for (int i = 1; i < strlen(function_string[1]); i++)
            flag[function_string[1][i] - 'a']++;
        start_of_if_path++;
    }

    if (function_string[2][0] == '-' && function_string[2][1] != '\0')
    {
        for (int i = 1; i < strlen(function_string[1]); i++)
            flag[function_string[2][i] - 'a']++;
        start_of_if_path++;
    }

    for (int i = 0; i < 26; i++)
    {
        if (i == 0 || i == 11)
            continue;
        if (flag[i] == 1)
            other_random = 1;
    }
    // a and l have been set
    a = flag[0];
    l = flag['l' - 'a'];

    if (other_random == 1)
    {
        error++;
        fprintf(stderr, "\x1b[31mInvalid flags!\x1b[0m\n");
        return 1;
    }

    int is_path = 0;
    int is_dash = 0;
    int is_tilde = 0;
    if (strcmp(function_string[start_of_if_path], "NULL") != 0)
    {
        is_path = start_of_if_path;
        if (strcmp(function_string[start_of_if_path], "-\n") == 0)
            is_dash = 1;
        else if (strcmp(function_string[start_of_if_path], "~\n") == 0)
            is_tilde = 1;
    }
    // is_path stores the index of path if it is entered

    struct dirent **entry;

    char right_now[4096];
    getcwd(right_now, sizeof(right_now));

    int n;

    if (is_path != 0 && is_dash == 0 && is_tilde == 0)
    {
        n = scandir(function_string[is_path], &entry, NULL, alphasort);
        if (n == 0)
        {
            fprintf(stderr, "\x1b[31m%s is not a directory\x1b[0m\n", function_string[is_path]);
            error = 1;
            return error;
        }
    }
    else if (is_dash == 1)
    {
        if (prevdir[0] == '\0')
        {
            printf("OLDPWD not set\n");
            error = 1; // pl check
            return error;
        }
        else
        {
            n = scandir(prevdir, &entry, NULL, alphasort);
            if (n == 0)
            {
                fprintf(stderr, "\x1b[31m%s is not a directory\x1b[0m\n", prevdir);
                error = 1;
                return error;
            }
        }
    }
    else if (is_tilde == 1)
    {
        n = scandir(current, &entry, NULL, alphasort);
        if (n == 0)
        {
            fprintf(stderr, "\x1b[31m%s is not a directory\x1b[0m\n", current);
            error = 1;
            return error;
        }
    }
    else
    {
        n = scandir(".", &entry, NULL, alphasort);
        if (n == 0)
        {
            fprintf(stderr, "\x1b[31mError\x1b[0m\n");
            error = 1;
            return error;
        }
    }

    if (a == 1 && l != 1)
    {
        for (int i = 0; i < n; i++)
            printf("%s\n", entry[i]->d_name);
    }
    else if (a != 1 && l != 1)
    {
        for (int i = 0; i < n; i++)
        {
            if (entry[i]->d_name[0] != '.')
                printf("%s\n", entry[i]->d_name);
        }
    }
    else if (a == 1 && l == 1)
    {
        // show all info and all files
        for (int i = 0; i < n; i++)
        {
            if (strcmp(entry[i]->d_name, ".") != 0 && strcmp(entry[i]->d_name, "..") != 0)
            {
                char name_of_file_with_path[4096];
                name_of_file_with_path[0] = '\0';
                if (is_path > 0)
                {
                    strcpy(name_of_file_with_path, function_string[is_path]);
                    strcpy(name_of_file_with_path + strlen(name_of_file_with_path), "/");
                }
                strcpy(name_of_file_with_path + strlen(name_of_file_with_path), entry[i]->d_name);
                error += displayFileDetails(name_of_file_with_path, entry[i]->d_name);
            }
        }
    }
    else
    {
        // show info about non-hidden files
        // works most of the time, but doesn't sometimes? I wonder why??
        for (int i = 0; i < n; i++)
        {
            if (entry[i]->d_name[0] != '.')
            {
                char name_of_file_with_path[4096];
                name_of_file_with_path[0] = '\0';
                if (is_path > 0)
                {
                    strcpy(name_of_file_with_path, function_string[is_path]);
                    strcpy(name_of_file_with_path + strlen(name_of_file_with_path), "/");
                }
                strcpy(name_of_file_with_path + strlen(name_of_file_with_path), entry[i]->d_name);
                error += displayFileDetails(name_of_file_with_path, entry[i]->d_name);
            }
        }
    }
    return error;
}

int displayFileDetails(const char *filename, char *current_file)
{
    struct stat fileStat;

    if (stat(filename, &fileStat) == -1)
    {
        fprintf(stderr, "\x1b[31mError in name\x1b[0m\n");
        return 1;
    }

    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

    printf(" %ld", (long)fileStat.st_nlink);

    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group *gr = getgrgid(fileStat.st_gid);
    printf(" %s %s", pw->pw_name, gr->gr_name);

    printf(" %lld", (long long)fileStat.st_size);

    char timeStr[50];
    struct tm *tmInfo = localtime(&fileStat.st_mtime);
    strftime(timeStr, sizeof(timeStr), "%b %d %H:%M", tmInfo);

    printf(" %s", timeStr);

    int is_dir = 0;
    if (S_ISDIR(fileStat.st_mode))
    {
        is_dir = 1;
    }

    if (access(current_file, X_OK) == 0 && is_dir != 1) // executable
        printf(" \033[32m%s\n\033[0m", current_file);
    else if (current_file[0] == '.') // hidden files
        printf(" \033[34m%s\n\033[0m", current_file);
    else if (current_file[strlen(current_file) - 1] == 'g' && current_file[strlen(current_file) - 2] == 'n' && current_file[strlen(current_file) - 3] == 'p' && current_file[strlen(current_file) - 4] == '.') // .png files
        printf(" \033[35m%s\n\033[0m", current_file);
    else if (is_dir == 1) // folder
        printf(" \033[34m%s\n\033[0m", current_file);
    else // normal file
        printf(" %s\n", current_file);

    return 0;
}