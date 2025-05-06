#include "minishell.h"

void free_cmd(t_cmd *cmd)
{
    t_cmd   *tmp;
    int     i;

    while (cmd)
    {
        tmp = cmd->next;
        i = 0;
        while (cmd->args && cmd->args[i])
            free(cmd->args[i++]);
        free(cmd->args);
        if (cmd->in_fd > 2)
            close(cmd->in_fd);
        if (cmd->out_fd > 2)
            close(cmd->out_fd);
        free(cmd);
        cmd = tmp;
    }
}

void free_env(t_env *env)
{
    t_env   *tmp;

    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

void free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
        free(array[i++]);
    free(array);
}

void clean_exit(t_data *data, int exit_code)
{
    free_cmd(data->cmd);
    free_env(data->env);
    exit(exit_code);
}