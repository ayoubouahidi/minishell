/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cases_of_expanding.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:05:25 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/24 16:53:22 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*case_of_squote(char *word, int *i, char *result)
{
	(*i)++;
	while (word[*i] && word[*i] != '\'')
	{
		result = ft_strjoin_char(result, word[*i]);
		(*i)++;
	}
	if (word[*i])
		(*i)++;
	return (result);
}

char	*case_of_dquote(char *word, int *i, char *result, t_env *env)
{
	(*i)++;
	while (word[*i] && word[*i] != '"')
	{
		if (word[*i] == '$' && word[*i + 1] && (ft_isalnum(word[*i + 1])
				|| word[*i + 1] == '_'))
			result = case_of_normal_var_with_dquotes(word, i, result, env);
		else if (word[*i] == '$' && word[*i + 1] && word[*i + 1] == '?')
		{
			(*i) += 2;
			result = ft_strjoin(result, ft_itoa(g_exit_status));
		}
		else
		{
			result = ft_strjoin_char(result, word[*i]);
			(*i)++;
		}
	}
	if (word[*i])
		(*i)++;
	return (result);
}

char	*case_of_normal_var(char *word, int *i, char *result, t_env *env)
{
	int (start), (j);
	char *(key), *(value);
	(*i)++;
	start = (*i);
	while (word[*i] && (ft_isalnum(word[*i]) || word[*i] == '_'))
		(*i)++;
	key = ft_substr(word, start, (*i) - start);
	value = get_env_value(env, key);
	if (!value)
		value = ft_strdup("");
	j = 0;
	while (value && value[j])
	{
		result = ft_strjoin_char(result, value[j]);
		j++;
	}
	return (result);
}

char	*case_of_var_with_next_char_squote(char *word, int *i, char *result)
{
	(*i)++;
	while (word[*i] && word[*i] != '\'')
	{
		result = ft_strjoin_char(result, word[*i]);
		(*i)++;
	}
	if (word[*i])
		(*i)++;
	return (result);
}

char	*case_of_var_with_next_char_dquote(char *word, int *i, char *result)
{
	(*i)++;
	while (word[*i] && word[*i] != '\"')
	{
		result = ft_strjoin_char(result, word[*i]);
		(*i)++;
	}
	if (word[*i])
		(*i)++;
	return (result);
}
