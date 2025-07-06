/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:51:49 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/06 12:15:49 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../libft/libft.h"
# include <stdbool.h>
// # include "parser.h"

# define SUCCESS 0
# define FAILURE 1
# define SYNTAX_ERROR 2
# define CMD_NOT_FOUND 127
# define EXIT_NON_NUMERIC 255

extern int	g_exit_status;

/* ======================== PARSING STRUCTURES ======================== */

typedef struct s_env
{
	char			*key;      /* BOTH - parsed from envp, used in builtins */
	char			*value;    /* BOTH - parsed from envp, used in builtins */
	struct s_env	*next;     /* BOTH - linked list structure */
}	t_env;

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

typedef enum
{
	QUOTED,
	NOT_QUOTED
} IS_QUOTED;

typedef struct s_token
{
	TYPE_TOKEN type;
	char *value;
	IS_QUOTED is_quoted;
	struct s_token *next;
}	t_token;

typedef struct s_redirections
{
	char					*file;        /* BOTH - parsed, used in execution */
	TYPE_TOKEN					type;         /* BOTH - parsed, used in execution */
	struct s_redirections	*next;        /* BOTH - linked list structure */
}							t_redirections;

typedef struct s_command
{
	char					**args;            /* BOTH - parsed, used in execution */
	char					*here_doc_file;    /* BOTH - created in parsing, used in execution */
	bool				    is_heredoc;        /* PARSING - heredoc flag */
	char 					*del;              /* PARSING - heredoc delimiter */
	t_redirections			*redirections;     /* BOTH - parsed, used in execution */
	struct s_command		*next;             /* BOTH - linked list structure */
}							t_command;

/* ======================== EXECUTION STRUCTURES ======================== */

typedef struct s_pipe
{
	int			fd[2];      /* EXECUTION - pipe file descriptors */
	int			pre_fd;     /* EXECUTION - previous pipe fd */
	pid_t		pids[1024]; /* EXECUTION - child process IDs */
	int			i;          /* EXECUTION - process counter */
	t_command	*cur;       /* EXECUTION - current command */
	pid_t		pid;        /* EXECUTION - current process ID */
}           t_pipe;

typedef struct s_data
{
	t_env       *env;         /* BOTH - initialized in main, used in parsing (expansion) and execution (builtins) */
	t_command   *cmd;         /* BOTH - parsed commands, executed */
	t_pipe      *pipe;        /* EXECUTION - pipe management */
	pid_t       pid;          /* EXECUTION - process management */
	bool        is_child;     /* EXECUTION - child process flag */
}   t_data;

/* ======================== EXECUTION FUNCTIONS ======================== */

int     is_builtin(char *cmd);
int     ft_cd(t_data *data, char **args);
int     ft_echo(char **args);
int     ft_exit(t_data *data, char **args);
int     ft_pwd(t_data *data);
int     ft_env(t_data *data, char **args);
int     ft_export(t_data *data, char **args);
int    ft_unset(t_data *data, char **args);
int	handle_builtin(t_data *data);


int    executer(t_data *data);
char    *get_path(t_data *data, char *cmd);
int execute_builtin(t_data *data);
void    execute_pipe(t_data *data);
int	execute_single_command(t_data *data);
char	*get_command_path(t_data *data);

/* ======================== REDIRECTIONS FUNCTIONS ======================== */
int setup_redirections(t_command *cmd);
int is_empty_or_whitespace(char *str);
int check_input_redirections(t_command *cmd, int *has_input);

/* ======================== ENVIRONMENT FUNCTIONS ======================== */

void init_env(t_data *data, char **envp);
void update_env(t_env *env, const char *key, const char *new_value);
void update_or_add_env(t_env **env, const char *key, const char *new_value);
void add_env_node(t_env **env, t_env *new_node);
char *extract_key(char *str);
char *extract_value(char *str);
char *get_env_value(t_env *env, const char *key);
t_env *new_env_node(char *key, char *value);
void sort_and_print_env(t_env *env);
bool	is_valid_key(const char *key);
char **env_to_array(t_env *env);
int	env_size(t_env *env);


/* ======================== UTILITY FUNCTIONS ======================== */

void cleanup_child_resources(char *path, char **envp);
void parent_cleanup(int *pre_fd, int *fd, t_command **cmd);
void    clean_exit(t_data *data, int exit_code);
void safe_close(int fd);
void	*ft_malloc(size_t size, int mode);

/* ======================== SIGNAL HANDLERS ======================== */
void signal_parent_handler(void);
void signal_child_handler(void);

/* redirection fd helpers */
int  save_std_fd(int *saved_in, int *saved_out);
void reset_std_fd(int saved_in, int saved_out);

#endif