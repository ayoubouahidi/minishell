/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:02:05 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/05 07:22:13 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

t_command	*init_and_validate_command(t_token **tokendd)
{
	t_command	*cmd;

	if (!tokendd || !*tokendd)
		return (NULL);
	cmd = init_command();
	if (!cmd)
		return (NULL);
	return (cmd);
}

bool	process_tokens(t_token **tokendd, t_command *cmd, char ***args,
		t_redirections **red_head)
{
	bool	in_red;

	in_red = false;
	while (tokendd && (*tokendd)->type != ENDF && (*tokendd)->type != PIPE)
	{
		if ((*tokendd)->type == WORD)
			process_word_token(*tokendd, args, &in_red);
		else
		{
			in_red = true;
			if (!process_redirection(*tokendd, cmd, red_head))
			{
				if (cmd->del)
					free(cmd->del);
				free(cmd);
				return (false);
			}
		}
		(*tokendd) = (*tokendd)->next;
	}
	return (true);
}

t_command	*parser_commande(t_token **tokendd)
{
	t_command		*cmd;
	char			**args;
	bool			in_red;
	t_redirections	*red_head;

	args = NULL;
	in_red = false;
	red_head = NULL;
	cmd = init_and_validate_command(tokendd);
	if (!cmd)
		return (NULL);
	if (!process_tokens(tokendd, cmd, &args, &red_head))
		return (NULL);
	cmd->args = args;
	cmd->redirections = red_head;
	return (cmd);
}

bool	validate_pipe_parse(t_token *token)
{
	t_token	*current_token;

	current_token = token;
	if (token->type == PIPE)
	{
		print_syntax_error(current_token);
		return (false);
	}
	while (current_token && current_token->type != ENDF)
	{
		if (current_token->type != WORD)
		{
			if (current_token->next && current_token->next->type != WORD)
			{
				print_syntax_error(current_token);
				return (false);
			}
		}
		current_token = current_token->next;
	}
	return (true);
}
t_token	*tokenize_input(char *line, t_env *envp)
{
	t_lexer	*lexer;
	char	*trim;

	t_token(*token), (*head_token);
	token = NULL;
	head_token = NULL;
	trim = ft_strtrim(line, " ");
	if (!syntaxe_error(trim))
	{
		write(1, "Quotes Error !\n", 15);
		free(trim);
		return (NULL);
	}
	lexer = creat_lexer(trim);
	while (1)
	{
		token = tokenize(lexer);
		expantion_remove_quotes(token, envp);
		ft_lstadd_back_token(&head_token, token);
		if (token->type == ENDF)
			break ;
	}
	if (!validate_pipe_parse(head_token))
		return (NULL);
	return (head_token);
}

t_command	*parcer(char *line, t_env *envp)
{
	t_token		*head_token;
	t_command	*commande;
	t_command	*head;

	commande = NULL;
	head = NULL;
	head_token = tokenize_input(line, envp);
	if (!head_token)
		return (NULL);
	while (head_token && head_token->type != ENDF)
	{
		commande = parser_commande(&head_token);
		ft_lstadd_back_cmd(&head, commande);
		head_token = head_token->next;
	}
	return (head);
}
