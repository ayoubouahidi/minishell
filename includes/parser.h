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
    char **args;
	char *infile;
	char *outfile;
    struct s_command *next;
} t_command;


typedef enum  
{
   UNCLOSED_DOUBLE_Q,
   UNCLODES_SINFGLE_Q 
} e_syntaxe;

// tokenation
typedef enum
{
	INTPUT_RED,
	OUTPUT_RED,
	HEREDOC,
	APPEND,
	PIPE,
	WORD,
	ENDF
} TYPE_TOKEN;


typedef struct s_token
{
	TYPE_TOKEN type;
	char *value;
	struct s_token *next;
}	t_token;

typedef struct s_lexer
{
	char c;
	unsigned int i;
	char *content;
}t_lexer;



// a revoire : lexer == '\t' || lexer == '\n' || lexer == '\r' || lexer == '\f' || lexer == '\v'


t_command *parse_line(char *line);
void free_commands(t_command *cmd);
t_command	*parcer();
char	*ft_strtrim(char const *s1, char const *set);
bool    syntaxe_error(char *str);
#endif
