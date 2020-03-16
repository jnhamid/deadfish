#ifndef _PIPE_H_
#define _PIPE_H_

#include <stdbool.h>

struct command_line {
    char **tokens;
    bool stdout_pipe;
    char *stdout_file;
};


void execute_pipeline(struct command_line *cmds);
void tokenize(char*);

#endif
