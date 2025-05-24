#include "../../includes/minishell.h"

char *get_env_value(t_env *env, const char *key)
{
    t_env *current = env;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return (current->value);
        current = current->next;
    }
    return (NULL);
}

t_env *new_env_node(char *key, char *value)
{
    t_env *new = malloc(sizeof(t_env));
    if (!new)
        return (NULL);

    new->key = ft_strdup(key);
    if (value)
        new->value = ft_strdup(value);
    else
        new->value = NULL;

    new->next = NULL;

    if (!new->key || (value && !new->value))
    {
        free(new->key);
        free(new->value);
        free(new);
        return (NULL);
    }

    return (new);
}


char *extract_value(char *str)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (str[i] && str[i] != '=')
        i++;
    
    if (!str[i])
        return (NULL);
    
    i++;
    char *value = malloc(ft_strlen(str + i) + 1);
    if (!value)
        return (NULL);
    while (str[i])
        value[j++] = str[i++];

    value[j] = '\0';
    return (value);
}

char *extract_key(char *str)
{
    int i = 0;

    while (str[i] && str[i] != '=')
        i++;
    char *key = malloc(i + 1);
    if (!key)
        return (NULL);
    i = 0;
    while (str[i] && str[i] != '=')
    {
        key[i] = str[i];
        i++;
    }
    
    key[i] = '\0';
    return (key);
}

void init_env(t_data *data, char **envp)
{
    int i = 0;
    t_env *env = NULL;
    char *key;
    char *value;

    while (envp[i])
    {
        key = extract_key(envp[i]);
        value = extract_value(envp[i]);
        if (key)
        {
            add_env_node(&env, new_env_node(key, value));
            free(key);
            free(value);
        }
        i++;
    }
    data->env = env;
}
