/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/07 08:43:28 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_cmd_not_found(t_data *data)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (data->cmd->args && data->cmd->args[0])
	{
		ft_putstr_fd(data->cmd->args[0], STDERR_FILENO);
		if (ft_strchr(data->cmd->args[0], '/'))
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		else
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else
		ft_putstr_fd("command not found\n", STDERR_FILENO);
}

static void	execute_external_child(t_data *data, char *path)
{
	char	**envp;

	if (setup_redirections(data->cmd) < 0)
		exit(1);
	envp = env_to_array(data->env);
	execve(path, data->cmd->args, envp);
	perror("minishell: execve");
	exit(126);
}

static void	handle_signal_status(int status)
{
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
	if (g_exit_status == 130)
		printf("\n");
	if (g_exit_status == 131)
		printf("Quit (core dumped)\n");
}

int	launch_external_command(t_data *data)
{
	char	*path;
	pid_t	pid;
	int		status;

	if (!data->cmd->args || !data->cmd->args[0])
		return (0);
	path = get_command_path(data);
	if (!path)
	{
		handle_cmd_not_found(data);
		return (127);
	}
	pid = fork();
	if (pid == -1)
		return ((perror("minishell: fork"), 1));
	if (pid == 0)
		(signal_child_handler(), execute_external_child(data, path));
	signal (SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal_parent_handler();
	handle_signal_status(status);
	return (g_exit_status);
}

int	executer(t_data *data)
{
	int	saved_in;
	int	saved_out;

	if (!data->cmd)
		return (FAILURE);
	if (!data->cmd->args || !data->cmd->args[0])
	{
		if (save_std_fd(&saved_in, &saved_out) < 0)
			return (FAILURE);
		if (setup_redirections(data->cmd) < 0)
			return (reset_std_fd(saved_in, saved_out), FAILURE);
		reset_std_fd(saved_in, saved_out);
		return (SUCCESS);
	}
	if (!data->cmd->next)
		return (handle_single_cmd(data));
	execute_pipe(data);
	return (g_exit_status);
}
