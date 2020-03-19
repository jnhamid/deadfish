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


bool background = false;


void sigint_handler(int signo){
    if(isatty(fileno(stdin))){
        printf("\n");
        printf("%s\n",prompt_line1());
        printf("%s", prompt_line2());


    }
}

// void sigchild_handler(int signo){
//     if(background){
//     int status;
//     pid_t pid;
//     pid = waitpid(-1, &status, WNOHANG);

//     while(pid != 0 &&  pid != -1){
        
//         }
//     }


// }

int main(void)
{
    init_ui();
    hist_init(5);
    signal(SIGINT, sigint_handler);
    signal(SIGCHLD, sigchild_handler);


    char *command;
   
    

    while (true) {
        char *curr_tok;
        char *next_tok;
        char *cmd[4096] = {0}; 
        int arg_counter = 0;
        // bool background = false;
        next_tok = command = read_command();


        if(command == NULL){
            break;
        }
        if(strcmp(command, "") == 0){

            continue;
        }

        LOG("%s\n", command);

        char *duppedCMD = strdup(command);

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
        // }
        // if(strncmp((cmd[arg_counter -1]), "&", 1) == 0) {
        //     LOG("%s\n", *cmd);
        //     background = true;
        //     cmd[arg_counter -1] = "\0";
        //     LOG("%s\n", *cmd);

           

        // }
        if ((strcmp(cmd[0], "cd")==0) || (strcmp(cmd[0], "exit")==0) || (strcmp(cmd[0], "jobs")==0) || (strstr(cmd[0], "!"))){
            int n = handle_builtin(*cmd[0], cmd);

            if(n == -1){
                perror("builtin commands");
            }
        }
        if(strncmp(duppedCMD, "!", 1) != 0){
           hist_add(duppedCMD); 
        }

        if(strcmp(cmd[0], "history")==0){
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
    }else 
    {
        if(!background){
            int status;
            waitpid(child, &status, 0);
            set_status(status);
        }
    }

    free(command);
    free(duppedCMD);
    fflush(stdout);
    }

    return 0;
}
   