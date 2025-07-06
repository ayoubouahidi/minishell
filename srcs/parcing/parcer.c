/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:02:05 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/05 07:22:13 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// t_command	*ft_lstnew_cmd(void *content)
// {
// 	t_command	*tmp;

// 	tmp = (t_command *)malloc(sizeof(t_command));
// 	if (!tmp)
// 		return (NULL);
// 	tmp->args = content;
// 	tmp->next = NULL;
// 	return (tmp);
// }

// void	ft_lstadd_back_token(t_token **lst, t_token *new)
// {
// 	t_token	*tmp;

// 	if (!lst || !new)
// 		return ;
// 	tmp = *lst;
// 	if (*lst)
// 	{
// 		while (tmp->next)
// 			tmp = tmp->next;
// 		tmp->next = new;
// 	}
// 	else
// 		*lst = new;
// }

// void	ft_lstadd_back_cmd(t_command **lst, t_command *new)
// {
// 	t_command	*tmp;

// 	if (!lst || !new)
// 		return ;
// 	tmp = *lst;
// 	if (*lst)
// 	{
// 		while (tmp->next)
// 			tmp = tmp->next;
// 		tmp->next = new;
// 	}
// 	else
// 		*lst = new;
// }

// int	ft_lstsize_parce(t_command *lst)
// {
// 	int	cmpt;

// 	cmpt = 0;
// 	while (lst != NULL)
// 	{
// 		lst = lst->next;
// 		cmpt++;
// 	}
// 	return (cmpt);
// }

// IS_QUOTED	is_token_quoted(const char *str, int len)
// {
// 	if (!str || len < 2)
// 		return (NOT_QUOTED);
// 	if ((str[0] == '"' && str[1] == '"' && len == 2) || (str[0] == '\''
// 			&& str[1] == '\'' && len == 2))
// 		return (QUOTED);
// 	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\''
// 				&& str[len - 1] == '\'')))
// 		return (QUOTED);
// 	return (NOT_QUOTED);
// }

// bool	syntaxe_error(char *str)
// {
// 	int	i;
// 	int	quotes_d;
// 	int	quotes_s;

// 	quotes_d = 0;
// 	quotes_s = 0;
// 	i = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '"' && !quotes_s)
// 		{
// 			quotes_d = !quotes_d;
// 		}
// 		else if (str[i] == '\'' && !quotes_d)
// 		{
// 			quotes_s = !quotes_s;
// 		}
// 		i++;
// 	}
// 	if (((quotes_d == 0) && (quotes_s == 0)))
// 		return (true);
// 	return (false);
// }

// t_token	*creat_token(TYPE_TOKEN type, char *value, IS_QUOTED is_quoted)
// {
// 	t_token	*token;

// 	token = (t_token *)malloc(sizeof(t_token));
// 	if (!token)
// 		return (NULL);
// 	token->type = type;
// 	token->value = value;
// 	token->is_quoted = is_quoted;
// 	token->next = NULL;
// 	return (token);
// }

// t_lexer	*creat_lexer(char *content)
// {
// 	t_lexer	*lexer;

// 	lexer = (t_lexer *)malloc(sizeof(t_lexer));
// 	if (!lexer)
// 		return (NULL);
// 	lexer->content = content;
// 	lexer->i = 0;
// 	lexer->c = lexer->content[lexer->i];
// 	return (lexer);
// }

// void	increment_using_index(t_lexer *lexer)
// {
// 	if (lexer->c != '\0' && lexer->i < ft_strlen(lexer->content))
// 	{
// 		lexer->i += 1;
// 		lexer->c = lexer->content[lexer->i];
// 	}
// }

// bool	is_token_delimiter(char c, int flag_single, int flag_double)
// {
// 	return (!flag_double && !flag_single && (c == '|' || c == '<' || c == '>'
// 			|| c == ' '));
// }

// void	handle_quotes(char c, int *flag_single, int *flag_double)
// {
// 	if (c == '\'' && !(*flag_double))
// 	{
// 		*flag_single = !(*flag_single);
// 	}
// 	else if (c == '\"' && !(*flag_single))
// 	{
// 		*flag_double = !(*flag_double);
// 	}
// }

// t_token	*string_process(t_lexer *lexer)
// {
// 	char	*value;
// 	int		flag_single;
// 	int		flag_double;
// 	size_t	start;

// 	flag_single = 0;
// 	flag_double = 0;
// 	start = lexer->i;
// 	while (lexer->c != '\0')
// 	{
// 		handle_quotes(lexer->c, &flag_single, &flag_double);
// 		if (is_token_delimiter(lexer->c, flag_single, flag_double))
// 			break ;
// 		increment_using_index(lexer);
// 	}
// 	value = ft_substr(lexer->content, start, lexer->i - start);
// 	return (creat_token(WORD, value, is_token_quoted(value, ft_strlen(value))));
// }

