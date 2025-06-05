#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <errno.h>
# include <signal.h>
# include <sys/stat.h>
# include "../libft/libft.h"

# define SUCCESS 0
# define FAILURE 1
# define SYNTAX_ERROR 2
# define CMD_NOT_FOUND 127
# define EXIT_NON_NUMERIC 255

extern int g_exit_status;

// Environment structure - must be defined first
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

// Token and parsing types
typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
}							t_token_type;

typedef enum s_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}							t_quote_type;

typedef struct s_redirections
{
	char					*file;
	t_token_type			type;
	t_quote_type			quote_type;
	struct s_redirections	*next;
}							t_redirections;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	t_quote_type			quote_type;
	struct s_token			*next;
}							t_token;

typedef struct s_command
{
	char					**args;
	char					*infile;
	char					*outfile;
	bool					is_append;
	bool					is_heredoc;
	char					*del;
	char					*appendfile;
	int						exit_status;
	int						append;
	int						heredoc;
	int						signal_detected;
	char					*here_doc_file;
	t_quote_type			quote_type;
	t_redirections			*redirections;
	struct s_command		*next;
}							t_command;

typedef struct s_gc
{
	void					*ptr;
	struct s_gc				*next;
}							t_gc;

typedef struct s_exec_env
{
	int						is_created;
	char					**env;
}							t_exec_env;

typedef struct s_pipe
{
    int			fd[2];
	int			pre_fd;
	pid_t		pids[1024];
	int			i;
	t_command	*cur;
	pid_t		pid;
}           t_pipe;

typedef struct s_data
{
    t_env       *env;
    t_command   *cmd;
    int         exit_status;
    t_pipe      *pipe;
    pid_t       pid;
    bool        is_child;
}   t_data;

// Function declarations
int							is_only_spaces(char *input);
t_token						*tokenize_input(char *input);
char						*get_operator(char *input, int *i,
								t_quote_type *quote_type);
char						*get_word(char *input, int *i,
								t_quote_type *quote_type);
int							ft_strncmp(const char *s1, const char *s2,
								size_t n);
t_token_type				get_token_type(char *value);
char						*process_input(char *input, int *i,
								t_quote_type *quote_type);
t_command					*parse_tokens(t_token *tokens);
int							check_cmds(t_token *tokens);
char						**ft_realloc_array(char **arr, char *new_str);
void						expand_variables_and_remove_quotes(t_token *tokens,
								t_env *env);
char						*ft_strjoin_char(char *str, char c);
char						*case_of_squote(char *word, int *i, char *result);
char						*case_of_dquote(char *word, int *i, char *result,
								t_env *env);
char						*case_of_normal_var_with_dquotes(char *word, int *i,
								char *result, t_env *env);
char						*case_of_normal_var(char *word, int *i,
								char *result, t_env *env);
char						*case_of_var_with_next_char_squote(char *word,
								int *i, char *result);
char						*case_of_var_with_next_char_dquote(char *word,
								int *i, char *result);
char						*case_of_var_with_next_char_digit(char *word,
								int *i, char *result);
char						*case_of_var_with_exit_status(int *i, char *result);
char						*case_of_word(char *word, int *i, char *result);
int							heredoc(t_command *cmds, t_env *env);
int							count_here_doc(t_command *cmds, char ***files);
int							count_redirections(t_command *cmds);
char						*expand_the_heredoc(char *input_heredoc,
								t_env *env);
void						ign_ctrl_c_with_exit_status(int pid, int *status,
								int *signal_detected);
void						unlink_files(int total_here_doc, char **files);
void						*ft_malloc(size_t size, int mode);
void						print_error(char *msg);

/* Builtins */
int is_builtin(char *cmd);
int     ft_cd(t_data *data, char **args);
int     ft_echo(char **args);
int     ft_exit(t_data *data, char **args);
int     ft_pwd(t_data *data);
int     ft_env(t_data *data, char **args);
int     ft_export(t_data *data, char **args);
int    ft_unset(t_data *data, char **args);

/* Execution */
void    executer(t_data *data, char **envp);
void    execute_external(t_data *data);
char    *get_path(t_data *data, char *cmd);
int execute_builtin(t_data *data);
void    execute_pipe(t_data *data);
int	execute_single_command(t_data *data);

/* pipe && redirections*/
char *get_file_name(char *files);
void setup_redirections(t_command *cmd);
void init_pipe_struct(t_data *data);
int setup_heredoc(t_command *cmd);

/* Environment */
void init_env(t_data *data, char **envp);
void update_env(t_env *env, const char *key, const char *new_value);
void add_env_node(t_env **env, t_env *new_node);
char *extract_key(char *str);
char *extract_value(char *str);
char *get_env_value(t_env *env, const char *key);
t_env *new_env_node(char *key, char *value);
void sort_and_print_env(t_env *env);
bool	is_valid_key(const char *key);
char **env_to_array(t_env *env);

/* Parsing */
t_command   *parse_line(char *line);
t_command   *parcer(char *line);
char    **split_pipes(const char *input);

/* Utility functions from old parsing system */
char *to_arg(t_token *token, char *args);
bool heredoc_check_append(t_token *token, char **del);
bool match_file(char *filename);
char *get_tmp_file(void);

/* Memory */
void	free_all_and_exit(t_data *data, int exit_code);
void cleanup_child_resources(char *path, char **envp);
void    free_env(t_env *env);
void free_cmd(t_command *cmd);
void    free_array(char **array);
void    free_data(t_data *data);
void    clean_exit(t_data *data, int exit_code);

/* Additional token types for old parsing system compatibility */
typedef enum
{
   UNCLOSED_DOUBLE_Q,
   UNCLODES_SINFGLE_Q 
} e_syntaxe;

typedef enum
{
	INTPUT_RED,
	OUTPUT_RED,
	HEREDOC,
	APPEND,
	PIPE,
	WORD,
	ENDF
} TYPE_TOKEN;

#endif