#include "../includes/parser.h"
#include "../includes/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    while (1) {
        char *line = readline("minishell$ ");
        if (!line)
            break;
        if (*line)
            add_history(line);

        t_command *cmd = parse_line(line);
        t_command *tmp = cmd;

        while (tmp) {
            if (tmp->args && tmp->args[0]) {
                if (is_builtin(tmp->args[0]))
                    execute_builtin(tmp);
                // else
                //     execute_external(tmp);
            }
            tmp = tmp->next;
        }

        free_commands(cmd);
        free(line);
    }
    return 0;
}
