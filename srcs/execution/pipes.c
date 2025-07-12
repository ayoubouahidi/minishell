/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/11 09:04:06 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static pid_t	create_pipe_and_fork(int *fd, t_command *cmd)
{
	pid_t	pid;

	if (cmd->next && pipe(fd) == -1)
	{
		perror("minishell: pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		signal_child_handler();
		perror("minishell: fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

static void	setup_child_io(int pre_fd, int *fd, t_command *cmd)
{
	if (pre_fd != -1)
	{
		dup2(pre_fd, STDIN_FILENO);
		safe_close(pre_fd);
	}
	if (cmd->next)
	{
		safe_close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		safe_close(fd[1]);
	}
}

static void	exec_ch_process(t_data *data, t_command *cmd)
{
	char	*path;
	char	**envp;

	if (setup_redirections(cmd) < 0)
		exit(1);
	if (cmd->args && is_builtin(cmd->args[0]))
	{
		execute_builtin(data, cmd);
		exit(g_exit_status);
	}
	if (!cmd->args || !cmd->args[0])
		exit(0);
	path = get_path(data, cmd->args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found", STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		exit(127);
	}
	envp = env_to_array(data->env);
	execve(path, cmd->args, envp);
	perror("minishell: execve");
	exit(126);
}

static void	wait_for_children(t_pipe *p, t_data *data)
{
	int	j;
	int	status;

	(void)data;
	j = 0;
	while (j < p->i)
	{
		waitpid(p->pids[j], &status, 0);
		if (j == p->i - 1)
		{
			if (WIFSIGNALED(status))
				g_exit_status = 128 + WTERMSIG(status);
			else if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
		}
		j++;
	}
}

void	execute_pipe(t_data *data)
{
	t_pipe	p;

	p.pre_fd = -1;
	p.i = 0;
	p.cur = data->cmd;
	while (p.cur)
	{
		p.pid = create_pipe_and_fork(p.fd, p.cur);
		p.pids[p.i] = p.pid;
		p.i++;
		if (p.pid == 0)
		{
			signal_child_handler();
			setup_child_io(p.pre_fd, p.fd, p.cur);
			exec_ch_process(data, p.cur);
		}
		else
		{
			parent_cleanup(&p.pre_fd, p.fd, &p.cur);
			p.cur = p.cur->next;
		}
	}
	signal(SIGINT, SIG_IGN);
	wait_for_children(&p, data);
	signal_parent_handler();
}
