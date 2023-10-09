#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS    32

int num_args;
int cmd_len;
char cmd[MAX_CMD_LEN];
char *args[MAX_ARGS];
char *token;
pid_t pid;

int main() {
    while (1) {
        printf("osh> "); fflush(stdout);

        if (fgets(cmd, MAX_CMD_LEN, stdin) == NULL) {
            // Ctrl + D detected
            printf("\nexit\n");
            break;
        }

        cmd_len = strlen(cmd);
        if (cmd_len > 1) {
            // Remove '\n'
            cmd[cmd_len - 1] = '\0';
        } else {
            // Only '\n' received.
            continue;
        }

        // Parse arguments
        num_args = 0;
        token = strtok(cmd, " ");
        while (token != NULL && num_args < MAX_ARGS - 1) {
            args[num_args] = token;
            token = strtok(NULL, " ");
            num_args++;
        }
        args[num_args] = NULL;

        // Handles exit
        if (strcmp(args[0], "exit") == 0) {
            printf("Process end\n");
            break;
        }

        pid = fork();
        if (pid < 0) {
            perror("Error: Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (execvp(args[0], args) == -1) {
                fprintf(stderr, "%s: command not found\n", args[0]);
                exit(EXIT_FAILURE); 
            }
        } else {
            wait(NULL);
        }
    }

    return 0;
}