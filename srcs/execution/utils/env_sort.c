#include "../../../includes/minishell.h"

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
