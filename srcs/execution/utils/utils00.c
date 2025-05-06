#include "minishell.h"

void free_all(t_data *data)
{
    if (data->cmd)
    {
        for (int i = 0; data->cmd->args[i]; i++)
            free(data->cmd->args[i]);
        free(data->cmd->args);
        free(data->cmd);
    }
}
