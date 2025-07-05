/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 07:04:55 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/05 08:42:38 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	handle_builtin(t_data *data)
{
	int	status;
	int	saved_in;
	int	saved_out;

	if (save_std_fd(&saved_in, &saved_out) < 0)
		return (1);
	if (setup_redirections(data->cmd) < 0)
	{
		reset_std_fd(saved_in, saved_out);
		return (1);
	}
	status = execute_builtin(data);
	reset_std_fd(saved_in, saved_out);
	return (status);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset"));
}

int	execute_builtin(t_data *data)
{
	char	*cmd;

	cmd = data->cmd->args[0];
	if (!ft_strcmp(cmd, "cd"))
		return (ft_cd(data, data->cmd->args));
	if (!ft_strcmp(cmd, "echo"))
		return (ft_echo(data->cmd->args));
	if (!ft_strcmp(cmd, "exit"))
		return (ft_exit(data, data->cmd->args));
	if (!ft_strcmp(cmd, "pwd"))
		return (ft_pwd(data));
	if (!ft_strcmp(cmd, "env"))
		return (ft_env(data, data->cmd->args));
	if (!ft_strcmp(cmd, "export"))
		return (ft_export(data, data->cmd->args));
	if (!ft_strcmp(cmd, "unset"))
		return (ft_unset(data, data->cmd->args));
	return (0);
}
