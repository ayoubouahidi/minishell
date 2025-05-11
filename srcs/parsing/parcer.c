#include "../../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../libft/libft.h"

static int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

static int is_word_char(char c)
{
    return (ft_isalnum(c) || c == '/' || c == '.' || c == '-' || c == '_' ||
            c == ':' || c == '=' || c == '+' || c == '%' || c == ',' || c == '@' || c == '~');
}

static void free_command(t_command *cmd)
{
    if (!cmd)
        return;
    if (cmd->args)
    {
        for (int i = 0; cmd->args[i]; i++)
            free(cmd->args[i]);
        free(cmd->args);
    }
    free(cmd->infile);
    free(cmd->outfile);
    free(cmd->del);
    free(cmd);
}

t_command *ft_lstnew_cmd(void *content)
{
    t_command *tmp = (t_command *)malloc(sizeof(t_command));
    if (!tmp)
        return (NULL);
    tmp->args = content;
    tmp->infile = NULL;
    tmp->outfile = NULL;
    tmp->del = NULL;
    tmp->is_heredoc = false;
    tmp->is_append = false;
    tmp->next = NULL;
    return (tmp);
}

void ft_lstadd_back_token(t_token **lst, t_token *new)
{
    if (!lst || !new)
        return;
    if (*lst)
    {
        t_token *tmp = *lst;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
    else
        *lst = new;
}

void ft_lstadd_back_cmd(t_command **lst, t_command *new)
{
    if (!lst || !new)
        return;
    if (*lst)
    {
        t_command *tmp = *lst;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
    else
        *lst = new;
}

bool syntax_error(char *str)
{
    int quotes_d = 0;
    int quotes_s = 0;
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '"' && !quotes_s)
            quotes_d = !quotes_d;
        else if (str[i] == '\'' && !quotes_d)
            quotes_s = !quotes_s;
        i++;
    }
    return (quotes_d != 0 || quotes_s != 0);
}

t_token *create_token(TYPE_TOKEN type, char *value)
{
    t_token *token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = value;
    token->next = NULL;
    return (token);
}

t_lexer *create_lexer(char *content)
{
    t_lexer *lexer = (t_lexer *)malloc(sizeof(t_lexer));
    if (!lexer)
        return (NULL);
    lexer->content = content;
    lexer->i = 0;
    lexer->c = lexer->content[lexer->i];
    return (lexer);
}

void lexer_advance(t_lexer *lexer)
{
    if (lexer->c != '\0' && lexer->i < ft_strlen(lexer->content))
    {
        lexer->i += 1;
        lexer->c = lexer->content[lexer->i];
    }
}

t_token *string_process(t_lexer *lexer)
{
    lexer_advance(lexer);
    int start = lexer->i;
    while (lexer->c != '"' && lexer->c != '\0')
        lexer_advance(lexer);
    if (lexer->c != '"')
    {
        printf("Error: Unclosed double quote\n");
        return (NULL);
    }
    char *value = ft_substr(lexer->content, start, lexer->i - start);
    lexer_advance(lexer);
    return (create_token(WORD, value));
}

t_token *word_token(t_lexer *lexer)
{
    int start = lexer->i;
    while (is_word_char(lexer->c))
        lexer_advance(lexer);
    char *value = ft_substr(lexer->content, start, lexer->i - start);
    return (create_token(WORD, value));
}

char *char_to_str(char c)
{
    char *val = (char *)malloc(2);
    if (!val)
        return (NULL);
    val[0] = c;
    val[1] = '\0';
    return (val);
}

t_token *check_heredoc(t_lexer *lexer)
{
    lexer_advance(lexer);
    if (lexer->c == '<')
    {
        lexer_advance(lexer);
        return (create_token(HEREDOC, ft_strdup("<<")));
    }
    return (create_token(INTPUT_RED, char_to_str('<')));
}

t_token *check_append(t_lexer *lexer)
{
    lexer_advance(lexer);
    if (lexer->c == '>')
    {
        lexer_advance(lexer);
        return (create_token(APPEND, ft_strdup(">>")));
    }
    return (create_token(OUTPUT_RED, char_to_str('>')));
}

