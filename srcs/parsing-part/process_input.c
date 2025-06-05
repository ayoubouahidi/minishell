/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:11:24 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/24 16:16:03 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*process_input(char *input, int *i, t_quote_type *quote_type)
{
	char	*value;

	if (input[*i] == '|' || input[*i] == '>' || input[*i] == '<')
		value = get_operator(input, i, quote_type);
	else
		value = get_word(input, i, quote_type);
	if (!value)
		return (NULL);
	return (value);
}
