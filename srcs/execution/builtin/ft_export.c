#include "minishell.h"

bool	is_valid_key(const char *key)
{
	int	i;

	if (!key || !key[0])
		return (false);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (false);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static void	print_invalid(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

static void	update_or_add_env(t_data *data, char *key, char *val)
{
	t_env	*env;

	env = data->env;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(val);
			return ;
		}
		env = env->next;
	}
	add_env_node(&data->env, new_env_node(key, val));
}

int	ft_export(t_data *data, char **args)
{
	char	*key;
	char	*val;
	int		i;

	if (!args[1])
		return (sort_and_print_env(data->env), SUCCESS);
	i = 1;
	key = extract_key(args[i]);
	while (args[i])
	{
		if (!ft_strchr(args[i], '=') || !is_valid_key(key))
			free(key), print_invalid(args[i]);
		else
		{
			val = extract_value(args[i]);
			if (!is_valid_key(key))
				print_invalid(args[i]);
			else
				update_or_add_env(data, key, val);
			free(key);
			free(val);
		}
		i++;
	}
	data->exit_status = SUCCESS;
	return (SUCCESS);
}
