#include "interactive.h"
#include <stdio.h>
#include "launch.h"
#include "parse.h"
#include <stdlib.h>
#include <string.h>
int interactive_mode(){
	while (1) {
        char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN] = {0};
        char input[50];

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }


        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        printf("Simple cmd = %s\n", input);
	    FILE* file = fopen("history.txt", "a");

    if (file != NULL) {
        fprintf(file, "%s\n", input);
        fclose(file);
    } else {
        perror("Error opening history file");
    }
        int nb_tokens = tokenize(input, tokens);
        if (nb_tokens == -1) {
            printf("Error while tokenizing:\n%s", input);
            exit(1);
        }

        CommandSequence cmd;
        int consumed = parse(tokens, nb_tokens, &cmd);
        if (consumed == -1) {
            printf("Error while parsing\n");
            exit(1);
        }

        int success = launch_command(&cmd);
        if (success == -1) {
            printf("Error while launching\n");
            exit(1);
        }
    }
}
