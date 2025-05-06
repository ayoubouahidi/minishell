/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:41:00 by ayouahid          #+#    #+#             */
/*   Updated: 2024/11/14 16:36:28 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

char	*ft_strnstr(const char *str, const char *find, size_t n)
{
	char	*str1;
	char	*find1;
	size_t	i;
	size_t	j;
	size_t	lenfind;

	str1 = (char *)str;
	find1 = (char *)find;
	lenfind = ft_strlen(find1);
	i = 0;
	while (str1[i] != '\0' && i < n)
	{
		j = 0;
		while (str1[i + j] == find1[j] && str1[i + j] != '\0'
			&& find1[j] != '\0' && i + j < n)
			j++;
		if (j == lenfind)
			break ;
		i++;
	}
	if (lenfind == 0)
		return (str1);
	if (lenfind != j)
		return (NULL);
	return (&str1[i]);
}
