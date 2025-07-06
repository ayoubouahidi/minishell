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
int	is_del(int flag);
t_token	*tokenize_input(char *line, t_env *envp);
bool	validate_pipe_parse(t_token *token);
t_command	*parser_commande(t_token **tokendd);
bool	process_redirection(t_token *token, t_command *cmd,
		t_redirections **red_head);
void	process_word_token(t_token *token, char ***args, bool *in_red);
t_command	*init_command(void);
void	ft_lstadd_back_red(t_redirections **lst, t_redirections *new);
t_redirections	*creat_red(char *file, TYPE_TOKEN type);
bool	heredoc_check_append(t_token *token, char **del);
void	print_syntax_error(t_token *token);
bool	match_file(char *file);
char	**to_arg(t_token *token, char **arg);
int	len_of_2d_array(char **args);
t_token	*tokenize(t_lexer *lexer);
t_token	*check_append(t_lexer *lexer);
t_token	*chech_herdoc(t_lexer *lexer);
char	*tostr(char c);
t_token	*string_process(t_lexer *lexer);
void	handle_quotes(char c, int *flag_single, int *flag_double);
bool	is_token_delimiter(char c, int flag_single, int flag_double);
void	increment_using_index(t_lexer *lexer);
t_lexer	*creat_lexer(char *content);
t_token	*creat_token(TYPE_TOKEN type, char *value, IS_QUOTED is_quoted);
bool	syntaxe_error(char *str);
IS_QUOTED	is_token_quoted(const char *str, int len);
int	ft_lstsize_parce(t_command *lst);
void	ft_lstadd_back_cmd(t_command **lst, t_command *new);
void	ft_lstadd_back_token(t_token **lst, t_token *new);
t_command	*ft_lstnew_cmd(void *content);
char	*join_char(char *str, char c);
char	*squotes_expand(int *i, char *result, char *final);
char	*double_quotes_expand(int *i, char *result, t_env *envp, char *final);
char	*next_char_squotes(char *result, int *i, char *final);
char	*next_char_dquotes(char *result, int *i, char *final);
char	*expand_process(int *i, char *result, t_env *envp, char *final);
char	*expanation_token_env_var(char *str, t_env *envp);
void	expantion_remove_quotes(t_token *token, t_env *envp);
int	is_inside_child(int flag);
void	here_doc_signal(int signum);
bool	check_quotes(char *del);
char	*expand_here_doc(char *line_heredoc, t_env *env);
void	handle_child_process(char *filename, char *del, t_env *env);
char	*randome_generate(void);
void	heredocprocess(t_command *cmd, t_env *env);
t_command	*init_and_validate_command(t_token **tokendd);

#endif