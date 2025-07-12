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

bool	heredoc_check_append(t_token *token, char ***heredoc_delimiters, \
		int *heredoc_count, bool **heredoc_quoted)
{
	char	**new_delimiters;
	bool	*new_quoted;
	int		i;

	token = token->next;
	new_delimiters = (char **)ft_malloc(sizeof(char *) * \
		((*heredoc_count) + 2), 1);
	new_quoted = (bool *)ft_malloc(sizeof(bool) * ((*heredoc_count) + 1), 1);
	if (!new_delimiters || !new_quoted)
		return (false);
	i = 0;
	while (i < *heredoc_count)
	{
		new_delimiters[i] = (*heredoc_delimiters)[i];
		new_quoted[i] = (*heredoc_quoted)[i];
		i++;
	}
	new_delimiters[*heredoc_count] = token->value;
	new_delimiters[*heredoc_count + 1] = NULL;
	new_quoted[*heredoc_count] = (token->is_quoted == QUOTED);
	*heredoc_delimiters = new_delimiters;
	*heredoc_quoted = new_quoted;
	(*heredoc_count)++;
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

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)ft_malloc(sizeof(t_command), 1);
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->is_heredoc = false;
	cmd->heredoc_delimiters = NULL;
	cmd->heredoc_quoted = NULL;
	cmd->heredoc_count = 0;
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
