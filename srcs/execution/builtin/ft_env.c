#include "minishell.h"

int ft_env(t_data *data, char **args)
{
    t_env   *env;

    if (args[1])
    {
        ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
        data->exit_status = FAILURE;
        return (FAILURE);
    }
    
    env = data->env;
    while (env)
    {
        if (env->value)
        {
            ft_putstr_fd(env->key, STDOUT_FILENO);
            ft_putchar_fd('=', STDOUT_FILENO);
            ft_putstr_fd(env->value, STDOUT_FILENO);
        }
        else
        {
            ft_putstr_fd(env->key, STDOUT_FILENO);
        }
        ft_putchar_fd('\n', STDOUT_FILENO);
        env = env->next;
    }
    data->exit_status = SUCCESS;
    return (SUCCESS);
}
