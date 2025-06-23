#include "../../../includes/minishell.h"

/*
 * HNA KANحدثوU ENVIRONMENT VARIABLE
 * 
 * Ashno katdir:
 * 1. Kandouro f environment linked list
 * 2. Kanقارنو l key m3a koll node
 * 3. Ila l9ina l key → kanحذف l old value o kanضع l new value
 * 4. Ila mal9يناهx → makanديروش والو (variable جديدة تتضاف b add_env_node)
 * 
 * Mathal: export USER="newuser"
 * - Kanقلب 3la USER f environment
 * - Kanبدل l value من "olduser" l "newuser"
 */
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

/*
 * HNA KANضيفوU NODE JDIDA L ENVIRONMENT
 * 
 * Ashno katdir:
 * 1. Ila l environment فارغ → l new node يولي l head
 * 2. Ila makanx فارغ → kanمشي l akhir node o kanضيف l new node
 * 
 * Environment structure: linked list
 * - Koll node فيه key, value, o pointer l next node
 * - Mathalan: USER="stoof" → HOME="/home/stoof" → PATH="/bin:/usr/bin" → NULL
 */
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

/*
 * HNA KAN3دوU L ENVIRONMENT VARIABLES
 * 
 * Ashno katdir:
 * 1. Kandouro f environment linked list
 * 2. Kan3د koll node (variable)
 * 3. Kanرج3و l total count
 * 
 * استعملوها bash n3rf كم variable 3ndna
 * - F env_to_array() bash nعملو malloc للarray
 * - F export bash nعرف size dyal l environment
 */
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

/*
 * HNA KANحولوU ENVIRONMENT L ARRAY
 * 
 * Ashno katdir:
 * 1. Kan3د environment variables bash n3rf l size
 * 2. Kanعمل malloc array dyal strings
 * 3. Koll variable kanحولوها l format "KEY=VALUE"
 * 4. Kanضع NULL f akhir array
 * 
 * Limada n7tajou array:
 * - execve() function تحتاج environment f array format
 * - Format: ["USER=stoof", "HOME=/home/stoof", "PATH=/bin", NULL]
 * 
 * Mathal:
 * - Linked list: USER="stoof" → HOME="/home/stoof"
 * - Array: ["USER=stoof", "HOME=/home/stoof", NULL]
 */
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

/*
 * HNA KANطبعوU ENVIRONMENT SORTED
 * 
 * Ashno katdir:
 * - Kanطبع koll environment variable
 * - Format: KEY=VALUE
 * - Normally كان supposed يكون sorted ولكن هنا simplified version
 * 
 * استعملوها f export command mn ghir arguments
 * - "export" لوحدو يطبع koll environment
 */
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
