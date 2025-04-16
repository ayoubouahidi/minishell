#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <fcntl.h>

#define SUCCESS 0

char	**ft_split(char *line);
int	is_builtin(char *cmd);
int	ft_cd(char **args);
void	ft_exit(void);


#endif