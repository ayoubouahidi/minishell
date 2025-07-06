/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/06 20:41:56 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	g_exit_status = 0;

static void	init_data(t_data *data, char **envp)
{
	init_env(data, envp);
	data->cmd = NULL;
	g_exit_status = 0;
	data->pid = 0;
}

static void	handle_line(t_data *data, char *line)
{
	if (*line)
	{
		add_history(line);
		data->cmd = parcer(line, data->env);
		if (data->cmd)
		{
			if (run_heredoc(data->cmd, data->env) == -1)
			{
				data->cmd = NULL;
				return ;
			}
			if (!(data->cmd->is_heredoc
					&& (!data->cmd->args || !data->cmd->args[0])))
				g_exit_status = executer(data);
			data->cmd = NULL;
		}
	}
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;
	char	*line;

	(void)ac;
	(void)av;
	ft_memset(&data, 0, sizeof(t_data));
	(init_data(&data, envp), signal_parent_handler());
	if (!isatty(STDIN_FILENO))
	{
		ft_putstr_fd("minishell: stdin is not a terminal\n", STDERR_FILENO);
		clean_exit(&data, FAILURE);
	}
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			clean_exit(&data, g_exit_status);
		}
		handle_line(&data, line);
		free(line);
	}
	ft_malloc(0, 0);
	return (g_exit_status);
}
