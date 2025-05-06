#include "minishell.h"

static bool has_n_option(char **args, int *i)
{
    bool    n_option;
    int     j;

    n_option = false;
    while (args[*i] && args[*i][0] == '-' && args[*i][1] == 'n')
    {
        j = 2;
        while (args[*i][j] == 'n')
            j++;
        if (args[*i][j] == '\0')
        {
            n_option = true;
            (*i)++;
        }
        else
            break;
    }
    return (n_option);
}

int ft_echo(t_data *data, char **args)
{
    int     i;
    bool    n_option;

    i = 1;
    n_option = has_n_option(args, &i);
    
    while (args[i])
    {
        ft_putstr_fd(args[i], STDOUT_FILENO);
        if (args[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }
    
    if (!n_option)
        ft_putchar_fd('\n', STDOUT_FILENO);
    
    data->exit_status = SUCCESS;
    return (SUCCESS);
}