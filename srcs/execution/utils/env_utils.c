#include "minishell.h"

void update_env(t_env *env, const char *key, const char *new_value)
{
    t_env *current = env;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = ft_strdup(new_value);
            return;
        }
        current = current->next;
    }
}

void add_env_node(t_env **env, t_env *new_node)
{
    t_env *tmp;

    if (!*env)
    {
        *env = new_node;
        return;
    }

    tmp = *env;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_node;
}
#include "minishell.h"

static void	swap_env(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_val;

	tmp_key = a->key;
	tmp_val = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_val;
}

static void	sort_env(t_env *env)
{
	t_env	*tmp;
	int		swapped;

	if (!env)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		tmp = env;
		while (tmp->next)
		{
			if (ft_strcmp(tmp->key, tmp->next->key) > 0)
			{
				swap_env(tmp, tmp->next);
				swapped = 1;
			}
			tmp = tmp->next;
		}
	}
}

void	sort_and_print_env(t_env *env)
{
	t_env	*copy;
	t_env	*tmp;

	copy = NULL;
	while (env)
	{
		if (env->value)
			add_env_node(&copy, new_env_node(env->key, env->value));
		env = env->next;
	}
	sort_env(copy);
	tmp = copy;
	while (tmp)
	{
		ft_putstr_fd(tmp->key, STDOUT_FILENO);
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putstr_fd(tmp->value, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		tmp = tmp->next;
	}
	free_env(copy);
}
