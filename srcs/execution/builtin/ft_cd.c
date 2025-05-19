#include "minishell.h"

int ft_cd(t_data *data, char **args)
{
    char    cwd[PATH_MAX];
    char    *path;
    char    *oldpwd;

    oldpwd = getcwd(NULL, 0);
    if (!oldpwd)
        return (perror("cd"), FAILURE);
    
    if (!args[1] || ft_strcmp(args[1], "~") == 0)
        path = get_env_value(data->env, "HOME");
    else if (ft_strcmp(args[1], "-") == 0)
        path = get_env_value(data->env, "OLDPWD");
    else
        path = args[1];
    
    if (!path)
        return (free(oldpwd), ft_putstr_fd("minishell: cd: HOME not set\n", 2), FAILURE);
    
    if (chdir(path) == -1)
    {
        free(oldpwd);
        ft_putstr_fd("minishell: cd: ", 2);
        perror(path);
        return (FAILURE);
    }
    
    update_env(data->env, "OLDPWD", oldpwd);
    if (!getcwd(cwd, PATH_MAX))
        return (free(oldpwd), perror("cd"), FAILURE);
    update_env(data->env, "PWD", cwd);
    
    free(oldpwd);
    data->exit_status = SUCCESS;
    return (SUCCESS);
}

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
