#ifndef MINISHELL_H
#define MINISHELL_H

#include "parser.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <fcntl.h>

#define SUCCESS 0

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

char	**ft_split(char *line);
int	is_builtin(char *cmd);
void execute_builtin(t_command *cmd);
void ex_cd_pwd(t_command *cmd);
void ex_echo_env(t_command *cmd);

#endif