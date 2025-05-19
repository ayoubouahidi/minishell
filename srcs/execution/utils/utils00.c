#include "minishell.h"

static char *search_path(char **paths, char *cmd)
{
    int i = 0;
    char *tmp;
    char *full_path;

    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(full_path, X_OK) == 0)
            return full_path;
        free(full_path);
        i++;
    }
    return NULL;
}

char *get_path(t_data *data, char *cmd)
{
    char **paths;
    char *env_path;
    char *final_path;

    if (!cmd || !data || !data->env)
        return NULL;
    env_path = get_env_value(data->env, "PATH");
    if (!env_path)
        return NULL;
    paths = ft_split(env_path, ':');
    if (!paths)
        return NULL;
    final_path = search_path(paths, cmd);
    free_array(paths);
    return final_path;
}
