#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>  
#include <readline/history.h>
#include <unistd.h>

#define WHITESPACE " \t\n\r\f\v"

typedef struct s_command
{
    char    **args;
    char    *infile;
    char    *outfile;
    bool    is_append;
    bool    is_heredoc;
    char    *del;
    char    *appendfile;
    struct s_command *next;
} t_command;

typedef enum {
    INTPUT_RED,
    OUTPUT_RED,
    HEREDOC,
    APPEND,
    PIPE,
    WORD,
    ENDF
} TYPE_TOKEN;

typedef struct s_token {
    TYPE_TOKEN type;
    char *value;
    struct s_token *next;
} t_token;

typedef struct s_lexer {
    char c;
    unsigned int i;
    char *content;
} t_lexer;

t_command *parcer(char *line);
void free_commands(t_command *cmd);

#endif