/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/05 11:39:33 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_command_path(t_data *data)
{
	char	*path;

	if (!data->cmd->args || !data->cmd->args[0])
		return (NULL);
	if (ft_strchr(data->cmd->args[0], '/'))
	{
		if (access(data->cmd->args[0], F_OK) != 0)
			return (NULL);
		if (access(data->cmd->args[0], X_OK) != 0)
			return (NULL);
		return (ft_strdup(data->cmd->args[0]));
	}
	path = get_path(data, data->cmd->args[0]);
	return (path);
}

static void	handle_cmd_not_found(t_data *data)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(data->cmd->args[0], STDERR_FILENO);
	if (ft_strchr(data->cmd->args[0], '/'))
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	else
		ft_putendl_fd(": command not found", STDERR_FILENO);
}

static void	execute_external_child(t_data *data, char *path)
{
	char	**envp;

	signal_child_handler();
	if (setup_redirections(data->cmd) < 0)
		exit(1);
	envp = env_to_array(data->env);
	execve(path, data->cmd->args, envp);
	perror("minishell: execve");
	cleanup_child_resources(path, envp);
	exit(126);
}

static int	launch_external_command(t_data *data)
{
	char	*path;
	pid_t	pid;
	int		status;

	path = get_command_path(data);
	if (!path)
	{
		handle_cmd_not_found(data);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		free(path);
		return (1);
	}
	if (pid == 0)
		execute_external_child(data, path);
	waitpid(pid, &status, 0);
	g_exit_status = WEXITSTATUS(status);
	free(path);
	return (g_exit_status);
}

int	executer(t_data *data)
{
	int	redir_status;

	if (!data->cmd)
		return (FAILURE);
	if (!data->cmd->args || !data->cmd->args[0])
	{
		if (setup_redirections(data->cmd) < 0)
			return (FAILURE);
		return (SUCCESS);
	}
	if (!data->cmd->next)
	{
		if (is_builtin(data->cmd->args[0]))
		{
			redir_status = setup_redirections(data->cmd);
			if (redir_status < 0)
				return (FAILURE);
			handle_builtin(data);
			return (g_exit_status);
		}
		g_exit_status = launch_external_command(data);
		return (g_exit_status);
	}
	execute_pipe(data);
	return (g_exit_status);
}
