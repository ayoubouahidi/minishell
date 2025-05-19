
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

#include "libft.h"
#include "parser.h"


# define SUCCESS 0
# define FAILURE 1
# define SYNTAX_ERROR 2
# define CMD_NOT_FOUND 127
# define EXIT_NON_NUMERIC 255

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;


// typedef struct s_command
// {
//     char **args;
// 	char **infile;
// 	char **outfile;
//     bool            is_append;
//     bool            is_heredoc;
//     char *del;
//     char **appendfile;
//     struct s_command *next;
// } t_command;

typedef struct s_pipe
{

    int			fd[2];
	int			pre_fd;
	pid_t		pids[1024];
	int			i;
	t_command	*current;
	pid_t		pid;
}           t_pipe;
typedef struct s_data
{
    t_env       *env;
    t_command   *cmd;
    int         exit_status;
    t_pipe      *pipe;
    pid_t       pid;
    t_pipe      *pipe;
    bool        is_child;
}   t_data;

/* Builtins */
int is_builtin(char *cmd);
int     ft_cd(t_data *data, char **args);
int     ft_echo(t_data *data, char **args);
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
char    **split_pipes(const char *input);

/* Memory */
void	free_all_and_exit(t_data *data, int exit_code);
void cleanup_child_resources(char *path, char **envp);


// void    free_cmd(t_command *cmd);
void    free_env(t_env *env);
void free_cmd(t_command *cmd);
void    free_array(char **array);
void    free_data(t_data *data);
void    clean_exit(t_data *data, int exit_code);
void    free_array(char **array);


#endif