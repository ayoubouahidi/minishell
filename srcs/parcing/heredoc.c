/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:22:01 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/03 19:04:19 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../libft/libft.h"
#include <stdbool.h>
#include "../../includes/parser.h"

int	countwords(char const *s, char c)
{
	size_t	words;
	size_t	i;

	i = 0;
	words = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		words++;
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	return (words);
}

void	handle_child_process(char *filename, char *del)
{
	char *line_heredoc;

	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0766);
	while (1)
	{
		line_heredoc = readline(">");
		if (ft_strcmp(line_heredoc, del) == 0) 
		{
			free(line_heredoc);
			close(fd);
			exit(0);
		}
		ft_putendl_fd(line_heredoc, fd);
		free(line_heredoc);
	}
}

char *randome_generate()
{
	int fd;
	char *name;
	char *buffer;
	int i = 0;

	fd = open("/dev/random", O_RDONLY);
	name = (char *)malloc (sizeof(char *) * 7);
	while (fd != -1 && i < 6)
	{
		buffer = (char *)malloc (sizeof(char *) * 2);
		read(fd, buffer,  6);
		buffer[1] = '\0';
		if (ft_isprint(buffer[0]))
		{
			name[i] = buffer[0];
			i++;
		}
		free(buffer);
	}
	close (fd);
	name[i] = '\0';
	return (name);
}
void	heredocprocess(t_command *cmd)
{
	int pid;

	char *filename = randome_generate();
	char *tmp = filename;
	filename = ft_strjoin("/tmp/", filename);
	free(tmp);
	printf("============================%s\n\n", filename);
	pid = fork();
	if (pid == 0)
		signal_child_handler(), handle_child_process(filename, cmd->del);
	waitpid(pid, NULL, 0);
	cmd->here_doc_file = filename;
}

int run_heredoc(t_command *cmd)
{
	if (cmd->is_heredoc)
	{
		heredocprocess(cmd);
		return -1;
	}
	return 0;
}
