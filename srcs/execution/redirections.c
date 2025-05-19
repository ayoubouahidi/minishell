#include "minishell.h"

static int open_input_file(char *filename)
{
	int fd;
	char *file = get_file_name(filename);

	fd = open(file, O_RDONLY);
	free(file);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(filename);
		exit(1);
	}
	return fd;
}

static int open_output_file(t_command *cmd)
{
	int fd;
	int flags = O_WRONLY | O_CREAT;
	char *file;

	if (cmd->is_append && cmd->appendfile)
		file = get_file_name(cmd->appendfile);
	else
		file = get_file_name(cmd->outfile);
    if (cmd->is_append)
        flags |= O_APPEND;
    else
	    flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	free(file);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd->is_append ? cmd->appendfile : cmd->outfile);
		exit(1);
	}
	return fd;
}

void setup_redirections(t_command *cmd)
{
	int fd;

	if (cmd->infile)
	{
		fd = open_input_file(cmd->infile);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->outfile || cmd->appendfile)
	{
		fd = open_output_file(cmd);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
    if (cmd->is_heredoc)
	{
		int heredoc_fd = setup_heredoc(cmd);
		if (heredoc_fd == -1)
			exit(1);
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}

}
