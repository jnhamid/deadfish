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
const char *hist_search_prefix(char *);
const char *hist_search_cnum(int);
unsigned int hist_last_cnum(void);
int getCount(void);



struct command
    {	
        char* realCommand;
        unsigned int cmdID;

    };

struct history_array
    {
        int head; 
        int tail;
        int limit;
        struct command *commands;

    };

#endif
