/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:51:49 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/12 09:41:23 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/stat.h>

# define SUCCESS 0
# define FAILURE 1
# define SYNTAX_ERROR 2
# define CMD_NOT_FOUND 127
# define EXIT_NON_NUMERIC 255

extern int					g_exit_status;

/* ======================== PARSING STRUCTURES ======================== */

typedef struct s_env
{
	char					*key;
	char					*value;
	struct s_env			*next;
}							t_env;

typedef enum token_type
{
	INTPUT_RED,
	OUTPUT_RED,
	HEREDOC,
	APPEND,
	PIPE,
	WORD,
	ENDF
}							t_TYPE_TOKEN;

typedef enum is_quoted
{
	QUOTED,
	NOT_QUOTED
}							t_IS_QUOTED;

typedef struct s_token
{
	t_TYPE_TOKEN			type;
	char					*value;
	t_IS_QUOTED				is_quoted;
	struct s_token			*next;
}							t_token;

typedef struct s_redirections
{
	char					*file;
	t_TYPE_TOKEN			type;
	struct s_redirections	*next;
}							t_redirections;

typedef struct s_command
{
	char					**args;
	char					*here_doc_file;
	bool					is_heredoc;
	char					**heredoc_delimiters;
	bool					*heredoc_quoted;
	int						heredoc_count;
	t_redirections			*redirections;
	struct s_command		*next;
}							t_command;

/* ======================== EXECUTION STRUCTURES ======================== */

typedef struct s_pipe
{
	int						fd[2];
	int						pre_fd;
	pid_t					pids[1024];
	int						i;
	t_command				*cur;
	pid_t					pid;
}							t_pipe;

typedef struct s_data
{
	t_env					*env;
	t_command				*cmd;
	t_pipe					*pipe;
	pid_t					pid;
	bool					is_child;
}							t_data;

/* ======================== EXECUTION FUNCTIONS ======================== */

int							is_builtin(char *cmd);
int							ft_cd(t_data *data, char **args);
int							ft_echo(char **args);
int							ft_exit(t_data *data, char **args);
int							ft_pwd(t_data *data);
int							ft_env(t_data *data, char **args);
int							ft_export(t_data *data, char **args);
int							ft_unset(t_data *data, char **args);
int							handle_builtin(t_data *data, t_command *cmd);
int							handle_single_cmd(t_data *data);
int							launch_external_command(t_data *data);

int							executer(t_data *data);
char						*get_path(t_data *data, char *cmd);
int							execute_builtin(t_data *data, t_command *cmd);
void						execute_pipe(t_data *data);
int							execute_single_command(t_data *data);
char						*get_command_path(t_data *data);
int							handle_cmd_not_found(t_data *data);

/* ======================== REDIRECTIONS FUNCTIONS ======================== */
int							setup_redirections(t_command *cmd);
int							is_empty_or_whitespace(char *str);

/* ======================== ENVIRONMENT FUNCTIONS ======================== */
void						init_env(t_data *data, char **envp);
void						update_env(t_env *env, const char *key,
								const char *new_value);
void						update_or_add_env(t_env **env, const char *key,
								const char *new_value);
void						add_env_node(t_env **env, t_env *new_node);
char						*extract_key(char *str);
char						*extract_value(char *str);
char						*get_env_value(t_env *env, const char *key);
t_env						*new_env_node(char *key, char *value);
void						sort_and_print_env(t_env *env);
bool						is_valid_key(const char *key);
char						**env_to_array(t_env *env);
int							env_size(t_env *env);

/* ======================== UTILITY FUNCTIONS ======================== */
void						parent_cleanup(int *pre_fd, int *fd,
								t_command **cmd);
void						clean_exit(t_data *data, int exit_code);
void						safe_close(int fd);
void						*ft_malloc(size_t size, int mode);

/* ======================== SIGNAL HANDLERS ======================== */
void						signal_parent_handler(void);
void						signal_child_handler(void);

/* redirection fd helpers */
int							save_std_fd(int *saved_in, int *saved_out);
void						reset_std_fd(int saved_in, int saved_out);

int							run_heredoc(t_command *cmd, t_env *env);

#endif
