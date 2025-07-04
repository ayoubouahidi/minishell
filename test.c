
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
    while (result[*i] && (ft_isalnum(result[*i]) || result[*i] == '_'))
    {
        (*i)++;
        count++;
    }
    var = ft_substr(result, pos, count);
    tmp1 = get_env_value(envp, var);
    
    free(var); 
    if (!tmp1)
        tmp1 = ft_strdup("");
    else
        tmp1 = ft_strdup(tmp1);
    
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
    char *exit_str;
    char *temp_final;
    
    (*i)++;
    while (result[*i] && result[*i] != '"')
    {
        if (result[*i] == '$' && (ft_isalnum(result[*i + 1]) || result[*i + 1] == '_'))
            final = normal_var(i, result, envp, final);
        else if (result[*i] == '$' && result[*i + 1] == '?')
        {
            exit_str = ft_itoa(g_exit_status);
            temp_final = ft_strjoin(final, exit_str);
            free(final);
            free(exit_str);
            final = temp_final;
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
    char *exit_str;
    char *temp_final;
    
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
    {
        exit_str = ft_itoa(g_exit_status);
        temp_final = ft_strjoin(final, exit_str);
        free(final);
        free(exit_str);
        final = temp_final;
        (*i) += 2;
    }
    else if (result[*i] == '$' && (!result[*i + 1] || (!ft_isalnum(result[*i + 1]) && result[*i + 1] != '_' && result[*i + 1] != '?')))
    {
        final = join_char(final, result[*i]);
        (*i)++;
    }
    else
        final = case_word(result, i, final);
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
	// if ()
	if(token->type == WORD)
	{
		result = expanation_token_env_var(token->value , envp);
	
		// printf("--------------------\n%s--------------------------\n", result);
		free(token->value);
		token->value = result;
		// printf("result :%s\n", token->value);
	}
	// return (result);
}