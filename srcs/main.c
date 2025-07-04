/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/04 20:00:25 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit_status = 0;

// static int	run_here_doc(t_command *cmd_list, t_env *env_list)
// {
// 	if (heredoc(cmd_list, env_list) == -1)
// 	{
// 		print_error("Error: heredoc failed\n");
// 		return (0);
// 	}
// 	if (cmd_list->signal_detected)
// 		return (0);
// 	return (1);
// }

// static void	parsing_cmd(char *input, t_data *persistent_data, char **env)
// {
// 	t_token		*tokens;
// 	t_command	*cmd_list;

// 	persistent_data->exit_status = 0;
// 	tokens = tokenize_input(input);
// 	if (!tokens)
// 	{
// 		print_error("minishell: syntax error\n");
// 		return ;
// 	}
// 	expand_variables_and_remove_quotes(tokens, persistent_data->env);
// 	cmd_list = parse_tokens(tokens);
// 	if (!cmd_list || !check_cmds(tokens))
// 	{
// 		print_error("minishell: syntax error\n");
// 		return ;
// 	}
// 	if (!run_here_doc(cmd_list, persistent_data->env))
// 	{
// 		free_cmd(cmd_list);
// 		return ;
// 	}
// 	persistent_data->cmd = cmd_list;
// 	executer(persistent_data, env);
// 	g_exit_status = persistent_data->exit_status;
// 	free_cmd(cmd_list);
// 	persistent_data->cmd = NULL;
// }

// static void	read_line_process(t_data *data, char **env)
// {
// 	char	*input;

// 	while (1)
// 	{
// 		if (isatty(STDIN_FILENO))
// 			input = readline("minishell> ");
// 		else
// 		{
// 			input = readline("");
// 			if (!input)
// 				break;
// 		}
// 		if (!input)
// 		{
// 			if (isatty(STDIN_FILENO))
// 				printf("exit\n");
// 			break ;
// 		}
// 		if (ft_strlen(input) > 0)
// 			add_history(input);
// 		if (is_only_spaces(input))
// 		{
// 			free(input);
// 			continue ;
// 		}
// 		parsing_cmd(input, data, env);
// 		free(input);
// 	}
// }

// int	main(int ac, char **av, char **envp)
// {
// 	t_data	data;

// 	(void)av;
// 	(void)ac;
// 	ft_memset(&data, 0, sizeof(t_data));
// 	init_env(&data, envp);
// 	read_line_process(&data, envp);
// 	clear_history();
// 	free_env(data.env);
// 	return (0);
// }
#include "parser.h"

void printlist(t_command *head)
{
    t_command *tmp = head;
    int i;

    while (tmp)
    {
        // printf("┌────────────────────────────────────────┐\n");
        // printf("│              Command Block             │\n");
        // printf("├────────────────────────────────────────┤\n");

        printf("│ here_doc_file : %-25s│\n", tmp->here_doc_file);
        printf("│ redirections  :  │\n");
		while (tmp->redirections)
		{
			printf("red ===== %s|\n", tmp->redirections->file);
			tmp->redirections = tmp->redirections->next;
		}
        printf("│ is_heredoc    : %-25s│\n", tmp->is_heredoc ? "true" : "false");
        printf("│ del           : %-25s│\n", tmp->del ? tmp->del : "(null)");

        printf("├─────────────── Arguments ──────────────┤\n");
        if (tmp->args)
        {
            for (i = 0; tmp->args[i]; i++)
                printf("│ arg[%d]        : %-24s│\n", i, tmp->args[i]);
        }
        else
        {
            printf("│ No arguments provided.                 │\n");
        }

        printf("└────────────────────────────────────────┘\n\n");

        tmp = tmp->next;
    }
}

static void init_data(t_data *data, char **envp)
{
    init_env(data, envp);
    data->cmd = NULL;
    data->exit_status = 0;
    data->pid = 0;
    data->is_child = false;
}



int main(int ac, char **av, char **envp)
{
    t_data  data;
    char    *line;

    (void)ac;
    (void)av;
    
    ft_memset(&data, 0, sizeof(t_data)); 
    init_data(&data, envp);
    signal_parent_handler();
    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            clean_exit(&data, data.exit_status);
        }
        if (*line)
        {
            add_history(line);
            data.cmd = parcer(line, data.env);
			// printlist(data.cmd);
			if (data.cmd && run_heredoc(data.cmd) == -1)
				return 0;
			if (data.cmd)
                executer(&data, envp);
            free_cmd(data.cmd);
            data.cmd = NULL;
        }
        free(line);
    }
    return (0);
}