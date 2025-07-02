/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/02 10:21:01 by elkharti         ###   ########.fr       */
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
			free(current->value);
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
		{
			free(current->value);
			current->value = ft_strdup(new_value);
			return ;
		}
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


int	env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}


char	**env_to_array(t_env *env)
{
	int		i;
	char	**arr;
	char	*temp;

	if (!env)
		return (NULL);
	arr = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->key && env->value)
		{
			temp = ft_strjoin(env->key, "=");
			if (temp)
			{
				arr[i] = ft_strjoin(temp, env->value);
				free(temp);
				if (!arr[i])
					return (free_array(arr), NULL);
				i++;
			}
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

static void	bubble_sort_env(t_env **sorted, int count)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(sorted[j]->key, sorted[j + 1]->key) > 0)
			{
				temp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_sorted_env(t_env **sorted, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(sorted[i]->key, STDOUT_FILENO);
		if (sorted[i]->value && ft_strlen(sorted[i]->value) > 0)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(sorted[i]->value, STDOUT_FILENO);
			ft_putchar_fd('\"', STDOUT_FILENO);
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}

void	sort_and_print_env(t_env *env)
{
	t_env	*current;
	t_env	*sorted[1000];
	int		count;

	count = 0;
	current = env;
	while (current && count < 1000)
	{
		sorted[count++] = current;
		current = current->next;
	}
	
	bubble_sort_env(sorted, count);
	print_sorted_env(sorted, count);
}
