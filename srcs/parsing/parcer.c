/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayouahid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:02:05 by ayouahid          #+#    #+#             */
/*   Updated: 2025/05/07 10:02:07 by ayouahid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../libft/libft.h"


// void free_commands(t_command *cmd) {
//     t_command *tmp;
//     while (cmd) {
//         tmp = cmd->next;
//         for (int i = 0; cmd->args && cmd->args[i]; i++) free(cmd->args[i]);
//         free(cmd->args);
//         free(cmd->infile);
//         free(cmd->outfile);
//         free(cmd);
//         cmd = tmp;
//     }
// }


// linked list functions 
void	printlist(t_command *head)
{
	t_command	*tmp;

	tmp = head;
	while (tmp != NULL)
	{
		printf("command ========\n infile ==> %s\n outfile ==> %s\n , del ==> %s\n ============\n", tmp->infile, tmp->outfile, tmp->del);
		tmp = tmp->next;
	}
}

t_command	*ft_lstnew_cmd(void *content)
{
	t_command	*tmp;

	tmp = (t_command *)malloc(sizeof(t_command));
	if (!tmp)
		return (NULL);
	tmp->args = content;
	tmp->next = NULL;
	return (tmp);
}

void	ft_lstadd_back_token(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!lst || !new)
		return ;
	tmp = *lst;
	if (*lst)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*lst = new;
}

void	ft_lstadd_back_cmd(t_command **lst, t_command *new)
{
	t_command	*tmp;

	if (!lst || !new)
		return ;
	tmp = *lst;
	if (*lst)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*lst = new;
}



int	ft_lstsize_parce(t_command *lst)
{
	int	cmpt;

	cmpt = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		cmpt++;
	}
	return (cmpt);
}

// void	add_node(char *line, t_command **head)
// {
// 	int i;
// 	t_command *cmd;
// 	int lenght_line;
// 	// char *line_pss; 
// 	cmd = NULL;
// 	head++;
// 	cmd++;

// 	i = 0;
// 	lenght_line = 0;
// 	// a refaire 
// 	while (line[i] != '\0')
// 	{
		
// 		// lenght_line++;// lenght_line++;
// 		// if (line[i] == ' ')
// 		// {
// 			// cmd = ft_lstnew_cmd(line_pss - lenght_line -1);
// 			// ft_lstadd_back_cmd(head, cmd);

// 			// printf("String => %s\n", line + i - lenght_line -1);
// 			// i++;
// 		// }
// 		// while (line[i] == ' ')
// 		// 	i++;
// 		// if (line[i] == '"')
// 		// {
// 		// 	while (line[i] != '"')
// 		// 		i++;
// 		// 	if (line[i] == '"')
// 		// 	{
// 		// 		cmd = ft_lstnew_cmd(line + (i - 1));
// 		// 		ft_lstadd_back_cmd(head, cmd);
// 		// 	}
// 		// 	else
// 		// 		write(1, "Error\n", 6);
// 		// }
// 		i++;
// 	}
// }

bool syntaxe_error(char *str)
{
	int i;
	int quotes_d;
	int quotes_s;

	quotes_d = 0;
	quotes_s = 0;
	i = 0;
	while(str[i] != '\0')
	{
		if (str[i] == '"' && !quotes_s )
		{
			quotes_d = !quotes_d;
		}
		else if (str[i] == '\'' && !quotes_d)
		{
			quotes_s = !quotes_s;
		}
		i++;
	}
	if(((quotes_d  == 0 ) && (quotes_s  == 0)))
		return (true);
	return (false);
}

// bool is_a_special_char(char c)
// {
// 	return (c == '>' || c == '<' || c == '>>' || c == '<<' || c == '|');
// }
// initialisation of tokan and lexer
t_token *creat_token(TYPE_TOKEN type, char *value)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

t_lexer *creat_lexer(char *content)
{
	t_lexer *lexer;

	lexer = (t_lexer *)malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->content = content;
	lexer->i = 0;
	lexer->c = lexer->content[lexer->i];
	return (lexer);
}

// some utils function of tokonization

void	increment_using_index(t_lexer *lexer)
{
	if (lexer->c != '\0' && lexer->i < ft_strlen(lexer->content))
	{
		lexer->i += 1;
		lexer->c = lexer->content[lexer->i];
	}
}

t_token *string_process(t_lexer *lexer)
{
	t_lexer tmp;
	int count;
	char* value;
	count = 0;

	increment_using_index(lexer);
	tmp = *lexer;
	while (tmp.c != '"')
	{
		count++;
		increment_using_index(&tmp);
	}
	value = (char *)malloc(count + 1);
	count = 0;
	while (lexer->c != '"')
	{
		value[count] = lexer->c; 
		increment_using_index(lexer);
		count++;
	}
	increment_using_index(lexer);
	value[count] = '\0';
	return (creat_token(WORD, value));
}

t_token* is_word(t_lexer *lexer)
{
	t_lexer tmp;
	int	count;
	char *value;

	tmp = *lexer;
	count = 0;
	while (ft_isalnum(tmp.c))
	{
		count++;
		increment_using_index(&tmp);
	}
	value = (char *)malloc(count + 1);	
	count = 0;
	while (ft_isalnum(lexer->c))
	{
		value[count] = lexer->c;
		count++;
		increment_using_index(lexer);
	}
	value[count] = '\0';
	return (creat_token(WORD, value));
}
char *tostr(char c)
{
	char *val;

	val = (char *)malloc(2);
	val[0] = c;
	val[1] = '\0';
	return (val);
}

t_token* chech_herdoc(t_lexer* lexer)
{
	t_lexer tmp;

	tmp = *lexer;
	increment_using_index(&tmp);
	increment_using_index(lexer);
	if (tmp.c == '<')
	{
		increment_using_index(lexer);
		return(creat_token(HEREDOC, "<<"));
	}
	return (creat_token(INTPUT_RED, tostr('<')));
}

t_token* check_append(t_lexer* lexer)
{
	t_lexer tmp;

	tmp = *lexer;
	increment_using_index(&tmp);
	increment_using_index(lexer);
	if (tmp.c == '>')
	{
		increment_using_index(lexer);
		return(creat_token(APPEND, ">>"));
	}
	return (creat_token(OUTPUT_RED, tostr('>')));
}
// tokenize

t_token	*tokenize(t_lexer *lexer) 
{
	while (lexer->c != '\0' && lexer->i < ft_strlen(lexer->content))
	{
		if(lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r' || lexer->c == '\f' || lexer->c == '\v')
			while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r' || lexer->c == '\f' || lexer->c == '\v')
				increment_using_index(lexer);
		if (ft_isalnum(lexer->c))
			return (is_word(lexer));
		if (lexer->c == '|')
		{
			increment_using_index(lexer);
			return (creat_token(PIPE, tostr('|')));
		}
		if (lexer->c == '<')
			return (chech_herdoc(lexer));
		if (lexer->c == '>')
			return (check_append(lexer));
		if(lexer->c == '"')
			return string_process(lexer);
		increment_using_index(lexer);
	}
	
	return (creat_token(ENDF, "END"));
}



// some utils function of parse commande 

char *to_arg(t_token* token, char *arg)
{
	int lenv;
	int lenghtcommande;
	char *new_commande;

	lenv = ft_strlen(token->value);
	lenghtcommande = ft_strlen(arg);
	new_commande = (char *)malloc(lenghtcommande + 1 + lenv + 1);
	if (arg)
	{
        ft_strlcpy(new_commande, arg, lenghtcommande + 1); 
        new_commande[lenghtcommande] = ' '; 
        ft_strlcpy(new_commande + lenghtcommande + 1, token->value, lenv + 1);
    } 
	else 
        ft_strlcpy(new_commande, token->value, lenv + 1); 
    return (new_commande);
}

// infile function u should check sysntaxe error


char *infile(t_token *token, char *arg)
{
	int lenv;
	int lenghtcommande;
	char *new_commande;
	// printf("test done");
	token = (token)->next;
	if ((token)->type != WORD)
	{
		printf("Syntaxe error \n : heredoc problem ");
		return(NULL);
	}
	lenv = ft_strlen((token)->value);
	lenghtcommande = ft_strlen(arg);
	new_commande = (char *)malloc(lenghtcommande + 1 + lenv + 1);
	if (arg)
	{
        ft_strlcpy(new_commande, arg, lenghtcommande + 1); 
        new_commande[lenghtcommande] = ' '; 
        ft_strlcpy(new_commande + lenghtcommande + 1, (token)->value, lenv + 1);
    }
	else 
        ft_strlcpy(new_commande, (token)->value, lenv + 1); 
    return (new_commande);
}
// heredoc function and APPEND

bool	heredoc_check_append(t_token *token, char **del)
{
	token = (token)->next;
	if ((token)->type != WORD)
	{
		printf("Syntaxe error : heredoc problem \n");
		return(false);
	}
	*del = token->value;
	return true;

}
  
// parser part


t_command* parser_commande(t_token** tokendd)
{
	t_command *cmd;
	// // t_command tmp;
	// t_token *token = *tokendd;
	char *args = NULL;
	char *infile_file = NULL;
	char *outfile_file = NULL;
	char *del = NULL;

	cmd = (t_command *)malloc(sizeof(t_command));
	while (tokendd && (*tokendd)->type != ENDF && (*tokendd)->type != PIPE)
	{
		if((*tokendd)->type == WORD)
			args = to_arg((*tokendd), args);
		else if ((*tokendd)->type == OUTPUT_RED)
			infile_file = infile((*tokendd), infile_file);
		else if ((*tokendd)->type == INTPUT_RED)
			outfile_file = infile((*tokendd), outfile_file);
		else if ((*tokendd)->type == HEREDOC)
			cmd->is_heredoc = heredoc_check_append((*tokendd), &del);
		else if ((*tokendd)->type == APPEND)
			cmd->is_append = heredoc_check_append((*tokendd), &del);
		(*tokendd) = (*tokendd)->next;
	}
	cmd->args = ft_split(args, ' ');
	cmd->infile = infile_file;
	cmd->outfile = outfile_file;
	cmd->del = del;
	cmd->next = NULL;
	return(cmd);
}


t_command	*parcer(char *line)
{
	char	*trim;
	t_token *token;
	t_token *head_token;
	t_lexer *lexer;
	t_command *commande;
	t_command *head;

	head_token = NULL;
	head = NULL;
	token = NULL;
	commande = NULL;
		trim = ft_strtrim(line, " ");
		if (syntaxe_error(trim))
		{
			lexer = creat_lexer(trim);
			while(1)
			{
				token = tokenize(lexer);
				printf("token(%d, %s)\n", token->type, token->value);
				ft_lstadd_back_token(&head_token, token);
				if (token->type  == ENDF)
					break;
			}
			while (head_token && head_token->type != ENDF)
			{
				commande = parser_commande(&head_token);
				ft_lstadd_back_cmd(&head, commande);
				printf("value %s\n", head_token->value);
				head_token = head_token->next;
			}
			printlist(head);
			head_token = NULL;
		}
		else 
		{
			write(1, "Quotes Error !\n", 15);
		}
		return(commande);
}



