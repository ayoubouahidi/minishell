/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/06 12:32:24 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

bool	is_valid_key(const char *key)
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

static void	print_invalid(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

static int	process_export_arg(t_data *data, char *arg)
{
	char	*key;
	char	*val_part;
	char	*value;

	key = extract_key(arg);
	if (!is_valid_key(key))
	{
		print_invalid(arg);
		return (FAILURE);
	}
	val_part = ft_strchr(arg, '=');
	if (val_part)
		value = ft_strdup(val_part + 1);
	else
		value = ft_strdup("");
	if (!value)
	{
		return (FAILURE);
	}
	update_or_add_env(&data->env, key, value);
	return (SUCCESS);
}

int	ft_export(t_data *data, char **args)
{
	int	i;
	int	exit_status;

	if (!args[1])
	{
		sort_and_print_env(data->env);
		g_exit_status = SUCCESS;
		return (SUCCESS);
	}
	i = 1;
	exit_status = SUCCESS;
	while (args[i])
	{
		if (process_export_arg(data, args[i]) == FAILURE)
			exit_status = FAILURE;
		i++;
	}
	g_exit_status = exit_status;
	return (exit_status);
}
