#include "minishell.h"

static pid_t	create_pipe_and_fork(t_command *cmd, int *fd)
{
	if (cmd->next && pipe(fd) == -1)
	{
		perror("minishell: pipe");
		exit(EXIT_FAILURE);
	}
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

static void	child_exec(t_data *data, t_command *cmd, int pre_fd, int *fd)
{
	char *path;

	if (pre_fd != -1)
	{
		dup2(pre_fd, STDIN_FILENO);
		close(pre_fd);
	}
	if (cmd->next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	setup_redirections(cmd);
	path = get_path(data, cmd->args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	execve(path, cmd->args, env_to_array(data->env));
	perror("minishell: execve");
	exit(126);
}

static void	update_parent_state(t_data *data)
{
	t_pipe *p = data->pipe;

	p->pids[p->i++] = p->pid;
	if (p->pre_fd != -1)
		close(p->pre_fd);
	if (p->current->next)
	{
		close(p->fd[1]);
		p->pre_fd = p->fd[0];
	}
	p->current = p->current->next;
}

void	execute_pipe(t_data *data)
{
	t_pipe  *p;
    int     j;
    int     status;
    
    p = data->pipe;
	p->pre_fd = -1;
	p->i = 0;
	p->current = data->cmd;
	while (p->current)
	{
		p->pid = create_pipe_and_fork(p->current, p->fd);
		if (p->pid == 0)
			child_exec(data, p->current, p->pre_fd, p->fd);
		else
			update_parent_state(data);
	}
    j = 0;
	while (j < data->pipe->i)
	{
		waitpid(data->pipe->pids[j], &status, 0);
		if (WIFEXITED(status) && j == data->pipe->i - 1)
			data->exit_status = WEXITSTATUS(status);
		j++;
	}
}
