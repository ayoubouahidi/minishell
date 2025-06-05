void	parsing_cmd(char *input, t_exec_env *exec_env)
{
	int			i;
	t_token		*tokens;
	t_env		*env_list;
	t_commands	*cmd_list;
	t_commands	*tmp_cmd_list;
	t_token		*tmp_token;
	t_env		*tmp_env;
	int			x;
	char		*line_of_heredoc;
	int			j;
	char		*result;
	char		*tmp;
	int			start;
	char		*key;
	char		*value;
	char		*file_name;
	int			fd;
	int			idx_last_fd;
	char		*tmp_line;

	// char		*value_of_env;
	i = 0;
	// handle the exit cmd
	// if ((input == NULL || !ft_strcmp(input, "exit")) && (ft_strlen(input) == 4))
	// {
	// 	printf("exit\n");
	// 	free(input);
	// 	exit(0);
	// }
	// split the cmd to tokens
	tokens = tokenize_input(input);
	if (!tokens)
	{
		g_exit_status = 2;
		printf("minishell: syntax error\n");
		return ;
	}
	// printf("====================================================================>\n");
	// print tokens => value & type
	// tmp_token = tokens;
	// while (tmp_token)
	// {
	// 	printf("TOKEN: [%s] Type: %d Quote: %d\n", tmp_token->value, tmp_token->type, tmp_token->quote_type);
	// 	tmp_token = tmp_token->next;
	// }
	// store the env variables in the env list
	env_list = init_env(exec_env->env);
	// print env list
	// tmp_env = env_list;
	// while (tmp_env)
	// {
	// 	if (tmp_env->value)
	// 		printf("%s=%s\n", tmp_env->key, tmp_env->value);
	// 	else
	// 		printf("Env: %s\n", tmp_env->key);
	// 	tmp_env = tmp_env->next;
	// }
	// print value of env
	// value_of_env = get_env_value(env_list, "USER");
	// if (value_of_env)
	// 	printf("value of env: %s\n", value_of_env);
	// else
	// 	printf("env not found\n");
	// // expand the env variables
	expand_variables_and_remove_quotes(tokens, env_list);
	// print tokens after expanding the env variables
	// printf("Tokens after expanding:\n");
	// tmp_token = tokens;
	// while (tmp_token)
	// {
	// 	printf("TOKEN: [%s] Type: %d Quote type: %d\n", tmp_token->value, tmp_token->type, tmp_token->quote_type);
	// 	tmp_token = tmp_token->next;
	// }
	// parse the tokens
	cmd_list = parse_tokens(tokens);
	// if (!cmd_list)
	if (!cmd_list || !check_cmds(tokens))
	{
		g_exit_status = 2;
		printf("minishell: syntax error\n");
		return ;
	}
	
	// print commands	
	// x = 1;
	// tmp_cmd_list = cmd_list;
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
	// 		i = 1;
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
	
	// create heredoc and store the fd in the cmd list
	t_commands *cmd_here_doc = cmd_list;
	while (cmd_here_doc)
	{
		if (cmd_here_doc->heredoc)
		{
			if (heredoc(cmd_here_doc, env_list) == -1)
			{
				printf("Error: heredoc failed\n");
				return ;
			}
			if (!cmd_list->args && cmd_list->heredoc)
				return ;
		}
		cmd_here_doc = cmd_here_doc->next;
	}
	// if (cmd_list->heredoc)
	// {
	// 	if (heredoc(cmd_list, env_list) == -1)
	// 	{
	// 		printf("Error: heredoc failed\n");
	// 		return ;
	// 	}
	// 	if (!cmd_list->args && cmd_list->heredoc)
	// 		return ;
	// }
	// if (!cmd_list->args)
	// 	return;
	// if (cmd_list->args)
	// {
	// 	printf("yesfjk\n");
	// 	printf("cmd args is %s\n", cmd_list->args[0]);
	// }
	// idx_last_fd = 0;
	// while (cmd_list->fds_of_heredoc[idx_last_fd] != -1)
	// 	idx_last_fd++;
	// idx_last_fd--;
	// int z = 0;
	// while ((tmp_line = get_next_line(cmd_list->fds_of_heredoc[idx_last_fd])))
	// {
	// 	if (!tmp_line)
	// 		break ;
	// 	printf("%d: %s", z, tmp_line);
	// 	free(tmp_line);
	// 	z++;
	// }
	// ---------- Execution Part ----------
	launch_execution(cmd_list, exec_env);
	// free token list and command list after execution
	// free_tokens(tokens);
	// free_commands(cmd_list);
	// ft_malloc(0, 0);
}