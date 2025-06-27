/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:46:47 by mdahani           #+#    #+#             */
/*   Updated: 2025/06/27 12:33:14 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_cmds(t_token *tokens)
{
	t_token	*tmp_tokens;

	tmp_tokens = tokens;
	while (tmp_tokens)
	{
		if (tmp_tokens->type == TOKEN_PIPE && tmp_tokens->next == NULL)
			return (FAILURE);
		if (tmp_tokens->type == TOKEN_PIPE)
		{
			tmp_tokens = tmp_tokens->next;
			if (tmp_tokens->type == TOKEN_PIPE)
				return (FAILURE);
		}
		tmp_tokens = tmp_tokens->next;
	}
	return (SUCCESS);
}
