/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:40:21 by ayouahid          #+#    #+#             */
/*   Updated: 2024/11/06 16:40:36 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t		lens;
	char		*str;
	size_t		i;

	if (!s || !f)
		return (NULL);
	lens = ft_strlen(s);
	str = malloc(sizeof(char) * (lens + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < lens)
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
