/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:54:34 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/11 09:09:16 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_malloc(size_t size, int mode)
{
	static t_list	*head_list;
	t_list			*new_node;
	void			*content;

	new_node = NULL;
	content = NULL;
	if (mode == 1)
	{
		content = malloc(size);
		if (!content)
		{
			ft_putstr_fd("malloc Failed: ", STDERR_FILENO);
			ft_malloc(0, 0);
			exit (EXIT_FAILURE);
		}
		new_node = ft_lstnew(content);
		ft_lstadd_back(&head_list, new_node);
	}
	else if (mode == 0)
	{
		ft_lstclear(&head_list, free);
		head_list = NULL;
	}
	return (content);
}

int	handle_single_cmd(t_data *data)
{
	if (is_builtin(data->cmd->args[0]))
	{
		g_exit_status = handle_builtin(data, data->cmd);
		return (g_exit_status);
	}
	g_exit_status = launch_external_command(data);
	return (g_exit_status);
}

int	handle_cmd_not_found(t_data *data)
{
	char	*cmd;

	if (!data->cmd || !data->cmd->args || !data->cmd->args[0])
		return (0);
	cmd = data->cmd->args[0];
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != 0)
		{
			ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
			return (127);
		}
		if (access(cmd, X_OK) != 0)
			return ((ft_putstr_fd("Permission denied\n", STDERR_FILENO)), 126);
		return ((ft_putstr_fd("Command not executable\n", STDERR_FILENO)), 126);
	}
	return ((ft_putstr_fd("command not found\n", STDERR_FILENO)), 127);
}
