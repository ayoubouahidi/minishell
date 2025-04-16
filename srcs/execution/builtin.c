#include "../includes/minishell.h"
#include "../includes/parser.h"
#include "../includes/utils.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return 0;
    return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") ||
            !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "pwd") ||
            !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") ||
            !ft_strcmp(cmd, "env"));
}
void execute_builtin(t_command *cmd)
{
    char *cwd;

    if (!cmd->args || !cmd->args[0])
        return;

    if (!ft_strcmp(cmd->args[0], "cd"))
    {
        if (!cmd->args[1])
            printf("cd: missing argument\n");
        else if (chdir(cmd->args[1]) != 0)
            perror("cd");
    }
    if (!ft_strcmp(cmd->args[0], "pwd"))
    {
        cwd = getcwd(NULL, 0);
        if (!cwd)
        {
            perror("pwd");
            return;
        }
        printf("%s\n", cwd);
        free(cwd);
    }
}
