#include "../../includes/minishell.h"


static void safe_close(int fd)
{
    if (fd >= 0)
        close(fd);
}


static pid_t create_pipe_and_fork(int *fd, t_command *cmd)
{
    pid_t pid;

    
    if (cmd->next && pipe(fd) == -1)
    {
        perror("minishell: pipe");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        exit(EXIT_FAILURE);
    }
    return pid;
}


static void setup_child(int pre_fd, int *fd, t_command *cmd)
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


static void execute_child(t_data *data, t_command *cmd)
{
	char *path;
	char **envp;

	if (setup_redirections(cmd) < 0)
		exit(1);
		
	if (cmd->args && is_builtin(cmd->args[0]))
	{
		execute_builtin(data);
		exit(data->exit_status);
	}
	
	if (!cmd->args || !cmd->args[0])
		exit(0);
		
	path = get_path(data, cmd->args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(127);
	}
	envp = env_to_array(data->env);
	execve(path, cmd->args, envp);
	perror("minishell: execve");
	free(path);
	free_array(envp);
	exit(126);
}


static void parent_cleanup(int *pre_fd, int *fd, t_command **cmd)
{
    if (*pre_fd != -1)
        safe_close(*pre_fd);
    if ((*cmd)->next)
    {
        safe_close(fd[1]);
        *pre_fd = fd[0];
    }
}


void execute_pipe(t_data *data)
{
	t_pipe	p;
	int		status;
	int		j;

	
	p.pre_fd = -1;
	p.i = 0;
	p.cur = data->cmd;
	
	
	while (p.cur)
	{
		p.pid = create_pipe_and_fork(p.fd, p.cur);
		p.pids[p.i++] = p.pid;
		
		if (p.pid == 0)
			setup_child(p.pre_fd, p.fd, p.cur), execute_child(data, p.cur);
		else
			
			parent_cleanup(&p.pre_fd, p.fd, &p.cur), p.cur = p.cur->next;
	}
	
	
	j = 0;
	while (j < p.i)
	{
		waitpid(p.pids[j], &status, 0);
		
		if (WIFEXITED(status) && j == p.i - 1)
			data->exit_status = WEXITSTATUS(status);
		j++;
	}
}
