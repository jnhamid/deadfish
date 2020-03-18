/**
 * Demonstrates string tokenization in C using the strspn(3) and strcspn(3)
 * functions. Unlike strtok(3), this implementation is thread safe. The code
 * is based on the following newsgroup post:
 *
 * https://groups.google.com/forum/message/raw?msg=comp.lang.c/ff0xFqRPH_Y/Cen0mgciXn8J
 */

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

/**
 * Retrieves the next token from a string.
 *
 * Parameters:
 * - str_ptr: maintains context in the string, i.e., where the next token in the
 *   string will be. If the function returns token N, then str_ptr will be
 *   updated to point to token N+1. To initialize, declare a char * that points
 *   to the string being tokenized. The pointer will be updated after each
 *   successive call to next_token.
 *
 * - delim: the set of characters to use as delimiters
 *
 * Returns: char pointer to the next token in the string.
 */
char *next_token(char **str_ptr, const char *delim)
{
    if (*str_ptr == NULL) {
        return NULL;
    }

    size_t tok_start = strspn(*str_ptr, delim);
    size_t tok_end = strcspn(*str_ptr + tok_start, delim);

    /* Zero length token. We must be finished. */
    if (tok_end  == 0) {
        *str_ptr = NULL;
        return NULL;
    }

    /* Take note of the start of the current token. We'll return it later. */
    char *current_ptr = *str_ptr + tok_start;

    /* Shift pointer forward (to the end of the current token) */
    *str_ptr += tok_start + tok_end;

    if (**str_ptr == '\0') {
        /* If the end of the current token is also the end of the string, we
         * must be at the last token. */
        *str_ptr = NULL;
    } else {
        /* Replace the matching delimiter with a NUL character to terminate the
         * token string. */
        **str_ptr = '\0';

        /* Shift forward one character over the newly-placed NUL so that
         * next_pointer now points at the first character of the next token. */
        (*str_ptr)++;
    }

    return current_ptr;
}

//stack over flow
char *replaceWord(const char *s, const char *oldW, const char *newW) 
{ 
    char *result; 
    int i, cnt = 0; 
    int newWlen = strlen(newW); 
    int oldWlen = strlen(oldW); 
  
    // Counting the number of times old word 
    // occur in the string 
    for (i = 0; s[i] != '\0'; i++) 
    { 
        if (strstr(&s[i], oldW) == &s[i]) 
        { 
            cnt++; 
  
            // Jumping to index after the old word. 
            i += oldWlen - 1; 
        } 
    } 
  
    // Making new string of enough length 
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1); 
  
    i = 0; 
    while (*s) 
    { 
        // compare the substring with the result 
        if (strstr(s, oldW) == s) 
        { 
            strcpy(&result[i], newW); 
            i += newWlen; 
            s += oldWlen; 
        } 
        else
            result[i++] = *s++; 
    } 
  
    result[i] = '\0'; 
    return result; 
} 

void runCMD(char* cmd[]){
    pid_t child = fork();
    if (child == 0) {
        
        execvp(cmd[0], cmd);

        close(fileno(stdin));
        close(fileno(stdout));
        close(fileno(stderr));
        exit(0);
    }
    else if (child == -1) {
        perror("fork");
    } else {


        int status;
        waitpid(child, &status, 0);
        set_status(status);

        }
}

void execute(char* command){
        char *curr_tok;
        char *next_tok;
        char *cmd[4096] = {0}; 
        int arg_counter = 0;

        next_tok = command;

        if(command == NULL){
            exit(1);
        }
        if(strcmp(command, "") == 0){

            return;
        }

        char *duppedCMD = strdup(command);

        if(strstr(duppedCMD, " | ") != NULL || strstr(duppedCMD, " > ") != NULL){
            tokenize(command);

            fflush(stdout);
            return;
        }

        if (feof(stdin) || strncmp(duppedCMD, "exit", 4) == 0) {
            exit(0);
        }
        if(strstr(command, "#") != 0){
            if (strcspn(command,"#") == 0) //free command if comment at beginning
            {
            free(command);
            free(duppedCMD);
             return;
            }
            *(command+strcspn(command,"#")) = '\0';
        }


        while((curr_tok = next_token(&next_tok, " \t")) != NULL) {

            cmd[arg_counter++] = curr_tok;
        }
        
        if(arg_counter == 0){
            return;
        }
        LOG("%s\n", cmd[0]);


        hist_add(duppedCMD);
        if ((strcmp(cmd[0], "cd")==0) || (strcmp(cmd[0], "exit")==0) || (strcmp(cmd[0], "history")==0) || (strcmp(cmd[0], "jobs")==0) || (strstr(cmd[0], "!"))){
            int n = handle_builtin(*cmd[0], cmd);

            if(n == -1){
                perror("builtin commands");
            }
        }

    cmd[arg_counter +1 ] = "\0";
    runCMD(cmd);
        free(command);
        free(duppedCMD);
        fflush(stdout);
}



//trying something

