#include "minishell.h"

int setup_heredoc(t_command *cmd)
{
    int fd[2];
    char *line;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return -1;
    }
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strcmp(line, cmd->del) == 0)
        {
            free(line);
            break;
        }
        ft_putstr_fd(line, fd[1]);
        write(fd[1], "\n", 1);
        free(line);
    }
    close(fd[1]);
    return fd[0];
}

