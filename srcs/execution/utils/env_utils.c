#include "../../../includes/minishell.h"


void	update_env(t_env *env, const char *key, const char *new_value)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(new_value);
			return ;
		}
		current = current->next;
	}
}


void	add_env_node(t_env **env, t_env *new_node)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new_node;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}


int	env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}


char	**env_to_array(t_env *env)
{
	int		i;
	char	**arr;
	char	*temp;

	if (!env)
		return (NULL);
	arr = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->key && env->value)
		{
			temp = ft_strjoin(env->key, "=");
			if (temp)
			{
				arr[i] = ft_strjoin(temp, env->value);
				free(temp);
				if (!arr[i])
					return (free_array(arr), NULL);
				i++;
			}
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}


void	sort_and_print_env(t_env *env)
{
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		env = env->next;
	}
}
