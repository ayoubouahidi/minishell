/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils00.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/06 12:33:21 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*search_path(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

char	*get_path(t_data *data, char *cmd)
{
	char	**paths;
	char	*env_path;
	char	*final_path;

	if (!cmd || !data || !data->env)
		return (NULL);
	env_path = get_env_value(data->env, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	final_path = search_path(paths, cmd);
	return (final_path);
}

static char	*extract_filename_content(char *files, int start, int end)
{
	char	*file;
	int		i;

	i = 0;
	file = ft_malloc(end - start + 1, 1);
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
	if ((files[start] == '\'' && files[end - 1] == '\'')
		|| (files[start] == '\"' && files[end - 1] == '\"'))
	{
		start++;
		end--;
	}
	return (extract_filename_content(files, start, end));
}

char	*get_command_path(t_data *data)
{
	char	*path;

	if (!data->cmd->args || !data->cmd->args[0])
		return (NULL);
	if (ft_strchr(data->cmd->args[0], '/'))
	{
		if (access(data->cmd->args[0], F_OK) != 0)
			return (NULL);
		if (access(data->cmd->args[0], X_OK) != 0)
			return (NULL);
		return (ft_strdup(data->cmd->args[0]));
	}
	path = get_path(data, data->cmd->args[0]);
	return (path);
}
