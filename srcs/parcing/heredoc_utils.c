/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:56:16 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 14:56:19 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int	is_inside_child(int flag)
{
	static int	set;

	if (flag == 1)
		set = 1;
	if (flag == 0)
		set = 0;
	return (set);
}

void	here_doc_signal(int signum)
{
	(void)signum;
	printf("is inside = %d", is_inside_child(2));
	if (is_inside_child(2))
	{
		exit(130);
	}
}

bool	check_quotes(char *del)
{
	int	i;

	if (!del)
		return (false);
	i = 0;
	while (del[i])
	{
		if (del[i] == '\'' || del[i] == '"')
			return (true);
		i++;
	}
	return (false);
}

char	*expand_here_doc(char *line_heredoc, t_env *env)
{
	char	*result;
	int		i;

	if (!line_heredoc)
		return (NULL);
	i = 0;
	result = ft_strdup("");
	while (line_heredoc[i])
	{
		if (line_heredoc[i] == '$' && line_heredoc[i + 1]
			&& (ft_isalpha(line_heredoc[i + 1]) || line_heredoc[i + 1] == '_'))
			result = normal_var(&i, line_heredoc, env, result);
		else if (line_heredoc[i] == '$' && line_heredoc[i + 1]
			&& ft_isdigit(line_heredoc[i + 1]))
			result = next_char_digits(line_heredoc, &i, result);
		else if (line_heredoc[i] == '$' && line_heredoc[i + 1] && line_heredoc \
			[i + 1] == '?')
			result = handle_exit_code(&i, result);
		else
			result = case_word(line_heredoc, &i, result);
	}
	return (result);
}

int	ft_isalnum_2(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
