#include "../../includes/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return 0;
    return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") ||
            !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "pwd") ||
            !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "export") ||
            !ft_strcmp(cmd, "unset"));
}

int execute_builtin(t_data *data)
{
    char *cmd = data->cmd->args[0];

    if (!ft_strcmp(cmd, "cd"))
        return ft_cd(data, data->cmd->args);
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
    if (!ft_strcmp(cmd, "unset"))
        return ft_unset(data, data->cmd->args);
    return 0;
}

int external_command(t_data *data)
{
    pid_t pid_ch;
    char *path;

    path = get_path(data, data->cmd->args[0]);
    if (!path)
    {
        perror("command not found");
        return 127;
    }
    pid_ch = fork();
    if (pid_ch == -1)
        return (perror("fork"), 1);
    if (pid_ch == 0)
    {
        if (execve(path, data->cmd->args, NULL) == -1)
        {
            perror("execve");
            exit(1);
        }
    }
    else
        waitpid(pid_ch, &data->exit_status, 0);

    free(path);
    return WEXITSTATUS(data->exit_status);
}

void executer(t_data *data, char **envp)
{
    (void)envp;
    if (is_builtin(data->cmd->args[0]) && !data->cmd->next)
        data->exit_status = execute_builtin(data);
    if (!is_builtin(data->cmd->args[0]) && !data->cmd->next)
        data->exit_status = external_command(data);
    if (data->cmd && data->cmd->next)
        execute_pipe(data);
}
