#ifndef PARSER_H
#define PARSER_H

typedef struct s_command
{
    char **args;
    char *infile;
    char *outfile;
    int append;
    struct s_command *next;
} t_command;

t_command *parse_line(char *line);
void free_commands(t_command *cmd);

#endif
