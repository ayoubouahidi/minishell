#include "../../../includes/minishell.h"

static char	*extract_filename(char *files, int start, int end)
{
	char	*file;
	int		i;

	i = 0;
	if ((files[start] == '\'' && files[end - 1] == '\'') ||
		(files[start] == '\"' && files[end - 1] == '\"'))
	{
		start++;
		end--;
	}
	file = malloc(end - start + 1);
	if (!file)
		return (NULL);
	while (start < end)
		file[i++] = files[start++];
	file[i] = '\0';
	return (file);
}

char	*get_file_name(char *files)
{
	int	start;
	int	end;

	if (!files)
		return (NULL);
	start = 0;
	while (files[start] && files[start] == ' ')
		start++;
	end = start;
	while (files[end] && files[end] != ' ')
		end++;
	return (extract_filename(files, start, end));
}
	int	start;
	int	end;

	if (!files)
		return (NULL);
	start = 0;
	while (files[start] && files[start] == ' ')
		start++;
	end = start;
	while (files[end] && files[end] != ' ')
		end++;
	return (extract_filename(files, start, end));
}
		return (NULL);
	start = 0;
	while (files[start] && files[start] == ' ')
		start++;
	end = start;
	while (files[end] && files[end] != ' ')
		end++;
	return (extract_filename(files, start, end));
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
