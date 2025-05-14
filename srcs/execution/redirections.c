#include "minishell.h"

void setup_redirections(t_command *cmd)
{
    int fd;
    char *file;
    int flags;

    if (cmd->infile)
    {
        file = get_file_name(cmd->infile);
        fd = open(file, O_RDONLY);
        if (fd == -1)
        {
            perror("minishell: infile");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
        free(file);
    }

    if (cmd->outfile || cmd->appendfile)
    {
        if (cmd->is_append && cmd->appendfile)
            file = get_file_name(cmd->appendfile);
        else
            file = get_file_name(cmd->outfile);

        flags = O_WRONLY | O_CREAT;
        if (cmd->is_append)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;

        fd = open(file, flags, 0644);
        if (fd == -1)
        {
            perror("minishell: outfile");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
        free(file);
    }
}
