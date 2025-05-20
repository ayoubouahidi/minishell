#include "minishell.h"

// Converts the environment linked list to an array of strings
char **env_to_array(t_env *env, t_gc *gc)
{
    int count = 0;
    t_env *temp = env;
    char **env_array;
    char *entry;

    // Count the number of environment variables
    while (temp)
    {
        count++;
        temp = temp->next;
    }

    // Allocate memory for the array (including NULL terminator)
    env_array = gc_malloc(gc, sizeof(char *) * (count + 1));
    if (!env_array)
        return NULL;

    temp = env;
    for (int i = 0; i < count; i++)
    {
        // Create "key=value" string for each environment variable
        entry = gc_malloc(gc, ft_strlen(temp->key) + ft_strlen(temp->value) + 2);
        if (!entry)
            return NULL;
        sprintf(entry, "%s=%s", temp->key, temp->value);
        env_array[i] = entry;
        temp = temp->next;
    }
    env_array[count] = NULL; // Null-terminate the array

    return env_array;
}
