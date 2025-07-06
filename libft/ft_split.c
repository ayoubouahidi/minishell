/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 10:11:53 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 09:36:52 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static int	countwords(char const *s, char c)
{
	size_t	words;
	size_t	i;

	i = 0;
	words = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		words++;
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	return (words);
}

static char	*mallocword(char *s, char c)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	while (s[i] != '\0' && s[i] != c)
		i++;
	str = (char *)ft_malloc(sizeof(char) * i + 1, 1);
	if (str == NULL)
		return (NULL);
	j = 0;
	while (i > j)
	{
		str[j] = s[j];
		j++;
	}
	str[j] = '\0';
	return (str);
}

static char	**fill_str(char **str, char const *s, char c, size_t	i)
{
	size_t	j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0' && s[i] != c)
		{
			str[j] = mallocword((char *)s + i, c);
			if (!str[j])
			{
				return (NULL);
			}
			j++;
		}
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	str[j] = NULL;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	int		words;
	char	**str;

	if (s == NULL)
		return (NULL);
	i = 0;
	words = countwords(s, c);
	str = (char **)ft_malloc(sizeof(char *) * (words + 1), 1);
	if (str == NULL)
		return (NULL);
	str = fill_str(str, s, c, i);
	return (str);
}
