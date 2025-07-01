











#include "../../includes/minishell.h"

char	*ft_strjoin_char(char *str, char c)
{
	char	*new_str;
	int		i;

	new_str = malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!new_str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	return (new_str);
}
