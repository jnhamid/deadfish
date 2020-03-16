/**
 * @file
 *
 * Text-based UI functionality. These functions are primarily concerned with
 * interacting with the readline library.
 */

#ifndef _UI_H_
#define _UI_H_

#include <sys/types.h>

void init_ui(void);
char *prompt_line1(void);
char *prompt_line2(void);
char *read_command(void);
char* set_status(pid_t);
#endif
