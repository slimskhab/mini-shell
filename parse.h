#define MAX_NB_TOKENS 1000
#define MAX_TOKEN_LEN 50
#define MAX_NB_ARGS 10
#define MAX_NB_CMDS 10
#define MAX_ARG_LEN 20
#include <stdbool.h>

struct SimpleCommand
{
 char program_name[MAX_TOKEN_LEN];
 char args[MAX_ARG_LEN][MAX_TOKEN_LEN];
};
typedef struct SimpleCommand SimpleCommand;
typedef enum CommandOperator
{
 OPERATOR_AND,
 OPERATOR_OR,
 OPERATOR_NONE,
} CommandOperator;
struct CompoundCommand
{
 SimpleCommand cmd1;
 SimpleCommand cmd2;
 CommandOperator op;
};
typedef struct CompoundCommand CompoundCommand;
struct CommandSequence
{
 CompoundCommand cmds[MAX_NB_CMDS];
 int nb_cmds;
 bool in_parallel;
};
typedef struct CommandSequence CommandSequence;
