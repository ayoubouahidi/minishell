#include "../../../includes/minishell.h"

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

static int	process_export_arg(t_data *data, char *arg)
{
	char	*key;
	char	*val;

	key = extract_key(arg);
	if (!is_valid_key(key))
	{
		print_invalid(arg);
		free(key);
		return (FAILURE);
	}
	else if (ft_strchr(arg, '='))
	{
		val = extract_value(arg);
		update_or_add_env(data, key, val);
		free(val);
		free(key);
	}
	else
		free(key);
	return (SUCCESS);
}

int	ft_export(t_data *data, char **args)
{
	int	i;
	int	exit_status;

	if (!args[1])
	{
		sort_and_print_env(data->env);
		data->exit_status = SUCCESS;
		return (SUCCESS);
	}
	i = 1;
	exit_status = SUCCESS;
	while (args[i])
	{
		if (process_export_arg(data, args[i]) == FAILURE)
			exit_status = FAILURE;
		i++;
	}
	data->exit_status = exit_status;
	return (exit_status);
}
