#include "launch.h"
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

// Execute a simple or compound command
// Returns 0 if there is no error, or -1 if there is an error
// It calls the other related launch_* functions
int launch_command(CommandSequence* command){





return -1;
}
// Execute a compound command
// Returns 0 if there is no error, or -1 if there is an error
// It calls the other related launch_* functions
int launch_compound_command(CompoundCommand* command){

	int error1=launch_simple_command(&command->cmd1);
	if(command->op==OPERATOR_NONE){
		return error1;
	}
	else if(command->op==OPERATOR_AND){
		if(error1==0){
			int error2=launch_simple_command(&command->cmd2);
			return error2;
		}else{
			return error1;
		}
	}else if(command->op==OPERATOR_OR){
		if(error1==0){
			return error1;
		}else{
			int error2=launch_simple_command(&command->cmd2);
                        return error2;
		}

	}



return -1;
}
// Execute a simple command
// Returns 0 if there is no error, or -1 if there is an error
int launch_simple_command(SimpleCommand* command){

	pid_t pid = fork();

	if(pid == 0){
		char* argv[100]={command->program_name,NULL};

		for(int i=0;i<command->nb_args;i++){
			argv[i+1]=command->args[i];
		}

		argv[command->nb_args+1] = NULL;
		execvp(command->program_name,argv);

	}

	int status;
	waitpid(pid,&status,0);
	if(WIFEXITED(status)){
		return WEXITSTATUS(status)? -1 : 0;

	}else{
		return -1;
	}

}
