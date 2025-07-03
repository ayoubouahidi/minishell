/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/03 18:59:09 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

static int	handle_builtin(t_data *data)
{
	int	status;
	
	if (setup_redirections(data->cmd) < 0)
		return (1);
	status = execute_builtin(data);
	return (status);
}


static char	*get_command_path(t_data *data)
{
	char	*path;

	if (ft_strchr(data->cmd->args[0], '/'))
	{
		if (access(data->cmd->args[0], F_OK) != 0)
			return (NULL);
		if (access(data->cmd->args[0], X_OK) != 0)
			return (NULL);
		return (ft_strdup(data->cmd->args[0]));
	}
	if (access(data->cmd->args[0], X_OK) == 0)
		path = ft_strdup(data->cmd->args[0]);
	else
		path = get_path(data, data->cmd->args[0]);
	return (path);
}


static int	launch_external_command(t_data *data)
{
	char	*path;
	char	**envp;
	pid_t	pid_ch;

	path = get_command_path(data);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(data->cmd->args[0], STDERR_FILENO);
		if (ft_strchr(data->cmd->args[0], '/'))
			ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		else
			ft_putendl_fd(": command not found", STDERR_FILENO);
		return (127);
	}
	pid_ch = fork();
	if (pid_ch == -1)
	{
		perror("minishell: fork");
		free(path);
		return (1);
	}
	if (pid_ch == 0)
	{
		if (setup_redirections(data->cmd) < 0)
			exit(1);
		envp = env_to_array(data->env);
		execve(path, data->cmd->args, envp);
		perror("minishell: execve");
		cleanup_child_resources(path, envp);
		exit(126);
	}
	waitpid(pid_ch, &data->exit_status, 0);
	free(path);
	return (WEXITSTATUS(data->exit_status));
}

void	executer(t_data *data, char **envp)
{
	(void)envp;
	int	saved_in;
	int	saved_out;

	if (!data->cmd)
		return ;
	if (!data->cmd->args || !data->cmd->args[0])
	{
		save_std_fd(&saved_in, &saved_out);
		if (setup_redirections(data->cmd) < 0)
			data->exit_status = FAILURE;
		reset_std_fd(saved_in, saved_out);
		return ;
	}
	if (!data->cmd->next)
	{
		if (is_builtin(data->cmd->args[0]))
		{
			data->exit_status = handle_builtin(data);
			return ;
		}
		data->exit_status = launch_external_command(data);
		return ;
	}
	execute_pipe(data);
}
