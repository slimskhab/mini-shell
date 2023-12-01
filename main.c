#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "parse.h"
#include "launch.h"
#include "batch.h"
#include "interactive.h"

int main(int argc, char* argv[])
{

	if(argc<2){
		interactive_mode();
	}else{
		for(int i=1;i<argc;i++){
			batch_mode(argv[i]);
		}
	}

	return 0;
}
