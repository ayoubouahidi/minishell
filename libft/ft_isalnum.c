/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:35:58 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 19:00:26 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <ctype.h>
#include <stdio.h>

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c) || (c >= '(' && c <= '.')
		|| c == '{' || c == '}' || c == '/' || c == '_');
}
