











#include "../../includes/minishell.h"


char	**ft_realloc_array(char **arr, char *new_str)
{
	int		len;
	int		i;
	char	**new_arr;

	len = 0;
	if (arr)
		while (arr[len])
			len++;
	new_arr = malloc(sizeof(char *) * (len + 2));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[len] = ft_strdup(new_str);
	new_arr[len + 1] = NULL;
	return (new_arr);
}
