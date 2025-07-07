/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/07 07:18:06 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	update_env(t_env *env, const char *key, const char *new_value)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->value = ft_strdup(new_value);
			return ;
		}
		current = current->next;
	}
}

void	update_or_add_env(t_env **env, const char *key, const char *new_value)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return ;
		current = current->next;
	}
	add_env_node(env, new_env_node((char *)key, (char *)new_value));
}

void	add_env_node(t_env **env, t_env *new_node)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new_node;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

static char	*create_env_string(t_env *env, int *success)
{
	char	*temp;
	char	*result;

	*success = 1;
	if (!env->key || !env->value)
		return (NULL);
	temp = ft_strjoin(env->key, "=");
	if (!temp)
	{
		*success = 0;
		return (NULL);
	}
	result = ft_strjoin(temp, env->value);
	if (!result)
		*success = 0;
	return (result);
}

char	**env_to_array(t_env *env)
{
	int		i;
	char	**arr;
	int		success;
	char	*env_str;

	if (!env)
		return (NULL);
	arr = ft_malloc(sizeof(char *) * (env_size(env) + 1), 1);
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		env_str = create_env_string(env, &success);
		if (env_str && success)
			arr[i++] = env_str;
		else if (!success)
			return (NULL);
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}
