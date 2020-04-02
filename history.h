/**
 * @file
 *
 * Contains shell history data structures and retrieval functions.
 */

#ifndef _HISTORY_H_
#define _HISTORY_H_

void hist_init(unsigned int);
void hist_destroy(void);
void hist_add(const char *);
void hist_print(void);
char *hist_search_prefix(char *);
char *hist_search_cnum(int);
unsigned int hist_last_cnum(void);
int getCount(void);
int hist_search_prefix_to_num(char *);

char* getMatch(int);



/**
    a struct for a command in history
*/
struct command
    {	
        char* realCommand;
        unsigned int cmdID;

    };


/**
    array for history
*/
struct history_array
    {
        int head; 
        int tail;
        int limit;
        struct command *commands;

    };

#endif
