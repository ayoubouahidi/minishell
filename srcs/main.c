/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:15:03 by mdahani           #+#    #+#             */
/*   Updated: 2025/06/22 17:11:05 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit_status;

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

static void	parsing_cmd(char *input, char **env)
{
	t_token		*tokens;
	t_data		data;
	t_command	*cmd_list;

	// Initialize data structure
	ft_memset(&data, 0, sizeof(t_data));
	data.exit_status = 0;
	
	tokens = tokenize_input(input);
	if (!tokens)
	{
		print_error("minishell: syntax error\n");
		return ;
	}

	// t_token *tmp_token = tokens;
	// while (tmp_token)
	// {
	// 	printf("TOKEN: [%s] Type: %d Quote: %d\n", tmp_token->value, tmp_token->type, tmp_token->quote_type);
	// 	tmp_token = tmp_token->next;
	// }
	
	// Initialize environment
	init_env(&data, env);
	
	expand_variables_and_remove_quotes(tokens, data.env);


	// printf("Tokens after expanding:\n");
	// tmp_token = tokens;
	// while (tmp_token)
	// {
	// 	printf("TOKEN: [%s] Type: %d Quote type: %d\n", tmp_token->value, tmp_token->type, tmp_token->quote_type);
	// 	tmp_token = tmp_token->next;
	// }

	
	cmd_list = parse_tokens(tokens);
	if (!cmd_list || !check_cmds(tokens))
	{
		print_error("minishell: syntax error\n");
		free_env(data.env);
		return ;
	}



	// int x = 1;
	// t_command *tmp_cmd_list = cmd_list;
	// while (tmp_cmd_list)
	// {
	// 	printf("Command %d:\n", x++);
		
	// 	if (tmp_cmd_list->args)
	// 	{
	// 		printf("  Args:\n");
	// 		for (int j = 0; tmp_cmd_list->args[j]; j++)
	// 			printf("    %s\n", tmp_cmd_list->args[j]);
	// 		printf("  Quote type: %d\n", tmp_cmd_list->quote_type);
	// 	}
	
	// 	if (tmp_cmd_list->redirections)
	// 	{
	// 		t_redirections *redir = tmp_cmd_list->redirections;
	// 		int i = 1;
	// 		while (redir)
	// 		{
	// 			const char *type_redir;
	// 			if (redir->type == TOKEN_REDIRECT_IN)
	// 				type_redir = "Input";
	// 			else if (redir->type == TOKEN_REDIRECT_OUT)
	// 				type_redir = "Output (overwrite)";
	// 			else if (redir->type == TOKEN_APPEND)
	// 				type_redir = "Output (append)";
	// 			else if (redir->type == TOKEN_HEREDOC)
	// 				type_redir = "Heredoc";
	// 			else
	// 				type_redir = "Unknown";
	
	// 			printf("  Redirection[%d]: %s (%s)\n", i++, redir->file, type_redir);
	// 			redir = redir->next;
	// 		}
	// 	}
	// 	tmp_cmd_list = tmp_cmd_list->next;
	// }

	
	
	if (!run_here_doc(cmd_list, data.env))
	{
		free_env(data.env);
		free_cmd(cmd_list);
		return ;
	}

	// Merge parsing with execution
	data.cmd = cmd_list;
	executer(&data, env);
	
	// Cleanup
	free_env(data.env);
	free_cmd(cmd_list);
}
	

static void	read_line_process(char **env)
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
		parsing_cmd(input, env);
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{

	(void)av;
	(void)ac;
	read_line_process(envp);
	clear_history();
	return (0);
}
