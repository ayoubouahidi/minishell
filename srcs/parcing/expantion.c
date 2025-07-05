#include "../../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../libft/libft.h"
#include <stdbool.h>
#include "../../includes/parser.h"

char *join_char(char *str, char c)
{
	char *newstr;
	int i;

	i = 0;
	newstr = malloc(sizeof(char) * ft_strlen(str) + 2);
	while (str[i] != '\0')
	{
		newstr[i] = str[i];
		i++;
	}
	newstr[i] = c;
	i++;
	newstr[i] = '\0';
	free(str);
	return (newstr);
}

char *handle_exit_code(int *i, char *final)
{
	char *exit_str;
	char *temp_final;
	
	exit_str = ft_itoa(g_exit_status);
	if (!exit_str)
		exit_str = ft_strdup("0");
	temp_final = ft_strjoin(final, exit_str);
	free(final);
	free(exit_str);
	final = temp_final;
	(*i) += 2;
	return (final);
}

char	*case_word(char	*result, int *i, char *final)
{
	if (!result || !i || !final)
		return final;
	if (result[*i] == '\0')
		return final;
	final = join_char(final, result[*i]);
	(*i)++;
	return (final);
}

char *normal_var(int *i, char *result, t_env *envp, char *final)
{
    char (*var), (*tmp1), (*new_final);
    int (count), (pos);
	count = 0;
    (*i)++;
    pos = *i;
    while (result[*i] && ft_isalnum(result[*i]) && result[*i] !='.'&& result[*i] != '"')
    {
        (*i)++;
        count++;
    }
    var = ft_substr(result, pos, count);
    tmp1 = ft_strdup(get_env_value(envp, var));
    free(var);
    if (tmp1)
	{
    	new_final = ft_strjoin(final, tmp1);
    	free(tmp1);
		free(final);
	}
	else
	{
		free(tmp1);
		return (final);
	}
    return new_final;
}

char *squotes_expand(int *i, char* result, char *final)
{
	(*i)++;
	while (result[*i] && result[*i] != '\'')
	{
		final = join_char(final, result[*i]);
		(*i)++;
	}
	if(result[*i])
		(*i)++;
	return (final);
}



char *double_quotes_expand(int *i, char *result, t_env *envp, char *final)
{
	(*i)++;
	if (result[*i] == '"')
    {
        (*i)++;
        return final;
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

char *next_char_squotes(char *result,int *i,char *final)
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

char *next_char_dquotes(char *result,int *i,char *final)
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

char *next_char_digits(char  *result, int *i, char *final)
{
	(*i) += 2;
	while (result[*i])
	{
		final = join_char(final, result[*i]);
		(*i)++;
	}
	return(final);
}



char *expand_process(int *i, char *result, t_env *envp, char *final)
{
	while (result[*i] != '\0')
	{
		if (result[*i] == '\'')
			final = squotes_expand(i, result, final);
		else if (result[*i] == '$' && (ft_isalpha(result[*i + 1]) || result[*i + 1] == '_'))
			final = normal_var(i, result, envp, final);
		else if (result[*i] == '"')
			final = double_quotes_expand(i, result, envp, final);
		else if (result[*i] == '$' && result[*i + 1] == '\'')
			final = next_char_squotes(result, i, final);
		else if (result[*i] == '$' && result[*i + 1] == '"')
			final = next_char_dquotes(result, i, final);
		else if (result[*i] == '$' && result[*i + 1] && ft_isdigit(result[*i + 1]))
			final = next_char_digits(result, i, final);
		else if (result[*i] == '$' && result[*i + 1] && result[*i + 1] == '?')
			final = handle_exit_code( i, final);
		else
			final = case_word(result, i, final);
	}
    return final;
}

char *expanation_token_env_var(char *str, t_env *envp)
{
	char	*result;
	t_env	*tmp1;
	char	*final;
	int i;
	int count;

	count = 0;
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
	char *result;

	if(token->type == WORD)
	{
		result = expanation_token_env_var(token->value , envp);
		free(token->value);
		token->value = result;
	}
}
