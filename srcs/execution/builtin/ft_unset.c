#include "minishell.h"

static t_env *exist(t_data *data, char *key)
{
	t_env *tmp;

	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static void remove_env(t_data *data, t_env *target)
{
	t_env *tmp;
	t_env *prev;

	if (!data || !target)
		return;
	tmp = data->env;
	prev = NULL;
	while (tmp)
	{
		if (tmp == target)
		{
			if (prev)
				prev->next = tmp->next;
			else
				data->env = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(t_data *data, char **args)
{
	int		i;
	t_env	*found;

	i = 1;
	while (args[i])
	{
		if (!is_valid_key(args[i]))
		{
			printf("unset: `%s': not a valid identifier\n", args[i]);
			data->exit_status = 1;
		}
		else
		{
			found = exist(data, args[i]);
			if (found)
				remove_env(data, found);
		}
		i++;
	}
	return (SUCCESS);
}