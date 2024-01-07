#include <stdio.h>

int main()
{
    fprintf(stderr, "\x1b[31mError: Unable to open file\x1b[0m\n");
    // DIR *dir = opendir("..");
    // struct dirent *entry;

    // while ((entry = readdir(dir)) != NULL)
    // {
    //     if (entry->d_name[0] != '.')
    //         printf("%s\n", entry->d_name);
    // }
    return 0;
}

// int SearchFilesAndFolders(char *dirPath, char *name, int d, int f)
// {
//     DIR *dir = opendir(dirPath);

//     if (dir == NULL)
//     {
//         printf("Error opening directory\n");
//         return 1;
//     }

//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL)
//     {
//         if (entry->d_type == DT_DIR) // folder
//         {
//             if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
//                 continue;

//             char newPath[4096];
//             snprintf(newPath, sizeof(newPath), "%s/%s", dirPath, entry->d_name);
//             if ((strncmp(entry->d_name, name, strlen(name)) == 0) && (d == 1 || (f == 0 && d == 0)))
//                 printf("Folder: \033[34m%s/%s\033[0m\n", dirPath, entry->d_name);
//             SearchFilesAndFolders(newPath, name, d, f);
//         }
//         else
//         {
//             if (strncmp(entry->d_name, name, strlen(name)) == 0 && (f == 1 || (f == 0 && d == 0)))
//                 printf("File: %s/%s\n", dirPath, entry->d_name);
//         }
//     }

//     closedir(dir);
//     return 0;
// }