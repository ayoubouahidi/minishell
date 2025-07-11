/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils01.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/07 07:18:04 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	save_std_fd(int *saved_in, int *saved_out)
{
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_in < 0 || *saved_out < 0)
		return (-1);
	return (0);
}

void	reset_std_fd(int saved_in, int saved_out)
{
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

void	clean_exit(t_data *data, int exit_code)
{
	(void)data;
	ft_malloc(0, 0);
	clear_history();
	g_exit_status = exit_code;
	exit(exit_code);
}

void	parent_cleanup(int *pre_fd, int *fd, t_command **cmd)
{
	if (*pre_fd != -1)
		safe_close(*pre_fd);
	if ((*cmd)->next)
	{
		safe_close(fd[1]);
		*pre_fd = fd[0];
	}
}

int	is_empty_or_whitespace(char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n')
			return (0);
		str++;
	}
	return (1);
}
