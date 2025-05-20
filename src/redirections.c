#include "minishell.h"

// ...existing code...

int handle_output_redirection(char *file_path)
{
    int fd;
    
    fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("minishell");
        return (1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell");
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}

int handle_redirections(t_command *cmd)
{
    // ...existing code...
    
    // Important: Process input redirections first, then output redirections
    // This ensures output files are created even if input file doesn't exist
    
    // First handle all output redirections
    for (int i = 0; i < cmd->num_redirections; i++)
    {
        if (cmd->redirections[i].type == REDIR_OUT || 
            cmd->redirections[i].type == REDIR_APPEND)
        {
            // Handle output redirection
            if (handle_output_redirection(cmd->redirections[i].file) != 0)
                return (1);
        }
    }
    
    // Then handle input redirections
    for (int i = 0; i < cmd->num_redirections; i++)
    {
        if (cmd->redirections[i].type == REDIR_IN)
        {
            // Handle input redirection
            if (handle_input_redirection(cmd->redirections[i].file) != 0)
                return (1);
        }
    }
    
    // ...existing code...
}

// ...existing code...