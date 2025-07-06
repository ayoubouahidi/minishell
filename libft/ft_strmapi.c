/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:40:21 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 09:22:52 by elkharti         ###   ########.fr       */
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
	str = ft_malloc(sizeof(char) * (lens + 1), 1);
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
