/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/11 19:47:17 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

bool	is_valid_key(const char *arg)
{
	int	i;

	if (!arg || !arg[0])
		return (false);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (false);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if ((arg[i + 1] == '\0' && (arg[i] == '-' || arg[i] == '-'))
			|| (arg[i - 1] == '\0' && (arg[i] == '-' || arg[i] == '-')))
			return (false);
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
		i++;
	}
	if (arg[i] == '=' && arg[i - 1] == '-')
		return (false);
	return (true);
}

static char	*extract_key_until_equal(char *arg)
{
	int		i;
	char	*key;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	key = ft_substr(arg, 0, i);
	return (key);
}

static int	process_export_arg(t_data *data, char *arg)
{
	char	*key;
	char	*val_part;
	char	*value;

	if (!is_valid_key(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return ((g_exit_status = 1), FAILURE);
	}
	key = extract_key_until_equal(arg);
	if (!key)
		return (FAILURE);
	val_part = ft_strchr(arg, '=');
	if (val_part)
	{
		value = ft_strdup(val_part + 1);
		if (!value)
			return (FAILURE);
		update_or_add_env(&data->env, key, value);
	}
	else
		update_or_add_env(&data->env, key, NULL);
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
