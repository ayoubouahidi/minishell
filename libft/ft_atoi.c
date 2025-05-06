/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 22:05:14 by ayouahid          #+#    #+#             */
/*   Updated: 2024/11/14 13:20:02 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int	fornotminette(int result, int cmpt, int cmpmoins)
{
	if (cmpt > 1)
		return (0);
	if (cmpmoins == 1)
		return (result * -1);
	return (result);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		cmpt;
	long	result;
	int		cmpmoins;

	cmpmoins = 0;
	result = 0;
	cmpt = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == 32 && str[i] != '\0'))
		i++;
	while (str[i] == '-' || str[i] == '+')
	{
		cmpt++;
		if (str[i] == '-')
			cmpmoins++;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	result = fornotminette(result, cmpt, cmpmoins);
	return (result);
}
