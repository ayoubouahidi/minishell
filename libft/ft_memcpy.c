/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 20:55:50 by ayouahid          #+#    #+#             */
/*   Updated: 2024/10/25 21:33:33 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void	*ft_memcpy(void *dest, const void *src, size_t numByte)
{
	size_t		i;
	char		*newdest;
	const char	*newsrc;

	newdest = (char *)dest;
	newsrc = (char *)src;
	i = 0;
	if (dest == src)
		return (dest);
	while (i < numByte)
	{
		newdest[i] = newsrc[i];
		i++;
	}
	return (dest);
}
