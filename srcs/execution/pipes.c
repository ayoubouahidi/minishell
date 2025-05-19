#include "minishell.h"

void init_pipe_struct(t_data *data)
{
    data->pipe = malloc(sizeof(t_pipe));
    if (!data->pipe)
    {
        perror("minishell: malloc");
        exit(1);
    }
    data->pipe->pre_fd = -1;
    data->pipe->i = 0;
    data->pipe->current = data->cmd;
    data->pipe->pid = 0;
}

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

    setup_redirections(cmd);
    path = get_path(data, cmd->args[0]);
    if (!path)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd->args[0], STDERR_FILENO);
        ft_putendl_fd(": command not found", STDERR_FILENO);
        exit(127);
    }
    execve(path, cmd->args, env_to_array(data->env));
    perror("minishell: execve");
    free(path);
    exit(126);
}

static void p_cleanup(int *pre_fd, int *fd, t_command **cmd)
{
    if (*pre_fd != -1)
        safe_close(*pre_fd);
    if ((*cmd)->next)
    {
        safe_close(fd[1]);
        *pre_fd = fd[0];
    }
    *cmd = (*cmd)->next;
}

void execute_pipe(t_data *data)
{
    int status;
    int j;
    
    j = 0;
    while (data->pipe->current)
    {
        data->pipe->pid = create_pipe_and_fork(data->pipe->fd, data->pipe->current);
        data->pipe->pids[data->pipe->i++] = data->pipe->pid;

        if (data->pipe->pid == 0)
        {
            setup_child(data->pipe->pre_fd, data->pipe->fd, data->pipe->current);
            execute_child(data, data->pipe->current);
        }
        else
            p_cleanup(&data->pipe->pre_fd, data->pipe->fd, &data->pipe->current);
    }
    while (j < data->pipe->i)
    {
        waitpid(data->pipe->pids[j], &status, 0);
        if (WIFEXITED(status) && j == data->pipe->i - 1)
            data->exit_status = WEXITSTATUS(status);
        j++;
    }
}
