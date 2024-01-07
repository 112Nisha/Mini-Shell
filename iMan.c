#include "headers.h"

void iMan(char command[1000][1000])
{
    if (strcmp(command[1], "NULL") == 0)
    {
        fprintf(stderr, "\x1b[31mUsage: iMan <command_name>\x1b[0m\n");
        return;
    }

    char url[4096];
    if (command[1][strlen(command[1]) - 1] == '\n')
        command[1][strlen(command[1]) - 1] = '\0';

    // url[0] = '/';
    strcpy(url, command[1]);

    struct addrinfo start;
    struct addrinfo *result;
    memset(&start, 0, sizeof(start));
    int size = sizeof(start);
    start.ai_family = AF_INET;       // IPv4
    start.ai_socktype = SOCK_STREAM; // TCP

    if (getaddrinfo("man.he.net", "80", &start, &result) != 0) // DNS resolver to make the man site into IP address
    {
        fprintf(stderr, "\x1b[31mUnable to establish connection\x1b[0m\n");
        return;
    }

    int sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sockfd == -1) // open socket
    {
        fprintf(stderr, "\x1b[31mUnable to open socket\x1b[0m\n");
        return;
    }

    if (connect(sockfd, result->ai_addr, result->ai_addrlen) == -1) // connect to socket
    {
        fprintf(stderr, "\x1b[31mUnable to connect\x1b[0m\n");
        return;
    }

    // to make the GET request
    char request[4096];
    strcpy(request, "GET /?topic=");
    strcat(request, url);
    strcat(request, "&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n");
    // printf("%s", request);

    if (send(sockfd, request, strlen(request), 0) == -1) // send the request
    {
        fprintf(stderr, "\x1b[31mUnable to send request\x1b[0m\n");
        return;
    }

    char response[4096 + 1];
    int total_bytes = 0;
    int bytes_received = 0;

    int i = 0;

    while ((bytes_received = recv(sockfd, response, 4096, 0)) > 0) // receive the response
    {
        total_bytes += bytes_received;
        response[bytes_received] = '\0';
        removeBetweenAndOutside(response);
        if (i == 0)
        {
            char* ptr1 = strstr(response, "NAME");
            if(ptr1 == NULL)
            {
                fprintf(stderr, "\x1b[31mNo manual entry for %s\x1b[0m\n", command[1]);
                return;
            }
            printf("%s", ptr1);
            i = 1;
        }
        else
            printf("ihih%s", response);
    }

    close(sockfd);
    freeaddrinfo(result);
    return;
}

void removeBetweenAndOutside(char *str)
{
    int i, j;
    int inside = 0;

    for (i = j = 0; str[i]; i++)
    {
        if (str[i] == '<')
            inside = 1;
        else if (str[i] == '>')
        {
            inside = 0;
            continue;
        }

        if (!inside)
            str[j++] = str[i];
    }

    str[j] = '\0';
}