/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:57:19 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/07 07:21:01 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

t_command	*ft_lstnew_cmd(void *content)
{
	t_command	*tmp;

	tmp = (t_command *)ft_malloc(sizeof(t_command), 1);
	if (!tmp)
		return (NULL);
	tmp->args = content;
	tmp->next = NULL;
	return (tmp);
}

void	ft_lstadd_back_token(t_token **lst, t_token *new)
{
	t_token	*tmp;

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

void	ft_lstadd_back_cmd(t_command **lst, t_command *new)
{
	t_command	*tmp;

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

int	ft_lstsize_parce(t_command *lst)
{
	int	cmpt;

	cmpt = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		cmpt++;
	}
	return (cmpt);
}

t_IS_QUOTED	is_token_quoted(const char *str, int len)
{
	int	i;

	if (!str || len < 2)
		return (NOT_QUOTED);
	if ((str[0] == '"' && str[1] == '"' && len == 2) || (str[0] == '\''
			&& str[1] == '\'' && len == 2))
		return (QUOTED);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\''
				&& str[len - 1] == '\'')))
		return (QUOTED);
	i = 0;
	while (i < len)
	{
		if (str[i] == '"' || str[i] == '\'')
			return (QUOTED);
		i++;
	}
	return (NOT_QUOTED);
}
