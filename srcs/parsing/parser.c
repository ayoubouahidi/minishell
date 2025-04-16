#include "../includes/parser.h"
#include "../includes/minishell.h"

static char **split_pipe(char *line) {
    char **parts = malloc(100 * sizeof(char *));
    int i = 0;
    char *token = strtok(line, "|");
    while (token) {
        parts[i++] = strdup(token);
        token = strtok(NULL, "|");
    }
    parts[i] = NULL;
    return parts;
}

static char **split_args(char *cmd) {
    char **args = malloc(100 * sizeof(char *));
    int i = 0;
    char *token = strtok(cmd, " \t\n");
    while (token) {
        args[i++] = strdup(token);
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
    return args;
}

static void handle_redirection(t_command *cmd) {
    char **new_args = malloc(100 * sizeof(char *));
    int j = 0;

    for (int i = 0; cmd->args[i]; i++) {
        if (strcmp(cmd->args[i], ">") == 0 && cmd->args[i + 1]) {
            cmd->outfile = strdup(cmd->args[++i]);
            cmd->append = 0;
        } else if (strcmp(cmd->args[i], ">>") == 0 && cmd->args[i + 1]) {
            cmd->outfile = strdup(cmd->args[++i]);
            cmd->append = 1;
        } else if (strcmp(cmd->args[i], "<") == 0 && cmd->args[i + 1]) {
            cmd->infile = strdup(cmd->args[++i]);
        } else {
            new_args[j++] = strdup(cmd->args[i]);
        }
    }
    new_args[j] = NULL;
    free(cmd->args);
    cmd->args = new_args;
}

t_command *parse_line(char *line) {
    t_command *head = NULL;
    t_command *prev = NULL;

    char **pipes = split_pipe(line);

    for (int i = 0; pipes[i]; i++) {
        t_command *cmd = malloc(sizeof(t_command));
        cmd->args = split_args(pipes[i]);
        cmd->infile = NULL;
        cmd->outfile = NULL;
        cmd->append = 0;
        cmd->next = NULL;

        handle_redirection(cmd);

        if (!head)
            head = cmd;
        else
            prev->next = cmd;
        prev = cmd;
    }

    // free pipes
    for (int i = 0; pipes[i]; i++) free(pipes[i]);
    free(pipes);

    return head;
}

void free_commands(t_command *cmd) {
    t_command *tmp;
    while (cmd) {
        tmp = cmd->next;
        for (int i = 0; cmd->args && cmd->args[i]; i++) free(cmd->args[i]);
        free(cmd->args);
        free(cmd->infile);
        free(cmd->outfile);
        free(cmd);
        cmd = tmp;
    }
}
