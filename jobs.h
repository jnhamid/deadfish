#ifndef _JOBS_H_
#define _JOBS_H_

#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

struct jobs_def{
	char *name;
	pid_t pid;
};

void sigchild_handler(int signo);

void job_add(char*, pid_t);

void print_jobs();


#endif