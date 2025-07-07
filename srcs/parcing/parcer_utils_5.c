/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_utils_5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:57:56 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/07 07:19:57 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool	heredoc_check_append(t_token *token, char **del)
{
	t_token	*prev;

	prev = token;
	token = token->next;
	if ((token)->type != WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		return (false);
	}
	*del = token->value;
	return (true);
}

t_redirections	*creat_red(char *file, t_TYPE_TOKEN type)
{
	t_redirections	*red;

	red = (t_redirections *)ft_malloc(sizeof(t_redirections), 1);
	if (!red)
		return (NULL);
	red->file = file;
	red->type = type;
	red->next = NULL;
	return (red);
}

void	ft_lstadd_back_red(t_redirections **lst, t_redirections *new)
{
	t_redirections	*tmp;

	if (!lst || !new)
		return ;
	tmp = *lst;
	if (*lst)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*lst = new;
}

// Initialize command structure
t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)ft_malloc(sizeof(t_command), 1);
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->is_heredoc = false;
	cmd->del = NULL;
	cmd->next = NULL;
	cmd->redirections = NULL;
	return (cmd);
}

void	process_word_token(t_token *token, char ***args, bool *in_red)
{
	if (!*in_red)
		*args = to_arg(token, *args);
	*in_red = false;
}
