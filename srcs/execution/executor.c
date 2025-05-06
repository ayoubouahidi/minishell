#include "../../includes/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") || 
           !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "pwd") ||
           !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "export") ||
           !ft_strcmp(cmd, "unset"));
}

int execute_builtin(t_data *data)
{
    char *cmd = data->cmd->args[0];
    
    if (!ft_strcmp(cmd, "cd"))
    {
        printf("test done\n");
        return ft_cd(data, data->cmd->args);
    }
    if (!ft_strcmp(cmd, "echo"))
        return ft_echo(data, data->cmd->args);
    if (!ft_strcmp(cmd, "exit"))
        return ft_exit(data, data->cmd->args);
    if (!ft_strcmp(cmd, "pwd"))
        return ft_pwd(data);
    if (!ft_strcmp(cmd, "env"))
        return ft_env(data, data->cmd->args);
    if (!ft_strcmp(cmd, "export"))
        return ft_export(data, data->cmd->args);
    return 0;
}

void executer(t_data *data, char **envp)
{
    (void)envp;
    if (is_builtin(data->cmd->args[0]))
        data->exit_status = execute_builtin(data);
    // Add else clause here if you have external command execution
}