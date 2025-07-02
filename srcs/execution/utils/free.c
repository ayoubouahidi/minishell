/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/01 10:57:38 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}


static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_all(t_data *data)
{
	if (data->cmd)
	{
		free_args(data->cmd->args);
		free(data->cmd);
	}
}

void	free_cmd(t_command *cmd)
{
	t_command		*tmp;
	t_redirections	*redir;
	t_redirections	*redir_tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_args(cmd->args);
		if (cmd->here_doc_file)
			free(cmd->here_doc_file);
		redir = cmd->redirections;
		while (redir)
		{
			redir_tmp = redir->next;
			if (redir->file)
				free(redir->file);
			free(redir);
			redir = redir_tmp;
		}
		free(cmd);
		cmd = tmp;
	}
}
