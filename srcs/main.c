/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/02 10:46:44 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit_status = 0;

static int	run_here_doc(t_command *cmd_list, t_env *env_list)
{
	if (heredoc(cmd_list, env_list) == -1)
	{
		print_error("Error: heredoc failed\n");
		return (0);
	}
	if (cmd_list->signal_detected)
		return (0);
	return (1);
}

static void	parsing_cmd(char *input, t_data *persistent_data, char **env)
{
	t_token		*tokens;
	t_command	*cmd_list;

	persistent_data->exit_status = 0;
	tokens = tokenize_input(input);
	if (!tokens)
	{
		print_error("minishell: syntax error\n");
		return ;
	}
	expand_variables_and_remove_quotes(tokens, persistent_data->env);
	cmd_list = parse_tokens(tokens);
	if (!cmd_list || !check_cmds(tokens))
	{
		print_error("minishell: syntax error\n");
		return ;
	}
	if (!run_here_doc(cmd_list, persistent_data->env))
	{
		free_cmd(cmd_list);
		return ;
	}
	persistent_data->cmd = cmd_list;
	executer(persistent_data, env);
	g_exit_status = persistent_data->exit_status;
	free_cmd(cmd_list);
	persistent_data->cmd = NULL;
}

static void	read_line_process(t_data *data, char **env)
{
	char	*input;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			input = readline("minishell> ");
		else
		{
			input = readline("");
			if (!input)
				break;
		}
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			break ;
		}
		if (ft_strlen(input) > 0)
			add_history(input);
		if (is_only_spaces(input))
		{
			free(input);
			continue ;
		}
		parsing_cmd(input, data, env);
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	(void)av;
	(void)ac;
	ft_memset(&data, 0, sizeof(t_data));
	init_env(&data, envp);
	read_line_process(&data, envp);
	clear_history();
	free_env(data.env);
	return (0);
}
