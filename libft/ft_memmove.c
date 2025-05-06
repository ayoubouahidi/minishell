/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 11:59:18 by ayouahid          #+#    #+#             */
/*   Updated: 2024/10/26 15:00:35 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

void	*ft_memmove(void *dest, const void *src, size_t numByte)
{
	size_t	i;
	char	*newdest;
	char	*newsrc;

	newdest = (char *)dest;
	newsrc = (char *)src;
	if (newdest > newsrc)
	{
		i = numByte;
		while (i-- > 0)
		{
			newdest[i] = newsrc[i];
		}
	}
	else
	{
		ft_memcpy(newdest, newsrc, numByte);
	}
	return (dest);
}
