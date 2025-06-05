#include "minishell.h"

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
    if (data->cmd)
        free_cmd(data->cmd);
    if (data->env)
        free_env(data->env);
    exit(exit_code);
}

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
void cleanup_child_resources(char *path, char **envp)
{
    free(path);
    free_array(envp);
}

void free_cmd(t_command *cmd)
{
    t_command *tmp;
    t_redirections *redir, *redir_tmp;

    while (cmd)
    {
        tmp = cmd->next;

        if (cmd->args)
        {
            int i = 0;
            while (cmd->args[i])
            {
                free(cmd->args[i]);
                i++;
            }
            free(cmd->args);
        }

        if (cmd->here_doc_file)
            free(cmd->here_doc_file);

        redir = cmd->redirections;
        while (redir)
        {
            redir_tmp = redir->next;
            if (redir->file)
                free(redir->file);
            free(redir);
            redir = redir_tmp;
        }

        free(cmd);
        cmd = tmp;
    }
}