t_token *tokenize(t_lexer *lexer)
{
    while (lexer->c != '\0')
    {
        if (ft_isspace(lexer->c))
        {
            while (ft_isspace(lexer->c))
                lexer_advance(lexer);
            continue;
        }
        if (is_word_char(lexer->c))
            return (word_token(lexer));
        if (lexer->c == '|')
        {
            lexer_advance(lexer);
            return (create_token(PIPE, char_to_str('|')));
        }
        if (lexer->c == '<')
            return (check_heredoc(lexer));
        if (lexer->c == '>')
            return (check_append(lexer));
        if (lexer->c == '"')
            return (string_process(lexer));
        printf("Error: Unknown character '%c'\n", lexer->c);
        lexer_advance(lexer);
    }
    return (create_token(ENDF, ft_strdup("END")));
}

char *join_arg(t_token *token, char *arg)
{
    char *new_arg;
    if (arg)
    {
        new_arg = ft_strjoin(arg, " ");
        free(arg);
        char *tmp = ft_strjoin(new_arg, token->value);
        free(new_arg);
        return (tmp);
    }
    return (ft_strdup(token->value));
}

char *handle_redirection(t_token **token, char *current)
{
    *token = (*token)->next;
    if (!*token || (*token)->type != WORD)
    {
        printf("Syntax error: Missing filename after redirection\n");
        return (NULL);
    }
    char *new = join_arg(*token, current);
    if (current)
        free(current);
    return (new);
}

bool handle_heredoc_append(t_token **token, char **del)
{
    *token = (*token)->next;
    if (!*token || (*token)->type != WORD)
    {
        printf("Syntax error: Missing delimiter after heredoc/append\n");
        return (false);
    }
    *del = ft_strdup((*token)->value);
    return (true);
}

t_command *parse_command(t_token **token)
{
    t_command *cmd = ft_lstnew_cmd(NULL);
    if (!cmd)
        return (NULL);
    char *args = NULL;
    while (*token && (*token)->type != ENDF && (*token)->type != PIPE)
    {
        if ((*token)->type == WORD)
            args = join_arg(*token, args);
        else if ((*token)->type == OUTPUT_RED)
            cmd->outfile = handle_redirection(token, cmd->outfile);
        else if ((*token)->type == INTPUT_RED)
            cmd->infile = handle_redirection(token, cmd->infile);
        else if ((*token)->type == HEREDOC)
            cmd->is_heredoc = handle_heredoc_append(token, &cmd->del);
        else if ((*token)->type == APPEND)
            cmd->is_append = handle_heredoc_append(token, &cmd->del);
        if (!args || ((*token)->type == OUTPUT_RED && !cmd->outfile) || ((*token)->type == INTPUT_RED && !cmd->infile))
        {
            free_command(cmd);
            return (NULL);
        }
        *token = (*token)->next;
    }
    if (args)
    {
        cmd->args = ft_split(args, ' ');
        free(args);
    }
    return (cmd);
}

void free_tokens(t_token *tokens)
{
    while (tokens)
    {
        t_token *tmp = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = tmp;
    }
}

void free_commands(t_command *head)
{
    while (head)
    {
        t_command *tmp = head->next;
        free_command(head);
        head = tmp;
    }
}

t_command *parcer(char *line)
{
    char *trim = ft_strtrim(line, " \t\n\r\f\v");
    if (!trim)
        return (NULL);
    if (syntax_error(trim))
    {
        printf("Error: Unclosed quotes\n");
        free(trim);
        return (NULL);
    }
    t_lexer *lexer = create_lexer(trim);
    if (!lexer)
    {
        free(trim);
        return (NULL);
    }
    t_token *tokens = NULL;
    t_token *current_token = NULL;
    while (1)
    {
        current_token = tokenize(lexer);
        if (!current_token)
            break;
        ft_lstadd_back_token(&tokens, current_token);
        if (current_token->type == ENDF)
            break;
    }
    free(lexer);
    free(trim);
    t_command *commands = NULL;
    t_token *current = tokens;
    while (current && current->type != ENDF)
    {
        t_command *cmd = parse_command(&current);
        if (!cmd)
        {
            free_tokens(tokens);
            free_commands(commands);
            return (NULL);
        }
        ft_lstadd_back_cmd(&commands, cmd);
        if (current && current->type == PIPE)
            current = current->next;
    }
    free_tokens(tokens);
    return (commands);
}
