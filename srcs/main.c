#include "parser.h"

static void init_data(t_data *data, char **envp)
{
    init_env(data, envp);
    data->cmd = NULL;
    data->exit_status = 0;
    data->pid = 0;
    data->is_child = false;
}

int main(int ac, char **av, char **envp)
{
    t_data  data;
    char    *line;

    (void)ac;
    (void)av;
    
    ft_memset(&data, 0, sizeof(t_data)); 
    init_data(&data, envp);

    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            clean_exit(&data, data.exit_status);
        }
        if (*line)
        {
            add_history(line);
            data.cmd = parcer(line, data.env);
            if (data.cmd)
            {
                // printf("test done\n");
                executer(&data, envp);
            }
            free_cmd(data.cmd);
            data.cmd = NULL;
        }
        free(line);
    }
    return (0);
}