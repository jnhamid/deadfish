#include "logger.h"
#include "history.h"
#include "builtin.h"
#include "next_token.h"

#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>



int change_dira(int argc, char *argv[]);
int exit_program(int argc, char *argv[]);
int history_print(int argc, char *argv[]);


struct builtin_def builtins[] = {
	{"cd", change_dira},
	{"exit", exit_program},
	{"history", history_print}
};

int change_dira(int argc, char *argv[]){
	struct passwd *pwd;

    uid_t uid = getuid();
    pwd = getpwuid(uid);

    LOG("%s\n", argv[0]);
    LOG("%s\n", argv[1]);


    if(argv[1] == NULL){
        chdir(pwd ->pw_dir);
    }else if(strstr(argv[1], "~")){
        chdir(pwd ->pw_dir);
    }
    else if(chdir(argv[1]) != 0){
        LOG("%s\n", "No such file or directory");
    }else{
        chdir(argv[1]);
    }
    return 1;
}
int history_print(int argc, char *argv[]){
	hist_print();
	return 1;
}

int exit_program(int argc, char *argv[]){
	hist_destroy();
	exit(0);
	return 1;
}

char *history_execution(int argc, char *argv[]){
	LOG("%s\n", argv[0]);

	char* command = argv[0];

	command++;

	LOG("%s\n",command);

	if(isdigit(command[0])){
		char* cmd;

		cmd = hist_search_cnum(atoi(command));

		LOG("%s\n", cmd);

		if(cmd == NULL){

		return NULL;
	}	
		char* dupp = strdup(cmd);
		execute(dupp);

		LOG("%s\n", cmd);

		
		return cmd;
		

		
	}else if(strcmp(command, "!") == 0){
		char* cmd;

		cmd = hist_search_cnum(hist_last_cnum());

		char* dupp = strdup(cmd);
		execute(dupp);

		return cmd;
		
	}else{
		char* cmd;

		cmd = hist_search_prefix(command);
		LOG("%s\n", cmd);
		if(cmd == NULL){
			return NULL;
		}

		char* dupp = strdup(cmd);
		execute(dupp);

		return cmd;
	}


	return NULL;
}




int handle_builtin(int argc, char *argv[]){
	LOG("%s\n", argv[0]);
	if(argv[0] == NULL){
		LOGP("Arg is null");
		return 1;
	}
	if(strncmp(argv[0], "!",1) == 0){
		history_execution(argc, argv);
	}
	for(int i = 0; i< sizeof(builtins)/sizeof(struct builtin_def); ++i){
		if(strcmp(argv[0], builtins[i].name) == 0){
			return builtins[i].function(argc, argv);
		}
	}
	return 1;
}










