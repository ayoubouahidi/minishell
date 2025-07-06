/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_utils_4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:57:46 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 14:57:48 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

t_token	*tokenize(t_lexer *lexer)
{
	bool	isword;
	bool	isdouble;

	isdouble = false;
	isword = false;
	while (lexer->c != '\0' && lexer->i < ft_strlen(lexer->content))
	{
		if (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n'
			|| lexer->c == '\r' || lexer->c == '\f' || lexer->c == '\v')
			while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n'
				|| lexer->c == '\r' || lexer->c == '\f' || lexer->c == '\v')
				increment_using_index(lexer);
		if (!ft_strchr("|<>", lexer->c))
			return (string_process(lexer));
		if (lexer->c == '|')
		{
			increment_using_index(lexer);
			return (creat_token(PIPE, tostr('|'), NOT_QUOTED));
		}
		if (lexer->c == '<')
			return (chech_herdoc(lexer));
		if (lexer->c == '>')
			return (check_append(lexer));
	}
	return (creat_token(ENDF, "END", NOT_QUOTED));
}

// some utils function of parse commande

int	len_of_2d_array(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

char	**to_arg(t_token *token, char **arg)
{
	char	**result;
	int		i;

	if (token->is_quoted == NOT_QUOTED && (!token->value
			|| token->value[0] == '\0'))
		return (arg);
	i = 0;
	if (!arg)
	{
		result = ft_malloc(sizeof(char *) * 2, 1);
		result[0] = ft_strdup(token->value);
		result[1] = NULL;
		return (result);
	}
	result = ft_malloc(sizeof(char *) * (len_of_2d_array(arg) + 2), 1);
	while (arg[i])
	{
		result[i] = arg[i];
		i++;
	}
	result[i] = ft_strdup(token->value);
	result[i + 1] = NULL;
	return (result);
}

bool	match_file(char *file)
{
	if (ft_isdigit(file[0]))
		return (false);
	return (true);
}

void	print_syntax_error(t_token *token)
{
	if (!token || token->next->type == ENDF)
		ft_putstr_fd("minishell: syntax error near unexpected token \
			`newline'\n",
			2);
	else if (token->type != WORD)
	{
		if (token->type == OUTPUT_RED)
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token\
				 `>'\n",
				2);
		else if (token->type == INTPUT_RED)
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected \
				token `<'\n",
				2);
		else if (token->type == APPEND)
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected \
				token `>>'\n",
				2);
		else if (token->type == HEREDOC)
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected \
				token `<<'\n",
				2);
		else if (token->type == PIPE)
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected \
				token `|'\n",
				2);
		else
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected \
				token\n",
				2);
	}
}
