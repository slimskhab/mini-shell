#include "batch.h"
#include <stdio.h>
#include "launch.h"
#include "parse.h"
#include <stdlib.h>
#include <string.h>

int batch_mode(char* file_name){
	FILE* file = fopen(file_name, "r");

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[50];

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove the newline character at the end of the line
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "exit") == 0) {
            break; // Exit the loop if the command is "exit"
        }

        printf("Executing command: %s\n", line);

        // Tokenize, parse, and execute the command
        char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN] = {0};
        int nb_tokens = tokenize(line, tokens);
        if (nb_tokens == -1) {
            printf("Error while tokenizing:\n%s", line);
            fclose(file);
            return -1;
        }

        CompoundCommand cmd;
        int consumed = parse_compound_command(tokens, nb_tokens, &cmd);
        if (consumed == -1) {
            printf("Error while parsing\n");
            fclose(file);
            return -1;
        }

        int success = launch_compound_command(&cmd);
        if (success == -1) {
            printf("Error while launching\n");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}
