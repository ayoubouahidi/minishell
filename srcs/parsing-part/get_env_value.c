











#include "../../includes/minishell.h"
#include "../libft/libft.h"

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
