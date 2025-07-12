/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:55:07 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 14:55:09 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char	*next_char_dquotes(char *result, int *i, char *final)
{
	(*i) += 2;
	while (result[*i] && result[*i] != '"')
	{
		final = join_char(final, result[*i]);
		(*i)++;
	}
	if (result[*i])
		(*i)++;
	return (final);
}

char	*next_char_digits(char *result, int *i, char *final)
{
	(*i) += 2;
	while (result[*i])
	{
		final = join_char(final, result[*i]);
		(*i)++;
	}
	return (final);
}

char	*expand_process(int *i, char *result, t_env *envp, char *final)
{
	while (result[*i] != '\0')
	{
		if (result[*i] == '\'')
			final = squotes_expand(i, result, final);
		else if (result[*i] == '$' && (ft_isalpha(result[*i + 1]) || result[*i \
				+ 1] == '_'))
			final = normal_var(i, result, envp, final);
		else if (result[*i] == '"')
			final = double_quotes_expand(i, result, envp, final);
		else if (result[*i] == '$' && result[*i + 1] == '\'')
			final = next_char_squotes(result, i, final);
		else if (result[*i] == '$' && result[*i + 1] == '"')
			final = next_char_dquotes(result, i, final);
		else if (result[*i] == '$' && result[*i + 1] && ft_isdigit(result[*i \
				+ 1]))
			final = next_char_digits(result, i, final);
		else if (result[*i] == '$' && result[*i + 1] && result[*i + 1] == '?')
			final = handle_exit_code(i, final);
		else
			final = case_word(result, i, final);
	}
	return (final);
}

char	*expanation_token_env_var(char *str, t_env *envp)
{
	char	*result;
	t_env	*tmp1;
	char	*final;
	int		i;

	i = 0;
	result = ft_strdup(str);
	tmp1 = envp;
	final = ft_strdup("");
	while (result[i] != '\0')
	{
		final = expand_process(&i, result, tmp1, final);
	}
	return (final);
}

void	expantion_remove_quotes(t_token *token, t_env *envp)
{
	char	*result;

	if (token->type == WORD)
	{
		result = expanation_token_env_var(token->value, envp);
		token->value = result;
	}
}
