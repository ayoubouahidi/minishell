#include "../../includes/minishell.h"

static int safe_open(const char *file, int flags, mode_t mode)
{
    if (!file)
        return (-1);
    int fd = open(file, flags, mode);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        perror(file);
    }
    return fd;
}

static int open_input_file(char *filename)
{
    int fd;
    char *file;
    
    if (!filename)
        return (-1);
        
    file = get_file_name(filename);
    if (!file)
        return (-1);

    fd = safe_open(file, O_RDONLY, 0);
    free(file);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        perror(filename);
        exit(1);
    }
    return fd;
}

static int open_output_file(t_command *cmd)
{
    int fd;
    int flags = O_WRONLY | O_CREAT;
    char *file;

    if (!cmd)
        return (-1);

    if (cmd->is_append && cmd->appendfile)
        file = get_file_name(cmd->appendfile);
    else if (cmd->outfile)
        file = get_file_name(cmd->outfile);
    else
        return (-1);
        
    if (!file)
        return (-1);
        
    if (cmd->is_append)
        flags |= O_APPEND;
    else
        flags |= O_TRUNC;
    fd = safe_open(file, flags, 0644);
    free(file);
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        perror("redirections");
        exit(1);
    }
    return fd;
}

void setup_redirections(t_command *cmd)
{
    int fd;

    if (!cmd)
        return;

    // Handle output redirection first to ensure the file is created
    if (cmd->outfile || (cmd->is_append && cmd->appendfile))
    {
        fd = open_output_file(cmd);
        if (fd != -1)
        {
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }
            close(fd);
        }
    }

    // Handle heredoc redirection
    if (cmd->is_heredoc && cmd->del)
    {
        fd = setup_heredoc(cmd);
        if (fd == -1)
            exit(1);
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            exit(1);
        }
        close(fd);
    }

    // Handle input redirection
    if (cmd->infile)
    {
        fd = open_input_file(cmd->infile);
        if (fd != -1)
        {
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }
            close(fd);
        }
    }
}


