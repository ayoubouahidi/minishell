/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/10 09:10:29 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	execute_external_child(t_data *data, char *path)
{
	char			**envp;
	t_command		*cmd;
	struct stat		file_info;

	cmd = data->cmd;
	if (setup_redirections(data->cmd) < 0)
		exit(1);
	envp = env_to_array(data->env);
	execve(path, data->cmd->args, envp);
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (stat(cmd->args[0], &file_info) == -1)
		return ;
	if (S_ISDIR(file_info.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		exit(126);
	}
	perror("minishell");
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
		return ((g_exit_status = 0), 0);
	path = get_command_path(data);
	if (!path)
	{
		g_exit_status = handle_cmd_not_found(data);
		return (g_exit_status);
	}
	pid = fork();
	if (pid == -1)
		return ((perror("minishell: fork"), 1));
	if (pid == 0)
	{
		signal_child_handler();
		execute_external_child(data, path);
	}
	signal(SIGINT, SIG_IGN);
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
			return (FAILURE);
		reset_std_fd(saved_in, saved_out);
		return (SUCCESS);
	}
	if (!data->cmd->next)
		return (g_exit_status = handle_single_cmd(data));
	execute_pipe(data);
	return (g_exit_status);
}
