#include <stdio.h>
#include <readline/readline.h>
#include <locale.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>



#include "history.h"
#include "logger.h"
#include "ui.h"
#include "next_token.h"


/* -- Private function forward declarations -- */
static int readline_init(void);
int key_up(int count, int key);
int key_down(int count, int key);
static char **command_completion(const char *text, int start, int end);
char *command_generator(const char *text, int state);





// static char prompt_str1[80] = "--[enter a command]--";
static char prompt_str2[80] = "-> ";
int mycount = 0;
char* status = "🐉";

bool script;
int n = 0;

int histIndex = 0;

static char* path = NULL;

DIR *dummy = NULL;




void init_ui(void)
{

    LOGP("Initializing UI...\n");

    char *locale = setlocale(LC_ALL, "en_US.UTF-8");
    LOG("Setting locale: %s\n",
            (locale != NULL) ? locale : "could not set locale!");

    rl_startup_hook = readline_init;

}

char* set_status(int s){
    if(s != 0){
        status = "🖕";
    }else{
        status = "🐉";
    }
    return status;
}

char *prompt_line1(void) {
    
    struct passwd *pwd;

    uid_t uid = getuid();
    pwd = getpwuid(uid);

    char *temp = (char*)malloc(8196 * sizeof(char));

    char hostbuffer[256];
    gethostname(hostbuffer, sizeof(hostbuffer));
 
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    if(strstr(cwd, pwd -> pw_dir) == cwd){
        sprintf(cwd, replaceWord(cwd, pwd -> pw_dir, "~"));
    }
    // if(script){

    sprintf(temp, "-[%s]-[%d]-[%s@%s:%s]--", status, getCount(), pwd -> pw_name, hostbuffer, cwd);
    // }
    return temp;

}

char *prompt_line2(void) {
    // if(script){
        return prompt_str2;

    // }
    // return NULL;
}

char *read_command(void)
{
    if (isatty(STDIN_FILENO)) {
        char* promptLine = prompt_line1();
        puts(promptLine);
        free(promptLine);
        return readline(prompt_line2());
    }
    else {
        size_t line_sz = 100;
        char* line = malloc(100 * sizeof(char));
        ssize_t read = getline(&line, &line_sz, stdin);

        if (read == -1) {
            // LOGP("Reached end of input stream. \n");
            free(line);
            return NULL;
        }
         if (line == NULL || strcmp(line, "") == 0) {
            free(line);
            return NULL;
        }

        size_t newline = strcspn(line, "\n");
        line[newline] = '\0';
        return line;
    }
}

int readline_init(void)
{
    rl_bind_keyseq("\\e[A", key_up);
    rl_bind_keyseq("\\e[B", key_down);
    rl_variable_bind("show-all-if-ambiguous", "on");
    rl_variable_bind("colored-completion-prefix", "on");
    rl_attempted_completion_function = command_completion;
    return 0;
}

int key_up(int count, int key)
{
    histIndex = hist_last_cnum();
    // LOG("%d\n",histIndex);

    /* Modify the command entry text: */
    rl_replace_line("User pressed 'up' key", 1);

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: reverse history search

    return 0;
}

int key_down(int count, int key)
{
    /* Modify the command entry text: */
    rl_replace_line("User pressed 'down' key", 1);

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: forward history search

    return 0;
}

char **command_completion(const char *text, int start, int end)
{
    /* Tell readline that if we don't find a suitable completion, it should fall
     * back on its built-in filename completion. */
    rl_attempted_completion_over = 0;

    return rl_completion_matches(text, command_generator);
}

/**
 * This function is called repeatedly by the readline library to build a list of
 * possible completions. It returns one match per function call. Once there are
 * no more completions available, it returns NULL.
 */
char *command_generator(const char *text, int state)
{
    // TODO: find potential matching completions for 'text.' If you need to
    // initialize any data structures, state will be set to '0' the first time
    // this function is called. You will likely need to maintain static/global
    // variables to track where you are in the search so that you don't start
    // over from the beginning.
    char *curr_tok;
    char *next_tok;

    // LOG("----------------state:%d\n", state);

    path = getenv("PATH");

    // LOG("path: %s\n", path);


    if(path == NULL || strcmp(path, "") == 0){
        return NULL;
        
    }


    if(state == 0){

        next_tok = strdup(path);
        
        curr_tok = next_token(&next_tok, ":");

        dummy = opendir(curr_tok);
    }

    if( dummy  == NULL){
        return NULL;
    }
    else{
        struct dirent *enter;
        while((enter = readdir(dummy)) != NULL){
                // LOG("enter name: %s  text: %s\n", enter -> d_name, text);
                if(strncmp(enter -> d_name, text, strlen(text)) == 0){
                    // LOG("%s\n", "Found a match");
                    return strdup(enter -> d_name);
                }
            }

        while((curr_tok = next_token(&next_tok, ":")) != NULL){
            dummy = opendir(curr_tok);
            if(dummy == NULL){
                break;
            }
             while((enter = readdir(dummy)) != NULL){
                // LOG("enter name: %s  text: %s\n", enter -> d_name, text);
                if(strncmp(enter -> d_name, text, strlen(text)) == 0){
                    // LOG("%s\n", "Found a match");
                    return strdup(enter -> d_name);
                }
            }
        }
        if(strncmp(text, "cd", strlen(text)) == 0){
               return strdup("cd");
        }
        if(strncmp(text, "exit", strlen(text)) == 0){
               return strdup("exit");
        }
        if(strncmp(text, "history", strlen(text)) == 0){
               return strdup("history");
        }
        if(strncmp(text, "jobs", strlen(text)) == 0){
               return strdup("jobs");
        }
    }

    return NULL;
}
