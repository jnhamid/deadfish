#include <string.h>
#include <stdlib.h>
 #include <sys/wait.h>

#include "logger.h"
#include "jobs.h"
#include "ui.h"

int jobsCounter = 0;

struct jobs_def jobsStruct[10] = {0};


void job_add(char *name, pid_t pid){

	if(jobsCounter == 10){
		return;
	}

	for(int i = 0; i < 10; i++){
		if (jobsStruct[i].pid < 1)
		{
			jobsStruct[i].pid = pid;
			jobsStruct[i].name = strdup(name); 
			break;
		}

	}
	jobsCounter++;
}

void removejob(pid_t pid){
	for(int i =0; i< 10; i++){
		if(jobsStruct[i].pid == pid || pid == -1){
	        jobsStruct[i].pid = -1;
	        free(jobsStruct[i].name);
	        jobsCounter--;
	    }  
 	}
 	return;
}


void print_jobs(){
	for(int i = 0; i < 10; i++){
		if(jobsStruct[i].pid > 0){
		printf("%d %s\n", jobsStruct[i].pid, jobsStruct[i].name);

		}
	}

}