// char	*tostr(char c)
// {
// 	char	*val;

// 	val = (char *)malloc(2);
// 	val[0] = c;
// 	val[1] = '\0';
// 	return (val);
// }

// t_token	*chech_herdoc(t_lexer *lexer)
// {
// 	t_lexer	tmp;

// 	tmp = *lexer;
// 	increment_using_index(&tmp);
// 	increment_using_index(lexer);
// 	if (tmp.c == '<')
// 	{
// 		increment_using_index(lexer);
// 		return (creat_token(HEREDOC, "<<", NOT_QUOTED));
// 	}
// 	return (creat_token(INTPUT_RED, tostr('<'), NOT_QUOTED));
// }

// t_token	*check_append(t_lexer *lexer)
// {
// 	t_lexer	tmp;

// 	tmp = *lexer;
// 	increment_using_index(&tmp);
// 	increment_using_index(lexer);
// 	if (tmp.c == '>')
// 	{
// 		increment_using_index(lexer);
// 		return (creat_token(APPEND, ">>", NOT_QUOTED));
// 	}
// 	return (creat_token(OUTPUT_RED, tostr('>'), NOT_QUOTED));
// }

// t_token	*tokenize(t_lexer *lexer)
// {
// 	bool	isword;
// 	bool	isdouble;

// 	isdouble = false;
// 	isword = false;
// 	while (lexer->c != '\0' && lexer->i < ft_strlen(lexer->content))
// 	{
// 		if (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n'
// 			|| lexer->c == '\r' || lexer->c == '\f' || lexer->c == '\v')
// 			while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n'
// 				|| lexer->c == '\r' || lexer->c == '\f' || lexer->c == '\v')
// 				increment_using_index(lexer);
// 		if (!ft_strchr("|<>", lexer->c))
// 			return (string_process(lexer));
// 		if (lexer->c == '|')
// 		{
// 			increment_using_index(lexer);
// 			return (creat_token(PIPE, tostr('|'), NOT_QUOTED));
// 		}
// 		if (lexer->c == '<')
// 			return (chech_herdoc(lexer));
// 		if (lexer->c == '>')
// 			return (check_append(lexer));
// 	}
// 	return (creat_token(ENDF, "END", NOT_QUOTED));
// }

// // some utils function of parse commande

// int	len_of_2d_array(char **args)
// {
// 	int	i;

// 	i = 0;
// 	while (args && args[i])
// 		i++;
// 	return (i);
// }

// char	**to_arg(t_token *token, char **arg)
// {
// 	char	**result;
// 	int		i;

// 	if (token->is_quoted == NOT_QUOTED && (!token->value
// 			|| token->value[0] == '\0'))
// 		return (arg);
// 	i = 0;
// 	if (!arg)
// 	{
// 		result = malloc(sizeof(char *) + 2);
// 		result[0] = ft_strdup(token->value);
// 		result[1] = NULL;
// 		return (result);
// 	}
// 	result = malloc(sizeof(char *) * (len_of_2d_array(arg) + 2));
// 	while (arg[i])
// 	{
// 		result[i] = arg[i];
// 		i++;
// 	}
// 	result[i] = ft_strdup(token->value);
// 	result[i + 1] = NULL;
// 	return (result);
// }

// bool	match_file(char *file)
// {
// 	if (ft_isdigit(file[0]))
// 		return (false);
// 	return (true);
// }

// void	print_syntax_error(t_token *token)
// {
// 	if (!token || token->next->type == ENDF)
// 		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
// 			2);
// 	else if (token->type != WORD)
// 	{
// 		if (token->type == OUTPUT_RED)
// 			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token `>'\n",
// 				2);
// 		else if (token->type == INTPUT_RED)
// 			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token `<'\n",
// 				2);
// 		else if (token->type == APPEND)
// 			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token `>>'\n",
// 				2);
// 		else if (token->type == HEREDOC)
// 			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token `<<'\n",
// 				2);
// 		else if (token->type == PIPE)
// 			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token `|'\n",
// 				2);
// 		else
// 			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token\n",
// 				2);
// 	}
// }

// bool	heredoc_check_append(t_token *token, char **del)
// {
// 	t_token	*prev;

// 	prev = token;
// 	token = token->next;
// 	if ((token)->type != WORD)
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
// 		return (false);
// 	}
// 	*del = token->value;
// 	return (true);
// }

// t_redirections	*creat_red(char *file, TYPE_TOKEN type)
// {
// 	t_redirections	*red;

// 	red = (t_redirections *)malloc(sizeof(t_redirections));
// 	if (!red)
// 		return (NULL);
// 	red->file = file;
// 	red->type = type;
// 	red->next = NULL;
// 	return (red);
// }

