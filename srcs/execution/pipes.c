#include "minishell.h"

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

static void setup_child_io(int pre_fd, int *fd, t_command *cmd)
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
    int fd[2];
    int pre_fd = -1;
    pid_t pids[128];
    int i = 0;
    t_command *current = data->cmd;

    while (current)
    {
        pid_t pid = create_pipe_and_fork(fd, current);
        pids[i++] = pid;

        if (pid == 0)
        {
            setup_child_io(pre_fd, fd, current);
            execute_child(data, current);
        }
        else
        {
            parent_cleanup(&pre_fd, fd, &current);
            current = current->next;
        }
    }

    int status;
    int j = 0;
    while (j < i)
    {
        waitpid(pids[j], &status, 0);
        if (WIFEXITED(status) && j == i - 1)
            data->exit_status = WEXITSTATUS(status);
        j++;
    }
}