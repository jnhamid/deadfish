/**
 * @file
 *
 * Text-based UI functionality. These functions are primarily concerned with
 * interacting with the readline library.
 */
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


char* searchStr = NULL;
char* lastStr =NULL;





// static char prompt_str1[80] = "--[enter a command]--";
static char prompt_str2[80] = "-> ";
int mycount = 0;
char* status = "🐉";

bool script;
int n = 0;

// int i; 
// int j;

int histIndex;

int keyIndex =0;

int matchC = 0;

int matchSize;



static char* path = NULL;

DIR *dummy = NULL;

static int built_in_index =0;

static char* built_in[4] = {"cd", "exit", "history", "jobs"};



/**
   function that runs my UI
*/
void init_ui(void)
{

    LOGP("Initializing UI...\n");

    char *locale = setlocale(LC_ALL, "en_US.UTF-8");
    LOG("Setting locale: %s\n",
            (locale != NULL) ? locale : "could not set locale!");

    rl_startup_hook = readline_init;

}

/**
   function that sets my status
*/
char* set_status(int s){
    if(s != 0){
        status = "🖕";
    }else{
        status = "🐉";
    }
    return status;
}

/**
   function that prints my promptline
*/
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

    sprintf(temp, "-[%s]-[%d]-[%s@%s:%s]--", status, getCount(), pwd -> pw_name, hostbuffer, cwd);
    return temp;

}

/**
   function that prints my promptline
*/
char *prompt_line2(void) {
        return prompt_str2;
}

/**
   function that runs a command for terminal or script
*/
char *read_command(void)
{
    if (isatty(STDIN_FILENO)) {
        char* promptLine = prompt_line1();
        puts(promptLine);
        return readline(prompt_line2());
    }
    else {
        size_t line_sz = 1;
        char* line = malloc(1 * sizeof(char));
        ssize_t read = getline(&line, &line_sz, stdin);

        if (read == -1) {
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


/**
   function that initilzies readline
*/
int readline_init(void)
{
    // i =0; 
    // j = hist_last_cnum() ;

    // while(i < 100 && j > 1){
    //     matches[i] = strdup(hist_search_cnum(j));
    //     i++;
    //     j--;
    // }
    // if(j == 1){
    //     j--;
    // }

    rl_bind_keyseq("\\e[A", key_up);
    rl_bind_keyseq("\\e[B", key_down);
    rl_variable_bind("show-all-if-ambiguous", "on");
    rl_variable_bind("colored-completion-prefix", "on");
    rl_attempted_completion_function = command_completion;
    histIndex = hist_last_cnum() + 1;
    return 0;
}

/**
    function that handles key up being presessed
*/
int key_up(int count, int key)
{

    // LOG("%s\n", rl_line_buffer);
    /* Modify the command entry text: */
    // LOG("%s\n", searchStr);
    // LOG("%s\n", rl_line_buffer);



    if(keyIndex == 0){
        searchStr = strdup(rl_line_buffer);
        matchSize = hist_search_prefix_to_num(searchStr);
        matchC = 0;
        keyIndex++;
    }

    // if(strcmp(searchStr, lastStr) == 0){
    //     printf("HI FOLKS");
    // }

    // LOG("%s\n", searchStr);


    if(strcmp(searchStr, "") == 0){

        if(histIndex > 1){
            histIndex--;
        }
        if(histIndex <= 1){
            rl_replace_line(hist_search_cnum(1), 1);
        }else{
            rl_replace_line(hist_search_cnum(histIndex), 1);

        }
    }
    else{
        // if(matchC++ > matchSize){
        //     rl_replace_line(getMatch(matchSize), 1);
        // }else{
            rl_replace_line(getMatch(matchC++), 1);
        // }
    }

    // if(j < 0){
    //     j++;
    // }

    // if(j >= i){
    //     rl_replace_line(matches[j-1], 1);
    // }else{
    //     rl_replace_line(matches[j], 1);
    //     j++;

    // }

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: reverse history search

    return 0;
}

/**
    function that handles key down being presessed
*/
int key_down(int count, int key)
{
    /* Modify the command entry text: */

    if(strcmp(searchStr, "") == 0){
        if(histIndex < hist_last_cnum() +1){
            histIndex++;
        }
        if(histIndex >= (hist_last_cnum() +1)){
            rl_replace_line("", 1);
        }else{
            rl_replace_line(hist_search_cnum(histIndex), 1);

        }
    }else{

        // if(--matchC > 0){
        //     rl_replace_line("", 1);

        // }else{
        // matchC--;
            rl_replace_line(getMatch(--matchC), 1);
        // }
    }

    //  if(j > 0){
    //     j--;
    // }

    // if(j <= i){
    //     rl_replace_line(matches[j+1], 1);
    // }else{
    //     rl_replace_line(matches[j], 1);
    //     j--;
    // }


    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: forward history search

    return 0;
}

/**
    function that handles tab being presessed
*/
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

    path = getenv("PATH");


    if(path == NULL || strcmp(path, "") == 0){
        if(built_in_index < 4 && strncmp(text, built_in[built_in_index], strlen(text)) == 0){
            LOG("match: %s\n", built_in[built_in_index]);

            return strdup(built_in[built_in_index++]);
        }
        return NULL;    
    }
    if(state == 0){

        built_in_index = 0;

        next_tok = strdup(path);

    }
    struct dirent *enter;

        if (dummy != NULL){
            while((enter = readdir(dummy)) != NULL){
                if(strncmp(enter -> d_name, text, strlen(text)) == 0){
                    return strdup(enter -> d_name);
                }
            }
        }
        while((curr_tok = next_token(&next_tok, ":")) != NULL){
            dummy = opendir(curr_tok);


            if(dummy == NULL){
                continue;
            }
            while((enter = readdir(dummy)) != NULL){
                if(strncmp(enter -> d_name, text, strlen(text)) == 0){
                    return strdup(enter -> d_name);
                }
            }
    }
    while(built_in_index < 4){
        if(strncmp(text, built_in[built_in_index++], strlen(text)) == 0){
            return strdup(built_in[built_in_index - 1]);
        }
    }

    return NULL;
}
