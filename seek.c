#include "headers.h"

int seek(char function_string[1000][1000], char *current, char prevdir[4096])
{
    if (strcmp(function_string[1], "NULL") == 0)
    {
        fprintf(stderr, "\x1b[31mIncorrect usage of seek!\n Include a filename.\x1b[0m\n");
        return 1;
    }
    int error = 0;
    int i;
    for (i = 0; i < 5; i++)
    {
        if (strcmp(function_string[i], "NULL") == 0)
            break;
    }
    function_string[i - 1][strlen(function_string[i - 1]) - 1] = '\0';

    int d = 0;
    int e = 0;
    int f = 0;
    int other_random = 0;
    int start_of_name = 1;
    int flag[26] = {0};

    if (function_string[1][0] == '-' && function_string[1][1] != '\0')
    {
        for (int i = 1; i < strlen(function_string[1]); i++)
            flag[function_string[1][i] - 'a']++;
        start_of_name++;
    }

    if (function_string[2][0] == '-' && function_string[2][1] != '\0')
    {
        for (int i = 1; i < strlen(function_string[1]); i++)
            flag[function_string[2][i] - 'a']++;
        start_of_name++;
    }

    if (function_string[3][0] == '-' && function_string[3][1] != '\0')
    {
        for (int i = 1; i < strlen(function_string[1]); i++)
            flag[function_string[2][i] - 'a']++;
        start_of_name++;
    }

    for (int i = 0; i < 26; i++)
    {
        if (i == 3 || i == 4 || i == 5)
            continue;
        if (flag[i] == 1)
            other_random = 1;
    }
    // flags have been set
    d = flag['d' - 'a'];
    e = flag['e' - 'a'];
    f = flag['f' - 'a'];

    if (other_random == 1)
    {
        error++;
        fprintf(stderr, "\x1b[31mInvalid flags!\x1b[0m\n");
        return 1;
    }

    if (flag['d' - 'a'] == 1 && flag['f' - 'a'] == 1)
    {
        error++;
        fprintf(stderr, "\x1b[31mInvalid flags!\x1b[0m\n");
        return 1;
    }

    int start_of_if_dir = start_of_name + 1;
    char *dir_path;
    if (strcmp(function_string[start_of_if_dir], "NULL") == 0)
        dir_path = ".";
    else
        strcpy(dir_path, function_string[start_of_if_dir]);

    if (e == 0)
    {
        int num = SearchFilesAndFolders(dir_path, function_string[start_of_name], d, f);
        if (num == 0)
        {
            fprintf(stderr, "\x1b[31mNo match found\x1b[0m\n");
            return 1;
        }
    }
    else
    {
        int num_files = Num_Files(dir_path, function_string[start_of_name]);
        int num_folders = Num_Folders(dir_path, function_string[start_of_name]);
        if ((num_files == 0 && f == 1) || (num_folders == 0 && d == 1))
        {
            fprintf(stderr, "\x1b[31mNo match found\x1b[0m\n");
            return 1;
        }
        else if ((num_files > 1 && f == 1))
        {
            fprintf(stderr, "\x1b[31mToo many such files\x1b[0m\n");
            return 1;
        }
        else if ((num_folders > 1 && d == 1))
        {
            fprintf(stderr, "\x1b[31mToo many such folders\x1b[0m\n");
            return 1;
        }
        else if((num_files == 1 && f == 0) || (num_folders == 1 && d == 0))
            Go_Into(dir_path, function_string[start_of_name], function_string, current, prevdir);
        else if((num_files == 1 && f == 1) || (num_folders == 1 && d == 1))
            Go_Into(dir_path, function_string[start_of_name], function_string, current, prevdir);
    }

    return 0;
}

int Go_Into(char *dirPath, char *name, char function_string[1000][1000], char *current, char prevdir[4096])
{
    DIR *dir = opendir(dirPath);

    if (dir == NULL)
    {
        fprintf(stderr, "\x1b[31mMissing permissions for task!\x1b[0m\n");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR) // folder
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char newPath[4096];
            snprintf(newPath, sizeof(newPath), "%s/%s", dirPath, entry->d_name);
            Go_Into(newPath, name, function_string, current, prevdir);
            if (strncmp(entry->d_name, name, strlen(name)) == 0)
            {
                function_string[1000][1000];
                strcpy(function_string[0], "warp");
                strcpy(function_string[1], newPath);
                strcpy(function_string[2], "NULL");
                warp(function_string, current, prevdir);
                return 0;
            }
        }
        else
        {
            if (strncmp(entry->d_name, name, strlen(name)) == 0)
            {
                FILE *file_pointer = fopen(name, "r");
                if (file_pointer == NULL)
                {
                    fprintf(stderr, "\x1b[31mMissing permissions for task!\x1b[0m\n");
                    return 1;
                }

                char line[4096];
                while (fgets(line, sizeof(line), file_pointer))
                    printf("%s", line);

                fclose(file_pointer);
            }
        }
    }

    closedir(dir);
    return 0;
}

int Num_Files(char *dirPath, char *name)
{
    int num = 0;
    DIR *dir = opendir(dirPath);

    if (dir == NULL)
    {
        fprintf(stderr, "\x1b[31mError opening directory\x1b[0m\n");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR) // folder
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char newPath[4096];
            snprintf(newPath, sizeof(newPath), "%s/%s", dirPath, entry->d_name);
            num += Num_Files(newPath, name);
        }
        else
        {
            if (strncmp(entry->d_name, name, strlen(name)) == 0)
                num++;
        }
    }

    closedir(dir);
    return num;
}

int Num_Folders(char *dirPath, char *name)
{
    int num = 0;
    DIR *dir = opendir(dirPath);

    if (dir == NULL)
    {
        fprintf(stderr, "\x1b[31mError opening directory\x1b[0m\n");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR) // folder
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char newPath[4096];
            snprintf(newPath, sizeof(newPath), "%s/%s", dirPath, entry->d_name);
            // num += Num_Folders(newPath, name);
            if (strncmp(entry->d_name, name, strlen(name)) == 0)
                num++;
            num += Num_Folders(newPath, name);
        }
    }

    closedir(dir);
    return num;
}

int SearchFilesAndFolders(char *dirPath, char *name, int d, int f)
{
    int num = 0;
    DIR *dir = opendir(dirPath);

    if (dir == NULL)
    {
        fprintf(stderr, "\x1b[31mError opening directory ()\x1b[0m\n");
        return 0;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR) // folder
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char newPath[4096];
            snprintf(newPath, sizeof(newPath), "%s/%s", dirPath, entry->d_name);
            if ((strncmp(entry->d_name, name, strlen(name)) == 0) && (d == 1 || (f == 0 && d == 0)))
            {
                printf("Folder: \033[34m%s/%s\033[0m\n", dirPath, entry->d_name);
                num++;
            }
            num += SearchFilesAndFolders(newPath, name, d, f);
        }
        else
        {
            if (strncmp(entry->d_name, name, strlen(name)) == 0 && (f == 1 || (f == 0 && d == 0)))
            {
                printf("File: %s/%s\n", dirPath, entry->d_name);
                num++;
            }
        }
    }

    closedir(dir);
    return num;
}