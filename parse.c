#include "parse.h"
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
int tokenize(char* input, char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN]) {

}

int parse_simple_command(
    char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN],
    int nb_tokens,
    struct SimpleCommand* command
) {

    
}


int parse_compound_command(char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN], int nb_tokens, struct CompoundCommand* command) {
    
}

int parse(char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN], int nb_tokens, CommandSequence* command) {
   
}
