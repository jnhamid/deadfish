/**
 * @file
 *
 * Global stuff for next_token
 */
#ifndef _NEXT_TOKEN_H_
#define _NEXT_TOKEN_H_


/**
   function that gets next token
*/
char *next_token(char **, const char *);


/**
   function that replaces a str
*/
char *replaceWord(const char *, const char *, const char*);


/**
   function that runs jobs
*/
void execute(char* command);

#endif
