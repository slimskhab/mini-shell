#include "launch.h"
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
// Execute a simple or compound command
// Returns 0 if there is no error, or -1 if there is an error
// It calls the other related launch_* functions
int launch_command(CommandSequence* command){
	int status;
	int result = 0;

    	for (int i = 0; i < command->nb_cmds; ++i) {
        	int error = launch_compound_command(&command->cmds[i]);
        	if (error == -1) {
        		result = -1;  // Set result to -1 if any command fails
        	}

        // If commands should be executed in parallel, don't wait
        	if (!command->in_parallel) {
            	// Wait for the completion of the last command
            		waitpid(-1, &status, 0);
        	}
    	}

	return result;
}
// Execute a compound command
// Returns 0 if there is no error, or -1 if there is an error
// It calls the other related launch_* functions
int launch_compound_command(CompoundCommand* command){
        int error1;
        int fd[2];
        if(command->op==OPERATOR_PIPE){
                pipe(fd);
                error1=launch_simple_command(&command->cmd1,-1,fd[1]);
                close(fd[1]);
        }
	else if (command->op == OPERATOR_REDIRECT) {
		pid_t pid = fork();
		if (pid == 0) {
			int file1_fd = open(command->cmd2.program_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        		if (file1_fd == -1) {
            			perror("open");
            			exit(EXIT_FAILURE);
        		}

	        	if (dup2(file1_fd, STDOUT_FILENO) == -1) {
        			perror("dup2");
            			close(file1_fd);
         	   		exit(EXIT_FAILURE);
        		}

   		     	close(file1_fd);
        		char *argv[100] = {command->cmd1.program_name, NULL};

       			for (int i = 0; i < command->cmd1.nb_args; i++) {
       				argv[i + 1] = command->cmd1.args[i];
       			}
       			argv[command->cmd1.nb_args + 1] = NULL;

			execvp(command->cmd1.program_name, argv);

			perror("execvp");
			exit(EXIT_FAILURE);
		}

		int status;
		waitpid(pid, &status, 0);

    		if (WIFEXITED(status)) {
        		return WEXITSTATUS(status) ? -1 : 0;
		} else {
        		return -1;
		}
	}
        else{
                error1=launch_simple_command(&command->cmd1,-1,-1); //-1 no read and no write
        }

        if(command->op==OPERATOR_NONE){
                return error1;
        }
        else if(command->op==OPERATOR_AND){
                if(error1==0){
                        int error2=launch_simple_command(&command->cmd2,-1,-1);
                        return error2;
                }else{
                        return error1;
                }
        }else if(command->op==OPERATOR_OR){
                if(error1==0){
                        return error1;
                }else{
                        int error2=launch_simple_command(&command->cmd2,-1,-1);
                        return error2;
                }

        }
        else if (command->op==OPERATOR_PIPE){
                int error2=launch_simple_command(&command->cmd2,fd[0],-1);
                close(fd[0]);
                return error2;

        }


return -1;
}
// Execute a simple command
// Returns 0 if there is no error, or -1 if there is an error
int launch_simple_command(SimpleCommand* command, int read_pipe,int write_pipe){

        pid_t pid = fork();

        if(pid == 0){
                char* argv[100]={command->program_name,NULL};

                for(int i=0;i<command->nb_args;i++){
                        argv[i+1]=command->args[i];
                }
                argv[command->nb_args+1] = NULL;

                if(read_pipe != -1){
                        dup2(read_pipe,0);
                }

                if(write_pipe != -1){
                        dup2(write_pipe,1);
                }



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

