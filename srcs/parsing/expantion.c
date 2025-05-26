#include "../../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../libft/libft.h"
#include <stdbool.h>
#include "../../includes/parser.h"

char *normal_var(int *i, char *result, t_env *envp)
{

	char *var;
    char *env_var = NULL;
    char *tmp;
    t_env *tmp1;
    int count = 0;
    int pos;

	(*i)++;
	pos = *i;
    while (result[*i] && ft_isalnum(result[*i]))
	{
		(*i)++;
		count++;
	}
	var = ft_substr(result, pos, count);
	tmp1 = envp;
	while (tmp1)
	{
		if (!strcmp(tmp1->key, var))
		{
			env_var = tmp1->value;
			break;
		}
		tmp1 = tmp1->next;
	}
	if (env_var)
	{
		tmp = ft_calloc(ft_strlen(result) - ft_strlen(var) + ft_strlen(env_var) + 1, sizeof(char));
		ft_strlcpy(tmp, result, pos);
		ft_strlcat(tmp, env_var, ft_strlen(tmp) + ft_strlen(env_var) + 1);
		ft_strlcat(tmp, result + pos + count, ft_strlen(tmp) + ft_strlen(result + pos + count) + 1);
	}
	else
	{
		tmp = ft_calloc(ft_strlen(result) - ft_strlen(var) + 1, sizeof(char));
		ft_strlcpy(tmp, result, pos);
		ft_strlcat(tmp, result + pos + count, ft_strlen(tmp) + ft_strlen(result + pos + count) + 1);
	}
	free(var);
	free(result); 
    *i = pos - 1;
	return tmp;	
}


char *squotes_expand(int *i, char* result)
{
	int count;
	char *tmp;
	int pos;

	printf ("test\n");
	(*i)++;
	count = 0;
	pos = *i;
	while (result[*i] && result[*i] != '\'')
	{
		(*i)++;
		count++;
	}
	tmp = ft_calloc(count + 1, sizeof(char));
	ft_strlcpy(tmp, result + pos, count + 1);
	return (tmp);
}


// char *normal_case_dquotes(int *i , char  *result,t_env *envp)
// {

// }


char *double_quotes_expand(int *i, char *result, t_env *envp)
{
	char *tmp;


	(*i)++;
	while (result[*i] && result[*i] != '"')
	{
		if (result[*i] == '$' && ft_isalnum(result[*i + 1]))
			tmp = normal_var(i, result, envp);
		else if (result[*i] == '$' && result[*i] == '?')
			tmp = ft_itoa(0); // a refaire
		else {
			tmp = ft_calloc(ft_strlen(result), sizeof(char));
			ft_strlcpy(tmp, result , ft_strlen(result));
			(*i)++;
		}
	}
	if (result[*i])
		(*i)++;
	return (tmp);
}

char *expand_process(int *i, char *result, t_env *envp)
{
	// printf("result char ==> %c\n", result[*i]);
	if (result[*i] == '\'')
		return (squotes_expand(i, result));
    else if (result[*i] == '$' && ft_isalpha(result[*i + 1]))
		return (normal_var(i, result, envp));
	else if (result[*i] == '"')
		return (double_quotes_expand(i, result, envp));
    return result;
}

char *expanation_token_env_var(char *str, t_env *envp)
{
	char	*result;

	t_env	*tmp1;
	int i;
	int count;
	// int	pos;
	// int j;

	
	count = 0;
	i = 0;
	result = ft_strdup(str);
	tmp1 = envp;
	while (result[i] != '\0')
	{
		result = expand_process(&i, result, tmp1);
		i++;
	}
	return (result);
}

void	expantion_remove_quotes(t_token *token, t_env *envp)
{
	char *result;

	// printf("test : done");
	if(token->type == WORD)
	{
		result = expanation_token_env_var(token->value , envp);
		free(token->value);
		token->value = result;
	}
	// return (result);
}


// #include "../../includes/minishell.h"
// #include "../libft/libft.h"

// static char	*get_limiter(char *limiter)
// {
// 	int		i;
// 	char	quote;
// 	char	*result;

