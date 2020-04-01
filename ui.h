/**
 * @file
 *
 * Text-based UI functionality. These functions are primarily concerned with
 * interacting with the readline library.
 */

#ifndef _UI_H_
#define _UI_H_

#include <sys/types.h>

/**
    function that handles key up being presessed
*/
void init_ui(void);
/**
    function that handles key up being presessed
*/
char *prompt_line1(void);
/**
    function that handles key up being presessed
*/
char *prompt_line2(void);
/**
    function that handles key up being presessed
*/
char *read_command(void);
/**
    function that handles key up being presessed
*/
char* set_status(pid_t);
#endif
