#include "../includes/minishell.h"
#include "../includes/parser.h"
#include "../includes/utils.h"

void ex_cd_pwd(t_command *cmd)
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

void ex_echo_env(t_command *cmd)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    if (cmd->args[0] && !ft_strcmp(cmd->args[0], "echo"))
    {
        if (cmd->args[1] && !ft_strcmp(cmd->args[1], "-n"))
            (newline = 0, i++);
        while (cmd->args[i])
        {
            printf("%s", cmd->args[i]);
            if (cmd->args[i + 1])
                printf(" ");
            i++;
        }
        if (newline)
            printf("\n");
    }
    if (cmd->args[0] && !ft_strcmp(cmd->args[0], "env"))
    {
        extern char **environ;
        for (int j = 0; environ[j]; j++)
            printf("%s\n", environ[j]);
    }
}
