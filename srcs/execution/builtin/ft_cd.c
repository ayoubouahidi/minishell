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