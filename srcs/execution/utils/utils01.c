/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils01.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/02 09:47:12 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int save_std_fd(int *saved_in, int *saved_out)
{
    *saved_in = dup(STDIN_FILENO);
    *saved_out = dup(STDOUT_FILENO);
    if (*saved_in < 0 || *saved_out < 0)
        return (-1);
    return (0);
}

/* restore original stdin/stdout fds */
void reset_std_fd(int saved_in, int saved_out)
{
    dup2(saved_in, STDIN_FILENO);
    dup2(saved_out, STDOUT_FILENO);
    close(saved_in);
    close(saved_out);
}

int	is_builtin(char *cmd)
{
	
	if (!cmd)
		return (0);
	
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") ||
		!ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "pwd") ||
		!ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "unset"));
}


int	execute_builtin(t_data *data)
{
	char	*cmd;
	
	cmd = data->cmd->args[0];
	if (!ft_strcmp(cmd, "cd"))
		return ft_cd(data, data->cmd->args);
	if (!ft_strcmp(cmd, "echo"))
		return ft_echo(data->cmd->args);
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

void	clean_exit(t_data *data, int exit_code)
{
	if (data->cmd)
		free_cmd(data->cmd);
	if (data->env)
		free_env(data->env);
	g_exit_status = exit_code;
	exit(exit_code);
}
