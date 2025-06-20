#include "../../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../libft/libft.h"
#include <stdbool.h>
#include "../../includes/parser.h"


// $ll$$all
// syntax error
//  >> v >> v
//  >> v > v

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
	return (newstr);
}

char	*case_word(char	*result, int *i,char *final)
{
	final = join_char(final, result[*i]);
	(*i)++;
	return (final);
}



char *normal_var(int *i, char *result, t_env *envp, char *final)
{
    char *var;
    char *tmp1;
    int count = 0;
    int pos;
    char *new_final;

    (*i)++;
    pos = *i;
    while (result[*i] && ft_isalnum(result[*i]))
    {
        (*i)++;
        count++;
    }
    var = ft_substr(result, pos, count);
    tmp1 = ft_strdup(get_env_value(envp, var));
    free(var); 
    if (!tmp1)
        tmp1 = ft_strdup("");
    new_final = ft_strjoin(final, tmp1);
    free(final);
    free(tmp1);
    return new_final;
}

/*
char *assemble_expanded_string(char *original, int quote_start,  int quote_end, char *expansion)
{
    char *before;
    char *after;
    char *result;
    
    before = ft_substr(original, 0, quote_start);
    after = ft_strdup(original + quote_end);
    
    if (!before || !after) {
        free(before);
        free(after);
        return NULL;
    }
    
    result = ft_strjoin(before, expansion);
    free(before);
    if (!result) {
        free(after);
        return NULL;
    }
    
    before = result;
    result = ft_strjoin(result, after);
    free(before);
    free(after);
    
    return result;
}

char *squotes_expand(int *i, char *result)
{
    int start;
    char *extracted;
    char *new_result;
    
    (*i)++; // Skip opening quote
    start = *i;
    
    // Find closing quote
    while (result[*i] && result[*i] != '\'')
        (*i)++;
    
    if (!result[*i]) {
        fprintf(stderr, "minishell: unmatched single quote\n");
        return NULL; // Or handle error differently
    }
    
    // Extract content between quotes
    extracted = ft_substr(result, start, *i - start);
    if (!extracted)
        return NULL;
    
    // Create new string combining parts before/after quotes
    new_result = assemble_expanded_string(result, start-1, *i+1, extracted);
    free(extracted);
    
    (*i)--; // Adjust position since expand_process will increment
    return new_result;
}*/


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
	while (result[*i] && result[*i] != '"')
	{
		if (result[*i] == '$' && ft_isalnum(result[*i + 1]))
			final = normal_var(i, result, envp, final);
		else if (result[*i] == '$' && result[*i] == '?')
		{
			final = ft_itoa(0); // a refaire
			(*i) += 2;
		}
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

// char *next_char_digits(char  *result, int *i, char *final)
// {

// }

char *expand_process(int *i, char *result, t_env *envp, char *final)
{
	if (result[*i] == '\'')
	{
		final = squotes_expand(i, result, final);
		printf("%d\n", *i);
	}
    else if (result[*i] == '$' && (ft_isalpha(result[*i + 1]) || result[*i + 1] == '_'))
	{
		final = normal_var(i, result, envp, final);
		printf("final : %s\n", final);
	}
	else if (result[*i] == '"')
		final = double_quotes_expand(i, result, envp, final);
	else if (result[*i] == '$' && result[*i + 1] == '\'')
		final = next_char_squotes(result, i, final);
	else if (result[*i] == '$' && result[*i + 1] == '"')
		final = next_char_dquotes(result, i, final);
	// else if (result[*i] == '$' && result[*i + 1] && ft_isdigit(result[*i + 1]))
	// 	final = next_char_digits(result, i, final);
	else
		final = case_word(result, i, final);
	printf("final : %s\n", final);
    return final;
}

char *expanation_token_env_var(char *str, t_env *envp)
{
	char	*result;
	t_env	*tmp1;
	char	*final;
	int i;
	int count;
	// int	pos;
	// int j;


	count = 0;
	i = 0;
	result = ft_strdup(str);
	tmp1 = envp;
	final = ft_strdup("");

	while (result[i] != '\0')
		final = expand_process(&i, result, tmp1, final);
	// printf("final in expand: %s\n", final);
	return (final);
}

void	expantion_remove_quotes(t_token *token, t_env *envp)
{
	char *result;

	// printf("test : done");
	if(token->type == WORD)
	{
		result = expanation_token_env_var(token->value , envp);
	
		printf("--------------------\n%s--------------------------\n", result);
		free(token->value);
		token->value = result;
		// printf("result :%s\n", token->value);
	}
	// return (result);
}
//  ayoub test's

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
// 		else'ayoub'abcd'ouahidi'ayoubOUAHIDIayoub'ayoub'ouahidillllllllllll''ayoub'ouahidi'ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss

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
{	return (final);
    int in_quotes = 0;
    char *expanded;
    
    // First, count the total length needed
    while (result[start_pos + total_len] && 
           (ft_isalnum(result[start_pos + total_len]) || 
            result[start_pos + total_len] == '\'' ||
            in_quotes))
    {
        if (result[start_pos + total_len] == '\'')	return (final);
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
// echo 'ayoub'abcd'ouahidi'ayoubOUAHIDIayoub'ayoub'ouahidillllllllllll''ayoub'ouahidi'ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
