/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:57:26 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/12 10:54:17 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool	syntaxe_error(char *str)
{
	int	i;
	int	quotes_d;
	int	quotes_s;

	quotes_d = 0;
	quotes_s = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '"' && !quotes_s)
		{
			quotes_d = !quotes_d;
		}
		else if (str[i] == '\'' && !quotes_d)
		{
			quotes_s = !quotes_s;
		}
		i++;
	}
	if (((quotes_d == 0) && (quotes_s == 0)))
		return (true);
	return (false);
}

t_token	*creat_token(t_TYPE_TOKEN type, char *value, t_IS_QUOTED is_quoted)
{
	t_token	*token;

	token = (t_token *)ft_malloc(sizeof(t_token), 1);
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->is_quoted = is_quoted;
	token->next = NULL;
	return (token);
}

t_lexer	*creat_lexer(char *content)
{
	t_lexer	*lexer;

	lexer = (t_lexer *)ft_malloc(sizeof(t_lexer), 1);
	if (!lexer)
		return (NULL);
	lexer->content = content;
	lexer->i = 0;
	lexer->c = lexer->content[lexer->i];
	return (lexer);
}

void	increment_using_index(t_lexer *lexer)
{
	if (lexer->c != '\0' && lexer->i < ft_strlen(lexer->content))
	{
		lexer->i += 1;
		lexer->c = lexer->content[lexer->i];
	}
}

bool	is_token_delimiter(char c, int flag_single, int flag_double)
{
	return (!flag_double && !flag_single && (c == '|' || c == '<' || c == '>'
			|| c == ' '));
}
