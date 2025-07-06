/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_utils_6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:58:02 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 14:58:03 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool	process_redirection(t_token *token, t_command *cmd,
		t_redirections **red_head)
{
	t_redirections	*red;

	red = NULL;
	if (token->type == OUTPUT_RED)
	{
		red = creat_red(token->next->value, OUTPUT_RED);
		ft_lstadd_back_red(red_head, red);
	}
	else if (token->type == INTPUT_RED)
	{
		red = creat_red(token->next->value, INTPUT_RED);
		ft_lstadd_back_red(red_head, red);
	}
	else if (token->type == HEREDOC)
		cmd->is_heredoc = heredoc_check_append(token, &cmd->del);
	else if (token->type == APPEND)
	{
		red = creat_red(token->next->value, APPEND);
		ft_lstadd_back_red(red_head, red);
	}
	return (token->type != HEREDOC || cmd->is_heredoc);
}

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