// void	ft_lstadd_back_red(t_redirections **lst, t_redirections *new)
// {
// 	t_redirections	*tmp;

// 	if (!lst || !new)
// 		return ;
// 	tmp = *lst;
// 	if (*lst)
// 	{
// 		while (tmp->next)
// 			tmp = tmp->next;
// 		tmp->next = new;
// 	}
// 	else
// 		*lst = new;
// }

// // Initialize command structure
// t_command	*init_command(void)
// {
// 	t_command	*cmd;

// 	cmd = (t_command *)malloc(sizeof(t_command));
// 	if (!cmd)
// 		return (NULL);
// 	cmd->args = NULL;
// 	cmd->is_heredoc = false;
// 	cmd->del = NULL;
// 	cmd->next = NULL;
// 	cmd->redirections = NULL;
// 	return (cmd);
// }
// void	process_word_token(t_token *token, char ***args, bool *in_red)
// {
// 	if (!*in_red)
// 		*args = to_arg(token, *args);
// 	*in_red = false;
// }

// bool	process_redirection(t_token *token, t_command *cmd,
// 		t_redirections **red_head)
// {
// 	t_redirections	*red;

// 	red = NULL;
// 	if (token->type == OUTPUT_RED)
// 	{
// 		red = creat_red(token->next->value, OUTPUT_RED);
// 		ft_lstadd_back_red(red_head, red);
// 	}
// 	else if (token->type == INTPUT_RED)
// 	{
// 		red = creat_red(token->next->value, INTPUT_RED);
// 		ft_lstadd_back_red(red_head, red);
// 	}
// 	else if (token->type == HEREDOC)
// 		cmd->is_heredoc = heredoc_check_append(token, &cmd->del);
// 	else if (token->type == APPEND)
// 	{
// 		red = creat_red(token->next->value, APPEND);
// 		ft_lstadd_back_red(red_head, red);
// 	}
// 	return (token->type != HEREDOC || cmd->is_heredoc);
// }

t_command	*parser_commande(t_token **tokendd)
{
	t_command		*cmd;
	char			**args;
	bool			in_red;
	t_redirections	*red_head;

	args = NULL;
	in_red = false;
	red_head = NULL;
	if (!tokendd || !*tokendd)
		return (NULL);
	cmd = init_command();
	if (!cmd)
		return (NULL);
	while (tokendd && (*tokendd)->type != ENDF && (*tokendd)->type != PIPE)
	{
		if ((*tokendd)->type == WORD)
			process_word_token(*tokendd, &args, &in_red);
		else
		{
			in_red = true;
			if (!process_redirection(*tokendd, cmd, &red_head))
			{
				if (cmd->del)
					free(cmd->del);
				free(cmd);
				return (NULL);
			}
		}
		(*tokendd) = (*tokendd)->next;
	}
	cmd->args = args;
	cmd->redirections = red_head;
	return (cmd);
}

bool	validate_pipe_parse(t_token *token)
{
	t_token	*current_token;

	current_token = token;
	if (token->type == PIPE)
	{
		print_syntax_error(current_token);
		return (false);
	}
	while (current_token && current_token->type != ENDF)
	{
		if (current_token->type != WORD)
		{
			if (current_token->next && current_token->next->type != WORD)
			{
				print_syntax_error(current_token);
				return (false);
			}
		}
		current_token = current_token->next;
	}
	return (true);
}
t_token	*tokenize_input(char *line, t_env *envp)
{
	t_lexer	*lexer;
	char	*trim;

	t_token(*token), (*head_token);
	token = NULL;
	head_token = NULL;
	trim = ft_strtrim(line, " ");
	if (!syntaxe_error(trim))
	{
		write(1, "Quotes Error !\n", 15);
		free(trim);
		return (NULL);
	}
	lexer = creat_lexer(trim);
	while (1)
	{
		token = tokenize(lexer);
		expantion_remove_quotes(token, envp);
		ft_lstadd_back_token(&head_token, token);
		if (token->type == ENDF)
			break ;
	}
	if (!validate_pipe_parse(head_token))
		return (NULL);
	return (head_token);
}

int	is_del(int flag)
{
	static int	i;

	if (flag == 0)
		i = 0;
	else if (flag == 1)
		i = 1;
	return (i);
}
t_command	*parcer(char *line, t_env *envp)
{
	t_token		*head_token;
	t_command	*commande;
	t_command	*head;

	commande = NULL;
	head = NULL;
	head_token = tokenize_input(line, envp);
	if (!head_token)
		return (NULL);
	while (head_token && head_token->type != ENDF)
	{
		commande = parser_commande(&head_token);
		ft_lstadd_back_cmd(&head, commande);
		head_token = head_token->next;
	}
	return (head);
}
