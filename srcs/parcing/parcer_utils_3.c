/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:57:39 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 14:57:40 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void	handle_quotes(char c, int *flag_single, int *flag_double)
{
	if (c == '\'' && !(*flag_double))
	{
		*flag_single = !(*flag_single);
	}
	else if (c == '\"' && !(*flag_single))
	{
		*flag_double = !(*flag_double);
	}
}

t_token	*string_process(t_lexer *lexer)
{
	char	*value;
	int		flag_single;
	int		flag_double;
	size_t	start;

	flag_single = 0;
	flag_double = 0;
	start = lexer->i;
	while (lexer->c != '\0')
	{
		handle_quotes(lexer->c, &flag_single, &flag_double);
		if (is_token_delimiter(lexer->c, flag_single, flag_double))
			break ;
		increment_using_index(lexer);
	}
	value = ft_substr(lexer->content, start, lexer->i - start);
	return (creat_token(WORD, value, is_token_quoted(value, ft_strlen(value))));
}

char	*tostr(char c)
{
	char	*val;

	val = (char *)malloc(2);
	val[0] = c;
	val[1] = '\0';
	return (val);
}

t_token	*chech_herdoc(t_lexer *lexer)
{
	t_lexer	tmp;

	tmp = *lexer;
	increment_using_index(&tmp);
	increment_using_index(lexer);
	if (tmp.c == '<')
	{
		increment_using_index(lexer);
		return (creat_token(HEREDOC, "<<", NOT_QUOTED));
	}
	return (creat_token(INTPUT_RED, tostr('<'), NOT_QUOTED));
}

t_token	*check_append(t_lexer *lexer)
{
	t_lexer	tmp;

	tmp = *lexer;
	increment_using_index(&tmp);
	increment_using_index(lexer);
	if (tmp.c == '>')
	{
		increment_using_index(lexer);
		return (creat_token(APPEND, ">>", NOT_QUOTED));
	}
	return (creat_token(OUTPUT_RED, tostr('>'), NOT_QUOTED));
}
