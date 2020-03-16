#ifndef _BUILTIN_H_
#define _BUILTIN_H_

struct builtin_def{
	char name[128];
	int (*function)(int argc, char *argv[]);
};

int handle_builtin(int argc, char *argv[]);

#endif