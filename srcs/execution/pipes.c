#include "../../includes/minishell.h"

void execute_pipe(t_data *data)
{
    pid_t   pid_ch;
    int     fd[2];
    int     pre_fd = 0;
    char    *path;

    while (data->cmd)
    {
        if (pipe(fd) == -1)
            perror("pipe");
        
        pid_ch = fork();
        if (pid_ch == -1)
            perror("fork failed");
        
        if (pid_ch == 0)
        {
            if (pre_fd != 0)
            {
                dup2(pre_fd, STDIN_FILENO);
                close(pre_fd);
            }
            if (data->cmd->next)
                dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            path = get_path(data, data->cmd->args[0]);
            if (execve(path, data->cmd->args, NULL) == -1)
            {
                perror("execve");
                exit(1);
            }
        }
        else
        {
            close(fd[1]);
            if (pre_fd != 0)
                close(pre_fd);
            pre_fd = fd[0];
            data->cmd = data->cmd->next;
            waitpid(pid_ch, NULL, 0);
        }
    }
}
