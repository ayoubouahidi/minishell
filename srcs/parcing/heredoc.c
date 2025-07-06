/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:56:05 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 14:56:08 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../includes/minishell.h"
#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void	handle_child_process(char *filename, char *del, t_env *env)
{
	char	(*line_heredoc), (*result);
	int		fd;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0766);
	while (1)
	{
		line_heredoc = readline("> ");
		if (!check_quotes(del))
		{
			result = expand_here_doc(line_heredoc, env);
			free(line_heredoc);
		}
		else
			result = line_heredoc;
		if (ft_strcmp(result, del) == 0)
		{

			close(fd);
			// ft_malloc(0, 0);
			exit(0);
		}
		ft_putendl_fd(result, fd);

	}
}

int	ft_isalnum_2(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

char	*randome_generate(void)
{
	int		fd;
	char	*name;
	char	*buffer;
	int		i;

	i = 0;
	fd = open("/dev/random", O_RDONLY);
	name = (char *)ft_malloc(7,1);
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
	char	*tmp;
	int		signal_num;

	filename = randome_generate();
	tmp = filename;
	filename = ft_strjoin("/tmp/", filename);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal_child_handler();
		handle_child_process(filename, cmd->del, env);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		signal_num = WTERMSIG(status);
		g_exit_status = 128 + signal_num;
		if (g_exit_status == 130)
			write(1, "\n", 1);
	}
	signal_parent_handler();
	cmd->here_doc_file = filename;
}

void	unlink_heredoc_files(t_command *cmd)
{
    t_command	*current;

    current = cmd;
    while (current)
    {
        if (current->is_heredoc && current->here_doc_file)
            unlink(current->here_doc_file);
        current = current->next;
    }
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
