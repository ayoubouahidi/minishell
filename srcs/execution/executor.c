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
    if (!ft_strcmp(cmd->args[0], "cd") || !ft_strcmp(cmd->args[0], "pwd"))
        ex_cd_pwd(cmd);
    else if (!ft_strcmp(cmd->args[0], "echo") || !ft_strcmp(cmd->args[0], "env"))
        ex_echo_env(cmd);
    // else if (!ft_strcmp(cmd->args[0], "export"))
    //     ex_export(cmd);
    // else if (!ft_strcmp(cmd->args[0], "unset"))
    //     ex_unset(cmd);
    // else if (!ft_strcmp(cmd->args[0], "exit"))
    //     ex_exit(cmd);
}