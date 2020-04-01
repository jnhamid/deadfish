/**
 * @file
 *
 * Global stuff for jobs
 */
#ifndef _JOBS_H_
#define _JOBS_H_

#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


/**
    jobs Struct
*/
struct jobs_def{
	char *name;
	pid_t pid;
};



/**
   function that adds jobs
*/
void job_add(char*, pid_t);



/**
   function that prints jobs
*/
void print_jobs();

/**
   function that removes jobs
*/
void removejob(pid_t pid);



#endif