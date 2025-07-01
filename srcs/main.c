











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

	
	ft_memset(&data, 0, sizeof(t_data));
	data.exit_status = 0;
	
	tokens = tokenize_input(input);
	if (!tokens)
	{
		print_error("minishell: syntax error\n");
		return ;
	}

	
	
	
	
	
	
	
	
	init_env(&data, env);
	
	expand_variables_and_remove_quotes(tokens, data.env);


	
	
	
	
	
	
	

	
	cmd_list = parse_tokens(tokens);
	if (!cmd_list || !check_cmds(tokens))
	{
		print_error("minishell: syntax error\n");
		free_env(data.env);
		return ;
	}



	
	
	
	
	
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	
	
	if (!run_here_doc(cmd_list, data.env))
	{
		free_env(data.env);
		free_cmd(cmd_list);
		return ;
	}

	
	data.cmd = cmd_list;
	executer(&data, env);
	
	
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
