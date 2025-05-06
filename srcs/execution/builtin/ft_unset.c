#include "minishell.h"

static bool exist(char **args)
{

}
void    ft_unset(t_data *data, char **args)
{
    int i;

    if (!args[1])
    {
        data->exit_status = 0;
        return ;
    }    
    i = 1;
}