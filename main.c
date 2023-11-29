#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "parse.h"
#include "launch.h"


int main(int argc, char* argv[])
{
	char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN] = { 0 };

	const char* SIMPLE_CMD = " ls -l /root && ls /usr";
        printf("Simple cmd = %s\n", SIMPLE_CMD);

        int nb_tokens = tokenize(SIMPLE_CMD, tokens);
        if (nb_tokens == -1) {
		printf("Error while tokenizing:\n%s", SIMPLE_CMD);
		exit(1);
	}
            	CompoundCommand cmd;
		int consumed=parse_compound_command(tokens,nb_tokens,&cmd);
		if(consumed==-1){
			printf("error while parsing\n");
			exit(1);
		}

		int success=launch_compound_command(&cmd);
		if(success==-1){
			printf("error while launching\n");
			exit(1);
		}





return 0;
}
