#include "../../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../libft/libft.h"
#include <stdbool.h>


char *expanation_token_env_var(char *str, t_env *envp)
{
	char	*result;
	char	*var;
	char	*env_var;
	char	*tmp;
	t_env	*tmp1;
	// char	*key;
	// char	*val;
	int i;
	int count;
	int	pos;
	int j;

	
	count = 0;
	i = 0;
	result = ft_strdup(str);
	tmp1 = envp;
	while (result[i] != '\0')
	{
		if (result[i] == '$' && ft_isalpha(result[i + 1]))
		{
			i++;
			pos = i;
			while (result[i] && ft_isalnum(result[i]) )
			{
				i++;
				count++;
			}
			var = ft_substr(result, pos, count);
			j = 0;
			while (tmp1)
			{
				// printf("var ==> (%s)\n", var);
				printf("env key ==> (%s)\n", tmp1->key);
				if (strcmp(tmp1->key, var))
				{
					env_var = tmp1->value;
					printf("<<Value>> ==> %s\n", tmp1->value);
					printf("test done ");
				}
				else
					env_var = NULL;
				tmp1 = tmp1->next;
			}

			// env_var = getenv(var);
			// env_var

			printf("env_var ==> %s\n", env_var);
			if (env_var)
			{
				tmp = ft_calloc(ft_strlen(result) - ft_strlen(var) + ft_strlen(env_var) + 1, sizeof(char));
				ft_strlcpy(tmp, result, pos);
				ft_strlcat(tmp, env_var, ft_strlen(tmp) + ft_strlen(env_var) + 1);
				ft_strlcat(tmp, result + pos + count, ft_strlen(tmp) + ft_strlen(result + pos + count) + 1);
				result = tmp;
			}
			else
			{
				tmp = ft_calloc(ft_strlen(result) - ft_strlen(var) + 1, sizeof(char));
				ft_strlcpy(tmp, result, pos);
				ft_strlcat(tmp, result + pos + count, ft_strlen(tmp) + ft_strlen(result + pos + count) + 1);
				result = tmp;
			}
		}
		i++;
	}
	return (result);
}