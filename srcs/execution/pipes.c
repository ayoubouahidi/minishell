#include "minishell.h"

static void setup_redirections(t_command *cmd)
{
    int fd;

    if (cmd->infile)
    {
        fd = open(cmd->infile, O_RDONLY);
        if (fd == -1)
        {
            perror("infile");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (cmd->outfile)
    {
        int flags = O_WRONLY | O_CREAT | (cmd->is_append ? O_APPEND : O_TRUNC);
        fd = open(cmd->outfile, flags, 0644);
        if (fd == -1)
        {
            perror("outfile");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

static void execute_child(t_data *data, int pre_fd, int *fd)
{
    char *path;
    char **envp = env_to_array(data->env);

    if (pre_fd != 0)
    {
        dup2(pre_fd, STDIN_FILENO);
        close(pre_fd);
    }

    if (data->cmd->next && !data->cmd->outfile)
        dup2(fd[1], STDOUT_FILENO);

    close(fd[0]);
    close(fd[1]);

    setup_redirections(data->cmd);

    if (ft_strchr(data->cmd->args[0], '/') && access(data->cmd->args[0], X_OK) == 0)
        path = ft_strdup(data->cmd->args[0]);
    else
        path = get_path(data, data->cmd->args[0]);

    if (!path)
    {
        perror("command not found");
        exit(127);
    }

    execve(path, data->cmd->args, envp);
    perror("execve");
    exit(1);
}

void execute_pipe(t_data *data)
{
    int fd[2];
    int pre_fd = 0;
    pid_t pid;

    while (data->cmd)
    {
        if (pipe(fd) == -1)
        {
            perror("pipe");
            exit(1);
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid == 0)
            execute_child(data, pre_fd, fd);
        else
        {
            close(fd[1]);
            if (pre_fd != 0)
                close(pre_fd);
            pre_fd = fd[0];
            waitpid(pid, &data->exit_status, 0);
            data->cmd = data->cmd->next;
        }
    }
}
