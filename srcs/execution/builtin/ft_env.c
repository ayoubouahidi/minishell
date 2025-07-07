/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/07 07:18:07 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_env(t_data *data, char **args)
{
	t_env	*env;

	if (args[1])
	{
		ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
		g_exit_status = FAILURE;
		return (FAILURE);
	}
	env = data->env;
	while (env)
	{
		if (env->value && env->key)
		{
			ft_putstr_fd(env->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		env = env->next;
	}
	g_exit_status = SUCCESS;
	return (SUCCESS);
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
	t_env	**sorted;
	int		count;
	int		env_count;

	env_count = env_size(env);
	sorted = ft_malloc(sizeof(t_env *) * env_count, 1);
	if (!sorted)
		return ;
	count = 0;
	current = env;
	while (current && count < env_count)
	{
		sorted[count++] = current;
		current = current->next;
	}
	bubble_sort_env(sorted, count);
	print_sorted_env(sorted, count);
}
