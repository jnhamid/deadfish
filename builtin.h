/**
 * @file
 *
 * Global builtin stuff
 */
#ifndef _BUILTIN_H_
#define _BUILTIN_H_


/**
	Struct for builtin in commands
*/
struct builtin_def{
	char name[128];
	int (*function)(int argc, char *argv[]);
};

/**
	Function for bank commands
*/
char* history_execution(int argc, char *argv[]);

/**
	function to handle builtin 
*/
int handle_builtin(int argc, char *argv[]);

#endif