#include "../../includes/minishell.h"

void execute_pipe(t_data *data)
{
    if (pipe(data->pipe->pipe_fd) == -1)
        exit(1);
    data->pipe->pid_1 = fork();
    if (data->pipe->pid_1 == 0)
    {
        dup2(data->pipe->pipe_fd[1], STDOUT_FILENO);
        close(data->pipe->pipe_fd[0]);
        close(data->pipe->pipe_fd[1]);
        data->pipe->path_1 = get_path(data, data->cmd->args);
        if (!data->pipe->path_1)
            perror("PATH not fond");
        
    }

}
