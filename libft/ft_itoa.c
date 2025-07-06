/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 20:06:36 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 09:27:47 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int	countdigits(long n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	if (n < 0)
	{
		n = -n;
		count++;
	}
	while (n != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static void	itoipart2(char *str, int numdi, long n)
{
	str[numdi] = '\0';
	numdi--;
	while (numdi >= 0)
	{
		str[numdi] = n % 10 + 48;
		n = n / 10;
		numdi--;
	}
}

char	*ft_itoa(int n)
{
	int		numdi;
	char	*str;
	long	number;

	number = n;
	numdi = countdigits(number);
	str = (char *)malloc(sizeof(char) * numdi + 1);
	if (!str)
		return (NULL);
	if (number < 0)
	{
		number = -number;
		str[0] = '-';
	}
	itoipart2(str + (n < 0), numdi - (n < 0), number);
	return (str);
}
