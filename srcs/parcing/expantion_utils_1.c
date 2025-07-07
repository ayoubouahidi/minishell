/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:54:34 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/07 07:18:11 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char	*join_char(char *str, char c)
{
	char	*newstr;
	int		i;

	i = 0;
	newstr = ft_malloc(sizeof(char) * ft_strlen(str) + 2, 1);
	while (str[i] != '\0')
	{
		newstr[i] = str[i];
		i++;
	}
	newstr[i] = c;
	i++;
	newstr[i] = '\0';
	return (newstr);
}

char	*handle_exit_code(int *i, char *final)
{
	char	*exit_str;
	char	*temp_final;

	exit_str = ft_itoa(g_exit_status);
	temp_final = ft_strjoin(final, exit_str);
	final = temp_final;
	(*i) += 2;
	return (final);
}

char	*case_word(char *result, int *i, char *final)
{
	if (!result || !i || !final)
		return (final);
	if (result[*i] == '\0')
		return (final);
	final = join_char(final, result[*i]);
	(*i)++;
	return (final);
}

char	*normal_var(int *i, char *result, t_env *envp, char *final)
{
	int		count;
	int		pos;
	char	*var;
	char	*tmp1;	
	char	*new_final;

	count = 0;
	(*i)++;
	pos = *i;
	while (result[*i] && ft_isalnum(result[*i]) && result[*i] != '.'
		&& result[*i] != '"')
	{
		(*i)++;
		count++;
	}
	var = ft_substr(result, pos, count);
	tmp1 = ft_strdup(get_env_value(envp, var));
	if (tmp1)
		new_final = ft_strjoin(final, tmp1);
	else
		return (final);
	return (new_final);
}

char	*squotes_expand(int *i, char *result, char *final)
{
	(*i)++;
	while (result[*i] && result[*i] != '\'')
	{
		final = join_char(final, result[*i]);
		(*i)++;
	}
	if (result[*i])
		(*i)++;
	return (final);
}
