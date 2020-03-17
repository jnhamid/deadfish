#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "history.h"
#include "next_token.h"
#include "logger.h"
#include "ui.h"
#include "builtin.h"
#include "pipe.h"

void sigint_handler(int signo){
    if(isatty(fileno(stdin))){
        printf("\n");
        printf("%s\n",prompt_line1());
        printf("%s", prompt_line2());


    }
}

void sigchild_handler(int signo){
    int status;
    waitpid(-1, &status, WNOHANG);
    set_status(status);
}

int main(void)
{
    init_ui();
    hist_init(100);
    signal(SIGINT, sigint_handler);
    signal(SIGCHLD, sigchild_handler);


    char *command;
   
    

    while (true) {
        char *curr_tok;
        char *next_tok;
        char *cmd[4096] = {0}; 
        int arg_counter = 0;

        next_tok = command = read_command();


        if(command == NULL){
            break;
        }
        if(strcmp(command, "") == 0){

            continue;
        }

        char *duppedCMD = strdup(command);
        hist_add(duppedCMD);

        if(strstr(duppedCMD, " | ") != NULL || strstr(duppedCMD, " > ") != NULL){
            tokenize(command);

            fflush(stdout);
            continue;
        }

        if (feof(stdin) || strncmp(duppedCMD, "exit", 4) == 0) {
            exit(0);
        }
        if(strstr(command, "#") != 0){
            if (strcspn(command,"#") == 0) //free command if comment at beginning
            {
            free(command);
            free(duppedCMD);
             continue;
            }
            *(command+strcspn(command,"#")) = '\0';
        }


        while((curr_tok = next_token(&next_tok, " \t")) != NULL) {

            cmd[arg_counter++] = curr_tok;
        }
        
        if(arg_counter == 0){
            continue;
        }
        if ((strcmp(cmd[0], "cd")==0) || (strcmp(cmd[0], "exit")==0) || (strcmp(cmd[0], "history")==0) || (strcmp(cmd[0], "jobs")==0) || (strcmp(command, "!")==0)){
            int n = handle_builtin(*cmd[0], cmd);

            if(n == -1){
                perror("builtin commands");
            }
        }

    cmd[arg_counter +1 ] = "\0";
    pid_t child = fork();
    if (child == 0) {
        
        execvp(cmd[0], cmd);
        
            LOG("Input command: %s\n", duppedCMD);

        close(fileno(stdin));
        close(fileno(stdout));
        close(fileno(stderr));
        exit(0);
    }
    else if (child == -1) {
        free(command);
        free(duppedCMD);
        perror("fork");
    } else {


        int status;
        waitpid(child, &status, 0);
        set_status(status);



        }
        free(command);
        free(duppedCMD);
        fflush(stdout);
    }

    return 0;
}
   