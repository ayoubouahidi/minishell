/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:26:30 by ayouahid          #+#    #+#             */
/*   Updated: 2025/05/20 11:06:49 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

size_t	ft_strlen(const char *a)
{
	int	i;

	i = 0;
	if (a == NULL)
		return (0);
	while (a[i] != '\0')
	{
		i++;
	}
	return (i);
}
