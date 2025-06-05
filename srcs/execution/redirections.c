#include "../../includes/minishell.h"

static int handle_input_redirection(t_redirections *redir)
{
	int fd;
	
	if (redir->type == TOKEN_HEREDOC)
	{
		return (0);
	}
	
	// Check if file is valid
	if (!redir->file || !redir->file[0])
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (-1);
	}
	
	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(redir->file);
		return (-1);
	}
	
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

static int handle_output_redirection(t_redirections *redir)
{
	int fd;
	int flags;
	
	// Check if file is valid
	if (!redir->file || !redir->file[0])
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (-1);
	}
	
	if (redir->type == TOKEN_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	
	fd = open(redir->file, flags, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(redir->file);
		return (-1);
	}
	
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

static int handle_heredoc_redirection(t_command *cmd)
{
	int fd;
	
	if (!cmd->here_doc_file)
		return (0);
		
	fd = open(cmd->here_doc_file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd->here_doc_file);
		return (-1);
	}
	
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

void setup_redirections(t_command *cmd)
{
	t_redirections *redir;

	if (!cmd || !cmd->redirections)
		return;
	
	// Process all redirections in order
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_IN)
		{
			if (handle_input_redirection(redir) < 0)
				exit(EXIT_FAILURE);
		}
		else if (redir->type == TOKEN_REDIRECT_OUT || redir->type == TOKEN_APPEND)
		{
			if (handle_output_redirection(redir) < 0)
				exit(EXIT_FAILURE);
		}
		else if (redir->type == TOKEN_HEREDOC)
		{
			if (handle_heredoc_redirection(cmd) < 0)
				exit(EXIT_FAILURE);
		}
		
		redir = redir->next;
	}
}


