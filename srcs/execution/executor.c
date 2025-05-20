#include "../../includes/minishell.h"

static int external_command(t_data *data);

static int handle_builtin(t_data *data)
{
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    int status;

    setup_redirections(data->cmd);
    status = execute_builtin(data);
    
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
    close(stdin_copy);
    close(stdout_copy);
    
    return status;
}

static int execute_single_command(t_data *data)
{
    if (is_builtin(data->cmd->args[0]))
        return handle_builtin(data);
    return external_command(data);
}


static int launch_external_command(t_data *data)
{
    char *path;
    char **envp;
    pid_t pid_ch;

    if (access(data->cmd->args[0], X_OK) == 0)
        path = ft_strdup(data->cmd->args[0]);
    else
        path = get_path(data, data->cmd->args[0]);
    
    if (!path)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(data->cmd->args[0], STDERR_FILENO);
        ft_putendl_fd(": command not found", STDERR_FILENO);
        return 127;
    }

    pid_ch = fork();
    if (pid_ch == -1)
    {
        perror("minishell: fork");
        free(path);
        return 1;
    }

    if (pid_ch == 0)
    {
        setup_redirections(data->cmd);
        envp = env_to_array(data->env);
        execve(path, data->cmd->args, envp);
        perror("minishell: execve");
        cleanup_child_resources(path, envp);
        exit(126);
    }

    waitpid(pid_ch, &data->exit_status, 0);
    free(path);
    return WEXITSTATUS(data->exit_status);
}

static int external_command(t_data *data)
{
    if (!data->cmd->args[0])
    {
        setup_redirections(data->cmd);
        return 0;
    }
    return launch_external_command(data);
}

void executer(t_data *data, char **envp)
{
    (void)envp;

    if (!data->cmd || !data->cmd->args)
        return;

    if (!data->cmd->args[0])
    {
        setup_redirections(data->cmd);
        return;
    }

    if (is_builtin(data->cmd->args[0]) && !data->cmd->next)
    {
        data->exit_status = handle_builtin(data);
        return;
    }

    if (!data->cmd->next)
    {
        data->exit_status = execute_single_command(data);
        return;
    }

    execute_pipe(data);
}