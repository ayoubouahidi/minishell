#include "minishell.h"

int ft_pwd(t_data *data)
{
    char    cwd[PATH_MAX];

    if (getcwd(cwd, PATH_MAX))
    {
        ft_putstr_fd(cwd, STDOUT_FILENO);
        ft_putchar_fd('\n', STDOUT_FILENO);
        data->exit_status = SUCCESS;
        return (SUCCESS);
    }
    perror("minishell: pwd");
    data->exit_status = FAILURE;
    return (FAILURE);
}