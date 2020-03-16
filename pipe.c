#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#include "pipe.h"
#include "next_token.h"
#include "ui.h"

void createCmd(char **cmd, int num, struct command_line *commands)
{
 
    int index = 0;
    commands[index].tokens = &cmd[0];
    commands[index].stdout_pipe = true;
    commands[index].stdout_file = NULL;
    index++;
    for(int i =0; i< num; i++){
        if(strcmp(cmd[i], "|") == 0){
            cmd[i] = 0;
            commands[index].tokens = &cmd[i + 1];
            commands[index].stdout_pipe = true;
            commands[index].stdout_file = NULL;
            index++;
        }
        else if(strcmp(cmd[i], ">") == 0){
            cmd[i] = 0;
            commands[index -1].stdout_file = cmd[i + 1];
        }

    }

    commands[index -1].stdout_pipe = false;
}

void execute_pipeline(struct command_line *cmds)
{
    int open_flags = O_RDWR | O_CREAT | O_TRUNC;

    if(cmds -> stdout_pipe != false){
        int fildes[2] = {0};


        if(pipe(fildes) == -1){
            perror("pipe");
            return;
        }
        int child = fork();
        if(child == 0){
            dup2(fildes[1], fileno(stdout));
            close(fildes[0]);
            execvp(cmds-> tokens[0], cmds-> tokens);
        }else if(child == -1){
            perror("child");
            return;
        }
        else{
            dup2(fildes[0], fileno(stdin));
            close(fildes[1]);
            execute_pipeline(++cmds);
        }
    }else{
        if (cmds-> stdout_file != NULL) {
            int open_perms = 0644;
            int fd = open(cmds-> stdout_file, open_flags, open_perms);
            if (fd == -1) {
                perror("open");
                return;
            }
            if (dup2(fd, fileno(stdout)) == -1) {
                close(fd);
                return;
            }
        }
        execvp(cmds-> tokens[0], cmds-> tokens);
        return;
    }
}




void tokenize(char* command){
    char *curr_tok;
    char *next_tok;
    int num = 0;
    char *cmd[_POSIX_ARG_MAX] = {0}; 


    next_tok = command;

    while((curr_tok = next_token(&next_tok, " \t")) != NULL) {

        cmd[num++] = curr_tok;
    } 

    struct command_line cmds[_POSIX_ARG_MAX];
    createCmd(cmd, num, cmds);
    pid_t child = fork();
    if (child == 0) {
        execute_pipeline(cmds);
    } else if (child == -1) {
        //error handling
        perror("fork");
    } else {
        int status;
        waitpid(child, &status, 0);
        set_status(status);
         //status //WHNSMTH -> project
    }
}









