/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/06 12:31:33 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(t_env *env, const char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env	*new_env_node(char *key, char *value)
{
	t_env	*new;

	new = ft_malloc(sizeof(t_env), 1);
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

char	*extract_value(char *str)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
		return (NULL);
	i++;
	value = ft_malloc(ft_strlen(str + i) + 1, 1);
	if (!value)
		return (NULL);
	while (str[i])
		value[j++] = str[i++];
	value[j] = '\0';
	return (value);
}

char	*extract_key(char *str)
{
	int		i;
	char	*key;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	key = ft_malloc(i + 1, 1);
	if (!key)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		key[i] = str[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

void	init_env(t_data *data, char **envp)
{
	int		i;
	t_env	*env;
	char	*key;
	char	*value;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		key = extract_key(envp[i]);
		value = extract_value(envp[i]);
		if (key)
		{
			add_env_node(&env, new_env_node(key, value));
		}
		i++;
	}
	data->env = env;
}
