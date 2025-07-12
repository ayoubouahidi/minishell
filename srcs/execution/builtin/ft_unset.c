/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/12 18:22:25 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static bool	is_valid_unset_key(const char *key)
{
	int	i;

	if (!key || !key[0])
		return (false);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (false);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

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
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

static void	process_unset_arg(t_data *data, char *arg)
{
	t_env	*found;

	if (!is_valid_unset_key(arg))
		return ;
	found = exist(data, arg);
	if (found)
		remove_env(data, found);
}

int	ft_unset(t_data *data, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		process_unset_arg(data, args[i]);
		i++;
	}
	g_exit_status = 0;
	return (0);
}
