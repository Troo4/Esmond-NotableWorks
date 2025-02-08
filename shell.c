#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "shell.h"
const int OUT_SLOT = 0;
const int IN_SLOT = 1;
char** parse(char* inp){ //parses the input into an array of strings, split by spaces
    char** ret = malloc(100 * sizeof(char*));
    char* tok = strtok(inp, " \n");
    int i = 0;
    while(tok != NULL){ 
        ret[i] = tok;
        i++;
        tok = strtok(NULL, " \n");
    }
    ret[i] = NULL;
    return ret;
}
void cd(char* path){ //changes the current working directory using the directory path provided
    if (path == NULL){
        char cur[100];
        getcwd(cur, 100);
        while(strcmp(cur, "/") != 0){ //handles "cd" with no argument like a shell would, navigating to root directory
            chdir("..");
            getcwd(cur, 100);
        }
        return;
    }
    int res = chdir(path);
    if(res != 0){
        printf("Error: Directory not found\n");
    }
    
}
void execute_commands(char** cmd){ //executes the command provided in the cmd array
    int bg = 0;
    bg = isBackground(cmd);
    pid_t pid = fork(); //forks a child process
    if(pid == -1){
        printf("Failed to fork a child process\n");
        return;
    }
    else if(pid == 0){
        execvp(cmd[0], cmd);
        printf("Failed to execute command '%s'\n", cmd[0]);
        exit(1);
    }
    else{
        if(bg != 1){ //no background tasks
            int status;
            waitpid(pid, &status, 0); //waits for the child process to finish
        }
    }
}
int isInput(char** inp, char** names){ //checks if the command contains a "<" character, and sets the input file name in the names array if so
    int i = 0;
    while(inp[i] != NULL){
        if(strcmp(inp[i], "<") == 0){
            names[IN_SLOT] = inp[i+1];
            inp[i] = NULL;
            inp[i+1] = NULL;
            return 1;
        }
        i++;
    }
    return 0;

}
int isOutput(char** inp, char** names){ //checks if the command contains a ">" character, and sets the output file name in the names array if so
    int i = 0;
    while(inp[i] != NULL){
        if(strcmp(inp[i], ">") == 0){
            names[OUT_SLOT] = inp[i+1];
            inp[i] = NULL;
            inp[i+1] = NULL;
            return 1;
        }
        i++;
    }
    return 0;
}
int isBackground(char** inp){ //checks if the command contains an "&" character, and returns 1 if so
    int i = 0;
    while(inp[i] != NULL){
        i++;
    }
    if(strcmp(inp[i-1], "&") == 0){
        inp[i-1] = NULL;
        return 1;
    }
    return 0;
}
void handleChild(int sig){ //handles the termination of child processes
    int status;
    while(waitpid(-1, &status, WNOHANG) > 0){
        //printf("Child process ended\n");
    }
}
void command(char** cmd){ //executes the command, checking for input/output redirection and background tasks
    char** names = malloc(2 * sizeof(char*));
    int in = isInput(cmd, names);
    int out = isOutput(cmd, names);
    if(in == 1 && out == 1){
        int pid = fork();
        if(pid == 0){
            FILE* file = freopen(names[IN_SLOT], "r", stdin);
            if(file == NULL){
                printf("Could not open file '%s'\n", names[IN_SLOT]);
                exit(1);
            }
            file = freopen(names[OUT_SLOT], "w", stdout);
            if(file == NULL){
                printf("Could not open file '%s'\n", names[OUT_SLOT]);
                exit(1);
            }
            execvp(cmd[0], cmd);
            printf("Command '%s' not recognized\n", cmd[0]);
            exit(1);
        }
        else{
            int status;
            waitpid(pid, &status, 0);
        }
    }
    else if(in == 1){
        int pid = fork();
        if(pid == 0){
            FILE* file = freopen(names[IN_SLOT], "r", stdin);
            if(file == NULL){
                printf("Could not open file '%s'\n", names[IN_SLOT]);
                exit(1);
            }
            execvp(cmd[0], cmd);
            printf("Command '%s' not recognized\n", cmd[0]);
            exit(1);
        }
        else{
            int status;
            waitpid(pid, &status, 0);
        }
    }
    else if(out == 1){
        int pid = fork();
        if(pid == 0){
            FILE* file = freopen(names[OUT_SLOT], "w", stdout);
            if(file == NULL){
                printf("Could not open file '%s'\n", names[OUT_SLOT]);
                exit(1);
            }
            execvp(cmd[0], cmd);
            printf("Command '%s' not recognized\n", cmd[0]);
            exit(1);
        }
        else{
            int status;
            waitpid(pid, &status, 0);
        }
    }
    else{
        execute_commands(cmd);
    }
    
    free(names);
}

int main(){
    signal(SIGCHLD,handleChild);
    int status = 1;
    while(status == 1){
        char wd[100];
        getcwd(wd, 100);
        printf("%s> ", wd);
        char inp[100];
        fgets(inp, 100, stdin);
        char** cmd = parse(inp); //parses the input into an array of strings   
        //printf("Command was:'%s'\n", cmd[0]);
        if(strcmp(cmd[0], "exit") == 0){ //checks if the command is an exit command

            status = 0; //exits the shell on next iteration
        }
        else if(strcmp(cmd[0], "cd") == 0){ //checks if the command is a cd command
            //printf("secondary command was: '%s'\n", cmd[1]);
            cd(cmd[1]); //calls the cd function to attempt to change the directory
        }
        else {
            //runs the command using the command function
            command(cmd);
            
            
        }
        
        free(cmd);
    }
    

}