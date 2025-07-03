/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:02:05 by ayouahid          #+#    #+#             */
/*   Updated: 2025/05/12 20:04:48 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../libft/libft.h"
#include <stdbool.h>

#include "parser.h"


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


void printlist(t_command *head)
{
    t_command *tmp = head;
    int i;

    while (tmp)
    {
        printf("┌────────────────────────────────────────┐\n");
        printf("│              Command Block             │\n");
        printf("├────────────────────────────────────────┤\n");

        printf("│ here_doc_file : %-25s│\n", tmp->here_doc_file);
        printf("│ redirections  :  │\n");
		while (tmp->redirections)
		{
			printf("red ===== %s|\n", tmp->redirections->file);
			tmp->redirections = tmp->redirections->next;
		}
        printf("│ is_heredoc    : %-25s│\n", tmp->is_heredoc ? "true" : "false");
        printf("│ del           : %-25s│\n", tmp->del ? tmp->del : "(null)");

        printf("├─────────────── Arguments ──────────────┤\n");
        if (tmp->args)
        {
            for (i = 0; tmp->args[i]; i++)
                printf("│ arg[%d]        : %-24s│\n", i, tmp->args[i]);
        }
        else
        {
            printf("│ No arguments provided.                 │\n");
        }

        printf("└────────────────────────────────────────┘\n\n");

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

t_lexer *creat_lexer(char *content)  // ls -la"fgrt"
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
		printf("this is i : %d\n", lexer->i);
	}
}

t_token *string_process(t_lexer *lexer)
{
	char *value;
	int flag_single;
	int flag_double;
	size_t start;

	start = lexer->i;
	flag_single = 0;
	flag_double = 0;
	while (lexer->c != '\0' )
	{
		if (lexer->c == '\'')
		{
			if (!flag_single)
				flag_single++;
			else
				flag_single--;
		}
		if (lexer->c == '\"' && !flag_single)
		{
			if (!flag_double)
				flag_double++;
			else
				flag_double--;
		}
		if (!flag_double && !flag_single && (lexer->c == '|' || lexer->c == '<' || lexer->c == '>' || lexer->c == ' ') )
			break ;
		increment_using_index(lexer); //f"c |<>"d
	}
	value = ft_substr(lexer->content, start, lexer->i - start);
	printf("walid : %s\n", value);
	return (creat_token(WORD, value));
}

// t_token* is_word(t_lexer *lexer)
// {
// 	t_lexer tmp;
// 	int	count;
// 	char *value;
// 	bool quoted;

// 	tmp = *lexer;
// 	count = 0;
// 	quoted = false;
// 	while (ft_isalnum(tmp.c))
// 	{
// 		count++;
// 		increment_using_index(&tmp);
// 	}
// 	// to handle export name="ayoub" 
// 	// lenght counter
// 	if (tmp.c == '=')
// 	{
// 		count++;
// 		increment_using_index(&tmp);
// 		if(tmp.c == '"')
// 		{
// 			increment_using_index(&tmp);
// 			quoted = true;
// 			while (tmp.c != '"')
// 			{	
// 				// printf("in token && quoted inside string ==> %c\n", tmp.c);

// 				count++;
// 				increment_using_index(&tmp);
// 			}
// 		}
// 		else
// 		{
// 			while (ft_isalnum(tmp.c))
// 			{
// 				count++;
// 				increment_using_index(&tmp);
// 			}
// 		}
// 	}
// 	if (quoted)
// 		count = count + 2;
// 	// finish
// 	value = (char *)malloc(count + 1);	
// 	count = 0;
// 	while (ft_isalnum(lexer->c))
// 	{
// 		value[count] = lexer->c;
// 		count++;
// 		increment_using_index(lexer);
// 	}
// 	// to handle export name="ayoub" 
// 	if (lexer->c == '=')
// 	{
// 		value[count] = lexer->c;
// 		count++;
// 		increment_using_index(lexer);
// 		if (lexer->c == '"')
// 		{
// 			value[count] = lexer->c;
// 			count++;
// 			increment_using_index(lexer);
// 			while (lexer->c != '"')
// 			{
// 				value[count] = lexer->c;
// 				printf("value is <%c>\n", value[count]);
// 				count++;
// 				increment_using_index(lexer);
// 			}
// 			value[count] = lexer->c;
// 			count++;
// 			increment_using_index(lexer);
// 		}
// 		else
// 		{
// 			while (ft_isalnum(lexer->c))
// 			{
// 				value[count] = lexer->c;
// 				count++;
// 				increment_using_index(lexer);
// 			}
// 		}
// 	}


// 	// finish
// 	value[count] = '\0';
// 	return (creat_token(WORD, value));
// }

char *is_word(t_lexer *lexer)
{
    t_lexer tmp;
    int count;
    char *value;

    tmp = *lexer;
    count = 0;
    
    // First pass: count all characters in the word
    while (ft_isalnum(tmp.c) || tmp.c == '=' || tmp.c == '"')
    {
        if (tmp.c == '"')
        {
            // Count the opening quote
            count++;
            increment_using_index(&tmp);
            
            // Count everything until closing quote
            while (tmp.c != '"' && tmp.c != '\0')
            {
                count++;
                increment_using_index(&tmp);
            }
            
            // Count the closing quote if present
            if (tmp.c == '"')
            {
                count++;
                increment_using_index(&tmp);
            }
        }
        else
        {
            count++;
            increment_using_index(&tmp);
        }
    }
    
    // Allocate memory
    value = (char *)malloc(count + 1);
    if (!value)
        return NULL;
    
    // Second pass: copy characters
    count = 0;
    
    while (ft_isalnum(lexer->c) || lexer->c == '=' || lexer->c == '"' || lexer->c == '\'')
    {
        if (lexer->c == '"' || lexer->c == '\'')
        {
            // Copy the opening quote
            value[count++] = lexer->c;
            increment_using_index(lexer);
            
            // Copy everything until closing quote
            while ((lexer->c != '"' || lexer->c == '\'') && lexer->c != '\0')
            {
                value[count++] = lexer->c;
                increment_using_index(lexer);
            }
            
            // Copy the closing quote if present
            if (lexer->c == '"' || lexer->c == '\'')
            {
                value[count++] = lexer->c;
                increment_using_index(lexer);
            }
        }
        else
        {
            value[count++] = lexer->c;
            increment_using_index(lexer);
        }
    }
    
    value[count] = '\0';
    return (value);
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

// t_token* retreive_key(lexer)
// {
// 	char *result;
	

// 	return (creat_token(WORD, result));
// }


// tokenize

t_token	*tokenize(t_lexer *lexer) 
{
	bool isword;	
	bool isdouble;

	isdouble = false;
	isword = false;
	while (lexer->c != '\0' && lexer->i < ft_strlen(lexer->content))
	{
		if(lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r' || lexer->c == '\f' || lexer->c == '\v')
			while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r' || lexer->c == '\f' || lexer->c == '\v')
				increment_using_index(lexer);
		if(!ft_strchr("|<>", lexer->c))
			return string_process(lexer);//f"ls"cat
		if (lexer->c == '|')
		{
			increment_using_index(lexer);
			return (creat_token(PIPE, tostr('|')));
		}
		if (lexer->c == '<')
			return (chech_herdoc(lexer));
		if (lexer->c == '>')
			return (check_append(lexer));
		}
		// increment_using_index(lexer);
		return (creat_token(ENDF, "END"));
	}
	




// some utils function of parse commande 

int	len_of_2d_array(char **args)
{
	int i;

	i = 0;
	while (args && args[i])
		i++;
	return i;
	
}

char	**to_arg(t_token* token, char **arg)
{
	char **result;
	int i;

	i = 0;
	if(!arg)
	{
		result = malloc(sizeof(char *) + 2);
		result[0] = ft_strdup(token->value);
		result[1] = NULL;
		return result;
	}
	result = malloc(sizeof(char *) * (len_of_2d_array(arg) + 2));
	while (arg[i])
	{
		result[i] = arg[i];
		i++;
	}
	result[i] = ft_strdup(token->value);
	result[i + 1] = NULL;
	return (result);
}

// char *to_arg(t_token* token, char *arg)
// {
// 	int lenv;
// 	int lenghtcommande;
// 	char *new_commande;

// 	lenv = ft_strlen(token->value);
// 	lenghtcommande = ft_strlen(arg);
// 	new_commande = (char *)malloc(lenghtcommande + 1 + lenv + 1);
// 	if (arg)
// 	{
//         ft_strlcpy(new_commande, arg, lenghtcommande + 1); 
//         new_commande[lenghtcommande] = ' '; 
//         ft_strlcpy(new_commande + lenghtcommande + 1, token->value, lenv + 1);
//     } 
// 	else 
//         ft_strlcpy(new_commande, token->value, lenv + 1); 
//     return (new_commande);
// }

// infile function u should check sysntaxe error


bool match_file(char *file)
{
	if(ft_isdigit(file[0]))
		return (false);
	return (true);
}


void	infile(t_token *token, int *flag_err)
{
    // char *new_commande;
    
    if (!token || token->next->type == ENDF) {
        ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
        *flag_err = 1;
        // return NULL;
    }
    token = token->next;
    if (token->type != WORD) {
        if (token->type == OUTPUT_RED)
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token `>'\n", 2);
        else if (token->type == INTPUT_RED)
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token `<'\n", 2);
        else if (token->type == APPEND)
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token `>>'\n", 2);
        else if (token->type == HEREDOC)
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token `<<'\n", 2);
        else if (token->type == PIPE)
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token `|'\n", 2);
        else
			ft_putstr_fd("minishell- ayoub : syntax error near unexpected token\n", 2);
        *flag_err = 1;
        // return NULL;
    }
    // return new_commande;
}

bool heredoc_check_append(t_token *token, char **del)
{
	t_token *prev;

	prev = token; 
    token = token->next;
    if ((token)->type != WORD)
    {
        printf("Syntaxe error: append or heredoc problem\n");
        return false;
    }
	// v
	// if(prev->type == APPEND)
	*del = token->value;
    return true;
}

// parser part

//  redrection function


t_redirections *creat_red(char *file, TYPE_TOKEN type)
{
	t_redirections *red;

	red = (t_redirections *)malloc(sizeof(t_redirections));
	if (!red)
		return (NULL);
	red->file = file;
	red->type =  type;
	red->next = NULL;
	return(red);
}
	

void	ft_lstadd_back_red(t_redirections **lst, t_redirections *new)
{
	t_redirections	*tmp;

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


t_command* parser_commande(t_token** tokendd)
{
    t_command *cmd;
    char **args = NULL;
    char *del = NULL;

    bool in_red = false;
    int flag_err = 0;
	t_redirections *red_head;
	t_redirections *red;

    if (!tokendd || !*tokendd)
        return NULL;
    cmd = (t_command *)malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->is_heredoc = false;
    cmd->del = NULL;
    cmd->next = NULL;
	red_head = NULL;
	red = NULL;
    
	while (tokendd && (*tokendd)->type != ENDF && (*tokendd)->type != PIPE)
	{
		if((*tokendd)->type == WORD)
		{
			if(!in_red)
				args = to_arg((*tokendd), args);
			in_red = false;
		}
		else {
			in_red = true;
			infile((*tokendd), &flag_err);
			if ((*tokendd)->type == OUTPUT_RED)
			{
				printf("in red\n");
				red = creat_red((*tokendd)->next->value, OUTPUT_RED);
				ft_lstadd_back_red(&red_head, red);

			}
			else if ((*tokendd)->type == INTPUT_RED)
			{
				red = creat_red((*tokendd)->next->value, INTPUT_RED);
				ft_lstadd_back_red(&red_head, red);
			}
			else if ((*tokendd)->type == HEREDOC)
				cmd->is_heredoc = heredoc_check_append((*tokendd), &del);
			else if ((*tokendd)->type == APPEND)
			{
				red = creat_red((*tokendd)->next->value, APPEND);
				ft_lstadd_back_red(&red_head, red);
			}
			else if ((*tokendd)->type == PIPE)
			{
				infile((*tokendd), &flag_err);
				printf("test done");				
			}
            if (flag_err == 1) {
				// if (args) free_array(args);
				if (del) free(del);
				return NULL;
			}
		}
		(*tokendd) = (*tokendd)->next;
	}
	cmd->args = args;
	cmd->del = del;
	cmd->redirections = red_head;
	return cmd;
}


t_command	*parcer(char *line, t_env *envp)
{
	char	*trim;
	// char *expantion;
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
				// printf("test 0");
				expantion_remove_quotes(token, envp);
				// if(token->type == WORD)
				// {
				// 	expantion = expanation_token_env_var(token->value , envp);
				// 	free(token->value);
				// 	token->value = expantion;
				// }
				ft_lstadd_back_token(&head_token, token);
				if (token->type  == ENDF)
					break;
			}
			while (head_token && head_token->type != ENDF)
			{
				commande = parser_commande(&head_token);
				ft_lstadd_back_cmd(&head, commande);
				// printf("value %s\n", head_token->value);
				head_token = head_token->next;
			}
			// printlist(head);
			head_token = NULL;
		}
		else 
		{
			write(1, "Quotes Error !\n", 15);
		}
		return(head);
}

