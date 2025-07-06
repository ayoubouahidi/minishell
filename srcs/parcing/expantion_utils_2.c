#include "../../includes/minishell.h"
#include "../../includes/parser.h"
#include "../../libft/libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char	*double_quotes_expand(int *i, char *result, t_env *envp, char *final)
{
	(*i)++;
	if (result[*i] == '"')
	{
		(*i)++;
		return (final);
	}
	while (result[*i] && result[*i] != '"')
	{
		if (result[*i] == '$' && ft_isalnum(result[*i + 1]))
			final = normal_var(i, result, envp, final);
		else if (result[*i] == '$' && result[*i + 1] == '?')
			final = handle_exit_code(i, final);
		else
		{
			final = join_char(final, result[*i]);
			(*i)++;
		}
	}
	if (result[*i])
		(*i)++;
	return (final);
}

char	*next_char_squotes(char *result, int *i, char *final)
{
	(*i) += 2;
	while (result[*i] && result[*i] != '\'')
	{
		final = join_char(final, result[*i]);
		(*i)++;
	}
	if (result[*i])
		(*i)++;
	return (final);
}

