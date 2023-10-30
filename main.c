#include <stdbool.h>
#include "parse.c"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main() {
    char input[50];

    printf("Suggested commands to try:\n");
    printf("Simple command: ls -l\n");
    printf("Compound command (without args): ls -l && pwd\n");
    printf("Compound command (with args): ls -l && pwd -a\n");
    printf("Command sequence with simple commands (without args): ls ; pwd\n");
    printf("Command sequence with simple commands (with args in 1): ls -l ; pwd\n");
    printf("Command sequence with simple commands (with args in both): ls -l ; pwd -a\n");
    printf("Command sequence with compound command and simple command (with args in 1): ls -l && hostname ; pwd -a\n");
    printf("Command sequence with compound commands (with args): ls -l && hostname ; pwd -a && mkdir file\n");

    while (1) {
        gets(input);

        char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN];
        int num_tokens = tokenize(input, tokens);
        CommandSequence command;

        if (num_tokens == -1) {
            printf("Error tokenizing the input.\n");
        } else {
            printf("Tokens:\n");
            for (int i = 0; i < num_tokens; i++) {
                printf("Token %d: %s\n", i + 1, tokens[i]);
            }

            int consumed_tokens = parse(tokens, num_tokens, &command);
            if (consumed_tokens == -1) {
                printf("Failed to parse CommandSequence.\n");
            } else {
                printf("Parsed CommandSequence:\n");
                printf("Number of compound commands is %d\n", command.nb_cmds);
                for (int i = 0; i < command.nb_cmds; i++) {
                    printf("\n*** Compound Command %d:\n", i + 1);
                    printf("**Command 1: %s\n", command.cmds[i].cmd1.program_name);
                    for (int j = 0; command.cmds[i].cmd1.args[j][0] != '\0'; j++) {
                        printf("*Argument %d: %s\n", j + 1, command.cmds[i].cmd1.args[j]);
                    }
                    printf("Command Operator: %s\n", (command.cmds[i].op == OPERATOR_AND) ? "&&" : (command.cmds[i].op == OPERATOR_OR) ? "||" : "none");

                    if (command.cmds[i].op != OPERATOR_NONE) {
                        printf("**Command 2: %s\n", command.cmds[i].cmd2.program_name);
                        for (int j = 0; command.cmds[i].cmd2.args[j][0] != '\0'; j++) {
                            printf("*Argument %d: %s\n", j + 1, command.cmds[i].cmd2.args[j]);
                        }
                    }
                }
                if (command.in_parallel) {
                    printf("Commands are in parallel.\n");
                }
            }
        }
    }

    return 0;
}

