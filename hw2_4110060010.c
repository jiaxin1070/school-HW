#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <limits.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#define HISTORY_FILE "~/.bash_history"
#define MAX_HISTORY_SIZE 100
int num = 0;
pid_t b_pids[100];
int bg_flag = 0;
char file[100][20];
int  bg_pid;
char* output_redirect;
char* b;
FILE* output_file_stream;
char* output_file;
int fd;
int background;

void execute_command(char* command) {

    if (strcmp(command, "pwd") == 0)
    {
        char cwd[PATH_MAX];
        if (getcwd(cwd, PATH_MAX) != NULL)
            printf("%s\n", cwd);
        else
            perror("getcwd");
    }

    else if (strncmp(command, "echo ", 4) == 0)
    {
        if (strncmp(command + 5, "-n", 2) == 0)
            printf("%s", command + 8);
        else if (strncmp(command + 5, "$PATH", 5) == 0)
        {
            char* p = getenv("PATH");
            if (p != NULL)
                printf("%s\n", p);
            else printf("no found\n");
        }
        else printf("%s\n", command + 5);
    }

    else if (strncmp(command, "cd ", 2) == 0)
        chdir(command + 3);

    else if (strncmp(command, "export", 6) == 0)
    {
        char* d = ":";
        char* p;
        char* temp[2];
        p = strtok(command + 13, d);
        int i = 0;
        temp[i] = strtok(NULL, d);
        //printf("%s\n",p);
        char* b = getenv(p);
        char* path = strcat(b, ":");
        path = strcat(path, temp[0]);
        int result = setenv(p, path, 1);
        if (result != 0)
            perror("setenv");
    }
    else
    {
        char* d = " ";
        char* p;
        char* temp[20];
        char c[100];


        strcpy(c, command);
        p = strtok(command, d);
        int l = strlen(p);
        int i = 0;

        while (p != NULL)
        {
            temp[i++] = p;
            p = strtok(NULL, " ");
        }
        pid_t child = fork();
        if (child == -1)
        {
            perror("fork");
            exit(1);
        }

        if (child == 0)
        {
            //execlp(p,p,temp,NULL);
            execlp(temp[0], temp[0], temp[1], temp[2], temp[3], NULL);
        }
        else
        {
            if (background == 1)
            {
                bg_pid = child;
                b_pids[num] = child;
                num++;
                printf("[%d] %d\n", num, child);
            }
            else
                wait(NULL);
        }
    }
}
void check_background_jobs(pid_t* background_pids, int* num_background_jobs)
{
    for (int i = 0; i < *num_background_jobs; i++)
    {
        int status;
        pid_t result = waitpid(background_pids[i], &status, WNOHANG);
        if (result == -1)
            perror("waitpid");

        else if (result == 0)
            printf("bash: bg: job %d already in background\n", *num_background_jobs);

        else
        {
            printf("[%d]+ Done\n", *num_background_jobs);
            for (int j = i; j < (*num_background_jobs) - 1; j++)
                background_pids[j] = background_pids[j + 1];
            (*num_background_jobs)--;
            i--;
        }
    }
}
int main() {
    char* input;
    struct passwd* user_info;
    char host[HOST_NAME_MAX];
    char cwd[PATH_MAX];
    char username[100];
    char* history[MAX_HISTORY_SIZE];
    int historyIndex = -1;
    using_history();
    stifle_history(MAX_HISTORY_SIZE);

    while (1)
    {

    L1:output_redirect = NULL;
        uid_t user_id = geteuid();
        user_info = getpwuid(user_id);
        if (user_info != NULL)
            strcpy(username, user_info->pw_name);
        else
            perror("getpwuid");

        if (gethostname(host, HOST_NAME_MAX) != 0)
            perror("gethostname");

        char* prompt = strcat(username, "@");
        prompt = strcat(prompt, host);
        if (getcwd(cwd, PATH_MAX) != NULL)
        {
            char path[] = "/home/";
            strcat(path, user_info->pw_name);
            if (strcmp(cwd, path) == 0)
                prompt = strcat(prompt, ":~$ ");

            else
            {
                prompt = strcat(prompt, ":");
                prompt = strcat(prompt, cwd);
                prompt = strcat(prompt, "$ ");
            }
        }
        else
            perror("getcwd");


        input = readline(prompt);
        if (input == NULL)
        {
            perror("fgets");
            exit(1);
        }

        char* output_redirect = strstr(input, ">");

        background = 0;
        if (input[strlen(input) - 1] == '&')
        {
            input[strlen(input) - 1] = '\0';
            background = 1;
        }

        input[strcspn(input, "\n")] = 0;
        add_history(input);

        if (strcmp(input, "exit") == 0)
            break;

        else
        {
            if (output_redirect)
            {
                char* output_file = output_redirect + 1;
                while (*output_file == ' ' || *output_file == '\n') {
                    output_file++;
                }

                pid_t pid = fork();
                if (pid == 0)
                {

                    FILE* fd = freopen(output_file, "w", stdout);

                    char* arg[32];
                    int i = 0;
                    char* token = strtok(input, " ");
                    while (token != NULL)
                    {
                        arg[i++] = token;
                        token = strtok(NULL, " ");
                    }
                    arg[i] = NULL;
                    execvp(arg[0], arg);
                    exit(errno);
                    fclose(fd);

                }
                else
                {
                    wait(NULL);
                    fflush(stdout);
                }
            }
            else if (strcmp(input, "bg") == 0)
                check_background_jobs(b_pids, &num);
            else execute_command(input);
        }


        free(input);


    }

    clear_history();
    return 0;
}