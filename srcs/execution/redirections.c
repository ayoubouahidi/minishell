/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/06 13:11:46 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_input_redirection(t_redirections *redir)
{
	int	fd;

	if (is_empty_or_whitespace(redir->file))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
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

static int	open_output_file(t_redirections *redir, int *fd)
{
	int	flags;

	if (is_empty_or_whitespace(redir->file))
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", STDERR_FILENO);
		return (-1);
	}
	if (redir->type == APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	*fd = open(redir->file, flags, 0644);
	if (*fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(redir->file);
		return (-1);
	}
	return (0);
}

static int	handle_output_redirection(t_redirections *redir)
{
	int	fd;

	if (open_output_file(redir, &fd) < 0)
		return (-1);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_heredoc_file(t_command *cmd)
{
	int	fd;

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

int	setup_redirections(t_command *cmd)
{
	t_redirections	*redir;
	int				has_input_redirection;

	if (!cmd)
		return (0);
	check_input_redirections(cmd, &has_input_redirection);
	if (cmd->is_heredoc && cmd->here_doc_file && !has_input_redirection)
	{
		if (handle_heredoc_file(cmd) < 0)
			return (-1);
	}
	if (!cmd->redirections)
		return (0);
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == INTPUT_RED && handle_input_redirection(redir) < 0)
			return (-1);
		else if ((redir->type == OUTPUT_RED || redir->type == APPEND)
			&& handle_output_redirection(redir) < 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
