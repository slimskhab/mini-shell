#include "parse.h"
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>



int tokenize(char* input, char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN]) {
        int token_count = 0;
        int token_index = 0;
        int in_token = 0;
        for (int i = 0; input[i] != '\0'; i++) {
                char current_char = input[i];

                if (isspace(current_char) || current_char == '\t') {
                        if (in_token) {
                                tokens[token_count][token_index] = '\0';
                                token_count++;
                                token_index = 0;
                                in_token = 0;
                        }
                } else {
                        tokens[token_count][token_index] = current_char;
                        token_index++;
                        in_token = 1;
                }
        }
        if (in_token) {
                tokens[token_count][token_index] = '\0';
                token_count++;
        }
        return token_count;
}

int parse_simple_command(
    char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN],
    int nb_tokens,
    struct SimpleCommand* command
) {

    int cmd_index = 0;

    for (int i = 0; i < nb_tokens; i++) {
        if ((strcmp(tokens[i], "&&") == 0)||(strcmp(tokens[i], "||") == 0)||(strcmp(tokens[i], "|") == 0)||(strcmp(tokens[i], "&") == 0) ) {
            break;
        }

        if (i == 0) {
            strncpy(command->program_name, tokens[i], MAX_TOKEN_LEN);
            command->program_name[MAX_TOKEN_LEN - 1] = '\0';
        } else {
            strncpy(command->args[cmd_index], tokens[i], MAX_TOKEN_LEN);
            command->args[cmd_index][MAX_TOKEN_LEN - 1] = '\0';
            cmd_index++;
        }
    }

    return cmd_index+1;
}


int parse_compound_command(char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN], int nb_tokens, struct CompoundCommand* command) {
    int consumed_tokens = 0;
    int cmd1_index = 0;
    bool is_cmd2_started = false;

    for (int i = 0; i < nb_tokens; i++) {
        if ((strcmp(tokens[i], "&&") == 0) || (strcmp(tokens[i], "||") == 0)) {
            if (strcmp(tokens[i], "&&") == 0) {
                command->op = OPERATOR_AND;

            } else if (strcmp(tokens[i], "||") == 0) {
                command->op = OPERATOR_OR;
            }
            is_cmd2_started = true;
            continue;
        }

        if (is_cmd2_started) {
            consumed_tokens = parse_simple_command(&tokens[i], nb_tokens - i, &command->cmd2);
            if (consumed_tokens == 0) {
                return -1;
            }
            i += consumed_tokens - 1;
        } else {
            consumed_tokens = parse_simple_command(&tokens[i], nb_tokens - i, &command->cmd1);
            if (consumed_tokens == 0) {
                return -1;
            }
            i += consumed_tokens - 1;
        }
    }

    if (!is_cmd2_started) {
        command->op = OPERATOR_NONE;
    }

    return consumed_tokens+1;
}

int parse(char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN], int nb_tokens, CommandSequence* command) {
    int consumed_tokens = 0;
    int cmd_index = 0;
    int number_of_consumed_tokens=0;
    int i = 0;
    for ( i = 0; i < nb_tokens; i++) {
        if (strcmp(tokens[i], ";") == 0) {
            if (cmd_index >= MAX_NB_CMDS) {
                return -1;
            }

            parse_compound_command(tokens, i, &command->cmds[cmd_index]);
            number_of_consumed_tokens=i+1;
            cmd_index++;
        }
    }

    parse_compound_command(tokens + number_of_consumed_tokens, nb_tokens-number_of_consumed_tokens, &command->cmds[cmd_index]);

    if (cmd_index >= MAX_NB_CMDS) {
        return -1;
    }

    if (i < nb_tokens) {
        if (parse_compound_command(tokens + i, nb_tokens - i, &command->cmds[cmd_index]) != 0) {
            return -1;
        }
        cmd_index++;
    }

    if (cmd_index == 0) {
        command->nb_cmds=1;
        return parse_compound_command(tokens, nb_tokens, &command->cmds[0]);
    }

    command->nb_cmds = cmd_index+1;
    command->in_parallel = false;

    return nb_tokens;
}
