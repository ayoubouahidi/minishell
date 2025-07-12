/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:56:05 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/07 08:59:31 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void	handle_each_heredoc(t_command *cmd, t_env *env, int fd, int i)
{
	char	*line_heredoc;
	char	*result;

	while (1)
	{
		line_heredoc = readline("> ");
		if (!line_heredoc)
		{
			ft_malloc(0, 0);
			close(fd);
			exit(0);
		}
		if (!check_quotes(cmd->heredoc_quoted, i))
		{
			result = expand_here_doc(line_heredoc, env);
			free(line_heredoc);
		}
		else
			result = line_heredoc;
		if (ft_strcmp(result, cmd->heredoc_delimiters[i]) == 0)
			break ;
		if (i == cmd->heredoc_count - 1)
			ft_putendl_fd(result, fd);
	}
}

void	handle_child_process(char *filename, t_command *cmd, t_env *env)
{
	int	fd;
	int	i;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0766);
	i = 0;
	while (i < cmd->heredoc_count)
	{
		handle_each_heredoc(cmd, env, fd, i);
		i++;
	}
	ft_malloc(0, 0);
	close(fd);
	exit(0);
}

char	*randome_generate(void)
{
	int		fd;
	char	*name;
	char	*buffer;
	int		i;

	i = 0;
	fd = open("/dev/random", O_RDONLY);
	name = (char *)ft_malloc(7, 1);
	while (fd != -1 && i < 6)
	{
		buffer = (char *)ft_malloc(2, 1);
		read(fd, buffer, 1);
		buffer[1] = '\0';
		if (ft_isalnum_2(buffer[0]))
		{
			name[i] = buffer[0];
			i++;
		}
	}
	close(fd);
	name[i] = '\0';
	return (name);
}

void	heredocprocess(t_command *cmd, t_env *env)
{
	int		pid;
	int		status;
	char	*filename;

	filename = randome_generate();
	filename = ft_strjoin("/tmp/", filename);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal_child_handler();
		handle_child_process(filename, cmd, env);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (g_exit_status == 130)
			write(1, "\n", 1);
	}
	signal_parent_handler();
	cmd->here_doc_file = filename;
}

int	run_heredoc(t_command *cmd, t_env *env)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		if (current->is_heredoc)
		{
			heredocprocess(current, env);
			if (g_exit_status == 130)
			{
				unlink_heredoc_files(cmd);
				return (-1);
			}
		}
		current = current->next;
	}
	return (0);
}
