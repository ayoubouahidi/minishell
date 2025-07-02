/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/01 10:57:38 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static t_env *exist(t_data *data, char *key)
{
	t_env *tmp;

	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static void remove_env(t_data *data, t_env *target)
{
	t_env *tmp;
	t_env *prev;

	if (!data || !target)
		return;
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
			return;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(t_data *data, char **args)
{
	int		i;
	t_env	*found;
	int		exit_status;

	i = 1;
	exit_status = SUCCESS;
	while (args[i])
	{
		if (!is_valid_key(args[i]))
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = FAILURE;
		}
		else
		{
			found = exist(data, args[i]);
			if (found)
				remove_env(data, found);
		}
		i++;
	}
	data->exit_status = exit_status;
	return (exit_status);
}
