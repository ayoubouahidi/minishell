/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 11:24:35 by ayouahid          #+#    #+#             */
/*   Updated: 2024/10/27 11:24:51 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	char	*s;
	size_t	i;

	s = (char *)src;
	i = 0;
	if (size == 0)
	{
		return (ft_strlen(s));
	}
	else
	{
		while (s[i] != '\0' && i < size - 1)
		{
			dest[i] = s[i];
			i++;
		}
	}
	dest[i] = '\0';
	return (ft_strlen(s));
}
