#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>  
#include <readline/history.h>
#include <unistd.h>
#include "minishell.h"

#define WHITESPACE " \t\n\r\f\v"

// typedef struct s_command
// {
//     // char *cmd;
//     char	**args;
// 	char	*infile;
// 	char	*outfile;
// 	bool	is_append;
// 	bool	is_heredoc;
//     struct s_command *next;
// } t_command;
// typedef struct s_command
// {
// 	char	**args;
// 	char	*infile;
// 	char	*outfile;
// 	bool	is_append;
// 	bool	is_heredoc;
// 	char	*del;
// 	char	*appendfile;
// 	struct s_command	*next;
// }				t_command;

typedef enum
{
   UNCLOSED_DOUBLE_Q,
   UNCLODES_SINFGLE_Q 
} e_syntaxe;

// tokenation





typedef struct s_lexer
{
	char c;
	unsigned int i;
	char *content;
}	t_lexer;



// a revoire : lexer == '\t' || lexer == '\n' || lexer == '\r' || lexer == '\f' || lexer == '\v'

int is_del(int flag);
t_command *parse_line(char *line);
void free_commands(t_command *cmd);
t_command	*parcer(char *line, t_env *envp);
char	*ft_strtrim(char const *s1, char const *set);
bool    syntaxe_error(char *str);

char	*expanation_token_env_var(char *str,  t_env *envp);
void	expantion_remove_quotes(t_token *token, t_env *envp);
char **newsplit(const char *input);
int run_heredoc(t_command *cmd, t_env *env);
char	*case_word(char	*result, int *i, char *final);
char *normal_var(int *i, char *result, t_env *envp, char *final);
char *next_char_digits(char  *result, int *i, char *final);
char *handle_exit_code(int *i, char *final);

#endif