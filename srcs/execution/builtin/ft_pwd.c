/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/11 19:48:20 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_pwd(t_data *data)
{
	char	cwd[PATH_MAX];

	(void)data;
	if (getcwd(cwd, PATH_MAX))
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		g_exit_status = SUCCESS;
		return (SUCCESS);
	}
	perror("minishell: pwd");
	g_exit_status = FAILURE;
	return (FAILURE);
}

void	safe_close(int fd)
{
	if (fd >= 0)
		close(fd);
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
