/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:00:00 by elkharti          #+#    #+#             */
/*   Updated: 2025/07/10 09:04:45 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	has_only_n(char *arg)
{
	int	i;

	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	process_n_flag(char **args, int *i)
{
	int	n_flag;

	n_flag = 0;
	while (args[*i] && ft_strncmp(args[*i], "-n", 2) == 0)
	{
		if (has_only_n(args[*i]))
			n_flag = 1;
		else
			break ;
		(*i)++;
	}
	return (n_flag);
}

int	ft_echo(char **args)
{
	int	n_flag;
	int	i;
	int	j;

	if (!args[1])
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	n_flag = 0;
	i = 1;
	j = 0;
	n_flag = process_n_flag(args, &i);
	while (args[i])
	{
		if (j != 0)
			ft_putchar_fd(' ', 1);
		ft_putstr_fd(args[i], 1);
		j++;
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