// 	i = 0;
// 	result = ft_strdup("");
// 	while (limiter && limiter[i])
// 	{
// 		if (limiter[i] == '"' || limiter[i] == '\'')
// 		{
// 			quote = limiter[i];
// 			i++;
// 			while (limiter[i] && limiter[i] != quote)
// 			{
// 				result = ft_strjoin_char(result, limiter[i]);
// 				i++;
// 			}
// 			i++;
// 		}
// 		else
// 			result = ft_strjoin_char(result, limiter[i++]);
// 	}
// 	return (result);
// }

// static char	*process_of_expanding(char *word, int *i, char *result, t_env *env)
// {
// 	if (word[*i] == '\'')
// 		result = case_of_squote(word, i, result);
// 	else if (word[*i] == '$' && word[*i + 1] && (ft_isalpha(word[*i + 1])
// 			|| word[*i + 1] == '_'))
// 		result = case_of_normal_var(word, i, result, env);
// 	else if (word[*i] == '"')
// 		result = case_of_dquote(word, i, result, env);
// 	else if (word[*i + 1] && word[*i] == '$' && word[*i + 1] == '\'')
// 		result = case_of_var_with_next_char_squote(word, i, result);
// 	else if (word[*i + 1] && word[*i] == '$' && word[*i + 1] == '\"')
// 		result = case_of_var_with_next_char_dquote(word, i, result);
// 	else if (word[*i] == '$' && word[*i + 1] && ft_isdigit(word[*i + 1]))
// 		result = case_of_var_with_next_char_digit(word, i, result);
// 	else if (word[*i] == '$' && word[*i + 1] && word[*i + 1] == '?')
// 		result = case_of_var_with_exit_status(i, result);
// 	else
// 		result = case_of_word(word, i, result);
// 	return (result);
// }

// static char	*expand_variable_value(char *word, t_env *env, int *is_here_doc)
// {
// 	int		i;
// 	char	*result;

// 	i = 0;
// 	if (*is_here_doc)
// 	{
// 		result = get_limiter(word);
// 		*is_here_doc = 0;
// 		return (result);
// 	}
// 	result = ft_strdup("");
// 	while (word[i])
// 		result = process_of_expanding(word, &i, result, env);
// 	return (result);
// }

// void	expand_variables_and_remove_quotes(t_token *tokens, t_env *env)
// {
// 	char	*expanded_value;
// 	int		is_here_doc;

// 	is_here_doc = 0;
// 	while (tokens)
// 	{
// 		if (tokens->type == TOKEN_HEREDOC)
// 			is_here_doc = 1;
// 		if (tokens->type == TOKEN_WORD)
// 		{
// 			expanded_value = expand_variable_value(tokens->value, env,
// 					&is_here_doc);
// 			tokens->value = expanded_value;
// 		}
// 		tokens = tokens->next;
// 	}
// }
//  */






// handle single quotes

/*
char *squotes_expand(char *result, int *i)
{
    int start_pos = *i;
    int total_len = 0;
    int in_quotes = 0;
    char *expanded;
    
    // First, count the total length needed
    while (result[start_pos + total_len] && 
           (ft_isalnum(result[start_pos + total_len]) || 
            result[start_pos + total_len] == '\'' ||
            in_quotes))
    {
        if (result[start_pos + total_len] == '\'')
        {
            in_quotes = !in_quotes; // Toggle quote state
        }
        total_len++;
    }
    
    // Allocate memory for the entire word
    expanded = ft_calloc(total_len + 1, sizeof(char));
    if (!expanded)
        return NULL;
    
    // Copy characters, handling quoted sections
    int j = 0;
    in_quotes = 0;
    
    for (int pos = start_pos; pos < start_pos + total_len; pos++)
    {
        if (result[pos] == '\'')
        {
            in_quotes = !in_quotes; // Toggle quote state
            // Skip the quote character itself
        }
        else
        {
            expanded[j++] = result[pos];
        }
    }
    
    expanded[j] = '\0';
    *i += total_len; // Update position in the original string
    
    return expanded;
}
*/







