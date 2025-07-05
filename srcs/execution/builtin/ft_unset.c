/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/05 08:20:14 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static t_env	*exist(t_data *data, char *key)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static void	remove_env(t_data *data, t_env *target)
{
	t_env	*tmp;
	t_env	*prev;

	if (!data || !target)
		return ;
	tmp = data->env;
	prev = NULL;
	while (tmp)
	{
		if (tmp == target)
		{
			if (prev)
				prev->next = tmp->next;
			else
				data->env = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

static int	process_unset_arg(t_data *data, char *arg)
{
	t_env	*found;

	if (!is_valid_key(arg))
	{
		ft_putstr_fd("unset: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (FAILURE);
	}
	found = exist(data, arg);
	if (found)
		remove_env(data, found);
	return (SUCCESS);
}

int	ft_unset(t_data *data, char **args)
{
	int		i;
	int		exit_status;

	i = 1;
	exit_status = SUCCESS;
	while (args[i])
	{
		if (process_unset_arg(data, args[i]) == FAILURE)
			exit_status = FAILURE;
		i++;
	}
	data->exit_status = exit_status;
	g_exit_status = exit_status;
	return (exit_status);
}
