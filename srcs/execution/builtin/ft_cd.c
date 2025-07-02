/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/01 10:57:38 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

static int	handle_cd_error(char *oldpwd, char *error_msg, t_data *data)
{
	free(oldpwd);
	ft_putstr_fd(error_msg, 2);
	data->exit_status = FAILURE;
	return (FAILURE);
}

static char	*get_target_path(t_data *data, char **args, char *oldpwd)
{
	char	*path;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		path = get_env_value(data->env, "HOME");
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value(data->env, "OLDPWD");
		if (!path)
		{
			handle_cd_error(oldpwd, "minishell: cd: OLDPWD not set\n", data);
			return (NULL);
		}
	}
	else
		path = args[1];
	if (!path)
	{
		handle_cd_error(oldpwd, "minishell: cd: HOME not set\n", data);
		return (NULL);
	}
	return (path);
}

static int	change_directory(char *path, char *oldpwd, t_data *data)
{
	char	cwd[PATH_MAX];

	if (chdir(path) == -1)
	{
		free(oldpwd);
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		data->exit_status = FAILURE;
		return (FAILURE);
	}
	update_env(data->env, "OLDPWD", oldpwd);
	if (!getcwd(cwd, PATH_MAX))
		return (free(oldpwd), perror("cd"), FAILURE);
	update_env(data->env, "PWD", cwd);
	return (SUCCESS);
}

int	ft_cd(t_data *data, char **args)
{
	char	*path;
	char	*oldpwd;

	if (count_args(args) > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		data->exit_status = FAILURE;
		return (FAILURE);
	}
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("cd"), FAILURE);
	path = get_target_path(data, args, oldpwd);
	if (!path)
		return (FAILURE);
	if (change_directory(path, oldpwd, data) == FAILURE)
		return (FAILURE);
	free(oldpwd);
	data->exit_status = SUCCESS;
	return (SUCCESS);
}
