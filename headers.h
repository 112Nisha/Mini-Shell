#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <unistd.h>
#include <grp.h>
#include <time.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <termios.h>
#include <time.h>

typedef struct process *process_pointer;
typedef struct process
{
    int ground; // 0 for bg, // 1 for fg
    int pid;
    char commands[1000][1000];
} process;

typedef struct Node *NodePtr;
typedef struct Node
{
    char *command;
    int pid;
    time_t start_time;
    NodePtr next;
} Node;

typedef struct activity *activity_pointer;
typedef struct activity
{
    char name[256];
    char pid[256];
    char state[256];
    activity_pointer next;
} activity;

extern NodePtr process_list;
extern char extra[4096];
extern pid_t pid_of_shell;
extern int return_from_ctrl_c;
extern char previous_dir[4096];

#define STATUS_FILE_PATH "/proc/%d/status"
#define STAT_FILE_PATH "/proc/%d/stat"

void pastevents_store(char command[1000][1000], char *directory);
void display_pastevents(char *directory);
void delete_pastevents(char *directory);
void execute_pastevents(int index, char *directory, char prevdir[4096]);
void handleCtrlC();
int proclore(char command[1000][1000]);
int print_pgid(int pid);
int print_state(int pid);
int Virtual_Memory(int pid);
int peek(char function_string[1000][1000], char *current, char prevdir[4096]);
int displayFileDetails(const char *filename, char *current_file);
void find_input(char *string_taken, char *current, char prevdir[4096]);
void execute_instr(char commands[1000][1000], char *current, char prevdir[4096], char *string_taken, process_pointer *p, int num_and_plus_one, int i);
NodePtr MakeNode(process p);
void add_node(NodePtr new_node);
void remove_node();
char *reconstruct(char *final, char command[1000][1000]);
int warp(char function_string[1000][1000], char *current, char prevdir[4096]);
void prompt(char code_dir[4096]);
int seek(char function_string[1000][1000], char *current, char prevdir[4096]);
int SearchFilesAndFolders(char *dirPath, char *name, int d, int f);
int Num_Folders(char *dirPath, char *name);
int Num_Files(char *dirPath, char *name);
int Go_Into(char *dirPath, char *name, char function_string[1000][1000], char *current, char prevdir[4096]);
void print_list(NodePtr List);

// Parts B and C
void piping_process(char *sub_sub_string, char *current, char *prevdir, char *original_Command, process_pointer *p, int n, int processth, int pipe_count);
void redirection_exec(char *command, char *current, char *prevdir, char *original_Command, process_pointer *p, int n, int processth);
void activites();
activity_pointer sort_by_name(activity_pointer head);
void DeleteAllNode(activity_pointer head);
activity_pointer AddANode(activity_pointer new_node, activity_pointer head);
activity_pointer MakeANode(char name[256], char pid[256], char state[256]);
void print_activities(activity_pointer head);
void iMan(char command[1000][1000]);
void removeBetweenAndOutside(char *str);
void neonate(int time_to_loop);
void die(const char *s);
void disableRawMode();
void enableRawMode();
void print();
void change_to_fg(int pid);
void change_to_bg(int pid);
void remove_node_with_pid(int pid);
void handleCtrlD();
void handleCtrlZ();
void delete_all();
void signal_handler(int pid, int signal);
void make_loop(char *current, char *prevdir);