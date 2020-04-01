/**
 * @file
 *
 * Global stuff for pipes
 */
#ifndef _PIPE_H_
#define _PIPE_H_

#include <stdbool.h>


/**
   struct for my commands
*/
struct command_line {
    char **tokens;
    bool stdout_pipe;
    char *stdout_file;
};


/**
   function that executes my pipeline
*/
void execute_pipeline(struct command_line *cmds);

/**
   function that tokenizes my commands
*/
void tokenize(char*);

#endif
