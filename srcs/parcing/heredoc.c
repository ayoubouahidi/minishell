/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:22:01 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/04 21:01:08 by elkharti         ###   ########.fr       */
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

int	is_inside_child(int flag)
{
	static int set;

	if(flag == 1)
		set = 1;
	if(flag == 0)
		set = 0;
	return set;
}

void	here_doc_signal(int signum)
{
	(void)signum;
	
	printf("is inside = %d", is_inside_child(2));
	if(is_inside_child(2))
	{
		exit(130);

	}
}


void	handle_child_process(char *filename, char *del)
{
	char *line_heredoc;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0766);
	while (1)
	{
		line_heredoc = readline("> ");
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
	int status;

	char *filename = randome_generate();
	char *tmp = filename;
	filename = ft_strjoin("/tmp/", filename);
	free(tmp);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal_child_handler();
		handle_child_process(filename, cmd->del);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status)) {
        int signal_num = WTERMSIG(status);
        g_exit_status = 128 + signal_num;
		if(g_exit_status == 130)
			write(1,"\n", 1); 
    }
	signal_parent_handler();
	cmd->here_doc_file = filename;
	write(1,"ff\n", 3);
}

int run_heredoc(t_command *cmd)
{
	if (cmd->is_heredoc)
	{
		heredocprocess(cmd);
		
	}
	return 0;
}
