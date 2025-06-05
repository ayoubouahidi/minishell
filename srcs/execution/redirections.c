#include "../../includes/minishell.h"

static int set_output_flags(t_command *cmd)
{
	if (cmd->is_append)
		return (O_WRONLY | O_CREAT | O_APPEND);
	return (O_WRONLY | O_CREAT | O_TRUNC);
}

static int handle_open_errors(t_command *cmd, int is_builtin, 
							int *has_return, int *exit_status)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(cmd->outfile ? cmd->outfile : cmd->infile);
	if (is_builtin && exit_status)
		*exit_status = 1;
	if (has_return && !is_builtin)
		*has_return = -2;
	return (-1);
}

int redirect_output_to_file(t_command *cmd, int is_builtin,
							int *exit_status, int *has_return)
{
	int		fd;
	char	*file;

	file = cmd->outfile ? cmd->outfile : cmd->appendfile;
	if (has_return && !is_builtin)
		*has_return = -2;
	if (!file || !file[0])
		return (-1);
	fd = open(file, set_output_flags(cmd), 0644);
	if (fd < 0)
		return (handle_open_errors(cmd, is_builtin, has_return, exit_status));
	if (is_builtin)
		*has_return = -1;
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int redirect_input_to_file(t_command *cmd, int is_builtin,
						int *exit_status, int *has_return)
{
	int		fd;
	char	*file;

	file = cmd->infile;
	if (has_return && !is_builtin)
		*has_return = -2;
	if (!file || !file[0])
		return (-1);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (handle_open_errors(cmd, is_builtin, has_return, exit_status));
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

void redirect_input_to_pipe(int read_pipe_end)
{
	dup2(read_pipe_end, STDIN_FILENO);
	close(read_pipe_end);
}

void redirect_output_to_pipe(int write_pipe_end)
{
	dup2(write_pipe_end, STDOUT_FILENO);
	close(write_pipe_end);
}

void setup_redirections(t_command *cmd)
{
	// Call this ONLY in the child process, after fork, before execve!
	if (!cmd)
		return;
	if (cmd->infile)
	{
		if (redirect_input_to_file(cmd, 0, NULL, NULL) == -1)
			exit(EXIT_FAILURE);
	}
	if (cmd->outfile || (cmd->is_append && cmd->appendfile))
	{
		if (redirect_output_to_file(cmd, 0, NULL, NULL) == -1)
			exit(EXIT_FAILURE);
	}
}


