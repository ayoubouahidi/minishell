/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/01 10:57:59 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int is_empty_or_whitespace(char *str)
{
	
	if (!str)
		return (1);
	
	while (*str)
	{
		
		if (*str != ' ' && *str != '\t' && *str != '\n')
			return (0);
		str++;
	}
	return (1);
}

static int handle_input_redirection(t_redirections *redir)
{
	int fd;
	
	
	if (is_empty_or_whitespace(redir->file))
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
	int	fd;
	int	flags;

	if (is_empty_or_whitespace(redir->file))
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", STDERR_FILENO);
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


int setup_redirections(t_command *cmd)
{
	t_redirections *redir;

	
	if (!cmd || !cmd->redirections)
		return (0);
	
	
	redir = cmd->redirections;
	while (redir)
	{
		
		if (redir->type == TOKEN_REDIRECT_IN)
		{
			if (handle_input_redirection(redir) < 0)
				return (-1);
		}
		else if (redir->type == TOKEN_REDIRECT_OUT || redir->type == TOKEN_APPEND)
		{
			if (handle_output_redirection(redir) < 0)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}